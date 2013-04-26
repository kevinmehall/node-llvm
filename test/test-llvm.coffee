llvm = require('../index')
assert = require('assert')

if typeof gc is 'function'
	# running with --expose-gc, do a sweep between tests so valgrind blames the right one
	afterEach -> gc()

describe "llvm", ->
	it "has globalContext", ->
		assert llvm.globalContext instanceof llvm.Context

describe "Context", ->
	it "has basic types", ->
		assert llvm.globalContext.doubleTy instanceof llvm.FPType
		assert llvm.globalContext.int1Ty instanceof llvm.IntegerType
		assert llvm.globalContext.int8Ty instanceof llvm.IntegerType
		assert llvm.globalContext.int32Ty instanceof llvm.IntegerType

describe "Type", ->
	it "can get a pointer type", ->
		assert.ok llvm.globalContext.int8Ty.getPointerTo() instanceof llvm.Type

describe "FunctionType", ->
	it "can be created by a context", ->
		double = llvm.globalContext.doubleTy
		ft = llvm.globalContext.getFunctionType(double, [double])
		assert ft instanceof llvm.FunctionType

makeModule = ->
	new llvm.Module("myModName", llvm.globalContext)

makeFn = (module, name="foo") ->
	int = llvm.globalContext.int32Ty
	ft = llvm.globalContext.getFunctionType(int, [int])
	module.getOrInsertFunction(name, ft)
	
describe "Module", ->
	it "has moduleIdentifier", ->
		m = makeModule()
		assert.equal m.moduleIdentifier, "myModName"

	it "can create functions", ->
		m = makeModule()
		assert makeFn(m) instanceof llvm.Function
		
	describe "getFunction", ->
		it "gets an existent function", ->
			m = makeModule()
			makeFn(m)
			assert m.getFunction("foo") instanceof llvm.Function

		it "returns undefined when the function does not exist", ->
			m = makeModule()
			assert.equal m.getFunction("foo"), undefined

describe "Function", ->
	m = makeModule()
	fn = makeFn(m)

	it "has a name (via Value)", ->
		assert.equal fn.name, "foo"

	it "has an arguments list", ->
		args = fn.arguments
		assert.equal args.length, 1
		assert args[0] instanceof llvm.Value

	it "has a functionType", ->
		assert fn.functionType instanceof llvm.FunctionType
		# getFunction makes a new FunctionType wrapper
		assert m.getFunction('foo').functionType instanceof llvm.FunctionType

describe "BasicBlock", ->
	it "has a name (via Value)", ->
		bb = new llvm.BasicBlock(llvm.globalContext, "bbName")
		assert.equal bb.name, "bbName"

describe "IRBuilder", ->
	ctx = llvm.globalContext
	module = null
	b = null
	v = null # a value
	p = null # a pointer
	x = null # a value that can't be constant-folded

	it "can be created", ->
		module = makeModule()
		fn = makeFn(module)
		bb = fn.addBasicBlock("entry")
		b = new llvm.IRBuilder(ctx)
		b.setInsertPoint(bb)
		v = ctx.int32Ty.const(123)

	it 'creates cast ops', ->
		p = b.createIntToPtr(v, ctx.int32Ty.getPointerTo())

	it 'creates load ops', ->
		x = b.createLoad(p, "x")
		assert.equal x.dump(), "  %x = load i32* inttoptr (i32 123 to i32*)"

	it 'creates store ops', ->
		assert.equal b.createStore(p, v).dump(), "  store i32* inttoptr (i32 123 to i32*), i32 123"

	it "creates binary ops", ->
		assert.equal b.createNSWAdd(v, x).dump(), "  %1 = add nsw i32 123, %x"
		assert.equal b.createNSWAdd(v, x, "add1").dump(), "  %add1 = add nsw i32 123, %x"

	it "creates binary ops that take NUW and NSW", ->
		assert.equal b.createAdd(v, x, "add2").dump(), "  %add2 = add i32 123, %x"
		assert.equal b.createAdd(v, x, "add3", true, true).dump(), "  %add3 = add nuw nsw i32 123, %x"

	it "creates unary ops", ->
		#assert.equal b.createNeg(x, "neg1").dump(), "  %neg1 = neg i32 %x"
		assert.equal b.createNSWNeg(x, "neg1").dump(), "  %neg1 = sub nsw i32 0, %x"

	it "creates allocas", ->
		assert.equal b.createAlloca(ctx.doubleTy, null, "al1").dump(), "  %al1 = alloca double"
		assert.equal b.createAlloca(ctx.doubleTy, v, "al2").dump(), "  %al2 = alloca double, i32 123"

	it "creates calls"

	it "creates GEP", ->
		assert.equal b.createGEP(p, [x], "gep").dump(), "  %gep = getelementptr i32* inttoptr (i32 123 to i32*), i32 %x"

	it "builds SwitchInsts", ->
		m = makeModule()

		int = llvm.globalContext.int32Ty
		ft = llvm.globalContext.getFunctionType(int, [int])
		fn = m.getOrInsertFunction('testSwitch', ft)
		b = new llvm.IRBuilder(ctx)
		
		entry = b.setInsertPoint fn.addBasicBlock('entry')
		sw = b.createSwitch(fn.arguments[0], entry)

		one = b.setInsertPoint fn.addBasicBlock('one')
		b.createRet(int.const(111))

		two = b.setInsertPoint fn.addBasicBlock('two')
		b.createRet(int.const(222))

		others = b.setInsertPoint fn.addBasicBlock('others')
		b.createRet(int.const(999))

		sw.setDefaultDest(others)
		sw.addCase(int.const(1), one)
		sw.addCase(int.const(2), two)

		assert.equal m.dump(), """
			; ModuleID = 'myModName'

			define i32 @testSwitch(i32) {
			entry:
			  switch i32 %0, label %others [
			    i32 1, label %one
			    i32 2, label %two
			  ]

			one:                                              ; preds = %entry
			  ret i32 111

			two:                                              ; preds = %entry
			  ret i32 222

			others:                                           ; preds = %entry
			  ret i32 999
			}
			
		"""

	it "can build a function", ->
		m = makeModule()
		fn = makeFn(m)
		bb = fn.addBasicBlock("entry")

		b = new llvm.IRBuilder(ctx)
		b.setInsertPoint(bb)

		a1 = fn.arguments[0]
		a1.name = 'n'
		assert.equal a1.name, 'n'

		v = b.createAdd(a1, ctx.int32Ty.const(5), "b")
		assert.equal v.name, "b"

		b.createRet(v)

		assert.equal m.dump(), """
			; ModuleID = 'myModName'

			define i32 @foo(i32 %n) {
			entry:
			  %b = add i32 %n, 5
			  ret i32 %b
			}

		"""

		engine = new llvm.ExecutionEngine(m)
		pm = new llvm.FunctionPassManager(m)

		pm.addTargetDataPass('')
		  .addBasicAliasAnalysisPass()
		  .doInitialization()
		  
		pm.run(fn)
		
		fnWrapped = engine.getFFIFunction(fn)
		assert.equal fnWrapped(10), 15

llvm = require('../index')
assert = require('assert')

if typeof gc is 'function'
	# running with --expose-gc, do a sweep between tests so valgrind blames the right one
	afterEach -> gc()

describe "llvm", ->
	it "has globalContext", ->
		assert llvm.globalContext instanceof llvm.Context

describe "Context", ->
	it "can produce double values", ->
		assert llvm.globalContext.getDouble(99) instanceof llvm.Value

describe "FunctionType", ->
	it "can be created by a context", ->
		double = llvm.globalContext.getDoubleTy()
		ft = llvm.globalContext.getFunctionType(double, [double])
		assert ft instanceof llvm.FunctionType

makeModule = ->
	new llvm.Module("myModName", llvm.globalContext)

makeFn = (module, name="foo") ->
	double = llvm.globalContext.getDoubleTy()
	ft = llvm.globalContext.getFunctionType(double, [double])
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
	it "can build a function", ->
		ctx = llvm.globalContext
		m = makeModule()
		fn = makeFn(m)
		bb = fn.addBasicBlock("entry")

		b = new llvm.IRBuilder(ctx)
		b.setInsertPoint(bb)

		a1 = fn.arguments[0]
		a1.name = 'n'
		assert.equal a1.name, 'n'

		v = b.createFAdd(a1, ctx.getDouble(5), "b")
		assert.equal v.name, "b"

		b.createRet(v)

		assert.equal m.dump(), """
			; ModuleID = 'myModName'

			define double @foo(double %n) {
			entry:
			  %b = fadd double %n, 5.000000e+00
			  ret double %b
			}

		"""

		engine = new llvm.ExecutionEngine(m)
		pm = new llvm.FunctionPassManager(m)

		pm.addTargetDataPass('')
		pm.addBasicAliasAnalysisPass()
		pm.doInitialization()
		pm.run(fn)
		
		fnWrapped = engine.getFFIFunction(fn)
		assert.equal fnWrapped(10), 15

llvm = module.exports = require('bindings')('llvm')

llvm.initializeNativeTarget()

ref = require('ref')
ffi = require('ffi')

llvm.Function.prototype.addBasicBlock = function (nameOrBlock){
	if (typeof nameOrBlock == 'string'){
		nameOrBlock = new llvm.BasicBlock(this.context, nameOrBlock);
	}
	return this._addBasicBlock(nameOrBlock);
}

function initContext(ctx){
	// FFI/ref types have signedness, while LLVM types do not
	// We'll just assume unsigned...
	ctx.voidTy.ffiType   = ref.types.void
	ctx.floatTy.ffiType  = ref.types.float
	ctx.doubleTy.ffiType = ref.types.double
	ctx.int1Ty.ffiType   = ref.types.bool
	ctx.int8Ty.ffiType   = ref.types.uint8
	ctx.int16Ty.ffiType  = ref.types.uint16
	ctx.int32Ty.ffiType  = ref.types.uint32
	ctx.int64Ty.ffiType   = ref.types.uint64
}
initContext(llvm.globalContext)

llvm.Type.prototype.getPointerTo = function(){
	var t = this._getPointerTo()
	t.ffiType = ref.refType(this.ffiType || ref.types.void);
	return t;
}

llvm.ExecutionEngine.prototype.getFFIFunction = function(func){
	if (!ffi) throw new Error("Couldn't load FFI module")
	var ptr = this.getPointerToFunction(func)

	var argTypes = func.functionType.paramTypes
	var args = []
	for (var i=0; i<argTypes.length; i++){
		args.push(argTypes[i].ffiType || ref.types.void)
	}

	var retType = func.functionType.returnType.ffiType || ref.types.void
	var fn = ffi.ForeignFunction(ptr, retType, args)
	fn.executionEngine = this; // keep it from being GC'd
	return fn
}
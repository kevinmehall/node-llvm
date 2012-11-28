#include "node-llvm.h"

static Handle<Value> functionTypeConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR_POINTER(pFunctionType, 2);
	setConst(args.This(), "context", args[1]);
	return scope.Close(args.This());
}

static void init(Handle<Object> target){
	pFunctionType.init(&functionTypeConstructor);
	pFunctionType.inherit(pType);
	pFunctionType.addToModule(target);
}

Proto<llvm::FunctionType> pFunctionType("FunctionType", &init);

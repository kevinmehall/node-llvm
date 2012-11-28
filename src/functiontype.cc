#include "node-llvm.h"

static void init(Handle<Object> target){
	pFunctionType.init();
	pFunctionType.inherit(pType);
	pFunctionType.addToModule(target);
}

Proto<llvm::FunctionType> pFunctionType("FunctionType", &init, "context");

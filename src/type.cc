#include "node-llvm.h"

static Handle<Value> typeConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR_POINTER(pType, 2);
	setConst(args.This(), "type", args[1]);
	return scope.Close(args.This());
}

static void init(Handle<Object> target){
	pType.init(&typeConstructor);
	pType.addToModule(target);
}

Proto<llvm::Type> pType("Type", &init);

#include "node-llvm.h"

static Handle<Value> BBConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR_POINTER(pBasicBlock, 2);
	setConst(args.This(), "function", args[1]);
	return scope.Close(args.This());
}

static void init(Handle<Object> target){
	pBasicBlock.init(&BBConstructor);
}

Proto<llvm::BasicBlock> pBasicBlock("BasicBlock", &init);

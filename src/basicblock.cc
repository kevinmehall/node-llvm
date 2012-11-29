#include "node-llvm.h"

static Handle<Value> BBConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR(1);
	UNWRAP_ARG(pContext, ctx, 0);

	llvm::BasicBlock* bb;
	// It can accept a pointer as 2nd argument from Function constructor
	if (args.Length() > 1 && args[1]->IsExternal()){
		bb = static_cast<llvm::BasicBlock*>(Local<External>::Cast(args[1])->Value());
	}else{
		STRING_ARG(name, 1);
		bb = llvm::BasicBlock::Create(*ctx, name);
	}
	
	pBasicBlock.wrap(args.This(), bb);
	setConst(args.This(), "context", args[0]);
	return scope.Close(args.This());
}

static void init(Handle<Object> target){
	pBasicBlock.init(&BBConstructor);
	pBasicBlock.inherit(pValue);
}

Proto<llvm::BasicBlock> pBasicBlock("BasicBlock", &init);

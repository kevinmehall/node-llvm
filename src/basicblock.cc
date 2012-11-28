#include "node-llvm.h"

static void init(Handle<Object> target){
	pBasicBlock.init();

	pBasicBlock.addToModule(target);
}

Proto<llvm::BasicBlock> pBasicBlock("BasicBlock", &init, "func");

#include "node-llvm.h"

class jsBasicBlock{
public:
	static void init(Handle<Object> target){
		pBasicBlock.init(target);
	}

};
Proto<llvm::BasicBlock> pBasicBlock("BasicBlock", &jsBasicBlock::init, "func");

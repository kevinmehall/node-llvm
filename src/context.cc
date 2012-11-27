#include "node-llvm.h"

class jsContext{
public:
	static void init(Handle<Object> target){
		pContext.init(target);
	}
};
Proto<llvm::LLVMContext> pContext("Context", &jsContext::init);

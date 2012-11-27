#include "node-llvm.h"

class jsModule{
public:
	static void init(Handle<Object> target){
		pModule.init(target);
	}

};
Proto<llvm::Module> pModule("Module", &jsModule::init, "context");

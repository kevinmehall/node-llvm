#include "node-llvm.h"

static void init(Handle<Object> target){
	pType.init();
	pType.addToModule(target);
}

Proto<llvm::Type> pType("Type", &init, "context");

#include "node-llvm.h"

extern ProtoBuilder::InitFnList ProtoBuilder::initFns;

extern "C" void Initialize(Handle<Object> target) {
	HandleScope  scope;

	ProtoBuilder::initAll(target);

	auto globalContext = pContext.create(&llvm::getGlobalContext(), Undefined());
	target->Set(String::NewSymbol("globalContext"), globalContext, CONST_PROP);
}

NODE_MODULE(llvm, Initialize)

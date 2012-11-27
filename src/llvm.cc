#include "node-llvm.h"

extern ProtoBuilder::InitFnList ProtoBuilder::initFns;

extern "C" void Initialize(Handle<Object> target) {
	HandleScope  scope;

	ProtoBuilder::initAll(target);
	
	target->Set(String::New("llvm"), String::New("foo"));
}

NODE_MODULE(llvm, Initialize)

#include "node-llvm.h"

extern ProtoBuilder::InitFnList ProtoBuilder::initFns;

Handle<Value> getGlobalContext(const Arguments& args){
	HandleScope scope;
	return scope.Close(pContext.create(&llvm::getGlobalContext(), Undefined()));
}

extern "C" void Initialize(Handle<Object> target) {
	HandleScope  scope;

	ProtoBuilder::initAll(target);

	Local<FunctionTemplate> f = FunctionTemplate::New(&getGlobalContext);
	target->Set(String::NewSymbol("getGlobalContext"), f->GetFunction());
}

NODE_MODULE(llvm, Initialize)

#include "node-llvm.h"
#include "llvm/Support/TargetSelect.h"

extern ProtoBuilder::ProtoList ProtoBuilder::initProto;

static Handle<Value> initializeNativeTarget(const Arguments& args){
	llvm::InitializeNativeTarget();
	return Undefined();
}

extern "C" void Initialize(Handle<Object> target) {
	HandleScope  scope;

	ProtoBuilder::initAll(target);

	Local<FunctionTemplate> f = FunctionTemplate::New(&initializeNativeTarget);
	target->Set(String::NewSymbol("initializeNativeTarget"), f->GetFunction());

	auto globalContext = pContext.create(&llvm::getGlobalContext(), Undefined());
	target->Set(String::NewSymbol("globalContext"), globalContext, CONST_PROP);
}

NODE_MODULE(llvm, Initialize)

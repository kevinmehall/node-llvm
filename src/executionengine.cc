#include "node-llvm.h"
#include "llvm/ExecutionEngine/JIT.h"
#include <node_buffer.h>

static Handle<Value> EEConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR(1);
	UNWRAP_ARG(pModule, mod, 0);
	setConst(args.This(), "module", args[0]);

	std::string error;
	auto ee = llvm::EngineBuilder(mod).setErrorStr(&error).create();

	if (!ee){
		THROW_ERROR((std::string("Could not construct ExecutionEngine: ")+error).c_str())
	}

	pExecutionEngine.wrap(args.This(), ee);

	return scope.Close(args.This());
}

// For FFI, based on ffi.c

// Callback required to prevent Node from trying to memcpy it
void wrap_pointer_cb(char *data, void *hint) {}

Handle<Value> WrapPointer(char *ptr) {
  HandleScope scope;
  void *user_data = NULL;
  Buffer *buf = Buffer::New(ptr, 0, wrap_pointer_cb, user_data);
  return scope.Close(buf->handle_);
}

static Handle<Value> getPointerToFunction(const Arguments& args){
	ENTER_METHOD(pExecutionEngine, 1);
	UNWRAP_ARG(pFunction, fn, 0);

	char* fp = (char*) self->getPointerToFunction(fn);
	if (!fp){
		THROW_ERROR("getPointerToFunction failed");
	}

	return scope.Close(WrapPointer(fp));
}

static void init(Handle<Object> target){
	pExecutionEngine.init(&EEConstructor);

	pExecutionEngine.addMethod("getPointerToFunction", &getPointerToFunction);

	pExecutionEngine.addToModule(target);
}

Proto<llvm::ExecutionEngine> pExecutionEngine("ExecutionEngine", &init);

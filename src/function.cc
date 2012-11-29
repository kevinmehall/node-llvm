#include "node-llvm.h"

static Handle<Value> functionConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR_POINTER(pFunction, 3);
	setConst(args.This(), "module", args[1]);

	auto v8ft = args[2];
	if (v8ft->IsUndefined()){
		// Avoid creating a new context wrapper
		auto ctx = Local<Object>::Cast(args[1])->Get(String::NewSymbol("context"));
		v8ft = Local<Value>::New(pFunctionType.create(self->getFunctionType(), ctx));
	}
	setConst(args.This(), "functionType", v8ft);

	auto v8Args = Local<Array>::New(Array::New());
	unsigned i = 0;
	for (auto AI = self->arg_begin(); AI != self->arg_end(); ++AI, ++i) {
		v8Args->Set(i, pValue.create(AI, args.This()));
	}
	setConst(args.This(), "arguments", v8Args);

	return scope.Close(args.This());
}

static Handle<Value> addBasicBlock(const Arguments& args){
	ENTER_METHOD(pFunction, 1);
	STRING_ARG(name, 0);

	auto bb = llvm::BasicBlock::Create(self->getContext(), name, self);
	return scope.Close(pBasicBlock.create(bb, args[1]));
}

static void init(Handle<Object> target){
	pFunction.init(&functionConstructor);
	pFunction.inherit(pValue);

	pFunction.addMethod("addBasicBlock", &addBasicBlock);

}

Proto<llvm::Function> pFunction("Function", &init);

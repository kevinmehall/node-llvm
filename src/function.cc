#include "node-llvm.h"

static Handle<Value> addBasicBlock(const Arguments& args){
	ENTER_METHOD(pFunction, 1);
	STRING_ARG(name, 0);

	auto bb = llvm::BasicBlock::Create(self->getContext(), name, self);
	return scope.Close(pBasicBlock.create(bb, args[1]));
}

Handle<Value> fnArguments(Local<String> property, const AccessorInfo &info){
	ENTER_ACCESSOR(pFunction);

	auto a = Local<Array>::New(Array::New());

	unsigned i = 0;
	for (auto AI = self->arg_begin(); AI != self->arg_end(); ++AI, ++i) {
		a->Set(i, pValue.create(AI, info.Holder()));
	}

	return scope.Close(a);
}

static void init(Handle<Object> target){
	pFunction.init();
	pFunction.inherit(pValue);

	pFunction.addMethod("addBasicBlock", &addBasicBlock);
	pFunction.addAccessor("arguments", &fnArguments);

	pFunction.addToModule(target);
}

Proto<llvm::Function> pFunction("Function", &init, "module");

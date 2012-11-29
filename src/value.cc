#include "node-llvm.h"

static Handle<Value> valueConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR_POINTER(pValue, 2);
	setConst(args.This(), "parent", args[1]);
	return scope.Close(args.This());
}

Handle<Value> getValueName(Local<String> property, const AccessorInfo &info){
	ENTER_ACCESSOR(pValue);
	return scope.Close(String::New(self->getName().str().c_str()));
}

void setValueName(Local<String> property, Local<Value> value, const AccessorInfo &info){
	ENTER_ACCESSOR(pValue);
	if (value->IsString())
		self->setName(*String::Utf8Value(value->ToString()));
}

static void init(Handle<Object> target){
	pValue.init(&valueConstructor);

	pValue.addAccessor("name", &getValueName, &setValueName);
}

Proto<llvm::Value> pValue("Value", &init);

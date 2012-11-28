#include "node-llvm.h"

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
	pValue.init();

	pValue.addAccessor("name", &getValueName, &setValueName);

	pValue.addToModule(target);
}

Proto<llvm::Value> pValue("Value", &init, "parent");

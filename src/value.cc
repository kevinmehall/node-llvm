#include "node-llvm.h"

class jsValue{
public:
	static void init(Handle<Object> target){
		pValue.init(target);
	}
};
Proto<llvm::Value> pValue("Value", &jsValue::init, "parent");

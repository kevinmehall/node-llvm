#include "node-llvm.h"

static Handle<Value> typeConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR_POINTER(pType, 2);
	setConst(args.This(), "context", args[1]);
	return scope.Close(args.This());
}

static Handle<Value> getPointerTo(const Arguments& args){
	ENTER_METHOD(pType, 0);
	// TODO: AddrSpace parameter
	auto ctx = args.This()->Get(String::NewSymbol("context"));
	return scope.Close(pType.create(self->getPointerTo(), ctx));
}

static Handle<Value> intConst(const Arguments& args){
	ENTER_METHOD(pIntegerType, 1);
	llvm::Value* v;
	if (args[0]->IsString()){
		STRING_ARG(val, 0);
		int radix = 10;
		if (args.Length() > 1){
			INT_ARG(_radix, 1); radix = _radix;
		}
		v = llvm::ConstantInt::get(self, val, radix);
	}else{
		INT_ARG(val, 0);
		v = llvm::ConstantInt::getSigned(self, val);
	}

	auto ctx = args.This()->Get(String::NewSymbol("context"));
	return scope.Close(pValue.create(v, ctx));
}

static Handle<Value> fpConst(const Arguments& args){
	ENTER_METHOD(pFPType, 1);
	llvm::Value* v;
	if (args[0]->IsString()){
		STRING_ARG(val, 0);
		v = llvm::ConstantFP::get(self, val);
	}else{
		INT_ARG(val, 0);
		v = llvm::ConstantFP::get(self, val);
	}
	auto ctx = args.This()->Get(String::NewSymbol("context"));
	return scope.Close(pValue.create(v, ctx));
}



static void init(Handle<Object> target){
	pType.init(&typeConstructor);
	pType.addMethod("getPointerTo", &getPointerTo);

	pIntegerType.init(&typeConstructor);
	pIntegerType.inherit(pType);
	pIntegerType.addMethod("const", &intConst);

	pFPType.init(&typeConstructor);
	pFPType.inherit(pType);
	pFPType.addMethod("const", &fpConst);
}

Proto<llvm::Type> pType("Type", &init);
Proto<llvm::IntegerType> pIntegerType("IntegerType");
// Does not exist in LLVM, but we make use it for .const
Proto<llvm::Type> pFPType("FPType");

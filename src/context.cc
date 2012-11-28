#include "node-llvm.h"

static Handle<Value> contextConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR_POINTER(pContext, 2);
	return scope.Close(args.This());
}

static Handle<Value> getFunctionType(const Arguments& args){
	ENTER_METHOD(pContext, 2);
	(void) self;
	UNWRAP_ARG(pType, retType, 0);
	ARRAY_UNWRAP_ARG(pType, llvm::Type, argTypes, 1);

	auto tp = llvm::FunctionType::get(retType, argTypes, false);
	return scope.Close(pFunctionType.create(tp, args.This()));
}

static Handle<Value> getDoubleTy(const Arguments& args){
	ENTER_METHOD(pContext, 0);
	return scope.Close(pType.create(llvm::Type::getDoubleTy(*self), args.This()));
}

static Handle<Value> getDouble(const Arguments& args){
	ENTER_METHOD(pContext, 1);
	DOUBLE_ARG(val, 0);
	auto dbltp = llvm::Type::getDoubleTy(*self);
	auto v = llvm::ConstantFP::get(dbltp, val);
	return scope.Close(pValue.create(v, args.This()));
}

static void init(Handle<Object> target){
	pContext.init(&contextConstructor);
	pContext.addMethod("getFunctionType", &getFunctionType);
	pContext.addMethod("getDoubleTy", &getDoubleTy);
	pContext.addMethod("getDouble", &getDouble);
	pContext.addToModule(target);
}


Proto<llvm::LLVMContext> pContext("Context", init);

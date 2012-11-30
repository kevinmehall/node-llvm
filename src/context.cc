#include "node-llvm.h"

static Handle<Value> contextConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR_POINTER(pContext, 2);

	setConst(args.This(), "voidTy", pType.create(llvm::Type::getVoidTy(*self)));
	setConst(args.This(), "labelTy", pType.create(llvm::Type::getLabelTy(*self)));
	//setConst(args.This(), "halfTy", pType.create(llvm::Type::getHalfTy(*self)));
	setConst(args.This(), "floatTy", pFPType.create(llvm::Type::getFloatTy(*self)));
	setConst(args.This(), "doubleTy", pFPType.create(llvm::Type::getDoubleTy(*self)));
	setConst(args.This(), "metadataTy", pType.create(llvm::Type::getMetadataTy(*self)));
	setConst(args.This(), "x86_FP80Ty", pType.create(llvm::Type::getX86_FP80Ty(*self)));
	setConst(args.This(), "fP128Ty", pType.create(llvm::Type::getFP128Ty(*self)));
	setConst(args.This(), "pPC_FP128Ty", pType.create(llvm::Type::getPPC_FP128Ty(*self)));
	setConst(args.This(), "x86_MMXTy", pType.create(llvm::Type::getX86_MMXTy(*self)));
	setConst(args.This(), "int1Ty", pIntegerType.create(llvm::Type::getInt1Ty(*self)));
	setConst(args.This(), "int8Ty", pIntegerType.create(llvm::Type::getInt8Ty(*self)));
	setConst(args.This(), "int16Ty", pIntegerType.create(llvm::Type::getInt16Ty(*self)));
	setConst(args.This(), "int32Ty", pIntegerType.create(llvm::Type::getInt32Ty(*self)));
	setConst(args.This(), "int64Ty", pIntegerType.create(llvm::Type::getInt64Ty(*self)));

	return scope.Close(args.This());
}

static Handle<Value> getIntNTy(const Arguments& args){
	ENTER_METHOD(pContext, 1);
	INT_ARG(width, 0);
	auto tp = llvm::Type::getIntNTy(*self, width);
	return scope.Close(pIntegerType.create(tp, args.This()));
}

static Handle<Value> getFunctionType(const Arguments& args){
	ENTER_METHOD(pContext, 2);
	(void) self;
	UNWRAP_ARG(pType, retType, 0);
	ARRAY_UNWRAP_ARG(pType, llvm::Type, argTypes, 1);

	auto tp = llvm::FunctionType::get(retType, argTypes, false);
	return scope.Close(pFunctionType.create(tp, args.This()));
}

static void init(Handle<Object> target){
	pContext.init(&contextConstructor);
	pContext.addMethod("getFunctionType", &getFunctionType);
	pContext.addMethod("getIntNTy", &getIntNTy);
}


Proto<llvm::LLVMContext> pContext("Context", init);

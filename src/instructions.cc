#include "node-llvm.h"

static Handle<Value> addIncoming(const Arguments& args){
	ENTER_METHOD(pPHINode, 2);
	UNWRAP_ARG(pValue, v, 0);
	UNWRAP_ARG(pBasicBlock, bb, 1);
	self->addIncoming(v, bb);
	return scope.Close(args.This());
}

static void initPhi(Handle<Object> target){
	pPHINode.init(&valueConstructor);
	pPHINode.inherit(pValue);
	pPHINode.addMethod("addIncoming", &addIncoming);
}

Proto<llvm::PHINode> pPHINode("PHINode", &initPhi);


static Handle<Value> setDefaultDest(const Arguments& args){
	ENTER_METHOD(pSwitchInst, 1);
	UNWRAP_ARG(pBasicBlock, defaultCase, 0);
	self->setDefaultDest(defaultCase);
	return Undefined();
}

static Handle<Value> addCase(const Arguments& args){
	ENTER_METHOD(pSwitchInst, 2);
	UNWRAP_ARG(pValue, val, 0);
	UNWRAP_ARG(pBasicBlock, dest, 1);
	llvm::ConstantInt* onVal = llvm::dyn_cast<llvm::ConstantInt>(val);
	if (!onVal) THROW_ERROR("Value is not a constant integer");
	self->addCase(onVal, dest);
	return Undefined();
}

static void initSwitch(Handle<Object> target){
	pSwitchInst.init(&valueConstructor);
	pSwitchInst.inherit(pValue);
	pSwitchInst.addMethod("setDefaultDest", &setDefaultDest);
	pSwitchInst.addMethod("addCase", &addCase);
}

Proto<llvm::SwitchInst> pSwitchInst("SwitchInst", &initSwitch);

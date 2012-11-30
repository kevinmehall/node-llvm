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

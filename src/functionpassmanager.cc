#include "node-llvm.h"
#include "llvm/Analysis/Verifier.h"
#include "llvm/Analysis/Passes.h"
#include "llvm/Target/TargetData.h"
#include "llvm/Transforms/Scalar.h"

static Handle<Value> FPMConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR(1);
	UNWRAP_ARG(pModule, mod, 0);
	setConst(args.This(), "module", args[0]);

	auto ee = new llvm::FunctionPassManager(mod);
	pFunctionPassManager.wrap(args.This(), ee);

	return scope.Close(args.This());
}

/*
static Handle<Value> add(const Arguments& args){
	ENTER_METHOD(pFunctionPassManager, 1);
	UNWRAP_ARG(pPass, pass, 0);
	self->add(pass);
	return scope.Close(Undefined());
}*/

static Handle<Value> doInit(const Arguments& args){
	ENTER_METHOD(pFunctionPassManager, 0);
	self->doInitialization();
	return scope.Close(Undefined());
}

static Handle<Value> doFini(const Arguments& args){
	ENTER_METHOD(pFunctionPassManager, 0);
	self->doFinalization();
	return scope.Close(Undefined());
}

static Handle<Value> run(const Arguments& args){
	ENTER_METHOD(pFunctionPassManager, 1);
	UNWRAP_ARG(pFunction, fn, 0);
	bool r = self->run(*fn);
	return scope.Close(Boolean::New(r));
}

//TODO: TargetData becomes DataLayout in LLVM trunk
static Handle<Value> addTargetDataPass(const Arguments& args){
	ENTER_METHOD(pFunctionPassManager, 1);
	STRING_ARG(l, 0);
	self->add(new llvm::TargetData(l));
	return scope.Close(args.This());
}


// Function pointers (with templates) aren't generic enough here
#define PASSFN(PASSTYPE) \
static Handle<Value> add##PASSTYPE(const Arguments& args){ \
	ENTER_METHOD(pFunctionPassManager, 0); \
	self->add(llvm::create##PASSTYPE()); \
	return scope.Close(args.This()); \
}

PASSFN(BasicAliasAnalysisPass);
PASSFN(InstructionCombiningPass);
PASSFN(ReassociatePass);
PASSFN(GVNPass);

static void init(Handle<Object> target){
	pFunctionPassManager.init(&FPMConstructor);

	pFunctionPassManager.addMethod("doInitialization", &doInit);
	pFunctionPassManager.addMethod("doFinalization", &doFini);
	pFunctionPassManager.addMethod("run", &run);

	pFunctionPassManager.addMethod("addTargetDataPass", &addTargetDataPass);
	pFunctionPassManager.addMethod("addDataLayoutPass", &addTargetDataPass); // New name in LLVM trunk
	pFunctionPassManager.addMethod("addBasicAliasAnalysisPass", &addBasicAliasAnalysisPass);
	pFunctionPassManager.addMethod("addInstructionCombiningPass", &addInstructionCombiningPass);
	pFunctionPassManager.addMethod("addReassociatePass", &addReassociatePass);
	pFunctionPassManager.addMethod("addGVNPass", &addGVNPass);

	pFunctionPassManager.addToModule(target);
}

Proto<llvm::FunctionPassManager> pFunctionPassManager("FunctionPassManager", &init);

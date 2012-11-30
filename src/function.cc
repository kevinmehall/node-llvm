#include "node-llvm.h"

static Handle<Value> functionConstructor(const Arguments& args){
	ENTER_CONSTRUCTOR_POINTER(pFunction, 3);
	setConst(args.This(), "module", args[1]);
	//setConst(args.This(), "parent", args[1]);

	// Use the module's context
	auto ctx = Local<Object>::Cast(args[1])->Get(String::NewSymbol("context"));
	setConst(args.This(), "context", ctx);

	// If the functionType is not passed, create it from the function
	auto v8ft = args[2];
	if (v8ft->IsUndefined()){
		v8ft = Local<Value>::New(pFunctionType.create(self->getFunctionType(), ctx));
	}
	setConst(args.This(), "functionType", v8ft);

	// Make arguments array
	// TODO: these should be frozen or something
	auto v8Args = Local<Array>::New(Array::New());
	unsigned i = 0;
	for (auto AI = self->arg_begin(); AI != self->arg_end(); ++AI, ++i) {
		v8Args->Set(i, pValue.create(AI, args.This()));
	}
	setConst(args.This(), "arguments", v8Args);

	// Make basicBlocks array
	auto v8bb = Local<Array>::New(Array::New());
	i=0;
	for (auto BI = self->begin(); BI != self->end(); ++BI, ++i) {
		auto bb = pBasicBlock.create(ctx, External::New(BI));
		setConst(bb, "parent", args.This());
		v8bb->Set(i, bb);
	}
	setConst(args.This(), "basicBlocks", v8bb);

	return scope.Close(args.This());
}

static Handle<Value> addBasicBlock(const Arguments& args){
	ENTER_METHOD(pFunction, 1);
	UNWRAP_ARG(pBasicBlock, bb, 0);

	// append to .basicBlocks v8 array
	auto v8a = Local<Array>::New(Local<Array>::Cast(args.This()->Get(String::NewSymbol("basicBlocks"))));
	v8a->Set(v8a->Length(), args[0]);

	// Append to llvm list
	self->getBasicBlockList().push_back(bb);
	setConst(Local<Object>::Cast(args[0]), "parent", args.This());

	return scope.Close(args[0]);
}

static Handle<Value> dump(const Arguments& args){
	ENTER_METHOD(pFunction, 0);
	std::string s;
	llvm::raw_string_ostream stream(s);
	self->print(stream, NULL);
	return scope.Close(String::New(stream.str().c_str()));
}

static void init(Handle<Object> target){
	pFunction.init(&functionConstructor);
	pFunction.inherit(pValue);

	pFunction.addMethod("_addBasicBlock", &addBasicBlock);
	pFunction.addMethod("dump", &dump);
}

Proto<llvm::Function> pFunction("Function", &init);

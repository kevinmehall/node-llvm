#include "node-llvm.h"

class jsIRBuilder{
	// http://llvm.org/doxygen/classllvm_1_1IRBuilder.html
public:
	static void init(Handle<Object> target){
		pIRBuilder.init(target);

		pIRBuilder.addStaticMethod("create", &createIRB);

		pIRBuilder.addMethod("getInsertBlock", &getInsertBlock);
		pIRBuilder.addMethod("setInsertPoint", &setInsertPoint);

		/*
		pIRBuilder.addMethod("createRet", &createRet);
		pIRBuilder.addMethod("createRetVoid", &createRetVoid);
		pIRBuilder.addMethod("createAggregateRet", &createAggregateRet);

		pIRBuilder.addMethod("createBr", &createRetVoid);
		pIRBuilder.addMethod("createCondBr", &createCondBr);
		pIRBuilder.addMethod("createSwitch", &createSwitch); // special type
		pIRBuilder.addMethod("createIndirectBr", &createIndirectBr);
		
		pIRBuilder.addMethod("createUnreachable", &createIndirectBr);

		pIRBuilder.addMethod("createInvoke", &createInvoke);
		pIRBuilder.addMethod("createCall", &createCall);

		pIRBuilder.addMethod("createAlloca", &createAlloca);
		pIRBuilder.addMethod("createLoad", &createLoad); // + aligned
		pIRBuilder.addMethod("createStore", &createStore); // + aligned
		*/

		// TODO: GEP
		// TODO: special: phi

		// TODO: unary ops

		// bool HasNUW=false, bool HasNSW=false
		//pIRBuilder.addMethod("createAdd", 	&BinOpWrMethod<&IRBuilder::CreateAdd> );
		//pIRBuilder.addMethod("createSub", 	&BinOpWrMethod<&IRBuilder::CreateSub> );
		//pIRBuilder.addMethod("createMul", 	&BinOpWrMethod<&IRBuilder::CreateMul> );
		//pIRBuilder.addMethod("createShl", 	&BinOpWrMethod<&IRBuilder::CreateShl> );

		// bool isExact=false
		//pIRBuilder.addMethod("createUDiv", 	&BinOpExMethod<&IRBuilder::CreateUDiv> );
		//pIRBuilder.addMethod("createSDiv", 	&BinOpExMethod<&IRBuilder::CreateSDiv> );
		//pIRBuilder.addMethod("createLShr", 	&BinOpExMethod<&IRBuilder::CreateLShr> );
		//pIRBuilder.addMethod("createAShr", 	&BinOpExMethod<&IRBuilder::CreateAShr> );
		
		pIRBuilder.addMethod("createNSWAdd", 	&BinOpMethod<&IRBuilder::CreateNSWAdd> );
		pIRBuilder.addMethod("createNUWAdd", 	&BinOpMethod<&IRBuilder::CreateNUWAdd> );
		pIRBuilder.addMethod("createFAdd", 		&BinOpMethod<&IRBuilder::CreateFAdd> ); // MDNode *FPMathTag=0)
		pIRBuilder.addMethod("createNSWSub", 	&BinOpMethod<&IRBuilder::CreateNSWSub> );
		pIRBuilder.addMethod("createNUWSub", 	&BinOpMethod<&IRBuilder::CreateNUWSub> );
		pIRBuilder.addMethod("createFSub", 		&BinOpMethod<&IRBuilder::CreateFSub> ); // MDNode *FPMathTag=0)
		pIRBuilder.addMethod("createNSWMul", 	&BinOpMethod<&IRBuilder::CreateNSWMul> );
		pIRBuilder.addMethod("createNUWMul", 	&BinOpMethod<&IRBuilder::CreateNUWMul> );
		pIRBuilder.addMethod("createFMul", 		&BinOpMethod<&IRBuilder::CreateFMul> ); // MDNode *FPMathTag=0)
		pIRBuilder.addMethod("createExactUDiv", &BinOpMethod<&IRBuilder::CreateExactUDiv> );
		pIRBuilder.addMethod("createExactSDiv", &BinOpMethod<&IRBuilder::CreateExactSDiv> );
		pIRBuilder.addMethod("createFDiv", 		&BinOpMethod<&IRBuilder::CreateFDiv> ); // MDNode *FPMathTag=0)
		pIRBuilder.addMethod("createURem", 		&BinOpMethod<&IRBuilder::CreateURem> );
		pIRBuilder.addMethod("createSRem", 		&BinOpMethod<&IRBuilder::CreateSRem> );
		pIRBuilder.addMethod("createFRem", 		&BinOpMethod<&IRBuilder::CreateFRem> ); // MDNode *FPMathTag=0)
		pIRBuilder.addMethod("createAnd", 		&BinOpMethod<&IRBuilder::CreateAnd> );
		pIRBuilder.addMethod("createOr", 		&BinOpMethod<&IRBuilder::CreateOr> );
		pIRBuilder.addMethod("createXor", 		&BinOpMethod<&IRBuilder::CreateXor> );

		pIRBuilder.addMethod("createICmpEQ",  &BinOpMethod<&IRBuilder::CreateICmpEQ> );
		pIRBuilder.addMethod("createICmpNE",  &BinOpMethod<&IRBuilder::CreateICmpNE> );
		pIRBuilder.addMethod("createICmpUGT", &BinOpMethod<&IRBuilder::CreateICmpUGT> );
		pIRBuilder.addMethod("createICmpUGE", &BinOpMethod<&IRBuilder::CreateICmpUGE> );
		pIRBuilder.addMethod("createICmpULT", &BinOpMethod<&IRBuilder::CreateICmpULT> );
		pIRBuilder.addMethod("createICmpULE", &BinOpMethod<&IRBuilder::CreateICmpULE> );
		pIRBuilder.addMethod("createICmpSGT", &BinOpMethod<&IRBuilder::CreateICmpSGT> );
		pIRBuilder.addMethod("createICmpSGE", &BinOpMethod<&IRBuilder::CreateICmpSGE> );
		pIRBuilder.addMethod("createICmpSLT", &BinOpMethod<&IRBuilder::CreateICmpSLT> );
		pIRBuilder.addMethod("createICmpSLE", &BinOpMethod<&IRBuilder::CreateICmpSLE> );
		pIRBuilder.addMethod("createFCmpOEQ", &BinOpMethod<&IRBuilder::CreateFCmpOEQ> );
		pIRBuilder.addMethod("createFCmpOGT", &BinOpMethod<&IRBuilder::CreateFCmpOGT> );
		pIRBuilder.addMethod("createFCmpOGE", &BinOpMethod<&IRBuilder::CreateFCmpOGE> );
		pIRBuilder.addMethod("createFCmpOLT", &BinOpMethod<&IRBuilder::CreateFCmpOLT> );
		pIRBuilder.addMethod("createFCmpOLE", &BinOpMethod<&IRBuilder::CreateFCmpOLE> );
		pIRBuilder.addMethod("createFCmpONE", &BinOpMethod<&IRBuilder::CreateFCmpONE> );
		pIRBuilder.addMethod("createFCmpORD", &BinOpMethod<&IRBuilder::CreateFCmpORD> );
		pIRBuilder.addMethod("createFCmpUNO", &BinOpMethod<&IRBuilder::CreateFCmpUNO> );
		pIRBuilder.addMethod("createFCmpUEQ", &BinOpMethod<&IRBuilder::CreateFCmpUEQ> );
		pIRBuilder.addMethod("createFCmpUGT", &BinOpMethod<&IRBuilder::CreateFCmpUGT> );
		pIRBuilder.addMethod("createFCmpUGE", &BinOpMethod<&IRBuilder::CreateFCmpUGE> );
		pIRBuilder.addMethod("createFCmpULT", &BinOpMethod<&IRBuilder::CreateFCmpULT> );
		pIRBuilder.addMethod("createFCmpULE", &BinOpMethod<&IRBuilder::CreateFCmpULE> );
		pIRBuilder.addMethod("createFCmpUNE", &BinOpMethod<&IRBuilder::CreateFCmpUNE> );

		//CreateICmp (CmpInst::Predicate P, Value *LHS, Value *RHS, const Twine &Name="")
		//CreateFCmp (CmpInst::Predicate P, Value *LHS, Value *RHS, const Twine &Name="")
		
		// TODO: there are more (CmpXXX)

		// TODO: cast (Value*, Type*) functions
	}

	static Handle<Value> createIRB(const Arguments& args){
		HandleScope scope;
		CHECK_N_ARGS(1);
		IRBuilder* b = new IRBuilder(*pContext.unwrap(args[0]));
		return scope.Close(pIRBuilder.create(b, args[0]));
	}

	static Handle<Value> getInsertBlock(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 0);
		return scope.Close(pBasicBlock.create(self->GetInsertBlock(), args.This()));
	}

	static Handle<Value> setInsertPoint(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 1);
		llvm::BasicBlock* p = pBasicBlock.unwrap(args[1]);
		// TODO: could also be Instruction
		self->SetInsertPoint(p);
		return args[1];
	}



	static const char* nameValue(Handle<Value> arg){
		return *String::Utf8Value(arg->ToString());
	}

	typedef llvm::Value* (IRBuilder::*UnaryOpFn)(llvm::Value*, const llvm::Twine&);
	template<UnaryOpFn method>
	static Handle<Value> UnaryOpMethod(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 1);

		llvm::Twine name;
		if (args.Length() > 1) name = nameValue(args[1]);

		llvm::Value* v = pValue.unwrap(args[0]);
		return scope.Close(pValue.create((self->*method)(v, name), args.This()));
	}

	typedef llvm::Value* (IRBuilder::*BinaryOpFn)(llvm::Value*, llvm::Value*, const llvm::Twine&);
	template<BinaryOpFn method>
	static Handle<Value> BinOpMethod(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 2);

		llvm::Twine name;
		if (args.Length() > 2) name = nameValue(args[2]);

		llvm::Value* l = pValue.unwrap(args[0]);
		llvm::Value* r = pValue.unwrap(args[1]);
		return scope.Close(pValue.create((self->*method)(l, r, name), args.This()));
	}
};
Proto<IRBuilder> pIRBuilder("IRBuilder", &jsIRBuilder::init, "parent");
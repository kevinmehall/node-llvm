#include "node-llvm.h"

class jsIRBuilder{
	// http://llvm.org/doxygen/classllvm_1_1IRBuilder.html
public:
	static void init(Handle<Object> target){
		pIRBuilder.init(&IRBConstructor);

		pIRBuilder.addMethod("setInsertPoint", &setInsertPoint);

		pIRBuilder.addMethod("createRet", &createRet);
		pIRBuilder.addMethod("createRetVoid", &createRetVoid);
		//pIRBuilder.addMethod("createAggregateRet", &createAggregateRet);

		pIRBuilder.addMethod("createBr", &createBr);
		pIRBuilder.addMethod("createCondBr", &createCondBr);
		
		//IRBuilder.addMethod("createSwitch", &createSwitch); // special type
		//pIRBuilder.addMethod("createIndirectBr", &createIndirectBr);
		
		//pIRBuilder.addMethod("createUnreachable", &createIndirectBr);

		//pIRBuilder.addMethod("createInvoke", &createInvoke);
		pIRBuilder.addMethod("createCall", &createCall);
		
		pIRBuilder.addMethod("createAlloca", &createAlloca);
		pIRBuilder.addMethod("createLoad", &createLoad); // + aligned, volatile
		pIRBuilder.addMethod("createStore", &createStore); // + aligned, volatile

		pIRBuilder.addMethod("createGEP", &createGEP);

		pIRBuilder.addMethod("createPHI", &createPHI);
		pIRBuilder.addMethod("createSelect", &createSelect);

		// bool HasNUW=false, bool HasNSW=false
		pIRBuilder.addMethod("createAdd", 	&BinOpWrMethod<&IRBuilder::CreateAdd> );
		pIRBuilder.addMethod("createSub", 	&BinOpWrMethod<&IRBuilder::CreateSub> );
		pIRBuilder.addMethod("createMul", 	&BinOpWrMethod<&IRBuilder::CreateMul> );
		pIRBuilder.addMethod("createShl", 	&BinOpWrMethod<&IRBuilder::CreateShl> );

		// bool isExact=false
		pIRBuilder.addMethod("createUDiv", 	&BinOpExMethod<&IRBuilder::CreateUDiv> );
		pIRBuilder.addMethod("createSDiv", 	&BinOpExMethod<&IRBuilder::CreateSDiv> );
		pIRBuilder.addMethod("createLShr", 	&BinOpExMethod<&IRBuilder::CreateLShr> );
		pIRBuilder.addMethod("createAShr", 	&BinOpExMethod<&IRBuilder::CreateAShr> );
		
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

//		pIRBuilder.addMethod("createNeg",       &UnaryOpMethod<&IRBuilder::CreateNeg> ); //  bool HasNUW=false, bool HasNSW=false)
		pIRBuilder.addMethod("createNSWNeg",    &UnaryOpMethod<&IRBuilder::CreateNSWNeg> );
		pIRBuilder.addMethod("createNUWNeg",    &UnaryOpMethod<&IRBuilder::CreateNUWNeg> );
		pIRBuilder.addMethod("createFNeg",      &UnaryOpMethod<&IRBuilder::CreateFNeg> ); // MDNode *FPMathTag=0
		pIRBuilder.addMethod("createNot",       &UnaryOpMethod<&IRBuilder::CreateNot> );
		pIRBuilder.addMethod("createIsNull",    &UnaryOpMethod<&IRBuilder::CreateIsNull> );
		pIRBuilder.addMethod("createIsNotNull", &UnaryOpMethod<&IRBuilder::CreateIsNotNull> );

		//CreateICmp (CmpInst::Predicate P, Value *LHS, Value *RHS, const Twine &Name="")
		//CreateFCmp (CmpInst::Predicate P, Value *LHS, Value *RHS, const Twine &Name="")
		
		pIRBuilder.addMethod("createTrunc", &CastOpMethod<&IRBuilder::CreateTrunc> );
		pIRBuilder.addMethod("createZExt", &CastOpMethod<&IRBuilder::CreateZExt> );
		pIRBuilder.addMethod("createSExt", &CastOpMethod<&IRBuilder::CreateSExt> );
		//pIRBuilder.addMethod("createZExtOrTrunc", &CastOpMethod<&IRBuilder::CreateZExtOrTrunc> );
		//pIRBuilder.addMethod("createSExtOrTrunc", &CastOpMethod<&IRBuilder::CreateSExtOrTrunc> );
		pIRBuilder.addMethod("createFPToUI", &CastOpMethod<&IRBuilder::CreateFPToUI> );
		pIRBuilder.addMethod("createFPToSI", &CastOpMethod<&IRBuilder::CreateFPToSI> );
		pIRBuilder.addMethod("createUIToFP", &CastOpMethod<&IRBuilder::CreateUIToFP> );
		pIRBuilder.addMethod("createSIToFP", &CastOpMethod<&IRBuilder::CreateSIToFP> );
		pIRBuilder.addMethod("createFPTrunc", &CastOpMethod<&IRBuilder::CreateFPTrunc> );
		pIRBuilder.addMethod("createFPExt", &CastOpMethod<&IRBuilder::CreateFPExt> );
		pIRBuilder.addMethod("createPtrToInt", &CastOpMethod<&IRBuilder::CreatePtrToInt> );
		pIRBuilder.addMethod("createIntToPtr", &CastOpMethod<&IRBuilder::CreateIntToPtr> );
		pIRBuilder.addMethod("createBitCast", &CastOpMethod<&IRBuilder::CreateBitCast> );
		pIRBuilder.addMethod("createZExtOrBitCast", &CastOpMethod<&IRBuilder::CreateZExtOrBitCast> );
		pIRBuilder.addMethod("createSExtOrBitCast", &CastOpMethod<&IRBuilder::CreateSExtOrBitCast> );
		pIRBuilder.addMethod("createTruncOrBitCast", &CastOpMethod<&IRBuilder::CreateTruncOrBitCast> );
		//pIRBuilder.addMethod("createCast", &CastOpMethod<&IRBuilder::CreateCast> );
		pIRBuilder.addMethod("createPointerCast", &CastOpMethod<&IRBuilder::CreatePointerCast> );
		//pIRBuilder.addMethod("createIntCast", &CastOpMethod<&IRBuilder::CreateIntCast> );
		pIRBuilder.addMethod("createFPCast", &CastOpMethod<&IRBuilder::CreateFPCast> );
	}

	static Handle<Value> IRBConstructor(const Arguments& args){
		ENTER_CONSTRUCTOR(1);
		CHECK_N_ARGS(1);
		UNWRAP_ARG(pContext, context, 0);
		setConst(args.This(), "context", args[0]);
		setConst(args.This(), "insertBlock", Undefined());
		IRBuilder* b = new IRBuilder(*context);
		pIRBuilder.wrap(args.This(), b);
		return scope.Close(args.This());
	}

	static Handle<Value> setInsertPoint(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 1);
		UNWRAP_ARG(pBasicBlock, p, 0); // TODO: could also be Instruction
		setConst(args.This(), "insertBlock", args[0]);
		self->SetInsertPoint(p);
		return scope.Close(args[0]);
	}

	#define RETURN_INSTR(TP, VAL) \
		return scope.Close(TP.create(VAL, args.This()->Get(String::NewSymbol("insertBlock"))));

	static Handle<Value> createRet(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 1);
		UNWRAP_ARG(pValue, v, 0);
		RETURN_INSTR(pValue, self->CreateRet(v));
	}

	static Handle<Value> createRetVoid(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 0);
		RETURN_INSTR(pValue, self->CreateRetVoid());
	}

	static Handle<Value> createBr(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 1);
		UNWRAP_ARG(pBasicBlock, dest, 0);
		RETURN_INSTR(pValue, self->CreateBr(dest));
	}

	static Handle<Value> createCondBr(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 3);
		UNWRAP_ARG(pValue, cond, 0);
		UNWRAP_ARG(pBasicBlock, destT, 1);
		UNWRAP_ARG(pBasicBlock, destF, 2);
		RETURN_INSTR(pValue, self->CreateCondBr(cond, destT, destF));
	}

	static Handle<Value> createAlloca(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 2);
		UNWRAP_ARG(pType, type, 0);
		auto arraySize = pValue.unwrap(args[1]); // can be null
		STRING_ARG(name, 2);
		RETURN_INSTR(pValue, self->CreateAlloca(type, arraySize, name));
	}

	static Handle<Value> createLoad(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 1);
		UNWRAP_ARG(pValue, ptr, 0);
		STRING_ARG(name, 1);
		RETURN_INSTR(pValue, self->CreateLoad(ptr, name));
	}

	static Handle<Value> createStore(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 2);
		UNWRAP_ARG(pValue, ptr, 0);
		UNWRAP_ARG(pValue, val, 1);
		RETURN_INSTR(pValue, self->CreateStore(ptr, val));
	}

	static Handle<Value> createGEP(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 2);
		UNWRAP_ARG(pValue, ptr, 0);
		ARRAY_UNWRAP_ARG(pValue, llvm::Value, idxList, 1);
		STRING_ARG(name, 2);
		RETURN_INSTR(pValue, self->CreateGEP(ptr, idxList, name));
	}

	static Handle<Value> createCall(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 2);
		UNWRAP_ARG(pValue, fn, 0);
		ARRAY_UNWRAP_ARG(pValue, llvm::Value, fnargs, 1);
		STRING_ARG(name, 2);
		RETURN_INSTR(pValue, self->CreateCall(fn, fnargs, name));
	}

	static Handle<Value> createPHI(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 1);
		UNWRAP_ARG(pType, type, 0);
		STRING_ARG(name, 1);
		RETURN_INSTR(pPHINode, self->CreatePHI(type, 2, name));
	}

	static Handle<Value> createSelect(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 3);
		UNWRAP_ARG(pValue, cond, 0);
		UNWRAP_ARG(pValue, vtrue, 1);
		UNWRAP_ARG(pValue, vfalse, 2);
		STRING_ARG(name, 3);
		RETURN_INSTR(pValue, self->CreateSelect(cond, vtrue, vfalse, name));
	}

	typedef llvm::Value* (IRBuilder::*UnaryOpFn)(llvm::Value*, const llvm::Twine&);
	template<UnaryOpFn method>
	static Handle<Value> UnaryOpMethod(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 1);
		UNWRAP_ARG(pValue, v, 0);
		STRING_ARG(name, 1);

		RETURN_INSTR(pValue, (self->*method)(v, name));
	}

	typedef llvm::Value* (IRBuilder::*BinaryOpFn)(llvm::Value*, llvm::Value*, const llvm::Twine&);
	template<BinaryOpFn method>
	static Handle<Value> BinOpMethod(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 2);
		UNWRAP_ARG(pValue, l, 0);
		UNWRAP_ARG(pValue, r, 1);
		STRING_ARG(name, 2);
		RETURN_INSTR(pValue, (self->*method)(l, r, name));
	}

	typedef llvm::Value* (IRBuilder::*BinaryOpWrFn)(llvm::Value*, llvm::Value*, const llvm::Twine&, bool, bool);
	template<BinaryOpWrFn method>
	static Handle<Value> BinOpWrMethod(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 2);
		UNWRAP_ARG(pValue, l, 0);
		UNWRAP_ARG(pValue, r, 1);
		STRING_ARG(name, 2);
		BOOL_ARG(hasNUW, 3);
		BOOL_ARG(hasNSW, 4);
		RETURN_INSTR(pValue, (self->*method)(l, r, name, hasNUW, hasNSW));
	}

	typedef llvm::Value* (IRBuilder::*BinaryOpExFn)(llvm::Value*, llvm::Value*, const llvm::Twine&, bool);
	template<BinaryOpExFn method>
	static Handle<Value> BinOpExMethod(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 2);
		UNWRAP_ARG(pValue, l, 0);
		UNWRAP_ARG(pValue, r, 1);
		STRING_ARG(name, 2);
		BOOL_ARG(isExact, 3);
		RETURN_INSTR(pValue, (self->*method)(l, r, name, isExact));
	}

	typedef llvm::Value* (IRBuilder::*CastOpFn)(llvm::Value*, llvm::Type*, const llvm::Twine&);
	template<CastOpFn method>
	static Handle<Value> CastOpMethod(const Arguments& args){
		ENTER_METHOD(pIRBuilder, 2);
		UNWRAP_ARG(pValue, v, 0);
		UNWRAP_ARG(pType, t, 1);
		STRING_ARG(name, 2);
		RETURN_INSTR(pValue, (self->*method)(v, t, name));
	}
};
Proto<IRBuilder> pIRBuilder("IRBuilder", &jsIRBuilder::init);

#pragma once

#include <v8.h>
#include <node.h>
#include <node_version.h>
#include <node_buffer.h>
#include <uv.h>

using namespace node;
using namespace v8;

#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Function.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/PassManager.h"
#include "llvm/Support/raw_ostream.h"

#include "protobuilder.h"

typedef llvm::IRBuilder<> IRBuilder; // just so I don't keep forgetting the <>

extern Proto<llvm::LLVMContext> pContext;
extern Proto<llvm::Module>      pModule;
extern Proto<llvm::Value>       pValue;
extern Proto<llvm::BasicBlock>  pBasicBlock;
extern Proto<IRBuilder>         pIRBuilder;
extern Proto<llvm::Function>    pFunction;
extern Proto<llvm::Type> 		pType;
extern Proto<llvm::IntegerType> pIntegerType;
extern Proto<llvm::Type> 		pFPType;
extern Proto<llvm::FunctionType> pFunctionType;
extern Proto<llvm::ExecutionEngine> pExecutionEngine;
extern Proto<llvm::FunctionPassManager> pFunctionPassManager;
extern Proto<llvm::PHINode> pPHINode;
extern Proto<llvm::SwitchInst> pSwitchInst;

// Common constructor for Value and subclasses
Handle<Value> valueConstructor(const Arguments& args);

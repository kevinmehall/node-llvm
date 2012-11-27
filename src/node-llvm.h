#pragma once

#include <v8.h>
#include <node.h>
#include <node_version.h>
#include <node_buffer.h>
#include <uv.h>

using namespace node;
using namespace v8;

#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/Support/IRBuilder.h"
#include "llvm/BasicBlock.h"

#include "protobuilder.h"

typedef llvm::IRBuilder<> IRBuilder; // just so I don't keep forgetting the <>

extern Proto<llvm::LLVMContext> pContext;
extern Proto<llvm::Module>      pModule;
extern Proto<llvm::BasicBlock>  pBasicBlock;
extern Proto<llvm::Value>       pValue;
extern Proto<IRBuilder>         pIRBuilder;

// FunctionType
// Function
// BasicBlock
// ConstantInt / ConstantFP

// Type
// PassManager
// FunctionPassManager

// ExecutionEngine

// Instruction
//	PHINode
//	SwitchInstruction
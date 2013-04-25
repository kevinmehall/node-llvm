NodeJS Bindings for LLVM
------------------------

Requires LLVM 3.2:

Ubuntu: `sudo apt-get install libllvm3.2 llvm-3.2-dev`  
Fedora: `sudo yum install llvm llvm-devel llvm-static`

  * Wraps the most important LLVM APIs.
  * A port of the LLVM [Kaleidoscope example](http://llvm.org/docs/tutorial/) can be found in `examples/`. Expressions must be entered on a single line.
  * Uses [Node-FFI](https://github.com/rbranson/node-ffi) to make JIT functions callable from JS. (Currently assumes all arguments and return types are `double` -- fix `index.js`)
  * Currently does not free LLVM objects' memory when a Module is GC'd.

## API: llvm

For details, see the LLVM [C++ API documentation](http://llvm.org/doxygen/index.html) and [Reference Manual](http://llvm.org/docs/LangRef.html).

    llvm = require('llvm')

**.globalContext**

### Context

**.voidTy**  
**.labelTy**  
**.floatTy**  
**.doubleTy**  
**.metadataTy**  
**.x86\_FP80Ty**  
**.fP128Ty**  
**.pPC\_FP128Ty**  
**.x86\_MMXTy**  
**.int1Ty**  
**.int8Ty**  
**.int16Ty**  
**.int32Ty**  
**.int64Ty**  
  
**.getFunctionType**(returnType, argTypes)  
**.getIntNTy**(width)  

### Module

**new llvm.module**(id, context)  

**.context**  
**.moduleIdentifier**   

**.getFunction**(name)  
**.getOrInsertFunction**(name, funcType)  
**.dump**()  

### Value

**.parent**  
**.name** [r/w]  
**.dump()**  

### Function

Inherits **Value**

**.module**  
**.context**  
**.functionType**  
**.arguments** -- modifications to the array are not reflected in the LLVM structure  
**.basicBlocks** -- modifications to the array are not reflected in the LLVM structure  

**.dump**()  
**.addBasicBlock**(nameOrBlock) - adds an existing block, or creates one if passed a string name. Returns the added block.  

### BasicBlock
Inherits **Value**

**new llvm.BasicBlock**(context)

**.context**  

### Type

**.getPointerTo**() -- PointerType to this type

### IntegerType

Inherits **Type**

**.const**(val, [radix]) -- Get ConstantInteger from JS number or string and radix  

### FPType

Inherits **Type**

**.const**(val) -- Get ConstantFP from JS number or string

### FunctionType

Inherits **Type**

### IRBuilder

**new llvm.IRBuilder**(context)  

**.setInsertPoint**(block)  
**.insertBlock** -- the current insertion point block 

The following methods create an instruction and add it at the current insertion point, returning the instruction's value:
  
**.createRet**(val)  
**.createRetVoid**()  
**.createBr**(destBlock)  
**.createCondBr**(cond, destT, destF)  
**.createSwitch**(V, defaultBlock) -- creates SwitchInst with `.addCase(value, block)` and `.setDefaultDest(block)`  
**.createUnreachable**()  
**.createCall**(func, args)  
**.createAlloca**(type, [arraySize], [name])  
**.createLoad**(ptr, [name])  
**.createStore**(val, ptr, [name])  
**.createGEP**(ptr, idxList, [name])  
**.createPHI**(type, [name]) -- creates PHINode with `.addIncoming(block, value)`  
**.createSelect**(cond, vTrue, vFalse)  
  
**.createAdd**(l, r, [name], [hasNUW], [hasNSW])  
**.createSub**(l, r, [name], [hasNUW], [hasNSW])  
**.createMul**(l, r, [name], [hasNUW], [hasNSW])  
**.createShl**(l, r, [name], [hasNUW], [hasNSW])  
 
**.createUDiv**(l, r, [name], [isExact])  
**.createSDiv**(l, r, [name], [isExact])  
**.createLShr**(l, r, [name], [isExact])  
**.createAShr**(l, r, [name], [isExact])  
 
**.createNSWAdd**(l, r, [name])  
**.createNUWAdd**(l, r, [name])  
**.createFAdd**(l, r, [name])  
**.createNSWSub**(l, r, [name])  
**.createNUWSub**(l, r, [name])  
**.createFSub**(l, r, [name])  
**.createNSWMul**(l, r, [name])  
**.createNUWMul**(l, r, [name])  
**.createFMul**(l, r, [name])  
**.createExactUDiv**(l, r, [name])  
**.createExactSDiv**(l, r, [name])  
**.createFDiv**(l, r, [name])  
**.createURem**(l, r, [name])  
**.createSRem**(l, r, [name])  
**.createFRem**(l, r, [name])  
**.createAnd**(l, r, [name])  
**.createOr**(l, r, [name])  
**.createXor**(l, r, [name])  
 
**.createICmpEQ**(l, r, [name])  
**.createICmpNE**(l, r, [name])  
**.createICmpUGT**(l, r, [name])  
**.createICmpUGE**(l, r, [name])  
**.createICmpULT**(l, r, [name])  
**.createICmpULE**(l, r, [name])  
**.createICmpSGT**(l, r, [name])  
**.createICmpSGE**(l, r, [name])  
**.createICmpSLT**(l, r, [name])  
**.createICmpSLE**(l, r, [name])  
**.createFCmpOEQ**(l, r, [name])  
**.createFCmpOGT**(l, r, [name])  
**.createFCmpOGE**(l, r, [name])  
**.createFCmpOLT**(l, r, [name])  
**.createFCmpOLE**(l, r, [name])  
**.createFCmpONE**(l, r, [name])  
**.createFCmpORD**(l, r, [name])  
**.createFCmpUNO**(l, r, [name])  
**.createFCmpUEQ**(l, r, [name])  
**.createFCmpUGT**(l, r, [name])  
**.createFCmpUGE**(l, r, [name])  
**.createFCmpULT**(l, r, [name])  
**.createFCmpULE**(l, r, [name])  
**.createFCmpUNE**(l, r, [name])  

**.createNSWNeg**(v, [name])  
**.createNUWNeg**(v, [name])  
**.createFNeg**(v, [name])  
**.createNot**(v, [name])  
**.createIsNull**(v, [name])  
**.createIsNotNull**(v, [name])  

**.createTrunc**(v, type, [name])  
**.createZExt**(v, type, [name])  
**.createSExt**(v, type, [name])  
**.createFPToUI**(v, type, [name])  
**.createFPToSI**(v, type, [name])  
**.createUIToFP**(v, type, [name])  
**.createSIToFP**(v, type, [name])  
**.createFPTrunc**(v, type, [name])  
**.createFPExt**(v, type, [name])  
**.createPtrToInt**(v, type, [name])  
**.createIntToPtr**(v, type, [name])  
**.createBitCast**(v, type, [name])  
**.createZExtOrBitCast**(v, type, [name])  
**.createSExtOrBitCast**(v, type, [name])  
**.createTruncOrBitCast**(v, type, [name])  
**.createPointerCast**(v, type, [name])  
**.createFPCast**(v, type, [name])  

### PHINode
Inherits **Value**

**.addIncoming**(value, block)  

### SwitchInst

**.addCase**(value, block)  
**.setDefaultDest**(block)  

### FunctionPassManager

**new llvm.FunctionPassManager**(module)

**.doInitialization**()  
**.doFinalization**()  
**.run**(function)  

The following methods create and add an `llvm::Pass` to the PassManager:

**.addDataLayoutPass**(dataLayout)  
**.addBasicAliasAnalysisPass**()  
**.addInstructionCombiningPass**()  
**.addReassociatePass**()  
**.addGVNPass**()  

### ExecutionEngine
**new llvm.ExecutionEngine**(module)  

**.getPointerToFunction**(fnName) -- Get 0-byte Buffer at the JIT'd function's address.  
**.getFFIFunction**(fnName) -- Wrap the compiled function with the [FFI](https://github.com/rbranson/node-ffi) library, returning a JavaScript callable.  

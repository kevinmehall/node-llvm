NodeJS Bindings for LLVM
------------------------

Requires LLVM 3.0:

	sudo apt-get install libllvm3.0 llvm-3.0-dev

Supported APIs
	- Context
	- Module: minimal
	- Function: minimal
	- FunctionType: minimal
	- Type: minimal
	- Value
	- BasicBlock: minimal
	- IRBuilder: minimal
	- FunctionPassManager
		- note: instead of .add()'ing Pass objects from CreateXYZPass(), use fpm.addXYZPass() to create and add
	- ExecutionEngine

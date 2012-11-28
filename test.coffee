llvm = require('bindings')('llvm')

ctx = llvm.getGlobalContext()
double = ctx.getDoubleTy()

m = new llvm.Module("m", ctx)
ft = ctx.getFunctionType(double, [double])
fn = m.getOrInsertFunction("testfn", ft)
bb = fn.addBasicBlock("entry")

b = llvm.IRBuilder.create(ctx)
b.setInsertPoint(bb)
v = fn.arguments[0]
v.name = 'n'
v = b.createFAdd(v, ctx.getDouble(5), "b")
b.createRet(v)

console.log m.dump()

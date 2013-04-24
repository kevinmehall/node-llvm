llvm = require '../index'
readline = require 'readline'

#===----------------------------------------------------------------------===#
# Lexer
#===----------------------------------------------------------------------===#
# The lexer returns strings if it is an unknown character, otherwise one
# of these for known things.

class Token
  constructor: (@name) ->

tok_eof = new Token('eof')
EOF = '\0'

# commands
tok_def = new Token('def')
tok_extern = new Token('extern')

# primary
tok_identifier = new Token('identifier')
tok_number = new Token('number')

# control
tok_if = new Token('if')
tok_then = new Token('then')
tok_else = new Token('else')
tok_for = new Token('for')
tok_in = new Token('in')

# operators
tok_binary = new Token('binary')
tok_unary = new Token('unary')

# var definition
tok_var = new Token('var')

isalnum = (c) -> /\w/.test(c)
isalpha = (c) -> /[a-zA-Z]/.test(c)
isdigit = (c) -> /[0-9.]/.test(c)
isspace = (c) -> /\s/.test(c)

isascii = (c) -> c not instanceof Token

class Lexer
  constructor: ->
    @buffer = []

  # add a line to the buffer
  feed: (line) ->
    for i in line then @buffer.push(i)

  getchar: ->
    if @buffer.length
      @buffer.shift()
    else
      EOF

  testchar: ->
    if @buffer.length
      @buffer[0]
    else
      EOF

  # gettok - Return the next token from standard input.
  gettok: ->
    lastChar = ' '

    # Skip any whitespace.
    while isspace(lastChar)
      lastChar = @getchar()

    if isalpha(lastChar) # identifier: [a-zA-Z][a-zA-Z0-9]*
      @val = lastChar
      while isalnum(@testchar())
        @val += @getchar()

      return switch @val
        when "def" then tok_def
        when "extern" then tok_extern
        when "if" then tok_if
        when "then" then tok_then
        when "else" then tok_else
        when "for" then tok_for
        when "in" then tok_in
        when "binary" then tok_binary
        when "unary" then tok_unary
        when "var" then tok_var
        else tok_identifier

    if isdigit(lastChar) or lastChar == '.'   # Number: [0-9.]+
      numStr = lastChar
      while isdigit(@testchar())
        numStr += @getchar()

      @val = parseFloat(numStr, 10)
      return tok_number

    if lastChar == '#'
      # Comment until end of line.

      while lastChar != EOF and lastChar != '\n' and lastChar != '\r'
        lastChar = @getchar()
      
      if lastChar != EOF
        return @gettok()
    
    # Check for end of file.  Don't eat the EOF.
    if lastChar == EOF
      return tok_eof

    # Otherwise, just return the character as its ascii value.
    return lastChar

#===----------------------------------------------------------------------===#
# Abstract Syntax Tree (aka Parse Tree)
#===----------------------------------------------------------------------===#

# ExprAST - Base class for all expression nodes.
class ExprAST
  toString: ->
    n = ["<#{this.constructor.name}>"]
    for own k, v of this
      n.push "\t#{k}: #{v?.toString().replace(/\n/g, '\n\t')}"
    n.join('\n')
    

# NumberExprAST - Expression class for numeric literals like "1.0".
class NumberExprAST extends ExprAST
  constructor: (@val) ->

# VariableExprAST - Expression class for referencing a variable, like "a".
class VariableExprAST extends ExprAST
  constructor: (@name) ->

# UnaryExprAST - Expression class for a unary operator.
class UnaryExprAST extends ExprAST
  constructor: (@opcode, @operand) ->

# BinaryExprAST - Expression class for a binary operator.
class BinaryExprAST extends ExprAST
  constructor: (@op, @lhs, @rhs) ->

# CallExprAST - Expression class for function calls.
class CallExprAST extends ExprAST
  constructor: (@callee, @args) ->

# IfExprAST - Expression class for if/then/else.
class IfExprAST extends ExprAST
  constructor: (@cond, @then, @else) ->

# ForExprAST - Expression class for for/in.
class ForExprAST extends ExprAST
  constructor: (@varName, @start, @end, @step, @body) ->
  
# VarExprAST - Expression class for var/in
class VarExprAST extends ExprAST
  constructor: (@varNames, @body) ->

# PrototypeAST - This class represents the "prototype" for a function,
# which captures its name, and its argument names (thus implicitly the number
# of arguments the function takes), as well as if it is an operator.
class PrototypeAST
  constructor: (@name, @args, @isOperator=false, @precedence=0) ->

  isUnaryOp:  -> @isOperator and @args.length == 1
  isBinaryOp: -> @isOperator and @args.length == 2
  getOperatorName: ->
    console.assert(@isUnaryOp() or @isBinaryOp())
    @name.slice(-1)

  createArgumentAllocas: (f) ->

# FunctionAST - This class represents a function definition itself.
class FunctionAST
  constructor: (@Proto, @body) ->

#===----------------------------------------------------------------------===#
# Parser
#===----------------------------------------------------------------------===#

class Parser
  constructor: ->
    @lexer = new Lexer()
    @curTok = null

    # This holds the precedence for each binary operator that is defined.
    # char -> int
    @binopPrecedence = {}

  # @curTok/getNextToken - Provide a simple token buffer.  @curTok is the current
  # token the parser is looking at.  getNextToken reads another token from the
  # lexer and updates @curTok with its results.
  getNextToken: ->
    @curTok = @lexer.gettok()
    #console.log 'tok', @curTok
    #console.trace 'at'
    @curTok

  # GetTokPrecedence - Get the precedence of the pending binary operator token.
  GetTokPrecedence: ->
    if !isascii(@curTok)
      return -1
    
    # Make sure it's a declared binop.
    tokPrec = @binopPrecedence[@curTok] ? -1

  # identifierexpr
  #   ::= identifier
  #   ::= identifier '(' expression* ')'
  ParseIdentifierExpr: ->
    idName = @lexer.val
    
    @getNextToken()  # eat identifier.
    
    if @curTok != '(' # Simple variable ref.
      return new VariableExprAST(idName)
    
    # Call.
    @getNextToken()  # eat (
    args = []
    if @curTok != ')'
      while true
        arg = @ParseExpression()
        if not arg then return null
        args.push(arg)

        if @curTok == ')' then break

        if @curTok != ','
          throw new Error("Expected ')' or ',' in argument list")
        @getNextToken()

    # Eat the ')'.
    @getNextToken()
    
    return new CallExprAST(idName, args)

  # numberexpr ::= number
  ParseNumberExpr: ->
    result = new NumberExprAST(@lexer.val)
    t1 = @curTok
    @getNextToken() # consume the number
    t2 = @curTok
    return result

  # parenexpr ::= '(' expression ')'
  ParseParenExpr: ->
    @getNextToken()  # eat (.
    v = @ParseExpression()
    if not v then return null
    
    if @curTok != ')'
      throw new Error("expected ')'")
    @getNextToken()  # eat ).
    return v

  # ifexpr ::= 'if' expression 'then' expression 'else' expression
  ParseIfExpr: ->
    @getNextToken()  # eat the if.
    
    # condition.
    Cond = @ParseExpression()
    if not Cond then return null
    
    if @curTok != tok_then
      throw new Error("expected then")
    @getNextToken()  # eat the then
    
    Then = @ParseExpression()
    if not Then then return null
    
    if @curTok != tok_else
      throw new Error("expected else")
    @getNextToken()
    
    Else = @ParseExpression()
    if not Else then return null
    
    return new IfExprAST(Cond, Then, Else)

  # forexpr ::= 'for' identifier '=' expr ',' expr (',' expr)? 'in' expression
  ParseForExpr: ->
    @getNextToken()  # eat the for.

    if @curTok != tok_identifier
      throw new Error("expected identifier after for")
    
    IdName = @lexer.val
    @getNextToken()  # eat identifier.
    
    if @curTok != '='
      throw new Error("expected '=' after for")
    @getNextToken()  # eat '='.
    
    Start = @ParseExpression()
    if not Start then return null

    if @curTok != ','
      throw new Error("expected ',' after for start value")
    @getNextToken()
    
    End = @ParseExpression()
    if not End then return null
    
    # The step value is optional.
    Step = null
    if @curTok == ','
      @getNextToken()
      Step = @ParseExpression()
      if not Step then return 0
    
    if @curTok != tok_in
      throw new Error("expected 'in' after for")
    @getNextToken()  # eat 'in'.
    
    Body = @ParseExpression()
    if not Body then return null

    return new ForExprAST(IdName, Start, End, Step, Body)

  # varexpr ::= 'var' identifier ('=' expression)? 
  #                    (',' identifier ('=' expression)?)* 'in' expression
  ParseVarExpr: ->
    @getNextToken()  # eat the var.

    VarNames = {}

    # At least one variable name is required.
    if @curTok != tok_identifier
      throw new Error("expected identifier after var")
    
    while true
      Name = @lexer.val
      @getNextToken()  # eat identifier.

      # Read the optional initializer.
      Init = null
      if @curTok == '='
        @getNextToken()  # eat the '='.
        
        Init = @ParseExpression()
        if not Init then return null
      
      VarNames[Name] = Init
      
      # End of var list, exit loop.
      if @curTok != ',' then break
      @getNextToken()  # eat the ','.
      
      if @curTok != tok_identifier
        throw new Error("expected identifier list after var")
    
    # At this point, we have to have 'in'.
    if @curTok != tok_in
      throw new Error("expected 'in' keyword after 'var'")
    @getNextToken()  # eat 'in'.
    
    Body = @ParseExpression()
    if not Body then return null
    
    return new VarExprAST(VarNames, Body)

  # primary
  #   ::= identifierexpr
  #   ::= numberexpr
  #   ::= parenexpr
  #   ::= ifexpr
  #   ::= forexpr
  #   ::= varexpr
  ParsePrimary: ->
    switch (@curTok)
      when tok_identifier then @ParseIdentifierExpr()
      when tok_number     then @ParseNumberExpr()
      when '('            then @ParseParenExpr()
      when tok_if         then @ParseIfExpr()
      when tok_for        then @ParseForExpr()
      when tok_var        then @ParseVarExpr()
      else Error("Unknown token #{@curTok.name} when expecting an expression")

  # unary
  #   ::= primary
  #   ::= '!' unary
  ParseUnary: ->
    # If the current token is not an operator, it must be a primary expr.
    if !isascii(@curTok) || @curTok == '(' || @curTok == ','
      return @ParsePrimary()
    
    # If this is a unary operator, read it.
    Opc = @curTok
    @getNextToken()
    if Operand = @ParseUnary()
      return new UnaryExprAST(Opc, Operand)
    else
      null

  # binoprhs
  #   ::= ('+' unary)*
  ParseBinOpRHS: (ExprPrec, LHS) ->
    # If this is a binop, find its precedence.
    while true
      TokPrec = @GetTokPrecedence()

      # If this is a binop that binds at least as tightly as the current binop,
      # consume it, otherwise we are done.
      if TokPrec < ExprPrec
        return LHS
      
      # Okay, we know this is a binop.
      BinOp = @curTok
      @getNextToken()  # eat binop
      
      # Parse the unary expression after the binary operator.
      RHS = @ParseUnary()
      if not RHS then return null
      
      # If BinOp binds less tightly with RHS than the operator after RHS, let
      # the pending operator take RHS as its LHS.
      NextPrec = @GetTokPrecedence()
      if TokPrec < NextPrec
        RHS = @ParseBinOpRHS(TokPrec+1, RHS)
        if not RHS then return null
      
      # Merge LHS/RHS.
      LHS = new BinaryExprAST(BinOp, LHS, RHS)

  # expression
  #   ::= unary binoprhs
  #
  ParseExpression: ->
    LHS = @ParseUnary()
    if not LHS then return null
    return @ParseBinOpRHS(0, LHS)

  # prototype
  #   ::= id '(' id* ')'
  #   ::= binary LETTER number? (id, id)
  #   ::= unary LETTER (id)
  ParsePrototype: ->
    FnName = ''
    
    Kind = 0  # 0 = identifier, 1 = unary, 2 = binary.
    BinaryPrecedence = 30
    
    switch @curTok
      when tok_identifier
        FnName = @lexer.val
        Kind = 0
        @getNextToken()
      when tok_unary
        @getNextToken()
        if !isascii(@curTok)
          throw new Error("Expected unary operator")
        FnName = "unary" + @curTok
        Kind = 1
        @getNextToken()
      when tok_binary
        @getNextToken()
        if !isascii(@curTok)
          return Error("Expected binary operator")
        FnName = "binary" + @curTok
        Kind = 2
        @getNextToken()
        
        # Read the precedence if present.
        if @curTok == tok_number
          if @lexer.val < 1 || @lexer.val > 100
            throw new Error("Invalid precedence: must be 1..100")
          BinaryPrecedence = @lexer.val
          @getNextToken()
      else
        throw new Error("Expected function name in prototype")

    if @curTok != '('
      throw new Error("Expected '(' in prototype")
    
    ArgNames = []
    while @getNextToken() == tok_identifier
      ArgNames.push(@lexer.val)
    if @curTok != ')'
      throw new Error("Expected ')' in prototype")
    
    # success.
    @getNextToken()  # eat ')'.
    
    # Verify right number of names for operator.
    if Kind && ArgNames.length != Kind
      throw new Error("Invalid number of operands for operator")
    
    return new PrototypeAST(FnName, ArgNames, Kind != 0, BinaryPrecedence)

  # definition ::= 'def' prototype expression
  ParseDefinition: ->
    @getNextToken()  # eat def.
    Proto = @ParsePrototype()
    if not Proto then return null

    if (E = @ParseExpression())
      return new FunctionAST(Proto, E)
    return null

  # toplevelexpr ::= expression
  ParseTopLevelExpr: ->
    if (E = @ParseExpression())
      # Make an anonymous proto.
      Proto = new PrototypeAST("", [])
      return new FunctionAST(Proto, E)
    return null

  # external ::= 'extern' prototype
  ParseExtern: ->
    @getNextToken()  # eat extern.
    return @ParsePrototype()

#===----------------------------------------------------------------------===#
# Code Generation
#===----------------------------------------------------------------------===#

TheModule = null
Builder = new llvm.IRBuilder(llvm.globalContext)
NamedValues = {}
TheFPM = null
doubleTy = llvm.globalContext.doubleTy

# CreateEntryBlockAlloca - Create an alloca instruction in the entry block of
# the function.  This is used for mutable variables etc.
CreateEntryBlockAlloca = (TheFunction, VarName) ->
  TmpB = new llvm.IRBuilder(llvm.globalContext)
  TmpB.setInsertPoint(TheFunction.basicBlocks[0])
  TmpB.createAlloca(doubleTy, 0, VarName)

NumberExprAST::Codegen = ->
  return doubleTy.const(@val)

VariableExprAST::Codegen = ->
  # Look this variable up in the function.
  V = NamedValues[@name]
  if not V then throw new Error("Unknown variable name")

  # Load the value.
  return Builder.createLoad(V, @name)

UnaryExprAST::Codegen = ->
  OperandV = @operand.Codegen()
  if not OperandV then return null
  
  F = TheModule.getFunction("unary"+@opcode)
  if not F
    throw new Error("Unknown unary operator")
  
  return Builder.createCall(F, [OperandV], "unop")

BinaryExprAST::Codegen = ->
  # Special case '=' because we don't want to emit the LHS as an expression.
  if @op == '='
    # Assignment requires the LHS to be an identifier.
    if @lhs not instanceof VariableExprAST
      throw new Error("destination of '=' must be a variable")
    # Codegen the RHS.
    Val = @rhs.Codegen()
    if not Val then return null

    # Look up the name.
    Variable = NamedValues[@lhs.name]
    if not Variable then throw new Error("Unknown variable name")

    Builder.createStore(Val, Variable)
    return Val
  
  L = @lhs.Codegen()
  R = @rhs.Codegen()
  if not (L and R) then return null
  
  switch @op
    when '+' then return Builder.createFAdd(L, R, "addtmp")
    when '-' then return Builder.createFSub(L, R, "subtmp")
    when '*' then return Builder.createFMul(L, R, "multmp")
    when '<'
      L = Builder.createFCmpULT(L, R, "cmptmp")
      # Convert bool 0/1 to double 0.0 or 1.0
      return Builder.createUIToFP(L, doubleTy, "booltmp")
  
  # If it wasn't a builtin binary operator, it must be a user defined one. Emit
  # a call to it.
  F = TheModule.getFunction("binary"+@op)
  console.assert(F, "binary operator not found!")
  
  Ops = [ L, R ]
  return Builder.createCall(F, Ops, "binop")

CallExprAST::Codegen = ->
  # Look up the name in the global module table.
  CalleeF = TheModule.getFunction(@callee)
  if not CalleeF
    throw new Error("Unknown function referenced")
  
  # If argument mismatch error.
  if CalleeF.arguments.length != @args.length
    throw new Error("Incorrect # arguments passed")

  ArgsV = []
  for i in @args
    if (l = i.Codegen())
      ArgsV.push(l)
    else
      return null

  return Builder.createCall(CalleeF, ArgsV, "calltmp")

IfExprAST::Codegen = ->
  CondV = @cond.Codegen()
  if not CondV then return null
  
  # Convert condition to a bool by comparing equal to 0.0.
  CondV = Builder.createFCmpONE(CondV, doubleTy.const(0.0), "ifcond")
  
  TheFunction = Builder.insertBlock.parent
  
  # Create blocks for the then and else cases.
  ThenBB = new llvm.BasicBlock(llvm.globalContext, "then",)
  ElseBB = new llvm.BasicBlock(llvm.globalContext, "else")
  MergeBB = new llvm.BasicBlock(llvm.globalContext, "ifcont")
  
  # Insert the 'then' block at the end of the function.
  TheFunction.addBasicBlock(ThenBB)
  
  Builder.createCondBr(CondV, ThenBB, ElseBB)
  
  # Emit then value.
  Builder.setInsertPoint(ThenBB)
  
  ThenV = @then.Codegen()
  if not ThenV then return null
  
  Builder.createBr(MergeBB)
  # Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  ThenBB = Builder.insertBlock
  
  # Emit else block.
  TheFunction.addBasicBlock(ElseBB)
  Builder.setInsertPoint(ElseBB)
  
  ElseV = @else.Codegen()
  if not ElseV then return null
  
  Builder.createBr(MergeBB)
  # Codegen of 'Else' can change the current block, update ElseBB for the PHI.
  ElseBB = Builder.insertBlock
  
  # Emit merge block.
  TheFunction.addBasicBlock(MergeBB)
  Builder.setInsertPoint(MergeBB)
  PN = Builder.createPHI(doubleTy, "iftmp")
  PN.addIncoming(ThenV, ThenBB)
  PN.addIncoming(ElseV, ElseBB)
  return PN

ForExprAST::Codegen = ->
  # Output this as:
  #   var = alloca double
  #   ...
  #   start = startexpr
  #   store start -> var
  #   goto loop
  # loop: 
  #   ...
  #   bodyexpr
  #   ...
  # loopend:
  #   step = stepexpr
  #   endcond = endexpr
  #
  #   curvar = load var
  #   nextvar = curvar + step
  #   store nextvar -> var
  #   br endcond, loop, endloop
  # outloop:
  
  TheFunction = Builder.insertBlock.parent

  # Create an alloca for the variable in the entry block.
  Alloca = CreateEntryBlockAlloca(TheFunction, @varName)
  
  # Emit the start code first, without 'variable' in scope.
  StartVal = @start.Codegen()
  if not StartVal then return null
  
  # Store the value into the alloca.
  Builder.createStore(StartVal, Alloca)
  
  # Make the new basic block for the loop header, inserting after current
  # block.
  LoopBB = TheFunction.addBasicBlock(new llvm.BasicBlock(Context, "loop"))
  
  # Insert an explicit fall through from the current block to the LoopBB.
  Builder.createBr(LoopBB)

  # Start insertion in LoopBB.
  Builder.setInsertPoint(LoopBB)
  
  # Within the loop, the variable is defined equal to the PHI node.  If it
  # shadows an existing variable, we have to restore it, so save it now.
  OldVal = NamedValues[@varName]
  NamedValues[@varName] = Alloca
  
  # Emit the body of the loop.  This, like any other expr, can change the
  # current BB.  Note that we ignore the value computed by the body, but don't
  # allow an error.
  if not @body.Codegen()
    return null
  
  # Emit the step value.
  if @step
    StepVal = @step.Codegen()
    if not StepVal then return null
  else
    # If not specified, use 1.0.
    StepVal = doubleTy.const(1.0)
  
  # Compute the end condition.
  EndCond = @end.Codegen()
  if not EndCond then return null
  
  # Reload, increment, and restore the alloca.  This handles the case where
  # the body of the loop mutates the variable.
  CurVar = Builder.createLoad(Alloca, @varName)
  NextVar = Builder.createFAdd(CurVar, StepVal, "nextvar")
  Builder.createStore(NextVar, Alloca)
  
  # Convert condition to a bool by comparing equal to 0.0.
  EndCond = Builder.createFCmpONE(EndCond, doubleTy.const(0.0), "loopcond")
  
  # Create the "after loop" block and insert it.
  AfterBB = TheFunction.addBasicBlock(new llvm.BasicBlock(Context, "afterloop"))
  
  # Insert the conditional branch into the end of LoopEndBB.
  Builder.createCondBr(EndCond, LoopBB, AfterBB)
  
  # Any new code will be inserted in AfterBB.
  Builder.setInsertPoint(AfterBB)
  
  # Restore the unshadowed variable.
  if OldVal
    NamedValues[@varName] = OldVal
  else
    delete NamedValues[@varName]
  
  # for expr always returns 0.0.
  return doubleTy.const(0.0) #Constant::getNullValue(Type::getDoubleTy(getGlobalContext()))


VarExprAST::Codegen = ->
  OldBindings = []
  
  TheFunction = Builder.insertBlock.parent

  # Register all variables and emit their initializer.
  for VarName, Init of @varNames
    # Emit the initializer before adding the variable to scope, this prevents
    # the initializer from referencing the variable itself, and permits stuff
    # like this:
    #  var a = 1 in
    #    var a = a in ...   # refers to outer 'a'.
    if Init
      InitVal = Init.Codegen()
      if not InitVal then return null
    else # If not specified, use 0.0.
      InitVal = doubleTy.const(0.0)
    
    Alloca = CreateEntryBlockAlloca(TheFunction, VarName)
    Builder.createStore(InitVal, Alloca)

    # Remember the old variable binding so that we can restore the binding when
    # we unrecurse.
    OldBindings.push(NamedValues[VarName])
    
    # Remember this binding.
    NamedValues[VarName] = Alloca
  
  # Codegen the body, now that all vars are in scope.
  BodyVal = @body.Codegen()
  if not BodyVal then return null
  
  # Pop all our variables from scope.
  for vn, i in @varNames
    NamedValues[vn] = OldBindings[i]

  # Return the body computation.
  return BodyVal

PrototypeAST::Codegen = ->
  # Make the function type:  double(double,double) etc.
  Doubles = (doubleTy for i in @args)
  FT = Context.getFunctionType(doubleTy, Doubles, false)
  
  F = TheModule.getOrInsertFunction(@name, FT, llvm.Function.ExternalLinkage)
  
  # If F already has a body, reject this.
  if F.basicBlocks.length
    throw new Error("Redefinition of function")

  # If F took a different number of args, reject.
  if F.arguments.length != @args.length
    throw new Error("Redefinition of function with different # args")
  
  # Set names for all arguments.
  for AI, i in F.arguments
    AI.name = @args[i]

  return F

# CreateArgumentAllocas - Create an alloca for each argument and register the
# argument in the symbol table so that references to it will succeed.
PrototypeAST::CreateArgumentAllocas = (F) ->
  for AI, Idx in F.arguments
    # Create an alloca for this variable.
    Alloca = CreateEntryBlockAlloca(F, @args[Idx])

    # Store the initial value into the alloca.
    Builder.createStore(AI, Alloca)

    # Add arguments to variable symbol table.
    NamedValues[@args[Idx]] = Alloca

FunctionAST::Codegen = ->
  NamedValues = {}
  
  TheFunction = @Proto.Codegen()
  if not TheFunction then return null
  
  # If this is an operator, install it.
  if @Proto.isBinaryOp()
    parser.binopPrecedence[@Proto.getOperatorName()] = @Proto.precedence
  
  # Create a new basic block to start insertion into.
  BB = TheFunction.addBasicBlock("entry")
  Builder.setInsertPoint(BB)
  
  # Add all arguments to the symbol table and create their allocas.
  @Proto.CreateArgumentAllocas(TheFunction)

  if (RetVal = @body.Codegen())
    # Finish off the function.
    Builder.createRet(RetVal)

    # Validate the generated code, checking for consistency.
    #verifyFunction(TheFunction)

    # Optimize the function.
    TheFPM.run(TheFunction)
    
    return TheFunction
  
  # Error reading body, remove function.
  TheFunction.eraseFromParent()

  if Proto.isBinaryOp()
    BinopPrecedence.erase(Proto.getOperatorName())
  return null

#===----------------------------------------------------------------------===#
# Top-Level parsing and JIT Driver
#===----------------------------------------------------------------------===#

TheExecutionEngine = null

HandleDefinition = ->
  if (F = parser.ParseDefinition())
    if (LF = F.Codegen())
      console.log "Read function definition:"
      console.log LF.dump()
  else
    # Skip token for error recovery.
    getNextToken()

HandleExtern = ->
  if (P = parser.ParseExtern())
    if (F = P.Codegen())
      console.log "Read extern:"
      console.log F.dump()
  else 
    # Skip token for error recovery.
    getNextToken()

HandleTopLevelExpression = ->
  # Evaluate a top-level expression into an anonymous function.
  if (F = parser.ParseTopLevelExpr())
    console.log "parse", F.body.toString()
    if (LF = F.Codegen())
      # JIT the function, returning a function pointer.
      FP = TheExecutionEngine.getFFIFunction(LF)
      console.log "Evaluated to", FP()
  else 
    # Skip token for error recovery.
    getNextToken()

# top ::= definition | external | expression | ';'
# Returns true if the next line is a continuation
MainExpr = ->
  switch parser.curTok
    when tok_eof then return
    when ';' then return# ignore top-level semicolons.
    when tok_def then HandleDefinition()
    when tok_extern then HandleExtern()
    else HandleTopLevelExpression()

runLine = (done) ->
  rl.prompt()
  rl.question 'ready> ', (line) ->
    parser.lexer.feed(line)
    parser.getNextToken()
    MainExpr()
    runLine(done)

#===----------------------------------------------------------------------===#
# "Library" functions that can be "extern'd" from user code.
#===----------------------------------------------------------------------===#

"""
# putchard - putchar that takes a double and returns 0.
extern "C" 
double putchard(double X) {
  putchar((char)X)
  return 0
}

# printd - printf that takes a double prints it as "%f\n", returning 0.
extern "C" 
double printd(double X) {
  printf("%f\n", X)
  return 0
}
"""

#===----------------------------------------------------------------------===#
# Main driver code.
#===----------------------------------------------------------------------===#

#InitializeNativeTarget()
Context = llvm.globalContext

# Install standard binary operators.
# 1 is lowest precedence.
parser = new Parser()
parser.binopPrecedence['='] = 2
parser.binopPrecedence['<'] = 10
parser.binopPrecedence['+'] = 20
parser.binopPrecedence['-'] = 20
parser.binopPrecedence['*'] = 40  # highest.

rl = readline.createInterface
  input: process.stdin
  output: process.stdout

# Make the module, which holds all the code.
TheModule = new llvm.Module("my cool jit", Context)

# Create the JIT. 
TheExecutionEngine = new llvm.ExecutionEngine(TheModule)
OurFPM = new llvm.FunctionPassManager(TheModule)

# Set up the optimizer pipeline.  Start with registering info about how the
# target lays out data structures.
OurFPM.addDataLayoutPass('') #TheExecutionEngine.dataLayout)
# Provide basic AliasAnalysis support for GVN.
OurFPM.addBasicAliasAnalysisPass()
# Promote allocas to registers.
#OurFPM.addPromoteMemoryToRegisterPass()
# Do simple "peephole" optimizations and bit-twiddling optzns.
OurFPM.addInstructionCombiningPass()
# Reassociate expressions.
OurFPM.addReassociatePass()
# Eliminate Common SubExpressions.
OurFPM.addGVNPass()
# Simplify the control flow graph (deleting unreachable blocks, etc).
#OurFPM.addCFGSimplificationPass()

OurFPM.doInitialization()

# Set the global so the code gen can use this.
TheFPM = OurFPM

# Run the main "interpreter loop" now.
runLine ->
  TheFPM = null

  # Print out all of the generated code.
  console.log TheModule.dump()

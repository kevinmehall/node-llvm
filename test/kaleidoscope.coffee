{Parser} = require '../examples/kaleidoscope'
assert = require('assert')

describe 'Kaleidoscope', ->
	parser = null
	before ->
		parser = new Parser(true)

	it 'does simple arithmetic', ->
		assert.equal parser.runLine("3+5*10"), 53

	it 'calls externs', ->
		parser.runLine("extern sin(arg);")
		assert.equal parser.runLine("sin(0)"), 0

	it 'compiles and runs fibonacci', ->
		parser.runLine """
			def fib(x)
			  if x < 3 then
			    1
			  else
			    fib(x-1)+fib(x-2)
		"""
		assert.equal parser.runLine("fib(10)"), 55
		assert.equal parser.runLine("fib(18)"), 2584

	it 'defines operators', ->
		parser.runLine "def binary : 1 (x y) y;"
		assert.equal parser.runLine("(3:4)"), 4

	it 'does fibonacci iteratively', ->
		parser.runLine """
		def fibi(x)
			  var a = 1, b = 1, c in
			  (for i = 3, i < x in
			     c = a + b :
			     a = b :
			     b = c) :
			  b;
		"""
		assert.equal parser.runLine("fibi(10)"), 55
		assert.equal parser.runLine("fibi(18)"), 2584

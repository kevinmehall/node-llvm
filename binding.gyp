{
  'variables': {
    "llvm_config": "llvm-config-3.0",
  },
  'targets': [  
    {
      'target_name': 'llvm',
      'sources': [ 
        './src/llvm.cc',
        './src/basicblock.cc',
        './src/context.cc',
        './src/irbuilder.cc',
        './src/module.cc',
        './src/value.cc',
        './src/function.cc',
        './src/type.cc',
        './src/functiontype.cc',
        './src/executionengine.cc',
        './src/functionpassmanager.cc',
        './src/instructions.cc',
      ],
      'cflags': [
        '-O3',
        '-Wall',
        '-Werror',
      ],
      'cflags_cc': [
        '-O3',
        '-Wall',
        '-Werror',
        '-std=c++0x',
        '-g',
      ],      
      'defines': [
        "__STDC_LIMIT_MACROS=1",
        "__STDC_CONSTANT_MACROS=1"
      ],
      'include_dirs+': [
        "src/",
        "<!@(<(llvm_config) --includedir)"
      ],
      "libraries": [
        "<!@(<(llvm_config) --ldflags)",
        "<!@(<(llvm_config) --libs core jit native)"
      ]
    }
  ]
}

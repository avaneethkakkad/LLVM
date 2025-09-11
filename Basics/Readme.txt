Setup used for this 

llvm-config --version 14.0.0

clang version 
Ubuntu clang version 14.0.0-1ubuntu1.1
Target: x86_64-pc-linux-gnu
Thread model: posix
InstalledDir: /usr/bin

opt version 
Ubuntu LLVM version 14.0.0
  
Optimized build.
Default target: x86_64-pc-linux-gnu
Host CPU: skylake

Definations : 

LLVM IR is a higher-level abstraction suitable for cross platform code generation and optimization .
object files are lower level, tied to specific architectures.

Hello world of LLVM 
1.clang -S -emit-llvm hello_world.cpp -o hello_world.ll
2.Verify functional correctness using lli (LLVM interpreter for execution )
3.llvm-dis to convert bytecode to human readable format 

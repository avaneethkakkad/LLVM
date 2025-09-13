steps
1.mkdir build && build 
2.cmake -DLLVM_DIR=$(llvm-config --cmakedir) ..
3. make 
4. opt -load-pass-plugin ./libConstantFolding.so -passes="const-fold-binop" ../hello.ll -S -o folded.ll


for git push : git remote set-url origin git@github.com:avaneethkakkad/LLVM.git


#### cs565-project1
#### USC CSCI565 - Spring 2014
#### Lyle Franklin

Description:
================
This project contains a couple LLVM pass to get familiar with the LLVM infrastructure.
The first pass pretty prints out the bytecode, while the second removes any trivially unreachable blocks from the bytecode.

Directory Structure:
================
+ build (llvm build directory)
| + Debug+Asserts
| | + bin (llvm and clang executables)
+ llvm (llvm source root)
| + cs565
| | + lib (source files for passes)
| | + tests (test files for passes)

Install Instructions:
================
######install subversion if not already installed
sudo apt-get install subversion

######checkout llvm
svn co http://llvm.org/svn/llvm-project/llvm/trunk llvm

######checkout clang
cd llvm/tools
svn co http://llvm.org/svn/llvm-project/cfe/trunk clang
cd ../..

######checkout clang extra (might not be necessary)
cd llvm/tools/clang/tools
svn co http://llvm.org/svn/llvm-project/clang-tools-extra/trunk extra
cd ../../../..

######checkout Compiler-RT
cd llvm/projects
svn co http://llvm.org/svn/llvm-project/compiler-rt/trunk compiler-rt
cd ../..

######build everything
mkdir build
cd build
../llvm/configure
make //takes a long time, 1-2 hours
cd ..

######build custom passes
cd llvm/cs565/tests
make
cd ../lib
make

######run prettyPrint on test1
make testPrint

######run prettyPrint on test2
make testPrint2

######run prettyPrint on test3
make testPrint3

######run naive unreachable block removal pass on test2
make testNaive

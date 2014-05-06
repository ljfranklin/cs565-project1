# Makefile for hello pass

# Path to top level of LLVM hierarchy
LEVEL = ../..

# Name of the library to build
LIBRARYNAME = cs565opt

# Make the shared library become a loadable module so the tools can
# dlopen/dlsym on the resulting library.
LOADABLE_MODULE = 1

# Might need to change this to .so in LINUX
# LIB_EXT = .dylib
LIB_EXT = .so

TEST_PRINT = $(LEVEL)/../build/Debug+Asserts/bin/opt -load $(LEVEL)/Debug+Asserts/lib/$(LIBRARYNAME)$(LIB_EXT) -prettyPrint <
TEST_NAIVE = $(LEVEL)/../build/Debug+Asserts/bin/opt -load $(LEVEL)/Debug+Asserts/lib/$(LIBRARYNAME)$(LIB_EXT) -cfgNaive <
TEST_LIVE = $(LEVEL)/../build/Debug+Asserts/bin/opt -load $(LEVEL)/Debug+Asserts/lib/$(LIBRARYNAME)$(LIB_EXT) -instnamer -liveVariables <

# Include the makefile implementation stuff
include $(LEVEL)/Makefile.common

testPrint:
	$(TEST_PRINT) ../tests/test1.bc > /dev/null

testPrint2:
	$(TEST_PRINT) ../tests/test2.bc > /dev/null

testPrint3:
	$(TEST_PRINT) ../tests/test3.bc > /dev/null	

testNaive:
	$(TEST_PRINT) ../tests/test2.bc > /dev/null 
	$(TEST_NAIVE) ../tests/test2.bc > ../tests/test2.opt.bc 
	$(TEST_PRINT) ../tests/test2.opt.bc > /dev/null 
	
testLive:
	$(TEST_LIVE) ../tests/test4.bc > /dev/null

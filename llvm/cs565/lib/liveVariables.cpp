#include "passes.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include <map>
#include <set>

namespace cs565 {
	bool LiveVariables::runOnFunction(Function &F) {
		
		errs() << F.getName() << "\n";
    
    	//build USE/DEF sets for each BB
    	std::map<BasicBlock*, std::set<std::string>> useMap;
    	std::map<BasicBlock*, std::set<std::string>> defMap;
    	for (Function::iterator currentBlock = F.begin(), endBlock = F.end(); currentBlock != endBlock; currentBlock++) {
    			
    		errs() << "\nBASIC BLOCK " << currentBlock->getName() << "\n";
    			
    		for (BasicBlock::reverse_iterator inst = currentBlock->rbegin(), instEnd = currentBlock->rend(); inst != instEnd; inst++) {
				errs() << *inst << "\n";
    		}
    	}
    
    	/*
		for (Function::iterator currentBlock = F.begin(), endBlock = F.end(); currentBlock != endBlock; currentBlock++) {
			
			errs() << "\nBASIC BLOCK " << currentBlock->getName() << "\n";
			
			for (succ_iterator iterator = succ_begin(currentBlock), endSucc = succ_end(currentBlock); iterator != endSucc; iterator++) {
			  	BasicBlock *succ = *iterator;
			  	
			  	errs() << "\nSUCCESSORS " << succ->getName() << "\n";
			}
			
		}
		*/
		
		//Function does not modify anything
		return false;
	}
	
	void LiveVariables::getAnalysisUsage(AnalysisUsage &Info) const {
		//This pass does no modifications
		Info.setPreservesAll();
	}
}

char cs565::LiveVariables::ID = 1;
static RegisterPass<cs565::LiveVariables> X("liveVariables", "(CS 565) - List live variables at the end of each Basic Block", false, false);

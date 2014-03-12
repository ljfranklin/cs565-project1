#include "passes.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"

namespace cs565 {
	bool CfgNaive::runOnFunction(Function &F) {
		
		errs() << F.getName() << "\n";

		SmallPtrSet<BasicBlock*, 8> visitedBlocks;

  		// Mark all reachable blocks.
  		for (df_ext_iterator<Function*, SmallPtrSet<BasicBlock*, 8>> currentBlock = df_ext_begin(&F, visitedBlocks), 
			endBlock = df_ext_end(&F, visitedBlocks); 
			currentBlock != endBlock; 
			currentBlock++) {
			//do nothing, iterator marks visited nodes automatically			
		}
    
		// Build set of unreachable blocks
		std::vector<BasicBlock*> unreachableBlocks;
		for (Function::iterator currentBlock = F.begin(), endBlock = F.end(); currentBlock != endBlock; currentBlock++) {
			if (visitedBlocks.count(currentBlock) == 0) {
				unreachableBlocks.push_back(currentBlock);
			}
		}

		// Remove unreachable blocks
		for (int i = 0, e = unreachableBlocks.size(); i != e; i++) {
			errs() << unreachableBlocks[i]->getName() << " is unreachable\n";
    			unreachableBlocks[i]->eraseFromParent();
  		}

		bool hasModifiedBlocks = (unreachableBlocks.size() > 0);
		return hasModifiedBlocks;
	}
	
	void CfgNaive::getAnalysisUsage(AnalysisUsage &Info) const {
		//This pass modifies the CFG
	}
}

char cs565::CfgNaive::ID = 0;
static RegisterPass<cs565::CfgNaive> X("cfgNaive", "(CS 565) - Naive Unreachable Basic Blocks Removal LLVM IR", false, false);

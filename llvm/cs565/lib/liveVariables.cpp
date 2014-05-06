#include "passes.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/SmallPtrSet.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/CFG.h"
#include <map>
#include <set>
#include <algorithm>

namespace cs565 {
	bool LiveVariables::runOnFunction(Function &F) {
		
		errs() << F.getName() << "\n";
    
    	//build USE + DEF sets for each BB
    	std::map<BasicBlock*, std::set<std::string>> useMap;
    	std::map<BasicBlock*, std::set<std::string>> defMap;
    	for (Function::iterator currentBlock = F.begin(), endBlock = F.end(); currentBlock != endBlock; currentBlock++) {
    		
    		std::set<std::string> use;
    		std::set<std::string> def;	
    		
    		for (BasicBlock::reverse_iterator inst = currentBlock->rbegin(), instEnd = currentBlock->rend(); inst != instEnd; inst++) {
				
				if (inst->hasName()) {
					def.insert(inst->getName());
					use.erase(inst->getName());
				}
				
				for(Instruction::op_iterator op = inst->op_begin(), ope = inst->op_end(); op != ope; op++) {
					
					if (dyn_cast<Instruction>(*op) || dyn_cast<Argument>(*op)) {
						Value* define = op->get();
						use.insert(define->getName());
					}
				}
    		}
    		
    		useMap[currentBlock] = use;
    		defMap[currentBlock] = def;
    	}
    
    	//Build IN + OUT sets
    	std::map<BasicBlock*, std::set<std::string>> inMap;
    	std::map<BasicBlock*, std::set<std::string>> outMap;
    	
    	bool isChanged = true;
    	
    	while (isChanged) {
    		isChanged = false;
    		for (Function::iterator currentBlock = F.begin(), endBlock = F.end(); currentBlock != endBlock; currentBlock++) {
			
				int originalInCount = inMap[currentBlock].size();
				int originalOutCount = outMap[currentBlock].size();
			
				for (succ_iterator iterator = succ_begin(currentBlock), endSucc = succ_end(currentBlock); iterator != endSucc; iterator++) {
				  	BasicBlock *succ = *iterator;
				  	
				  	std::set<std::string> succIn = inMap[succ];
				  	outMap[currentBlock].insert(succIn.begin(), succIn.end());
				}
			
				std::set<std::string> inSet;
			
				std::set<std::string> diff;
				std::set_difference(outMap[currentBlock].begin(), outMap[currentBlock].end(), 
					defMap[currentBlock].begin(), defMap[currentBlock].end(), 
					std::inserter(diff, diff.end()));
			
				inSet.insert(useMap[currentBlock].begin(), useMap[currentBlock].end());
				inSet.insert(diff.begin(), diff.end());
			
				inMap[currentBlock] = inSet;
				
				int newInCount = inMap[currentBlock].size();
				int newOutCount = outMap[currentBlock].size();
				
				if (newInCount != originalInCount || newOutCount != originalOutCount) {
					isChanged = true;
				}
			}
    	}
    	
		
		
		//Print sets
    	for (Function::iterator currentBlock = F.begin(), endBlock = F.end(); currentBlock != endBlock; currentBlock++) {
    			
    		errs() << "\nBASIC BLOCK " << currentBlock->getName() << "\nInstructions:\n";
    		
    		for (BasicBlock::iterator inst = currentBlock->begin(), instEnd = currentBlock->end(); inst != instEnd; inst++) {
				errs() << *inst << "\n";
    		}
    		
    		/*
    		errs() << " USE \n";
    		for (std::set<std::string>::iterator use = useMap[currentBlock].begin(), useEnd = useMap[currentBlock].end(); use != useEnd; use++) {
				errs() << "\t" << *use << "\n";
    		}
    		
    		errs() << " DEF \n";
    		for (std::set<std::string>::iterator def = defMap[currentBlock].begin(), defEnd = defMap[currentBlock].end(); def != defEnd; def++) {
				errs() << "\t" << *def << "\n";
    		}
    		*/
    		
    		errs() << "IN:\n";
    		for (std::set<std::string>::iterator o = inMap[currentBlock].begin(), oEnd = inMap[currentBlock].end(); o != oEnd; o++) {
				errs() << "  " << *o << "\n";
    		}
    		if (inMap[currentBlock].size() == 0) {
    			errs() << "  None\n";
    		}
    		
    		errs() << "OUT:\n";
    		for (std::set<std::string>::iterator o = outMap[currentBlock].begin(), oEnd = outMap[currentBlock].end(); o != oEnd; o++) {
				errs() << "  " << *o << "\n";
    		}
    		if (outMap[currentBlock].size() == 0) {
    			errs() << "  None\n";
    		}
    	}
		
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

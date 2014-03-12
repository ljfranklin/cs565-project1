#include "passes.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include <map>

namespace cs565 {
	bool PrettyPrint::runOnFunction(Function &F) {
		errs() << "FUNCTION ";
		errs().write_escaped(F.getName()) << "\n";
		
		int instructionCounter = 0;

		//Maps variable declaration pointers to the declaration's instruction number
		std::map<Value*, int> varMap; 

		for (Function::iterator blk = F.begin(), blkEnd = F.end(); blk != blkEnd; ++blk) {
		
  			errs() << "\nBASIC BLOCK " << blk->getName() << "\n";

			for (BasicBlock::iterator inst = blk->begin(), instEnd = blk->end(); inst != instEnd; ++inst) {

				//print instruction number
				errs() << "%" << ++instructionCounter << ":\t" << inst->getOpcodeName();

				//This Inst is a named operand or an unnamed load operand, add to map				
				if (inst->hasName() || dyn_cast<llvm::LoadInst>(inst)) {
					varMap[inst] = instructionCounter;
				}

				for (User::op_iterator i = inst->op_begin(), e = inst->op_end(); i != e; ++i) {
  					Value *v = *i;
					
					//Value is referencing a previously declared variable, print the line number
					if (varMap.count(v)) {
						errs() << " %" << varMap[v];
					} 
					//Value is a label, print its name
					else if (v->hasName()) {
						errs() << " " << v->getName();
					} 
					//Value is a constant, print its value
					else if (ConstantInt* constInt = dyn_cast<llvm::ConstantInt>(v)) {
						errs() << " " << constInt->getValue();
					} 
					//Value is something else, print placeholder
					else {
						errs() << " XXX";
					}
				}

				errs() << "\n";
			}
		}

		errs() << "\n";

		//Function does not modify anything
		return false;
	}
	
	void PrettyPrint::getAnalysisUsage(AnalysisUsage &Info) const {
		//This pass does no modifications
		Info.setPreservesAll();
	}
}

char cs565::PrettyPrint::ID = 0;
static RegisterPass<cs565::PrettyPrint> X("prettyPrint", "(CS 565) - Pretty Print LLVM IR", false, false);

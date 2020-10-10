#include "Slice.h"
#include "CFSlice.h"
#include <vector>
#include <sstream>
#include <string>
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Constants.h"
#include "llvm/ADT/StringExtras.h"

using namespace llvm;

  //----------------------------------------------------------------------------------------------------
  // labelBB: Takes a Function F and labels every BasicBlock 
  //----------------------------------------------------------------------------------------------------
  void labelBB(Function *F)
  {
	for(Function::iterator i=F->begin(), fend=F->end(); i!=fend; ++i)
	{
		BasicBlock *blk = &*i;
		blk->setName("BB"); //it automatically enumerates name
	}
  }


  //----------------------------------------------------------------------------------------------------
  // functionInstructionCount: Takes a Function F and returns the total number of instructions 
  //----------------------------------------------------------------------------------------------------
  unsigned functionInstructionCount(Function *F)
  {
	unsigned inst_count = 0;
	for(Function::iterator i=F->begin(), iend=F->end(); i!=iend; ++i)
	{
		BasicBlock *blk = &*i;
		for(BasicBlock::iterator j=blk->begin(),jend=blk->end(); j!=jend; ++j)
		{
			inst_count++;
		} 
	}
	return inst_count;
  }

  //----------------------------------------------------------------------------------------------------
  // buildControlReplicant: Takes a Function F and constructs a control slice where every BasicBlock
  // terminator is a slicing criterion. 
  //----------------------------------------------------------------------------------------------------
  CFSlice * buildControlReplicant(Function *F)
  {
	CFSlice *cReplicant = new CFSlice(F);
	for(Function::iterator i=F->begin(), iend=F->end(); i!=iend; ++i)
	{
		BasicBlock *blk = &*i;
		TerminatorInst *t = blk->getTerminator();
		cReplicant->addCriterion(t);
	}
	return cReplicant;
  } 


  //----------------------------------------------------------------------------------------------------
  // replicateFunction: Takes a Function F and clones it, adding it to the current module 
  //----------------------------------------------------------------------------------------------------
Function * replicateFunction(Function *F, std::string postfix)
  {
	ValueToValueMapTy Vmap;
	Function *newFunc = CloneFunction(F, Vmap, /*true,*/ NULL); 
	std::string suffixed_name = F->getName().str() + postfix;
	newFunc->setName(suffixed_name);
	
	//Inserting into the original Module
	F->getParent()->getFunctionList().push_back(newFunc);
	return newFunc;
  }


  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  // Helper functions taken from the LegUp: Target/Verilog/utils.cpp
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	void setMetadataStr(Instruction *I, std::string kind, std::string value) {
	    I->setMetadata(I->getContext().getMDKindID(kind),
	                   MDNode::get(I->getContext(),
	                               MDString::get(I->getContext(),
	                                             value)));
	}
	
	void setMetadataInt(Instruction *I, std::string kind, int value) {
	    setMetadataStr(I, kind, utostr(value));
	}
  //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~





  //----------------------------------------------------------------------------------------------------
  // callReplicantFunction
  // When given a replicate function and an original function will call the replicate function as the
  // the first instruction in the original functions body.
  //----------------------------------------------------------------------------------------------------
  void callReplicantFunction(Function *orig, Function *replicant)
  {

	Module *M = orig->getParent(); //Get the module that we are working with	
	unsigned instanceCount = 0;	
	std::vector<CallInst *>pslice_calls;
	//Find all locations where the original function is called
	for(Module::iterator mi=M->begin(), mend=M->end(); mi!=mend; ++mi) {
		Function *f = &*mi;
		for(Function::iterator fi=f->begin(), fend=f->end(); fi!=fend; ++fi) {
			BasicBlock *blk = &*fi;
			for(BasicBlock::iterator bi=blk->begin(), bend=blk->end(); bi!=bend; ++bi) {
				Instruction *inst = &*bi;
				if(CallInst *cinst = dyn_cast<CallInst>(inst)) { //We have found a call instruction
					Function *calledF = cinst->getCalledFunction();
					if(calledF == orig) { //We have found a call to our original function!
						BasicBlock *callParentBB = cinst->getParent(); //parent BasicBlock of the called instruction
						Function *callParentFunc = callParentBB->getParent(); //parent function of the called instruction
						Module *callParentMod = callParentFunc->getParent(); //Parent Module of the called instruction
						errs() << "Found a call to: " << calledF->getName() << " in " << callParentFunc->getName() <<"\n";

						//Here we need to insert a call to the pslice above the original
						Instruction *cinst_clone = cinst->clone();
						CallInst *pslice_call = dyn_cast<CallInst>(cinst_clone);
						pslice_call->setCalledFunction(replicant);
						callParentBB->getInstList().insert(/*cinst*/(BasicBlock::iterator) cinst,pslice_call); //insert pslice_call before cinst
						pslice_calls.push_back(pslice_call);

	
						//We need to include a store instruction above each function call with the threadId as the operand
						LLVMContext& ctx = callParentBB->getContext();

						//for the original function call
						ConstantInt *orig_threadID = ConstantInt::get(ctx, APInt(32, StringRef("1"), 10));
						AllocaInst *orig_call_threadID = new AllocaInst(IntegerType::get(ctx, 32), f->getParent()->getDataLayout().getAllocaAddrSpace(), NULL, orig->getName().str() + "_orig_threadID", &f->getEntryBlock().front());
						StoreInst *orig_thread_store = new StoreInst(orig_threadID, orig_call_threadID, false);
						callParentBB->getInstList().insert(/*cinst*/(BasicBlock::iterator) cinst, orig_call_threadID);
						callParentBB->getInstList().insert(/*cinst*/(BasicBlock::iterator) cinst, orig_thread_store);
						//For the pSlice function call

						ConstantInt *pslice_threadID = ConstantInt::get(ctx, APInt(32, StringRef("0"), 10));
						AllocaInst *pslice_call_threadID = new AllocaInst(IntegerType::get(ctx, 32), f->getParent()->getDataLayout().getAllocaAddrSpace(), NULL, orig->getName().str() + "_pslice_threadID", &f->getEntryBlock().front());
						StoreInst *pslice_thread_store = new StoreInst(pslice_threadID, pslice_call_threadID, false);
						callParentBB->getInstList().insert((BasicBlock::iterator) pslice_call, pslice_call_threadID);
						callParentBB->getInstList().insert((BasicBlock::iterator) pslice_call, pslice_thread_store);

						static LLVMContext TheContext;

						std::string wrapperName = "legup_pthreadpoll";
						std::vector<Value *> newParam;
						ConstantInt *pslice_poll_threadID = ConstantInt::get(ctx, APInt(32, StringRef("0"), 10));
						newParam.push_back(pslice_poll_threadID);
						std::vector<Type *> newParamType;
						newParamType.push_back(pslice_poll_threadID->getType());
						// create the function prototype for the wrapper
						FunctionType *FTy = FunctionType::get(
						    Type::getInt8PtrTy(TheContext, 0), newParamType, false);
						Constant *wrapperF = callParentMod->getOrInsertFunction(wrapperName, FTy);
						cast<Function>(wrapperF)->setLinkage(GlobalValue::ExternalLinkage);
						// create the call instruction to pthread join
						CallInst *pSlice_poll = CallInst::Create(wrapperF,newParam, "", callParentBB->getTerminator());
						setMetadataStr(pSlice_poll, "TYPE", "legup_wrapper_pthreadpoll");
						setMetadataInt(pSlice_poll, "NUMTHREADS", 1);
					
						replicant->setDoesNotReturn();	
							
			
						//These store instructions also require metadata
						setMetadataInt(orig_thread_store, "legup_pthread_functionthreadID", 1);
						setMetadataInt(orig_thread_store, "legup_pthread_wrapper_inst", 0);
						setMetadataInt(pslice_thread_store, "legup_pthread_functionthreadID", 1);
						setMetadataInt(pslice_thread_store, "legup_pthread_wrapper_inst", 1);


						//We need to add some Metadata so to call this guy in parallel
						setMetadataStr(pslice_call, "PTHREADNAME", replicant->getName());
						//setMetadataInt(pslice_call, "NUMTHREADS", instanceCount+1);
						setMetadataInt(pslice_call, "THREADID", instanceCount);
						setMetadataInt(pslice_call, "FUNCTIONID", 0);
						setMetadataStr(pslice_call, "TYPE", "legup_wrapper_pthreadcall");

						//setMetadataStr(cinst, "PTHREADNAME", orig->getName());
						//setMetadataInt(cinst, "NUMTHREADS", 2);
						//setMetadataInt(cinst, "THREADID", 1);
						//setMetadataInt(cinst, "FUNCTIONID", 1);
						//setMetadataStr(cinst, "TYPE", "legup_wrapper_pthreadcall");
						instanceCount++;	
					}
				}
			}
		}	
	}
	for(std::vector<CallInst *>::iterator psi=pslice_calls.begin(), psend=pslice_calls.end(); psi!=psend; ++psi) {
		CallInst *p = *psi;
		setMetadataInt(p, "NUMTHREADS", instanceCount);
	}
  }
  

  //----------------------------------------------------------------------------------------------------
  // functionSanityCheck: Takes a Function F and ensures it is compilable. This is typically performed
  // after the removeSlice function call 
  //----------------------------------------------------------------------------------------------------
  void functionSanityCheck(Function *F)
  {
	for(Function::iterator i=F->begin(), iend=F->end(); i!=iend; ++i) {
		BasicBlock *blk = &*i;
		for(BasicBlock::iterator j=blk->begin(), jend=blk->end(); j!=jend; ++j) {
			Instruction *inst = &*j;
			errs() << *inst <<"\n";
		}
	}
  } 

  //----------------------------------------------------------------------------------------------------
  // getReturns
  // gets all return instructions for the function
  //----------------------------------------------------------------------------------------------------
  std::vector<ReturnInst *>* getReturns(Function *F)
  {
	std::vector<ReturnInst *>* r = new std::vector<ReturnInst *>;
	for(Function::iterator fi=F->begin(), fend=F->end(); fi!=fend; ++fi) {
		BasicBlock *blk = &*fi;
		TerminatorInst *t = blk->getTerminator();
		if(ReturnInst *rt = dyn_cast<ReturnInst>(t)) { 
			r->push_back(rt);
			errs() << "Found a return: \n";
		}
	}
	return r;
  }

  //----------------------------------------------------------------------------------------------------
  // relocateReturns:
  // move all function returns up to the last unpruned point so that pslice can return after opt
  //----------------------------------------------------------------------------------------------------
  void relocateReturns(Function *F, Slice *p)
  {
	//create a vector of all return instructions
	std::vector<ReturnInst *>* returns = getReturns(F); 
	for(std::vector<ReturnInst *>::iterator ri=returns->begin(), rend=returns->end(); ri!=rend; ++ri) {
		ReturnInst *r = *ri;
		BasicBlock *blk = r->getParent();	
		if(p->usesBB(blk)) 
			errs() << "\t\tThe return is included in the slice already!\n";
		else
			errs() << "\t\tThe return has been removed from the slice!\n";
	}
		
  }
  
  //----------------------------------------------------------------------------------------------------
  // pruneUnusedBlocks: 
  // prunes all BasicBlocks in F that are not required to compute Slice p
  // This is not a complete implementation yet and only takes into account simple cases where BBs can be
  // skipped.
  //----------------------------------------------------------------------------------------------------
  void pruneUnusedBlocks(Function *F, Slice *p)
  {
	BasicBlock *entryBlk = &F->getEntryBlock();
	for(Function::iterator fi=F->begin(), fe=F->end(); fi!=fe; ++fi) {
		BasicBlock *blk = &*fi;
		if(!p->usesBB(blk)) {
			if(entryBlk != blk) {
				if(BasicBlock *pred = blk->getSinglePredecessor()) {
					if(BranchInst *blk_br = dyn_cast<BranchInst>(blk->getTerminator()) ) {
						if(blk_br->getNumSuccessors() == 1) {
							BasicBlock *succ = blk_br->getSuccessor(0);
							if(BranchInst* pred_br = dyn_cast<BranchInst>(pred->getTerminator())) {
								unsigned pred_succ_idx = 0;
								for(unsigned i=0; i < pred_br->getNumSuccessors(); i++) {
									BasicBlock *pred_succ = pred_br->getSuccessor(i);
									if(pred_succ == blk) {
										pred_succ_idx = i;
									}
								}
								succ->removePredecessor(blk);
								pred_br->setSuccessor(pred_succ_idx, succ);
								blk->replaceSuccessorsPhiUsesWith(pred);
	
							}
						}
					}
				}
				std::string s = blk->getName().str();
			}
		}
	}
	relocateReturns(F,p);
  }


  //----------------------------------------------------------------------------------------------------
  // removeSlice: Takes a Slice p and a Function F and removes all instructions from F that are
  // present in p, leaving an executable inverse of the p slice behind.
  // WARNING: after executing this function F will be altered. 
  //----------------------------------------------------------------------------------------------------
  void removeSlice(Function *F, Slice *p)
  {
 	for(Slice::iterator i=p->begin(), ie=p->end(); i!=ie; ++i) {
		Value *curr = *i;
		Value *blank = UndefValue::get(curr->getType());
		if(Instruction *inst = dyn_cast<Instruction>(curr))
		{
			if(!isa<TerminatorInst>(inst)) {
				bool remove = true;
				//Special case for printf
				if(CallInst *cInst = dyn_cast<CallInst>(inst)) {
					Function *tmp_func = cInst->getCalledFunction();
					if(tmp_func->getName() == "printf") 
						remove = false;
				}
					
				if(remove) {	
					errs() << "\t\tRemoving:   " << *inst <<"\n";
					inst->replaceAllUsesWith(blank);
					inst->eraseFromParent();
				}
			}
		}
	} 
  
  }


  //----------------------------------------------------------------------------------------------------
  // nameAllLoadOps: names every load instruction in given function
  //----------------------------------------------------------------------------------------------------
  void nameAllLoadOps(Function *F)
  {
	for(Function::iterator fi=F->begin(), fend=F->end(); fi!=fend; ++fi) {
		BasicBlock *blk = &*fi;
		for(BasicBlock::iterator bi=blk->begin(), bend=blk->end(); bi!=bend; ++bi) { 
			Instruction *inst = &*bi;
			if(isa<LoadInst>(inst)) {
				inst->setName("LD");
			}
		}
	}
  }

  //----------------------------------------------------------------------------------------------------
  // printAllLoadOps
  // instantiates a call to printf for every load operation found in the given function
  // This is useful when simulating to determine memory prefetch runahead
  // prints them in the format <name>, <address> and will only print if the load operation has a name
  //----------------------------------------------------------------------------------------------------
  void printAllLoadOps(Function *F)
  {
  	static LLVMContext TheContext;
	LLVMContext& ctx = TheContext;
	IRBuilder<> builder(ctx);
	Module *M = F->getParent();
	for(Function::iterator fi=F->begin(), fend=F->end(); fi!=fend; ++fi) {
		BasicBlock *blk = &*fi;
		for(BasicBlock::iterator bi=blk->begin(), bend=blk->end(); bi!=bend; ++bi) {
			Instruction *inst = &*bi;
			if(LoadInst *ld_inst = dyn_cast<LoadInst>(inst)) {
				if(ld_inst->hasName()) {
					Function *printf_func = M->getFunction("printf"); 
					if(!printf_func)
						errs() << "Could not find the printf function!\n";
					std::vector<Value *> Args; //printf arguments
				
					builder.SetInsertPoint(ld_inst);
					//First argument: the string	
					std::stringstream ss;
					ss << F->getName().str() << " - " << ld_inst->getName().str() << "  0x%x\n";	
					std::string s = ss.str();
					Constant *format_const = ConstantDataArray::getString(ctx, s);					
					
					GlobalVariable *var = new GlobalVariable
								(*M, format_const->getType(),
								true, GlobalValue::PrivateLinkage, format_const, ".str");		
					Constant *zero = Constant::getNullValue(llvm::IntegerType::getInt32Ty(ctx));
					std::vector<Constant*> indices;
					indices.push_back(zero);
					indices.push_back(zero);
					Constant *var_ref = ConstantExpr::getGetElementPtr(format_const->getType(), var, indices);
					Args.push_back(var_ref);					

					//Second argument: the load address
					Args.push_back(ld_inst->getPointerOperand());	

					//Call the printf function with the args
					CallInst *printf_call = builder.CreateCall(printf_func, Args, "printfcall");
					printf_call->setTailCall(false);

				}
			}	
		}	
	}

  }







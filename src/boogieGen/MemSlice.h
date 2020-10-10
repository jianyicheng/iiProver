#ifndef MEMSLICE_LIB
#define MEMSLICE_LIB 

#include "Slice.h"
#include <string>
#include <vector>
#include <exception>
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/Cloning.h"

using namespace llvm;

//MemorySlice class from Slice base class
//Only accepts memory operations as slicing criterion 
class MemSlice: public Slice
{
	public: 
		MemSlice(Function *iF):Slice(iF) { } //constructor
		MemSlice( const MemSlice &obj ): Slice( obj ) { } //copy constructor
		MemSlice& operator=( const MemSlice &obj ); //assignment operator
		void addCriterion(Value *v); //adds a slicing criterion to the slice, ensures type TerminationInst
		unsigned estimateRunahead(); //estimates the runahead for this MemorySlice 
		unsigned estimateCycles(std::vector<Instruction *> *path);
		unsigned computeCriticalPaths(BasicBlock *blk);
		unsigned computeCriticalPathHelper(BasicBlock *blk, std::vector<std::vector<Instruction *>*>* paths);
		MemSlice merge(MemSlice *in); //Merges the current slice and returns the merged version
	private:
};
	//Assignment Operator
	MemSlice& MemSlice::operator= ( const MemSlice &obj ) 
	{ 
		Slice::operator=(static_cast<Slice const&>(obj));
		return *this; 
	} 

	void MemSlice::addCriterion(Value *v) 
	{
		if(isa<LoadInst>(v) || isa<StoreInst>(v)) {
			addIfNotPresent(v);
			if(!v->hasName())
				v->setName("LD"); //assign a default name to load operations
			criterion.add(v);
			update();
			getDepBranches();
			getDepMem();
			checkForDuplicates();
		}
		else {
			//Debugging
			if(isa<CallInst>(v)) {
				addIfNotPresent(v);
				criterion.add(v);
				update();
				getDepBranches();
				getDepMem();
				checkForDuplicates();
			} else {
				errs() << "Error: " << *v <<  " is not a memory operation and cannot be a MemSlice slicing criterion!\n";	
				assert(false);
			}
		}
	} 
	
	unsigned MemSlice::estimateRunahead()
	{
		unsigned totalRunahead = 0;
		for(Function::iterator ib=F->begin(), eb=F->end(); ib!=eb; ++ib) {
			BasicBlock *blk = &*ib;
			totalRunahead += computeCriticalPaths(blk);	
		}
		return totalRunahead;
	}

	unsigned MemSlice::computeCriticalPaths(BasicBlock *blk)
	{
		std::vector<std::vector<Instruction *>*> *paths = new std::vector<std::vector<Instruction *>*>;	
		for(BasicBlock::iterator bi=blk->begin(), be=blk->end(); bi != be; ++bi) {
			std::vector<Instruction *> *path = new std::vector<Instruction *>;			
			Instruction *curr = &*bi;
			path->push_back(curr);
			paths->push_back(path);
		}

		unsigned original = computeCriticalPathHelper(blk, paths);

		std::vector<std::vector<Instruction *>*> *slice_paths = new std::vector<std::vector<Instruction *>*>;	
		for(iterator is=sliceInst->begin(), ie=sliceInst->end(); is != ie; ++is)
		{
			Value *curr = *is;
			if(Instruction *currInst = dyn_cast<Instruction>(curr)) {
				if(currInst->getParent() == blk) {
					std::vector<Instruction *> *path = new std::vector<Instruction *>;
					path->push_back(currInst);
					slice_paths->push_back(path);	
				}
			}
		}
	
		unsigned critPathLength = computeCriticalPathHelper(blk, slice_paths);
		
		unsigned runahead = 0;
		if(critPathLength != 0) {
			runahead = (original - critPathLength);
		}

		delete slice_paths;
		delete paths;	
		return runahead;
	}	


	unsigned MemSlice::computeCriticalPathHelper(BasicBlock *blk, std::vector<std::vector<Instruction *>*>* paths)
	{
		bool fixedpoint = false;
		while(!fixedpoint){
			fixedpoint = true;		
			for(std::vector<std::vector<Instruction *>*>::iterator ipaths=paths->begin(),
			    epaths=paths->end(); ipaths != epaths; ++ipaths) {
				std::vector<Instruction *> *path = *ipaths;
					for(std::vector<Instruction *>::iterator ipath=path->begin(), epath=path->end();
					    ipath != epath; ++ipath) {
						Instruction *curr = *ipath;
						for(unsigned op=0; op<curr->getNumOperands();op++) {
							if(Instruction *operand = dyn_cast<Instruction>(curr->getOperand(op))){
								if(operand->getParent() == blk)
								{
									bool already_exists = false;
									for(std::vector<Instruction *>::iterator as=path->begin(),
									    ae=path->end(); as != ae; ++as) {
										Instruction *I = *as;
										if(I == operand)
											already_exists = true;
									}
									if(!already_exists) {
										path->push_back(operand);
										fixedpoint = false;
									}
								}
							}
						}
					}
			}
		}
		
		unsigned crit_path = 0;	
		for(std::vector<std::vector<Instruction *>*>::iterator ipaths=paths->begin(),
		    epaths=paths->end(); ipaths != epaths; ++ipaths) {
			std::vector<Instruction *> *path = *ipaths;
			if(estimateCycles(path) > crit_path) {
				crit_path = estimateCycles(path);		
			}
		}

		return crit_path;
	}

	unsigned MemSlice::estimateCycles(std::vector<Instruction *> *path)
	{
		unsigned total = 0;
		for(std::vector<Instruction *>::iterator i=path->begin(), epath=path->end(); i!=epath; ++i) {
			Instruction * curr = *i;	
			const char* op = Instruction::getOpcodeName(curr->getOpcode());
			if(strcmp(op,"getelementptr") == 0)
				total = total + 0;
			else if(strcmp(op,"mul") == 0)
				total = total + 2;
			else if(strcmp(op,"fadd") == 0)
				total = total + 6;
			else if(strcmp(op,"add") == 0)
				total = total + 1;
			else if(strcmp(op,"phi") == 0)
				total = total + 0;
			else if(strcmp(op,"br") == 0)
				total = total + 1;
			else if(strcmp(op,"ret") == 0)
				total = total + 1;
			else
				total = total + 1; //default case
			
		}
		return total;
	}

	
	//------------------------------------------------------------------------------
	// merge: merges the current slice with the input and returns it 
	//------------------------------------------------------------------------------
	MemSlice MemSlice::merge(MemSlice *in)
	{
		MemSlice merged(F); 
		
		//Copy the Criterion over
		SliceCriterion criterion_this; 
		SliceCriterion criterion_in;
	
		criterion_this = copyCriterion();
		criterion_in = in->copyCriterion();

		for(SliceCriterion::iterator i=criterion_this.begin(), iend=criterion_this.end(); i!=iend; ++i) {
			Value *v = *i;
			merged.addCriterion(v);
		}

		for(SliceCriterion::iterator j=criterion_in.begin(), jend=criterion_in.end(); j!=jend; ++j) {
			Value *v = *j;
			merged.addCriterion(v);
		}

		//Check and add all other instructions
		for(MemSlice::iterator ii=begin(), iiend=end(); ii!=iiend; ++ii) {
			Value *item_this = *ii;
			merged.addIfNotPresent(item_this);
		}	
		for(MemSlice::iterator jj=in->begin(), jjend=in->end(); jj!=jjend; ++jj) {
			Value *item_in = *jj;	
			merged.addIfNotPresent(item_in);
		}

		return merged;
	}

#endif

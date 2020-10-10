//===-- Slice.h - Slice class definition -------*- C++ -*-===//
//
//                   PerfectPrefetcher: Speculative Slices for High Level Synthesis 
//
// This file is distributed under the StitchUp 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// 
///
//===----------------------------------------------------------------------===//

#ifndef SLICESET_LIB
#define SLICESET_LIB

#include <string>
#include <vector>
#include <exception>
#include <fstream>
#include "llvm/IR/InstrTypes.h"
#include "SliceArrayRefs.h"
#include "SliceMem.h"
#include "SliceCriterion.h"

using namespace llvm;

//Slice Base class
	class Slice {
		public:
			typedef std::vector<Value *>::iterator iterator;
			typedef std::vector<Value *>::const_iterator const_iterator;
			Slice(Function *iF); //Default Constructor
			~Slice(); //Default Deconstructor
			Slice(const Slice &obj); //Copy Constructor
			Slice& operator= (const Slice &obj); //Assignment Operator
			
			
			virtual void addCriterion(Value *v); //Adds a slicing criterion to the sliceInst
			bool getDepChain(Value *v); //Calculates the dependancy chain for the sliceInst set
			void getDepMem(); //Adds instructions that touch memory to the Slice
			bool checkIfExists(Value *v); //Checks to see if a Value exists in the sliceInst set
			bool addIfNotPresent(Value *v); //Adds a value to the sliceInst set if it is not already present
			void getDepBranches(); //Fills in the branch decision which influence the address calc
			void update(); //Recomputes the sliceInst, usually performed after the addition of a slicing criterion
			unsigned instructionCount(); //Returns the number of instructions within the sliceInst
			SliceCriterion copyCriterion(); //returns a copy of the underlying slice criterion for this slice
			iterator begin(); //Returns the begining of a sliceInst Slice::iterator	
			iterator end(); //Returns the end of a sliceInst Slice::iterator
			const_iterator begin() const; //Returns the begining of a sliceInst Slice::iterator	
			const_iterator end() const; //Returns the end of a sliceInst Slice::iterator
			void printCriterion(std::ofstream &out); //prints all the criterion associated with this sliceInst
			Slice merge(Slice *in); //returns a slice which is the merged product of the current and input
			bool usesBB(BasicBlock *b); //returns true if the slice makes use of BasicBlock b
			Value *getFirstCriterion(); //returns the first Criterion added to make the slice
			
			//---Debugging---
			void print(); //Prints the sliceInst set
			void checkForDuplicates(); //Checks to make sure there are not duplicate instructions in the sliceInst
        
		protected:
			std::vector<Value *>* sliceInst; //Set of LLVM values that influence control
			Function *F; //The current LLVM Input function that we are analysing
			SliceArrayRef sArrays;
			SliceMem sMem;
			SliceCriterion criterion; //set of sliceInst criterion 
			 
	}; //Class Slice

	//constructor
	Slice::Slice(Function *iF)
	{
		F = iF;
		sliceInst = new std::vector<Value *>;
	}

	//deconstructor
	Slice::~Slice()
	{
		delete sliceInst;	
	}

	Slice::Slice(const Slice &obj) //Copy Constructor
	{
		sliceInst = new std::vector<Value *>;
		F = obj.F;
		criterion = obj.criterion;
		sArrays = obj.sArrays;
		sMem = obj.sMem;
		for(Slice::const_iterator i=obj.begin(), iend=obj.end(); i!=iend; ++i) {
			Value *v = *i;
			addIfNotPresent(v);
		} 
	}

	Slice& Slice::operator= (const Slice &obj) //Assignment Operator
	{
		sliceInst = new std::vector<Value *>;
		F = obj.F;
		criterion = obj.criterion;
		sArrays = obj.sArrays;
		sMem = obj.sMem;
		for(Slice::const_iterator i=obj.begin(), iend=obj.end(); i!=iend; ++i) {
			Value *v = *i;
			addIfNotPresent(v);
		} 
		return *this;
	}

	Slice::iterator Slice::begin() { return sliceInst->begin(); }
	Slice::iterator Slice::end() { return sliceInst->end(); }
	Slice::const_iterator Slice::begin() const { return sliceInst->begin(); }
	Slice::const_iterator Slice::end() const { return sliceInst->end(); }
	
	void Slice::addCriterion(Value *v)
	{
		criterion.add(v);
		addIfNotPresent(v);
		update();
		getDepBranches();
		getDepMem();
		
	}

	bool Slice::getDepChain(Value *v)
	{
		bool fp = true;
		if(addIfNotPresent(v))
			fp = false;
		if(Instruction *inst = dyn_cast<Instruction>(v)){
			if(!isa<ReturnInst>(inst)){
			for(unsigned op=0; op<inst->getNumOperands(); op++){
				if(!checkIfExists(inst->getOperand(op)) & !isa<Constant>(inst->getOperand(op)))	
					getDepChain(inst->getOperand(op));
			}	
			}
		}
		
		return fp;
	}

	void Slice::update() 
	{
		bool fp = false;
		while(!fp)
		{
			fp = true;
			for(Slice::iterator is=begin(), ie=end(); is!=ie; ++is)
			{
				Value* item = *is;
				if(Instruction *inst = dyn_cast<Instruction>(item))
				{
					if(!isa<ReturnInst>(inst)) {
						for(unsigned i=0; i<inst->getNumOperands(); i++)
						{
							Value *operand = inst->getOperand(i);
							if(addIfNotPresent(operand)) { fp = false; }	
						}
					}
				}	
			}
		}
	}

	void Slice::getDepBranches()
	{
		//We need to find the Basic block for all instructions 
		//in the sliceInst and add it's terminator instruction
		//This needs to be a fixed point.
		bool fixedpoint = false;
		while(!fixedpoint)
		{
			fixedpoint = true;
			for(Slice::iterator s=sliceInst->begin(), se=sliceInst->end(); s != se; ++s) {
				Value *sliceInstmem = *s;
				for(Function::iterator i=F->begin(), ie=F->end(); i != ie; ++i)	{
					BasicBlock *blk = &*i;
					for(BasicBlock::iterator j=blk->begin(), je=blk->end(); j != je; ++j) {
						Value *inst = &*j;
						if(sliceInstmem==inst){
							TerminatorInst *t = blk->getTerminator();	
							if(!getDepChain(t))
								fixedpoint = false;
						}
					}
				}
				if(PHINode *phiInst = dyn_cast<PHINode>(sliceInstmem)) {
					for(PHINode::block_iterator b=phiInst->block_begin(), bend=phiInst->block_end(); b!=bend; ++b) {
						BasicBlock *blk = *b;	
						if(!getDepChain(blk->getTerminator()))
						{
							fixedpoint=false;
							if(!usesBB(blk))
								errs() << "It doesn't appear to be used in the Slice though!\n";
						}
					}
				}
			}
	
			for(Function::iterator fi=F->begin(), fend=F->end(); fi!=fend; ++fi) {
				BasicBlock *b = &*fi;
				TerminatorInst *tInst = b->getTerminator();
				if(BranchInst *bInst = dyn_cast<BranchInst>(tInst)) {
					if(bInst->isConditional()) {
						for(unsigned i=0; i < bInst->getNumSuccessors(); i++) {
							BasicBlock *succ = bInst->getSuccessor(i);
							if(usesBB(succ)) {
								if(!getDepChain(bInst)) 
									fixedpoint = false;
							}		
						}
					} 
				}
			}
		}	
		return;
	}


	void Slice::getDepMem()
	{
		bool fixedpoint = false;
		while(!fixedpoint) 
		{
			fixedpoint = true;
			for(Slice::iterator s=sliceInst->begin(), se=sliceInst->end(); s!=se; ++s) {
				Value *inst = *s;
				if(LoadInst *loadi = dyn_cast<LoadInst>(inst)){
					if(checkIfExists(loadi)) { //Load is used so we need to tag memory
						Value *loadptr = loadi->getPointerOperand();	
						//We need to tag the corresponding memory location
						if(sMem.addIfNotPresent(loadptr)) {
							//If it's an array access tag the entire array
							if(GetElementPtrInst *t=dyn_cast<GetElementPtrInst>(loadptr))
								sArrays.addIfNotPresent(t);		
							addIfNotPresent(loadptr);
							fixedpoint = false;
						}	
					}
				}
			}

			for(Function::iterator fs=F->begin(), fe=F->end(); fs!=fe; ++fs) {
				BasicBlock *blk = &*fs;
				for(BasicBlock::iterator bs=blk->begin(), be=blk->end(); bs!=be; ++bs) {
					Value *inst = &*bs;
					if(StoreInst *storei = dyn_cast<StoreInst>(inst)){
						Value *storeval = storei->getValueOperand();
						Value *storeptr = storei->getPointerOperand();
						//if the memory location has been tagged then
						//inst and op1 should be added to the sliceInst
						if(sMem.checkIfExists(storeptr)) {
							if(addIfNotPresent(storeval)) {fixedpoint=false;}
							if(addIfNotPresent(storei)) {fixedpoint=false;}
						}
						//Check to see if whole array is flagged
						if(GetElementPtrInst *t = dyn_cast<GetElementPtrInst>(storeptr)) {
							if(sArrays.checkIfExists(t)){
								if(addIfNotPresent(storeval)){fixedpoint=false;}
								if(addIfNotPresent(storei)){fixedpoint=false;}	
							}	
						}
					}
				}
			}
			if(!fixedpoint)
				update();
		} 	
	   getDepBranches();
	}




	bool Slice::addIfNotPresent(Value *v)
	{
		if(!checkIfExists(v))
		{
			sliceInst->push_back(v);
			return true;
		}
		return false;
	}


	//------------------------------------------------------------------------------
	// getFirstCriterion: returns the first criterion used to construct the slice 
	//------------------------------------------------------------------------------
	Value *Slice::getFirstCriterion()
	{	return criterion.getFirstCriterion();	}

	//------------------------------------------------------------------------------
	// merge: merges the current slice with the input and returns it 
	//------------------------------------------------------------------------------
	Slice Slice::merge(Slice *in)
	{
		Slice merged(F); 
		
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
		for(Slice::iterator ii=begin(), iiend=end(); ii!=iiend; ++ii) {
			Value *item_this = *ii;
			merged.addIfNotPresent(item_this);
		}	
		for(Slice::iterator jj=in->begin(), jjend=in->end(); jj!=jjend; ++jj) {
			Value *item_in = *jj;	
			merged.addIfNotPresent(item_in);
		}

		return merged;
	}

	//------------------------------------------------------------------------------
	// copyCriterion: returns a copy of the underlying slice criterion for this slice
	//------------------------------------------------------------------------------
	SliceCriterion Slice::copyCriterion()
	{
		SliceCriterion tmp = criterion;
		return tmp;
	}

	//------------------------------------------------------------------------------
	// checkIfExists: checks to see if the input Value v exists in the Slice 
	//------------------------------------------------------------------------------
	bool Slice::checkIfExists(Value *v)
	{
		for(Slice::iterator is=sliceInst->begin(), ie=sliceInst->end(); is != ie; ++is)
		{
			Value *curr = *is;
			if(curr == v) 
				return true;
		}
		return false;
	}

	//------------------------------------------------------------------------------
	// instructionCount: returns the number of instructions within the current sliceInst
	//------------------------------------------------------------------------------
	unsigned Slice::instructionCount()
	{
		unsigned count=0;
		for(Slice::iterator i=sliceInst->begin(), e=sliceInst->end(); i!=e; ++i)
		{
			Value *tmp = *i;
			if(isa<Instruction>(tmp))
				count++;	
		}
		return count;
	}

	void Slice::print()
	{
		errs() << "Printing Instructions for Slice of Function:  " << F->getName().str() << "\n\n";
		for(Slice::iterator is=sliceInst->begin(), ie=sliceInst->end(); is != ie; ++is)
		{
			Value *curr = *is;
			if(Instruction *inst = dyn_cast<Instruction>(curr))
				errs() << "\t" << *inst <<"\n";
		}
		return;
	}	

	//------------------------------------------------------------------------	
	// usesBB: returns true if the slice makes use of BasicBlock b
	//------------------------------------------------------------------------	
	bool Slice::usesBB(BasicBlock *b)
	{
		for(Slice::iterator i=begin(), iend=end(); i!=iend; ++i) {
			Value *v = *i;
			if(Instruction *inst = dyn_cast<Instruction>(v)){
				if(inst->isUsedInBasicBlock(b)) {
					return true;
				}
				TerminatorInst *tinst = b->getTerminator();
				if(tinst == inst) {
					return true;	
				}
			}
		}
		return false;
	}	

	
	void Slice::printCriterion(std::ofstream &out) //Prints all Criterion associated with current sliceInst
	{
		criterion.print(out);
	}

	void Slice::checkForDuplicates() //Checks for if the same value is more than once in the sliceInst
	{
		for(Slice::iterator i=sliceInst->begin(), ie=sliceInst->end(); i != ie; ++i) {
			Value *iitem = *i;
			unsigned occurence = 0;
			for(Slice::iterator j=sliceInst->begin(), je=sliceInst->end(); j != je; ++j) {
				Value *jitem = *j; 
				if(iitem == jitem)
					occurence++;
			}
			
			if(occurence > 1)
				errs() << "Appears " <<occurence<<" times: " << *iitem <<"\n";
		}
	}


#endif

#ifndef ISLICE_LIB
#define ISLICE_LIB 

#include "Slice.h"
#include <string>
#include <vector>
#include <exception>
#include "llvm/IR/InstrTypes.h"

using namespace llvm;

//InverseSlice class from Slice base class
//When given a Slice computes the inverse of it from the original function
class ISlice: public Slice
{
	public: 
		ISlice(Function *iF):Slice(iF) {  }
		void addCriterion(Value *v); //adds a slicing criterion to the slice, ensures type TerminationInst
		void inverse(Slice* p); //Calculates the inverse of Slice p from F
};

	void ISlice::addCriterion(Value *v) { } //You cannot add criterion to an inverse slice

	//------------------------------------------------------------------------------------------
	//	invert: Calculates the inverse of slice p from the input function
	//------------------------------------------------------------------------------------------
	void ISlice::inverse(Slice* p)
	{
		for(Function::iterator fs=F->begin(), fe=F->end(); fs!=fe; ++fs) {
			BasicBlock *blk = &*fs;
			for(BasicBlock::iterator bs=blk->begin(), be=blk->end(); bs!=be; ++bs) {
				Instruction *inst = &*bs;	
				if(!p->checkIfExists(inst))
					addIfNotPresent(inst);
			}
		}
	}
	

#endif

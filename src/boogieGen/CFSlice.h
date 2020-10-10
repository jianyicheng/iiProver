#ifndef CFSLICE_LIB
#define CFSLICE_LIB 

#include "Slice.h"
#include <string>
#include <vector>
#include <exception>
#include "llvm/IR/InstrTypes.h"

using namespace llvm;

//CFSlice class from Slice base class
class CFSlice: public Slice
{
	public: 
		CFSlice(Function *iF):Slice(iF) {  }
		void addCriterion(Value *v); //adds a slicing criterion to the slice, ensures type TerminationInst
};

	void CFSlice::addCriterion(Value *v) {
		assert(isa<TerminatorInst>(v)); //Slicing Criterion can only be branch instructions
		addIfNotPresent(v);
		criterion.add(v);
		update();
		getDepBranches();
		getDepMem();
		checkForDuplicates();
	}

#endif

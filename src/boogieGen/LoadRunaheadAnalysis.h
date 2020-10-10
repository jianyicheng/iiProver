#ifndef LDRUNAHEAD_ANALYSIS_LIB
#define LDRUNAHEAD_ANALYSIS_LIB

#include "Slice.h"
#include "MemSlice.h"
#include <string>
#include <vector>
#include <exception>
#include <fstream>
#include "llvm/IR/InstrTypes.h"

using namespace llvm;

//LoadRunaheadAnalysis 
//Analysis to determine which load operations should be executed in a speculative slice
class LoadRunaheadAnalysis
{
	typedef std::vector<MemSlice *>::iterator iterator;
	public: 
		LoadRunaheadAnalysis(Function *iF); //constructor
		~LoadRunaheadAnalysis(); //destructor
		void extractAllLoads(); //Extracts every load operation from F
		iterator begin();
		iterator end();
		MemSlice mergeAll(); //Merges all memory slices into a single slice

		//Development & Testing Functions
		void printAnalysis(std::ofstream &out);
		void printToErrs(); //Prints Load list to errs()
		void addReadVirt(); //Adds every readVirt() call instruction (as if it were an original load)
	private:
		Function *F; //the function being evaluated
		std::vector<MemSlice*>* loadSlices; //Set of loadSlices considered for thread creation
		
}; //class LoadRunaheadAnalysis


	LoadRunaheadAnalysis::LoadRunaheadAnalysis(Function *iF) //Constructor
	{
		F = iF;
		loadSlices = new std::vector<MemSlice*>;
		//debugging
			addReadVirt();

		extractAllLoads();
	}	

        //----------DEBUG-----------------
        void LoadRunaheadAnalysis::addReadVirt() {
                errs() << "\t\t\tCalling addReadVirt()" << "\n";
                for(Function::iterator i=F->begin(), ie=F->end(); i != ie; ++i) {
                        BasicBlock *blk = &*i;
                        for(BasicBlock::iterator j=blk->begin(), je=blk->end(); j != je; ++j) {
                                Value *inst = &*j;
                                if(CallInst *cinst = dyn_cast<CallInst>(inst)) {
					Function *readVirt = cinst->getCalledFunction();
					if(readVirt->getName() == "readVirt") {
                                        	errs() << "\t\t\tFound a virtual address load call: " << *cinst << "\n";
						MemSlice *m = new MemSlice(F);
						m->addCriterion(cinst);
						loadSlices->push_back(m);
					}
                                }
                        }
                }
        }
        //----------DEBUG-----------------


	LoadRunaheadAnalysis::~LoadRunaheadAnalysis()//Deconstructor	
	{
		delete loadSlices;
	}

	LoadRunaheadAnalysis::iterator LoadRunaheadAnalysis::begin()
	{	return loadSlices->begin();	}

	LoadRunaheadAnalysis::iterator LoadRunaheadAnalysis::end()
	{	return loadSlices->end();	}


	//----------------------------------------------------------------------------
	// mergeAll: 
	// merges all memory slices in the function together into a single slice
	//----------------------------------------------------------------------------
	MemSlice LoadRunaheadAnalysis::mergeAll() {
		MemSlice allLoads(F);
		for(LoadRunaheadAnalysis::iterator i=begin(), iend=end(); i!=iend; ++i) {
			MemSlice *m = *i;
			allLoads = allLoads.merge(m);	
		}		
		return allLoads;
	}

	//----------------------------------------------------------------------------
	// extractAllLoads: 
	// Creates a MemSlice for every load operation in the original program
	// and adds it to the loadSlices list
	//----------------------------------------------------------------------------
	void LoadRunaheadAnalysis::extractAllLoads()	
	{
		for(Function::iterator fi=F->begin(), fend=F->end(); fi!=fend; ++fi)
		{
			BasicBlock *blk = &*fi;
			for(BasicBlock::iterator bi=blk->begin(), bend=blk->end(); bi!=bend; ++bi)
			{
				Instruction *inst = &*bi;
				if(isa<LoadInst>(inst))
				{
					MemSlice *m = new MemSlice(F);
					m->addCriterion(inst);
					loadSlices->push_back(m);
				}
			}
		} 
	}

	//-----DEVELOPMENT AND TESTING MEMBER FUNCTIONS----	
	void LoadRunaheadAnalysis::printAnalysis(std::ofstream &out)
	{
		out << "---------------LOAD ANALYSIS-------------------\n";
		for(iterator i=this->begin(), e=this->end(); i!=e; ++i)
		{
			MemSlice *m = *i;
			m->printCriterion(out);
			out <<  m->estimateRunahead() << "\n";	
		}	
	}

	void LoadRunaheadAnalysis::printToErrs()
	{
		errs() << "-----------------LOAD RUNAHEAD CONTENTS-------------------\n";
		for(iterator i=this->begin(), e=this->end(); i!=e; ++i) {
			MemSlice *m = *i;
			errs() << *m->getFirstCriterion() << "\n";
		}
		errs() << "\n\n";
	}

#endif

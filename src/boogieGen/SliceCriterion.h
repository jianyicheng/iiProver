#ifndef SLICE_CRITERION_LIB
#define SLICE_CRITERION_LIB 

#include "Slice.h"
#include <string>
#include <vector>
#include <exception>
#include <fstream>
#include "llvm/IR/InstrTypes.h"

using namespace llvm;

//SliceCriterion used to keep track of slicing criterion in the Slice class 
class SliceCriterion
{
	public: 
		typedef std::vector<Value*>::iterator iterator;
		typedef std::vector<Value*>::const_iterator const_iterator;
		SliceCriterion(); //Default constructor
		~SliceCriterion(); //Deconstructor
		SliceCriterion(const SliceCriterion &obj); //copy constructor
		SliceCriterion& operator= (const SliceCriterion &obj ); //assignment operator

		void add(Value *v); //add a slicing criterion
		bool checkIfExists(Value *v); //returns true if the slicing criterion is already present
		iterator begin(); //start of the criterion iterator
		iterator end(); //end of the criterion iterator
		const_iterator begin() const; //start of the criterion iterator
		const_iterator end() const; //end of the criterion iterator
		bool isEmpty(); //returns true if no criterions have been added
		Value *getFirstCriterion(); //returns the first criterion added to this slice

		void print(std::ofstream &out); //prints the criterion to an output
	private:
		Function *F;
		std::vector<Value *>* criterion;
		Value *firstCriterion; //Contains the first criterion that was added to the slice
}; //SliceCriterion class

	//Constructor
	SliceCriterion::SliceCriterion()
	{ 
		criterion = new std::vector<Value *>;	
	}	

	//Deconstructor
	SliceCriterion::~SliceCriterion()
	{
		delete criterion;
	}

	//Copy constructor
	SliceCriterion::SliceCriterion(const SliceCriterion &obj)
	{
		criterion = new std::vector<Value *>;
		for(SliceCriterion::const_iterator i=obj.begin(), iend=obj.end(); i!=iend; ++i) {
			Value *v = *i;
			add(v);
		}
	}

	//Assignment Operator
	SliceCriterion& SliceCriterion::operator= (const SliceCriterion &obj)
	{
		criterion = new std::vector<Value *>;
		for(SliceCriterion::const_iterator i=obj.begin(), iend=obj.end(); i!=iend; ++i) {
			Value *v = *i;
			add(v);
		}
		return *this;	
	}

	SliceCriterion::iterator SliceCriterion::begin() { return criterion->begin(); }
	SliceCriterion::iterator SliceCriterion::end() { return criterion->end(); }
	SliceCriterion::const_iterator SliceCriterion::begin() const { return criterion->begin(); }
	SliceCriterion::const_iterator SliceCriterion::end() const { return criterion->end(); }

	bool SliceCriterion::isEmpty() { return criterion->empty(); }
	Value* SliceCriterion::getFirstCriterion() { return firstCriterion; }

	void SliceCriterion::add(Value *v)
	{
		if(!checkIfExists(v))
			criterion->push_back(v);
		if(criterion->empty())
			firstCriterion = v;	
	}

	bool SliceCriterion::checkIfExists(Value *v)	
	{
		for(iterator i=begin(), iend=end(); i!=iend; ++i)
		{
			Value *curr = *i;
			if(v == curr)
				return true;
		}
		return false;
	}	


	void SliceCriterion::print(std::ofstream &out)
	{
		for(iterator i=this->begin(), iend=this->end(); i!=iend; ++i)
		{
			Value *v = *i;
			out << v->getName().str() << "\t";
		}
	}
#endif

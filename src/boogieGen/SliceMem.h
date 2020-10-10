//===-- SliceMem.h - SliceMem class definition -------*- C++ -*-===//
//
//                    StitchUp: Fault Tolerant High Level Synthesis 
//
// This file is distributed under the StitchUp 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
/// Contains the Control Memory Set data structure and methods for
/// operating over it. Used in the Control Flow Analysis as part of the Control Dependence Set
/// to keep track of memory locations that have an effect on the CFG of the program.
///
//===----------------------------------------------------------------------===//

#ifndef SLICEMEM_LIB
#define SLICEMEM_LIB 

#include <vector>
#include <exception>
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/DerivedTypes.h"

using namespace llvm;

	class SliceMem {
		public:
			typedef std::vector<Value *>::iterator iterator;
			typedef std::vector<Value *>::const_iterator const_iterator;
			SliceMem(); //Default Constructor
			~SliceMem(); //Default Deconstructor
			SliceMem( const SliceMem &obj ); //Copy Constructor
			SliceMem& operator= ( const SliceMem &obj ); //Assignment Operator

			bool checkIfExists(Value *v); //returns true if the Value is in the Smem
			void add(Value* v); //Adds a value to the Smem
			bool addIfNotPresent(Value *v); //Checks if a value is in the Smem and adds it to the set
			void remove(Value *v); //Removes from Smem
			void print(); //Prints the entire Smem, used for debugging	
			iterator begin(); //Returns the begining of a Smem iterator	
			iterator end(); //Returns the end of a Smem iterator
			const_iterator begin() const; //Returns the begining of a Smem iterator	
			const_iterator end() const; //Returns the end of a Smem iterator
		private:
			std::vector<Value *>* Smem; //Set of LLVM values that influence control
	
	}; //Class SliceMem

	//constructor
	SliceMem::SliceMem()
	{
		Smem = new std::vector<Value *>;
	}

	//deconstructor
	SliceMem::~SliceMem()
	{
		delete Smem;	
	}

	//copy constructor
	SliceMem::SliceMem( const SliceMem &obj )
	{
		Smem = new std::vector<Value *>;
		for(SliceMem::const_iterator i=obj.begin(), iend=obj.end(); i!=iend; ++i) {
			Value *v = *i;
			addIfNotPresent(v);
		}	
	}

	//assignment operator
	SliceMem& SliceMem::operator= ( const SliceMem &obj )
	{
		Smem = new std::vector<Value *>;
		for(SliceMem::const_iterator i=obj.begin(), iend=obj.end(); i!=iend; ++i) {
			Value *v = *i;
			addIfNotPresent(v);
		}	
		return *this;
	}

	SliceMem::iterator SliceMem::begin() { return Smem->begin(); }
	SliceMem::iterator SliceMem::end() { return Smem->end(); }
	SliceMem::const_iterator SliceMem::begin() const { return Smem->begin(); }
	SliceMem::const_iterator SliceMem::end() const { return Smem->end(); }

	//Iterates through the Smem and returns true if value is found
	bool SliceMem::checkIfExists(Value *v)
	{
		for(SliceMem::iterator cdsIter = Smem->begin(), Final=Smem->end(); cdsIter != Final; ++cdsIter)
		{
			Value *curr = *cdsIter;
			if(curr == v) { return true; }	
		}
		return false;
	}


	void SliceMem::add(Value *v)
	{
		Smem->push_back(v); 
	}

	//returns true if it could sucessfully add the item
	bool SliceMem::addIfNotPresent(Value *v)
	{
		if(!checkIfExists(v)){
			add(v);
			return true;
		}
		return false;
	}	

	void SliceMem::remove(Value *v)
	{
		for(SliceMem::iterator cdsIter = Smem->begin(), Final=Smem->end(); cdsIter != Final; ++cdsIter)
		{
			Value *curr = *cdsIter;
			if(curr == v) { Smem->erase(cdsIter); return; }
		}
	}
	
	//used for debugging
	void SliceMem::print()
	{
		errs() << "{";
		for(SliceMem::iterator it = this->begin(), itend=this->end(); it != itend; ++it)
		{
			Value *curr = *it;
			errs() << *curr <<",";
		}
		errs() << "}\n";
	}

#endif


//===-- SliceArrayRef.h - SliceArrayRef class definition -------*- C++ -*-===//
//
//                    StitchUp: Fault Tolerant High Level Synthesis 
//
// This file is distributed under the StitchUp 
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
///
/// \file
///
/// A class that is used to track which arrays and memory locations are used
/// when making control flow decisions.
//===----------------------------------------------------------------------===//

#ifndef SLICEARRAYREFS_LIB
#define SLICEARRAYREFS_LIB 

#include <vector>
#include <exception>
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Value.h"

using namespace llvm;

	class SliceArrayRef {
		public:
			typedef std::vector<Value *>::iterator iterator;
			typedef std::vector<Value *>::const_iterator const_iterator;
			SliceArrayRef(); //Default Constructor
			~SliceArrayRef(); //Default Deconstructor
			SliceArrayRef( const SliceArrayRef &obj ); //Copy Constructor
			SliceArrayRef& operator=( const SliceArrayRef &obj ); //Assignment Operator

			bool checkIfExists(Value *v); //returns true if the Value is in the SArray
			bool checkIfExists(GetElementPtrInst *v);
			void add(Value* v); //Adds a value to the SArray
			bool addIfNotPresent(Value *v); //Checks if a value is in the SArray and adds it to the set
			bool addIfNotPresent(GetElementPtrInst *v); //Extracts the Value from GetElementPtrInst then calls addIfNotPresent
			void remove(Value *v); //Removes from SArray
			void print(); //Prints the entire SArray, used for debugging	
			iterator begin(); //Returns the begining of a SArray iterator	
			iterator end(); //Returns the end of a SArray iterator
			const_iterator begin() const; //Returns the begining of a SArray iterator	
			const_iterator end() const; //Returns the end of a SArray iterator
		private:
			std::vector<Value *>* SArray; //Set of LLVM values that influence control
	
	}; //Class SliceArrayRef

	//constructor
	SliceArrayRef::SliceArrayRef()
	{
		SArray = new std::vector<Value *>;
	}

	//deconstructor
	SliceArrayRef::~SliceArrayRef()
	{
		delete SArray;	
	}

	//copy constructor
	SliceArrayRef::SliceArrayRef( const SliceArrayRef &obj )
	{
		SArray = new std::vector<Value *>;
		for(SliceArrayRef::const_iterator i=obj.begin(), iend=obj.end(); i!=iend; ++i) {
			Value *v = *i;
			addIfNotPresent(v);
		}
	}

	//assignment operator
	SliceArrayRef& SliceArrayRef::operator= ( const SliceArrayRef &obj )
	{
		SArray = new std::vector<Value *>;
		for(SliceArrayRef::const_iterator i=obj.begin(), iend=obj.end(); i!=iend; ++i) {
			Value *v = *i;
			addIfNotPresent(v);
		}
		return *this;
	}

	
	SliceArrayRef::iterator SliceArrayRef::begin() { return SArray->begin(); }
	SliceArrayRef::iterator SliceArrayRef::end() { return SArray->end(); }
	SliceArrayRef::const_iterator SliceArrayRef::begin() const { return SArray->begin(); }
	SliceArrayRef::const_iterator SliceArrayRef::end() const { return SArray->end(); }

	//Iterates through the SArray and returns true if value is found
	bool SliceArrayRef::checkIfExists(Value *v)
	{
		for(SliceArrayRef::iterator cdsIter = SArray->begin(), Final=SArray->end(); cdsIter != Final; ++cdsIter)
		{
			Value *curr = *cdsIter;
			if(curr == v) { return true; }	
		}
		return false;
	}

	bool SliceArrayRef::checkIfExists(GetElementPtrInst *v)
	{
		Value * array = v->getPointerOperand();
		return checkIfExists(array); 		
	}

	void SliceArrayRef::add(Value *v)
	{
		SArray->push_back(v); 
	}

	//returns true if it could sucessfully add the item
	bool SliceArrayRef::addIfNotPresent(Value *v)
	{
		if(!checkIfExists(v)){
			add(v);
			return true;
		}
		return false;
	}	

	//Extracts the Value from GetElementPtrInst then calls addIfNotPresent
	bool SliceArrayRef::addIfNotPresent(GetElementPtrInst *v)
	{
		Value * array = v->getPointerOperand();	
		return addIfNotPresent(array);
	}

	void SliceArrayRef::remove(Value *v)
	{
		for(SliceArrayRef::iterator cdsIter = SArray->begin(), Final=SArray->end(); cdsIter != Final; ++cdsIter)
		{
			Value *curr = *cdsIter;
			if(curr == v) { SArray->erase(cdsIter); return; }
		}
	}
	
	//used for debugging
	void SliceArrayRef::print()
	{
		errs() << "{";
		for(SliceArrayRef::iterator it = this->begin(), itend=this->end(); it != itend; ++it)
		{
			Value *curr = *it;
			errs() << *curr <<",";
		}
		errs() << "}\n";
	}
#endif


#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <cxxabi.h>
#include "llvm/IR/Type.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/TypeBuilder.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Dominators.h"
#include <regex>
#include <string>
#include <fstream>
using namespace llvm;

namespace {
struct phiNode {
    BasicBlock *bb;
    std::string res;
    Value *ip;
    Instruction* instr;
};
struct varList {
    Function *func;
    std::vector<Value *> vars;
};
struct invariance {
    Loop *loop;
    Instruction *indVarChange;
    std::string invar;
};
std::vector<varList *> codeVarList;
std::vector<phiNode *> phiList;
std::vector<invariance *> invarList;
std::fstream bpl;

std::string demangle(const char* name)
{
   int status = -1;
   std::unique_ptr<char, void(*)(void*)> res { abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
   return (status == 0) ? res.get() : std::string(name);
}

// This is function is determine if it is a function argument
bool isFuncArg(Value *instrVar, Function *F){
    for (auto fi = F->arg_begin(); fi != F->arg_end(); ++fi)
    {
        Value *arg = fi;
        if (instrVar == arg)
            return true;
    }
    return false;
}   // end of isFuncArg

// This is a function to print hidden basic block label
std::string getBlockLabel(BasicBlock *BB){
    std::string block_address;
    raw_string_ostream string_stream(block_address);
    BB->printAsOperand(string_stream, false);
    
    std::string temp = string_stream.str();
    
    for (unsigned int i = 0; i<temp.length(); ++i)
    {
        if (temp[i] == '-')
            temp.replace(i, 1, "_");
    }
    
    return temp;
}   // end of getBlockLabel

// This function is to check if the variable has been declared
bool varFoundInList(Value *instrVar, Function *F){
    bool funcFound = false;
    bool varFound = false;
    varList *list;
    
    // check if it is a constant
    if (dyn_cast<llvm::ConstantInt>(instrVar))
        return true;
    
    // search function in variable list
    for (auto it = codeVarList.begin(); it != codeVarList.end(); ++it)
    {
        varList *temp = *it;
        // errs() << "debug check function :" << F->getName() << "\n";// << " VS "<< temp->func->getName() << "\n";
        if (temp->func == F)
        {
            funcFound = true;
            // errs() << "Function " << F->getName() << " found.\n";
            list = *it;
            // Function found => search for var
            if (!isFuncArg(instrVar, F))
            {
                for (auto itVar = list->vars.begin(); itVar != list->vars.end(); ++itVar)
                {
                    Value *itvar = *itVar;
                    // errs() << "debug check var: " << printRegNameInBoogie(itvar) << "\n";
                    if (itvar == instrVar)
                        varFound = true;
                }
                if (!varFound)
                    list->vars.push_back(instrVar);
            }
            else
                varFound = true;
        }
    }
    
    if (!funcFound)
    {
        // Function not found => start new
        list = new varList;
        if (!isFuncArg(instrVar, F))
        {
            list->func = F;
            list->vars.push_back(instrVar);
        }
        // errs() << "Function " << F->getName() << " not found and added.\n";
        codeVarList.push_back(list);
    }
    
    return varFound;
}   // end of varFoundInList

// This is a function to print IR variable name
std::string printRegNameInBoogie(Value *I){
    std::string instrResName;
    raw_string_ostream string_stream(instrResName);
    I->printAsOperand(string_stream, false);
    if (ConstantInt *constVar = dyn_cast<ConstantInt>(I))
    {
        
        // errs() << "This is a constant: " << *I << "----";
        // if (constVar->getType()->isIntegerTy())
        if (constVar->isNegative())
        {
            string_stream.str().replace(0,1," ");
            // errs() << "Output test: bv32sub(0bv32, " + string_stream.str()+"bv32)\n";
            return ("bv32neg(" + string_stream.str()+"bv32)"); //*(constVar->getType())
        }
        else
            return(string_stream.str()+"bv32");
    }
    return string_stream.str();
}       // end of printRegNameInBoogie

void varDeclaration(Value *instrVar, bool isbool){
    bpl << "\tvar " << printRegNameInBoogie(instrVar) << ":bv32;\n";
}   // end of varDeclaration

void printBoogieHeader(void) {
    bpl << "\n//*********************************************\n";
    bpl <<   "//    Boogie code generated from LLVM\n";
    bpl <<   "//*********************************************\n";
    bpl << "// Bit vector function prototypes\n";
    bpl << "// Arithmetic\n";
    bpl << "function {:bvbuiltin \"bvadd\"} bv32add(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvsub\"} bv32sub(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvmul\"} bv32mul(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvudiv\"} bv32udiv(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvurem\"} bv32urem(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvsdiv\"} bv32sdiv(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvsrem\"} bv32srem(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvsmod\"} bv32smod(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvneg\"} bv32neg(bv32) returns(bv32);\n";
    bpl << "// Bitwise operations\n";
    bpl << "function {:bvbuiltin \"bvand\"} bv32and(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvor\"} bv32or(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvnot\"} bv32not(bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvxor\"} bv32xor(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvnand\"} bv32nand(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvnor\"} bv32nor(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvxnor\"} bv32xnor(bv32,bv32) returns(bv32);\n";
    bpl << "// Bit shifting\n";
    bpl << "function {:bvbuiltin \"bvshl\"} bv32shl(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvlshr\"} bv32lshr(bv32,bv32) returns(bv32);\n";
    bpl << "function {:bvbuiltin \"bvashr\"} bv32ashr(bv32,bv32) returns(bv32);\n";
    bpl << "// Unsigned comparison\n";
    bpl << "function {:bvbuiltin \"bvult\"} bv32ult(bv32,bv32) returns(bool);\n";
    bpl << "function {:bvbuiltin \"bvule\"} bv32ule(bv32,bv32) returns(bool);\n";
    bpl << "function {:bvbuiltin \"bvugt\"} bv32ugt(bv32,bv32) returns(bool);\n";
    bpl << "function {:bvbuiltin \"bvuge\"} bv32uge(bv32,bv32) returns(bool);\n";
    bpl << "// Signed comparison\n";
    bpl << "function {:bvbuiltin \"bvslt\"} bv32slt(bv32,bv32) returns(bool);\n";
    bpl << "function {:bvbuiltin \"bvsle\"} bv32sle(bv32,bv32) returns(bool);\n";
    bpl << "function {:bvbuiltin \"bvsgt\"} bv32sgt(bv32,bv32) returns(bool);\n";
    bpl << "function {:bvbuiltin \"bvsge\"} bv32sge(bv32,bv32) returns(bool);\n\n";
    bpl << "\n// Datatype conversion from bool to bv32\n";
    bpl << "procedure {:inline 1} bool2bv32 (i: bool) returns ( o: bv32)\n";
    bpl << "{\n";
    bpl << "\tif (i == true)\n";
    bpl << "\t{\n";
    bpl << "\t\to := 1bv32;\n";
    bpl << "\t}\n";
    bpl << "\telse\n";
    bpl << "\t{\n";
    bpl << "\t\to := 0bv32;\n";
    bpl << "\t}\n";
    bpl << "}\n";
}

// print argument of the function in prototype
void printFunctionPrototype(Function *F){
    bpl << "\nprocedure {:inline 1} " << static_cast<std::string>((F->getName())) << "(";
    if (!(F->arg_empty()))
    {
        for (auto fi = F->arg_begin(); fi != F->arg_end(); ++fi)
        {
            bpl << printRegNameInBoogie(fi) << ": bv32";
            
            auto fi_comma = fi;
            fi_comma++;
            if (fi_comma != F->arg_end())
                bpl << ", ";
        }
    }
    // only returns the bank address information
    bpl << ") returns (";
    
    bpl << ") \n";	// JC: add your customised return variables here
    
    bpl << "{\n";
} // end of printFunctionPrototype

// This section is to collect all the variables have been used and prepare for declaration in Boogie
void printVarDeclarations(Function *F)
{
    for (auto BB = F->begin(); BB != F->end(); ++BB) {                      // Basic block level
        for (auto I = BB->begin(); I != BB->end(); ++I) {                   // Instruction level
            switch (I->getOpcode()) {
                case 1:     // ret
                    // no need for var declarations
                    break;
                    
                case 2:     // br
                    if (I->getNumOperands() == 3)
                    {
                        if (!varFoundInList((Value *)(I->getOperand(0)), F))
                            varDeclaration((Value *)(I->getOperand(0)), 0);
                    }
                    break;
                    
                case 8:     // add
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                    
                case 10:     // sub
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                case 12:     // mul
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                case 20:     // shl
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                case 21:     // lshr
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                case 22:     // ashr
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                case 23:     // and
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                case 24:     // or
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                
                case 27:     // load
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    break;
                    
                case 28:     // store
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                
                case 29:     // getelementptr                               // this is what we want to modify
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);     // leave output random
                    break;
                    
                case 33:     // load
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    break;
                    
                case 34:     // zext
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    break;
                    
                case 46:    // icmp
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 1);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                case 48:     // phi
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 0);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    break;
                case 49:     // call
                    if (I->getNumOperands() > 1)
                    {
                        for (unsigned int i = 0; i != (I->getNumOperands()- 1); ++i)
                        {
                            if (!varFoundInList((Value *)(I->getOperand(i)), F))
                                varDeclaration((Value *)(I->getOperand(i)), 0);
                        }
                    }
                    break;
                case 50:     // select
                    if (!varFoundInList(&*I, &*F))
                        varDeclaration(&*I, 1);
                    if (!varFoundInList((Value *)(I->getOperand(0)), F))
                        varDeclaration((Value *)(I->getOperand(0)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(1)), F))
                        varDeclaration((Value *)(I->getOperand(1)), 0);
                    if (!varFoundInList((Value *)(I->getOperand(2)), F))
                        varDeclaration((Value *)(I->getOperand(2)), 0);
                    break;
                    
                default:
                    ;
            }
            
        }                                                               // End of instruction level analysis
    }                                                                   // End of basic block level analysis
}

// To split phi instruction to several load in corresponding basic blocks
void anlyzePhiInstr(Module &M)
{
    for (auto F = M.begin(); F != M.end(); ++F) {   // Function level
        for (auto BB = F->begin(); BB != F->end(); ++BB) {  // Basic block level
            for (auto I = BB->begin(); I != BB->end(); ++I) {   // Instruction level
                if (I->getOpcode() == 48)       // phi instruction
                {
                    if (llvm::PHINode *phiInst = dyn_cast<llvm::PHINode>(&*I)) {
                        phiNode *phiTfInst = new phiNode [phiInst->getNumIncomingValues()];
                        
                        for (unsigned int it = 0; it < phiInst->getNumIncomingValues(); ++it)
                        {
                            phiTfInst[it].res = printRegNameInBoogie(&*I);
                            phiTfInst[it].bb = phiInst->getIncomingBlock(it);
                            phiTfInst[it].ip = phiInst->getIncomingValue(it);
                            phiTfInst[it].instr = phiInst;
                            
                            phiList.push_back(&phiTfInst[it]);
                        } // end for
                    }  // end if
                } // end if
            }  // End of instruction level
        }   // End of basic block level
    }   // End of function level
}   // end of anlyzePhiInstr

// Translate all IR instruction to Boogie instruction
void instrDecoding(Function *F)
{
    // get all loop information
    llvm::DominatorTree* DT = new llvm::DominatorTree();
    DT->recalculate(*F);
    //generate the LoopInfoBase for the current function
    llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>* KLoop = new llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>();
    KLoop->releaseMemory();
    KLoop->analyze(*DT);
    
    int indexCounter = 0;
    for (auto BB = F->begin(); BB != F->end(); ++BB) {                      // Basic block level
        // bpl << "// For basic block: " << BB->getName() << "\n";
        bpl << "\n\t// For basic block: bb_" << getBlockLabel(&*BB) << "\n";
        bpl << "\tbb_" << getBlockLabel(&*BB) << ":\n";
        // errs() << "debug: " << *BB << "\n";
        // Here add assertion of loop invariant conditions at start of the loop
        if(KLoop->isLoopHeader(&*BB))
        {
            // phiNode *phiInst;
            // unsigned int idxNameAddr;
            if (BasicBlock *BBExit = KLoop->getLoopFor(&*BB)->getExitingBlock())
            {
                Instruction *exitCond;
                Instruction *startCond;
                Instruction *indVarBehaviour;
                std::string endBound;
                std::string startBound;
                std::string indvarName;
                std::string endSign;
                std::string startSign;
                std::string loopInverseCheck;   // check if exitcondition inversed
                bool equalSign = 0;
            
                auto instrexit = BBExit->end();
                --instrexit;
                
                if (printRegNameInBoogie((Value *)instrexit->getOperand(0)) != "undef")
                {
                    // instruction contains end loop index
                    for (auto exit_i = BBExit->begin(); exit_i != BBExit->end(); ++exit_i)
                    {
                        if (printRegNameInBoogie(&*exit_i) == printRegNameInBoogie(instrexit->getOperand(0)))
                            exitCond = &*exit_i;
                    }
                    endBound = printRegNameInBoogie(exitCond->getOperand(1));
                    
                    // check exit equality
                    loopInverseCheck = printRegNameInBoogie(exitCond);
                    loopInverseCheck.resize(9);
                    if (loopInverseCheck == "%exitcond")
                    {
                        if (CmpInst *cmpInst = dyn_cast<CmpInst>(&*exitCond)) {
                            if (cmpInst->getPredicate() == CmpInst::ICMP_NE)
                                equalSign = 1;
                            else if (cmpInst->getPredicate() == CmpInst::ICMP_UGT)
                                equalSign = 1;
                            else if (cmpInst->getPredicate() == CmpInst::ICMP_ULT)
                                equalSign = 1;
                            else if (cmpInst->getPredicate() == CmpInst::ICMP_SGT)
                                equalSign = 1;
                            else if (cmpInst->getPredicate() == CmpInst::ICMP_SLT)
                                equalSign = 1;
                        }
                    }
                    else
                    {
                        if (CmpInst *cmpInst = dyn_cast<CmpInst>(&*exitCond)) {
                            if (cmpInst->getPredicate() == CmpInst::ICMP_EQ)
                                equalSign = 1;
                            else if (cmpInst->getPredicate() == CmpInst::ICMP_UGE)
                                equalSign = 1;
                            else if (cmpInst->getPredicate() == CmpInst::ICMP_ULE)
                                equalSign = 1;
                            else if (cmpInst->getPredicate() == CmpInst::ICMP_SGE)
                                equalSign = 1;
                            else if (cmpInst->getPredicate() == CmpInst::ICMP_SLE)
                                equalSign = 1;
                        }
                    }
                    
                    // instruction contains start loop index
                    // this will not hold for matrix transfer... where exit condition not compatible...
                    
                    for (auto invarI = BBExit->begin(); invarI != BBExit->end(); ++invarI)
                    {
                        if (invarI->getOpcode() != 2)   // label name and variable name can be same causing bugs...
                        {
                            for (auto invarIt = phiList.begin(); invarIt != phiList.end(); ++invarIt)
                            {
                                phiNode *phiTfInst = *invarIt;
                                for (unsigned int it = 0; it < invarI->getNumOperands(); ++it)
                                {
                                    if (phiTfInst->res == printRegNameInBoogie((Value *)invarI->getOperand(it)))
                                    {
                                        if (invarI->getOpcode() == 8 || invarI->getOpcode() == 10)
                                        {
                                            indVarBehaviour = &*invarI;
                                            startCond = dyn_cast<Instruction>(invarI->getOperand(it));
                                        }
                                    }
                                }
                            }
                        }
                    }
                    
                    if (indVarBehaviour->getOpcode() == 8)    // add
                    {
                        startSign = "bv32sge(";
                        if (equalSign)
                            endSign = "bv32sle(";
                        else
                            endSign = "bv32slt(";
                    }
                    else if (indVarBehaviour->getOpcode() == 10)  // sub
                    {
                        startSign = "bv32sle(";
                        if (equalSign)
                            endSign = "bv32sge(";
                        else
                            endSign = "bv32sgt(";
                    }
                    else
                        errs() <<"Undefined index behaviour: "<< *indVarBehaviour << ", " << indVarBehaviour->getOpcode() << "\n";
                    
                    
                    indvarName = printRegNameInBoogie(startCond);
                    
                    // start and exit sign
                    if (llvm::PHINode *phiInst = dyn_cast<llvm::PHINode>(&*startCond)) {
                        for (unsigned int it = 0; it < phiInst->getNumIncomingValues(); ++it)
                        {
                            if (ConstantInt *constVar = dyn_cast<ConstantInt>(phiInst->getIncomingValue(it)))
                                startBound = printRegNameInBoogie(constVar);
                        } // end for
                    }  // end if
                    
                    // construct loop invariant string
                    invariance *currInvar = new invariance;
                    currInvar->loop = KLoop->getLoopFor(&*BB);
                    currInvar->indVarChange = indVarBehaviour;
                    currInvar->invar = startSign + indvarName + "," + startBound + ") && " + endSign + indvarName + "," + endBound + ")";
                    // debug
                    errs() << "Found loop invariant condition: (" << currInvar->invar << ")\n\n";
                    invarList.push_back(currInvar);
                    
                    // print
                    bpl << "\tassert ( " << currInvar->invar << ");\n";
                    bpl << "\thavoc " << indvarName << ";\n";
                    bpl << "\tassume ( " << currInvar->invar << ");\n";
                }   // end of check undef
            }
            else
            {
                // mark - this is the case of while(1)
                // Due to unfinished automated loop invariants generation, this section will be filled in the future updates
                errs() << "Found infintite loop entry: " << *BB << "\n";
                errs() << "Found infintite loop: " << *(KLoop->getLoopFor(&*BB)) << "\n";
                
            }
            
        }   // end of insert loop invariants in the beginning
        
        // Here add assertion of loop invariant conditions right before loop index change
        BasicBlock *currBB = &*BB;
        Instruction *endLoopCheckPoint;
        std::string endLoopInvar;
        int dim = 0;
        int search;
        if(KLoop->getLoopFor(&*BB))
        {
            if (currBB == KLoop->getLoopFor(&*BB)->getExitingBlock())
            {
                auto instrexit = currBB->end();
                --instrexit;
                Instruction *instrExit = &*instrexit;
                if (printRegNameInBoogie((Value *)instrExit->getOperand(0)) != "undef")
                {
                    // errs() << "loop exit: " << *BB << "\n";
                    search = 0;
                    for (auto it = invarList.begin(); it != invarList.end(); ++it)
                    {
                        invariance *currInvar = *it;
                        if (KLoop->getLoopFor(&*BB) == currInvar->loop)
                        {
                            search++;
                            endLoopCheckPoint = currInvar->indVarChange;
                            endLoopInvar = currInvar->invar;
                        }
                    }
                    if (search != 1)
                        errs() << "Error: Loop invariant errors at end of the loop - " << search << "conditions matched.\n";
                }
            }
        }
        
        // Start instruction printing
        for (auto I = BB->begin(); I != BB->end(); ++I) {                   // Instruction level
            // start printing end loop assertion here
            if ((Instruction *)&*I == endLoopCheckPoint)
            {
                errs() << "Matched loop invariant condition " << search << ": (" << endLoopInvar << ")\n\n";
                bpl << "\tassert ( " << endLoopInvar << ");\n";
                // skip current basic block
                I = BB->end();
                --I;
                // bpl << "\treturn;\n\tassume false;\n";
            }
            
            switch (I->getOpcode()) {
                case 1:     // ret
                    bpl << "\treturn;\n";  //default return - memory not accessed.
                    break;
                    
                case 2:     // br
                    // do phi resolution here
                    for (auto it = phiList.begin(); it != phiList.end(); ++it)
                    {
                        phiNode *phiTfInst = *it;
                        if (phiTfInst->bb == &*BB)
                            bpl << "\t" << phiTfInst->res << " := " << printRegNameInBoogie(phiTfInst->ip) << ";\n";
                    }
                    
                    // add branch instruction here
                    if (endLoopCheckPoint->getParent() == I->getParent()) {
                        // typically for loop jumping back to iteration
                        // show original in comments
                        if (I->getNumOperands() == 1) // if (inst->isConditional())?
                            errs() << "Error: Mistaken a simple br as a loop exit condition check: " << *I << "\n";
                        else if (I->getNumOperands() == 3)
                            bpl << "//\tif(" << printRegNameInBoogie((Value *)I->getOperand(0)) << " == 1bv32) {goto bb_"<< getBlockLabel((BasicBlock *)(I->getOperand(2))) << ";} else {goto bb_" << getBlockLabel((BasicBlock *)(I->getOperand(1))) << ";}\n";
                        else
                            errs() << "Error: Instruction decoding error at br instruction: " << *I << "\n";
                        
                        bpl << "\tgoto bb_"<< getBlockLabel((BasicBlock *)(I->getOperand(2))) << ";\n";
                    }
                    else
                    {   // normal cases
                        if (I->getNumOperands() == 1) // if (inst->isConditional())?
                            bpl << "\tgoto bb_"<< getBlockLabel((BasicBlock *)(I->getOperand(0))) << ";\n";
                        else if (I->getNumOperands() == 3)
                            bpl << "\tif(" << printRegNameInBoogie((Value *)I->getOperand(0)) << " == 1bv32) {goto bb_"<< getBlockLabel((BasicBlock *)(I->getOperand(2))) << ";} else {goto bb_" << getBlockLabel((BasicBlock *)(I->getOperand(1))) << ";}\n";
                        else
                            errs() << "Error: Instruction decoding error at br instruction: " << *I << "\n";
                    }
                    break;
                    
                case 8:     // add
                    if (OverflowingBinaryOperator *op = dyn_cast<OverflowingBinaryOperator>(I)) {
                        if ((op->hasNoUnsignedWrap()) && (op->hasNoSignedWrap()))
                            // has both nuw and nsw
                            bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32add("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else if (op->hasNoUnsignedWrap())
                            // only nuw
                            bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32add("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else if (op->hasNoSignedWrap())
                            // only nsw
                            bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32add("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else
                            // normal add
                            bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32add("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    }
                    else
                        errs() << "Error: Instruction decoding error at add instruction: " << *I << "\n";
                    break;
                    
                case 10:     // sub
                    if (OverflowingBinaryOperator *op = dyn_cast<OverflowingBinaryOperator>(I)) {
                        if ((op->hasNoUnsignedWrap()) && (op->hasNoSignedWrap()))
                            // has both nuw and nsw
                            bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32sub("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else if (op->hasNoUnsignedWrap())
                            // only nuw
                            bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32sub("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else if (op->hasNoSignedWrap())
                            // only nsw
                            bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32sub("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else
                            // normal add
                            bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32sub("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    }
                    else
                        errs() << "Error: Instruction decoding error at sub instruction: " << *I << "\n";
                    break;
                    
                case 12:     // mul
                    bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32mul("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                case 20:     // shl
                    bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32shl("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                case 21:     // lshr
                    bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32lshr("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                    
                case 22:     // ashr
                    bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32ashr("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                case 23:     // and
                    bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32and("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                case 24:     // or
                    bpl << "\t" << printRegNameInBoogie(&*I) << " := bv32or("<< printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                case 27:     // load
                    // JC: add your if(*) here
                        bpl << "\t" << printRegNameInBoogie(&*I) << " := "<< printRegNameInBoogie((Value *)I->getOperand(0)) << ";\n";
                    
                    break;
                    
                case 28:    // store
                    // JC: add your if(*) here
                        bpl << "\t" << printRegNameInBoogie((Value *)I->getOperand(1)) << " := "<< printRegNameInBoogie((Value *)I->getOperand(0)) << ";\n";
                    break;
                case 29:     // getelementptr                               // this can be ignored
                	// JC: in your case, the address value does matter, so please rewrite the following code in comment  
                	// to replace the address with 0bv32

                    /*for (auto ait = arrayInfo.begin(); ait != arrayInfo.end(); ++ait)
                    {   // get dimension
                        arrayNode *AN = *ait;
                        std::vector<uint64_t> size = AN->size;
                        GlobalVariable *addr = AN->addr;
                        
                        if (I->getOperand(0) == addr)
                        {
                            dim = size.size();
                            break;
                        }
                    }
                    
                    if (dim == 0)
                        errs() << "Error: unknown array accessed - cannot find size: " << *I << "\n";
                    else if (dim == 1)
                        bpl << "\t" << printRegNameInBoogie((Value *)I) << " := " << printRegNameInBoogie((Value *)I->getOperand(0)) << "[" << printRegNameInBoogie((Value *)I->getOperand(2)) << "];\n";
                    else if (dim == 2)
                        bpl << "\t" << printRegNameInBoogie((Value *)I) << " := " << printRegNameInBoogie((Value *)I->getOperand(0)) << "[" << printRegNameInBoogie((Value *)I->getOperand(1)) << "][" << printRegNameInBoogie((Value *)I->getOperand(2)) << "];\n";
                    else
                        errs() << "Error: 2+ dimension array accessed - not compatible yet: " << *I << "\n";*/
                        bpl << "\t" << printRegNameInBoogie(&*I) << " := 0bv32;\n";
                    break;
                    
                case 33:     // trunc
                    // ignore trunc instructions
                    bpl << "\t" << printRegNameInBoogie(&*I) << " := "<< printRegNameInBoogie((Value *)I->getOperand(0)) << ";\n";
                    break;
                    
                case 34:     // zext   - may be modified later
                    // possible types are i1, i8. i16, i24, i32, i40, i48, i56, i64, i88, i96, i128, ref, float
                    if (I->getType()->getTypeID() == 10) // check if it is integer type
                    {
                        IntegerType *resType = dyn_cast<IntegerType>(I->getType());
                        IntegerType *oprType = dyn_cast<IntegerType>(I->getOperand(0)->getType());
                        
                        if (!resType)
                            errs() << "Error found in getting result type of zext instruction: " << *I << "\n";
                        if (!oprType)
                            errs() << "Error found in getting operand type of zext instruction: " << *I << "\n";
                        
                        if (resType->getBitWidth() > oprType->getBitWidth())
                            bpl << "\t" << printRegNameInBoogie(&*I) << " := " << printRegNameInBoogie((Value *)I->getOperand(0)) << ";\n";
                        else
                        {
                            // do mod
                            // case errs() << printRegNameInBoogie(&*I) << ":=" << printRegNameInBoogie((Value *)I->getOperand(0)) << " % " << ??? << ";\n";
                        }
                    }
                    else
                        errs() << "Error: Undefined type in zext instruction: " << *I << "\n";
                    break;
                    
                case 46:    // icmp
                    if (CmpInst *cmpInst = dyn_cast<CmpInst>(&*I)) {
                        if (cmpInst->getPredicate() == CmpInst::ICMP_EQ) {
                            bpl << "\tif (" << printRegNameInBoogie((Value *)I->getOperand(0)) << " == " << printRegNameInBoogie((Value *)I->getOperand(1)) << ") { " << printRegNameInBoogie(&*I) << " := 1bv32; } else { " << printRegNameInBoogie(&*I) << " := 0bv32; }\n";
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_NE) {
                            bpl << "\tif (" << printRegNameInBoogie((Value *)I->getOperand(0)) << " != " << printRegNameInBoogie((Value *)I->getOperand(1)) << ") { " << printRegNameInBoogie(&*I) << " := 1bv32; } else { " << printRegNameInBoogie(&*I) << " := 0bv32; }\n";
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_UGT) {
                            bpl << "\tif (bv32ugt(" << printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printRegNameInBoogie(&*I) << " := 1bv32; } else { " << printRegNameInBoogie(&*I) << " := 0bv32; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_UGE) {
                            bpl << "\tif (bv32uge(" << printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printRegNameInBoogie(&*I) << " := 1bv32; } else { " << printRegNameInBoogie(&*I) << " := 0bv32; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_ULT) {
                            bpl << "\tif (bv32ult(" << printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printRegNameInBoogie(&*I) << " := 1bv32; } else { " << printRegNameInBoogie(&*I) << " := 0bv32; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_ULE) {
                            bpl << "\tif (bv32ule(" << printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printRegNameInBoogie(&*I) << " := 1bv32; } else { " << printRegNameInBoogie(&*I) << " := 0bv32; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_SGT) {
                            bpl << "\tif (bv32sgt(" << printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printRegNameInBoogie(&*I) << " := 1bv32; } else { " << printRegNameInBoogie(&*I) << " := 0bv32; }\n";
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_SGE) {
                            bpl << "\tif (bv32sge(" << printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printRegNameInBoogie(&*I) << " := 1bv32; } else { " << printRegNameInBoogie(&*I) << " := 0bv32; }\n";
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_SLT) {
                            bpl << "\tif (bv32slt(" << printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printRegNameInBoogie(&*I) << " := 1bv32; } else { " << printRegNameInBoogie(&*I) << " := 0bv32; }\n";
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_SLE) {
                            bpl << "\tif (bv32sle(" << printRegNameInBoogie((Value *)I->getOperand(0)) << ", " << printRegNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printRegNameInBoogie(&*I) << " := 1bv32; } else { " << printRegNameInBoogie(&*I) << " := 0bv32; }\n";
                        }
                        else
                            errs() << "Error: Instruction decoding error at icmp instruction: " << *I << "\n";
                    }
                    break;
                case 48:     // phi
                    // Has been done in previous section
                    break;
                case 49:     // call
                    errs() << "Error: Call functions found in the function: " << *I << "\n";
                    break;
                case 50:     // select
                    bpl << "\tif (" << printRegNameInBoogie((Value *)I->getOperand(0)) << " == 1bv32) { " << printRegNameInBoogie(&*I) << " := " << printRegNameInBoogie((Value *)I->getOperand(1)) <<  "; } else { " << printRegNameInBoogie(&*I) << " := " << printRegNameInBoogie((Value *)I->getOperand(2)) << "; }\n";
                    break;
                default:
                    errs() << "Error: <Invalid operator>" << I->getOpcodeName() << "\t" << I->getOpcode() << "\n ";
                    I->print(errs());
                    errs() << "\n ";
            }   // end of switch
           
        }                                                               // End of instruction level analysis
        
    }                                     // End of basic block level analysis
}   // end of instrDecoding

// This function is to print variable declaration in Boogie
void varDeclaration(Value *instrVar, std::fstream bpl, bool isbool){
    bpl << "\tvar " << printRegNameInBoogie(instrVar) << ":bv32;\n";
}   // end of varDeclaration

void interpretToBoogie(Module &M){
	printBoogieHeader();

	// search phi instructions for insertion
    anlyzePhiInstr(M);

    // get all pointer values (interprocedural)
    for (auto F = M.begin(); F != M.end(); ++F) {    // Function level
        if (F->size() != 0)                          // ignore all empty functions
        {
        	bpl << "\n// For function: " << static_cast<std::string>((F->getName()));
        	Function *f = &*F;
        	// print function prototypes
        	printFunctionPrototype(f);
            // variable definitions
            printVarDeclarations(f);
            bpl << "\n";
            // decode all instructions
            instrDecoding(f);
        	bpl << "}\n";   // indicate end of function
        }
        else
            errs() << "Function: " << static_cast<std::string>((F->getName())) << "is empty so ignored in Boogie\n";
    }                                                                       // End of function level analysis
    // errs() << "check\n";
    errs() << "\nTransfering to Boogie finished. \n";
}

}
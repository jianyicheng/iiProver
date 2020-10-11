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
#include "llvm/IR/DataLayout.h"
#include "boogieGen.h"

#include <math.h>
#include <regex>
#include <string>
#include <fstream>

using namespace llvm;

std::string output_dir = "./";

namespace boogieGen{

  bool boogieGen::runOnModule(Module &M) {

    pplLoop = NULL;
    
    // JC TODO: features not supported: function calls, 2D arrays, global arrays(TODO)
    // 0. init array accesses
    init(M);

    // 1. array partition analysis
    // programSlice(M);  
    programSlice(M);

    // 2. extract memory accesses
    memoryPatternGen(M);
    
    // 3. extract memory schedule
    extractSchedule(M);

    // 4. generate SMT queries
    mainGen();

    // interpretToBoogie(M);
    
    bpl.close();
    errs() << "Boogie generated successfully. II = "<<II<<"\n";    
    return false;
  }

  void boogieGen::programSlice(Module &M){
    std::vector<Instruction *> depInstr;
    std::vector<Instruction *> revInstr;

    for (auto &F: M) {
      if (F.size() != 0 && strstr(((std::string) F.getName()).c_str(), "ssdm") == NULL){ // ignore all empty functions
        Function *f = &F;
        llvm::DominatorTree* DT = new llvm::DominatorTree();
        DT->recalculate(*f);
        //generate the LoopInfoBase for the current function
        llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>* KLoop = new llvm::LoopInfoBase<llvm::BasicBlock, llvm::Loop>();
        KLoop->releaseMemory();
        KLoop->analyze(*DT);

        for (auto BB = f->begin(); BB != f->end(); ++BB) {  // Basic block level
          for (auto I = BB->begin(); I != BB->end(); ++I) {   // Instruction level
            if (KLoop->isLoopHeader(&*BB)){
              depInstr.push_back(&*I);
              if (isa<BranchInst>(I)){
                std::string pplIIBuff;
                raw_string_ostream string_stream(pplIIBuff);
                dyn_cast<Metadata>(dyn_cast<MDNode>(I->getMetadata("llvm.loop")->getOperand(3))->getOperand(1))->printAsOperand(string_stream);
                int pplII = std::stoi(pplIIBuff.substr(pplIIBuff.find(" ")+1));
                if (pplII != 0){
                  if (pplLoop == NULL){
                    pplLoop = KLoop->getLoopFor(&*BB);
                    II = pplII;
                  }
                  else{
                    errs() << "Multiple loops set to pipelined. It will be supported in the future.\n";
                    assert(0);
                  }
                }
              }
            }
            else if (isa<LoadInst>(I) || isa<StoreInst>(I))
              depInstr.push_back(&*I);
          }
        }

        auto bound = depInstr.size();
        for (int iter = 0; iter < bound; iter++){
          Instruction *instr = depInstr[iter];
          for (int i = 0; i < instr->getNumOperands(); i++){
            if (isa<StoreInst>(instr) && i == 0)
              continue;
            if (Instruction *ii = dyn_cast<Instruction>(instr->getOperand(i))){
              if (std::find(depInstr.begin(), depInstr.end(), ii) == depInstr.end())
                depInstr.push_back(ii);
            }
          }
          bound = depInstr.size();
        }

        for (auto BB = f->begin(); BB != f->end(); ++BB) {  // Basic block level
          for (auto I = BB->begin(); I != BB->end(); ++I) {   // Instruction level   
            Instruction *instr = &*I;
            if (std::find(depInstr.begin(), depInstr.end(), instr) == depInstr.end() && !isa<TerminatorInst>(instr))
              revInstr.push_back(instr);
          }
        }

        for (auto &r: revInstr){
#ifdef IIPROVER_DEBUG
          errs() << "Removing " << *r << "\n";
#endif
          r->replaceAllUsesWith(UndefValue::get(dyn_cast<Value>(r)->getType()));
          r->eraseFromParent();
        }
      }
    }
    if (II == -2){
      errs() << "Error: No loop is to be pipelined.\n";
      assert(0);
    }
  }

  void boogieGen::memoryPatternGen(Module &M){
    phiAnalysis(M);
    for (auto &F: M) {
      if (F.size() != 0 && strstr(((std::string) F.getName()).c_str(), "ssdm") == NULL){ // ignore all empty functions
        bpl << "\n// For function: " << static_cast<std::string>((F.getName()));
        Function *f = &F;
        printFuncPrototype(f);
        printVarDeclarations(f);
        bpl << "\n";
        funcGen(f);
        bpl << "}\n";   // indicate end of function
      }
#ifdef IIPROVER_DEBUG
      else
        errs() << "Function: " << static_cast<std::string>((F.getName())) << "is empty so ignored in Boogie\n";
#endif
    }
  }

  void boogieGen::mainGen(void){
    bpl << "procedure main () {\n";
    for (int i = 0; i < 3; i++)
      bpl << "\tvar label_"<<i<<": bv64;\n\tvar address_"<<i<<": bv64;\n\tvar iteration_"<<i<<": bv64;\n\tvar load_"<<i<<": bool;\n\tvar valid_"<<i<<": bool;\n\tvar array_"<<i<<": bv64;\n\tvar offset_"<<i<<": bv64;\n\tvar start_time_"<<i<<": bv64;\n";
    
    for (auto fi = top->arg_begin(); fi != top->arg_end(); ++fi)
    {

      if (fi->getType()->isPointerTy()){
        if (isa<ArrayType>(dyn_cast<PointerType>(fi->getType())->getElementType())){
          if (isa<IntegerType>(dyn_cast<ArrayType>(dyn_cast<PointerType>(fi->getType())->getElementType())->getArrayElementType()))
            bpl << "\tvar " << printNameInBoogie(fi) << ": [bv64]bv"<< dyn_cast<IntegerType>(dyn_cast<ArrayType>(dyn_cast<PointerType>(fi->getType())->getElementType())->getArrayElementType())->getBitWidth()<<";\n";
          else
            bpl << "\tvar " << printNameInBoogie(fi) << ": [bv64]real;\n";
        }
        else if (isa<IntegerType>(dyn_cast<PointerType>(fi->getType())->getElementType())) // pointer - unverified
          bpl << "\tvar " << printNameInBoogie(fi) << ": [bv64]bv"<<dyn_cast<IntegerType>(dyn_cast<PointerType>(fi->getType())->getElementType())->getBitWidth()<<";\n";
        else
          bpl << "\tvar " << printNameInBoogie(fi) << ": [bv64]real;\n";
      }
      else {
        if (fi->getType()->isIntegerTy())
          bpl << "\tvar " << printNameInBoogie(fi) << ": bv"<<dyn_cast<IntegerType>(fi->getType())->getBitWidth()<<";\n";
        else
          bpl << "\tvar " << printNameInBoogie(fi) << ": real;\n";
      } 
    }
    bpl << "\n\tvar II: bv64;\n\tvar latency_0: bv64;\n\tvar end_time_0:bv64;\n\tII := "<<II<<"bv64;\n";

    for (int i = 0; i < 3; i++){
      bpl << "\tcall label_" << i << ", address_" << i << ", iteration_" << i << ", load_" << i << ", valid_" << i << ", array_" << i << " := " << ((std::string)top->getName()).c_str() << "(";  
      for (auto fi = top->arg_begin(); fi != top->arg_end(); ++fi) {
        bpl << printNameInBoogie(fi);
        auto fi_comma = fi;
        fi_comma++;
        if (fi_comma != top->arg_end())
          bpl << ", ";
      }
      bpl << ");\n";
    }
    bpl << "\tcall latency_0 := getLatency(label_0);\n";

    for (int i = 0; i < 3; i++)
      bpl << "\tcall offset_"<<i<<" := getOffset(label_"<<i<<");\n\tstart_time_" << i << " := bv64add(bv64mul(II, iteration_" << i << "), offset_" << i << ");\n";

    bpl << "\tend_time_0 := bv64add(bv64add(bv64mul(II, iteration_0), offset_0), latency_0);\n";

    bpl << "\tassert (!valid_0 || !valid_1 || address_0 != address_1 || (load_0 && load_1) || label_0 == label_1 || bv64sgt(iteration_0, iteration_1) || bv64sgt(start_time_1, end_time_0));\n";
    bpl << "\tassert (!valid_0 || !valid_1 || !valid_2 || label_0 == label_1 || label_0 == label_2 || label_1 == label_2 || !(start_time_0 == start_time_1 && start_time_1 == start_time_2));\n";

    bpl << "}\n";

  }

  void boogieGen::extractSchedule(Module &M){
    for (auto i = accesses.begin(); i != accesses.end(); ++i){
      memoryNode *mn = *i;
      mn->offset = -1;
      mn->latency = 0;
    }
    std::string file = (std::string)top->getName()+"/proj/soluion1/.autopilot/db/"+(std::string)top->getName()+".verbose.sched.rpt";
    std::ifstream sch(file);
    int st = 0, count = -1;
    if (sch.is_open()){
      std::string line;
      while (std::getline(sch, line)){
        if (line.find("(II) = ") != std::string::npos){
          int tempIdx = line.find("(II) = ")+7;
          while (isdigit(line[tempIdx]))
            tempIdx++;
          std::string s = line.substr(line.find("(II) = ")+7, tempIdx-line.find("(II) = ")-7);
          int reportII = std::stoi(s);
          if (II != -1 && reportII != II){
            errs() << "Mismatched IIs for the given loop.\n";
            assert(0);
          }
          else
            II = reportII;
        }

        if (line.find("<SV = ") != std::string::npos)
          st++;
        if (line.find("ST_") != std::string::npos){
          for (auto i = accesses.begin(); i != accesses.end(); ++i){
            memoryNode *mn = *i;
            if (mn->load){
              if (line.find(" load ") != std::string::npos && line.find(val2str(mn->instr)) != std::string::npos){
                if (mn->offset == -1)
                  mn->offset = st-1;
                mn->latency = mn->latency +1;
              }
            }
            else {
              if (line.find(" store ") != std::string::npos && line.find(val2str(mn->instr->getOperand(1))) != std::string::npos){
                if (mn->offset == -1)
                  mn->offset = st-1;
                mn->latency = mn->latency +1;
              }
            }
          }
        }
      }
    }
    else
      errs() << "Schedule not found.\n";
#ifdef IIPROVER_DEBUG
    errs() << "Found total "<< st <<" states.\n";
#endif
    bpl << "procedure {:inline 1} getOffset(label: bv64) returns (offset:bv64){\n\t";
    for (auto i = accesses.begin(); i != accesses.end(); ++i){
      memoryNode *mn = *i;
      bpl << "if (label == " << mn->label << "bv64){\n\t\toffset := " << mn->offset << "bv64;\n\t}\n\telse ";
    }
    bpl << "{\n\t\toffset := bv64neg(1bv64);\n\t}\n\treturn;\n}\n"; 

    bpl << "procedure {:inline 1} getLatency(label: bv64) returns (latency:bv64){\n\t";
    for (auto i = accesses.begin(); i != accesses.end(); ++i){
      memoryNode *mn = *i;
      bpl << "if (label == " << mn->label << "bv64){\n\t\tlatency := " << mn->latency << "bv64;\n\t}\n\telse ";
    }
    bpl << "{\n\t\tlatency := bv64neg(1bv64);\n\t}\n\treturn;\n}\n"; 

  }

  void boogieGen::phiAnalysis(Module &M)
  {
      for (auto F = M.begin(); F != M.end(); ++F) {   // Function level
          for (auto BB = F->begin(); BB != F->end(); ++BB) {  // Basic block level
              for (auto I = BB->begin(); I != BB->end(); ++I) {   // Instruction level
                if (llvm::PHINode *phiInst = dyn_cast<llvm::PHINode>(&*I)) {
                    phiNode *phi = new phiNode [phiInst->getNumIncomingValues()];    
                    for (unsigned int it = 0; it < phiInst->getNumIncomingValues(); ++it)
                    {
                        phi[it].res = printNameInBoogie(&*I);
                        phi[it].bb = phiInst->getIncomingBlock(it);
                        phi[it].ip = phiInst->getIncomingValue(it);
                        phi[it].instr = phiInst;
                        phis.push_back(&phi[it]);
                    } 
                } 
            } 
          }
      }
#ifdef IIPROVER_DEBUG
      errs() << "Found " << phis.size()/2 << " phi nodes.\n";
#endif
  }

  void boogieGen::printFuncPrototype(Function *F){
      bpl << "\nprocedure {:inline 1} " << static_cast<std::string>((F->getName())) << "(";
      if (!(F->arg_empty()))
      {
        for (auto fi = F->arg_begin(); fi != F->arg_end(); ++fi)
        {
          if (fi->getType()->isPointerTy()){
            if (isa<ArrayType>(dyn_cast<PointerType>(fi->getType())->getElementType()))
              bpl << printNameInBoogie(fi) << ": [bv64]bv"<<dyn_cast<IntegerType>(dyn_cast<ArrayType>(dyn_cast<PointerType>(fi->getType())->getElementType())->getArrayElementType())->getBitWidth();
            else
              bpl << printNameInBoogie(fi) << ": [bv64]real";
          }
          else {
            if (fi->getType()->isIntegerTy())
              bpl << printNameInBoogie(fi) << ": bv"<<dyn_cast<IntegerType>(fi->getType())->getBitWidth();
            else
              bpl << printNameInBoogie(fi) << ": real";
          } 
          auto fi_comma = fi;
          fi_comma++;
          if (fi_comma != F->arg_end())
            bpl << ", ";
        }
      }

      bpl << ") returns (";
      bpl << "label: bv64, address: bv64, iteration: bv64, load: bool, valid: bool, array: bv64";
      bpl << ") \n"; 
      // JC: add global arrays here
      bpl << "{\n";
  }

  void boogieGen::init(Module &M){
    std::string topName = M.getName().substr(0, M.getName().find("/"));
    bpl.open (topName+"/output.bpl", std::fstream::out);
    for (auto& F: M){
      std::string name = demangle(((std::string) F.getName()).c_str());
      name = name.substr(0, name.find("("));
      // errs() << name << "\n";
      if (name == topName)
        top = &F;
    }
    assert(top);
    printBoogieHeader();
  }

  void boogieGen::printBoogieHeader(void) {
    bpl << "\n//*********************************************\n";
    bpl <<   "//    Boogie code generated from LLVM\n";
    bpl <<   "//*********************************************\n";
    bpl << "// Float function prototypes\n";
    bpl << "function {:bvbuiltin \"fp.add\"} fadd(rmode, float24e8,float24e8) returns(float24e8);\n";
    bpl << "function {:bvbuiltin \"fp.sub\"} fsub(rmode, float24e8,float24e8) returns(float24e8);\n";
    bpl << "function {:bvbuiltin \"fp.mul\"} fmul(rmode, float24e8,float24e8) returns(float24e8);\n";
    bpl << "function {:bvbuiltin \"fp.div\"} fdiv(rmode, float24e8,float24e8) returns(float24e8);\n";
    bpl << "function {:bvbuiltin \"fp.rem\"} frem(rmode, float24e8,float24e8) returns(float24e8);\n";
    bpl << "function {:bvbuiltin \"fp.sqrt\"} fsqrt(rmode, float24e8,float24e8) returns(float24e8);\n";
    bpl << "function {:bvbuiltin \"fp.leq\"} fleq(float24e8,float24e8) returns(bool);\n";
    bpl << "function {:bvbuiltin \"fp.geq\"} fgeq(float24e8,float24e8) returns(bool);\n";
    bpl << "function {:bvbuiltin \"fp.gt\"} fgt(float24e8,float24e8) returns(bool);\n";
    bpl << "function {:bvbuiltin \"fp.lt\"} flt(float24e8,float24e8) returns(bool);\n";
    bpl << "function {:bvbuiltin \"fp.eq\"} feq(float24e8,float24e8) returns(bool);\n";
    bpl << "function {:bvbuiltin \"fp.abs\"} fabs(float24e8) returns(float24e8);\n";
    bpl << "function {:bvbuiltin \"fp.neg\"} fneg(float24e8) returns(float24e8);\n";
    bpl << "function {:bvbuiltin \"(_ to_fp 8 24)\"} to_float(real) returns(float24e8);\n";

    bpl << "// Double function prototypes\n";
    bpl << "function {:bvbuiltin \"fp.add\"} dadd(rmode, float53e11,float53e11) returns(float53e11);\n";
    bpl << "function {:bvbuiltin \"fp.sub\"} dsub(rmode, float53e11,float53e11) returns(float53e11);\n";
    bpl << "function {:bvbuiltin \"fp.mul\"} dmul(rmode, float53e11,float53e11) returns(float53e11);\n";
    bpl << "function {:bvbuiltin \"fp.div\"} ddiv(rmode, float53e11,float53e11) returns(float53e11);\n";
    bpl << "function {:bvbuiltin \"fp.rem\"} drem(rmode, float53e11,float53e11) returns(float53e11);\n";
    bpl << "function {:bvbuiltin \"fp.sqrt\"} dsqrt(rmode, float53e11,float53e11) returns(float53e11);\n";
    bpl << "function {:bvbuiltin \"fp.leq\"} dleq(float53e11,float53e11) returns(bool);\n";
    bpl << "function {:bvbuiltin \"fp.geq\"} dgeq(float53e11,float53e11) returns(bool);\n";
    bpl << "function {:bvbuiltin \"fp.gt\"} dgt(float53e11,float53e11) returns(bool);\n";
    bpl << "function {:bvbuiltin \"fp.lt\"} dlt(float53e11,float53e11) returns(bool);\n";
    bpl << "function {:bvbuiltin \"fp.eq\"} deq(float53e11,float53e11) returns(bool);\n";
    bpl << "function {:bvbuiltin \"fp.abs\"} dabs(float53e11) returns(float53e11);\n";
    bpl << "function {:bvbuiltin \"fp.neg\"} dneg(float53e11) returns(float53e11);\n";
    bpl << "function {:bvbuiltin \"(_ to_fp 11 53)\"} to_double(real) returns(float53e11);\n";

    bpl << "// Bit vector function prototypes\n";
    int MIN_BIT = 1;
    int MAX_BIT = 64;
    int step = 1;
    bpl << "// Arithmetic\n";
    for (int i = MIN_BIT; i <= MAX_BIT; i+=step){
      bpl << "function {:bvbuiltin \"bvadd\"} bv"<<i<<"add(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvsub\"} bv"<<i<<"sub(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvmul\"} bv"<<i<<"mul(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvudiv\"} bv"<<i<<"udiv(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvurem\"} bv"<<i<<"urem(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvsdiv\"} bv"<<i<<"sdiv(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvsrem\"} bv"<<i<<"srem(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvsmod\"} bv"<<i<<"smod(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvneg\"} bv"<<i<<"neg(bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"(_ to_fp 8 24)\"} bv"<<i<<"float(rmode, bv"<<i<<") returns(float24e8);\n";
      bpl << "function {:bvbuiltin \"(_ to_fp 8 24) RNA\" }{:ai \"True\" } bv"<<i<<"sfloat(rmode, bv"<<i<<") returns(float24e8);\n";
      bpl << "function {:bvbuiltin \"(_ to_fp 11 53)\"} bv"<<i<<"double(rmode, bv"<<i<<") returns(float53e11);\n";
      bpl << "function {:bvbuiltin \"(_ to_fp 11 53) RNA\" }{:ai \"True\" } bv"<<i<<"sdouble(rmode, bv"<<i<<") returns(float53e11);\n";
      bpl << "function {:bvbuiltin \"(_ fp.to_ubv "<<i<<")\"} float2ubv"<<i<<"(rmode, float24e8) returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"(_ fp.to_sbv "<<i<<")\"} float2sbv"<<i<<"(rmode, float24e8) returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"(_ fp.to_ubv "<<i<<")\"} double2ubv"<<i<<"(rmode, float53e11) returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"(_ fp.to_sbv "<<i<<")\"} double2sbv"<<i<<"(rmode, float53e11) returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"(_ int2bv "<<i<<")\"} int2bv"<<i<<"(int) returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bv2int\"} bv"<<i<<"int(bv"<<i<<") returns(int);\n";
    }
    bpl << "// Bitwise operations\n";
    for (int i = MIN_BIT; i <= MAX_BIT; i+=step){
      bpl << "function {:bvbuiltin \"bvand\"} bv"<<i<<"and(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvor\"} bv"<<i<<"or(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvnot\"} bv"<<i<<"not(bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvxor\"} bv"<<i<<"xor(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvnand\"} bv"<<i<<"nand(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvnor\"} bv"<<i<<"nor(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvxnor\"} bv"<<i<<"xnor(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
    }
    bpl << "// Bit shifting\n";
    for (int i = MIN_BIT; i <= MAX_BIT; i+=step){
      bpl << "function {:bvbuiltin \"bvshl\"} bv"<<i<<"shl(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvlshr\"} bv"<<i<<"lshr(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
      bpl << "function {:bvbuiltin \"bvashr\"} bv"<<i<<"ashr(bv"<<i<<",bv"<<i<<") returns(bv"<<i<<");\n";
    }
    bpl << "// Unsigned comparison\n";
    for (int i = MIN_BIT; i <= MAX_BIT; i+=step){
      bpl << "function {:bvbuiltin \"bvult\"} bv"<<i<<"ult(bv"<<i<<",bv"<<i<<") returns(bool);\n";
      bpl << "function {:bvbuiltin \"bvule\"} bv"<<i<<"ule(bv"<<i<<",bv"<<i<<") returns(bool);\n";
      bpl << "function {:bvbuiltin \"bvugt\"} bv"<<i<<"ugt(bv"<<i<<",bv"<<i<<") returns(bool);\n";
      bpl << "function {:bvbuiltin \"bvuge\"} bv"<<i<<"uge(bv"<<i<<",bv"<<i<<") returns(bool);\n";
    }
    bpl << "// Signed comparison\n";
    for (int i = MIN_BIT; i <= MAX_BIT; i+=step){
      bpl << "function {:bvbuiltin \"bvslt\"} bv"<<i<<"slt(bv"<<i<<",bv"<<i<<") returns(bool);\n";
      bpl << "function {:bvbuiltin \"bvsle\"} bv"<<i<<"sle(bv"<<i<<",bv"<<i<<") returns(bool);\n";
      bpl << "function {:bvbuiltin \"bvsgt\"} bv"<<i<<"sgt(bv"<<i<<",bv"<<i<<") returns(bool);\n";
      bpl << "function {:bvbuiltin \"bvsge\"} bv"<<i<<"sge(bv"<<i<<",bv"<<i<<") returns(bool);\n";
    }
    /*bpl << "// Datatype conversion from bool to bit vector\n";
    for (int i = MIN_BIT; i <= MAX_BIT; i+=step){
      bpl << "procedure {:inline 1} bool2bv"<<i<<" (i: bool) returns ( o: bv"<<i<<")\n";
      bpl << "{\n";
      bpl << "\tif (i == true)\n";
      bpl << "\t{\n";
      bpl << "\t\to := 1bv"<<i<<";\n";
      bpl << "\t}\n";
      bpl << "\telse\n";
      bpl << "\t{\n";
      bpl << "\t\to := 0bv"<<i<<";\n";
      bpl << "\t}\n";
      bpl << "}\n";
    }*/
    bpl << "// Datatype conversion from other bv to bv\n";
    for (int i = MIN_BIT; i <= MAX_BIT; i+=step){
      for (int j = i+1; j <= MAX_BIT; j++){
        bpl << "function {:bvbuiltin \"zero_extend "<<j-i<<"\"} zext.bv"<<i<<".bv"<<j<<"(bv"<<i<<") returns(bv"<<j<<");\n";
        bpl << "function {:bvbuiltin \"sign_extend "<<j-i<<"\"} sext.bv"<<i<<".bv"<<j<<"(bv"<<i<<") returns(bv"<<j<<");\n";
      }
    }

  }
  
  std::string boogieGen::demangle(const char* name)
  {
     int status = -1;
     std::unique_ptr<char, void(*)(void*)> res { abi::__cxa_demangle(name, NULL, NULL, &status), std::free };
     return (status == 0) ? res.get() : std::string(name);
  }

  std::string boogieGen::val2str(Value *I){
    std::string instrResName;
    raw_string_ostream string_stream(instrResName);
    I->printAsOperand(string_stream, false);
    return string_stream.str();
  }

  std::string boogieGen::printNameInBoogie(Value *I){
    std::string instrResName;
    raw_string_ostream string_stream(instrResName);
    I->printAsOperand(string_stream, false);
    instrResName = std::regex_replace(string_stream.str(), std::regex("%"),"$");
    if (ConstantInt *constVar = dyn_cast<ConstantInt>(I))
    {
      int bits = dyn_cast<IntegerType>(I->getType())->getBitWidth();
      if (constVar->isNegative())
        return ("bv"+std::to_string(bits)+"neg("+instrResName.substr(1)+"bv"+std::to_string(bits)+")");
      else if (instrResName == "false")
        return("0bv1");
      else if (instrResName == "true")
        return("1bv1");
      else 
        return(instrResName+"bv"+std::to_string(bits));
    }
    else if (isa<ConstantFP>(I)){
      std::string type = "";
      if (I->getType()->isDoubleTy())
        type = "f53e11";
      else if (I->getType()->isFloatTy())
        type = "f24e8";
      
      if (instrResName.find("0x") != std::string::npos){
        long long int num;
        double f;
        sscanf(instrResName.c_str(), "%llx", &num);
        f = *((double*)&num);
        instrResName = std::to_string(f)+"e0";
      }
      else if (instrResName.find("e") == std::string::npos)
        instrResName += "e0";
      else if (instrResName.find("+") != std::string::npos)
        instrResName.replace(instrResName.find("+"), 1, "0"); // boogie does not support 1e+2
      instrResName = "0x"+instrResName+type;
    }
    else if (instrResName == "undef"){
      assert(isa<IntegerType>(I->getType())); // JC TODO: converting undef to floating point
      int bits = dyn_cast<IntegerType>(I->getType())->getBitWidth();
      instrResName += ".bv"+std::to_string(bits);
    }

    return instrResName;
  }

  void boogieGen::printVarDeclarations(Function *F) {
    std::vector<Value *> vars;
    for (auto BB = F->begin(); BB != F->end(); BB++) {
      for (auto I = BB->begin(); I != BB->end(); I++) { 
        switch (I->getOpcode()) {
          case Instruction::Ret:     // ret
              // no need for var declarations
              break;
              
          case Instruction::Br:     // br
              if (I->getNumOperands() == 3)
              {
                  if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                      varDeclaration((Value *)(I->getOperand(0)));
              }
              break;
              
          case Instruction::Add:     // add
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;

          case Instruction::Alloca:     // alloca
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              break;

          case Instruction::FMul:     // fmul
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;
          
          case Instruction::FAdd:     // fadd
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;

          case Instruction::Sub:     // sub
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;
          case Instruction::Mul:     // mul
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;
          case Instruction::Shl:     // shl
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;
          case Instruction::LShr:     // lshr
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;
          case Instruction::AShr:     // ashr
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;
          case Instruction::And:     // and
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;
          case Instruction::Or:     // or
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;
          
          case Instruction::Load:     // load
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              break;
              
          case Instruction::Store:     // store
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;
          
          case Instruction::GetElementPtr:     // getelementptr                               // this is what we want to modify
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);     // leave output random
              break;
              
          case Instruction::ZExt:     // zext
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              break;

          case Instruction::SExt:     // sext
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              break;
          case Instruction::Trunc:     // trunc
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              break;
          case Instruction::BitCast:     // BitCast
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              break;

          case Instruction::ICmp:    // icmp
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;

          case Instruction::FCmp:    // fcmp
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;

          case Instruction::PHI:     // phi
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              break;
          case Instruction::Call:     // call
              break;
          case Instruction::Select:     // select
              if (!varFoundInList(&*I, &vars, F))
                  varDeclaration(&*I);
              if (!varFoundInList((Value *)(I->getOperand(0)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(0)));
              if (!varFoundInList((Value *)(I->getOperand(1)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(1)));
              if (!varFoundInList((Value *)(I->getOperand(2)), &vars, F))
                  varDeclaration((Value *)(I->getOperand(2)));
              break;
              
          default:
            errs() << "Declaring: Found unkown instruction: " << *I << "\n";
        }
      }
    }
    bpl << "\tvar boogie_fp_mode : rmode;\n\tboogie_fp_mode := RNE;\n\tvalid := false;\n";
  }

  bool boogieGen::varFoundInList(Value *var, std::vector<Value *> *vars, Function *F){
    bool varFound = false;
  
    // check if it is a constant
    if (dyn_cast<llvm::ConstantInt>(var) || dyn_cast<llvm::ConstantFP>(var))
        return true;
    
    bool isArg = false;
    for (auto fi = F->arg_begin(); fi != F->arg_end(); ++fi)
    {
        Value *arg = fi;
        if (var == arg)
            return true;
    }
    if (!isArg)
    {
      for (auto itVar = vars->begin(); itVar != vars->end(); ++itVar)
      {
        Value *itvar = *itVar;
        if (itvar == var)
          varFound = true;
      }
      if (!varFound)
        vars->push_back(var);
    }
    else
        varFound = true;
    return varFound;
  }

  void boogieGen::varDeclaration(Value *var){
    // if (printNameInBoogie(var) != "undef"){
      if (var->getType()->isIntegerTy()){
        if (var->getType()->isPointerTy())
          bpl << "\tvar " << printNameInBoogie(var) << ": [bv64]bv32;\n"; // todo: getElementType()
        else
          bpl << "\tvar " << printNameInBoogie(var)  << " : bv" <<dyn_cast<IntegerType>(var->getType())->getBitWidth()<<";\n";
      }
      else if (var->getType()->isPointerTy())
        bpl << "\tvar " << printNameInBoogie(var) << " : bv" <<dyn_cast<IntegerType>(dyn_cast<PointerType>(var->getType())->getElementType())->getBitWidth()<<";\n";
      else if (var->getType()->isFloatTy() || var->getType()->isDoubleTy())
        bpl << "\tvar " << printNameInBoogie(var) << ": float53e11;\n"; // float = float24e8
    // }
  }

  std::string boogieGen::getBlockLabel(BasicBlock *BB){
    std::string block_address;
    raw_string_ostream string_stream(block_address);
    BB->printAsOperand(string_stream, false);
      
    std::string temp = string_stream.str();
      
    for (unsigned int i = 0; i<temp.length(); ++i)
    {
      if (temp[i] == '-')
        temp.replace(i, 1, "_");
    }  
    return std::regex_replace(temp.c_str(), std::regex("%"),"$");
  }

  void boogieGen::funcGen(Function *F){
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
          for (auto I = BB->begin(); I != BB->end(); I++){
            if (llvm::PHINode *phiInst = dyn_cast<llvm::PHINode>(I)){
              invariance *invar = new invariance;
              std::string startSign, endSign, startBound, endBound;
              invar->loop = KLoop->getLoopFor(&*BB);
              int incr = -1;
              int eq = -1;
              for (auto ii = BB->begin(); ii != BB->end(); ii++){
                if (CmpInst *cmpInst = dyn_cast<CmpInst>(&*ii)){ // JC: depends Vitis HLS - need check
                  if (cmpInst->getPredicate() == CmpInst::ICMP_EQ || cmpInst->getPredicate() == CmpInst::ICMP_NE || cmpInst->getPredicate() == CmpInst::ICMP_UGT || cmpInst->getPredicate() == CmpInst::ICMP_ULT || cmpInst->getPredicate() == CmpInst::ICMP_SGT || cmpInst->getPredicate() == CmpInst::ICMP_SLT)
                    eq = 0;
                  else if (cmpInst->getPredicate() == CmpInst::ICMP_UGE || cmpInst->getPredicate() == CmpInst::ICMP_ULE || cmpInst->getPredicate() == CmpInst::ICMP_SGE || cmpInst->getPredicate() == CmpInst::ICMP_SLE)
                    eq = 1;
                  endBound = printNameInBoogie(cmpInst->getOperand(1));
                }
                else if (ii->getOpcode() == Instruction::Add){
                  if (ii->getOperand(0) == &*I || ii->getOperand(1) == &*I)
                    incr = 1;
                }
                else if (ii->getOpcode() == Instruction::Sub){
                  if (ii->getOperand(0) == &*I || ii->getOperand(1) == &*I)
                    incr = 0;
                }
              }
              assert(incr != -1 && eq != -1);

              std::string bits = "";
              if (IntegerType* temp = dyn_cast<IntegerType>(I->getType()))
                bits = std::to_string(temp->getBitWidth());

              if (incr){
                startSign = "bv"+bits+"uge(";
                if (eq)
                    endSign = "bv"+bits+"ule(";
                else
                    endSign = "bv"+bits+"ult(";
              }
              else {
                startSign = "bv"+bits+"ule(";
                if (eq)
                    endSign = "bv"+bits+"uge(";
                else
                    endSign = "bv"+bits+"ugt(";
              }
              
              invar->instr = phiInst;
              for (unsigned int it = 0; it < phiInst->getNumIncomingValues(); ++it)
              {
                if (ConstantInt *constVar = dyn_cast<ConstantInt>(phiInst->getIncomingValue(it)))
                  startBound = printNameInBoogie(constVar);
              } // end for
               
              invar->invar = startSign + printNameInBoogie(&*I) + "," + startBound + ") && " + endSign + printNameInBoogie(&*I) + "," + endBound + ")";
              invariances.push_back(invar);
              bpl << "\tassert ( " << invar->invar << ");\n";
              bpl << "\thavoc " << printNameInBoogie(&*I) << ";\n";
              bpl << "\tassume ( " << invar->invar << ");\n";
            }
          }            
        }   // end of insert loop invariants in the beginning
        
        std::string callName;
        int search;
        memoryNode *mn;
        llvm::PHINode *loopiter;
        std::string ftype;
        // Start instruction printing
        for (auto I = BB->begin(); I != BB->end(); ++I) {                   // Instruction level            
            switch (I->getOpcode()) {
                case Instruction::Ret:     // ret
                    bpl << "\treturn;\n";  //default return - memory not accessed.
                    break;
                    
                case Instruction::Br:     // br
                    // loop invariant at exit
                    for (auto it = invariances.begin(); it != invariances.end(); ++it){
                      invariance *in = *it;
                      llvm::PHINode *inst = in->instr;
                      for (unsigned int t = 0; t < inst->getNumIncomingValues(); ++t){
                        if (!isa<ConstantInt>(inst->getIncomingValue(t)) && inst->getIncomingBlock(t) == &*BB){
                          bpl << "\tassert(" << in->invar << ");\n";
                          bpl << "\treturn;\n\tassume false;\n";
                        }
                      }
                    }
                    // do phi resolution here
                    for (auto it = phis.begin(); it != phis.end(); ++it)
                    {
                        phiNode *phiTfInst = *it;
                        if (phiTfInst->bb == &*BB)
                            bpl << "\t" << phiTfInst->res << " := " << printNameInBoogie(phiTfInst->ip) << ";\n";
                    }
                    if (I->getNumOperands() == 1) // if (inst->isConditional())?
                        bpl << "\tgoto bb_"<< getBlockLabel((BasicBlock *)(I->getOperand(0))) << ";\n";
                    else if (I->getNumOperands() == 3)
                        bpl << "\tif(" << printNameInBoogie((Value *)I->getOperand(0)) << " == 1bv1) {goto bb_"<< getBlockLabel((BasicBlock *)(I->getOperand(2))) << ";} else {goto bb_" << getBlockLabel((BasicBlock *)(I->getOperand(1))) << ";}\n";
                    else
                        errs() << "Error: Instruction decoding error at br instruction: " << *I << "\n";
                    break;
                    
                case Instruction::Add:     // add
                    if (OverflowingBinaryOperator *op = dyn_cast<OverflowingBinaryOperator>(I)) {
                        if ((op->hasNoUnsignedWrap()) && (op->hasNoSignedWrap()))
                            // has both nuw and nsw
                            bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"add("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else if (op->hasNoUnsignedWrap())
                            // only nuw
                            bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"add("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else if (op->hasNoSignedWrap())
                            // only nsw
                            bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"add("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else
                            // normal add
                            bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"add("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    }
                    else
                        errs() << "Error: Instruction decoding error at add instruction: " << *I << "\n";
                    break;

                case Instruction::FAdd:     // fadd
                    if (I->getType()->isDoubleTy())
                      bpl << "\t" << printNameInBoogie(&*I) << " := dadd(boogie_fp_mode, "<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    else
                      bpl << "\t" << printNameInBoogie(&*I) << " := fadd(boogie_fp_mode, "<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;

                case Instruction::FMul:     // fmul
                    if (I->getType()->isDoubleTy())
                      bpl << "\t" << printNameInBoogie(&*I) << " := dmul(boogie_fp_mode, "<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    else
                      bpl << "\t" << printNameInBoogie(&*I) << " := fmul(boogie_fp_mode, "<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                    
                case Instruction::Sub:     // sub
                    if (OverflowingBinaryOperator *op = dyn_cast<OverflowingBinaryOperator>(I)) {
                        if ((op->hasNoUnsignedWrap()) && (op->hasNoSignedWrap()))
                            // has both nuw and nsw
                            bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"sub("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else if (op->hasNoUnsignedWrap())
                            // only nuw
                            bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"sub("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else if (op->hasNoSignedWrap())
                            // only nsw
                            bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"sub("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";       // ---might has problems...
                        else
                            // normal add
                            bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"sub("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    }
                    else
                        errs() << "Error: Instruction decoding error at sub instruction: " << *I << "\n";
                    break;

                case Instruction::BitCast:
                    if (I->getType()->isIntegerTy()){
                      if (I->getOperand(0)->getType()->isDoubleTy())
                        bpl << "\t" << printNameInBoogie(&*I) << " := double2ubv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() << "(boogie_fp_mode, " << printNameInBoogie((Value *)I->getOperand(0)) << ");\n";
                      else if (I->getOperand(0)->getType()->isFloatTy())
                        bpl << "\t" << printNameInBoogie(&*I) << " := float2ubv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() << "(boogie_fp_mode, " << printNameInBoogie((Value *)I->getOperand(0)) << ");\n";
                      else
                        bpl << "\t" << printNameInBoogie(&*I) << " := " << printNameInBoogie((Value *)I->getOperand(0)) << ";\n";
                    }
                    else if (I->getOperand(0)->getType()->isIntegerTy()){
                      if (I->getType()->isDoubleTy())
                        bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getOperand(0)->getType())->getBitWidth() << "double(boogie_fp_mode, " << printNameInBoogie((Value *)I->getOperand(0)) << ");\n";                        
                      else if (I->getType()->isFloatTy())
                        bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getOperand(0)->getType())->getBitWidth() << "float(boogie_fp_mode, " << printNameInBoogie((Value *)I->getOperand(0)) << ");\n"; 
                      else
                        bpl << "\t" << printNameInBoogie(&*I) << " := " << printNameInBoogie((Value *)I->getOperand(0)) << ";\n";
                    }
                    else
                      bpl << "\t" << printNameInBoogie(&*I) << " := " << printNameInBoogie((Value *)I->getOperand(0)) << ";\n";
                    
                    break;
                    
                case Instruction::Mul:     // mul
                    bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"mul("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                case Instruction::Shl:     // shl
                    bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"shl("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                case Instruction::LShr:     // lshr
                    bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"lshr("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                    
                case Instruction::AShr:     // ashr
                    bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"ashr("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                case Instruction::And:     // and
                    bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"and("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                case Instruction::Or:     // or
                    bpl << "\t" << printNameInBoogie(&*I) << " := bv"<< dyn_cast<IntegerType>(I->getType())->getBitWidth() <<"or("<< printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ");\n";
                    break;
                case Instruction::Load:     // load
                    // JC: add your if(*) here
                    bpl << "//\t" << printNameInBoogie(&*I) << " := "<< printNameInBoogie((Value *)dyn_cast<Instruction>(I->getOperand(0))->getOperand(0)) << "[" << printNameInBoogie((Value *)dyn_cast<Instruction>(I->getOperand(0))->getOperand(1)) << "];\n";
                    bpl << "\t havoc " << printNameInBoogie(&*I) << ";\n"; 
                    if (pplLoop->contains(&*BB)){
                      mn = new memoryNode;
                      mn->load = true;
                      mn->instr = &*I;
                      mn->label = accesses.size();
                      accesses.push_back(mn);
                      for (auto k = invariances.begin(); k != invariances.end(); k++){
                        invariance *in = *k;
                        if (in->loop == KLoop->getLoopFor(I->getParent()))
                          loopiter = in->instr;
                      }
                      search = -1;
                      for (auto k = arrays.begin(); k != arrays.end(); k++){
                        Value *an = *k;
                        if ((Value *)(dyn_cast<Instruction>(I->getOperand(0))->getOperand(0)) == an)
                          search = k-arrays.begin();
                      }
                      if (search == -1){
                        search = arrays.size();
                        arrays.push_back((Value *)(dyn_cast<Instruction>(I->getOperand(0))->getOperand(0)));
                      }
                      if (dyn_cast<IntegerType>(loopiter->getType())->getBitWidth() == 64)
                        bpl << "\tif(*){\n\t\tlabel := "<< mn->label <<"bv64;\n\t\taddress := "<< printNameInBoogie((Value *)dyn_cast<Instruction>(I->getOperand(0))->getOperand(2)) <<";\n\t\titeration := " << printNameInBoogie(loopiter) << ";\n\t\tload := true;\n\t\tvalid := true;\n\t\tarray := " << search << "bv64;\n\t\treturn;\n\t}\n";
                      else
                        bpl << "\tif(*){\n\t\tlabel := "<< mn->label <<"bv64;\n\t\taddress := "<< printNameInBoogie((Value *)dyn_cast<Instruction>(I->getOperand(0))->getOperand(2)) <<";\n\t\titeration := zext.bv" << dyn_cast<IntegerType>(loopiter->getType())->getBitWidth() << ".bv64(" << printNameInBoogie(loopiter) << ");\n\t\tload := true;\n\t\tvalid := true;\n\t\tarray := " << search << "bv64;\n\t\treturn;\n\t}\n";
                    }
                    break;
                    
                case Instruction::Store:    // store
                    // JC: add your if(*) here
                    bpl << "//\t" << printNameInBoogie((Value *)dyn_cast<Instruction>(I->getOperand(1))->getOperand(0)) << "[" << printNameInBoogie((Value *)dyn_cast<Instruction>(I->getOperand(1))->getOperand(2)) << "] := "<< printNameInBoogie((Value *)I->getOperand(0)) << ";\n";
                    if (pplLoop->contains(&*BB)){
                      mn = new memoryNode;
                      mn->load = false;
                      mn->instr = &*I;
                      mn->label = accesses.size();
                      accesses.push_back(mn);
                      for (auto k = invariances.begin(); k != invariances.end(); k++){
                        invariance *in = *k;
                        if (in->loop == KLoop->getLoopFor(I->getParent()))
                          loopiter = in->instr;
                      }
                      search = -1;
                      for (auto k = arrays.begin(); k != arrays.end(); k++){
                        Value *an = *k;
                        if ((Value *)(dyn_cast<Instruction>(I->getOperand(1))->getOperand(0)) == an)
                          search = k - arrays.begin();
                      }
                      if (search == -1){
                        search = arrays.size();
                        arrays.push_back((Value *)dyn_cast<Instruction>(I->getOperand(1))->getOperand(0));
                      }
                      if (dyn_cast<IntegerType>(loopiter->getType())->getBitWidth() == 64)
                        bpl << "\tif(*){\n\t\tlabel := "<< mn->label <<"bv64;\n\t\taddress := "<< printNameInBoogie((Value *)dyn_cast<Instruction>(I->getOperand(1))->getOperand(2)) <<";\n\t\titeration := " << printNameInBoogie(loopiter) << ";\n\t\tload := false;\n\t\tvalid := true;\n\t\tarray := " << search << "bv64;\n\t\treturn;\n\t}\n";
                      else
                        bpl << "\tif(*){\n\t\tlabel := "<< mn->label <<"bv64;\n\t\taddress := "<< printNameInBoogie((Value *)dyn_cast<Instruction>(I->getOperand(1))->getOperand(2)) <<";\n\t\titeration := zext.bv" << dyn_cast<IntegerType>(loopiter->getType())->getBitWidth() << ".bv64(" << printNameInBoogie(loopiter) << ");\n\t\tload := false;\n\t\tvalid := true;\n\t\tarray := " << search << "bv64;\n\t\treturn;\n\t}\n";
                    }
                    break;
                case Instruction::GetElementPtr:     // getelementptr                               // this can be ignored
                    break;
                    
                case Instruction::Trunc:     // trunc
                    assert(isa<IntegerType>(I->getType()));
                    assert(isa<IntegerType>(I->getOperand(0)->getType()));
                    bpl << "\t" << printNameInBoogie(&*I) << " := " << printNameInBoogie((Value *)I->getOperand(0)) << "["<<dyn_cast<IntegerType>(I->getType())->getBitWidth()<<":0];\n";
                    break;
                    
                case Instruction::ZExt:     // zext
                    bpl << "\t" << printNameInBoogie(&*I) << " := zext.bv" << dyn_cast<IntegerType>(I->getOperand(0)->getType())->getBitWidth() << ".bv" << dyn_cast<IntegerType>(I->getType())->getBitWidth() << "(" << printNameInBoogie((Value *)I->getOperand(0)) << ");\n";
                    break;

                case Instruction::SExt:     // sext
                    errs() << "Undefined SEXT instruction " << *I << "\n";
                    break;
                    
                case Instruction::ICmp:    // icmp
                    if (CmpInst *cmpInst = dyn_cast<CmpInst>(&*I)) {
                        if (cmpInst->getPredicate() == CmpInst::ICMP_EQ) {
                            bpl << "\tif (" << printNameInBoogie((Value *)I->getOperand(0)) << " == " << printNameInBoogie((Value *)I->getOperand(1)) << ") { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_NE) {
                            bpl << "\tif (" << printNameInBoogie((Value *)I->getOperand(0)) << " != " << printNameInBoogie((Value *)I->getOperand(1)) << ") { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_UGT) {
                            bpl << "\tif (bv1ugt(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_UGE) {
                            bpl << "\tif (bv1uge(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_ULT) {
                            bpl << "\tif (bv1ult(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_ULE) {
                            bpl << "\tif (bv1ule(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_SGT) {
                            bpl << "\tif (bv1sgt(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_SGE) {
                            bpl << "\tif (bv1sge(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_SLT) {
                            bpl << "\tif (bv1slt(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else if (cmpInst->getPredicate() == CmpInst::ICMP_SLE) {
                            bpl << "\tif (bv1sle(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ") == true) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else
                            errs() << "Error: Instruction decoding error at icmp instruction: " << *I << "\n";
                    }
                    break;
                case Instruction::FCmp:    // fcmp
                    ftype = (I->getOperand(0)->getType()->isDoubleTy())?"d":"f";
                    if (FCmpInst *cmpInst = dyn_cast<FCmpInst>(&*I)) {
                        if (cmpInst->getPredicate() == FCmpInst::FCMP_OEQ) {
                            bpl << "\tif (" << printNameInBoogie((Value *)I->getOperand(0)) << " == " << printNameInBoogie((Value *)I->getOperand(1)) << ") { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else if (cmpInst->getPredicate() == FCmpInst::FCMP_ONE) {
                            bpl << "\tif (" << printNameInBoogie((Value *)I->getOperand(0)) << " != " << printNameInBoogie((Value *)I->getOperand(1)) << ") { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else if (cmpInst->getPredicate() == FCmpInst::FCMP_UGT) {
                            bpl << "\tif (" << ftype << "gt(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ")) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == FCmpInst::FCMP_UGE) {
                            bpl << "\tif (" << ftype << "ge(" <<  printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ")) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == FCmpInst::FCMP_ULT) {
                            bpl << "\tif (" <<  ftype << "lt(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ")) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == FCmpInst::FCMP_ULE) {
                            bpl << "\tif (" <<  ftype << "le(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ")) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n"; // ---might has problems...
                        }
                        else if (cmpInst->getPredicate() == FCmpInst::FCMP_OGT) {
                            bpl << "\tif (" <<  ftype << "gt(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ")) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else if (cmpInst->getPredicate() == FCmpInst::FCMP_OGE) {
                            bpl << "\tif (" <<  ftype << "ge(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ")) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else if (cmpInst->getPredicate() == FCmpInst::FCMP_OLT) {
                            bpl << "\tif (" <<  ftype << "lt(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ")) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else if (cmpInst->getPredicate() == FCmpInst::FCMP_OLE) {
                            bpl << "\tif (" <<  ftype << "le(" << printNameInBoogie((Value *)I->getOperand(0)) << ", " << printNameInBoogie((Value *)I->getOperand(1)) << ")) { " << printNameInBoogie(&*I) << " := 1bv1; } else { " << printNameInBoogie(&*I) << " := 0bv1; }\n";
                        }
                        else
                            errs() << "Error: Instruction decoding error at icmp instruction: " << *I << "\n";
                    }
                    break;

                case Instruction::PHI:     // phi
                    // Has been done in previous section
                    break;
                case Instruction::Call:     // call
                    callName = I->getOperand(I->getNumOperands()-1)->getName();
                    if (strstr(callName.c_str(), "ssdm") != NULL){ // vitis predefined functions
                      if (strstr(callName.c_str(), "PartSelect") != NULL){
                        // bpl << "\tif (" << printNameInBoogie((Value *)I->getOperand(0)) << " == 1bv32) { " << printNameInBoogie(&*I) << " := " << printNameInBoogie((Value *)I->getOperand(1)) <<  "; } else { " << printNameInBoogie(&*I) << " := " << printNameInBoogie((Value *)I->getOperand(2)) << "; }\n";
                        assert(isa<IntegerType>(I->getType()));
                        assert(isa<IntegerType>(I->getOperand(0)->getType()));
                        assert(isa<ConstantInt>(I->getOperand(1)));
                        assert(isa<ConstantInt>(I->getOperand(2)));
                        int bwRes = extractNum(callName.c_str(), "_ssdm_op_PartSelect.i");  // result bw
                        int bw0 = dyn_cast<IntegerType>(I->getOperand(0)->getType())->getBitWidth();
                        std::string temp = printNameInBoogie(I->getOperand(1));
                        int b1 = std::stoi(temp.substr(0, temp.find("bv")));
                        temp = printNameInBoogie(I->getOperand(2));
                        int b2 = std::stoi(temp.substr(0, temp.find("bv")));
                        assert(bwRes == abs(b1-b2)+1);
                        if (b1 < b2) // forward mode
                          bpl << "\t" << printNameInBoogie(&*I) << " := " << "(bv"<<bw0<<"and("<<(1 << (b1-b2+2))-1 << "bv" << bw0 << ", bv" << bw0 << "lshr(" <<printNameInBoogie((Value *)(I->getOperand(0))) << "," << b1 << "bv" << bw0 <<")))["<<bwRes<<":0];\n";
                        else
                          errs() << "Undefined instruction found." << *I << "\n";
                      }
                      else if (strstr(callName.c_str(), "SpecPipeline") != NULL || strstr(callName.c_str(), "SpecTopModule")  != NULL || strstr(callName.c_str(), "SpecLoopTripCount")  != NULL || strstr(callName.c_str(), "SpecLoopName")  != NULL || strstr(callName.c_str(), "SpecBitsMap")  != NULL)
                        ;
                      else if (strstr(callName.c_str(), "BitConcatenate") != NULL){
                        int bwRes = extractNum(callName.c_str(), "_ssdm_op_BitConcatenate.i");
                        assert(isa<IntegerType>(I->getOperand(0)->getType()));
                        assert(isa<IntegerType>(I->getOperand(1)->getType()));
                        int bw0 = dyn_cast<IntegerType>(I->getOperand(0)->getType())->getBitWidth();
                        int bw1 = dyn_cast<IntegerType>(I->getOperand(1)->getType())->getBitWidth();
                        assert (bw0+bw1 == bwRes);
                        bpl << "\t" << printNameInBoogie(&*I) << " := bv"<<bwRes<<"or(bv"<<bwRes<<"shl(zext.bv" << bw0 << ".bv"<<bwRes<<"("<< printNameInBoogie((Value *)I->getOperand(0)) << "), " << bw1 << "bv"<<bwRes<<"), zext.bv" << bw1 << ".bv"<<bwRes<<"(" << printNameInBoogie((Value *)I->getOperand(1)) << "));\n";
                      }
                      else
                        errs() << "Error: Call functions found in the function: " << *I << "\n";
                    }
                    else if (dyn_cast<Function>(I->getOperand(I->getNumOperands()-1))->size() > 0)
                      errs() << "Error: Call functions found in the function: " << *I << "\n";
                    break;
                case Instruction::Select:     // select
                    bpl << "\tif (" << printNameInBoogie((Value *)I->getOperand(0)) << " == 1bv64) { " << printNameInBoogie(&*I) << " := " << printNameInBoogie((Value *)I->getOperand(1)) <<  "; } else { " << printNameInBoogie(&*I) << " := " << printNameInBoogie((Value *)I->getOperand(2)) << "; }\n";
                    break;
                default:
                    errs() << "Decoding: Found unkown instruction: " << *I << "\n";
            }   // end of switch
           
        }                                                               // End of instruction level analysis
        
    } 
  }

  int boogieGen::extractNum(std::string line, std::string substr){
    if (line.find(substr) == std::string::npos){
      std::string s = "Missing constraints " + substr + " in " + line +".";
      errs() << s << "\n";
      assert(0);
    }
    else{
      int tempIdx = line.find(substr)+substr.length();
      while (isdigit(line[tempIdx]))
        tempIdx++;
      std::string s = line.substr(line.find(substr)+substr.length(), tempIdx-line.find(substr)-substr.length());
      if (!s.empty() && std::all_of(s.begin(), s.end(), ::isdigit))
        return std::stoi(s);
      else{
        std::string s = "Invalid constraint extracted for " + substr + " in " + line;
        errs() << s << "\n";
        assert(0);
      }
    }
    return -1;
  }  
}




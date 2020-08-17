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
#include "boogieGen.h"

#include <regex>
#include <string>
#include <fstream>

using namespace llvm;

std::string output_dir = "./";

namespace {

  struct boogieGen : public ModulePass {
    static char ID; 
    boogieGen() : ModulePass(ID) {}
    bool runOnModule(Module &M) override {
      
      bpl.open ("output.bpl", std::fstream::out);
      interpretToBoogie(M);
      bpl.close();
      errs() << "Boogie generated successfully.\n";    
      return false;
    }
  };
}

char boogieGen::ID = 0;
static RegisterPass<boogieGen> X("boogieGen", "Boogie program generator from C code");


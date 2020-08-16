//===- Renamer.cpp - Loop Learn Pass --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Rename all the function arguments.
//
//===----------------------------------------------------------------------===//

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"

using namespace llvm;

struct Renamer : public FunctionPass {
  static char ID;
  Renamer() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override;
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.setPreservesAll();
  }
};

char Renamer::ID = 0;
static RegisterPass<Renamer> X("renamer", "Rename Function arguments",
                               false /* Only looks at CFG */,
                               true /* Transformation Pass */);

bool Renamer::runOnFunction(Function &F) {
  if (skipFunction(F))
    return false;

  for (auto &A : F.args()) {
    std::string Name = A.getName();
    A.setName(Name + "boba");
  }

  return true;
}

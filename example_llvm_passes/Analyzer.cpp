//===--- Analyzer.cpp ---------------- Loop Learn Pass --------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// Analyze pipelined loops memory accesses patterns using Scalar Evolution.
//
//===----------------------------------------------------------------------===//

#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

struct Analyzer : public FunctionPass {
  static char ID;
  Analyzer() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override;
  void getAnalysisUsage(AnalysisUsage &AU) const override {
    AU.addRequired<LoopInfoWrapperPass>();
    AU.addRequired<ScalarEvolutionWrapperPass>();
    AU.setPreservesAll();
  }
};

char Analyzer::ID = 0;
static RegisterPass<Analyzer> X("analyzer", "Analyze access patterns",
                                false /* Only looks at CFG */,
                                false /* Transformation Pass */);

static MDNode *GetLoopMetadata(const Loop *L, StringRef Attr) {
  MDNode *LoopID = L->getLoopID();
  if (!LoopID)
    return nullptr;

  assert(LoopID->getNumOperands() > 0 && "Loop ID needs at least one operand");
  assert(LoopID->getOperand(0) == LoopID && "Loop ID should refer to itself");

  for (unsigned i = 1, e = LoopID->getNumOperands(); i < e; ++i) {
    MDNode *MD = dyn_cast<MDNode>(LoopID->getOperand(i));
    if (!MD)
      continue;

    MDString *S = dyn_cast<MDString>(MD->getOperand(0));
    if (!S)
      continue;

    if (Attr.equals(S->getString()))
      return MD;
  }
  return nullptr;
}

static bool IsPipelined(const Loop *L) {
  if (MDNode *MD = GetLoopMetadata(L, "llvm.loop.pipeline.enable")) {
    auto II = mdconst::extract<ConstantInt>(MD->getOperand(1))->getZExtValue();
    if (II != 0)
      return true;
  }

  // II ==0 implies pipeline off
  return false;
}

bool Analyzer::runOnFunction(Function &F) {
  if (skipFunction(F))
    return false;

  auto &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
  auto &SE = getAnalysis<ScalarEvolutionWrapperPass>().getSE();

  // Iterate over top level loops
  for (auto L : LI)
    if (IsPipelined(L)) {

      // Print the predicted loop trip count
      L->print(dbgs());
      dbgs() << "\tBackedge Taken Count: " << *SE.getBackedgeTakenCount(L)
             << "\n";

      for (auto *BB : L->blocks())
        for (auto &I : *BB) {
          if (auto *Ld = dyn_cast<LoadInst>(&I)) {
            // Print the load access pattern
            Ld->print(dbgs());
            dbgs() << "\n\tAccess Pattern: "
                   << *SE.getSCEV(Ld->getPointerOperand()) << "\n";
          }

          if (auto *St = dyn_cast<StoreInst>(&I)) {
            // Print the store access pattern
            St->print(dbgs());
            dbgs() << "\n\tAccess Pattern: "
                   << *SE.getSCEV(St->getPointerOperand()) << "\n";
          }
        }
    }

  // We didn't modify the IR (analysis only)
  return false;
}

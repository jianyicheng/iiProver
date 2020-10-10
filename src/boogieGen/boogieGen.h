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

namespace boogieGen{
    class boogieGen : public ModulePass {
    public:
        static char ID; 
        boogieGen() : ModulePass(ID) {}
        bool runOnModule(Module &M) override;

    private:
        struct invariance {
            Loop *loop;
            llvm::PHINode *instr;
            std::string invar;
        };
        struct phiNode {
            BasicBlock *bb;
            std::string res;
            Value *ip;
            Instruction* instr;
        };
        struct memoryNode {
            bool load;
            int label;
            Instruction *instr;
            int offset;
            int latency;
        };
        std::fstream bpl;   // outout Boogie program
        Loop *pplLoop = NULL;
        Function *top = NULL;
        int II = -2;
        std::vector<Value *> arrays;
        std::vector<memoryNode *> accesses;
        std::vector<phiNode *> phis;
        std::vector<invariance *> invariances;
        void init(Module &);
        std::string demangle(const char*);
        void printBoogieHeader(void);
        std::string val2str(Value *);
        void memoryPatternGen(Module &);
        void extractSchedule(Module &);
        std::string printNameInBoogie(Value *);
        void printFuncPrototype(Function *);
        void printVarDeclarations(Function *);
        bool varFoundInList(Value *, std::vector<Value *> *, Function *);
        void varDeclaration(Value *);
        std::string getBlockLabel(BasicBlock *);
        void loopInvarCheck(Function *);
        void phiAnalysis(Module &);
        void funcGen(Function *);
        void mainGen(void);
        int extractNum(std::string, std::string);
        void programSlice(Module &);
    };
    char boogieGen::ID = 0;
    static RegisterPass<boogieGen> X("boogieGen", "Boogie program generator from C code");

}



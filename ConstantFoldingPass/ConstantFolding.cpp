// const-fold-binop.cpp
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Analysis/ConstantFolding.h"   // ConstantFoldBinaryOpOperands
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/Pass.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

struct ConstFoldBinOpPass : public PassInfoMixin<ConstFoldBinOpPass> {
  PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    bool Changed = false;
    const DataLayout &DL = F.getParent()->getDataLayout();

    // Iterate basic blocks and instructions safely while possibly erasing
    for (BasicBlock &BB : F) {
      for (auto InstIt = BB.begin(); InstIt != BB.end();) {
        Instruction *I = &*InstIt++;
        // Only handle binary ops
        if (auto *BO = dyn_cast<BinaryOperator>(I)) {
          // Only fold when both operands are ConstantInt
          if (auto *C1 = dyn_cast<ConstantInt>(BO->getOperand(0))) {
            if (auto *C2 = dyn_cast<ConstantInt>(BO->getOperand(1))) {
              // Attempt to constant-fold this binary op
              if (Constant *Folded =
                      ConstantFoldBinaryOpOperands(BO->getOpcode(), C1, C2, DL)) {
                // Replace uses and erase old instruction
                BO->replaceAllUsesWith(Folded);
                BO->eraseFromParent();
                Changed = true;
                // continue with next instruction (we already advanced InstIt)
                continue;
              }
            }
          }
        }
      }
    }

    return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
  }
};

} // end anonymous namespace

// Plugin registration for opt (new pass manager)
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
  return {
    LLVM_PLUGIN_API_VERSION, "ConstFoldBinOpPass", LLVM_VERSION_STRING,
    [](PassBuilder &PB) {
      PB.registerPipelineParsingCallback(
        [](StringRef Name, FunctionPassManager &FPM,
           ArrayRef<PassBuilder::PipelineElement>) {
          if (Name == "const-fold-binop") {
            FPM.addPass(ConstFoldBinOpPass());
            return true;
          }
          return false;
        });
    }
  };
}


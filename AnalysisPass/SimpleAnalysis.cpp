#include "llvm/IR/Function.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

// Our analysis result
struct SimpleAnalysisInfo {
    unsigned InstCount;
};

// Define the analysis pass
struct SimpleAnalysisPass : public AnalysisInfoMixin<SimpleAnalysisPass> {
    using Result = SimpleAnalysisInfo;

    static AnalysisKey Key;

    Result run(Function &F, FunctionAnalysisManager &) {
        unsigned Count = 0;
        for (auto &BB : F) {
            for (auto &I : BB) {
                Count++;
            }
        }
        errs() << "Function " << F.getName() 
               << " has " << Count << " instructions.\n";
        return {Count};
    }
};

AnalysisKey SimpleAnalysisPass::Key;

// Register the pass plugin

extern "C" LLVM_ATTRIBUTE_WEAK PassPluginLibraryInfo llvmGetPassPluginInfo() {
    return {
        LLVM_PLUGIN_API_VERSION, "SimpleAnalysis", "v0.1",
        [](PassBuilder &PB) {
            // Normal analysis registration
            PB.registerAnalysisRegistrationCallback(
                [](FunctionAnalysisManager &FAM) {
                    FAM.registerPass([] { return SimpleAnalysisPass(); });
                });

            // Add a pipeline hook so we can invoke it by name
            PB.registerPipelineParsingCallback(
                [](StringRef Name, FunctionPassManager &FPM,
                   ArrayRef<PassBuilder::PipelineElement>) {
                    if (Name == "simple-analysis") {
                        // Wrap analysis in a FunctionPass so it can run standalone
                        FPM.addPass(RequireAnalysisPass<SimpleAnalysisPass, Function>());
                        return true;
                    }
                    return false;
                });
        }
    };
}


#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/DependenceAnalysis.h"
#include "llvm/Analysis/LoopAccessAnalysis.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/PassPlugin.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Analysis/OptimizationRemarkEmitter.h"
#include <map>

using namespace llvm;


#define DEBUG_TYPE "check-alc-conditions"


namespace {
    struct ALC_Conditions_Checker : public PassInfoMixin<ALC_Conditions_Checker> {


        PreservedAnalyses run(Loop &loop, LoopAnalysisManager &AM,
                              LoopStandardAnalysisResults &AR, LPMUpdater &U) {

            Loop *L = &loop;
            //only apply the pass on innermost loop
            if (!L->getSubLoops().empty()) {
                return PreservedAnalyses::all();
            }

            bool functionCall = containsFunctionCall(L);
            bool vectorizable = isVectorizable(loop, AM, AR);
            bool outPutDependency = containsOutputDependency(L);

            llvm::outs() << "\nFunction:  " << L->getHeader()->getParent()->getName() << '\n';
            const ArrayRef<BasicBlock *> &allBlocks = L->getBlocks();
            const DebugLoc &location = allBlocks.front()->getFirstNonPHIOrDbg()->getDebugLoc();
            llvm::outs() << "Loop at line number: " << location.getLine() - 1 << "\n";

            if (functionCall) {
                llvm::outs() << "Loop contains function call" << '\n';
            } else {
                llvm::outs() << "Loop doesn't contain function call" << '\n';
            }

            if (vectorizable) {
                llvm::outs() << "Loop doesn't contain memory dependency" << '\n';
            } else {
                llvm::outs() << "Loop contains memory dependency" << '\n';
            }

            if (outPutDependency) {
                llvm::outs() << "Loop contains output dependency" << '\n';
            } else {
                llvm::outs() << "Loop doesn't contain output dependency" << '\n';
            }


            BasicBlock *const &firstNode = L->getHeader();
            BasicBlock *const &loopLatch = L->getLoopLatch();  //supposing to have only one exiting node

            int numberOfPaths = 0;
            std::map<BasicBlock *const, bool> visited;

            countNumberOfPaths(firstNode, loopLatch, numberOfPaths, visited, allBlocks);
            llvm::outs() << "Number of paths: " << numberOfPaths << '\n';

            if(numberOfPaths > 1){
                llvm::outs()<<"If Conversion can be applied \n";
            }else{
                llvm::outs()<<"If Conversion can NOT be applied \n";
            }

            if(!functionCall && !outPutDependency && vectorizable && numberOfPaths > 1){
                llvm::outs()<<"ALC can be applied \n";
            }else{
                llvm::outs()<<"ALC can NOT be applied \n";
            }


            return PreservedAnalyses::all();
        }


        void countNumberOfPaths(BasicBlock *const &src, BasicBlock *const &dest, int &path_count,
                                std::map<BasicBlock *const, bool> &visited, ArrayRef<BasicBlock *> allBlocks) {
            visited[src] = true;
            if (src == dest) {
                path_count++;
            } else {
                for (BasicBlock *succ: successors(src)) {
                    if (!visited[succ]) {
                        bool belongsToLoop = std::any_of(allBlocks.begin(), allBlocks.end(),
                                                         [&succ](BasicBlock *item) {
                                                             return item == succ;
                                                         });
                        if (belongsToLoop)
                            countNumberOfPaths(succ, dest, path_count, visited, allBlocks);
                    }
                }
            }
            visited[src] = false;
        }

        //Assumption: all blocks end with branch instruction
        static bool containsFunctionCall(Loop *L) {

            for (const auto &block: L->getBlocks()) {
                const Instruction *I = block->getFirstNonPHIOrDbg();
                for (const auto &instr: block->getInstList()) {
                    if (I == nullptr) {
                        break;
                    }
                    // to avoid debug instructions
                    if (I != &instr) {
                        I = I->getNextNonDebugInstruction();
                        continue;
                    }

                    if (isa<CallInst>(instr)) {
                        return true;
                    }
                    I = I->getNextNonDebugInstruction();
                }

            }
            return false;
        }

        static bool isVectorizable(Loop &L, LoopAnalysisManager &AM,
                                   LoopStandardAnalysisResults &LAR) {

            LoopAccessAnalysis::Result &info = AM.getResult<LoopAccessAnalysis>(L, LAR);
            return info.canVectorizeMemory();

        }

        static bool containsOutputDependency(Loop *L) {

            // If this PHINode is not in the header block, then we know that we
            // can convert it to select during if-conversion.
            int counter = 0;
            for (const auto &instr: L->getHeader()->getInstList()) {
                if (isa<PHINode>(instr)) {
                    counter++;
                }
            }
            return counter > 1;

        }

    };
}




// registering the pass to new PM
extern "C" ::llvm::PassPluginLibraryInfo LLVM_ATTRIBUTE_WEAK
llvmGetPassPluginInfo() {
    return {
            LLVM_PLUGIN_API_VERSION, "check-alc-conditions", "v0.1",
            [](PassBuilder &PB) {
                PB.registerPipelineParsingCallback(
                        [](StringRef Name, LoopPassManager &LPM,
                           ArrayRef<PassBuilder::PipelineElement>) {
                            if (Name == "check-alc-conditions") {
                                LPM.addPass(ALC_Conditions_Checker());
                                return true;
                            }
                            return false;
                        }
                );
            }
    };
}




#ifndef SMOL_JIT
#define SMOL_JIT

#include "llvm/ADT/StringRef.h"
#include "llvm/ExecutionEngine/JITSymbol.h"
#include "llvm/ExecutionEngine/Orc/CompileUtils.h"
#include "llvm/ExecutionEngine/Orc/Core.h"
#include "llvm/ExecutionEngine/Orc/ExecutionUtils.h"
#include "llvm/ExecutionEngine/Orc/IRCompileLayer.h"
#include "llvm/ExecutionEngine/Orc/JITTargetMachineBuilder.h"
#include "llvm/ExecutionEngine/Orc/RTDyldObjectLinkingLayer.h"
#include "llvm/ExecutionEngine/Orc/TargetProcessControl.h"
#include "llvm/ExecutionEngine/SectionMemoryManager.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/LLVMContext.h"
#include <memory>

// A JIT using LLVM's On-Request-Compilation(ORC) APIs
// mainly copied from https://github.com/llvm/llvm-project/blob/main/llvm/examples/Kaleidoscope/include/KaleidoscopeJIT.h
class SmolJIT {
    public:
        SmolJIT(std::unique_ptr<llvm::orc::TargetProcessControl> TPC,
                std::unique_ptr<llvm::orc::ExecutionSession> ES,
                llvm::orc::JITTargetMachineBuilder JTMB,
                llvm::DataLayout DL)
            : TPC(std::move(TPC)), ES(std::move(ES)), DL(std::move(DL)),
              Mangle(*this->ES, this->DL),
              ObjectLayer(*this->ES,
                          []() { return std::make_unique<llvm::SectionMemoryManager>(); }),
              
              CompileLayer(*this->ES, ObjectLayer,
                           std::make_unique<llvm::orc::ConcurrentIRCompiler>(std::move(JTMB))),
              MainJD(this->ES->createBareJITDylib("<main>")) {
                  MainJD.addGenerator(
                    llvm::cantFail(llvm::orc::DynamicLibrarySearchGenerator::GetForCurrentProcess(
                        DL.getGlobalPrefix()
                    )));
            }
    public:
        ~SmolJIT() {
            if (auto Err = ES->endSession())
                ES->reportError(std::move(Err));
        }

        static llvm::Expected<std::unique_ptr<SmolJIT>> Create() {
            auto SSP = std::make_shared<llvm::orc::SymbolStringPool>();
            auto TPC = llvm::orc::SelfTargetProcessControl::Create(SSP);
            if (!TPC)
                return TPC.takeError();

            auto ES = std::make_unique<llvm::orc::ExecutionSession>(std::move(SSP));

            llvm::orc::JITTargetMachineBuilder JTMB(
                (*TPC)->getTargetTriple()
            );

            auto DL = JTMB.getDefaultDataLayoutForTarget();
            if (!DL)
                return DL.takeError();

            return std::make_unique<SmolJIT>(std::move(*TPC), std::move(ES),
                                             std::move(JTMB), std::move(*DL));
        }

        const llvm::DataLayout &getDataLayout() const { return DL; }

        llvm::orc::JITDylib &getMainJITDylib() { return MainJD; }

        llvm::Error addModule(llvm::orc::ThreadSafeModule TSM,
                              llvm::orc::ResourceTrackerSP RT = nullptr) {
            if (!RT)
                RT = MainJD.getDefaultResourceTracker();
            return CompileLayer.add(RT, std::move(TSM));
        }

        llvm::Expected<llvm::JITEvaluatedSymbol> lookup(llvm::StringRef Name) {
            return ES->lookup({&MainJD}, Mangle(Name.str()));
        }

    private:
        std::unique_ptr<llvm::orc::TargetProcessControl> TPC;
        std::unique_ptr<llvm::orc::ExecutionSession> ES; // provides context for running JIT'd code
        llvm::DataLayout DL;                  // Used for symbol mangling 
        llvm::orc::MangleAndInterner Mangle;  // ''

        llvm::orc::RTDyldObjectLinkingLayer ObjectLayer; // adds object files
        llvm::orc::IRCompileLayer CompileLayer;          // adds LLVM modules

        llvm::orc::JITDylib &MainJD;
};

#endif
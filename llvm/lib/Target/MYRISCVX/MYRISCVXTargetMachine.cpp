#include "MYRISCVXTargetMachine.h"
#include "MYRISCVX.h"
// #include "MYRISCVXISelDAGToDAG.h"
#include "MYRISCVXTargetObjectFile.h"

#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

extern "C" void LLVMInitializeMYRISCVXTarget() {
  RegisterTargetMachine<MYRISCVX32TargetMachine> X(getTheMYRISCVX32Target());
  RegisterTargetMachine<MYRISCVX64TargetMachine> Y(getTheMYRISCVX64Target());
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options) {
  std::string Ret = "";
  Ret += "e";
  Ret += "-m:m";

  // ポインタのサイズを指定する
  // RV32の場合は32bit
  // RV64の場合は64bit
  if (TT.isArch64Bit()) {
    Ret += "-p:64:64";
  } else {
    Ret += "-p:32:32";
  }

  // 8bitと16bit変数は32bitにアラインする
  // 64bit変数は64bitにアラインする
  Ret += "-i8:8:32-i16:16:32-i64:64";

  // 整数レジスタ・スタックのアラインメント
  if (TT.isArch64Bit()) {
    Ret += "-n64-S128";
  } else {
    Ret += "-n32-S64";
  }

  return Ret;
}

static Reloc::Model getEffectiveRelocModel(bool JIT,
                                           Optional<Reloc::Model> RM) {
  if (!RM.hasValue() || JIT) {
    return Reloc::Static;
  }

  return *RM;
}

MYRISCVXTargetMachine::MYRISCVXTargetMachine(const Target &T, const Triple &TT,
                                             StringRef CPU, StringRef FS,
                                             const TargetOptions &Options,
                                             Optional<Reloc::Model> RM,
                                             Optional<CodeModel::Model> CM,
                                             CodeGenOpt::Level OL, bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(TT, CPU, Options), TT, CPU, FS,
                        Options, getEffectiveRelocModel(JIT, RM),
                        getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<MYRISCVXTargetObjectFile>()),
      ABI(MYRISCVXABIInfo::computeTargetABI(Options.MCOptions.getABIName())),
      DefaultSubtarget(TT, CPU, CPU, FS, *this) {
  initAsmInfo();
}

MYRISCVXTargetMachine::~MYRISCVXTargetMachine() {}

MYRISCVX32TargetMachine::MYRISCVX32TargetMachine(
    const Target &T, const Triple &TT, StringRef CPU, StringRef FS,
    const TargetOptions &Options, Optional<Reloc::Model> RM,
    Optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT)
    : MYRISCVXTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT) {}

void MYRISCVX32TargetMachine::anchor() {}

MYRISCVX64TargetMachine::MYRISCVX64TargetMachine(
    const Target &T, const Triple &TT, StringRef CPU, StringRef FS,
    const TargetOptions &Options, Optional<Reloc::Model> RM,
    Optional<CodeModel::Model> CM, CodeGenOpt::Level OL, bool JIT)
    : MYRISCVXTargetMachine(T, TT, CPU, FS, Options, RM, CM, OL, JIT) {}

void MYRISCVX64TargetMachine::anchor() {}
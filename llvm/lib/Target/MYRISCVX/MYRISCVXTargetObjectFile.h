#ifndef LLVM_LIB_TARGET_MYRISCVX_MYRISCVXTARGETOBJECTFILE_H
#define LLVM_LIB_TARGET_MYRISCVX_MYRISCVXTARGETOBJECTFILE_H

#include "MYRISCVXTargetMachine.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"

namespace llvm {
class MYRISCVXTargetMachine;

class MYRISCVXTargetObjectFile : public TargetLoweringObjectFileELF {
  const MYRISCVXTargetMachine *TM;

public:
  void Initialize(MCContext &Ctx, const TargetMachine &TM) override;
};
} // namespace llvm

#endif
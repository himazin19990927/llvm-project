#ifndef LLVM_LIB_TARGET_MYRISCVX_MYRISCVXFRAMELOWERING_H
#define LLVM_LIB_TARGET_MYRISCVX_MYRISCVXFRAMELOWERING_H

#include "MYRISCVX.h"
#include "llvm/CodeGen/TargetFrameLowering.h"

namespace llvm {
class MYRISCVXSubtarget;

class MYRISCVXFrameLowering : public TargetFrameLowering {
protected:
  const MYRISCVXSubtarget &STI;

public:
  explicit MYRISCVXFrameLowering(const MYRISCVXSubtarget &sti)
      : TargetFrameLowering(StackGrowsDown, Align(16), 0), STI(sti) {}

  bool hasFP(const MachineFunction &MF) const override;

  void emitPrologue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
  void emitEpilogue(MachineFunction &MF, MachineBasicBlock &MBB) const override;
};
} // namespace llvm

#endif
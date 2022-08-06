#include "MYRISCVXRegisterInfo.h"
#include "MYRISCVX.h"
#include "MYRISCVXSubtarget.h"
// #include "MYRISCVXMachineFunction.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "MYRISCVX-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "MYRISCVXGenRegisterInfo.inc"

MYRISCVXRegisterInfo::MYRISCVXRegisterInfo(const MYRISCVXSubtarget &ST,
                                           unsigned HwMode)
    : MYRISCVXGenRegisterInfo(MYRISCVX::RA, 0, 0, 0, HwMode), Subtarget(ST) {}

const MCPhysReg *
MYRISCVXRegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  // MYRISCVXCalingConv.tdで定義したCSR_LP32のリストをそのまま返せば良い
  return CSR_LP32_SaveList;
}

const uint32_t *
MYRISCVXRegisterInfo::getCallPreservedMask(const MachineFunction &MF,
                                           CallingConv::ID) const {
  return CSR_LP32_RegMask;
}

BitVector
MYRISCVXRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  // zero, ra, fp, sp, gp, tpはシステムが使用する予約済みレジスタなので
  // レジスタ割当に使用しない
  static const uint16_t ReservedCPURegs[] = {MYRISCVX::ZERO, MYRISCVX::RA,
                                             MYRISCVX::FP,   MYRISCVX::SP,
                                             MYRISCVX::GP,   MYRISCVX::TP};
  BitVector Reserved(getNumRegs());

  for (unsigned I = 0; I < array_lengthof(ReservedCPURegs); ++I) {
    Reserved.set(ReservedCPURegs[I]);
  }

  return Reserved;
}

void MYRISCVXRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                               int SPAdj, unsigned FIOperandNum,
                                               RegScavenger *RS) const {
  // フレームインデックスを削除するための関数
  // ここではまだ実装しない
}

Register
MYRISCVXRegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  return TFI->hasFP(MF) ? (MYRISCVX::FP) : (MYRISCVX::SP);
}
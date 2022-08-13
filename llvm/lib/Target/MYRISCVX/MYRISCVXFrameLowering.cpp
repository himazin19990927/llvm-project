//===-- MYRISCVXFrameLowering.cpp - MYRISCVX Frame Information ------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file contains the MYRISCVX implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "MYRISCVXFrameLowering.h"

#include "MYRISCVXInstrInfo.h"
#include "MYRISCVXMachineFunction.h"
#include "MYRISCVXSubtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/IR/DataLayout.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

// @{ MYRISCVXFrameLowering_emitPrologue
// 関数のプロローグを生成する: 関数フレームの確保とCallee Savedレジスタの退避
void MYRISCVXFrameLowering::emitPrologue(MachineFunction &MF,
                                         MachineBasicBlock &MBB) const {
  assert(&MF.front() == &MBB && "Shrink-wrapping not yer supported");
  MachineFrameInfo MFI = MF.getFrameInfo();

  const MYRISCVXInstrInfo &TII =
      *static_cast<const MYRISCVXInstrInfo *>(STI.getInstrInfo());

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc dl = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();
  unsigned SP = MYRISCVX::SP;

  // まず最終的なスタックフレームのサイズを取得する
  uint64_t StackSize = MFI.getStackSize();

  // もしスタックフレームのサイズが0ならば、スタックフレームの調整は必要ない
  if (StackSize == 0 && !MFI.adjustsStack()) {
    return;
  }

  MachineModuleInfo &MMI = MF.getMMI();
  const MCRegisterInfo *MRI = MMI.getContext().getRegisterInfo();

  // スタックフレームの調整を行う(マイナス方向)
  TII.adjustStackPtr(SP, -StackSize, MBB, MBBI);

  unsigned CFIIndex =
      MF.addFrameInst(MCCFIInstruction::cfiDefCfaOffset(nullptr, -StackSize));
  BuildMI(MBB, MBBI, dl, TII.get(TargetOpcode::CFI_INSTRUCTION))
      .addCFIIndex(CFIIndex);

  const std::vector<CalleeSavedInfo> &CSI = MFI.getCalleeSavedInfo();

  if (CSI.size()) {
    // もし退避しなければならないCallee Savedレジスタが存在していれば
    // スタックに退避するための命令を生成する
    for (unsigned i = 0; i < CSI.size(); ++i) {
      ++MBBI;
    }

    for (std::vector<CalleeSavedInfo>::const_iterator I = CSI.begin(),
                                                      E = CSI.end();
         I != E; ++I) {
      int64_t Offset = MFI.getObjectOffset(I->getFrameIdx());
      unsigned Reg = I->getReg();
      {
        // reg is in CPU Regs.
        unsigned CFIIndex = MF.addFrameInst(MCCFIInstruction::createOffset(
            nullptr, MRI->getDwarfRegNum(Reg, 1), Offset));
        BuildMI(MBB, MBBI, dl, TII.get(TargetOpcode::CFI_INSTRUCTION))
            .addCFIIndex(CFIIndex);
      }
    }
  }
}
// @} MYRISCVXFrameLowering_emitPrologue

// @{ MYRISCVXFrameLowering_emitEpilogue
void MYRISCVXFrameLowering::emitEpilogue(MachineFunction &MF,
                                         MachineBasicBlock &MBB) const {
  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  MachineFrameInfo MFI = MF.getFrameInfo();

  const MYRISCVXInstrInfo &TII =
      *static_cast<const MYRISCVXInstrInfo *>(STI.getInstrInfo());
  DebugLoc dl = MBBI->getDebugLoc();

  // スタックポインタSPを使用する
  unsigned SP = MYRISCVX::SP;

  // スタックフレームのサイズを取得する
  uint64_t StackSize = MFI.getStackSize();

  if(!StackSize) {
    return;
  }

  // スタックポインタ(SP)の調整を行う(プラス方向)
  TII.adjustStackPtr(SP, StackSize, MBB, MBBI);
}
// @} MYRISCVXFrameLowering_emitEpilogue

// hasFP - Return true if the specified function should have a dedicated frame
// pointer register.  This is true if the function has variable sized allocas,
// if it needs dynamic stack realignment, if frame pointer elimination is
// disabled, or if the frame address is taken.
bool MYRISCVXFrameLowering::hasFP(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  const TargetRegisterInfo *TRI = STI.getRegisterInfo();

  return MF.getTarget().Options.DisableFramePointerElim(MF) ||
         MFI.hasVarSizedObjects() || MFI.isFrameAddressTaken() ||
         TRI->needsStackRealignment(MF);
}

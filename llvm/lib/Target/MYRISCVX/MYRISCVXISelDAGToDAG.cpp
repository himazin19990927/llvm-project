//===-- MYRISCVXISelDAGToDAG.cpp - A Dag to Dag Inst Selector for MYRISCVX
//--------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===------------------------------------------------------------------------------===//
//
// This file defines an instruction selector for the MYRISCVX target.
//
//===------------------------------------------------------------------------------===//

#include "MYRISCVXISelDAGToDAG.h"
#include "MYRISCVX.h"
#include "MYRISCVXMatInt.h"

#include "MYRISCVXMachineFunction.h"
#include "MYRISCVXRegisterInfo.h"
#include "MYRISCVXTargetMachine.h"
#include "llvm/CodeGen/MachineConstantPool.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAGISel.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Intrinsics.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
using namespace llvm;

#define DEBUG_TYPE "MYRISCVX-isel"

//===----------------------------------------------------------------------===//
// Instruction Selector Implementation
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// MYRISCVXDAGToDAGISel - MYRISCVX specific code to select MYRISCVX machine
// instructions for SelectionDAG operations.
//===----------------------------------------------------------------------===//

bool MYRISCVXDAGToDAGISel::runOnMachineFunction(MachineFunction &MF) {
  Subtarget = &MF.getSubtarget<MYRISCVXSubtarget>();
  bool Ret = SelectionDAGISel::runOnMachineFunction(MF);

  return Ret;
}

bool MYRISCVXDAGToDAGISel::SelectAddrFI(SDValue Addr, SDValue &Base) {
  if (auto FIN = dyn_cast<FrameIndexSDNode>(Addr)) {
    Base = CurDAG->getTargetFrameIndex(FIN->getIndex(), Subtarget->getXLenVT());
    return true;
  }
  return false;
}

static SDNode *selectImm(SelectionDAG *CurDAG, const SDLoc &DL, int64_t Imm,
                         MVT XLenVT) {

  // generateInstSeqでは定数生成のための命令列を返す
  // 各命令は(opcode, imm)のフィールドを持つ構造体で表される
  // opcodeはMYRISCVX::LUI, MYRISCVX::ADDIW, MYRISCVX::ADDIのいずれかである
  // いずれの命令もレジスタと即値の入力を受け取り、結果をレジスタに格納する
  MYRISCVXMatInt::InstSeq Seq;
  MYRISCVXMatInt::generateInstSeq(Imm, XLenVT == MVT::i64, Seq);

  // 命令によって生成された値
  SDNode *Result = nullptr;

  // 命令の入力として使用する値(初期値はゼロレジスタ)
  SDValue SrcReg = CurDAG->getRegister(MYRISCVX::ZERO, XLenVT);

  for (MYRISCVXMatInt::Inst &Inst : Seq) {
    SDValue SDImm = CurDAG->getTargetConstant(Inst.Imm, DL, XLenVT);

    if (Inst.Opc == MYRISCVX::LUI) {
      Result = CurDAG->getMachineNode(MYRISCVX::LUI, DL, XLenVT, SDImm);
    } else {
      Result = CurDAG->getMachineNode(Inst.Opc, DL, XLenVT, SrcReg, SDImm);
    }

    SrcReg = SDValue(Result, 0);
  }

  return Result;
}

/// Select instructions not customized! Used for
/// expanded, promoted and normal instructions
void MYRISCVXDAGToDAGISel::Select(SDNode *Node) {
  unsigned Opcode = Node->getOpcode();
  MVT XLenVT = Subtarget->getXLenVT();
  SDLoc DL(Node);
  EVT VT = Node->getValueType(0);

  LLVM_DEBUG(errs() << "Selecting: "; Node->dump(CurDAG); errs() << "\n");

  // すでに命令ノードに置き換わっている場合は変換の必要はない
  if (Node->isMachineOpcode()) {
    LLVM_DEBUG(errs() << "== "; Node->dump(CurDAG); errs() << "\n");
    Node->setNodeId(-1);
    return;
  }

  // 特殊な変換が必要であればここで処理する
  switch (Opcode) {
  default:
    break;
  case ISD::Constant: {
    auto ConstNode = cast<ConstantSDNode>(Node);
    int64_t Imm = ConstNode->getSExtValue();
    if (XLenVT == MVT::i64) {
      ReplaceNode(Node, selectImm(CurDAG, SDLoc(Node), Imm, XLenVT));
      return;
    }
    break;
  }
  }

  // デフォルトでは以下の関数が呼び出されて変換が行われる
  SelectCode(Node);
}

void MYRISCVXDAGToDAGISel::processFunctionAfterISel(MachineFunction &MF) {}

FunctionPass *llvm::createMYRISCVXISelDag(MYRISCVXTargetMachine &TM,
                                          CodeGenOpt::Level OptLevel) {
  return new MYRISCVXDAGToDAGISel(TM, OptLevel);
}

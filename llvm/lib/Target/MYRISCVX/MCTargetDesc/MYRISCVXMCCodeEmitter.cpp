//===-- MYRISCVXMCCodeEmitter.cpp - Convert MYRISCVX Code to Machine Code
//---------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the MYRISCVXMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//
//

#include "MYRISCVXMCCodeEmitter.h"
#include "MCTargetDesc/MYRISCVXBaseInfo.h"
#include "MCTargetDesc/MYRISCVXFixupKinds.h"
// #include "MCTargetDesc/MYRISCVXMCExpr.h"
#include "MCTargetDesc/MYRISCVXMCTargetDesc.h"

#include "llvm/CodeGen/Register.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstBuilder.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/EndianStream.h"
#include "llvm/Support/raw_ostream.h"

#define DEBUG_TYPE "mccodeemitter"

#define GET_INSTRMAP_INFO
#include "MYRISCVXGenInstrInfo.inc"
#undef GET_INSTRMAP_INFO

namespace llvm {

MCCodeEmitter *createMYRISCVXMCCodeEmitter(const MCInstrInfo &MCII,
                                           const MCRegisterInfo &MRI,
                                           MCContext &Ctx) {
  return new MYRISCVXMCCodeEmitter(MCII, Ctx, true);
}
} // End of namespace llvm

void MYRISCVXMCCodeEmitter::EmitByte(unsigned char C, raw_ostream &OS) const {
  OS << (char)C;
}

// @{ MYRISCVXMCCodeEmitter_cpp_EmitInstruction
void MYRISCVXMCCodeEmitter::EmitInstruction(uint64_t Val, unsigned Size,
                                            raw_ostream &OS) const {
  // Output the instruction encoding in little endian byte order.
  for (unsigned i = 0; i < Size; ++i) {
    unsigned Shift = IsLittleEndian ? i * 8 : (Size - 1 - i) * 8;
    EmitByte((Val >> Shift) & 0xff, OS);
  }
}
// @} MYRISCVXMCCodeEmitter_cpp_EmitInstruction

// @{ MYRISCVXMCCodeEmitter_cpp_encodeInstruction_expandFunctionCall
// @{ MYRISCVXMCCodeEmitter_cpp_encodeInstruction
// encodeInstruction()では、MCInst形式の命令を引数MIから受け取り、機械語に変換して
// raw_ostream &OSを経由して返す
void MYRISCVXMCCodeEmitter::encodeInstruction(
    const MCInst &MI, raw_ostream &OS, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &STI) const {
  // 未実装
}
// @} MYRISCVXMCCodeEmitter_cpp_encodeInstruction

// @{ MYRISCVXMCCodeEmitter_getBranch12TargetOpValue
// 分岐命令のターゲットオペランドをエンコードする
// 機械語的には最後の1ビットは省略されているので, 1ビットシフトが必要
unsigned MYRISCVXMCCodeEmitter::getBranch12TargetOpValue(
    const MCInst &MI, unsigned OpNo, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);

  if (MO.isImm()) {
    unsigned Res = MO.getImm();
    assert((Res & 1) == 0 && "LSB is non-zero");
    return Res >> 1;
  }
  // オペランドが即値ではなかった場合,
  // リロケーション情報を残すための特殊な処理が必要
  return getExprOpValue(MI, MO.getExpr(), Fixups, STI);
}
// @} MYRISCVXMCCodeEmitter_getBranch12TargetOpValue

// @{ MYRISCVXMCCodeEmitter_getBranch20TargetOpValue
// 分岐命令のターゲットオペランドをエンコードする
// 機械語的には最後の1ビットは省略されているので, 1ビットシフトが必要
unsigned MYRISCVXMCCodeEmitter::getBranch20TargetOpValue(
    const MCInst &MI, unsigned OpNo, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(OpNo);

  if (MO.isImm()) {
    unsigned Res = MO.getImm();
    assert((Res & 1) == 0 && "LSB is non-zero");
    return Res >> 1;
  }
  // オペランドが即値ではなかった場合,
  // リロケーション情報を残すための特殊な処理が必要
  return getExprOpValue(MI, MO.getExpr(), Fixups, STI);
}
// @} MYRISCVXMCCodeEmitter_getBranch20TargetOpValue

// @{ MYRISCVXMCCodeEmitter_getExprOpValue_Head
// @{ MYRISCVXMCCodeEmitter_getExprOpValue_Fixup
// @{ MYRISCVXMCCodeEmitter_getExprOpValue
unsigned
MYRISCVXMCCodeEmitter::getExprOpValue(const MCInst &MI, const MCExpr *Expr,
                                      SmallVectorImpl<MCFixup> &Fixups,
                                      const MCSubtargetInfo &STI) const {
  // 未実装
  return 0;
}
// @} MYRISCVXMCCodeEmitter_getExprOpValue_Fixup
// @} MYRISCVXMCCodeEmitter_getExprOpValue

// @{ MYRISCVXMCCodeEmitter_getMachineOpValue
/// getMachineOpValue - Return binary encoding of operand. If the machine
/// operand requires relocation, record the relocation and return zero.
unsigned
MYRISCVXMCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  if (MO.isReg()) {
    unsigned Reg = MO.getReg();
    unsigned RegNo = Ctx.getRegisterInfo()->getEncodingValue(Reg);
    return RegNo;
  } else if (MO.isImm()) {
    return static_cast<unsigned>(MO.getImm());
  }
  // MO must be an Expr.
  assert(MO.isExpr());
  return getExprOpValue(MI, MO.getExpr(), Fixups, STI);
}
// @} MYRISCVXMCCodeEmitter_getMachineOpValue

#include "MYRISCVXGenMCCodeEmitter.inc"

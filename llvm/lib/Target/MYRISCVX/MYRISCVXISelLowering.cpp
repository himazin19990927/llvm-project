#include "MYRISCVXISelLowering.h"

// #include "MYRISCVXMachineFunction.h"
#include "MYRISCVXSubtarget.h"
#include "MYRISCVXTargetMachine.h"
#include "MYRISCVXTargetObjectFile.h"

#include "llvm/ADT/Statistic.h"

#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/Codegen/CallingConvLower.h"

#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/GlobalVariable.h"

#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

#define DEBUG_TYPE "MYRISCVX-lower"

STATISTIC(NumTailCalls, "Number of tail calls");

MYRISCVXTargetLowering::MYRISCVXTargetLowering(const MYRISCVXTargetMachine &TM,
                                               const MYRISCVXSubtarget &STI)
    : TargetLowering(TM), Subtarget(STI), ABI(TM.getABI()) {
      
    }

SDValue MYRISCVXTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &dl,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  // ここではまだ実装しない
  // 多くの場合は以下が含まれる
  // - 引数を受け取った物理レジスタを仮想的なレジスタに移す作業
  // - スタックを経由した引数を仮想的なレジスタに移す作業
  return Chain;
}

SDValue
MYRISCVXTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                    bool IsVarArg,
                                    const SmallVectorImpl<ISD::OutputArg> &Outs,
                                    const SmallVectorImpl<SDValue> &OutVals,
                                    const SDLoc &dl, SelectionDAG &DAG) const {
  // ここではまだ何も実装しない
  // 多くの場合は、戻り地を持っている仮想レジスタを物理レジスタに移す作業が含まれる
  return Chain;
}
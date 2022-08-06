#ifndef LLVM_LIB_TARGET_MYRISCVX_MYRISCVXISELLOWERING_H
#define LLVM_LIB_TARGET_MYRISCVX_MYRISCVXISELLOWERING_H

#include "MCTargetDesc/MYRISCVXABIInfo.h"
// #include "MCTargetDesc/MYRISCVXBaseInfo.h"
#include "MYRISCVX.h"

#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/TargetLowering.h"
#include "llvm/IR/Function.h"
#include <deque>

namespace llvm {
class MYRISCVXFunctionInfo;
class MYRISCVXSubtarget;

class MYRISCVXTargetLowering : public TargetLowering {
public:
  explicit MYRISCVXTargetLowering(const MYRISCVXTargetMachine &TM,
                                  const MYRISCVXSubtarget &STI);

protected:
  const MYRISCVXSubtarget &Subtarget;
  const MYRISCVXABIInfo &ABI;

private:
  SDValue LowerFormalArguments(SDValue Chain, CallingConv::ID CallConv,
                               bool IsVarArg,
                               const SmallVectorImpl<ISD::InputArg> &Ins,
                               const SDLoc &dl, SelectionDAG &DAG,
                               SmallVectorImpl<SDValue> &InVals) const override;

  SDValue LowerReturn(SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
                      const SmallVectorImpl<ISD::OutputArg> &Outs,
                      const SmallVectorImpl<SDValue> &OutVals, const SDLoc &dl,
                      SelectionDAG &DAG) const override;
};
} // namespace llvm

#endif
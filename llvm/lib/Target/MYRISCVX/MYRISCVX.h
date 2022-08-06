#ifndef LLVM_LIB_TARGET_MYRISCVX_MYRISCVX_H
#define LLVM_LIB_TARGET_MYRISCVX_MYRISCVX_H

#include "MCTargetDesc/MYRISCVXMCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace llvm {
class MYRISCVXTargetMachine;
class FunctionPass;

FunctionPass *createMYRISCVXExpandPseudoPass();
void initializeMYRISCVXExpandPseudoPass(PassRegistry &);
} // namespace llvm

#endif
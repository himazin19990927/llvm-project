#include "MYRISCVXMCTargetDesc.h"
// #include "MYRISCVXTargetStreamer"
// #include "MCTargetDesc/MYRISCVXInstPrinter.h"
// #include "MYRISCVXMCAsmInfo.h"

#include "llvm/MC/MCELFStreamer.h"
#include "llvm/MC/MCInstPrinter.h"
#include "llvm/MC/MCInstrAnalysis.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MachineLocation.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Support/TargetRegistry.h"

// #include "MCTargetDesc/MYRISCVStreamer";
// #include "MCTargetDesc/MYRISCVXMCAsmInfo.h"
// #include "MCTargetDesc/MYRISCVXInstPrinter.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#include "MYRISCVXGenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "MYRISCVXGenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "MYRISCVXGenRegisterInfo.inc"

extern "C" void LLVMInitializeMYRISCVXTargetMC() {
  for (Target *T : {&getTheMYRISCVX32Target(), &getTheMYRISCVX64Target()}) {
  }
}
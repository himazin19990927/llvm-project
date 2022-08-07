#ifndef LLVM_LIB_TARGET_MYRISCVX_MCTARGETDESC_MYRISCVXMCTARGETDESC_H
#define LLVM_LIB_TARGET_MYRISCVX_MCTARGETDESC_MYRISCVXMCTARGETDESC_H

#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {
class MCAsmBackend;
class MCCodeEmitter;
class MCContext;
class MCInstrInfo;
class MCObjectTargetWriter;
class MCRegisterInfo;
class MCSubtargetInfo;
class StringRef;
class Target;
class Triple;
class raw_ostream;
class raw_pwrite_stream;

Target &getTheMYRISCVX32Target();
Target &getTheMYRISCVX64Target();

MCCodeEmitter *createMYRISCVXMCCodeEmitter(const MCInstrInfo &MCII,
                                           const MCRegisterInfo &MRI,
                                           MCContext &Ctx);

MCAsmBackend *createMYRISCVXAsmBackend(const Target &T,
                                       const MCSubtargetInfo &STI,
                                       const MCRegisterInfo &MRI,
                                       const MCTargetOptions &Options);

std::unique_ptr<MCObjectTargetWriter>
createMYRISCVXELFObjectWriter(const Triple &TT, bool Is64Bit);
} // namespace llvm

#define GET_REGINFO_ENUM
#include "MYRISCVXGenRegisterInfo.inc"

#define GET_INSTRINFO_ENUM
#include "MYRISCVXGenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "MYRISCVXGenSubtargetInfo.inc"

#endif
#include "MYRISCVX.h"
#include "llvm/IR/Module.h"
#include "llvm/Support/TargetRegistry.h"

using namespace llvm;

namespace llvm {
// MYRISCVX32ターゲットインスタンスを返す
// staticなので常に同じインスタンスが返される
Target &getTheMYRISCVX32Target() {
  static Target TheMYRISCVX32Target;
  return TheMYRISCVX32Target;
}

// MYRISCVX64ターゲットインスタンスを返す
// staticなので常に同じインスタンスが返される
Target &getTheMYRISCVX64Target() {
  static Target TheMYRISCVX64Target;
  return TheMYRISCVX64Target;
}

extern "C" void LLVMInitializeMYRISCVXTargetInfo() {
  RegisterTarget<Triple::myriscvx32, true> X(
      getTheMYRISCVX32Target(), "myriscv32", "MYRISCVX (32-bit)", "MYRISCVX");
  RegisterTarget<Triple::myriscvx64, true> Y(
      getTheMYRISCVX64Target(), "myriscv64", "MYRISCVX (64-bit)", "MYRISCVX");
}

} // namespace llvm
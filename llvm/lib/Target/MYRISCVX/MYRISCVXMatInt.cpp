#include "MYRISCVXMatInt.h"
#include "MCTargetDesc/MYRISCVXMCTargetDesc.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/Support/MachineValueType.h"
#include "llvm/Support/MathExtras.h"
#include <cstdint>

namespace llvm {

namespace MYRISCVXMatInt {

// 64bit整数を生成するための関数
void generateInstSeq(int64_t Val, bool IsRV64, InstSeq &Res) {
  // 32bit以内に収まる場合
  if (isInt<32>(Val)) {
    // HI20とLO12に分割して命令を生成する
    int64_t Hi20 = ((Val + 0x800) >> 12) & 0xFFFFF;
    int64_t Lo12 = SignExtend64<12>(Val);

    // 整数が12bit以内に収まっていればHi20は生成しなくても良い
    // Hi20 != 0であれば12bit以上の整数であるため、それを生成する
    if (Hi20 != 0) {
      Res.push_back(Inst(MYRISCVX::LUI, Hi20));
    }

    // 下位の12bitを生成するためのADDI/ADDIW命令を生成する
    if ((Lo12 != 0) || (Hi20 == 0)) {
      unsigned AddiOpc =
          (IsRV64 && (Hi20 != 0)) ? MYRISCVX::ADDIW : MYRISCVX::ADDI;
      Res.push_back(Inst(AddiOpc, Lo12));
    }
    return;
  }

  assert(IsRV64 && "Can't emit >32bit imm for non-RV53 target");

  // 生成する定数が32bitに収まらない場合
  // 定数を上位52bitと下位12bitに分ける
  int64_t Lo12 = SignExtend64<12>(Val);
  int64_t Hi52 = ((uint64_t)Val + 0x800ull) >> 12;

  int ShiftAmount = 12 + findFirstSet((uint64_t)Hi52);
  Hi52 = SignExtend64(Hi52 >> (ShiftAmount - 12), 64 - ShiftAmount);

  // 上位の52bitを生成するためにgenerateInstSeqを再帰的に呼び出す
  generateInstSeq(Hi52, IsRV64, Res);

  // 生成した上位ビットをシフト命令で所望の位置に戻す
  Res.push_back(Inst(MYRISCVX::SLLI, ShiftAmount));

  // 生成した下位12bitをADDI命令で生成する
  if (Lo12 != 0) {
    Res.push_back(Inst(MYRISCVX::ADDI, Lo12));
  }
}

} // namespace MYRISCVXMatInt
} // namespace llvm
include "llvm/Target/Target.td"

// MYRISCVXの属性としてFeatureRV64を定義する
def FeatureRV64 : SubtargetFeature<"64bit", "HasRV64", "true", "RV64 support">;

// CPUモデルcpu-rv32/cpu-rv64
// cpu-rv64のときはFeatureRV64が有効になる
def : ProcessorModel<"cpu-rv32", NoSchedModel, []>;
def : ProcessorModel<"cpu-rv64", NoSchedModel, [FeatureRV64]>;


def RV64 : HwMode<"+64bit">; // MYRISCVX64用のスイッチを定義
def RV32 : HwMode<"-64bit">; // MYRISCVX32用のスイッチを定義

include "MYRISCVXRegisterInfo.td" // レジスタ定義を含んだtdファイル
include "MYRISCVXSchedule.td"
include "MYRISCVXInstrInfo.td" // 命令定義を含んだtdファイル
include "MYRISCVXCallingConv.td"

def MYRISCVXInstrInfo : InstrInfo;


// MYRISCVXターゲットアーキテクチャを作成する
def MYRISCVX : Target {
    let InstructionSet = MYRISCVXInstrInfo;
}
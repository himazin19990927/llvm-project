; RUN: llc --march=myriscvx32 < %s \
; RUN:    | FileCheck -check-prefix=MYRVX32I %s

; RUN: llc --march=myriscvx64 < %s \
; RUN:  | FileCheck -check-prefix=MYRVX64I %s

define dso_local void @simple_func() #0 {
  ; MYRVX32I-LABEL: simple_func:
  ; MYRVX32I:       # %bb.0:
  ; MYRVX32I-NEXT:  ret

  ; MYRVX64I-LABEL: simple_func:
  ; MYRVX64I:       # %bb.0:
  ; MYRVX64I-NEXT:  ret

entry:
    ret void
}
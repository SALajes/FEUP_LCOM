/* void (kbc_asm_ih)() */
/* need _C_LABEL */
#include <machine/asm.h>
/* need macros */
#include "i8042.h"

.global _C_LABEL(scan_code)
.global _C_LABEL(kbc_asm_ih)

.data
  validity_mask:
    .byte 0xC0
.text
  _C_LABEL(kbc_asm_ih):
      inb $KBD_STAT_REG
      and validity_mask, %al
      jnz invalid_status
      inb $KBD_OUT_BUFF
      movb %al, _C_LABEL(scan_code)
    invalid_status:
      ret 


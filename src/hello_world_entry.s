.section .text

.globl my_entry_pt

my_entry_pt:
  mv a0, zero
  lui a0, 0x10012
  ori a1, a0, 0x08
  ori a2, a0, 0x0C
  ori a3, zero, 1
  ori a4, zero, 0x20
  # slli a4, a3, 4
  sw a4, (a0)
  sw a3, (a1)
  sw a3, (a2)
  
  mv ra, zero
  mv sp, zero
  mv gp, zero
  mv tp, zero
  mv t0, zero
  mv t1, zero
  mv t2, zero
  mv fp, zero
  mv s1, zero
  mv a0, zero
  mv a1, zero
  mv a2, zero
  mv a3, zero
  mv a4, zero
  mv a5, zero
  mv a6, zero
  mv a7, zero
  mv s2, zero
  mv s3, zero
  mv s4, zero
  mv s5, zero
  mv s6, zero
  mv s7, zero
  mv s8, zero
  mv s9, zero
  mv s10, zero
  mv s11, zero
  mv t3, zero
  mv t4, zero
  mv t5, zero
  mv t6, zero
  
  # Load 0x80003FF0 into Stack Pointer and Global Pointer
  ori a0, zero, 0xFF
  slli a0, a0, 4
  lui sp, 0x80003
  or sp, sp, a0
  
  call main
  
  loop:
  nop
  nop
  nop
  nop
  nop
  j loop

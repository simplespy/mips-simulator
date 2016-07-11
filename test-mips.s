	.file	1 "test-mips.cpp"
	.section .mdebug.abi32
	.previous
	.nan	legacy
	.module	fp=xx
	.module	nooddspreg
	.abicalls
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,4
	.text
	.align	2
	.globl	main
$LFB1021 = .
	.cfi_startproc
	.set	nomips16
	.set	nomicromips
	.ent	main
	.type	main, @function
main:
	.frame	$fp,48,$31		# vars= 16, regs= 2/0, args= 16, gp= 8
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-48
	.cfi_def_cfa_offset 48
	sw	$31,44($sp)
	sw	$fp,40($sp)
	.cfi_offset 31, -4
	.cfi_offset 30, -8
	move	$fp,$sp
	.cfi_def_cfa_register 30
	lui	$28,%hi(__gnu_local_gp)
	addiu	$28,$28,%lo(__gnu_local_gp)
	.cprestore	16
	lw	$2,%got(__stack_chk_guard)($28)
	lw	$2,0($2)
	sw	$2,36($fp)
	addiu	$2,$fp,28
	move	$5,$2
	lw	$4,%got(_ZSt3cin)($28)
	lw	$2,%call16(_ZNSirsERi)($28)
	move	$25,$2
	.reloc	1f,R_MIPS_JALR,_ZNSirsERi
1:	jalr	$25
	nop

	lw	$28,16($fp)
	move	$3,$2
	addiu	$2,$fp,32
	move	$5,$2
	move	$4,$3
	lw	$2,%call16(_ZNSirsERi)($28)
	move	$25,$2
	.reloc	1f,R_MIPS_JALR,_ZNSirsERi
1:	jalr	$25
	nop

	lw	$28,16($fp)
	lw	$3,28($fp)
	lw	$2,32($fp)
	addu	$2,$3,$2
	move	$5,$2
	lw	$4,%got(_ZSt4cout)($28)
	lw	$2,%call16(_ZNSolsEi)($28)
	move	$25,$2
	.reloc	1f,R_MIPS_JALR,_ZNSolsEi
1:	jalr	$25
	nop

	lw	$28,16($fp)
	move	$2,$0
	lw	$3,%got(__stack_chk_guard)($28)
	lw	$4,36($fp)
	lw	$3,0($3)
	beq	$4,$3,$L3
	nop

	lw	$2,%call16(__stack_chk_fail)($28)
	move	$25,$2
	.reloc	1f,R_MIPS_JALR,__stack_chk_fail
1:	jalr	$25
	nop

$L3:
	move	$sp,$fp
	.cfi_def_cfa_register 29
	lw	$31,44($sp)
	lw	$fp,40($sp)
	addiu	$sp,$sp,48
	.cfi_restore 30
	.cfi_restore 31
	.cfi_def_cfa_offset 0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	main
	.cfi_endproc
$LFE1021:
	.size	main, .-main
	.align	2
$LFB1022 = .
	.cfi_startproc
	.set	nomips16
	.set	nomicromips
	.ent	_Z41__static_initialization_and_destruction_0ii
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
	.frame	$fp,32,$31		# vars= 0, regs= 2/0, args= 16, gp= 8
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	.cfi_def_cfa_offset 32
	sw	$31,28($sp)
	sw	$fp,24($sp)
	.cfi_offset 31, -4
	.cfi_offset 30, -8
	move	$fp,$sp
	.cfi_def_cfa_register 30
	lui	$28,%hi(__gnu_local_gp)
	addiu	$28,$28,%lo(__gnu_local_gp)
	.cprestore	16
	sw	$4,32($fp)
	sw	$5,36($fp)
	lw	$3,32($fp)
	li	$2,1			# 0x1
	bne	$3,$2,$L6
	nop

	lw	$3,36($fp)
	li	$2,65535			# 0xffff
	bne	$3,$2,$L6
	nop

	lui	$2,%hi(_ZStL8__ioinit)
	addiu	$4,$2,%lo(_ZStL8__ioinit)
	lw	$2,%call16(_ZNSt8ios_base4InitC1Ev)($28)
	move	$25,$2
	.reloc	1f,R_MIPS_JALR,_ZNSt8ios_base4InitC1Ev
1:	jalr	$25
	nop

	lw	$28,16($fp)
	lui	$2,%hi(__dso_handle)
	addiu	$6,$2,%lo(__dso_handle)
	lui	$2,%hi(_ZStL8__ioinit)
	addiu	$5,$2,%lo(_ZStL8__ioinit)
	lw	$4,%got(_ZNSt8ios_base4InitD1Ev)($28)
	lw	$2,%call16(__cxa_atexit)($28)
	move	$25,$2
	.reloc	1f,R_MIPS_JALR,__cxa_atexit
1:	jalr	$25
	nop

	lw	$28,16($fp)
$L6:
	nop
	move	$sp,$fp
	.cfi_def_cfa_register 29
	lw	$31,28($sp)
	lw	$fp,24($sp)
	addiu	$sp,$sp,32
	.cfi_restore 30
	.cfi_restore 31
	.cfi_def_cfa_offset 0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	_Z41__static_initialization_and_destruction_0ii
	.cfi_endproc
$LFE1022:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.align	2
$LFB1023 = .
	.cfi_startproc
	.set	nomips16
	.set	nomicromips
	.ent	_GLOBAL__sub_I_main
	.type	_GLOBAL__sub_I_main, @function
_GLOBAL__sub_I_main:
	.frame	$fp,32,$31		# vars= 0, regs= 2/0, args= 16, gp= 8
	.mask	0xc0000000,-4
	.fmask	0x00000000,0
	.set	noreorder
	.set	nomacro
	addiu	$sp,$sp,-32
	.cfi_def_cfa_offset 32
	sw	$31,28($sp)
	sw	$fp,24($sp)
	.cfi_offset 31, -4
	.cfi_offset 30, -8
	move	$fp,$sp
	.cfi_def_cfa_register 30
	li	$5,65535			# 0xffff
	li	$4,1			# 0x1
	.option	pic0
	jal	_Z41__static_initialization_and_destruction_0ii
	nop

	.option	pic2
	move	$sp,$fp
	.cfi_def_cfa_register 29
	lw	$31,28($sp)
	lw	$fp,24($sp)
	addiu	$sp,$sp,32
	.cfi_restore 30
	.cfi_restore 31
	.cfi_def_cfa_offset 0
	j	$31
	nop

	.set	macro
	.set	reorder
	.end	_GLOBAL__sub_I_main
	.cfi_endproc
$LFE1023:
	.size	_GLOBAL__sub_I_main, .-_GLOBAL__sub_I_main
	.section	.ctors,"aw",@progbits
	.align	2
	.word	_GLOBAL__sub_I_main
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 5.3.1-14ubuntu2) 5.3.1 20160413"

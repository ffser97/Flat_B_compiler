	.text
	.file	"llbc"
	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:                                 # %entry
	pushq	%rax
.Ltmp0:
	.cfi_def_cfa_offset 16
	movl	$0, t(%rip)
	xorl	%eax, %eax
	testb	%al, %al
	jne	.LBB0_6
	.align	16, 0x90
.LBB0_1:                                # %while
                                        # =>This Loop Header: Depth=1
                                        #     Child Loop BB0_2 Depth 2
	movl	$1, ans(%rip)
	movl	$.L__unnamed_1, %edi
	movl	$a, %esi
	movl	$b, %edx
	movl	$n, %ecx
	callq	scanf
	movl	a(%rip), %eax
	xorl	%edx, %edx
	divl	n(%rip)
	movl	%edx, a(%rip)
	cmpl	$0, b(%rip)
	je	.LBB0_5
	.align	16, 0x90
.LBB0_2:                                # %while1
                                        #   Parent Loop BB0_1 Depth=1
                                        # =>  This Inner Loop Header: Depth=2
	testb	$1, b(%rip)
	je	.LBB0_4
# BB#3:                                 # %if
                                        #   in Loop: Header=BB0_2 Depth=2
	movl	ans(%rip), %eax
	imull	a(%rip), %eax
	xorl	%edx, %edx
	divl	n(%rip)
	movl	%edx, ans(%rip)
.LBB0_4:                                # %ifcont
                                        #   in Loop: Header=BB0_2 Depth=2
	movl	a(%rip), %eax
	imull	%eax, %eax
	xorl	%edx, %edx
	divl	n(%rip)
	movl	%edx, a(%rip)
	movl	b(%rip), %eax
	shrl	%eax
	movl	%eax, b(%rip)
	jne	.LBB0_2
.LBB0_5:                                # %whilecont2
                                        #   in Loop: Header=BB0_1 Depth=1
	movl	ans(%rip), %esi
	movl	$.L__unnamed_2, %edi
	callq	printf
	movl	t(%rip), %eax
	incl	%eax
	movl	%eax, t(%rip)
	cmpl	$1000000, %eax          # imm = 0xF4240
	jb	.LBB0_1
.LBB0_6:                                # %whilecont
	popq	%rax
	retq
.Lfunc_end0:
	.size	main, .Lfunc_end0-main
	.cfi_endproc

	.type	a,@object               # @a
	.comm	a,4,4
	.type	b,@object               # @b
	.comm	b,4,4
	.type	n,@object               # @n
	.comm	n,4,4
	.type	ans,@object             # @ans
	.comm	ans,4,4
	.type	t,@object               # @t
	.comm	t,4,4
	.type	.L__unnamed_1,@object   # @0
	.section	.rodata.str1.1,"aMS",@progbits,1
.L__unnamed_1:
	.asciz	"%d%d%d"
	.size	.L__unnamed_1, 7

	.type	.L__unnamed_2,@object   # @1
.L__unnamed_2:
	.asciz	"%d \n"
	.size	.L__unnamed_2, 5


	.section	".note.GNU-stack","",@progbits

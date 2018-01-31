//Alexandra Montgomery
// nCr.s


.globl nCr
	.type	nCr, @function
nCr:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%r12
	pushq	%rbx
	subq	$16, %rsp
	.cfi_offset 12, -24
	.cfi_offset 3, -32
	movl	%edi, -20(%rbp)
	movl	%esi, -24(%rbp)
	movl	-20(%rbp), %eax
	movl	%eax, %edi
	movl	$0, %eax
	call	Factorial
	movl	%eax, %ebx
	movl	-24(%rbp), %eax
	movl	%eax, %edi
	movl	$0, %eax
	call	Factorial
	movl	%eax, %r12d
	movl	-20(%rbp), %eax
	subl	-24(%rbp), %eax
	movl	%eax, %edi
	movl	$0, %eax
	call	Factorial
	movl	%r12d, %edx
	imull	%eax, %edx
	movl	%edx, %ecx
	movl	%ebx, %eax
	cltd
	idivl	%ecx
	addq	$16, %rsp
	popq	%rbx
	popq	%r12
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	nCr, .-nCr
	.globl	Factorial
	.type	Factorial, @function

.globl Factorial
	.type	Factorial, @function
Factorial:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	cmpl	$0, -4(%rbp)
	jne	.L4
	movl	$1, %eax
	jmp	.L5
.L4:
	movl	-4(%rbp), %eax
	subl	$1, %eax
	movl	%eax, %edi
	call	Factorial
	imull	-4(%rbp), %eax
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	Factorial, .-Factorial
	.ident	"GCC: (Debian 4.9.2-10) 4.9.2"
	.section	.note.GNU-stack,"",@progbits

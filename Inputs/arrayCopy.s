	.file	"arrayCopy.c"
	.text
	.p2align 4,,15
.globl main
	.type	main, @function
main:
	pushl	%ebp
	movl	%esp, %ebp
	subl	$32, %esp
	leal	-24(%ebp), %eax
	movl	$1, -32(%ebp)
	movl	$2, -28(%ebp)
	movl	$3, -24(%ebp)
	movl	$4, -20(%ebp)
	movl	$5, -16(%ebp)
	movl	$6, -12(%ebp)
	movl	$7, -8(%ebp)
	movl	$0, -4(%ebp)
	.p2align 4,,7
	.p2align 3
.L2:
	movl	(%eax), %edx
	addl	$4, %eax
	testl	%edx, %edx
	jne	.L2
	xorl	%eax, %eax
	leave
	ret
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.6 20110731 (Red Hat 4.4.6-3)"
	.section	.note.GNU-stack,"",@progbits

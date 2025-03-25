	.file	"main.c"
	.text
	.globl	global_ret_address
	.bss
	.align 8
	.type	global_ret_address, @object
	.size	global_ret_address, 8
global_ret_address:
	.zero	8
	.globl	called
	.align 4
	.type	called, @object
	.size	called, 4
called:
	.zero	4
	.globl	sfoo
	.align 16
	.type	sfoo, @object
	.size	sfoo, 24
sfoo:
	.zero	24
	.section	.rodata
.LC0:
	.string	"%d\t"
.LC1:
	.string	"exit bar!"
	.text
	.globl	bar
	.type	bar, @function
bar:
.LFB6:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$112, %rsp
	movl	called(%rip), %eax
	cmpl	$1, %eax
	jne	.L2
#APP
# 26 "main.c" 1
	movq %rbp, %rax
	
# 0 "" 2
#NO_APP
	movq	%rax, -80(%rbp)
	movq	-80(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -88(%rbp)
	movq	-88(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -96(%rbp)
	movq	-80(%rbp), %rax
	movq	%rax, -104(%rbp)
	movq	8+sfoo(%rip), %rax
	subq	-104(%rbp), %rax
	leaq	7(%rax), %rdx
	testq	%rax, %rax
	cmovs	%rdx, %rax
	sarq	$3, %rax
	movl	%eax, 16+sfoo(%rip)
	movl	16+sfoo(%rip), %eax
	cltq
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, sfoo(%rip)
	movl	$0, -4(%rbp)
	jmp	.L3
.L4:
	movq	8+sfoo(%rip), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	negq	%rdx
	leaq	(%rax,%rdx), %rcx
	movq	sfoo(%rip), %rdx
	movl	16+sfoo(%rip), %eax
	subl	-4(%rbp), %eax
	cltq
	salq	$2, %rax
	subq	$4, %rax
	addq	%rax, %rdx
	movl	(%rcx), %eax
	movl	%eax, (%rdx)
	addl	$1, -4(%rbp)
.L3:
	movl	16+sfoo(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L4
	movl	$0, -8(%rbp)
	jmp	.L5
.L6:
	movq	sfoo(%rip), %rax
	movl	-8(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -8(%rbp)
.L5:
	movl	16+sfoo(%rip), %eax
	cmpl	%eax, -8(%rbp)
	jl	.L6
	movl	$10, %edi
	call	putchar@PLT
	movq	-96(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -112(%rbp)
	movq	-80(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, global_ret_address(%rip)
	movq	-96(%rbp), %rax
	movq	-96(%rbp), %rdx
	movq	-112(%rbp), %rcx
#APP
# 49 "main.c" 1
	movq %rax, %rbp
	movq %rdx, %rsp
	jmp *%rcx
	
# 0 "" 2
#NO_APP
	jmp	.L7
.L2:
#APP
# 65 "main.c" 1
	movq %rbp, %rax
	
# 0 "" 2
#NO_APP
	movq	%rax, -24(%rbp)
#APP
# 70 "main.c" 1
	movq %rbp, %rax
	
# 0 "" 2
#NO_APP
	movq	%rax, -32(%rbp)
	movq	-24(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -40(%rbp)
	movq	-40(%rbp), %rax
	movq	%rax, 8+sfoo(%rip)
	movq	-24(%rbp), %rax
	movq	%rax, -48(%rbp)
	movq	8+sfoo(%rip), %rax
	subq	-48(%rbp), %rax
	leaq	7(%rax), %rdx
	testq	%rax, %rax
	cmovs	%rdx, %rax
	sarq	$3, %rax
	movl	%eax, 16+sfoo(%rip)
	movl	16+sfoo(%rip), %eax
	cltq
	salq	$2, %rax
	movq	%rax, %rdi
	call	malloc@PLT
	movq	%rax, sfoo(%rip)
	movl	$0, -12(%rbp)
	jmp	.L8
.L9:
	movq	8+sfoo(%rip), %rax
	movl	-12(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	negq	%rdx
	leaq	(%rax,%rdx), %rcx
	movq	sfoo(%rip), %rdx
	movl	16+sfoo(%rip), %eax
	subl	-12(%rbp), %eax
	cltq
	salq	$2, %rax
	subq	$4, %rax
	addq	%rax, %rdx
	movl	(%rcx), %eax
	movl	%eax, (%rdx)
	addl	$1, -12(%rbp)
.L8:
	movl	16+sfoo(%rip), %eax
	cmpl	%eax, -12(%rbp)
	jl	.L9
	movl	$0, -16(%rbp)
	jmp	.L10
.L11:
	movq	sfoo(%rip), %rax
	movl	-16(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	addl	$1, -16(%rbp)
.L10:
	movl	16+sfoo(%rip), %eax
	cmpl	%eax, -16(%rbp)
	jl	.L11
	movl	$10, %edi
	call	putchar@PLT
	movq	-40(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rax
	movq	(%rax), %rax
	movq	%rax, -64(%rbp)
	movq	-40(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, -72(%rbp)
	movq	-24(%rbp), %rax
	movq	8(%rax), %rax
	movq	%rax, global_ret_address(%rip)
	movq	-56(%rbp), %rax
	movq	-56(%rbp), %rdx
	movq	-72(%rbp), %rcx
#APP
# 99 "main.c" 1
	movq %rax, %rbp
	movq %rdx, %rsp
	jmp *%rcx
	
# 0 "" 2
#NO_APP
.L7:
	leaq	.LC1(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	bar, .-bar
	.section	.rodata
.LC2:
	.string	"Hello foo!"
.LC3:
	.string	"secret code %d!\n"
	.text
	.globl	foo
	.type	foo, @function
foo:
.LFB7:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$13, -4(%rbp)
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	puts@PLT
	movl	$0, %eax
	call	bar
	movl	-4(%rbp), %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	foo, .-foo
	.globl	gen
	.type	gen, @function
gen:
.LFB8:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
#APP
# 125 "main.c" 1
	movq %rbp, %rax
	
# 0 "" 2
#NO_APP
	movq	%rax, -16(%rbp)
	movl	$0, -4(%rbp)
	jmp	.L14
.L15:
	movq	sfoo(%rip), %rax
	movl	-4(%rbp), %edx
	movslq	%edx, %rdx
	salq	$2, %rdx
	addq	%rdx, %rax
	movl	(%rax), %eax
	cltq
#APP
# 132 "main.c" 1
	push %rax
# 0 "" 2
#NO_APP
	addl	$1, -4(%rbp)
.L14:
	movl	16+sfoo(%rip), %eax
	cmpl	%eax, -4(%rbp)
	jl	.L15
	movq	global_ret_address(%rip), %rax
#APP
# 139 "main.c" 1
	jmp *%rax
	
# 0 "" 2
#NO_APP
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	gen, .-gen
	.globl	start
	.type	start, @function
start:
.LFB9:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	leaq	foo(%rip), %rax
#APP
# 149 "main.c" 1
	call *%rax
	
# 0 "" 2
#NO_APP
	movl	$1, called(%rip)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE9:
	.size	start, .-start
	.section	.rodata
.LC4:
	.string	"\n%d\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB10:
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$0, %eax
	call	start
	movl	called(%rip), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	movl	$0, %eax
	call	gen
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE10:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:

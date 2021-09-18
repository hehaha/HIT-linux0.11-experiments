
ESP0 = 4
KERNEL_STACK = 12


.globl switch_to 

.align 2
switch_to:
    pushl %ebp
    movl %esp,%ebp
    pushl %ecx
    pushl %ebx
    pushl %eax
    movl 8(%ebp),%ebx
    cmpl %ebx,current
    je 1f
# 切换PCB
    movl %ebx,%eax
    xchgl %eax,current
# TSS中的内核栈指针的重写
    movl tss,%ecx 
    addl $4096,%ebx
    movl %ebx,ESP0(%ecx)
# 切换内核栈
    movl %esp,KERNEL_STACK(%eax)
    movl 8(%ebp),%ebx
    movl KERNEL_STACK(%ebx),%esp
# 切换LDT
    movl 12(%ebp),%ecx
    lldt %cx
    movl $0x17,%ecx
    mov %cx,%fs
# 和后面的 clts 配合来处理协处理器，由于和主题关系不大，此处不做论述
    cmpl %eax,last_task_used_math
    jne 1f
    clts
1:  
    popl %eax
    popl %ebx
    popl %ecx
    popl %ebp
    ret

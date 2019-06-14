.script:
# multiply
    #.string ",>,>++++++++[<------<------>>-]<<[>[>+>+<<-]>>[<<+>>-]<<<-]>>>++++++[<++++++++>-]<."
# hello world
    #.string "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>."
# numbers
    .string "+++++++[->+++++++<]>-.+.+.+.+.+.+.+.+.+."
 
.data
STDOUT = 1
SYSWRITE = 1
SYSEXIT = 60
EXIT_SUCCESS = 0
.asciz "./test"
.bss
.equ bufSize, 1024
.lcomm buf, bufSize
 
.text
    .global main
 
main:
 
    call _load_from_file
 
 
# reserve space
 
    # int tab[30000] -> 4B*30000
    # pointer* code,itr,code_end,ptr -> 8B*4
    # int loop_delta -> 4B*1
    # sum: 4*30000+8*4+4*1 = 120037
    pushq %rbp
    movq %rsp, %rbp
    subq $120037, %rsp
 
# create variables
 
    # int tab[30000]{};
    # set zeros with `memset`
    leaq -120037(%rbp), %rax # rax = tab;
    movq %rax, %rdi # arg1 = rax
    movl $0, %esi # arg2 = 0
    movl $120000, %edx # arg3 = 4*30000
    call memset # memset(tab, 0, 4*30000);
 
    # char* code = "script";
    movq $buf, -8(%rbp)
 
    # char* itr = code;
    movq $buf, -16(%rbp)
 
    # char* end = code;
    movq $buf, -24(%rbp)
    # end += strlen(code);
    movq -8(%rbp), %rdi # arg1 = code
    call strlen
    addq %rax, -24(%rbp)
 
    # int* ptr = tab;
    leaq -120036(%rbp), %rax
    movq %rax, -32(%rbp)
 
    # int loop_delta = 0;
    movl $0, -36(%rbp)
 
    # char ch = 'c';
    movb $99, -37(%rbp)
 
   
    # read the file
    movl %eax, %edi     # fd
    leaq buf, %rsi      # buf
    movl $bufSize, %edx # count
    movl $0, %eax       # SYS_READ
    syscall
 
 
# main while loop
.main_loop:
 
    # while(itr != end)
    movq -16(%rbp), %rax
    cmpq -24(%rbp), %rax
    je .close_main_loop
 
    # ch = *code_itr;
    movq -16(%rbp), %rax
    movl (%rax), %eax
    movb %al, -37(%rbp)
 
# if '+' ------------- new char --------------
    cmpb $'+', -37(%rbp)
    jne .if_char_plus
 
    # ++*ptr;
    movq -32(%rbp), %rax
    movl (%rax), %eax
    leal 1(%rax), %edx
    movq -32(%rbp), %rax
    movl %edx, (%rax)
 
    jmp .if_end
.if_char_plus:
 
# if '-' ------------- new char --------------
    cmpb $'-', -37(%rbp)
    jne .if_char_minus
 
    # --*ptr;
    movq -32(%rbp), %rax
    movl (%rax), %eax
    leal -1(%rax), %edx
    movq -32(%rbp), %rax
    movl %edx, (%rax)
 
    jmp .if_end
.if_char_minus:
 
# if '>' ------------- new char --------------
    cmpb $'>', -37(%rbp)
    jne .if_char_right_shift
 
    # ++ptr;
    addq $4, -32(%rbp)
 
    jmp .if_end
.if_char_right_shift:    
 
# if '<' ------------- new char --------------
    cmpb $'<', -37(%rbp)
    jne .if_char_left_shift
 
    # --ptr;
    subq $4, -32(%rbp)
 
    jmp .if_end
.if_char_left_shift:
 
# if '.' ------------- new char --------------
    cmpb $'.', -37(%rbp)
    jne .if_char_dot
 
    # putchar(*ptr);
    movq -32(%rbp), %rax
    movl (%rax), %edi
    call putchar
 
    jmp .if_end
.if_char_dot:
 
# if ',' ------------- new char --------------
    cmpb $',', -37(%rbp)
    jne .if_char_comma
 
    # *ptr = getchar
    call getchar
    movq -32(%rbp), %rdx
    movl %eax, (%rdx)
 
    jmp .if_end
.if_char_comma:
 
# if '[' ------------- new char --------------
    cmpb $'[', -37(%rbp)
    jne .if_char_begin_loop
 
# if(*ptr == 0) 'icbl_L1'
    #movq -32(%rbp), %rax
    #cmpl $0, (%rax)
    #jne .icbl_L1
 
    #movq -32(%rbp), %rax
    #movl $32, (%rax)
 
    movq -32(%rbp), %rax
    cmpl $0, (%rax)
    jne .icbl_L1
 
    # loop_delta = 0;
    movl $0, -36(%rbp)
 
# while(true) 'icbl_L2' -> 'icbl_L2end'
.icbl_L2:
 
    # ++itr;
    addq $1, -16(%rbp)
 
# if(*itr == ']') 'icbl_L3'
    movq -16(%rbp), %rax
    cmpb $']', (%rax)
    jne .icbl_L3
 
    # if(loop_delta == 0) break;
    cmpl $0, -36(%rbp)
    je .icbl_2Lend
 
    # --loop_delta;
    subl $1, -36(%rbp)
 
    jmp .icbl_L2
.icbl_L3:
 
# else if(*itr == '[') 'icbl_L4'
    movq -16(%rbp), %rax
    cmpb $'[', (%rax)
    jne .icbl_L4
 
    # ++loop_delta;
    addl $1, -36(%rbp)
 
.icbl_L4:
 
    jmp .icbl_L2 # 'while(true)'
.icbl_2Lend:
 
.icbl_L1: # 'if(*ptr == 0)'
 
    jmp .if_end
.if_char_begin_loop:
 
# if ']' ------------- new char --------------
    cmpb $']', -37(%rbp)
    jne .if_char_end_loop
 
    # loop_delta = 0;
    movl $0, -36(%rbp)
 
# while(true) 'icel_w1' -> 'icel_w1end'
.icel_w1:
 
    # --itr;
    subq $1, -16(%rbp)
 
# if(*itr == '[') 'icel_if1'
    movq -16(%rbp), %rax
    cmpb $'[', (%rax)
    jne .icel_if1
 
# if(loop_delta == 0) 'icel_if11'
    cmpl $0, -36(%rbp)
    jne .icel_if11
 
    # --itr;
    subq $1, -16(%rbp)
 
    # break;
    jmp .icel_w1end
 
.icel_if11:
 
    # --loop_delta;
    subl $1, -36(%rbp)
 
    jne .icel_if1
.icel_if1:
 
# else if(*itr == ']') 'icel_if2'
    movq -16(%rbp), %rax
    cmpb $']', (%rax)
    jne .icel_if2
 
    # ++loop_delta;
    addl $1, -36(%rbp)
 
.icel_if2:
 
    jmp .icel_w1
.icel_w1end:
 
    jmp .if_end
.if_char_end_loop:
 
.if_end:
 
    # ++itr;
    addq $1, -16(%rbp)
 
    jmp .main_loop
.close_main_loop:
 
    # return 0;
    movl $0, %eax
    leave
    ret
 
_load_from_file:
    push %rbp
    mov %rsp, %rbp
    push %rbx           # save rbx
    # get file name from parameter and put in to rdi
    # argv in rsi
    # open the file
 
    movq 8(%rsi), %rdi  # path
    movl $0, %esi       # flags
    movl $0666, %edx    # mode
    movl $2, %eax       # SYS_OPEN
    syscall
    movl %eax, %ebx     # save fd
   
    # read the file
    movl %eax, %edi     # fd
    leaq buf, %rsi      # buf
    movl $bufSize, %edx # count
    movl $0, %eax       # SYS_READ
    syscall
 
    # close the file
    movl %ebx, %esi     # fd
    movl $3, %eax       # SYS_CLOSE
    syscall
 
    exit:
    xor %eax, %eax      # return 0
    pop %rbx            # restore rbx
 
    mov %rbp, %rsp
    pop %rbp
    ret
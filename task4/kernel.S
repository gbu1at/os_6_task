  global start                    ; the entry symbol for ELF

    extern entry
    extern main

    section  .data 

    MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
    FLAGS        equ 0x0            ; multiboot flags
    CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum
                                    ; (magic number + checksum + flags should equal 0)

    section .text:                  ; start of the text (code) section
    align 4                         ; the code must be 4 byte aligned
        dd MAGIC_NUMBER             ; write the magic number to the machine code,
        dd FLAGS                    ; the flags,
        dd CHECKSUM                 ; and the checksum

    start:                          ; the loader label (defined as entry point in linker script)
      mov ebx, 0xb8000 ; VGA area base
      mov ecx, 80*25 ; console size

      ; Clear screen
      mov edx, 0x0020;  space symbol (0x20) on black background
    clear_loop:
      mov [ebx + ecx], edx
      dec ecx
      cmp ecx, -1
      jnz clear_loop
      call main
q:
	jmp q
    .loop:
        jmp .loop                   ; loop forever

global load_idt
global isr_timer_handler
extern timer_interrupt_handler
extern log

section .text

load_idt:
    mov eax, [esp + 4]
    lidt [eax]
    ret

isr_timer_handler:
    pusha
    call timer_interrupt_handler
    popa               
    iret                

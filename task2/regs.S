
    global read_gdtr
    global read_idtr
    global write_idtr

    section .data:                  
    align 4                         
value:
        dd 0xCCCCCCCC
        dd 0xFFFFFFFF

    section .text:                  ; start of the text (code) section

read_gdtr:
    sgdt [eax]
    ret

read_idtr:
    sidt [eax]
    ret


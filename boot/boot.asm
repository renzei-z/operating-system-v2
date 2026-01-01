	[BITS 16]
	[ORG 0x7C00]

_start:
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7C00

	mov [BOOT_DRIVE], dl 	; BIOS loads the BOOT_DRIVE into dl

	mov bx, 0x1000 		; Loading kernel into 0x1000
	mov dh, [LOADED_SECTORS]		; # of sectors
	mov dl, [BOOT_DRIVE] 	; Technically unnecessary
	call disk_load
	
	cli
	lgdt [gdt_descriptor]

	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	jmp CODE_SEG:init_pm


disk_load:
	push dx

	mov ah, 0x00
	int 0x13
	jc disk_error

	mov ah, 0x02
	mov al, dh
	mov ch, 0x00
	mov dh, 0x00
	mov cl, 0x02
	int 0x13
	jc disk_error
	pop dx
	ret
disk_error:
	mov ah, 0x0E
	mov al, 'E'
	int 0x10
	jmp $
	
[BITS 32]
init_pm:
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call 0x1000
	
	jmp $

	;;  -- GDT --
gdt_start:
	dq 0x0 			; Null Descriptor
gdt_code:
	dw 0xFFFF 		; Limit
	dw 0x0			; Base (0-15)
	db 0x0			; Base (16-23)
	db 10011010b    	; Access byte: Present(1) Ring(00) S(1) Code(1) Conforming(0) Readable(1)
	db 11001111b    	; Flags: Granularity(1) 32-bit(1) + Limit (bits 16-19)
	db 0x0			; Base (24-31)
gdt_data:
	dw 0xFFFF
	dw 0x0
	db 0x0
	db 10010010b		; Same as gdt_code, but Code(0) and Writable(1)
	db 11001111b
	db 0x0
gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start - 1 ; Size of GDT
	dd gdt_start		   ; Memory addr of GDT

	CODE_SEG equ gdt_code - gdt_start
	DATA_SEG equ gdt_data - gdt_start

	LOADED_SECTORS db 30
	BOOT_DRIVE db 0
	
	times 510-($-$$) db 0
	dw 0xAA55

	

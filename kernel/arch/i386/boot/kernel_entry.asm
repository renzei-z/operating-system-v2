	[BITS 32]
	section .text.entry
	[EXTERN kmain]
	global _kernel_start
_kernel_start:
	call kmain
	jmp $

;; This should be extracted to its own file
;; 	global outb
;; outb:
;; 	mov dx, [esp + 4]
;; 	mov al, [esp + 8]
;; 	out dx, al
;; 	ret

;; 	global inb
;; inb:
;; 	push ebp
;; 	mov ebp, esp
;; 	xor eax, eax
;; 	mov dx, [esp + 8]
;; 	in al, dx

;; 	mov esp, ebp
;; 	pop ebp
;; 	ret

	global idt_load
	extern idtptr
idt_load:
	lidt [idtptr]
	ret

	global gdt_flush
gdt_flush:
	mov eax, [esp+4]
	lgdt [eax]
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	jmp 0x08:.flush
.flush:
	ret

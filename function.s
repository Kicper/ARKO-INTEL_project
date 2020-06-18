section .data
four:   dq 4
zero:	dq 0
three:	dq 3
err1 db 'Incorrect size of first map!', 0xa
len1 equ $ - err1
err2 db 'Incorrect size of second map', 0xa
len2 equ $ - err2


section .text
global function



	; 0x7fffffffdcf0 - bitmap1 address
	; 0x7fffffffdca0 - bitmap2 address

	; rdi - bitmap1
	; rsi - bitmap2
	; rcx - width2
	; rdx - width1
	; r8 - height1
	; r9 - height2



function:
	push rbp
	mov rbp, rsp

	mov r15, rdi	; save copy of string begin pointer
	cmp rdx, [four]
	jne first_error
	cmp r8, [four]
	jne first_error
	cmp rcx, [four]
	jb second_error
	cmp r9, [four]
	jb second_error

	mov r10, rdx
	mov r11, rcx
	shl r10, 16
	shl r11, 16

	mov ax, dx
	mov bl, [four]
	div bl
	mov bl, ah
	mov r10b, bl		; r10b is padding of width1
	mov ax, cx
	mov bl, [four]
	div bl
	mov bl, ah
	mov r11b, bl		; r11b is padding of width2

	; r10: | --width1-- | --padd1-- |
	; r11: | --width2-- | --padd2-- |
	;      | --16bits-- | --16bits- |

	mov rax, r9
	sub rax, [four]
	mov r12, rax		; (r12+1) is number of bitmap1 which can be fitted to bitmap2 height
	shl r12, 16
	mov rax, rcx
	sub rax, [four]
	mov r13, rax		; (r13+1) is number of bitmap1 which can be fitted to bitmap2 width
	shl r13, 16
	mov ax, [zero]
	dec ax
	mov r12w, ax		; r12w iterator in height
	shl r8, 16
	shl r9, 16
	mov r15, 1
	jmp loop_height

	; r12: | --num_h-- | --iter_with_fields_1-- |
	; r13: | --num_w-- | --iter_with_fields_2-- |
	; r8:  | ---h1---- | ----iter_IN_field_1--- |
	; r9:  | ---h2---- | ----iter_IN_field_2--- |
	; r15: | ---sum--- | ---------flag--------- |
	;      | --16bits- | --------16bits-------- |



loop_height:
	mov rbx, r12
	shr rbx, 16
	cmp bx, r12w
	jz end
	inc r12w
	mov ax, [zero]
	dec ax
	mov r13w, ax		; r13w iterator in width
	jmp loop_width



loop_width:
	cmp r15w, [zero]
	jz increase
	mov rbx, r13
	shr rbx, 16
	cmp bx, r13w
	jz loop_height
	inc r13w
	mov rbx, r11
	shr rbx, 16
	mov ax, bx
	mov cx, [three]
	mul cx
	add ax, r11w
	mul r12w
	mov cx, ax
	mov ax, r13w
	mov dx, [three]
	mul dx
	add ax, cx
	mov r14w, ax		; r14w is a pointer to left-down pixel in bitmap2
	mov r8w, [zero]
	dec r8w				; set inner iterator r8w to -1
	mov r15w, [zero] 
	jmp choose_pixel_height	



choose_pixel_height:
	cmp [three], r8w
	jz loop_width
	inc r8w
	mov r9w, [zero]
	dec r9w				; set inner iterator r9w to -1
	jmp choose_pixel_width



choose_pixel_width:
	mov rax, [zero]
	mov rbx, [zero]
	mov rcx, [zero]
	mov rdx, [zero]
	cmp [three], r9w
	jz choose_pixel_height
	inc r9w
	mov rbx, r11
	shr rbx, 16
	mov ax, bx
	mov dx, [three]
	mul dx
	add ax, r11w
	mul r8w
	mov bx, ax
	mov ax, r9w
	mov dx, [three]
	mul dx
	add ax, bx			; inner shift for bitmap2
	mov rbx, rsi		; bitmap2 address
	mov dx, r14w
	add bx, dx			; bitmap2 address with 4x4 shift
	add bx, ax			; bitmap2 address with 4x4 shift and inner shift for bitmap2
	mov cx, 12
	mov ax, cx
	mul r8w
	mov cx, ax
	mov ax, [three]
	mul r9w
	add ax, cx			; inner shift for bitmap1
	mov rcx, rdi		; bitmap1 address
	add cx, ax			; bitmap1 address with inner shift for bitmap1
	mov al, [rbx]
	mov dl, [rcx]
	cmp al, dl
	jnz change_flag
	inc rbx
	inc rcx
	mov al, [rbx]
	mov dl, [rcx]
	cmp al, dl
	jnz change_flag
	inc rbx
	inc rcx
	mov al, [rbx]
	mov dl, [rcx]
	cmp al, dl
	jnz change_flag
	jmp choose_pixel_width



change_flag:
	mov r15w, 1
	jmp loop_width



increase:
	mov rax, r15
	shr rax, 16
	inc ax
	mov r15, rax
	shl r15, 16
	mov r15w, 1
	jmp loop_width



first_error:
	mov	edx, len1
	mov	ecx, err1
	mov	ebx, 1
	mov	eax, 4
	int	0x80
	jmp end



second_error:
	mov	edx, len2
	mov	ecx, err2
	mov	ebx, 1
	mov	eax, 4
	int	0x80
	jmp end



end:
	mov [rdi], r15b
	shr r15, 16
	mov [rsi], r15b

	mov rsp, rbp
	pop rbp
	ret


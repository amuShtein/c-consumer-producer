section .text

global enter_region
global leave_region

enter_region:
	push ebp
	mov ebp, esp

	mov eax, dword[ebp+8]
	.loop:
	mov ecx, 1
	xchg ecx, dword[eax]


	cmp ecx, 0
	jne .loop

	mov esp, ebp
	pop ebp
	ret

leave_region:
	push ebp
	mov ebp, esp

	mov eax, dword[ebp+8]
	mov dword[eax], 0

	mov esp, ebp
	pop ebp
	ret
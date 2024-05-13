PUBLIC target_decorator
.data
	extern setStructure:PROC

.code
target_decorator proc
	push rax
	push rcx
	mov rcx, rax
	call setStructure
	pop rcx
	pop rax
	mov rcx,[rax+08]
	mov [r13+000006B0h],rcx
	mov rcx,r14
	ret
target_decorator endp
end
PUBLIC bullet_decorator
.data
	extern setBulletStructure:PROC

.code
bullet_decorator proc
	push rax
	push rcx
	mov rcx, rbx
	call setBulletStructure
	pop rcx
	pop rax
	movss DWORD ptr [rbx+00000B14h],xmm6
	movss DWORD ptr [rbx+00000B18h],xmm6
;	movaps xmm6,[rsp+40h]
;	add rsp,50h
;	pop rbx
	ret
bullet_decorator endp
end
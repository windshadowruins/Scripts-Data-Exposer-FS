PUBLIC rmr_decorator
.data
	extern getRootMotionReductionFactor:PROC
    
.code
rmr_decorator proc
    ;movss xmm6 ,dword ptr [r8 + 0x4]
    lea rax, [r10  + 50h]
    push rax
    push rcx
    mov rcx, r8
    call getRootMotionReductionFactor
    pop rcx
    pop rax
    movss xmm6, dword ptr [r8 + 04h]
    subss xmm6, dword ptr [r8]
    ;movaps xmmword ptr [r10 + 20h ], XMM7
    ret
rmr_decorator endp
END

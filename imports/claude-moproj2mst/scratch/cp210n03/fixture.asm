EXTERN g_fail:DWORD
PUBLIC open_cont, duplicate_cont, factory_cont, cast_cont, project_cont, acceptance_cont
.code
synthetic_open PROC
sub rsp,28h
mov ecx,700000h
mov edx,7018d8h
call outer
open_cont::
nop
add rsp,28h
ret
synthetic_open ENDP
outer PROC
mov [rsp+10h],rdx
mov [rsp+8h],rcx
sub rsp,1d8h
call duplicate
duplicate_cont::
test rax,rax
jnz failed
call factory
factory_cont::
test rax,rax
jz failed
call cast
cast_cont::
test rax,rax
jz failed
call project
project_cont::
test rax,rax
jz failed
call acceptance
acceptance_cont::
test rax,rax
jz failed
mov eax,100h
jmp done
failed:
xor eax,eax
done:
add rsp,1d8h
ret
outer ENDP
duplicate PROC
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
xor eax,eax
cmp g_fail,0
je duplicate_fail
mov eax,0
ret
duplicate_fail:
mov eax,1
ret
duplicate ENDP
factory PROC
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
xor eax,eax
cmp g_fail,1
je factory_fail
mov eax,100h
ret
factory_fail:
mov eax,0
ret
factory ENDP
cast PROC
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
xor eax,eax
cmp g_fail,2
je cast_fail
mov eax,100h
ret
cast_fail:
mov eax,0
ret
cast ENDP
project PROC
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
xor eax,eax
cmp g_fail,3
je project_fail
mov eax,100h
ret
project_fail:
mov eax,0
ret
project ENDP
acceptance PROC
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
xor eax,eax
cmp g_fail,4
je acceptance_fail
mov eax,101h
ret
acceptance_fail:
mov eax,0
ret
acceptance ENDP
END

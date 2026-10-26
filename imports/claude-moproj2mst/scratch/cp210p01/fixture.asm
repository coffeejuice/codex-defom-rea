EXTERN g_fail:DWORD
EXTERN g_regcheck:DWORD
PUBLIC open_cont, duplicate_cont, factory_cont, cast_cont, project_cont, acceptance_cont
.code
synthetic_open PROC
push rbx
push rsi
push rdi
push rbp
push r12
push r13
push r14
push r15
sub rsp,38h
mov [rsp+20h],rsp
mov rbx,17
mov rsi,34
mov rdi,51
mov rbp,68
mov r12,85
mov r13,102
mov r14,119
mov r15,136
mov ecx,700000h
mov edx,7018d8h
call outer
open_cont::
mov [rsp+28h],rax
mov rax,rsp
cmp rax,[rsp+20h]
jne register_bad
cmp rbx,17
jne register_bad
cmp rsi,34
jne register_bad
cmp rdi,51
jne register_bad
cmp rbp,68
jne register_bad
cmp r12,85
jne register_bad
cmp r13,102
jne register_bad
cmp r14,119
jne register_bad
cmp r15,136
jne register_bad
cmp rcx,700000h
jne register_bad
cmp rdx,7018d8h
jne register_bad
mov g_regcheck,1
register_bad:
mov rax,[rsp+28h]
add rsp,38h
pop r15
pop r14
pop r13
pop r12
pop rbp
pop rdi
pop rsi
pop rbx
ret
synthetic_open ENDP
outer PROC
db 048h,089h,054h,024h,010h,048h,089h,04ch,024h,008h,048h,081h,0ech,0d8h,001h,000h,000h
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
db 048h,089h,054h,024h,010h,048h,089h,04ch,024h,008h,048h,081h,0ech,0b8h,000h,000h,000h
xor eax,eax
cmp g_fail,0
je duplicate_fail
mov eax,0
jmp duplicate_done
duplicate_fail:
mov eax,1
duplicate_done:
add rsp,0b8h
ret
duplicate ENDP
factory PROC
db 048h,089h,054h,024h,010h,048h,089h,04ch,024h,008h,048h,083h,0ech,048h
xor eax,eax
cmp g_fail,1
je factory_fail
mov eax,100h
jmp factory_done
factory_fail:
mov eax,0
factory_done:
add rsp,48h
ret
factory ENDP
cast PROC
db 048h,089h,05ch,024h,010h,048h,089h,074h,024h,018h,057h,041h,054h,041h,055h,041h,056h,041h,057h,048h,083h,0ech,060h
xor eax,eax
cmp g_fail,2
je cast_fail
mov eax,100h
jmp cast_done
cast_fail:
mov eax,0
cast_done:
add rsp,60h
pop r15
pop r14
pop r13
pop r12
pop rdi
mov rsi,[rsp+18h]
mov rbx,[rsp+10h]
ret
cast ENDP
project PROC
db 048h,089h,04ch,024h,008h,048h,081h,0ech,088h,000h,000h,000h,048h,0c7h,044h,024h,048h,0feh,0ffh,0ffh,0ffh
xor eax,eax
cmp g_fail,3
je project_fail
mov eax,100h
jmp project_done
project_fail:
mov eax,0
project_done:
add rsp,88h
ret
project ENDP
acceptance PROC
db 048h,089h,054h,024h,010h,048h,089h,04ch,024h,008h,048h,081h,0ech,068h,001h,000h,000h
xor eax,eax
cmp g_fail,4
je acceptance_fail
mov eax,101h
jmp acceptance_done
acceptance_fail:
mov eax,0
acceptance_done:
add rsp,168h
ret
acceptance ENDP
END

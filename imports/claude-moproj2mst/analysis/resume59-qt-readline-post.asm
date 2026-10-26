Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C3380: 40 57              push        rdi
  00000000670C3382: 48 83 EC 30        sub         rsp,30h
  00000000670C3386: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C338F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670C3394: 48 8B D9           mov         rbx,rcx
  00000000670C3397: 48 8B 41 28        mov         rax,qword ptr [rcx+28h]
  00000000670C339B: 48 85 C0           test        rax,rax
  00000000670C339E: 74 1A              je          00000000670C33BA
  00000000670C33A0: 01 51 30           add         dword ptr [rcx+30h],edx
  00000000670C33A3: 48 8B 00           mov         rax,qword ptr [rax]
  00000000670C33A6: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000670C33A9: 39 4B 30           cmp         dword ptr [rbx+30h],ecx
  00000000670C33AC: 0F 8E C2 00 00 00  jle         00000000670C3474
  00000000670C33B2: 89 4B 30           mov         dword ptr [rbx+30h],ecx
  00000000670C33B5: E9 BA 00 00 00     jmp         00000000670C3474
  00000000670C33BA: 01 91 A0 00 00 00  add         dword ptr [rcx+00000000000000A0h],edx
  00000000670C33C0: 44 8B 81 A0 00 00  mov         r8d,dword ptr [rcx+00000000000000A0h]
                    00
  00000000670C33C7: 48 8B 81 98 00 00  mov         rax,qword ptr [rcx+0000000000000098h]
                    00
  00000000670C33CE: 44 3B 40 08        cmp         r8d,dword ptr [rax+8]
  00000000670C33D2: 7C 64              jl          00000000670C3438
  00000000670C33D4: C7 81 A0 00 00 00  mov         dword ptr [rcx+00000000000000A0h],0
                    00 00 00 00
  00000000670C33DE: 48 8D 05 4B D7 22  lea         rax,[00000000672F0B30h]
                    00
  00000000670C33E5: 48 39 81 98 00 00  cmp         qword ptr [rcx+0000000000000098h],rax
                    00
  00000000670C33EC: 74 34              je          00000000670C3422
  00000000670C33EE: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670C33F3: F0 83 05 35 D7 22  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670C33FB: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670C3400: 48 81 C1 98 00 00  add         rcx,98h
                    00
  00000000670C3407: E8 C4 CF FA FF     call        00000000670703D0
  00000000670C340C: 90                 nop
  00000000670C340D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670C3412: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670C3416: 75 0A              jne         00000000670C3422
  00000000670C3418: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670C341D: E8 AE CC FA FF     call        00000000670700D0
  00000000670C3422: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670C3425: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670C3428: FF 50 70           call        qword ptr [rax+70h]
  00000000670C342B: 48 8B D0           mov         rdx,rax
  00000000670C342E: 48 8B CB           mov         rcx,rbx
  00000000670C3431: E8 6A EF FF FF     call        00000000670C23A0
  00000000670C3436: EB 3C              jmp         00000000670C3474
  00000000670C3438: 41 81 F8 00 40 00  cmp         r8d,4000h
                    00
  00000000670C343F: 7E 33              jle         00000000670C3474
  00000000670C3441: 33 D2              xor         edx,edx
  00000000670C3443: 48 81 C1 98 00 00  add         rcx,98h
                    00
  00000000670C344A: E8 11 FA FA FF     call        0000000067072E60
  00000000670C344F: 48 8B D0           mov         rdx,rax
  00000000670C3452: 48 8D 8B 98 00 00  lea         rcx,[rbx+0000000000000098h]
                    00
  00000000670C3459: E8 72 CF FA FF     call        00000000670703D0
  00000000670C345E: 8B 83 A0 00 00 00  mov         eax,dword ptr [rbx+00000000000000A0h]
  00000000670C3464: 01 83 A4 00 00 00  add         dword ptr [rbx+00000000000000A4h],eax
  00000000670C346A: C7 83 A0 00 00 00  mov         dword ptr [rbx+00000000000000A0h],0
                    00 00 00 00
  00000000670C3474: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670C3479: 48 83 C4 30        add         rsp,30h
  00000000670C347D: 5F                 pop         rdi
  00000000670C347E: C3                 ret
  00000000670C347F: CC                 int         3
  00000000670C3480: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670C3485: 57                 push        rdi
  00000000670C3486: 48 83 EC 20        sub         rsp,20h
  00000000670C348A: 48 8B 41 28        mov         rax,qword ptr [rcx+28h]
  00000000670C348E: 48 8B FA           mov         rdi,rdx
  00000000670C3491: 48 8B D9           mov         rbx,rcx
  00000000670C3494: 48 85 C0           test        rax,rax
  00000000670C3497: 74 0D              je          00000000670C34A6
  00000000670C3499: 48 8B 00           mov         rax,qword ptr [rax]
  00000000670C349C: 44 8B 40 08        mov         r8d,dword ptr [rax+8]
  00000000670C34A0: 44

  Summary

      1A7000 .text

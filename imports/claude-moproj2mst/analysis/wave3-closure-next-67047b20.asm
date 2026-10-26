Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067047B20: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000067047B25: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000067047B2A: 53                 push        rbx
  0000000067047B2B: 56                 push        rsi
  0000000067047B2C: 57                 push        rdi
  0000000067047B2D: 41 54              push        r12
  0000000067047B2F: 41 55              push        r13
  0000000067047B31: 41 56              push        r14
  0000000067047B33: 41 57              push        r15
  0000000067047B35: 48 83 EC 40        sub         rsp,40h
  0000000067047B39: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067047B42: 41 8B F9           mov         edi,r9d
  0000000067047B45: 4C 8B FA           mov         r15,rdx
  0000000067047B48: 4C 8B E1           mov         r12,rcx
  0000000067047B4B: B9 28 00 00 00     mov         ecx,28h
  0000000067047B50: E8 31 0F 13 00     call        0000000067178A86
  0000000067047B55: 48 8B D8           mov         rbx,rax
  0000000067047B58: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000067047B5D: 45 33 F6           xor         r14d,r14d
  0000000067047B60: 4C 89 30           mov         qword ptr [rax],r14
  0000000067047B63: 4C 89 70 08        mov         qword ptr [rax+8],r14
  0000000067047B67: C7 40 10 01 00 00  mov         dword ptr [rax+10h],1
                    00
  0000000067047B6E: 41 8B 44 24 14     mov         eax,dword ptr [r12+14h]
  0000000067047B73: 89 43 14           mov         dword ptr [rbx+14h],eax
  0000000067047B76: 89 7B 18           mov         dword ptr [rbx+18h],edi
  0000000067047B79: 41 0F B7 44 24 1C  movzx       eax,word ptr [r12+1Ch]
  0000000067047B7F: 66 89 43 1C        mov         word ptr [rbx+1Ch],ax
  0000000067047B83: 41 0F B7 44 24 1E  movzx       eax,word ptr [r12+1Eh]
  0000000067047B89: 66 89 43 1E        mov         word ptr [rbx+1Eh],ax
  0000000067047B8D: 41 8B 44 24 20     mov         eax,dword ptr [r12+20h]
  0000000067047B92: 89 43 20           mov         dword ptr [rbx+20h],eax
  0000000067047B95: 41 8B C6           mov         eax,r14d
  0000000067047B98: 8B BC 24 A0 00 00  mov         edi,dword ptr [rsp+00000000000000A0h]
                    00
  0000000067047B9F: 83 FF 08           cmp         edi,8
  0000000067047BA2: 0F 9F C0           setg        al
  0000000067047BA5: 83 E0 01           and         eax,1
  0000000067047BA8: 03 C0              add         eax,eax
  0000000067047BAA: 83 C8 01           or          eax,1
  0000000067047BAD: 89 43 24           mov         dword ptr [rbx+24h],eax
  0000000067047BB0: 49 63 44 24 20     movsxd      rax,dword ptr [r12+20h]
  0000000067047BB5: 85 C0              test        eax,eax
  0000000067047BB7: 0F 84 D4 00 00 00  je          0000000067047C91
  0000000067047BBD: 48 8B C8           mov         rcx,rax
  0000000067047BC0: B8 08 00 00 00     mov         eax,8
  0000000067047BC5: 48 F7 E1           mul         rax,rcx
  0000000067047BC8: 48 C7 C1 FF FF FF  mov         rcx,0FFFFFFFFFFFFFFFFh
                    FF
  0000000067047BCF: 48 0F 40 C1        cmovo       rax,rcx
  0000000067047BD3: 48 8B C8           mov         rcx,rax
  0000000067047BD6: E8 75 0D 13 00     call        0000000067178950
  0000000067047BDB: 48 89 43 08        mov         qword ptr [rbx+8],rax
  0000000067047BDF: 44 89 B4 24 98 00  mov         dword ptr [rsp+0000000000000098h],r14d
                    00 00
  0000000067047BE7: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  0000000067047BF0: 45 3B 74 24 20     cmp         r14d,dword ptr [r12+20h]
  0000000067047BF5: 0F 8D 96 00 00 00  jge         0000000067047C91
  0000000067047BFB: 49 63 CE           movsxd      rcx,r14d
  0000000067047BFE: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  0000000067047C02: 4C 8D 2C C8        lea         r13,[rax+rcx*8]
  0000000067047C06: 4C 89 6C 24 28     mov         qword ptr [rsp+28h],r13
  0000000067047C0B: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  0000000067047C10: 48 8B 34 C8        mov         rsi,qword ptr [rax+rcx*8]
  0000000067047C14: 49 3B F4           cmp         rsi,r12
  0000000067047C17: 74 64              je          0000000067047C7D
  0000000067047C19: 41 F6 44 24 24 02  test        byte ptr [r12+24h],2
  0000000067047C1F: 74 17              je          0000000067047C38
  0000000067047C21: 48 63 D7           movsxd      rdx,edi
  0000000067047C24: 49 63 4C 24 18     movsxd      rcx,dword ptr [r12+18h]
  0000000067047C29: E8 72 88 FD FF     call        00000000670204A0
  0000000067047C2E: 48 8B F8           mov         rdi,rax
  0000000067047C31: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000067047C36: EB 12              jmp         0000000067047C4A
  0000000067047C38: 49 63 4C 24 18     movsxd      rcx,dword ptr [r12+18h]
  0000000067047C3D: E8 5E 87 FD FF     call        00000000670203A0
  0000000067047C42: 48 8B F8           mov         rdi,rax
  0000000067047C45: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000067047C4A: 48 85 FF           test        rdi,rdi
  0000000067047C4D: 75 06              jne         0000000067047C55
  0000000067047C4F: E8 5C 43 FD FF     call        000000006701BFB0
  0000000067047C54: 90                 nop
  0000000067047C55: 48 8B D7           mov         rdx,rdi
  0000000067047C58: 48 8B CE           mov         rcx,rsi
  0000000067047C5B: 41 FF D7           call        r15
  0000000067047C5E: 90                 nop
  0000000067047C5F: 8B 46 08           mov         eax,dword ptr [rsi+8]
  0000000067047C62: 89 47 08           mov         dword ptr [rdi+8],eax
  0000000067047C65: 49 89 7D 00        mov         qword ptr [r13],rdi
  0000000067047C69: 4C 8B EF           mov         r13,rdi
  0000000067047C6C: 48 89 7C 24 28     mov         qword ptr [rsp+28h],rdi
  0000000067047C71: 48 8B 36           mov         rsi,qword ptr [rsi]
  0000000067047C74: 8B BC 24 A0 00 00  mov         edi,dword ptr [rsp+00000000000000A0h]
                    00
  0000000067047C7B: EB 97              jmp         0000000067047C14
  0000000067047C7D: 49 89 5D 00        mov         qword ptr [r13],rbx
  0000000067047C81: 41 FF C6           inc         r14d
  0000000067047C84: 44 89 B4 24 98 00  mov         dword ptr [rsp+0000000000000098h],r14d
                    00 00
  0000000067047C8C: E9 5F FF FF FF     jmp         0000000067047BF0
  0000000067047C91: 48 8B C3           mov         rax,rbx
  0000000067047C94: 48 83 C4 40        add         rsp,40h
  0000000067047C98: 41 5F              pop         r15
  0000000067047C9A: 41 5E              pop         r14
  0000000067047C9C: 41 5D              pop         r13
  0000000067047C9E: 41 5C              pop         r12
  0000000067047CA0: 5F                 pop         rdi
  0000000067047CA1: 5E                 pop         rsi
  0000000067047CA2: 5B                 pop         rbx
  0000000067047CA3: C3                 ret

  Summary

      1A7000 .text

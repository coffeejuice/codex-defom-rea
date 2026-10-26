Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B6010: 40 56              push        rsi
  00000000670B6012: 57                 push        rdi
  00000000670B6013: 41 54              push        r12
  00000000670B6015: 48 83 EC 30        sub         rsp,30h
  00000000670B6019: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B6022: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000670B6027: 48 89 6C 24 60     mov         qword ptr [rsp+60h],rbp
  00000000670B602C: 49 8B D8           mov         rbx,r8
  00000000670B602F: 48 8B F2           mov         rsi,rdx
  00000000670B6032: 4C 8B E1           mov         r12,rcx
  00000000670B6035: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000670B6039: C7 87 20 01 00 00  mov         dword ptr [rdi+0000000000000120h],0
                    00 00 00 00
  00000000670B6043: 48 8D 8F 90 00 00  lea         rcx,[rdi+0000000000000090h]
                    00
  00000000670B604A: 48 8D 05 DF AA 23  lea         rax,[00000000672F0B30h]
                    00
  00000000670B6051: 48 83 CD FF        or          rbp,0FFFFFFFFFFFFFFFFh
  00000000670B6055: 48 39 01           cmp         qword ptr [rcx],rax
  00000000670B6058: 74 31              je          00000000670B608B
  00000000670B605A: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000000670B605F: F0 83 05 C9 AA 23  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670B6067: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000000670B606C: E8 5F A3 FB FF     call        00000000670703D0
  00000000670B6071: 90                 nop
  00000000670B6072: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  00000000670B6077: 8B CD              mov         ecx,ebp
  00000000670B6079: F0 0F C1 08        lock xadd   dword ptr [rax],ecx
  00000000670B607D: 03 CD              add         ecx,ebp
  00000000670B607F: 75 0A              jne         00000000670B608B
  00000000670B6081: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000000670B6086: E8 45 A0 FB FF     call        00000000670700D0
  00000000670B608B: C6 87 F8 00 00 00  mov         byte ptr [rdi+00000000000000F8h],1
                    01
  00000000670B6092: 8B 87 88 00 00 00  mov         eax,dword ptr [rdi+0000000000000088h]
  00000000670B6098: 83 E0 20           and         eax,20h
  00000000670B609B: 0F 94 C0           sete        al
  00000000670B609E: 84 C0              test        al,al
  00000000670B60A0: 74 5E              je          00000000670B6100
  00000000670B60A2: 48 63 8F 18 01 00  movsxd      rcx,dword ptr [rdi+0000000000000118h]
                    00
  00000000670B60A9: 48 03 CB           add         rcx,rbx
  00000000670B60AC: 48 81 F9 00 40 00  cmp         rcx,4000h
                    00
  00000000670B60B3: 7E 14              jle         00000000670B60C9
  00000000670B60B5: 49 8B CC           mov         rcx,r12
  00000000670B60B8: E8 43 FD FF FF     call        00000000670B5E00
  00000000670B60BD: 84 C0              test        al,al
  00000000670B60BF: 75 08              jne         00000000670B60C9
  00000000670B60C1: 48 8B C5           mov         rax,rbp
  00000000670B60C4: E9 B1 00 00 00     jmp         00000000670B617A
  00000000670B60C9: 48 81 FB 00 40 00  cmp         rbx,4000h
                    00
  00000000670B60D0: 7F 2E              jg          00000000670B6100
  00000000670B60D2: 48 8D 8F 00 01 00  lea         rcx,[rdi+0000000000000100h]
                    00
  00000000670B60D9: 8B D3              mov         edx,ebx
  00000000670B60DB: E8 60 F2 FF FF     call        00000000670B5340
  00000000670B60E0: 48 83 FB 01        cmp         rbx,1
  00000000670B60E4: 75 07              jne         00000000670B60ED
  00000000670B60E6: 0F B6 0E           movzx       ecx,byte ptr [rsi]
  00000000670B60E9: 88 08              mov         byte ptr [rax],cl
  00000000670B60EB: EB 0E              jmp         00000000670B60FB
  00000000670B60ED: 4C 8B C3           mov         r8,rbx
  00000000670B60F0: 48 8B D6           mov         rdx,rsi
  00000000670B60F3: 48 8B C8           mov         rcx,rax
  00000000670B60F6: E8 A3 29 0C 00     call        0000000067178A9E
  00000000670B60FB: 48 8B C3           mov         rax,rbx
  00000000670B60FE: EB 7A              jmp         00000000670B617A
  00000000670B6100: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B6107: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670B610A: 4C 8B C3           mov         r8,rbx
  00000000670B610D: 48 8B D6           mov         rdx,rsi
  00000000670B6110: FF 90 F0 00 00 00  call        qword ptr [rax+00000000000000F0h]
  00000000670B6116: 48 8B F0           mov         rsi,rax
  00000000670B6119: 48 85 C0           test        rax,rax
  00000000670B611C: 79 59              jns         00000000670B6177
  00000000670B611E: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B6125: E8 E6 18 FF FF     call        00000000670A7A10
  00000000670B612A: 8B D8              mov         ebx,eax
  00000000670B612C: B8 02 00 00 00     mov         eax,2
  00000000670B6131: 83 FB 08           cmp         ebx,8
  00000000670B6134: 0F 44 D8           cmove       ebx,eax
  00000000670B6137: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670B613C: 48 8B 8F F0 00 00  mov         rcx,qword ptr [rdi+00000000000000F0h]
                    00
  00000000670B6143: E8 D8 18 FF FF     call        00000000670A7A20
  00000000670B6148: 90                 nop
  00000000670B6149: 89 9F 20 01 00 00  mov         dword ptr [rdi+0000000000000120h],ebx
  00000000670B614F: 48 8D 8F 90 00 00  lea         rcx,[rdi+0000000000000090h]
                    00
  00000000670B6156: 48 8B D0           mov         rdx,rax
  00000000670B6159: E8 72 A2 FB FF     call        00000000670703D0
  00000000670B615E: 90                 nop
  00000000670B615F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670B6164: F0 0F C1 28        lock xadd   dword ptr [rax],ebp
  00000000670B6168: 83 C5 FF           add         ebp,0FFFFFFFFh
  00000000670B616B: 75 0A              jne         00000000670B6177
  00000000670B616D: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670B6172: E8 59 9F FB FF     call        00000000670700D0
  00000000670B6177: 48 8B C6           mov         rax,rsi
  00000000670B617A: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670B617F: 48 8B 6C 24 60     mov         rbp,qword ptr [rsp+60h]
  00000000670B6184: 48 83 C4 30        add         rsp,30h
  00000000670B6188: 41 5C              pop         r12
  00000000670B618A: 5F                 pop         rdi
  00000000670B618B: 5E                 pop         rsi
  00000000670B618C: C3                 ret

  Summary

      1A7000 .text

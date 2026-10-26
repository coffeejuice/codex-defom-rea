Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670BA3D0: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000670BA3D4: 8B 80 88 00 00 00  mov         eax,dword ptr [rax+0000000000000088h]
  00000000670BA3DA: C1 E8 04           shr         eax,4
  00000000670BA3DD: 24 01              and         al,1
  00000000670BA3DF: C3                 ret
  00000000670BA3E0: 48 8B 51 08        mov         rdx,qword ptr [rcx+8]
  00000000670BA3E4: 33 C0              xor         eax,eax
  00000000670BA3E6: 39 82 88 00 00 00  cmp         dword ptr [rdx+0000000000000088h],eax
  00000000670BA3EC: 0F 95 C0           setne       al
  00000000670BA3EF: C3                 ret
  00000000670BA3F0: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000670BA3F4: 8B 80 88 00 00 00  mov         eax,dword ptr [rax+0000000000000088h]
  00000000670BA3FA: 83 E0 01           and         eax,1
  00000000670BA3FD: C3                 ret
  00000000670BA3FE: CC                 int         3
  00000000670BA3FF: CC                 int         3
  00000000670BA400: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000670BA404: 8B 80 88 00 00 00  mov         eax,dword ptr [rax+0000000000000088h]
  00000000670BA40A: D1 E8              shr         eax,1
  00000000670BA40C: 83 E0 01           and         eax,1
  00000000670BA40F: C3                 ret
  00000000670BA410: 40 53              push        rbx
  00000000670BA412: 48 83 EC 20        sub         rsp,20h
  00000000670BA416: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000670BA41A: 8B 02              mov         eax,dword ptr [rdx]
  00000000670BA41C: 89 83 88 00 00 00  mov         dword ptr [rbx+0000000000000088h],eax
  00000000670BA422: F6 02 04           test        byte ptr [rdx],4
  00000000670BA425: 74 08              je          00000000670BA42F
  00000000670BA427: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670BA42A: FF 50 78           call        qword ptr [rax+78h]
  00000000670BA42D: EB 02              jmp         00000000670BA431
  00000000670BA42F: 33 C0              xor         eax,eax
  00000000670BA431: 48 89 83 B8 00 00  mov         qword ptr [rbx+00000000000000B8h],rax
                    00
  00000000670BA438: 48 8B 83 A8 00 00  mov         rax,qword ptr [rbx+00000000000000A8h]
                    00
  00000000670BA43F: C7 83 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],0
                    00 00 00 00
  00000000670BA449: 48 89 83 A0 00 00  mov         qword ptr [rbx+00000000000000A0h],rax
                    00
  00000000670BA450: C7 83 E4 00 00 00  mov         dword ptr [rbx+00000000000000E4h],0
                    00 00 00 00
  00000000670BA45A: C6 83 E1 00 00 00  mov         byte ptr [rbx+00000000000000E1h],1
                    01
  00000000670BA461: B0 01              mov         al,1
  00000000670BA463: 48 83 C4 20        add         rsp,20h
  00000000670BA467: 5B                 pop         rbx
  00000000670BA468: C3                 ret
  00000000670BA469: CC                 int         3
  00000000670BA46A: CC                 int         3
  00000000670BA46B: CC                 int         3
  00000000670BA46C: CC                 int         3
  00000000670BA46D: CC                 int         3
  00000000670BA46E: CC                 int         3
  00000000670BA46F: CC                 int         3
  00000000670BA470: 40 57              push        rdi
  00000000670BA472: 48 83 EC 30        sub         rsp,30h
  00000000670BA476: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670BA47F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670BA484: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000670BA488: 83 BB 88 00 00 00  cmp         dword ptr [rbx+0000000000000088h],0
                    00
  00000000670BA48F: 74 76              je          00000000670BA507
  00000000670BA491: E8 DA 6D 0B 00     call        0000000067171270
  00000000670BA496: 33 FF              xor         edi,edi
  00000000670BA498: 89 BB 88 00 00 00  mov         dword ptr [rbx+0000000000000088h],edi
  00000000670BA49E: 48 8D 8B 90 00 00  lea         rcx,[rbx+0000000000000090h]
                    00
  00000000670BA4A5: 48 8D 05 84 66 23  lea         rax,[00000000672F0B30h]
                    00
  00000000670BA4AC: 48 39 01           cmp         qword ptr [rcx],rax
  00000000670BA4AF: 74 2D              je          00000000670BA4DE

  Summary

      1A7000 .text

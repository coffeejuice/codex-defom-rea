Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654DD3D0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000654DD3D5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000654DD3DA: 57                 push        rdi
  00000000654DD3DB: 48 83 EC 30        sub         rsp,30h
  00000000654DD3DF: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000654DD3E3: 48 8B F9           mov         rdi,rcx
  00000000654DD3E6: 48 8B F2           mov         rsi,rdx
  00000000654DD3E9: 48 8B 8B A0 02 00  mov         rcx,qword ptr [rbx+00000000000002A0h]
                    00
  00000000654DD3F0: 48 85 C9           test        rcx,rcx
  00000000654DD3F3: 74 1B              je          00000000654DD410
  00000000654DD3F5: 4C 8B 83 80 02 00  mov         r8,qword ptr [rbx+0000000000000280h]
                    00
  00000000654DD3FC: 4C 8D 0D F5 42 25  lea         r9,[00000000657316F8h]
                    00
  00000000654DD403: 48 8D 15 8E 30 24  lea         rdx,[0000000065720498h]
                    00
  00000000654DD40A: FF 15 10 93 1B 00  call        qword ptr [0000000065696720h]
  00000000654DD410: 48 8B 8B 00 04 00  mov         rcx,qword ptr [rbx+0000000000000400h]
                    00
  00000000654DD417: 48 8B D6           mov         rdx,rsi
  00000000654DD41A: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000654DD41D: FF 90 A8 01 00 00  call        qword ptr [rax+00000000000001A8h]
  00000000654DD423: 48 8B D6           mov         rdx,rsi
  00000000654DD426: 48 8B CF           mov         rcx,rdi
  00000000654DD429: E8 D2 78 FC FF     call        00000000654A4D00
  00000000654DD42E: 48 8B 8B A0 02 00  mov         rcx,qword ptr [rbx+00000000000002A0h]
                    00
  00000000654DD435: 48 85 C9           test        rcx,rcx
  00000000654DD438: 74 23              je          00000000654DD45D
  00000000654DD43A: 4C 8B 83 80 02 00  mov         r8,qword ptr [rbx+0000000000000280h]
                    00
  00000000654DD441: 4C 8D 0D B0 42 25  lea         r9,[00000000657316F8h]
                    00
  00000000654DD448: 48 8D 15 49 30 24  lea         rdx,[0000000065720498h]
                    00
  00000000654DD44F: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  00000000654DD457: FF 15 BB 92 1B 00  call        qword ptr [0000000065696718h]
  00000000654DD45D: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000654DD462: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  00000000654DD467: 48 83 C4 30        add         rsp,30h
  00000000654DD46B: 5F                 pop         rdi
  00000000654DD46C: C3                 ret

  Summary

      695000 .text

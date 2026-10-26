Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653F742C: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  00000000653F7431: 33 FF              xor         edi,edi
  00000000653F7433: 48 85 DB           test        rbx,rbx
  00000000653F7436: 74 38              je          00000000653F7470
  00000000653F7438: 0F 1F 84 00 00 00  nop         dword ptr [rax+rax+0000000000000000h]
                    00 00
  00000000653F7440: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  00000000653F7444: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  00000000653F7448: 75 26              jne         00000000653F7470
  00000000653F744A: 48 8B CB           mov         rcx,rbx
  00000000653F744D: FF 15 15 F4 29 00  call        qword ptr [0000000065696868h]
  00000000653F7453: 48 8D 0D 4E 3A 32  lea         rcx,[000000006571AEA8h]
                    00
  00000000653F745A: 48 8B D0           mov         rdx,rax
  00000000653F745D: 48 8B D8           mov         rbx,rax
  00000000653F7460: FF 15 72 FD 29 00  call        qword ptr [00000000656971D8h]
  00000000653F7466: 48 85 C0           test        rax,rax
  00000000653F7469: 75 08              jne         00000000653F7473
  00000000653F746B: 48 85 DB           test        rbx,rbx
  00000000653F746E: 75 D0              jne         00000000653F7440
  00000000653F7470: 48 8B C7           mov         rax,rdi
  00000000653F7473: 48 8B 7C 24 30     mov         rdi,qword ptr [rsp+30h]
  00000000653F7478: 48 85 C0           test        rax,rax
  00000000653F747B: 0F 95 C0           setne       al
  00000000653F747E: 48 83 C4 20        add         rsp,20h
  00000000653F7482: 5B                 pop         rbx
  00000000653F7483: C3                 ret
  00000000653F7484: 85 C0              test        eax,eax
  00000000653F7486: 0F 95 C0           setne       al
  00000000653F7489: 48 83 C4 20        add         rsp,20h
  00000000653F748D: 5B                 pop         rbx
  00000000653F748E: C3                 ret
  00000000653F748F: CC                 int         3

  Summary

      695000 .text

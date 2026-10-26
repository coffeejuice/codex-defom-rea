Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654A5C10: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  00000000654A5C14: 4C 8B C1           mov         r8,rcx
  00000000654A5C17: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000000654A5C1A: C1 EA 0F           shr         edx,0Fh
  00000000654A5C1D: F6 C2 01           test        dl,1
  00000000654A5C20: 75 14              jne         00000000654A5C36
  00000000654A5C22: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000654A5C26: 33 D2              xor         edx,edx
  00000000654A5C28: 48 81 C1 A8 03 00  add         rcx,3A8h
                    00
  00000000654A5C2F: 48 FF 25 42 11 1F  jmp         qword ptr [0000000065696D78h]
                    00
  00000000654A5C36: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000654A5C39: 48 FF A0 38 02 00  jmp         qword ptr [rax+0000000000000238h]
                    00
  00000000654A5C40: 40 55              push        rbp
  00000000654A5C42: 56                 push        rsi
  00000000654A5C43: 57                 push        rdi
  00000000654A5C44: 48 83 EC 30        sub         rsp,30h
  00000000654A5C48: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654A5C51: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000654A5C56: 49 8B F0           mov         rsi,r8
  00000000654A5C59: 48 8B F9           mov         rdi,rcx
  00000000654A5C5C: 48 8B 69 08        mov         rbp,qword ptr [rcx+8]
  00000000654A5C60: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  00000000654A5C64: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  00000000654A5C68: 41 8B C1           mov         eax,r9d
  00000000654A5C6B: C1 E8 0F           shr         eax,0Fh
  00000000654A5C6E: 24 01              and         al,1
  00000000654A5C70: 74 75              je          00000000654A5CE7
  00000000654A5C72: 41 C1 E9 0A        shr         r9d,0Ah
  00000000654A5C76: 41 80 E1 01        and         r9b,1
  00000000654A5C7A: 75 6B              jne         00000000654A5CE7
  00000000654A5C7C: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000654A5C7F: 4C 8B C2           mov         r8,rdx
  00000000654A5C82: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000654A5C87: FF 90 A8 02 00 00  call        qword ptr [rax+00000000000002A8h]
  00000000654A5C8D: 48 8B D8           mov         rbx,rax

  Summary

      695000 .text

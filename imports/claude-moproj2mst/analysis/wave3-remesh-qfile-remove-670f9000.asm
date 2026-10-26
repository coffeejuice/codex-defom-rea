Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670F9000: 40 57              push        rdi
  00000000670F9002: 48 83 EC 30        sub         rsp,30h
  00000000670F9006: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670F900F: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000670F9014: 48 8B F9           mov         rdi,rcx
  00000000670F9017: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000670F901B: 33 C0              xor         eax,eax
  00000000670F901D: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000000670F9021: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  00000000670F9025: 48 83 C1 20        add         rcx,20h
  00000000670F9029: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670F902E: E8 2D 68 00 00     call        00000000670FF860
  00000000670F9033: 0F B6 D8           movzx       ebx,al
  00000000670F9036: 84 C0              test        al,al
  00000000670F9038: 75 3E              jne         00000000670F9078
  00000000670F903A: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000000670F903F: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670F9044: E8 D7 82 04 00     call        0000000067141320
  00000000670F9049: 90                 nop
  00000000670F904A: 4C 8B C0           mov         r8,rax
  00000000670F904D: BA 09 00 00 00     mov         edx,9
  00000000670F9052: 48 8B CF           mov         rcx,rdi
  00000000670F9055: E8 E6 E9 FA FF     call        00000000670A7A40
  00000000670F905A: 90                 nop
  00000000670F905B: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000000670F9060: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F9064: 75 0E              jne         00000000670F9074
  00000000670F9066: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000000670F906B: E8 60 70 F7 FF     call        00000000670700D0
  00000000670F9070: 8A C3              mov         al,bl
  00000000670F9072: EB 06              jmp         00000000670F907A
  00000000670F9074: 8A C3              mov         al,bl
  00000000670F9076: EB 02              jmp         00000000670F907A
  00000000670F9078: 8A C3              mov         al,bl
  00000000670F907A: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000670F907F: 48 83 C4 30        add         rsp,30h
  00000000670F9083: 5F                 pop         rdi
  00000000670F9084: C3                 ret

  Summary

      1A7000 .text

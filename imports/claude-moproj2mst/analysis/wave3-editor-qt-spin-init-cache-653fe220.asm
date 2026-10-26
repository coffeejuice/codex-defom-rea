Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653FE220: 83 FA 02           cmp         edx,2
  00000000653FE223: 0F 84 86 00 00 00  je          00000000653FE2AF
  00000000653FE229: 57                 push        rdi
  00000000653FE22A: 48 83 EC 30        sub         rsp,30h
  00000000653FE22E: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653FE237: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000653FE23C: 48 8B D9           mov         rbx,rcx
  00000000653FE23F: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000653FE243: 83 A1 8C 02 00 00  and         dword ptr [rcx+000000000000028Ch],0FFFFFFFEh
                    FE
  00000000653FE24A: 83 FA 01           cmp         edx,1
  00000000653FE24D: 74 14              je          00000000653FE263
  00000000653FE24F: 48 81 C1 00 02 00  add         rcx,200h
                    00
  00000000653FE256: 49 8B D0           mov         rdx,r8
  00000000653FE259: FF 15 B9 A6 29 00  call        qword ptr [0000000065698918h]
  00000000653FE25F: 84 C0              test        al,al
  00000000653FE261: 74 42              je          00000000653FE2A5
  00000000653FE263: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000653FE268: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  00000000653FE26F: E8 EC 17 FC FF     call        00000000653BFA60
  00000000653FE274: 90                 nop
  00000000653FE275: 48 8B D0           mov         rdx,rax
  00000000653FE278: 48 8B CF           mov         rcx,rdi
  00000000653FE27B: E8 D0 7C 1D 00     call        00000000655D5F50
  00000000653FE280: 90                 nop
  00000000653FE281: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000653FE286: FF 15 D4 84 29 00  call        qword ptr [0000000065696760h]
  00000000653FE28C: 48 8D 8B 00 02 00  lea         rcx,[rbx+0000000000000200h]
                    00
  00000000653FE293: 33 D2              xor         edx,edx
  00000000653FE295: FF 15 4D 96 29 00  call        qword ptr [00000000656978E8h]
  00000000653FE29B: 8B D0              mov         edx,eax
  00000000653FE29D: 48 8B CF           mov         rcx,rdi
  00000000653FE2A0: E8 6B 7C 1D 00     call        00000000655D5F10
  00000000653FE2A5: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000653FE2AA: 48 83 C4 30        add         rsp,30h
  00000000653FE2AE: 5F                 pop         rdi
  00000000653FE2AF: F3 C3              rep ret

  Summary

      695000 .text

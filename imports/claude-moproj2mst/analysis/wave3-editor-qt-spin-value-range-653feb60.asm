Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653FEB60: 40 57              push        rdi
  00000000653FEB62: 48 83 EC 40        sub         rsp,40h
  00000000653FEB66: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653FEB6F: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000653FEB74: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000653FEB78: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000653FEB7D: FF 15 D5 86 29 00  call        qword ptr [0000000065697258h]
  00000000653FEB83: 90                 nop
  00000000653FEB84: 48 8D 9F 20 02 00  lea         rbx,[rdi+0000000000000220h]
                    00
  00000000653FEB8B: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000653FEB90: 48 8B CB           mov         rcx,rbx
  00000000653FEB93: E8 98 84 F7 FF     call        0000000065377030
  00000000653FEB98: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  00000000653FEB9D: 85 C0              test        eax,eax
  00000000653FEB9F: 4C 0F 4F C3        cmovg       r8,rbx
  00000000653FEBA3: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000653FEBA6: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000653FEBAB: 48 8B CF           mov         rcx,rdi
  00000000653FEBAE: FF 50 20           call        qword ptr [rax+20h]
  00000000653FEBB1: 90                 nop
  00000000653FEBB2: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000653FEBB7: FF 15 23 7B 29 00  call        qword ptr [00000000656966E0h]
  00000000653FEBBD: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000653FEBC2: 48 83 C4 40        add         rsp,40h
  00000000653FEBC6: 5F                 pop         rdi
  00000000653FEBC7: C3                 ret

  Summary

      695000 .text

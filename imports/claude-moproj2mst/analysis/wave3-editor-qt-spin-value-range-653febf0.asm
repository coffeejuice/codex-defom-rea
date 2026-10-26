Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653FEBF0: 40 57              push        rdi
  00000000653FEBF2: 48 83 EC 40        sub         rsp,40h
  00000000653FEBF6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653FEBFF: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000653FEC04: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  00000000653FEC08: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000653FEC0D: FF 15 45 86 29 00  call        qword ptr [0000000065697258h]
  00000000653FEC13: 90                 nop
  00000000653FEC14: 48 8D 9F 10 02 00  lea         rbx,[rdi+0000000000000210h]
                    00
  00000000653FEC1B: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000653FEC20: 48 8B CB           mov         rcx,rbx
  00000000653FEC23: E8 08 84 F7 FF     call        0000000065377030
  00000000653FEC28: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000653FEC2D: 85 C0              test        eax,eax
  00000000653FEC2F: 48 0F 48 D3        cmovs       rdx,rbx
  00000000653FEC33: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000653FEC36: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  00000000653FEC3B: 48 8B CF           mov         rcx,rdi
  00000000653FEC3E: FF 50 20           call        qword ptr [rax+20h]
  00000000653FEC41: 90                 nop
  00000000653FEC42: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000653FEC47: FF 15 93 7A 29 00  call        qword ptr [00000000656966E0h]
  00000000653FEC4D: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000653FEC52: 48 83 C4 40        add         rsp,40h
  00000000653FEC56: 5F                 pop         rdi
  00000000653FEC57: C3                 ret

  Summary

      695000 .text

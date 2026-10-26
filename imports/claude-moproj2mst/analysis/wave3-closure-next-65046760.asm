Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065046760: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000065046765: 57                 push        rdi
  0000000065046766: 48 83 EC 30        sub         rsp,30h
  000000006504676A: 48 8B 59 F8        mov         rbx,qword ptr [rcx-8]
  000000006504676E: 48 8B F9           mov         rdi,rcx
  0000000065046771: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000065046776: FF 15 BC 00 65 00  call        qword ptr [0000000065696838h]
  000000006504677C: 48 8D 4F F0        lea         rcx,[rdi-10h]
  0000000065046780: 8B 10              mov         edx,dword ptr [rax]
  0000000065046782: 89 93 A4 00 00 00  mov         dword ptr [rbx+00000000000000A4h],edx
  0000000065046788: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006504678B: 89 93 A8 00 00 00  mov         dword ptr [rbx+00000000000000A8h],edx
  0000000065046791: 8B 50 08           mov         edx,dword ptr [rax+8]
  0000000065046794: 89 93 AC 00 00 00  mov         dword ptr [rbx+00000000000000ACh],edx
  000000006504679A: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  000000006504679D: 89 83 B0 00 00 00  mov         dword ptr [rbx+00000000000000B0h],eax
  00000000650467A3: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000650467A8: 48 83 C4 30        add         rsp,30h
  00000000650467AC: 5F                 pop         rdi
  00000000650467AD: E9 7E F1 FF FF     jmp         0000000065045930

  Summary

      695000 .text

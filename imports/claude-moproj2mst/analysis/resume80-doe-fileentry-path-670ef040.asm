Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670EF040: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  00000000670EF045: 57                 push        rdi
  00000000670EF046: 48 83 EC 20        sub         rsp,20h
  00000000670EF04A: 48 8B FA           mov         rdi,rdx
  00000000670EF04D: 48 8B D9           mov         rbx,rcx
  00000000670EF050: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000000670EF058: E8 73 FD FF FF     call        00000000670EEDD0
  00000000670EF05D: 4C 8B 1B           mov         r11,qword ptr [rbx]
  00000000670EF060: 4C 89 1F           mov         qword ptr [rdi],r11
  00000000670EF063: F0 41 83 03 01     lock add    dword ptr [r11],1
  00000000670EF068: 48 8B C7           mov         rax,rdi
  00000000670EF06B: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670EF070: 48 83 C4 20        add         rsp,20h
  00000000670EF074: 5F                 pop         rdi
  00000000670EF075: C3                 ret

  Summary

      1A7000 .text

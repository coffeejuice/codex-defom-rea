Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412F0860: 4C 8B DC           mov         r11,rsp
  00000001412F0863: 53                 push        rbx
  00000001412F0864: 48 83 EC 50        sub         rsp,50h
  00000001412F0868: 48 8B 11           mov         rdx,qword ptr [rcx]
  00000001412F086B: 48 8B 41 30        mov         rax,qword ptr [rcx+30h]
  00000001412F086F: 4D 8D 4B C8        lea         r9,[r11-38h]
  00000001412F0873: 49 89 43 D0        mov         qword ptr [r11-30h],rax
  00000001412F0877: 48 8B 00           mov         rax,qword ptr [rax]
  00000001412F087A: 49 89 53 C8        mov         qword ptr [r11-38h],rdx
  00000001412F087E: 0F 28 44 24 20     movaps      xmm0,xmmword ptr [rsp+20h]
  00000001412F0883: 66 0F 7F 44 24 20  movdqa      xmmword ptr [rsp+20h],xmm0
  00000001412F0889: 49 89 53 D8        mov         qword ptr [r11-28h],rdx
  00000001412F088D: 49 89 43 E0        mov         qword ptr [r11-20h],rax
  00000001412F0891: 4D 8D 43 D8        lea         r8,[r11-28h]
  00000001412F0895: 0F 28 4C 24 30     movaps      xmm1,xmmword ptr [rsp+30h]
  00000001412F089A: 66 0F 7F 4C 24 30  movdqa      xmmword ptr [rsp+30h],xmm1
  00000001412F08A0: 49 8D 53 E8        lea         rdx,[r11-18h]
  00000001412F08A4: 48 8B D9           mov         rbx,rcx
  00000001412F08A7: E8 04 FE FF FF     call        00000001412F06B0
  00000001412F08AC: 48 8B 4B 30        mov         rcx,qword ptr [rbx+30h]
  00000001412F08B0: E8 B3 F3 1E 00     call        00000001414DFC68
  00000001412F08B5: 45 33 DB           xor         r11d,r11d
  00000001412F08B8: 4C 89 5B 30        mov         qword ptr [rbx+30h],r11
  00000001412F08BC: 4C 89 5B 38        mov         qword ptr [rbx+38h],r11
  00000001412F08C0: 48 83 C4 50        add         rsp,50h
  00000001412F08C4: 5B                 pop         rbx
  00000001412F08C5: C3                 ret

  Summary

     1E02000 .text

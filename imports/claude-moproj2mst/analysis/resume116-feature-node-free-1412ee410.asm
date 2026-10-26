Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412EE410: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001412EE415: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000001412EE41A: 57                 push        rdi
  00000001412EE41B: 48 83 EC 20        sub         rsp,20h
  00000001412EE41F: 48 8B 41 30        mov         rax,qword ptr [rcx+30h]
  00000001412EE423: 48 8B D9           mov         rbx,rcx
  00000001412EE426: 48 8B 70 08        mov         rsi,qword ptr [rax+8]
  00000001412EE42A: 48 8B FE           mov         rdi,rsi
  00000001412EE42D: 80 7E 29 00        cmp         byte ptr [rsi+29h],0
  00000001412EE431: 75 20              jne         00000001412EE453
  00000001412EE433: 48 8B 57 10        mov         rdx,qword ptr [rdi+10h]
  00000001412EE437: 48 8B CB           mov         rcx,rbx
  00000001412EE43A: E8 31 F6 FF FF     call        00000001412EDA70
  00000001412EE43F: 48 8B 3F           mov         rdi,qword ptr [rdi]
  00000001412EE442: 48 8B CE           mov         rcx,rsi
  00000001412EE445: E8 1E 18 1F 00     call        00000001414DFC68
  00000001412EE44A: 80 7F 29 00        cmp         byte ptr [rdi+29h],0
  00000001412EE44E: 48 8B F7           mov         rsi,rdi
  00000001412EE451: 74 E0              je          00000001412EE433
  00000001412EE453: 48 8B 43 30        mov         rax,qword ptr [rbx+30h]
  00000001412EE457: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  00000001412EE45C: 48 89 40 08        mov         qword ptr [rax+8],rax
  00000001412EE460: 48 8B 43 30        mov         rax,qword ptr [rbx+30h]
  00000001412EE464: 48 C7 43 38 00 00  mov         qword ptr [rbx+38h],0
                    00 00
  00000001412EE46C: 48 89 00           mov         qword ptr [rax],rax
  00000001412EE46F: 48 8B 43 30        mov         rax,qword ptr [rbx+30h]
  00000001412EE473: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000001412EE478: 48 89 40 10        mov         qword ptr [rax+10h],rax
  00000001412EE47C: 48 83 C4 20        add         rsp,20h
  00000001412EE480: 5F                 pop         rdi
  00000001412EE481: C3                 ret

  Summary

     1E02000 .text

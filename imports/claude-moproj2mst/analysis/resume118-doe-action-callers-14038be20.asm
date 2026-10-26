Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014038BE20: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014038BE25: 48 83 EC 28        sub         rsp,28h
  000000014038BE29: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014038BE2E: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  000000014038BE32: 48 8B 89 98 00 00  mov         rcx,qword ptr [rcx+0000000000000098h]
                    00
  000000014038BE39: FF 15 29 9F A7 01  call        qword ptr [0000000141E05D68h]
  000000014038BE3F: 44 0F B6 D8        movzx       r11d,al
  000000014038BE43: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014038BE48: 44 88 98 90 00 00  mov         byte ptr [rax+0000000000000090h],r11b
                    00
  000000014038BE4F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014038BE54: 48 83 C1 28        add         rcx,28h
  000000014038BE58: E8 93 F0 88 00     call        0000000140C1AEF0
  000000014038BE5D: 48 8B C8           mov         rcx,rax
  000000014038BE60: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014038BE65: 44 8B 40 48        mov         r8d,dword ptr [rax+48h]
  000000014038BE69: 33 D2              xor         edx,edx
  000000014038BE6B: E8 80 B7 FD FF     call        00000001403675F0
  000000014038BE70: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014038BE75: E8 B6 D9 FF FF     call        0000000140389830
  000000014038BE7A: 48 83 C4 28        add         rsp,28h
  000000014038BE7E: C3                 ret

  Summary

     1E02000 .text

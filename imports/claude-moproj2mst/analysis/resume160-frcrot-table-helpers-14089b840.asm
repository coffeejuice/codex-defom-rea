Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014089B840: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089B845: 48 83 EC 38        sub         rsp,38h
  000000014089B849: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B84E: E8 9D F9 FF FF     call        000000014089B1F0
  000000014089B853: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B858: E8 13 01 00 00     call        000000014089B970
  000000014089B85D: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014089B865: EB 0B              jmp         000000014089B872
  000000014089B867: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014089B86B: 83 C0 01           add         eax,1
  000000014089B86E: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014089B872: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B877: 8B 40 04           mov         eax,dword ptr [rax+4]
  000000014089B87A: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014089B87E: 7D 7C              jge         000000014089B8FC
  000000014089B880: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089B885: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B88A: 83 7C 88 1C 00     cmp         dword ptr [rax+rcx*4+1Ch],0
  000000014089B88F: 7E 66              jle         000000014089B8F7
  000000014089B891: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089B896: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B89B: 48 63 4C 88 1C     movsxd      rcx,dword ptr [rax+rcx*4+1Ch]
  000000014089B8A0: BA 08 00 00 00     mov         edx,8
  000000014089B8A5: E8 BE 7A BD 00     call        0000000141473368
  000000014089B8AA: 4C 8B D8           mov         r11,rax
  000000014089B8AD: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089B8B2: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B8B7: 4C 89 5C C8 28     mov         qword ptr [rax+rcx*8+28h],r11
  000000014089B8BC: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089B8C1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B8C6: 48 83 7C C8 28 00  cmp         qword ptr [rax+rcx*8+28h],0
  000000014089B8CC: 74 29              je          000000014089B8F7
  000000014089B8CE: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  000000014089B8D3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B8D8: 4C 63 44 88 1C     movsxd      r8,dword ptr [rax+rcx*4+1Ch]
  000000014089B8DD: 49 C1 E0 03        shl         r8,3
  000000014089B8E1: 48 63 44 24 20     movsxd      rax,dword ptr [rsp+20h]
  000000014089B8E6: 33 D2              xor         edx,edx
  000000014089B8E8: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B8ED: 48 8B 4C C1 28     mov         rcx,qword ptr [rcx+rax*8+28h]
  000000014089B8F2: E8 59 5F BD 00     call        0000000141471850
  000000014089B8F7: E9 6B FF FF FF     jmp         000000014089B867
  000000014089B8FC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B901: E8 8A 03 00 00     call        000000014089BC90
  000000014089B906: 85 C0              test        eax,eax
  000000014089B908: 7E 50              jle         000000014089B95A
  000000014089B90A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B90F: E8 7C 03 00 00     call        000000014089BC90
  000000014089B914: 48 63 C8           movsxd      rcx,eax
  000000014089B917: BA 08 00 00 00     mov         edx,8
  000000014089B91C: E8 47 7A BD 00     call        0000000141473368
  000000014089B921: 4C 8B D8           mov         r11,rax
  000000014089B924: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B929: 4C 89 58 48        mov         qword ptr [rax+48h],r11
  000000014089B92D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B932: 48 83 78 48 00     cmp         qword ptr [rax+48h],0
  000000014089B937: 74 21              je          000000014089B95A
  000000014089B939: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B93E: E8 4D 03 00 00     call        000000014089BC90
  000000014089B943: 4C 63 C0           movsxd      r8,eax
  000000014089B946: 49 C1 E0 03        shl         r8,3
  000000014089B94A: 33 D2              xor         edx,edx
  000000014089B94C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B951: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  000000014089B955: E8 F6 5E BD 00     call        0000000141471850
  000000014089B95A: 48 83 C4 38        add         rsp,38h
  000000014089B95E: C3                 ret

  Summary

     1E02000 .text

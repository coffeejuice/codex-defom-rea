Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140377950: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140377955: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014037795A: 48 83 EC 48        sub         rsp,48h
  000000014037795E: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140377967: 48 8D 44 24 20     lea         rax,[rsp+20h]
  000000014037796C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140377971: 33 D2              xor         edx,edx
  0000000140377973: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140377978: E8 F3 F2 1C 00     call        0000000140546C70
  000000014037797D: 4C 8B C0           mov         r8,rax
  0000000140377980: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140377985: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014037798A: FF 15 F0 40 A9 01  call        qword ptr [0000000141E0BA80h]
  0000000140377990: 90                 nop
  0000000140377991: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140377996: 48 8D 05 BB 43 FB  lea         rax,[000000014232BD58h]
                    01
  000000014037799D: 48 89 01           mov         qword ptr [rcx],rax
  00000001403779A0: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001403779A5: 48 8D 05 74 43 FB  lea         rax,[000000014232BD20h]
                    01
  00000001403779AC: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001403779B0: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001403779B5: 48 83 C1 28        add         rcx,28h
  00000001403779B9: E8 B2 57 DE FF     call        000000014015D170
  00000001403779BE: 90                 nop
  00000001403779BF: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001403779C4: C6 40 34 01        mov         byte ptr [rax+34h],1
  00000001403779C8: 33 D2              xor         edx,edx
  00000001403779CA: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001403779CF: E8 4C 01 00 00     call        0000000140377B20
  00000001403779D4: 33 D2              xor         edx,edx
  00000001403779D6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001403779DB: E8 A0 01 00 00     call        0000000140377B80
  00000001403779E0: 4C 8B 5C 24 50     mov         r11,qword ptr [rsp+50h]
  00000001403779E5: 41 C6 43 35 00     mov         byte ptr [r11+35h],0
  00000001403779EA: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001403779EF: 48 83 C4 48        add         rsp,48h
  00000001403779F3: C3                 ret

  Summary

     1E02000 .text

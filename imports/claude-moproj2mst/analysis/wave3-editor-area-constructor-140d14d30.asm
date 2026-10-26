Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D14D30: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140D14D35: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D14D3A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D14D3F: 48 83 EC 48        sub         rsp,48h
  0000000140D14D43: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D14D4C: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140D14D51: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D14D56: FF 15 7C 4C 0F 01  call        qword ptr [0000000141E099D8h]
  0000000140D14D5C: 90                 nop
  0000000140D14D5D: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D14D62: 48 8D 05 FF 83 CE  lea         rax,[00000001429FD168h]
                    01
  0000000140D14D69: 48 89 01           mov         qword ptr [rcx],rax
  0000000140D14D6C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D14D71: 48 8D 05 B8 83 CE  lea         rax,[00000001429FD130h]
                    01
  0000000140D14D78: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140D14D7C: B2 01              mov         dl,1
  0000000140D14D7E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D14D83: FF 15 47 4C 0F 01  call        qword ptr [0000000141E099D0h]
  0000000140D14D89: B9 48 00 00 00     mov         ecx,48h
  0000000140D14D8E: E8 29 9D 75 00     call        000000014146EABC
  0000000140D14D93: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D14D98: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140D14D9E: 74 16              je          0000000140D14DB6
  0000000140D14DA0: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140D14DA5: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140D14DAA: E8 61 84 09 00     call        0000000140DAD210
  0000000140D14DAF: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D14DB4: EB 09              jmp         0000000140D14DBF
  0000000140D14DB6: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140D14DBF: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D14DC4: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D14DC9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D14DCE: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D14DD3: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  0000000140D14DD7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D14DDC: E8 AF 00 00 00     call        0000000140D14E90
  0000000140D14DE1: 33 D2              xor         edx,edx
  0000000140D14DE3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D14DE8: E8 83 01 00 00     call        0000000140D14F70
  0000000140D14DED: 90                 nop
  0000000140D14DEE: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140D14DF3: 48 83 C4 48        add         rsp,48h
  0000000140D14DF7: C3                 ret

  Summary

     1E02000 .text

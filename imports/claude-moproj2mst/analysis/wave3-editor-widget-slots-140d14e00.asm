Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D14E00: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D14E05: 48 83 EC 48        sub         rsp,48h
  0000000140D14E09: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D14E12: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D14E17: 48 8D 05 4A 83 CE  lea         rax,[00000001429FD168h]
                    01
  0000000140D14E1E: 48 89 01           mov         qword ptr [rcx],rax
  0000000140D14E21: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D14E26: 48 8D 05 03 83 CE  lea         rax,[00000001429FD130h]
                    01
  0000000140D14E2D: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140D14E31: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140D14E36: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140D14E3A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D14E3F: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140D14E44: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D14E49: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140D14E4F: 74 1C              je          0000000140D14E6D
  0000000140D14E51: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140D14E56: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D14E59: BA 01 00 00 00     mov         edx,1
  0000000140D14E5E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D14E63: FF 50 18           call        qword ptr [rax+18h]
  0000000140D14E66: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D14E6B: EB 09              jmp         0000000140D14E76
  0000000140D14E6D: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140D14E76: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D14E7B: FF 15 47 4B 0F 01  call        qword ptr [0000000141E099C8h]
  0000000140D14E81: 48 83 C4 48        add         rsp,48h
  0000000140D14E85: C3                 ret

  Summary

     1E02000 .text

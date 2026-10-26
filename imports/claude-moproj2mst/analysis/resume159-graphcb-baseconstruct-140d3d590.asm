Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D3D590: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D3D595: 48 83 EC 58        sub         rsp,58h
  0000000140D3D599: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D3D5A2: 33 D2              xor         edx,edx
  0000000140D3D5A4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D3D5A9: FF 15 E1 84 0C 01  call        qword ptr [0000000141E05A90h]
  0000000140D3D5AF: 90                 nop
  0000000140D3D5B0: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D3D5B5: 48 8D 05 FC 1A CD  lea         rax,[0000000142A0F0B8h]
                    01
  0000000140D3D5BC: 48 89 01           mov         qword ptr [rcx],rax
  0000000140D3D5BF: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D3D5C4: 48 83 C1 10        add         rcx,10h
  0000000140D3D5C8: E8 C3 11 01 00     call        0000000140D4E790
  0000000140D3D5CD: 90                 nop
  0000000140D3D5CE: B9 10 00 00 00     mov         ecx,10h
  0000000140D3D5D3: E8 E4 14 73 00     call        000000014146EABC
  0000000140D3D5D8: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D3D5DD: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140D3D5E3: 74 2B              je          0000000140D3D610
  0000000140D3D5E5: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140D3D5EA: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140D3D5EF: FF 15 83 E1 0C 01  call        qword ptr [0000000141E0B778h]
  0000000140D3D5F5: 4C 8B 5C 24 38     mov         r11,qword ptr [rsp+38h]
  0000000140D3D5FA: 48 8D 05 47 12 3D  lea         rax,[000000014210E848h]
                    01
  0000000140D3D601: 49 89 03           mov         qword ptr [r11],rax
  0000000140D3D604: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D3D609: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D3D60E: EB 09              jmp         0000000140D3D619
  0000000140D3D610: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  0000000140D3D619: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140D3D61E: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D3D623: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D3D628: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140D3D62D: 48 89 41 48        mov         qword ptr [rcx+48h],rax
  0000000140D3D631: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140D3D639: 4C 8D 0D B0 13 CD  lea         r9,[0000000142A0E9F0h]
                    01
  0000000140D3D640: 4C 8B 44 24 60     mov         r8,qword ptr [rsp+60h]
  0000000140D3D645: 48 8D 15 CC 13 CD  lea         rdx,[0000000142A0EA18h]
                    01
  0000000140D3D64C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D3D651: FF 15 79 6B 0C 01  call        qword ptr [0000000141E041D0h]
  0000000140D3D657: 4C 8B 5C 24 60     mov         r11,qword ptr [rsp+60h]
  0000000140D3D65C: 41 C7 43 40 00 00  mov         dword ptr [r11+40h],0
                    00 00
  0000000140D3D664: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140D3D669: 48 83 C4 58        add         rsp,58h
  0000000140D3D66D: C3                 ret

  Summary

     1E02000 .text

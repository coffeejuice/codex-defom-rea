Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CC1200: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140CC1205: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140CC120A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CC120F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CC1214: 48 83 EC 68        sub         rsp,68h
  0000000140CC1218: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140CC1221: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140CC1226: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CC122B: 33 C0              xor         eax,eax
  0000000140CC122D: 85 C0              test        eax,eax
  0000000140CC122F: 0F 84 98 00 00 00  je          0000000140CC12CD
  0000000140CC1235: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CC123D: 48 39 44 24 20     cmp         qword ptr [rsp+20h],rax
  0000000140CC1242: 0F 84 80 00 00 00  je          0000000140CC12C8
  0000000140CC1248: B9 08 00 00 00     mov         ecx,8
  0000000140CC124D: E8 6A D8 7A 00     call        000000014146EABC
  0000000140CC1252: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CC1257: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140CC125C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CC1261: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140CC1267: 74 22              je          0000000140CC128B
  0000000140CC1269: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CC1271: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CC1274: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140CC1279: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CC127C: 48 89 01           mov         qword ptr [rcx],rax
  0000000140CC127F: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140CC1284: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140CC1289: EB 09              jmp         0000000140CC1294
  0000000140CC128B: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  0000000140CC1294: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140CC1299: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140CC129E: 48 89 01           mov         qword ptr [rcx],rax
  0000000140CC12A1: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140CC12A6: 48 83 C0 08        add         rax,8
  0000000140CC12AA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CC12AF: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CC12B7: 48 83 C0 08        add         rax,8
  0000000140CC12BB: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140CC12C3: E9 6D FF FF FF     jmp         0000000140CC1235
  0000000140CC12C8: E9 D1 00 00 00     jmp         0000000140CC139E
  0000000140CC12CD: 33 C0              xor         eax,eax
  0000000140CC12CF: 85 C0              test        eax,eax
  0000000140CC12D1: 74 79              je          0000000140CC134C
  0000000140CC12D3: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CC12DB: 48 39 44 24 20     cmp         qword ptr [rsp+20h],rax
  0000000140CC12E0: 74 68              je          0000000140CC134A
  0000000140CC12E2: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140CC12E7: B9 08 00 00 00     mov         ecx,8
  0000000140CC12EC: E8 8F 1F 34 FF     call        0000000140003280
  0000000140CC12F1: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CC12F6: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140CC12FC: 74 1F              je          0000000140CC131D
  0000000140CC12FE: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140CC1303: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CC130B: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CC130E: 48 89 01           mov         qword ptr [rcx],rax
  0000000140CC1311: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140CC1316: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CC131B: EB 09              jmp         0000000140CC1326
  0000000140CC131D: 48 C7 44 24 58 00  mov         qword ptr [rsp+58h],0
                    00 00 00
  0000000140CC1326: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140CC132B: 48 83 C0 08        add         rax,8
  0000000140CC132F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CC1334: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CC133C: 48 83 C0 08        add         rax,8
  0000000140CC1340: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140CC1348: EB 89              jmp         0000000140CC12D3
  0000000140CC134A: EB 52              jmp         0000000140CC139E
  0000000140CC134C: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140CC1351: 48 39 84 24 88 00  cmp         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140CC1359: 74 43              je          0000000140CC139E
  0000000140CC135B: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140CC1360: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CC1368: 48 2B C1           sub         rax,rcx
  0000000140CC136B: 48 C1 F8 03        sar         rax,3
  0000000140CC136F: 48 85 C0           test        rax,rax
  0000000140CC1372: 7E 2A              jle         0000000140CC139E
  0000000140CC1374: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140CC1379: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CC1381: 4C 2B C0           sub         r8,rax
  0000000140CC1384: 49 C1 F8 03        sar         r8,3
  0000000140CC1388: 49 C1 E0 03        shl         r8,3
  0000000140CC138C: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CC1394: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140CC1399: E8 62 DD 7A 00     call        000000014146F100
  0000000140CC139E: 48 83 C4 68        add         rsp,68h
  0000000140CC13A2: C3                 ret

  Summary

     1E02000 .text

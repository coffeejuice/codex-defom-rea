Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140012FE0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140012FE5: 48 83 EC 58        sub         rsp,58h
  0000000140012FE9: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140012FF2: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140012FF7: 48 83 B8 C8 02 00  cmp         qword ptr [rax+00000000000002C8h],0
                    00 00
  0000000140012FFF: 0F 85 9E 00 00 00  jne         00000001400130A3
  0000000140013005: B9 40 00 00 00     mov         ecx,40h
  000000014001300A: E8 AD BA 45 01     call        000000014146EABC
  000000014001300F: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140013014: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014001301A: 74 13              je          000000014001302F
  000000014001301C: 33 D2              xor         edx,edx
  000000014001301E: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140013023: E8 38 BF 13 00     call        000000014014EF60
  0000000140013028: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014001302D: EB 09              jmp         0000000140013038
  000000014001302F: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140013038: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014001303D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140013042: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140013047: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014001304C: 48 89 81 C8 02 00  mov         qword ptr [rcx+00000000000002C8h],rax
                    00
  0000000140013053: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140013058: E8 F3 06 00 00     call        0000000140013750
  000000014001305D: 48 8B C8           mov         rcx,rax
  0000000140013060: E8 AB 1E 01 00     call        0000000140024F10
  0000000140013065: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014001306A: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140013070: 75 0B              jne         000000014001307D
  0000000140013072: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  000000014001307B: EB 1C              jmp         0000000140013099
  000000014001307D: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140013082: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140013086: 48 63 48 04        movsxd      rcx,dword ptr [rax+4]
  000000014001308A: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014001308F: 48 8D 44 08 28     lea         rax,[rax+rcx+28h]
  0000000140013094: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140013099: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014001309E: E8 1D 93 13 00     call        000000014014C3C0
  00000001400130A3: 48 83 C4 58        add         rsp,58h
  00000001400130A7: C3                 ret

  Summary

     1E02000 .text

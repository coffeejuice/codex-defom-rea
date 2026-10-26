Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDA080: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CDA085: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDA08A: 48 83 EC 48        sub         rsp,48h
  0000000140CDA08E: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140CDA097: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140CDA09C: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA0A1: FF 15 31 F9 12 01  call        qword ptr [0000000141E099D8h]
  0000000140CDA0A7: 90                 nop
  0000000140CDA0A8: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA0AD: 48 8D 05 14 C0 D0  lea         rax,[00000001429E60C8h]
                    01
  0000000140CDA0B4: 48 89 01           mov         qword ptr [rcx],rax
  0000000140CDA0B7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA0BC: 48 8D 05 CD BF D0  lea         rax,[00000001429E6090h]
                    01
  0000000140CDA0C3: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140CDA0C7: B2 01              mov         dl,1
  0000000140CDA0C9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA0CE: FF 15 FC F8 12 01  call        qword ptr [0000000141E099D0h]
  0000000140CDA0D4: B9 48 00 00 00     mov         ecx,48h
  0000000140CDA0D9: E8 DE 49 79 00     call        000000014146EABC
  0000000140CDA0DE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CDA0E3: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140CDA0E9: 74 16              je          0000000140CDA101
  0000000140CDA0EB: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140CDA0F0: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140CDA0F5: E8 16 31 0D 00     call        0000000140DAD210
  0000000140CDA0FA: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CDA0FF: EB 09              jmp         0000000140CDA10A
  0000000140CDA101: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140CDA10A: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140CDA10F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CDA114: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA119: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140CDA11E: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  0000000140CDA122: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140CDA127: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140CDA12B: C6 40 11 00        mov         byte ptr [rax+11h],0
  0000000140CDA12F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA134: E8 17 01 00 00     call        0000000140CDA250
  0000000140CDA139: 33 D2              xor         edx,edx
  0000000140CDA13B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CDA140: E8 DB 01 00 00     call        0000000140CDA320
  0000000140CDA145: 90                 nop
  0000000140CDA146: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140CDA14B: 48 83 C4 48        add         rsp,48h
  0000000140CDA14F: C3                 ret

  Summary

     1E02000 .text

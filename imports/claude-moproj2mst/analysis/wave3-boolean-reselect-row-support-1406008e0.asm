Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001406008E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001406008E5: 48 83 EC 48        sub         rsp,48h
  00000001406008E9: 48 8D 0D C8 5D 77  lea         rcx,[0000000143D766B8h]
                    03
  00000001406008F0: E8 9B 77 CC FF     call        00000001402C8090
  00000001406008F5: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001406008FA: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140600900: 74 78              je          000000014060097A
  0000000140600902: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014060090A: 4C 8D 0D 47 89 F6  lea         r9,[0000000143569258h]
                    02
  0000000140600911: 4C 8D 05 68 89 F6  lea         r8,[0000000143569280h]
                    02
  0000000140600918: 33 D2              xor         edx,edx
  000000014060091A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014060091F: E8 D8 EF E6 00     call        000000014146F8FC
  0000000140600924: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140600929: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  000000014060092F: 74 49              je          000000014060097A
  0000000140600931: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140600936: 0F B6 40 10        movzx       eax,byte ptr [rax+10h]
  000000014060093A: 85 C0              test        eax,eax
  000000014060093C: 75 1F              jne         000000014060095D
  000000014060093E: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140600943: E8 78 AD AE FF     call        00000001400EB6C0
  0000000140600948: 48 8B C8           mov         rcx,rax
  000000014060094B: E8 70 00 00 00     call        00000001406009C0
  0000000140600950: 33 D2              xor         edx,edx
  0000000140600952: 48 8B C8           mov         rcx,rax
  0000000140600955: FF 15 BD B5 80 01  call        qword ptr [0000000141E0BF18h]
  000000014060095B: EB 1D              jmp         000000014060097A
  000000014060095D: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140600962: E8 59 AD AE FF     call        00000001400EB6C0
  0000000140600967: 48 8B C8           mov         rcx,rax
  000000014060096A: E8 51 00 00 00     call        00000001406009C0
  000000014060096F: B2 01              mov         dl,1
  0000000140600971: 48 8B C8           mov         rcx,rax
  0000000140600974: FF 15 9E B5 80 01  call        qword ptr [0000000141E0BF18h]
  000000014060097A: 48 83 C4 48        add         rsp,48h
  000000014060097E: C3                 ret

  Summary

     1E02000 .text

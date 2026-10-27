Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001405F8CA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001405F8CA5: 48 83 EC 48        sub         rsp,48h
  00000001405F8CA9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001405F8CB2: B9 68 00 00 00     mov         ecx,68h
  00000001405F8CB7: E8 00 5E E7 00     call        000000014146EABC
  00000001405F8CBC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001405F8CC1: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001405F8CC7: 74 11              je          00000001405F8CDA
  00000001405F8CC9: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001405F8CCE: E8 7D 45 00 00     call        00000001405FD250
  00000001405F8CD3: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001405F8CD8: EB 09              jmp         00000001405F8CE3
  00000001405F8CDA: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001405F8CE3: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001405F8CE8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001405F8CED: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001405F8CF2: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001405F8CF7: 48 89 41 30        mov         qword ptr [rcx+30h],rax
  00000001405F8CFB: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001405F8D00: 48 83 C1 20        add         rcx,20h
  00000001405F8D04: 33 D2              xor         edx,edx
  00000001405F8D06: E8 15 3C 6C 00     call        0000000140CBC920
  00000001405F8D0B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001405F8D10: 48 83 C1 10        add         rcx,10h
  00000001405F8D14: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001405F8D19: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001405F8D1D: FF 50 18           call        qword ptr [rax+18h]
  00000001405F8D20: 48 83 C4 48        add         rsp,48h
  00000001405F8D24: C3                 ret

  Summary

     1E02000 .text

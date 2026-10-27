Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140025580: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140025585: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014002558A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014002558F: 48 83 EC 48        sub         rsp,48h
  0000000140025593: 33 C0              xor         eax,eax
  0000000140025595: 85 C0              test        eax,eax
  0000000140025597: 74 49              je          00000001400255E2
  0000000140025599: B9 04 00 00 00     mov         ecx,4
  000000014002559E: E8 19 95 44 01     call        000000014146EABC
  00000001400255A3: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400255A8: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001400255AE: 74 1A              je          00000001400255CA
  00000001400255B0: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400255B5: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400255BA: 8B 00              mov         eax,dword ptr [rax]
  00000001400255BC: 89 01              mov         dword ptr [rcx],eax
  00000001400255BE: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001400255C3: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400255C8: EB 09              jmp         00000001400255D3
  00000001400255CA: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000001400255D3: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001400255D8: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001400255DD: 48 89 01           mov         qword ptr [rcx],rax
  00000001400255E0: EB 5C              jmp         000000014002563E
  00000001400255E2: 33 C0              xor         eax,eax
  00000001400255E4: 85 C0              test        eax,eax
  00000001400255E6: 74 41              je          0000000140025629
  00000001400255E8: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001400255ED: B9 04 00 00 00     mov         ecx,4
  00000001400255F2: E8 89 DC FD FF     call        0000000140003280
  00000001400255F7: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400255FC: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140025602: 74 1A              je          000000014002561E
  0000000140025604: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140025609: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014002560E: 8B 00              mov         eax,dword ptr [rax]
  0000000140025610: 89 01              mov         dword ptr [rcx],eax
  0000000140025612: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140025617: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014002561C: EB 09              jmp         0000000140025627
  000000014002561E: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140025627: EB 15              jmp         000000014002563E
  0000000140025629: 41 B8 04 00 00 00  mov         r8d,4
  000000014002562F: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140025634: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140025639: E8 C2 9A 44 01     call        000000014146F100
  000000014002563E: 48 83 C4 48        add         rsp,48h
  0000000140025642: C3                 ret

  Summary

     1E02000 .text

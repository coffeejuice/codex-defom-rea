Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D34BD0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D34BD5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D34BDA: 48 83 EC 68        sub         rsp,68h
  0000000140D34BDE: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140D34BE7: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140D34BEC: E8 5F 01 00 00     call        0000000140D34D50
  0000000140D34BF1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D34BF6: 45 33 C0           xor         r8d,r8d
  0000000140D34BF9: 33 D2              xor         edx,edx
  0000000140D34BFB: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140D34C00: FF 15 A2 51 0D 01  call        qword ptr [0000000141E09DA8h]
  0000000140D34C06: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D34C0B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D34C10: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D34C13: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D34C18: FF 50 60           call        qword ptr [rax+60h]
  0000000140D34C1B: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D34C20: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D34C25: E8 66 48 00 00     call        0000000140D39490
  0000000140D34C2A: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D34C2F: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140D34C35: 0F 84 80 00 00 00  je          0000000140D34CBB
  0000000140D34C3B: 48 8D 15 1E 1A CD  lea         rdx,[0000000142A06660h]
                    01
  0000000140D34C42: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D34C47: FF 15 C3 F4 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D34C4D: 48 8B D0           mov         rdx,rax
  0000000140D34C50: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D34C55: FF 15 F5 F0 0C 01  call        qword ptr [0000000141E03D50h]
  0000000140D34C5B: 0F B6 C0           movzx       eax,al
  0000000140D34C5E: 85 C0              test        eax,eax
  0000000140D34C60: 74 0C              je          0000000140D34C6E
  0000000140D34C62: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140D34C67: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D34C6C: EB 4D              jmp         0000000140D34CBB
  0000000140D34C6E: 48 8D 15 FB 19 CD  lea         rdx,[0000000142A06670h]
                    01
  0000000140D34C75: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140D34C7A: FF 15 90 F4 0C 01  call        qword ptr [0000000141E04110h]
  0000000140D34C80: 48 8B D0           mov         rdx,rax
  0000000140D34C83: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D34C88: FF 15 C2 F0 0C 01  call        qword ptr [0000000141E03D50h]
  0000000140D34C8E: 0F B6 C0           movzx       eax,al
  0000000140D34C91: 85 C0              test        eax,eax
  0000000140D34C93: 74 26              je          0000000140D34CBB
  0000000140D34C95: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140D34C9A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D34C9F: E8 5C 70 07 00     call        0000000140DABD00
  0000000140D34CA4: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D34CA9: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140D34CAF: 74 0A              je          0000000140D34CBB
  0000000140D34CB1: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140D34CB6: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D34CBB: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D34CC0: 48 83 C4 68        add         rsp,68h
  0000000140D34CC4: C3                 ret

  Summary

     1E02000 .text

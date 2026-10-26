Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400AAB60: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  00000001400AAB65: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400AAB6A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400AAB6F: 48 83 EC 58        sub         rsp,58h
  00000001400AAB73: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400AAB7C: 0F B6 44 24 70     movzx       eax,byte ptr [rsp+70h]
  00000001400AAB81: 85 C0              test        eax,eax
  00000001400AAB83: 74 0A              je          00000001400AAB8F
  00000001400AAB85: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400AAB8A: E8 D1 FD FF FF     call        00000001400AA960
  00000001400AAB8F: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001400AAB94: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400AAB99: FF 15 B1 93 D5 01  call        qword ptr [0000000141E03F50h]
  00000001400AAB9F: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400AABA4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400AABA9: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400AABAE: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400AABB3: FF 15 6F 96 D5 01  call        qword ptr [0000000141E04228h]
  00000001400AABB9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400AABBE: 48 83 C1 60        add         rcx,60h
  00000001400AABC2: 48 8B D0           mov         rdx,rax
  00000001400AABC5: E8 E6 1B 24 01     call        00000001412EC7B0
  00000001400AABCA: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000001400AABCE: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400AABD3: FF 15 B7 96 D5 01  call        qword ptr [0000000141E04290h]
  00000001400AABD9: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001400AABDE: 88 44 24 30        mov         byte ptr [rsp+30h],al
  00000001400AABE2: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400AABE7: FF 15 F3 AD D5 01  call        qword ptr [0000000141E059E0h]
  00000001400AABED: 8A 44 24 30        mov         al,byte ptr [rsp+30h]
  00000001400AABF1: 48 83 C4 58        add         rsp,58h
  00000001400AABF5: C3                 ret

  Summary

     1E02000 .text

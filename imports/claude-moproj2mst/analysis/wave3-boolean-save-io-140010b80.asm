Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140010B80: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140010B85: 48 83 EC 78        sub         rsp,78h
  0000000140010B89: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140010B92: C6 44 24 28 01     mov         byte ptr [rsp+28h],1
  0000000140010B97: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140010B9F: E8 6C C9 00 00     call        000000014001D510
  0000000140010BA4: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140010BA9: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140010BAF: 0F 84 AD 00 00 00  je          0000000140010C62
  0000000140010BB5: 48 8D 0D FC 5A D6  lea         rcx,[0000000143D766B8h]
                    03
  0000000140010BBC: E8 AF AC 2B 00     call        00000001402CB870
  0000000140010BC1: 0F B6 C0           movzx       eax,al
  0000000140010BC4: 85 C0              test        eax,eax
  0000000140010BC6: 0F 84 96 00 00 00  je          0000000140010C62
  0000000140010BCC: 45 33 C0           xor         r8d,r8d
  0000000140010BCF: 48 8D 15 FA 79 0E  lea         rdx,[00000001420F85D0h]
                    02
  0000000140010BD6: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140010BDB: E8 A0 41 01 00     call        0000000140024D80
  0000000140010BE0: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140010BE5: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140010BEA: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140010BEF: 45 33 C9           xor         r9d,r9d
  0000000140010BF2: 45 33 C0           xor         r8d,r8d
  0000000140010BF5: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140010BFA: 48 8D 0D 4F 57 D6  lea         rcx,[0000000143D76350h]
                    03
  0000000140010C01: E8 CA FF 2B 00     call        00000001402D0BD0
  0000000140010C06: 90                 nop
  0000000140010C07: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140010C0C: FF 15 CE 4D DF 01  call        qword ptr [0000000141E059E0h]
  0000000140010C12: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140010C17: E8 B4 74 08 00     call        00000001400980D0
  0000000140010C1C: 45 33 C0           xor         r8d,r8d
  0000000140010C1F: 48 8D 15 BA 79 0E  lea         rdx,[00000001420F85E0h]
                    02
  0000000140010C26: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140010C2B: E8 50 41 01 00     call        0000000140024D80
  0000000140010C30: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140010C35: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140010C3A: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140010C3F: 45 33 C9           xor         r9d,r9d
  0000000140010C42: 45 33 C0           xor         r8d,r8d
  0000000140010C45: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140010C4A: 48 8D 0D FF 56 D6  lea         rcx,[0000000143D76350h]
                    03
  0000000140010C51: E8 7A FF 2B 00     call        00000001402D0BD0
  0000000140010C56: 90                 nop
  0000000140010C57: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140010C5C: FF 15 7E 4D DF 01  call        qword ptr [0000000141E059E0h]
  0000000140010C62: 8A 44 24 28        mov         al,byte ptr [rsp+28h]
  0000000140010C66: 48 83 C4 78        add         rsp,78h
  0000000140010C6A: C3                 ret

  Summary

     1E02000 .text

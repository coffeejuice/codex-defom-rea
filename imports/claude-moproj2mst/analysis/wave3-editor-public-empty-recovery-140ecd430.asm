Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140ECD430: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140ECD434: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140ECD439: 48 83 EC 38        sub         rsp,38h
  0000000140ECD43D: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000140ECD442: 85 C0              test        eax,eax
  0000000140ECD444: 0F 84 AD 00 00 00  je          0000000140ECD4F7
  0000000140ECD44A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD44F: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECD456: 33 D2              xor         edx,edx
  0000000140ECD458: 48 8B 89 C0 09 00  mov         rcx,qword ptr [rcx+00000000000009C0h]
                    00
  0000000140ECD45F: FF 15 53 B7 F3 00  call        qword ptr [0000000141E08BB8h]
  0000000140ECD465: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD46A: E8 71 3A 26 FF     call        0000000140130EE0
  0000000140ECD46F: 0F B6 C0           movzx       eax,al
  0000000140ECD472: 85 C0              test        eax,eax
  0000000140ECD474: 74 25              je          0000000140ECD49B
  0000000140ECD476: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECD47B: 48 8B 80 50 01 00  mov         rax,qword ptr [rax+0000000000000150h]
                    00
  0000000140ECD482: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD487: 48 8B 89 50 01 00  mov         rcx,qword ptr [rcx+0000000000000150h]
                    00
  0000000140ECD48E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ECD491: BA 02 00 00 00     mov         edx,2
  0000000140ECD496: FF 50 58           call        qword ptr [rax+58h]
  0000000140ECD499: EB 46              jmp         0000000140ECD4E1
  0000000140ECD49B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECD4A0: 48 8B 80 50 01 00  mov         rax,qword ptr [rax+0000000000000150h]
                    00
  0000000140ECD4A7: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD4AC: 48 8B 89 50 01 00  mov         rcx,qword ptr [rcx+0000000000000150h]
                    00
  0000000140ECD4B3: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ECD4B6: BA 08 00 00 00     mov         edx,8
  0000000140ECD4BB: FF 50 58           call        qword ptr [rax+58h]
  0000000140ECD4BE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECD4C3: 48 8B 80 50 01 00  mov         rax,qword ptr [rax+0000000000000150h]
                    00
  0000000140ECD4CA: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD4CF: 48 8B 89 50 01 00  mov         rcx,qword ptr [rcx+0000000000000150h]
                    00
  0000000140ECD4D6: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ECD4D9: BA 04 00 00 00     mov         edx,4
  0000000140ECD4DE: FF 50 60           call        qword ptr [rax+60h]
  0000000140ECD4E1: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140ECD4E6: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140ECD4EB: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD4F0: E8 0B 35 FF FF     call        0000000140EC0A00
  0000000140ECD4F5: EB 34              jmp         0000000140ECD52B
  0000000140ECD4F7: C6 44 24 21 00     mov         byte ptr [rsp+21h],0
  0000000140ECD4FC: 48 8D 54 24 21     lea         rdx,[rsp+21h]
  0000000140ECD501: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD506: E8 F5 34 FF FF     call        0000000140EC0A00
  0000000140ECD50B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140ECD510: 48 8B 80 50 01 00  mov         rax,qword ptr [rax+0000000000000150h]
                    00
  0000000140ECD517: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140ECD51C: 48 8B 89 50 01 00  mov         rcx,qword ptr [rcx+0000000000000150h]
                    00
  0000000140ECD523: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ECD526: 33 D2              xor         edx,edx
  0000000140ECD528: FF 50 68           call        qword ptr [rax+68h]
  0000000140ECD52B: 48 83 C4 38        add         rsp,38h
  0000000140ECD52F: C3                 ret

  Summary

     1E02000 .text

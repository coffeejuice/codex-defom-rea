Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407C7D00: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001407C7D05: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407C7D0A: 48 83 EC 48        sub         rsp,48h
  00000001407C7D0E: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001407C7D17: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001407C7D1C: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001407C7D20: 48 83 C1 10        add         rcx,10h
  00000001407C7D24: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001407C7D29: E8 F2 8B 8A FF     call        0000000140070920
  00000001407C7D2E: 0F B6 C0           movzx       eax,al
  00000001407C7D31: 85 C0              test        eax,eax
  00000001407C7D33: 74 31              je          00000001407C7D66
  00000001407C7D35: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001407C7D3A: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001407C7D3E: 48 83 C1 10        add         rcx,10h
  00000001407C7D42: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001407C7D47: E8 04 D3 4A 00     call        0000000140C75050
  00000001407C7D4C: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407C7D4F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001407C7D54: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001407C7D59: FF 15 81 DC 63 01  call        qword ptr [0000000141E059E0h]
  00000001407C7D5F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001407C7D64: EB 19              jmp         00000001407C7D7F
  00000001407C7D66: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000001407C7D6F: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001407C7D74: FF 15 66 DC 63 01  call        qword ptr [0000000141E059E0h]
  00000001407C7D7A: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001407C7D7F: 48 83 C4 48        add         rsp,48h
  00000001407C7D83: C3                 ret

  Summary

     1E02000 .text

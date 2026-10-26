Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140919B80: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140919B85: 48 83 EC 48        sub         rsp,48h
  0000000140919B89: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140919B92: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140919B97: E8 24 44 00 00     call        000000014091DFC0
  0000000140919B9C: 90                 nop
  0000000140919B9D: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140919BA2: 48 8D 05 FF 47 EA  lea         rax,[00000001427BE3A8h]
                    01
  0000000140919BA9: 48 89 01           mov         qword ptr [rcx],rax
  0000000140919BAC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140919BB1: 48 83 C1 30        add         rcx,30h
  0000000140919BB5: FF 15 05 A9 4E 01  call        qword ptr [0000000141E044C0h]
  0000000140919BBB: 90                 nop
  0000000140919BBC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140919BC1: 48 83 C1 38        add         rcx,38h
  0000000140919BC5: E8 16 5E 87 FF     call        000000014018F9E0
  0000000140919BCA: 90                 nop
  0000000140919BCB: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140919BD0: 48 83 C1 58        add         rcx,58h
  0000000140919BD4: 48 8D 05 25 2B 7A  lea         rax,[00000001400BC700h]
                    FF
  0000000140919BDB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140919BE0: 4C 8D 0D D9 E5 7B  lea         r9,[00000001400D81C0h]
                    FF
  0000000140919BE7: 41 B8 03 00 00 00  mov         r8d,3
  0000000140919BED: BA 08 00 00 00     mov         edx,8
  0000000140919BF2: E8 59 51 B5 00     call        000000014146ED50
  0000000140919BF7: 90                 nop
  0000000140919BF8: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140919BFD: 48 81 C1 80 00 00  add         rcx,80h
                    00
  0000000140919C04: FF 15 B6 A8 4E 01  call        qword ptr [0000000141E044C0h]
  0000000140919C0A: 90                 nop
  0000000140919C0B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140919C10: 48 81 C1 88 00 00  add         rcx,88h
                    00
  0000000140919C17: FF 15 A3 A8 4E 01  call        qword ptr [0000000141E044C0h]
  0000000140919C1D: 4C 8B 5C 24 50     mov         r11,qword ptr [rsp+50h]
  0000000140919C22: 41 C7 43 28 01 00  mov         dword ptr [r11+28h],1
                    00 00
  0000000140919C2A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140919C2F: C7 40 40 00 00 00  mov         dword ptr [rax+40h],0
                    00
  0000000140919C36: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140919C3B: C7 40 44 00 00 00  mov         dword ptr [rax+44h],0
                    00
  0000000140919C42: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140919C47: C7 40 48 64 00 00  mov         dword ptr [rax+48h],64h
                    00
  0000000140919C4E: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140919C53: C7 40 74 FF FF FF  mov         dword ptr [rax+74h],0FFFFFFFFh
                    FF
  0000000140919C5A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140919C5F: C6 40 78 00        mov         byte ptr [rax+78h],0
  0000000140919C63: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140919C68: C7 40 7C 00 00 00  mov         dword ptr [rax+7Ch],0
                    00
  0000000140919C6F: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140919C77: EB 0B              jmp         0000000140919C84
  0000000140919C79: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140919C7D: 83 C0 01           add         eax,1
  0000000140919C80: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140919C84: 83 7C 24 30 03     cmp         dword ptr [rsp+30h],3
  0000000140919C89: 7D 11              jge         0000000140919C9C
  0000000140919C8B: 48 63 4C 24 30     movsxd      rcx,dword ptr [rsp+30h]
  0000000140919C90: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140919C95: C6 44 08 70 01     mov         byte ptr [rax+rcx+70h],1
  0000000140919C9A: EB DD              jmp         0000000140919C79
  0000000140919C9C: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140919CA1: 48 83 C4 48        add         rsp,48h
  0000000140919CA5: C3                 ret

  Summary

     1E02000 .text

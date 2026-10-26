Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140546CA0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140546CA5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140546CAA: 48 83 EC 48        sub         rsp,48h
  0000000140546CAE: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140546CB7: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140546CBC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546CC1: E8 BA DD E5 FF     call        00000001403A4A80
  0000000140546CC6: 90                 nop
  0000000140546CC7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546CCC: 48 8D 05 35 1A EF  lea         rax,[0000000142438708h]
                    01
  0000000140546CD3: 48 89 01           mov         qword ptr [rcx],rax
  0000000140546CD6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546CDB: 48 8D 05 EE 19 EF  lea         rax,[00000001424386D0h]
                    01
  0000000140546CE2: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140546CE6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546CEB: 48 83 C1 78        add         rcx,78h
  0000000140546CEF: FF 15 CB D7 8B 01  call        qword ptr [0000000141E044C0h]
  0000000140546CF5: 90                 nop
  0000000140546CF6: BA 01 00 00 00     mov         edx,1
  0000000140546CFB: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546D00: FF 15 52 F6 8B 01  call        qword ptr [0000000141E06358h]
  0000000140546D06: BA 01 00 00 00     mov         edx,1
  0000000140546D0B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546D10: FF 15 32 F6 8B 01  call        qword ptr [0000000141E06348h]
  0000000140546D16: 33 D2              xor         edx,edx
  0000000140546D18: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546D1D: FF 15 2D F6 8B 01  call        qword ptr [0000000141E06350h]
  0000000140546D23: 4C 8B 5C 24 50     mov         r11,qword ptr [rsp+50h]
  0000000140546D28: 49 C7 43 68 00 00  mov         qword ptr [r11+68h],0
                    00 00
  0000000140546D30: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140546D35: 48 C7 80 B8 08 00  mov         qword ptr [rax+00000000000008B8h],0
                    00 00 00 00 00
  0000000140546D40: C7 44 24 30 01 00  mov         dword ptr [rsp+30h],1
                    00 00
  0000000140546D48: EB 0B              jmp         0000000140546D55
  0000000140546D4A: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140546D4E: 83 C0 01           add         eax,1
  0000000140546D51: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140546D55: 83 7C 24 30 04     cmp         dword ptr [rsp+30h],4
  0000000140546D5A: 7D 1D              jge         0000000140546D79
  0000000140546D5C: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140546D60: 83 E8 01           sub         eax,1
  0000000140546D63: 48 63 C8           movsxd      rcx,eax
  0000000140546D66: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140546D6B: 48 C7 84 C8 C0 08  mov         qword ptr [rax+rcx*8+00000000000008C0h],0
                    00 00 00 00 00 00
  0000000140546D77: EB D1              jmp         0000000140546D4A
  0000000140546D79: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546D7E: 48 83 C1 78        add         rcx,78h
  0000000140546D82: 48 8D 15 16 F9 EE  lea         rdx,[000000014243669Fh]
                    01
  0000000140546D89: FF 15 C9 D0 8B 01  call        qword ptr [0000000141E03E58h]
  0000000140546D8F: 4C 8B 5C 24 50     mov         r11,qword ptr [rsp+50h]
  0000000140546D94: 41 C7 43 70 00 00  mov         dword ptr [r11+70h],0
                    00 00
  0000000140546D9C: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140546DA4: 4C 8D 0D DD 05 EF  lea         r9,[0000000142437388h]
                    01
  0000000140546DAB: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  0000000140546DB0: 48 8D 15 F1 05 EF  lea         rdx,[00000001424373A8h]
                    01
  0000000140546DB7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546DBC: FF 15 0E D4 8B 01  call        qword ptr [0000000141E041D0h]
  0000000140546DC2: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140546DCA: 4C 8D 0D EF 05 EF  lea         r9,[00000001424373C0h]
                    01
  0000000140546DD1: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  0000000140546DD6: 48 8D 15 03 06 EF  lea         rdx,[00000001424373E0h]
                    01
  0000000140546DDD: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546DE2: FF 15 E8 D3 8B 01  call        qword ptr [0000000141E041D0h]
  0000000140546DE8: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  0000000140546DED: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140546DF2: FF 15 10 E8 8B 01  call        qword ptr [0000000141E05608h]
  0000000140546DF8: 90                 nop
  0000000140546DF9: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140546DFE: 48 83 C4 48        add         rsp,48h
  0000000140546E02: C3                 ret

  Summary

     1E02000 .text

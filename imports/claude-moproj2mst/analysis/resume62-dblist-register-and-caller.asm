Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140651EAA: 0F B6 84 24 68 06  movzx       eax,byte ptr [rsp+0000000000000668h]
                    00 00
  0000000140651EB2: 85 C0              test        eax,eax
  0000000140651EB4: 0F 85 A7 00 00 00  jne         0000000140651F61
  0000000140651EBA: 48 8D 0D F7 47 72  lea         rcx,[0000000143D766B8h]
                    03
  0000000140651EC1: E8 FA 9F C7 FF     call        00000001402CBEC0
  0000000140651EC6: 0F B6 C0           movzx       eax,al
  0000000140651EC9: 85 C0              test        eax,eax
  0000000140651ECB: 0F 84 90 00 00 00  je          0000000140651F61
  0000000140651ED1: 48 8D 84 24 40 03  lea         rax,[rsp+0000000000000340h]
                    00 00
  0000000140651ED9: 48 89 84 24 48 03  mov         qword ptr [rsp+0000000000000348h],rax
                    00 00
  0000000140651EE1: 48 8D 84 24 50 03  lea         rax,[rsp+0000000000000350h]
                    00 00
  0000000140651EE9: 48 89 84 24 58 03  mov         qword ptr [rsp+0000000000000358h],rax
                    00 00
  0000000140651EF1: 48 8B 94 24 50 06  mov         rdx,qword ptr [rsp+0000000000000650h]
                    00 00
  0000000140651EF9: 48 8B 8C 24 48 03  mov         rcx,qword ptr [rsp+0000000000000348h]
                    00 00
  0000000140651F01: FF 15 D1 3A 7B 01  call        qword ptr [0000000141E059D8h]
  0000000140651F07: 48 89 84 24 D8 05  mov         qword ptr [rsp+00000000000005D8h],rax
                    00 00
  0000000140651F0F: 48 8B 84 24 D8 05  mov         rax,qword ptr [rsp+00000000000005D8h]
                    00 00
  0000000140651F17: 48 89 84 24 E0 05  mov         qword ptr [rsp+00000000000005E0h],rax
                    00 00
  0000000140651F1F: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  0000000140651F27: 48 8B 8C 24 58 03  mov         rcx,qword ptr [rsp+0000000000000358h]
                    00 00
  0000000140651F2F: FF 15 5B A6 7B 01  call        qword ptr [0000000141E0C590h]
  0000000140651F35: 48 89 84 24 E8 05  mov         qword ptr [rsp+00000000000005E8h],rax
                    00 00
  0000000140651F3D: 4C 8B 84 24 E0 05  mov         r8,qword ptr [rsp+00000000000005E0h]
                    00 00
  0000000140651F45: 48 8B 94 24 E8 05  mov         rdx,qword ptr [rsp+00000000000005E8h]
                    00 00
  0000000140651F4D: 48 8B 8C 24 40 06  mov         rcx,qword ptr [rsp+0000000000000640h]
                    00 00
  0000000140651F55: E8 46 35 00 00     call        00000001406554A0
  0000000140651F5A: 88 84 24 94 00 00  mov         byte ptr [rsp+0000000000000094h],al
                    00

  Summary

     1E02000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C79390: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C79395: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C7939A: 48 83 EC 48        sub         rsp,48h
  0000000140C7939E: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140C793A7: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C793AC: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C793AF: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140C793B4: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C793B9: FF 50 68           call        qword ptr [rax+68h]
  0000000140C793BC: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C793C1: 45 33 C0           xor         r8d,r8d
  0000000140C793C4: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140C793C9: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C793CE: E8 ED 39 06 00     call        0000000140CDCDC0
  0000000140C793D3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C793D8: E8 93 00 00 00     call        0000000140C79470
  0000000140C793DD: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140C793E1: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140C793E5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C793EA: E8 51 43 06 00     call        0000000140CDD740
  0000000140C793EF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C793F4: 48 83 C1 18        add         rcx,18h
  0000000140C793F8: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140C793FD: E8 DE 43 AB 00     call        000000014172D7E0
  0000000140C79402: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140C7940A: 4C 8D 0D A7 0D C8  lea         r9,[00000001428FA1B8h]
                    01
  0000000140C79411: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  0000000140C79416: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  0000000140C7941A: 48 8D 15 B7 0D C8  lea         rdx,[00000001428FA1D8h]
                    01
  0000000140C79421: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C79426: FF 15 A4 AD 18 01  call        qword ptr [0000000141E041D0h]
  0000000140C7942C: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000140C79434: 4C 8D 0D C5 0D C8  lea         r9,[00000001428FA200h]
                    01
  0000000140C7943B: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  0000000140C79440: 4D 8B 40 10        mov         r8,qword ptr [r8+10h]
  0000000140C79444: 48 8D 15 CD 0D C8  lea         rdx,[00000001428FA218h]
                    01
  0000000140C7944B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C79450: FF 15 7A AD 18 01  call        qword ptr [0000000141E041D0h]
  0000000140C79456: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C7945B: 48 83 C4 48        add         rsp,48h
  0000000140C7945F: C3                 ret

  Summary

     1E02000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C78C10: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C78C15: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C78C1A: 48 81 EC B8 00 00  sub         rsp,0B8h
                    00
  0000000140C78C21: 48 C7 44 24 68 FE  mov         qword ptr [rsp+68h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C78C2A: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140C78C33: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140C78C3C: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140C78C44: 48 83 C2 18        add         rdx,18h
  0000000140C78C48: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140C78C4D: E8 9E 4E 3F FF     call        000000014006DAF0
  0000000140C78C52: 90                 nop
  0000000140C78C53: 48 8D 44 24 38     lea         rax,[rsp+38h]
  0000000140C78C58: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C78C5D: EB 43              jmp         0000000140C78CA2
  0000000140C78C5F: 33 C0              xor         eax,eax
  0000000140C78C61: 83 F8 01           cmp         eax,1
  0000000140C78C64: 74 0B              je          0000000140C78C71
  0000000140C78C66: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  0000000140C78C6F: EB 16              jmp         0000000140C78C87
  0000000140C78C71: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140C78C79: 48 83 C1 18        add         rcx,18h
  0000000140C78C7D: E8 CE BF 50 FF     call        0000000140184C50
  0000000140C78C82: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140C78C87: 48 8B 54 24 70     mov         rdx,qword ptr [rsp+70h]
  0000000140C78C8C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C78C91: E8 EA 36 18 00     call        0000000140DFC380
  0000000140C78C96: 48 8B C8           mov         rcx,rax
  0000000140C78C99: 48 83 C1 10        add         rcx,10h
  0000000140C78C9D: E8 5E 0B 17 00     call        0000000140DE9800
  0000000140C78CA2: 33 C0              xor         eax,eax
  0000000140C78CA4: 83 F8 01           cmp         eax,1
  0000000140C78CA7: 74 0B              je          0000000140C78CB4
  0000000140C78CA9: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  0000000140C78CB2: EB 16              jmp         0000000140C78CCA
  0000000140C78CB4: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140C78CBC: 48 83 C1 18        add         rcx,18h
  0000000140C78CC0: E8 8B BF 50 FF     call        0000000140184C50
  0000000140C78CC5: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140C78CCA: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140C78CCF: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C78CD4: E8 A7 36 18 00     call        0000000140DFC380
  0000000140C78CD9: 48 8B C8           mov         rcx,rax
  0000000140C78CDC: E8 9F 33 18 00     call        0000000140DFC080
  0000000140C78CE1: 0F B6 C0           movzx       eax,al
  0000000140C78CE4: 85 C0              test        eax,eax
  0000000140C78CE6: 0F 84 6F 01 00 00  je          0000000140C78E5B
  0000000140C78CEC: 33 C0              xor         eax,eax
  0000000140C78CEE: 83 F8 01           cmp         eax,1
  0000000140C78CF1: 74 0E              je          0000000140C78D01
  0000000140C78CF3: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00 00
  0000000140C78CFF: EB 19              jmp         0000000140C78D1A
  0000000140C78D01: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140C78D09: 48 83 C1 18        add         rcx,18h
  0000000140C78D0D: E8 3E BF 50 FF     call        0000000140184C50
  0000000140C78D12: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140C78D1A: 48 8B 94 24 80 00  mov         rdx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C78D22: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C78D27: E8 54 36 18 00     call        0000000140DFC380
  0000000140C78D2C: 48 8B C8           mov         rcx,rax
  0000000140C78D2F: 48 83 C1 10        add         rcx,10h
  0000000140C78D33: E8 C8 66 9D FF     call        000000014064F400
  0000000140C78D38: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C78D3B: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C78D40: EB 63              jmp         0000000140C78DA5
  0000000140C78D42: 33 C0              xor         eax,eax
  0000000140C78D44: 83 F8 01           cmp         eax,1
  0000000140C78D47: 74 0E              je          0000000140C78D57
  0000000140C78D49: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  0000000140C78D55: EB 19              jmp         0000000140C78D70
  0000000140C78D57: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140C78D5F: 48 83 C1 18        add         rcx,18h
  0000000140C78D63: E8 E8 BE 50 FF     call        0000000140184C50
  0000000140C78D68: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140C78D70: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140C78D78: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C78D7D: E8 FE 35 18 00     call        0000000140DFC380
  0000000140C78D82: 48 83 C0 08        add         rax,8
  0000000140C78D86: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140C78D8E: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140C78D96: 8B 08              mov         ecx,dword ptr [rax]
  0000000140C78D98: 83 E9 01           sub         ecx,1
  0000000140C78D9B: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140C78DA3: 89 08              mov         dword ptr [rax],ecx
  0000000140C78DA5: 33 C0              xor         eax,eax
  0000000140C78DA7: 83 F8 01           cmp         eax,1
  0000000140C78DAA: 74 0E              je          0000000140C78DBA
  0000000140C78DAC: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00 00
  0000000140C78DB8: EB 19              jmp         0000000140C78DD3
  0000000140C78DBA: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140C78DC2: 48 83 C1 18        add         rcx,18h
  0000000140C78DC6: E8 85 BE 50 FF     call        0000000140184C50
  0000000140C78DCB: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140C78DD3: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140C78DDB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C78DE0: E8 9B 35 18 00     call        0000000140DFC380
  0000000140C78DE5: 83 78 08 00        cmp         dword ptr [rax+8],0
  0000000140C78DE9: 74 6B              je          0000000140C78E56
  0000000140C78DEB: 41 B0 01           mov         r8b,1
  0000000140C78DEE: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000140C78DF3: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140C78DF8: E8 93 43 06 00     call        0000000140CDD190
  0000000140C78DFD: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140C78E05: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140C78E0D: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140C78E15: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140C78E1D: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140C78E25: FF 15 DD B0 18 01  call        qword ptr [0000000141E03F08h]
  0000000140C78E2B: 88 44 24 58        mov         byte ptr [rsp+58h],al
  0000000140C78E2F: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140C78E34: FF 15 A6 CB 18 01  call        qword ptr [0000000141E059E0h]
  0000000140C78E3A: 44 0F B6 5C 24 58  movzx       r11d,byte ptr [rsp+58h]
  0000000140C78E40: 45 85 DB           test        r11d,r11d
  0000000140C78E43: 74 0C              je          0000000140C78E51
  0000000140C78E45: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C78E4A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C78E4F: EB 05              jmp         0000000140C78E56
  0000000140C78E51: E9 EC FE FF FF     jmp         0000000140C78D42
  0000000140C78E56: E9 04 FE FF FF     jmp         0000000140C78C5F
  0000000140C78E5B: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140C78E60: E8 BB 86 FD FF     call        0000000140C51520
  0000000140C78E65: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140C78E6A: 48 81 C4 B8 00 00  add         rsp,0B8h
                    00
  0000000140C78E71: C3                 ret

  Summary

     1E02000 .text

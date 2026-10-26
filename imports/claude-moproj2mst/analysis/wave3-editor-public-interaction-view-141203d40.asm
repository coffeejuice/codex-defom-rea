Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141203D40: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000141203D45: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000141203D4A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141203D4F: 48 83 EC 38        sub         rsp,38h
  0000000141203D53: C6 44 24 21 00     mov         byte ptr [rsp+21h],0
  0000000141203D58: 83 7C 24 50 00     cmp         dword ptr [rsp+50h],0
  0000000141203D5D: 75 0A              jne         0000000141203D69
  0000000141203D5F: C7 44 24 24 01 00  mov         dword ptr [rsp+24h],1
                    00 00
  0000000141203D67: EB 08              jmp         0000000141203D71
  0000000141203D69: C7 44 24 24 00 00  mov         dword ptr [rsp+24h],0
                    00 00
  0000000141203D71: 0F B6 44 24 24     movzx       eax,byte ptr [rsp+24h]
  0000000141203D76: 88 44 24 20        mov         byte ptr [rsp+20h],al
  0000000141203D7A: 4C 8D 0D D7 B5 AE  lea         r9,[0000000142CEF358h]
                    01
  0000000141203D81: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141203D86: 48 8D 15 FB B5 AE  lea         rdx,[0000000142CEF388h]
                    01
  0000000141203D8D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203D92: FF 15 F8 07 C0 00  call        qword ptr [0000000141E04590h]
  0000000141203D98: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141203D9C: 83 7C 24 50 01     cmp         dword ptr [rsp+50h],1
  0000000141203DA1: 74 09              je          0000000141203DAC
  0000000141203DA3: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141203DA8: 85 C0              test        eax,eax
  0000000141203DAA: 74 3A              je          0000000141203DE6
  0000000141203DAC: 4C 8D 0D 0D B6 AE  lea         r9,[0000000142CEF3C0h]
                    01
  0000000141203DB3: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141203DB8: 48 8D 15 29 B6 AE  lea         rdx,[0000000142CEF3E8h]
                    01
  0000000141203DBF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203DC4: FF 15 C6 07 C0 00  call        qword ptr [0000000141E04590h]
  0000000141203DCA: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141203DCE: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141203DD3: 85 C0              test        eax,eax
  0000000141203DD5: 74 0F              je          0000000141203DE6
  0000000141203DD7: BA 01 00 00 00     mov         edx,1
  0000000141203DDC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203DE1: E8 DA 37 00 00     call        00000001412075C0
  0000000141203DE6: 83 7C 24 50 02     cmp         dword ptr [rsp+50h],2
  0000000141203DEB: 74 09              je          0000000141203DF6
  0000000141203DED: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141203DF2: 85 C0              test        eax,eax
  0000000141203DF4: 74 3A              je          0000000141203E30
  0000000141203DF6: 4C 8D 0D 13 B6 AE  lea         r9,[0000000142CEF410h]
                    01
  0000000141203DFD: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141203E02: 48 8D 15 37 B6 AE  lea         rdx,[0000000142CEF440h]
                    01
  0000000141203E09: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203E0E: FF 15 7C 07 C0 00  call        qword ptr [0000000141E04590h]
  0000000141203E14: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141203E18: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141203E1D: 85 C0              test        eax,eax
  0000000141203E1F: 74 0F              je          0000000141203E30
  0000000141203E21: BA 02 00 00 00     mov         edx,2
  0000000141203E26: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203E2B: E8 90 37 00 00     call        00000001412075C0
  0000000141203E30: 83 7C 24 50 03     cmp         dword ptr [rsp+50h],3
  0000000141203E35: 74 09              je          0000000141203E40
  0000000141203E37: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141203E3C: 85 C0              test        eax,eax
  0000000141203E3E: 74 62              je          0000000141203EA2
  0000000141203E40: 4C 8D 0D 29 B6 AE  lea         r9,[0000000142CEF470h]
                    01
  0000000141203E47: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141203E4C: 48 8D 15 3D B6 AE  lea         rdx,[0000000142CEF490h]
                    01
  0000000141203E53: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203E58: FF 15 32 07 C0 00  call        qword ptr [0000000141E04590h]
  0000000141203E5E: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141203E62: 4C 8D 0D 47 B6 AE  lea         r9,[0000000142CEF4B0h]
                    01
  0000000141203E69: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141203E6E: 48 8D 15 5B B6 AE  lea         rdx,[0000000142CEF4D0h]
                    01
  0000000141203E75: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203E7A: FF 15 10 07 C0 00  call        qword ptr [0000000141E04590h]
  0000000141203E80: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141203E85: 85 C0              test        eax,eax
  0000000141203E87: 74 19              je          0000000141203EA2
  0000000141203E89: BA 03 00 00 00     mov         edx,3
  0000000141203E8E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203E93: E8 28 37 00 00     call        00000001412075C0
  0000000141203E98: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203E9D: E8 1E 74 00 00     call        000000014120B2C0
  0000000141203EA2: 83 7C 24 50 05     cmp         dword ptr [rsp+50h],5
  0000000141203EA7: 74 09              je          0000000141203EB2
  0000000141203EA9: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141203EAE: 85 C0              test        eax,eax
  0000000141203EB0: 74 3A              je          0000000141203EEC
  0000000141203EB2: 4C 8D 0D 37 B6 AE  lea         r9,[0000000142CEF4F0h]
                    01
  0000000141203EB9: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141203EBE: 48 8D 15 5B B6 AE  lea         rdx,[0000000142CEF520h]
                    01
  0000000141203EC5: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203ECA: FF 15 C0 06 C0 00  call        qword ptr [0000000141E04590h]
  0000000141203ED0: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141203ED4: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141203ED9: 85 C0              test        eax,eax
  0000000141203EDB: 74 0F              je          0000000141203EEC
  0000000141203EDD: BA 05 00 00 00     mov         edx,5
  0000000141203EE2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203EE7: E8 D4 36 00 00     call        00000001412075C0
  0000000141203EEC: 83 7C 24 50 06     cmp         dword ptr [rsp+50h],6
  0000000141203EF1: 74 09              je          0000000141203EFC
  0000000141203EF3: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141203EF8: 85 C0              test        eax,eax
  0000000141203EFA: 74 3A              je          0000000141203F36
  0000000141203EFC: 4C 8D 0D 4D B6 AE  lea         r9,[0000000142CEF550h]
                    01
  0000000141203F03: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141203F08: 48 8D 15 69 B6 AE  lea         rdx,[0000000142CEF578h]
                    01
  0000000141203F0F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203F14: FF 15 76 06 C0 00  call        qword ptr [0000000141E04590h]
  0000000141203F1A: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141203F1E: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141203F23: 85 C0              test        eax,eax
  0000000141203F25: 74 0F              je          0000000141203F36
  0000000141203F27: BA 06 00 00 00     mov         edx,6
  0000000141203F2C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203F31: E8 8A 36 00 00     call        00000001412075C0
  0000000141203F36: 83 7C 24 50 07     cmp         dword ptr [rsp+50h],7
  0000000141203F3B: 74 09              je          0000000141203F46
  0000000141203F3D: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141203F42: 85 C0              test        eax,eax
  0000000141203F44: 74 3A              je          0000000141203F80
  0000000141203F46: 4C 8D 0D 5B B6 AE  lea         r9,[0000000142CEF5A8h]
                    01
  0000000141203F4D: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141203F52: 48 8D 15 77 B6 AE  lea         rdx,[0000000142CEF5D0h]
                    01
  0000000141203F59: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203F5E: FF 15 2C 06 C0 00  call        qword ptr [0000000141E04590h]
  0000000141203F64: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141203F68: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141203F6D: 85 C0              test        eax,eax
  0000000141203F6F: 74 0F              je          0000000141203F80
  0000000141203F71: BA 07 00 00 00     mov         edx,7
  0000000141203F76: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203F7B: E8 40 36 00 00     call        00000001412075C0
  0000000141203F80: 83 7C 24 50 08     cmp         dword ptr [rsp+50h],8
  0000000141203F85: 74 09              je          0000000141203F90
  0000000141203F87: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141203F8C: 85 C0              test        eax,eax
  0000000141203F8E: 74 3A              je          0000000141203FCA
  0000000141203F90: 4C 8D 0D 61 B6 AE  lea         r9,[0000000142CEF5F8h]
                    01
  0000000141203F97: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141203F9C: 48 8D 15 7D B6 AE  lea         rdx,[0000000142CEF620h]
                    01
  0000000141203FA3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203FA8: FF 15 E2 05 C0 00  call        qword ptr [0000000141E04590h]
  0000000141203FAE: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141203FB2: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141203FB7: 85 C0              test        eax,eax
  0000000141203FB9: 74 0F              je          0000000141203FCA
  0000000141203FBB: BA 08 00 00 00     mov         edx,8
  0000000141203FC0: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203FC5: E8 F6 35 00 00     call        00000001412075C0
  0000000141203FCA: 83 7C 24 50 09     cmp         dword ptr [rsp+50h],9
  0000000141203FCF: 74 09              je          0000000141203FDA
  0000000141203FD1: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141203FD6: 85 C0              test        eax,eax
  0000000141203FD8: 74 3A              je          0000000141204014
  0000000141203FDA: 4C 8D 0D 67 B6 AE  lea         r9,[0000000142CEF648h]
                    01
  0000000141203FE1: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141203FE6: 48 8D 15 83 B6 AE  lea         rdx,[0000000142CEF670h]
                    01
  0000000141203FED: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141203FF2: FF 15 98 05 C0 00  call        qword ptr [0000000141E04590h]
  0000000141203FF8: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141203FFC: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141204001: 85 C0              test        eax,eax
  0000000141204003: 74 0F              je          0000000141204014
  0000000141204005: BA 09 00 00 00     mov         edx,9
  000000014120400A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120400F: E8 AC 35 00 00     call        00000001412075C0
  0000000141204014: 83 7C 24 50 0A     cmp         dword ptr [rsp+50h],0Ah
  0000000141204019: 74 09              je          0000000141204024
  000000014120401B: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141204020: 85 C0              test        eax,eax
  0000000141204022: 74 3A              je          000000014120405E
  0000000141204024: 4C 8D 0D 75 B6 AE  lea         r9,[0000000142CEF6A0h]
                    01
  000000014120402B: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141204030: 48 8D 15 91 B6 AE  lea         rdx,[0000000142CEF6C8h]
                    01
  0000000141204037: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120403C: FF 15 4E 05 C0 00  call        qword ptr [0000000141E04590h]
  0000000141204042: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141204046: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  000000014120404B: 85 C0              test        eax,eax
  000000014120404D: 74 0F              je          000000014120405E
  000000014120404F: BA 0A 00 00 00     mov         edx,0Ah
  0000000141204054: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204059: E8 62 35 00 00     call        00000001412075C0
  000000014120405E: 83 7C 24 50 0B     cmp         dword ptr [rsp+50h],0Bh
  0000000141204063: 74 09              je          000000014120406E
  0000000141204065: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014120406A: 85 C0              test        eax,eax
  000000014120406C: 74 3A              je          00000001412040A8
  000000014120406E: 4C 8D 0D 7B B6 AE  lea         r9,[0000000142CEF6F0h]
                    01
  0000000141204075: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  000000014120407A: 48 8D 15 97 B6 AE  lea         rdx,[0000000142CEF718h]
                    01
  0000000141204081: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204086: FF 15 04 05 C0 00  call        qword ptr [0000000141E04590h]
  000000014120408C: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141204090: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141204095: 85 C0              test        eax,eax
  0000000141204097: 74 0F              je          00000001412040A8
  0000000141204099: BA 0B 00 00 00     mov         edx,0Bh
  000000014120409E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412040A3: E8 18 35 00 00     call        00000001412075C0
  00000001412040A8: 83 7C 24 50 0C     cmp         dword ptr [rsp+50h],0Ch
  00000001412040AD: 74 09              je          00000001412040B8
  00000001412040AF: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001412040B4: 85 C0              test        eax,eax
  00000001412040B6: 74 3A              je          00000001412040F2
  00000001412040B8: 4C 8D 0D 89 B6 AE  lea         r9,[0000000142CEF748h]
                    01
  00000001412040BF: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001412040C4: 48 8D 15 9D B6 AE  lea         rdx,[0000000142CEF768h]
                    01
  00000001412040CB: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412040D0: FF 15 BA 04 C0 00  call        qword ptr [0000000141E04590h]
  00000001412040D6: 88 44 24 21        mov         byte ptr [rsp+21h],al
  00000001412040DA: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  00000001412040DF: 85 C0              test        eax,eax
  00000001412040E1: 74 0F              je          00000001412040F2
  00000001412040E3: BA 0C 00 00 00     mov         edx,0Ch
  00000001412040E8: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412040ED: E8 CE 34 00 00     call        00000001412075C0
  00000001412040F2: 83 7C 24 50 0D     cmp         dword ptr [rsp+50h],0Dh
  00000001412040F7: 74 09              je          0000000141204102
  00000001412040F9: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001412040FE: 85 C0              test        eax,eax
  0000000141204100: 74 3A              je          000000014120413C
  0000000141204102: 4C 8D 0D 7F B6 AE  lea         r9,[0000000142CEF788h]
                    01
  0000000141204109: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  000000014120410E: 48 8D 15 93 B6 AE  lea         rdx,[0000000142CEF7A8h]
                    01
  0000000141204115: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120411A: FF 15 70 04 C0 00  call        qword ptr [0000000141E04590h]
  0000000141204120: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141204124: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141204129: 85 C0              test        eax,eax
  000000014120412B: 74 0F              je          000000014120413C
  000000014120412D: BA 0D 00 00 00     mov         edx,0Dh
  0000000141204132: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204137: E8 84 34 00 00     call        00000001412075C0
  000000014120413C: 83 7C 24 50 0E     cmp         dword ptr [rsp+50h],0Eh
  0000000141204141: 74 09              je          000000014120414C
  0000000141204143: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141204148: 85 C0              test        eax,eax
  000000014120414A: 74 3A              je          0000000141204186
  000000014120414C: 4C 8D 0D 75 B6 AE  lea         r9,[0000000142CEF7C8h]
                    01
  0000000141204153: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141204158: 48 8D 15 89 B6 AE  lea         rdx,[0000000142CEF7E8h]
                    01
  000000014120415F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204164: FF 15 26 04 C0 00  call        qword ptr [0000000141E04590h]
  000000014120416A: 88 44 24 21        mov         byte ptr [rsp+21h],al
  000000014120416E: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141204173: 85 C0              test        eax,eax
  0000000141204175: 74 0F              je          0000000141204186
  0000000141204177: BA 0F 00 00 00     mov         edx,0Fh
  000000014120417C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204181: E8 3A 34 00 00     call        00000001412075C0
  0000000141204186: 83 7C 24 50 0F     cmp         dword ptr [rsp+50h],0Fh
  000000014120418B: 74 09              je          0000000141204196
  000000014120418D: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141204192: 85 C0              test        eax,eax
  0000000141204194: 74 3A              je          00000001412041D0
  0000000141204196: 4C 8D 0D 6B B6 AE  lea         r9,[0000000142CEF808h]
                    01
  000000014120419D: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001412041A2: 48 8D 15 7F B6 AE  lea         rdx,[0000000142CEF828h]
                    01
  00000001412041A9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412041AE: FF 15 DC 03 C0 00  call        qword ptr [0000000141E04590h]
  00000001412041B4: 88 44 24 21        mov         byte ptr [rsp+21h],al
  00000001412041B8: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  00000001412041BD: 85 C0              test        eax,eax
  00000001412041BF: 74 0F              je          00000001412041D0
  00000001412041C1: BA 0F 00 00 00     mov         edx,0Fh
  00000001412041C6: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412041CB: E8 F0 33 00 00     call        00000001412075C0
  00000001412041D0: 83 7C 24 50 14     cmp         dword ptr [rsp+50h],14h
  00000001412041D5: 74 09              je          00000001412041E0
  00000001412041D7: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001412041DC: 85 C0              test        eax,eax
  00000001412041DE: 74 3A              je          000000014120421A
  00000001412041E0: 4C 8D 0D 61 B6 AE  lea         r9,[0000000142CEF848h]
                    01
  00000001412041E7: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001412041EC: 48 8D 15 7D B6 AE  lea         rdx,[0000000142CEF870h]
                    01
  00000001412041F3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412041F8: FF 15 92 03 C0 00  call        qword ptr [0000000141E04590h]
  00000001412041FE: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141204202: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141204207: 85 C0              test        eax,eax
  0000000141204209: 74 0F              je          000000014120421A
  000000014120420B: BA 14 00 00 00     mov         edx,14h
  0000000141204210: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204215: E8 A6 33 00 00     call        00000001412075C0
  000000014120421A: 83 7C 24 50 15     cmp         dword ptr [rsp+50h],15h
  000000014120421F: 74 09              je          000000014120422A
  0000000141204221: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141204226: 85 C0              test        eax,eax
  0000000141204228: 74 3A              je          0000000141204264
  000000014120422A: 4C 8D 0D 6F B6 AE  lea         r9,[0000000142CEF8A0h]
                    01
  0000000141204231: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141204236: 48 8D 15 8B B6 AE  lea         rdx,[0000000142CEF8C8h]
                    01
  000000014120423D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204242: FF 15 48 03 C0 00  call        qword ptr [0000000141E04590h]
  0000000141204248: 88 44 24 21        mov         byte ptr [rsp+21h],al
  000000014120424C: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141204251: 85 C0              test        eax,eax
  0000000141204253: 74 0F              je          0000000141204264
  0000000141204255: BA 15 00 00 00     mov         edx,15h
  000000014120425A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120425F: E8 5C 33 00 00     call        00000001412075C0
  0000000141204264: 83 7C 24 50 16     cmp         dword ptr [rsp+50h],16h
  0000000141204269: 74 09              je          0000000141204274
  000000014120426B: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141204270: 85 C0              test        eax,eax
  0000000141204272: 74 3A              je          00000001412042AE
  0000000141204274: 4C 8D 0D 7D B6 AE  lea         r9,[0000000142CEF8F8h]
                    01
  000000014120427B: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141204280: 48 8D 15 91 B6 AE  lea         rdx,[0000000142CEF918h]
                    01
  0000000141204287: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120428C: FF 15 FE 02 C0 00  call        qword ptr [0000000141E04590h]
  0000000141204292: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141204296: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  000000014120429B: 85 C0              test        eax,eax
  000000014120429D: 74 0F              je          00000001412042AE
  000000014120429F: BA 16 00 00 00     mov         edx,16h
  00000001412042A4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412042A9: E8 12 33 00 00     call        00000001412075C0
  00000001412042AE: 83 7C 24 50 17     cmp         dword ptr [rsp+50h],17h
  00000001412042B3: 74 09              je          00000001412042BE
  00000001412042B5: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001412042BA: 85 C0              test        eax,eax
  00000001412042BC: 74 3A              je          00000001412042F8
  00000001412042BE: 4C 8D 0D 73 B6 AE  lea         r9,[0000000142CEF938h]
                    01
  00000001412042C5: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001412042CA: 48 8D 15 87 B6 AE  lea         rdx,[0000000142CEF958h]
                    01
  00000001412042D1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412042D6: FF 15 B4 02 C0 00  call        qword ptr [0000000141E04590h]
  00000001412042DC: 88 44 24 21        mov         byte ptr [rsp+21h],al
  00000001412042E0: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  00000001412042E5: 85 C0              test        eax,eax
  00000001412042E7: 74 0F              je          00000001412042F8
  00000001412042E9: BA 17 00 00 00     mov         edx,17h
  00000001412042EE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412042F3: E8 C8 32 00 00     call        00000001412075C0
  00000001412042F8: 83 7C 24 50 18     cmp         dword ptr [rsp+50h],18h
  00000001412042FD: 74 09              je          0000000141204308
  00000001412042FF: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141204304: 85 C0              test        eax,eax
  0000000141204306: 74 3A              je          0000000141204342
  0000000141204308: 4C 8D 0D 69 B6 AE  lea         r9,[0000000142CEF978h]
                    01
  000000014120430F: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141204314: 48 8D 15 7D B6 AE  lea         rdx,[0000000142CEF998h]
                    01
  000000014120431B: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204320: FF 15 6A 02 C0 00  call        qword ptr [0000000141E04590h]
  0000000141204326: 88 44 24 21        mov         byte ptr [rsp+21h],al
  000000014120432A: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  000000014120432F: 85 C0              test        eax,eax
  0000000141204331: 74 0F              je          0000000141204342
  0000000141204333: BA 18 00 00 00     mov         edx,18h
  0000000141204338: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120433D: E8 7E 32 00 00     call        00000001412075C0
  0000000141204342: 83 7C 24 50 19     cmp         dword ptr [rsp+50h],19h
  0000000141204347: 74 09              je          0000000141204352
  0000000141204349: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014120434E: 85 C0              test        eax,eax
  0000000141204350: 74 3A              je          000000014120438C
  0000000141204352: 4C 8D 0D 5F B6 AE  lea         r9,[0000000142CEF9B8h]
                    01
  0000000141204359: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  000000014120435E: 48 8D 15 73 B6 AE  lea         rdx,[0000000142CEF9D8h]
                    01
  0000000141204365: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120436A: FF 15 20 02 C0 00  call        qword ptr [0000000141E04590h]
  0000000141204370: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141204374: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141204379: 85 C0              test        eax,eax
  000000014120437B: 74 0F              je          000000014120438C
  000000014120437D: BA 19 00 00 00     mov         edx,19h
  0000000141204382: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204387: E8 34 32 00 00     call        00000001412075C0
  000000014120438C: 83 7C 24 50 1A     cmp         dword ptr [rsp+50h],1Ah
  0000000141204391: 74 09              je          000000014120439C
  0000000141204393: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141204398: 85 C0              test        eax,eax
  000000014120439A: 74 3A              je          00000001412043D6
  000000014120439C: 4C 8D 0D 55 B6 AE  lea         r9,[0000000142CEF9F8h]
                    01
  00000001412043A3: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001412043A8: 48 8D 15 71 B6 AE  lea         rdx,[0000000142CEFA20h]
                    01
  00000001412043AF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412043B4: FF 15 D6 01 C0 00  call        qword ptr [0000000141E04590h]
  00000001412043BA: 88 44 24 21        mov         byte ptr [rsp+21h],al
  00000001412043BE: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  00000001412043C3: 85 C0              test        eax,eax
  00000001412043C5: 74 0F              je          00000001412043D6
  00000001412043C7: BA 1A 00 00 00     mov         edx,1Ah
  00000001412043CC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412043D1: E8 EA 31 00 00     call        00000001412075C0
  00000001412043D6: 83 7C 24 50 1B     cmp         dword ptr [rsp+50h],1Bh
  00000001412043DB: 74 09              je          00000001412043E6
  00000001412043DD: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001412043E2: 85 C0              test        eax,eax
  00000001412043E4: 74 3A              je          0000000141204420
  00000001412043E6: 4C 8D 0D 5B B6 AE  lea         r9,[0000000142CEFA48h]
                    01
  00000001412043ED: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001412043F2: 48 8D 15 77 B6 AE  lea         rdx,[0000000142CEFA70h]
                    01
  00000001412043F9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412043FE: FF 15 8C 01 C0 00  call        qword ptr [0000000141E04590h]
  0000000141204404: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141204408: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  000000014120440D: 85 C0              test        eax,eax
  000000014120440F: 74 0F              je          0000000141204420
  0000000141204411: BA 1B 00 00 00     mov         edx,1Bh
  0000000141204416: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120441B: E8 A0 31 00 00     call        00000001412075C0
  0000000141204420: 83 7C 24 50 1D     cmp         dword ptr [rsp+50h],1Dh
  0000000141204425: 74 09              je          0000000141204430
  0000000141204427: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014120442C: 85 C0              test        eax,eax
  000000014120442E: 74 3A              je          000000014120446A
  0000000141204430: 4C 8D 0D 61 B6 AE  lea         r9,[0000000142CEFA98h]
                    01
  0000000141204437: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  000000014120443C: 48 8D 15 75 B6 AE  lea         rdx,[0000000142CEFAB8h]
                    01
  0000000141204443: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204448: FF 15 42 01 C0 00  call        qword ptr [0000000141E04590h]
  000000014120444E: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141204452: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141204457: 85 C0              test        eax,eax
  0000000141204459: 74 0F              je          000000014120446A
  000000014120445B: BA 1D 00 00 00     mov         edx,1Dh
  0000000141204460: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204465: E8 56 31 00 00     call        00000001412075C0
  000000014120446A: 83 7C 24 50 1E     cmp         dword ptr [rsp+50h],1Eh
  000000014120446F: 74 09              je          000000014120447A
  0000000141204471: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141204476: 85 C0              test        eax,eax
  0000000141204478: 74 3A              je          00000001412044B4
  000000014120447A: 4C 8D 0D 57 B6 AE  lea         r9,[0000000142CEFAD8h]
                    01
  0000000141204481: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141204486: 48 8D 15 7B B6 AE  lea         rdx,[0000000142CEFB08h]
                    01
  000000014120448D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204492: FF 15 F8 00 C0 00  call        qword ptr [0000000141E04590h]
  0000000141204498: 88 44 24 21        mov         byte ptr [rsp+21h],al
  000000014120449C: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  00000001412044A1: 85 C0              test        eax,eax
  00000001412044A3: 74 0F              je          00000001412044B4
  00000001412044A5: BA 1E 00 00 00     mov         edx,1Eh
  00000001412044AA: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412044AF: E8 0C 31 00 00     call        00000001412075C0
  00000001412044B4: 83 7C 24 50 1F     cmp         dword ptr [rsp+50h],1Fh
  00000001412044B9: 74 09              je          00000001412044C4
  00000001412044BB: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001412044C0: 85 C0              test        eax,eax
  00000001412044C2: 74 3A              je          00000001412044FE
  00000001412044C4: 4C 8D 0D 6D B6 AE  lea         r9,[0000000142CEFB38h]
                    01
  00000001412044CB: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001412044D0: 48 8D 15 91 B6 AE  lea         rdx,[0000000142CEFB68h]
                    01
  00000001412044D7: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412044DC: FF 15 AE 00 C0 00  call        qword ptr [0000000141E04590h]
  00000001412044E2: 88 44 24 21        mov         byte ptr [rsp+21h],al
  00000001412044E6: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  00000001412044EB: 85 C0              test        eax,eax
  00000001412044ED: 74 0F              je          00000001412044FE
  00000001412044EF: BA 1F 00 00 00     mov         edx,1Fh
  00000001412044F4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412044F9: E8 C2 30 00 00     call        00000001412075C0
  00000001412044FE: 83 7C 24 50 20     cmp         dword ptr [rsp+50h],20h
  0000000141204503: 74 09              je          000000014120450E
  0000000141204505: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014120450A: 85 C0              test        eax,eax
  000000014120450C: 74 3A              je          0000000141204548
  000000014120450E: 4C 8D 0D 83 B6 AE  lea         r9,[0000000142CEFB98h]
                    01
  0000000141204515: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  000000014120451A: 48 8D 15 A7 B6 AE  lea         rdx,[0000000142CEFBC8h]
                    01
  0000000141204521: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204526: FF 15 64 00 C0 00  call        qword ptr [0000000141E04590h]
  000000014120452C: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141204530: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141204535: 85 C0              test        eax,eax
  0000000141204537: 74 0F              je          0000000141204548
  0000000141204539: BA 20 00 00 00     mov         edx,20h
  000000014120453E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204543: E8 78 30 00 00     call        00000001412075C0
  0000000141204548: 83 7C 24 50 21     cmp         dword ptr [rsp+50h],21h
  000000014120454D: 74 09              je          0000000141204558
  000000014120454F: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141204554: 85 C0              test        eax,eax
  0000000141204556: 74 3A              je          0000000141204592
  0000000141204558: 4C 8D 0D 99 B6 AE  lea         r9,[0000000142CEFBF8h]
                    01
  000000014120455F: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141204564: 48 8D 15 BD B6 AE  lea         rdx,[0000000142CEFC28h]
                    01
  000000014120456B: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204570: FF 15 1A 00 C0 00  call        qword ptr [0000000141E04590h]
  0000000141204576: 88 44 24 21        mov         byte ptr [rsp+21h],al
  000000014120457A: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  000000014120457F: 85 C0              test        eax,eax
  0000000141204581: 74 0F              je          0000000141204592
  0000000141204583: BA 21 00 00 00     mov         edx,21h
  0000000141204588: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120458D: E8 2E 30 00 00     call        00000001412075C0
  0000000141204592: 83 7C 24 50 22     cmp         dword ptr [rsp+50h],22h
  0000000141204597: 74 09              je          00000001412045A2
  0000000141204599: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014120459E: 85 C0              test        eax,eax
  00000001412045A0: 74 3B              je          00000001412045DD
  00000001412045A2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412045A7: E8 34 02 00 00     call        00000001412047E0
  00000001412045AC: 4C 8D 0D A5 B6 AE  lea         r9,[0000000142CEFC58h]
                    01
  00000001412045B3: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001412045B8: 48 8D 15 C1 B6 AE  lea         rdx,[0000000142CEFC80h]
                    01
  00000001412045BF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412045C4: FF 15 C6 FF BF 00  call        qword ptr [0000000141E04590h]
  00000001412045CA: 88 44 24 21        mov         byte ptr [rsp+21h],al
  00000001412045CE: BA 22 00 00 00     mov         edx,22h
  00000001412045D3: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412045D8: E8 E3 2F 00 00     call        00000001412075C0
  00000001412045DD: 83 7C 24 50 24     cmp         dword ptr [rsp+50h],24h
  00000001412045E2: 74 09              je          00000001412045ED
  00000001412045E4: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001412045E9: 85 C0              test        eax,eax
  00000001412045EB: 74 3A              je          0000000141204627
  00000001412045ED: 4C 8D 0D B4 B6 AE  lea         r9,[0000000142CEFCA8h]
                    01
  00000001412045F4: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001412045F9: 48 8D 15 D0 B6 AE  lea         rdx,[0000000142CEFCD0h]
                    01
  0000000141204600: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204605: FF 15 85 FF BF 00  call        qword ptr [0000000141E04590h]
  000000014120460B: 88 44 24 21        mov         byte ptr [rsp+21h],al
  000000014120460F: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  0000000141204614: 85 C0              test        eax,eax
  0000000141204616: 74 0F              je          0000000141204627
  0000000141204618: BA 24 00 00 00     mov         edx,24h
  000000014120461D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204622: E8 99 2F 00 00     call        00000001412075C0
  0000000141204627: 83 7C 24 50 2A     cmp         dword ptr [rsp+50h],2Ah
  000000014120462C: 74 09              je          0000000141204637
  000000014120462E: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000141204633: 85 C0              test        eax,eax
  0000000141204635: 74 3A              je          0000000141204671
  0000000141204637: 4C 8D 0D BA B6 AE  lea         r9,[0000000142CEFCF8h]
                    01
  000000014120463E: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141204643: 48 8D 15 D6 B6 AE  lea         rdx,[0000000142CEFD20h]
                    01
  000000014120464A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120464F: FF 15 3B FF BF 00  call        qword ptr [0000000141E04590h]
  0000000141204655: 88 44 24 21        mov         byte ptr [rsp+21h],al
  0000000141204659: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  000000014120465E: 85 C0              test        eax,eax
  0000000141204660: 74 0F              je          0000000141204671
  0000000141204662: BA 2A 00 00 00     mov         edx,2Ah
  0000000141204667: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120466C: E8 4F 2F 00 00     call        00000001412075C0
  0000000141204671: 83 7C 24 50 27     cmp         dword ptr [rsp+50h],27h
  0000000141204676: 74 09              je          0000000141204681
  0000000141204678: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014120467D: 85 C0              test        eax,eax
  000000014120467F: 74 3A              je          00000001412046BB
  0000000141204681: 4C 8D 0D C0 B6 AE  lea         r9,[0000000142CEFD48h]
                    01
  0000000141204688: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  000000014120468D: 48 8D 15 E4 B6 AE  lea         rdx,[0000000142CEFD78h]
                    01
  0000000141204694: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204699: FF 15 F1 FE BF 00  call        qword ptr [0000000141E04590h]
  000000014120469F: 88 44 24 21        mov         byte ptr [rsp+21h],al
  00000001412046A3: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  00000001412046A8: 85 C0              test        eax,eax
  00000001412046AA: 74 0F              je          00000001412046BB
  00000001412046AC: BA 27 00 00 00     mov         edx,27h
  00000001412046B1: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412046B6: E8 05 2F 00 00     call        00000001412075C0
  00000001412046BB: 83 7C 24 50 04     cmp         dword ptr [rsp+50h],4
  00000001412046C0: 74 09              je          00000001412046CB
  00000001412046C2: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001412046C7: 85 C0              test        eax,eax
  00000001412046C9: 74 3A              je          0000000141204705
  00000001412046CB: 4C 8D 0D D6 B6 AE  lea         r9,[0000000142CEFDA8h]
                    01
  00000001412046D2: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000001412046D7: 48 8D 15 FA B6 AE  lea         rdx,[0000000142CEFDD8h]
                    01
  00000001412046DE: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001412046E3: FF 15 A7 FE BF 00  call        qword ptr [0000000141E04590h]
  00000001412046E9: 88 44 24 21        mov         byte ptr [rsp+21h],al
  00000001412046ED: 0F B6 44 24 21     movzx       eax,byte ptr [rsp+21h]
  00000001412046F2: 85 C0              test        eax,eax
  00000001412046F4: 74 0F              je          0000000141204705
  00000001412046F6: BA 04 00 00 00     mov         edx,4
  00000001412046FB: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204700: E8 BB 2E 00 00     call        00000001412075C0
  0000000141204705: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014120470A: 85 C0              test        eax,eax
  000000014120470C: 74 5A              je          0000000141204768
  000000014120470E: 4C 8D 0D F3 B6 AE  lea         r9,[0000000142CEFE08h]
                    01
  0000000141204715: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  000000014120471A: 48 8D 15 0F B7 AE  lea         rdx,[0000000142CEFE30h]
                    01
  0000000141204721: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204726: FF 15 64 FE BF 00  call        qword ptr [0000000141E04590h]
  000000014120472C: 4C 8D 0D 25 B7 AE  lea         r9,[0000000142CEFE58h]
                    01
  0000000141204733: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141204738: 48 8D 15 41 B7 AE  lea         rdx,[0000000142CEFE80h]
                    01
  000000014120473F: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204744: FF 15 46 FE BF 00  call        qword ptr [0000000141E04590h]
  000000014120474A: 4C 8D 0D 57 B7 AE  lea         r9,[0000000142CEFEA8h]
                    01
  0000000141204751: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000141204756: 48 8D 15 6B B7 AE  lea         rdx,[0000000142CEFEC8h]
                    01
  000000014120475D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141204762: FF 15 28 FE BF 00  call        qword ptr [0000000141E04590h]
  0000000141204768: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014120476D: E8 8E 76 00 00     call        000000014120BE00
  0000000141204772: 48 83 C4 38        add         rsp,38h
  0000000141204776: C3                 ret

  Summary

     1E02000 .text

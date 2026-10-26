Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CE6740: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CE6745: 56                 push        rsi
  0000000140CE6746: 57                 push        rdi
  0000000140CE6747: 48 81 EC 58 03 00  sub         rsp,358h
                    00
  0000000140CE674E: 48 C7 84 24 40 03  mov         qword ptr [rsp+0000000000000340h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140CE675A: B9 40 00 00 00     mov         ecx,40h
  0000000140CE675F: E8 58 83 78 00     call        000000014146EABC
  0000000140CE6764: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140CE6769: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140CE676F: 74 1C              je          0000000140CE678D
  0000000140CE6771: 48 8B 94 24 70 03  mov         rdx,qword ptr [rsp+0000000000000370h]
                    00 00
  0000000140CE6779: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CE677E: E8 DD 8D 9D 00     call        00000001416BF560
  0000000140CE6783: 48 89 84 24 48 03  mov         qword ptr [rsp+0000000000000348h],rax
                    00 00
  0000000140CE678B: EB 0C              jmp         0000000140CE6799
  0000000140CE678D: 48 C7 84 24 48 03  mov         qword ptr [rsp+0000000000000348h],0
                    00 00 00 00 00 00
  0000000140CE6799: 48 8B 84 24 48 03  mov         rax,qword ptr [rsp+0000000000000348h]
                    00 00
  0000000140CE67A1: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140CE67A6: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140CE67AB: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140CE67B0: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  0000000140CE67B5: 48 8D 15 CC BA D0  lea         rdx,[00000001429F2288h]
                    01
  0000000140CE67BC: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000140CE67C4: E8 B7 E8 33 FF     call        0000000140025080
  0000000140CE67C9: 48 8D BC 24 30 03  lea         rdi,[rsp+0000000000000330h]
                    00 00
  0000000140CE67D1: 48 8B F0           mov         rsi,rax
  0000000140CE67D4: B9 10 00 00 00     mov         ecx,10h
  0000000140CE67D9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE67DB: 48 8D BC 24 08 01  lea         rdi,[rsp+0000000000000108h]
                    00 00
  0000000140CE67E3: 48 8D B4 24 30 03  lea         rsi,[rsp+0000000000000330h]
                    00 00
  0000000140CE67EB: B9 10 00 00 00     mov         ecx,10h
  0000000140CE67F0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE67F2: 45 33 C0           xor         r8d,r8d
  0000000140CE67F5: 33 D2              xor         edx,edx
  0000000140CE67F7: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140CE67FC: FF 15 66 D9 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6802: 48 8D BC 24 70 02  lea         rdi,[rsp+0000000000000270h]
                    00 00
  0000000140CE680A: 48 8B F0           mov         rsi,rax
  0000000140CE680D: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6812: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6814: 45 33 C0           xor         r8d,r8d
  0000000140CE6817: 33 D2              xor         edx,edx
  0000000140CE6819: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140CE6821: FF 15 41 D9 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6827: 48 8D BC 24 60 02  lea         rdi,[rsp+0000000000000260h]
                    00 00
  0000000140CE682F: 48 8B F0           mov         rsi,rax
  0000000140CE6832: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6837: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6839: 45 33 C0           xor         r8d,r8d
  0000000140CE683C: 33 D2              xor         edx,edx
  0000000140CE683E: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140CE6846: FF 15 1C D9 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE684C: 48 8D BC 24 50 02  lea         rdi,[rsp+0000000000000250h]
                    00 00
  0000000140CE6854: 48 8B F0           mov         rsi,rax
  0000000140CE6857: B9 10 00 00 00     mov         ecx,10h
  0000000140CE685C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE685E: 45 33 C0           xor         r8d,r8d
  0000000140CE6861: 33 D2              xor         edx,edx
  0000000140CE6863: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140CE686B: FF 15 F7 D8 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6871: 48 8D BC 24 40 02  lea         rdi,[rsp+0000000000000240h]
                    00 00
  0000000140CE6879: 48 8B F0           mov         rsi,rax
  0000000140CE687C: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6881: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6883: 45 33 C0           xor         r8d,r8d
  0000000140CE6886: 33 D2              xor         edx,edx
  0000000140CE6888: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140CE6890: FF 15 D2 D8 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6896: 48 8D BC 24 30 02  lea         rdi,[rsp+0000000000000230h]
                    00 00
  0000000140CE689E: 48 8B F0           mov         rsi,rax
  0000000140CE68A1: B9 10 00 00 00     mov         ecx,10h
  0000000140CE68A6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE68A8: 45 33 C0           xor         r8d,r8d
  0000000140CE68AB: 33 D2              xor         edx,edx
  0000000140CE68AD: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140CE68B5: FF 15 AD D8 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE68BB: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  0000000140CE68C3: 48 8B F0           mov         rsi,rax
  0000000140CE68C6: B9 10 00 00 00     mov         ecx,10h
  0000000140CE68CB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE68CD: 45 33 C0           xor         r8d,r8d
  0000000140CE68D0: 33 D2              xor         edx,edx
  0000000140CE68D2: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000140CE68DA: FF 15 88 D8 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE68E0: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  0000000140CE68E8: 48 8B F0           mov         rsi,rax
  0000000140CE68EB: B9 10 00 00 00     mov         ecx,10h
  0000000140CE68F0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE68F2: 45 33 C0           xor         r8d,r8d
  0000000140CE68F5: 33 D2              xor         edx,edx
  0000000140CE68F7: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140CE68FF: FF 15 63 D8 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6905: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  0000000140CE690D: 48 8B F0           mov         rsi,rax
  0000000140CE6910: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6915: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6917: 45 33 C0           xor         r8d,r8d
  0000000140CE691A: 33 D2              xor         edx,edx
  0000000140CE691C: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140CE6924: FF 15 3E D8 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE692A: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  0000000140CE6932: 48 8B F0           mov         rsi,rax
  0000000140CE6935: B9 10 00 00 00     mov         ecx,10h
  0000000140CE693A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE693C: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  0000000140CE6944: 48 8D B4 24 08 01  lea         rsi,[rsp+0000000000000108h]
                    00 00
  0000000140CE694C: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6951: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6953: 48 8D 84 24 70 02  lea         rax,[rsp+0000000000000270h]
                    00 00
  0000000140CE695B: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CE6960: 48 8D 84 24 60 02  lea         rax,[rsp+0000000000000260h]
                    00 00
  0000000140CE6968: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140CE696D: 48 8D 84 24 50 02  lea         rax,[rsp+0000000000000250h]
                    00 00
  0000000140CE6975: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CE697A: 48 8D 84 24 40 02  lea         rax,[rsp+0000000000000240h]
                    00 00
  0000000140CE6982: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CE6987: 48 8D 84 24 30 02  lea         rax,[rsp+0000000000000230h]
                    00 00
  0000000140CE698F: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CE6994: 48 8D 84 24 20 02  lea         rax,[rsp+0000000000000220h]
                    00 00
  0000000140CE699C: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CE69A1: 48 8D 84 24 10 02  lea         rax,[rsp+0000000000000210h]
                    00 00
  0000000140CE69A9: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CE69AE: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  0000000140CE69B6: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CE69BB: 4C 8D 8C 24 F0 01  lea         r9,[rsp+00000000000001F0h]
                    00 00
  0000000140CE69C3: 4C 8D 84 24 E0 01  lea         r8,[rsp+00000000000001E0h]
                    00 00
  0000000140CE69CB: 48 8D 15 C6 B8 D0  lea         rdx,[00000001429F2298h]
                    01
  0000000140CE69D2: 48 8D 0D 27 FD 08  lea         rcx,[0000000143D76700h]
                    03
  0000000140CE69D9: E8 E2 89 5E FF     call        00000001402CF3C0
  0000000140CE69DE: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  0000000140CE69E3: 48 8D 15 CE B8 D0  lea         rdx,[00000001429F22B8h]
                    01
  0000000140CE69EA: 48 8D 8C 24 C8 01  lea         rcx,[rsp+00000000000001C8h]
                    00 00
  0000000140CE69F2: E8 89 E6 33 FF     call        0000000140025080
  0000000140CE69F7: 48 8D BC 24 20 03  lea         rdi,[rsp+0000000000000320h]
                    00 00
  0000000140CE69FF: 48 8B F0           mov         rsi,rax
  0000000140CE6A02: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6A07: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6A09: 48 8D BC 24 B8 01  lea         rdi,[rsp+00000000000001B8h]
                    00 00
  0000000140CE6A11: 48 8D B4 24 20 03  lea         rsi,[rsp+0000000000000320h]
                    00 00
  0000000140CE6A19: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6A1E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6A20: 45 33 C0           xor         r8d,r8d
  0000000140CE6A23: 33 D2              xor         edx,edx
  0000000140CE6A25: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  0000000140CE6A2D: FF 15 35 D7 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6A33: 48 8D BC 24 10 03  lea         rdi,[rsp+0000000000000310h]
                    00 00
  0000000140CE6A3B: 48 8B F0           mov         rsi,rax
  0000000140CE6A3E: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6A43: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6A45: 45 33 C0           xor         r8d,r8d
  0000000140CE6A48: 33 D2              xor         edx,edx
  0000000140CE6A4A: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  0000000140CE6A52: FF 15 10 D7 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6A58: 48 8D BC 24 00 03  lea         rdi,[rsp+0000000000000300h]
                    00 00
  0000000140CE6A60: 48 8B F0           mov         rsi,rax
  0000000140CE6A63: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6A68: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6A6A: 45 33 C0           xor         r8d,r8d
  0000000140CE6A6D: 33 D2              xor         edx,edx
  0000000140CE6A6F: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  0000000140CE6A77: FF 15 EB D6 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6A7D: 48 8D BC 24 F0 02  lea         rdi,[rsp+00000000000002F0h]
                    00 00
  0000000140CE6A85: 48 8B F0           mov         rsi,rax
  0000000140CE6A88: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6A8D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6A8F: 45 33 C0           xor         r8d,r8d
  0000000140CE6A92: 33 D2              xor         edx,edx
  0000000140CE6A94: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  0000000140CE6A9C: FF 15 C6 D6 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6AA2: 48 8D BC 24 E0 02  lea         rdi,[rsp+00000000000002E0h]
                    00 00
  0000000140CE6AAA: 48 8B F0           mov         rsi,rax
  0000000140CE6AAD: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6AB2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6AB4: 45 33 C0           xor         r8d,r8d
  0000000140CE6AB7: 33 D2              xor         edx,edx
  0000000140CE6AB9: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  0000000140CE6AC1: FF 15 A1 D6 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6AC7: 48 8D BC 24 D0 02  lea         rdi,[rsp+00000000000002D0h]
                    00 00
  0000000140CE6ACF: 48 8B F0           mov         rsi,rax
  0000000140CE6AD2: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6AD7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6AD9: 45 33 C0           xor         r8d,r8d
  0000000140CE6ADC: 33 D2              xor         edx,edx
  0000000140CE6ADE: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  0000000140CE6AE6: FF 15 7C D6 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6AEC: 48 8D BC 24 C0 02  lea         rdi,[rsp+00000000000002C0h]
                    00 00
  0000000140CE6AF4: 48 8B F0           mov         rsi,rax
  0000000140CE6AF7: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6AFC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6AFE: 45 33 C0           xor         r8d,r8d
  0000000140CE6B01: 33 D2              xor         edx,edx
  0000000140CE6B03: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140CE6B0B: FF 15 57 D6 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6B11: 48 8D BC 24 B0 02  lea         rdi,[rsp+00000000000002B0h]
                    00 00
  0000000140CE6B19: 48 8B F0           mov         rsi,rax
  0000000140CE6B1C: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6B21: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6B23: 45 33 C0           xor         r8d,r8d
  0000000140CE6B26: 33 D2              xor         edx,edx
  0000000140CE6B28: 48 8D 8C 24 98 01  lea         rcx,[rsp+0000000000000198h]
                    00 00
  0000000140CE6B30: FF 15 32 D6 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6B36: 48 8D BC 24 A0 02  lea         rdi,[rsp+00000000000002A0h]
                    00 00
  0000000140CE6B3E: 48 8B F0           mov         rsi,rax
  0000000140CE6B41: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6B46: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6B48: 45 33 C0           xor         r8d,r8d
  0000000140CE6B4B: 33 D2              xor         edx,edx
  0000000140CE6B4D: 48 8D 8C 24 A8 01  lea         rcx,[rsp+00000000000001A8h]
                    00 00
  0000000140CE6B55: FF 15 0D D6 11 01  call        qword ptr [0000000141E04168h]
  0000000140CE6B5B: 48 8D BC 24 90 02  lea         rdi,[rsp+0000000000000290h]
                    00 00
  0000000140CE6B63: 48 8B F0           mov         rsi,rax
  0000000140CE6B66: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6B6B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6B6D: 48 8D BC 24 80 02  lea         rdi,[rsp+0000000000000280h]
                    00 00
  0000000140CE6B75: 48 8D B4 24 B8 01  lea         rsi,[rsp+00000000000001B8h]
                    00 00
  0000000140CE6B7D: B9 10 00 00 00     mov         ecx,10h
  0000000140CE6B82: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE6B84: 48 8D 84 24 10 03  lea         rax,[rsp+0000000000000310h]
                    00 00
  0000000140CE6B8C: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CE6B91: 48 8D 84 24 00 03  lea         rax,[rsp+0000000000000300h]
                    00 00
  0000000140CE6B99: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140CE6B9E: 48 8D 84 24 F0 02  lea         rax,[rsp+00000000000002F0h]
                    00 00
  0000000140CE6BA6: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CE6BAB: 48 8D 84 24 E0 02  lea         rax,[rsp+00000000000002E0h]
                    00 00
  0000000140CE6BB3: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CE6BB8: 48 8D 84 24 D0 02  lea         rax,[rsp+00000000000002D0h]
                    00 00
  0000000140CE6BC0: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CE6BC5: 48 8D 84 24 C0 02  lea         rax,[rsp+00000000000002C0h]
                    00 00
  0000000140CE6BCD: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CE6BD2: 48 8D 84 24 B0 02  lea         rax,[rsp+00000000000002B0h]
                    00 00
  0000000140CE6BDA: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CE6BDF: 48 8D 84 24 A0 02  lea         rax,[rsp+00000000000002A0h]
                    00 00
  0000000140CE6BE7: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CE6BEC: 4C 8D 8C 24 90 02  lea         r9,[rsp+0000000000000290h]
                    00 00
  0000000140CE6BF4: 4C 8D 84 24 80 02  lea         r8,[rsp+0000000000000280h]
                    00 00
  0000000140CE6BFC: 48 8D 15 C5 B6 D0  lea         rdx,[00000001429F22C8h]
                    01
  0000000140CE6C03: 48 8D 0D F6 FA 08  lea         rcx,[0000000143D76700h]
                    03
  0000000140CE6C0A: E8 B1 87 5E FF     call        00000001402CF3C0
  0000000140CE6C0F: 48 81 C4 58 03 00  add         rsp,358h
                    00
  0000000140CE6C16: 5F                 pop         rdi
  0000000140CE6C17: 5E                 pop         rsi
  0000000140CE6C18: C3                 ret

  Summary

     1E02000 .text

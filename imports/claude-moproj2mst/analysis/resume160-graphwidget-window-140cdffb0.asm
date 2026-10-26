Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDFFB0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140CDFFB4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDFFB9: 56                 push        rsi
  0000000140CDFFBA: 57                 push        rdi
  0000000140CDFFBB: 48 81 EC A8 04 00  sub         rsp,4A8h
                    00
  0000000140CDFFC2: C6 44 24 68 01     mov         byte ptr [rsp+68h],1
  0000000140CDFFC7: 48 8B 8C 24 C0 04  mov         rcx,qword ptr [rsp+00000000000004C0h]
                    00 00
  0000000140CDFFCF: 48 83 C1 30        add         rcx,30h
  0000000140CDFFD3: E8 18 AF F3 FF     call        0000000140C1AEF0
  0000000140CDFFD8: 48 85 C0           test        rax,rax
  0000000140CDFFDB: 75 07              jne         0000000140CDFFE4
  0000000140CDFFDD: 32 C0              xor         al,al
  0000000140CDFFDF: E9 DA 07 00 00     jmp         0000000140CE07BE
  0000000140CDFFE4: 83 BC 24 C8 04 00  cmp         dword ptr [rsp+00000000000004C8h],0
                    00 00
  0000000140CDFFEC: 75 07              jne         0000000140CDFFF5
  0000000140CDFFEE: 32 C0              xor         al,al
  0000000140CDFFF0: E9 C9 07 00 00     jmp         0000000140CE07BE
  0000000140CDFFF5: 48 8D 0D BC 66 09  lea         rcx,[0000000143D766B8h]
                    03
  0000000140CDFFFC: E8 2F B8 5E FF     call        00000001402CB830
  0000000140CE0001: 0F B6 C0           movzx       eax,al
  0000000140CE0004: 85 C0              test        eax,eax
  0000000140CE0006: 0F 84 C3 00 00 00  je          0000000140CE00CF
  0000000140CE000C: 48 8D 0D A5 66 09  lea         rcx,[0000000143D766B8h]
                    03
  0000000140CE0013: E8 78 80 5E FF     call        00000001402C8090
  0000000140CE0018: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140CE001D: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140CE0022: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CE0025: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140CE002A: FF 90 78 02 00 00  call        qword ptr [rax+0000000000000278h]
  0000000140CE0030: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140CE0035: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140CE003B: 0F 84 89 00 00 00  je          0000000140CE00CA
  0000000140CE0041: C6 84 24 80 00 00  mov         byte ptr [rsp+0000000000000080h],0
                    00 00
  0000000140CE0049: 44 0F B6 84 24 80  movzx       r8d,byte ptr [rsp+0000000000000080h]
                    00 00 00
  0000000140CE0052: 48 8D 94 24 C8 04  lea         rdx,[rsp+00000000000004C8h]
                    00 00
  0000000140CE005A: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CE005F: E8 0C B8 F9 FF     call        0000000140C7B870
  0000000140CE0064: 88 44 24 68        mov         byte ptr [rsp+68h],al
  0000000140CE0068: 0F B6 44 24 68     movzx       eax,byte ptr [rsp+68h]
  0000000140CE006D: 85 C0              test        eax,eax
  0000000140CE006F: 74 4B              je          0000000140CE00BC
  0000000140CE0071: C6 84 24 90 00 00  mov         byte ptr [rsp+0000000000000090h],1
                    00 01
  0000000140CE0079: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CE007E: E8 8D 01 FB FF     call        0000000140C90210
  0000000140CE0083: 0F B6 94 24 90 00  movzx       edx,byte ptr [rsp+0000000000000090h]
                    00 00
  0000000140CE008B: 48 8B C8           mov         rcx,rax
  0000000140CE008E: E8 4D AA 09 00     call        0000000140D7AAE0
  0000000140CE0093: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140CE009B: 48 83 BC 24 88 00  cmp         qword ptr [rsp+0000000000000088h],0
                    00 00 00
  0000000140CE00A4: 74 14              je          0000000140CE00BA
  0000000140CE00A6: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CE00AE: 48 8D 0D 4B 66 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CE00B5: E8 26 EA 5E FF     call        00000001402CEAE0
  0000000140CE00BA: EB 0E              jmp         0000000140CE00CA
  0000000140CE00BC: 33 D2              xor         edx,edx
  0000000140CE00BE: 48 8D 0D 3B 66 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CE00C5: E8 16 EA 5E FF     call        00000001402CEAE0
  0000000140CE00CA: E9 88 06 00 00     jmp         0000000140CE0757
  0000000140CE00CF: 48 8D 0D E2 65 09  lea         rcx,[0000000143D766B8h]
                    03
  0000000140CE00D6: E8 E5 B6 5E FF     call        00000001402CB7C0
  0000000140CE00DB: 0F B6 C0           movzx       eax,al
  0000000140CE00DE: 85 C0              test        eax,eax
  0000000140CE00E0: 0F 84 71 06 00 00  je          0000000140CE0757
  0000000140CE00E6: 45 33 C0           xor         r8d,r8d
  0000000140CE00E9: 33 D2              xor         edx,edx
  0000000140CE00EB: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140CE00F3: FF 15 6F 40 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE00F9: 48 8D BC 24 30 03  lea         rdi,[rsp+0000000000000330h]
                    00 00
  0000000140CE0101: 48 8B F0           mov         rsi,rax
  0000000140CE0104: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0109: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE010B: 45 33 C0           xor         r8d,r8d
  0000000140CE010E: 33 D2              xor         edx,edx
  0000000140CE0110: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140CE0118: FF 15 4A 40 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE011E: 48 8D BC 24 20 03  lea         rdi,[rsp+0000000000000320h]
                    00 00
  0000000140CE0126: 48 8B F0           mov         rsi,rax
  0000000140CE0129: B9 10 00 00 00     mov         ecx,10h
  0000000140CE012E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0130: 45 33 C0           xor         r8d,r8d
  0000000140CE0133: 33 D2              xor         edx,edx
  0000000140CE0135: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140CE013D: FF 15 25 40 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE0143: 48 8D BC 24 10 03  lea         rdi,[rsp+0000000000000310h]
                    00 00
  0000000140CE014B: 48 8B F0           mov         rsi,rax
  0000000140CE014E: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0153: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0155: 45 33 C0           xor         r8d,r8d
  0000000140CE0158: 33 D2              xor         edx,edx
  0000000140CE015A: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140CE0162: FF 15 00 40 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE0168: 48 8D BC 24 00 03  lea         rdi,[rsp+0000000000000300h]
                    00 00
  0000000140CE0170: 48 8B F0           mov         rsi,rax
  0000000140CE0173: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0178: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE017A: 45 33 C0           xor         r8d,r8d
  0000000140CE017D: 33 D2              xor         edx,edx
  0000000140CE017F: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000140CE0187: FF 15 DB 3F 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE018D: 48 8D BC 24 F0 02  lea         rdi,[rsp+00000000000002F0h]
                    00 00
  0000000140CE0195: 48 8B F0           mov         rsi,rax
  0000000140CE0198: B9 10 00 00 00     mov         ecx,10h
  0000000140CE019D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE019F: 45 33 C0           xor         r8d,r8d
  0000000140CE01A2: 33 D2              xor         edx,edx
  0000000140CE01A4: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140CE01AC: FF 15 B6 3F 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE01B2: 48 8D BC 24 E0 02  lea         rdi,[rsp+00000000000002E0h]
                    00 00
  0000000140CE01BA: 48 8B F0           mov         rsi,rax
  0000000140CE01BD: B9 10 00 00 00     mov         ecx,10h
  0000000140CE01C2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE01C4: 45 33 C0           xor         r8d,r8d
  0000000140CE01C7: 33 D2              xor         edx,edx
  0000000140CE01C9: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140CE01D1: FF 15 91 3F 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE01D7: 48 8D BC 24 D0 02  lea         rdi,[rsp+00000000000002D0h]
                    00 00
  0000000140CE01DF: 48 8B F0           mov         rsi,rax
  0000000140CE01E2: B9 10 00 00 00     mov         ecx,10h
  0000000140CE01E7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE01E9: 45 33 C0           xor         r8d,r8d
  0000000140CE01EC: 33 D2              xor         edx,edx
  0000000140CE01EE: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  0000000140CE01F6: FF 15 6C 3F 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE01FC: 48 8D BC 24 C0 02  lea         rdi,[rsp+00000000000002C0h]
                    00 00
  0000000140CE0204: 48 8B F0           mov         rsi,rax
  0000000140CE0207: B9 10 00 00 00     mov         ecx,10h
  0000000140CE020C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE020E: 45 33 C0           xor         r8d,r8d
  0000000140CE0211: 33 D2              xor         edx,edx
  0000000140CE0213: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000140CE021B: FF 15 47 3F 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE0221: 48 8D BC 24 B0 02  lea         rdi,[rsp+00000000000002B0h]
                    00 00
  0000000140CE0229: 48 8B F0           mov         rsi,rax
  0000000140CE022C: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0231: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0233: 45 33 C0           xor         r8d,r8d
  0000000140CE0236: 33 D2              xor         edx,edx
  0000000140CE0238: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  0000000140CE0240: FF 15 22 3F 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE0246: 48 8D BC 24 A0 02  lea         rdi,[rsp+00000000000002A0h]
                    00 00
  0000000140CE024E: 48 8B F0           mov         rsi,rax
  0000000140CE0251: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0256: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0258: 48 8D 84 24 30 03  lea         rax,[rsp+0000000000000330h]
                    00 00
  0000000140CE0260: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CE0265: 48 8D 84 24 20 03  lea         rax,[rsp+0000000000000320h]
                    00 00
  0000000140CE026D: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140CE0272: 48 8D 84 24 10 03  lea         rax,[rsp+0000000000000310h]
                    00 00
  0000000140CE027A: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CE027F: 48 8D 84 24 00 03  lea         rax,[rsp+0000000000000300h]
                    00 00
  0000000140CE0287: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CE028C: 48 8D 84 24 F0 02  lea         rax,[rsp+00000000000002F0h]
                    00 00
  0000000140CE0294: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CE0299: 48 8D 84 24 E0 02  lea         rax,[rsp+00000000000002E0h]
                    00 00
  0000000140CE02A1: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CE02A6: 48 8D 84 24 D0 02  lea         rax,[rsp+00000000000002D0h]
                    00 00
  0000000140CE02AE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CE02B3: 48 8D 84 24 C0 02  lea         rax,[rsp+00000000000002C0h]
                    00 00
  0000000140CE02BB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CE02C0: 4C 8D 8C 24 B0 02  lea         r9,[rsp+00000000000002B0h]
                    00 00
  0000000140CE02C8: 4C 8D 84 24 A0 02  lea         r8,[rsp+00000000000002A0h]
                    00 00
  0000000140CE02D0: 48 8D 15 E1 DB D0  lea         rdx,[00000001429EDEB8h]
                    01
  0000000140CE02D7: 48 8D 0D 22 64 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CE02DE: E8 DD F0 5E FF     call        00000001402CF3C0
  0000000140CE02E3: 48 C7 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],0
                    00 00 00 00 00 00
  0000000140CE02EF: 4C 8D 84 24 E8 01  lea         r8,[rsp+00000000000001E8h]
                    00 00
  0000000140CE02F7: 48 8D 15 E2 DB D0  lea         rdx,[00000001429EDEE0h]
                    01
  0000000140CE02FE: 48 8D 8C 24 D8 01  lea         rcx,[rsp+00000000000001D8h]
                    00 00
  0000000140CE0306: E8 75 4D 34 FF     call        0000000140025080
  0000000140CE030B: 48 8D BC 24 90 04  lea         rdi,[rsp+0000000000000490h]
                    00 00
  0000000140CE0313: 48 8B F0           mov         rsi,rax
  0000000140CE0316: B9 10 00 00 00     mov         ecx,10h
  0000000140CE031B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE031D: 48 8D BC 24 C8 01  lea         rdi,[rsp+00000000000001C8h]
                    00 00
  0000000140CE0325: 48 8D B4 24 90 04  lea         rsi,[rsp+0000000000000490h]
                    00 00
  0000000140CE032D: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0332: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0334: 45 33 C0           xor         r8d,r8d
  0000000140CE0337: 33 D2              xor         edx,edx
  0000000140CE0339: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  0000000140CE0341: FF 15 21 3E 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE0347: 48 8D BC 24 D0 03  lea         rdi,[rsp+00000000000003D0h]
                    00 00
  0000000140CE034F: 48 8B F0           mov         rsi,rax
  0000000140CE0352: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0357: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0359: 45 33 C0           xor         r8d,r8d
  0000000140CE035C: 33 D2              xor         edx,edx
  0000000140CE035E: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  0000000140CE0366: FF 15 FC 3D 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE036C: 48 8D BC 24 C0 03  lea         rdi,[rsp+00000000000003C0h]
                    00 00
  0000000140CE0374: 48 8B F0           mov         rsi,rax
  0000000140CE0377: B9 10 00 00 00     mov         ecx,10h
  0000000140CE037C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE037E: 45 33 C0           xor         r8d,r8d
  0000000140CE0381: 33 D2              xor         edx,edx
  0000000140CE0383: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  0000000140CE038B: FF 15 D7 3D 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE0391: 48 8D BC 24 B0 03  lea         rdi,[rsp+00000000000003B0h]
                    00 00
  0000000140CE0399: 48 8B F0           mov         rsi,rax
  0000000140CE039C: B9 10 00 00 00     mov         ecx,10h
  0000000140CE03A1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE03A3: 45 33 C0           xor         r8d,r8d
  0000000140CE03A6: 33 D2              xor         edx,edx
  0000000140CE03A8: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  0000000140CE03B0: FF 15 B2 3D 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE03B6: 48 8D BC 24 A0 03  lea         rdi,[rsp+00000000000003A0h]
                    00 00
  0000000140CE03BE: 48 8B F0           mov         rsi,rax
  0000000140CE03C1: B9 10 00 00 00     mov         ecx,10h
  0000000140CE03C6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE03C8: 45 33 C0           xor         r8d,r8d
  0000000140CE03CB: 33 D2              xor         edx,edx
  0000000140CE03CD: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  0000000140CE03D5: FF 15 8D 3D 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE03DB: 48 8D BC 24 90 03  lea         rdi,[rsp+0000000000000390h]
                    00 00
  0000000140CE03E3: 48 8B F0           mov         rsi,rax
  0000000140CE03E6: B9 10 00 00 00     mov         ecx,10h
  0000000140CE03EB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE03ED: 45 33 C0           xor         r8d,r8d
  0000000140CE03F0: 33 D2              xor         edx,edx
  0000000140CE03F2: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140CE03FA: FF 15 68 3D 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE0400: 48 8D BC 24 80 03  lea         rdi,[rsp+0000000000000380h]
                    00 00
  0000000140CE0408: 48 8B F0           mov         rsi,rax
  0000000140CE040B: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0410: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0412: 45 33 C0           xor         r8d,r8d
  0000000140CE0415: 33 D2              xor         edx,edx
  0000000140CE0417: 48 8D 8C 24 98 01  lea         rcx,[rsp+0000000000000198h]
                    00 00
  0000000140CE041F: FF 15 43 3D 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE0425: 48 8D BC 24 70 03  lea         rdi,[rsp+0000000000000370h]
                    00 00
  0000000140CE042D: 48 8B F0           mov         rsi,rax
  0000000140CE0430: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0435: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0437: 45 33 C0           xor         r8d,r8d
  0000000140CE043A: 33 D2              xor         edx,edx
  0000000140CE043C: 48 8D 8C 24 A8 01  lea         rcx,[rsp+00000000000001A8h]
                    00 00
  0000000140CE0444: FF 15 1E 3D 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE044A: 48 8D BC 24 60 03  lea         rdi,[rsp+0000000000000360h]
                    00 00
  0000000140CE0452: 48 8B F0           mov         rsi,rax
  0000000140CE0455: B9 10 00 00 00     mov         ecx,10h
  0000000140CE045A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE045C: 45 33 C0           xor         r8d,r8d
  0000000140CE045F: 33 D2              xor         edx,edx
  0000000140CE0461: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  0000000140CE0469: FF 15 F9 3C 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE046F: 48 8D BC 24 50 03  lea         rdi,[rsp+0000000000000350h]
                    00 00
  0000000140CE0477: 48 8B F0           mov         rsi,rax
  0000000140CE047A: B9 10 00 00 00     mov         ecx,10h
  0000000140CE047F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0481: 48 8D BC 24 40 03  lea         rdi,[rsp+0000000000000340h]
                    00 00
  0000000140CE0489: 48 8D B4 24 C8 01  lea         rsi,[rsp+00000000000001C8h]
                    00 00
  0000000140CE0491: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0496: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0498: 48 8D 84 24 D0 03  lea         rax,[rsp+00000000000003D0h]
                    00 00
  0000000140CE04A0: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CE04A5: 48 8D 84 24 C0 03  lea         rax,[rsp+00000000000003C0h]
                    00 00
  0000000140CE04AD: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140CE04B2: 48 8D 84 24 B0 03  lea         rax,[rsp+00000000000003B0h]
                    00 00
  0000000140CE04BA: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CE04BF: 48 8D 84 24 A0 03  lea         rax,[rsp+00000000000003A0h]
                    00 00
  0000000140CE04C7: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CE04CC: 48 8D 84 24 90 03  lea         rax,[rsp+0000000000000390h]
                    00 00
  0000000140CE04D4: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CE04D9: 48 8D 84 24 80 03  lea         rax,[rsp+0000000000000380h]
                    00 00
  0000000140CE04E1: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CE04E6: 48 8D 84 24 70 03  lea         rax,[rsp+0000000000000370h]
                    00 00
  0000000140CE04EE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CE04F3: 48 8D 84 24 60 03  lea         rax,[rsp+0000000000000360h]
                    00 00
  0000000140CE04FB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CE0500: 4C 8D 8C 24 50 03  lea         r9,[rsp+0000000000000350h]
                    00 00
  0000000140CE0508: 4C 8D 84 24 40 03  lea         r8,[rsp+0000000000000340h]
                    00 00
  0000000140CE0510: 48 8D 15 D9 D9 D0  lea         rdx,[00000001429EDEF0h]
                    01
  0000000140CE0517: 48 8D 0D E2 61 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CE051E: E8 9D EE 5E FF     call        00000001402CF3C0
  0000000140CE0523: 4C 8D 84 24 C8 04  lea         r8,[rsp+00000000000004C8h]
                    00 00
  0000000140CE052B: 48 8D 15 42 A1 D0  lea         rdx,[00000001429EA674h]
                    01
  0000000140CE0532: 48 8D 8C 24 90 02  lea         rcx,[rsp+0000000000000290h]
                    00 00
  0000000140CE053A: E8 41 4B 34 FF     call        0000000140025080
  0000000140CE053F: 48 8D BC 24 80 04  lea         rdi,[rsp+0000000000000480h]
                    00 00
  0000000140CE0547: 48 8B F0           mov         rsi,rax
  0000000140CE054A: B9 10 00 00 00     mov         ecx,10h
  0000000140CE054F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0551: 48 8D BC 24 80 02  lea         rdi,[rsp+0000000000000280h]
                    00 00
  0000000140CE0559: 48 8D B4 24 80 04  lea         rsi,[rsp+0000000000000480h]
                    00 00
  0000000140CE0561: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0566: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0568: 45 33 C0           xor         r8d,r8d
  0000000140CE056B: 33 D2              xor         edx,edx
  0000000140CE056D: 48 8D 8C 24 F0 01  lea         rcx,[rsp+00000000000001F0h]
                    00 00
  0000000140CE0575: FF 15 ED 3B 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE057B: 48 8D BC 24 70 04  lea         rdi,[rsp+0000000000000470h]
                    00 00
  0000000140CE0583: 48 8B F0           mov         rsi,rax
  0000000140CE0586: B9 10 00 00 00     mov         ecx,10h
  0000000140CE058B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE058D: 45 33 C0           xor         r8d,r8d
  0000000140CE0590: 33 D2              xor         edx,edx
  0000000140CE0592: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  0000000140CE059A: FF 15 C8 3B 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE05A0: 48 8D BC 24 60 04  lea         rdi,[rsp+0000000000000460h]
                    00 00
  0000000140CE05A8: 48 8B F0           mov         rsi,rax
  0000000140CE05AB: B9 10 00 00 00     mov         ecx,10h
  0000000140CE05B0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE05B2: 45 33 C0           xor         r8d,r8d
  0000000140CE05B5: 33 D2              xor         edx,edx
  0000000140CE05B7: 48 8D 8C 24 10 02  lea         rcx,[rsp+0000000000000210h]
                    00 00
  0000000140CE05BF: FF 15 A3 3B 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE05C5: 48 8D BC 24 50 04  lea         rdi,[rsp+0000000000000450h]
                    00 00
  0000000140CE05CD: 48 8B F0           mov         rsi,rax
  0000000140CE05D0: B9 10 00 00 00     mov         ecx,10h
  0000000140CE05D5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE05D7: 45 33 C0           xor         r8d,r8d
  0000000140CE05DA: 33 D2              xor         edx,edx
  0000000140CE05DC: 48 8D 8C 24 20 02  lea         rcx,[rsp+0000000000000220h]
                    00 00
  0000000140CE05E4: FF 15 7E 3B 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE05EA: 48 8D BC 24 40 04  lea         rdi,[rsp+0000000000000440h]
                    00 00
  0000000140CE05F2: 48 8B F0           mov         rsi,rax
  0000000140CE05F5: B9 10 00 00 00     mov         ecx,10h
  0000000140CE05FA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE05FC: 45 33 C0           xor         r8d,r8d
  0000000140CE05FF: 33 D2              xor         edx,edx
  0000000140CE0601: 48 8D 8C 24 30 02  lea         rcx,[rsp+0000000000000230h]
                    00 00
  0000000140CE0609: FF 15 59 3B 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE060F: 48 8D BC 24 30 04  lea         rdi,[rsp+0000000000000430h]
                    00 00
  0000000140CE0617: 48 8B F0           mov         rsi,rax
  0000000140CE061A: B9 10 00 00 00     mov         ecx,10h
  0000000140CE061F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0621: 45 33 C0           xor         r8d,r8d
  0000000140CE0624: 33 D2              xor         edx,edx
  0000000140CE0626: 48 8D 8C 24 40 02  lea         rcx,[rsp+0000000000000240h]
                    00 00
  0000000140CE062E: FF 15 34 3B 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE0634: 48 8D BC 24 20 04  lea         rdi,[rsp+0000000000000420h]
                    00 00
  0000000140CE063C: 48 8B F0           mov         rsi,rax
  0000000140CE063F: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0644: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0646: 45 33 C0           xor         r8d,r8d
  0000000140CE0649: 33 D2              xor         edx,edx
  0000000140CE064B: 48 8D 8C 24 50 02  lea         rcx,[rsp+0000000000000250h]
                    00 00
  0000000140CE0653: FF 15 0F 3B 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE0659: 48 8D BC 24 10 04  lea         rdi,[rsp+0000000000000410h]
                    00 00
  0000000140CE0661: 48 8B F0           mov         rsi,rax
  0000000140CE0664: B9 10 00 00 00     mov         ecx,10h
  0000000140CE0669: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE066B: 45 33 C0           xor         r8d,r8d
  0000000140CE066E: 33 D2              xor         edx,edx
  0000000140CE0670: 48 8D 8C 24 60 02  lea         rcx,[rsp+0000000000000260h]
                    00 00
  0000000140CE0678: FF 15 EA 3A 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE067E: 48 8D BC 24 00 04  lea         rdi,[rsp+0000000000000400h]
                    00 00
  0000000140CE0686: 48 8B F0           mov         rsi,rax
  0000000140CE0689: B9 10 00 00 00     mov         ecx,10h
  0000000140CE068E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE0690: 45 33 C0           xor         r8d,r8d
  0000000140CE0693: 33 D2              xor         edx,edx
  0000000140CE0695: 48 8D 8C 24 70 02  lea         rcx,[rsp+0000000000000270h]
                    00 00
  0000000140CE069D: FF 15 C5 3A 12 01  call        qword ptr [0000000141E04168h]
  0000000140CE06A3: 48 8D BC 24 F0 03  lea         rdi,[rsp+00000000000003F0h]
                    00 00
  0000000140CE06AB: 48 8B F0           mov         rsi,rax
  0000000140CE06AE: B9 10 00 00 00     mov         ecx,10h
  0000000140CE06B3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE06B5: 48 8D BC 24 E0 03  lea         rdi,[rsp+00000000000003E0h]
                    00 00
  0000000140CE06BD: 48 8D B4 24 80 02  lea         rsi,[rsp+0000000000000280h]
                    00 00
  0000000140CE06C5: B9 10 00 00 00     mov         ecx,10h
  0000000140CE06CA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CE06CC: 48 8D 84 24 70 04  lea         rax,[rsp+0000000000000470h]
                    00 00
  0000000140CE06D4: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CE06D9: 48 8D 84 24 60 04  lea         rax,[rsp+0000000000000460h]
                    00 00
  0000000140CE06E1: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140CE06E6: 48 8D 84 24 50 04  lea         rax,[rsp+0000000000000450h]
                    00 00
  0000000140CE06EE: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CE06F3: 48 8D 84 24 40 04  lea         rax,[rsp+0000000000000440h]
                    00 00
  0000000140CE06FB: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CE0700: 48 8D 84 24 30 04  lea         rax,[rsp+0000000000000430h]
                    00 00
  0000000140CE0708: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CE070D: 48 8D 84 24 20 04  lea         rax,[rsp+0000000000000420h]
                    00 00
  0000000140CE0715: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CE071A: 48 8D 84 24 10 04  lea         rax,[rsp+0000000000000410h]
                    00 00
  0000000140CE0722: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CE0727: 48 8D 84 24 00 04  lea         rax,[rsp+0000000000000400h]
                    00 00
  0000000140CE072F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CE0734: 4C 8D 8C 24 F0 03  lea         r9,[rsp+00000000000003F0h]
                    00 00
  0000000140CE073C: 4C 8D 84 24 E0 03  lea         r8,[rsp+00000000000003E0h]
                    00 00
  0000000140CE0744: 48 8D 15 C5 D7 D0  lea         rdx,[00000001429EDF10h]
                    01
  0000000140CE074B: 48 8D 0D AE 5F 09  lea         rcx,[0000000143D76700h]
                    03
  0000000140CE0752: E8 69 EC 5E FF     call        00000001402CF3C0
  0000000140CE0757: 48 8B 8C 24 C0 04  mov         rcx,qword ptr [rsp+00000000000004C0h]
                    00 00
  0000000140CE075F: 48 83 C1 30        add         rcx,30h
  0000000140CE0763: E8 88 A7 F3 FF     call        0000000140C1AEF0
  0000000140CE0768: 48 8B C8           mov         rcx,rax
  0000000140CE076B: E8 F0 CF FF FF     call        0000000140CDD760
  0000000140CE0770: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140CE0775: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140CE077B: 74 11              je          0000000140CE078E
  0000000140CE077D: 8B 94 24 C8 04 00  mov         edx,dword ptr [rsp+00000000000004C8h]
                    00
  0000000140CE0784: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140CE0789: E8 12 FC 07 00     call        0000000140D603A0
  0000000140CE078E: 48 8B 8C 24 C0 04  mov         rcx,qword ptr [rsp+00000000000004C0h]
                    00 00
  0000000140CE0796: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140CE079A: 8B 94 24 C8 04 00  mov         edx,dword ptr [rsp+00000000000004C8h]
                    00
  0000000140CE07A1: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140CE07A5: E8 66 B0 0D 00     call        0000000140DBB810
  0000000140CE07AA: 88 44 24 68        mov         byte ptr [rsp+68h],al
  0000000140CE07AE: 48 8D 0D 03 5F 09  lea         rcx,[0000000143D766B8h]
                    03
  0000000140CE07B5: E8 D6 A0 5E FF     call        00000001402CA890
  0000000140CE07BA: 8A 44 24 68        mov         al,byte ptr [rsp+68h]
  0000000140CE07BE: 48 81 C4 A8 04 00  add         rsp,4A8h
                    00
  0000000140CE07C5: 5F                 pop         rdi
  0000000140CE07C6: 5E                 pop         rsi
  0000000140CE07C7: C3                 ret

  Summary

     1E02000 .text

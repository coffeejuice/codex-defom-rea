Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140015BF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140015BF5: 56                 push        rsi
  0000000140015BF6: 57                 push        rdi
  0000000140015BF7: 48 81 EC 88 0C 00  sub         rsp,0C88h
                    00
  0000000140015BFE: 48 C7 84 24 70 0C  mov         qword ptr [rsp+0000000000000C70h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140015C0A: 48 8B 8C 24 A0 0C  mov         rcx,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140015C12: E8 F9 78 00 00     call        000000014001D510
  0000000140015C17: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140015C1C: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140015C22: 75 05              jne         0000000140015C29
  0000000140015C24: E9 2E 16 00 00     jmp         0000000140017257
  0000000140015C29: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140015C2E: E8 5D 17 09 00     call        00000001400A7390
  0000000140015C33: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140015C38: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140015C3E: 75 05              jne         0000000140015C45
  0000000140015C40: E9 12 16 00 00     jmp         0000000140017257
  0000000140015C45: 48 8B 8C 24 A0 0C  mov         rcx,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140015C4D: E8 7E 7B 00 00     call        000000014001D7D0
  0000000140015C52: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140015C57: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  0000000140015C5D: 75 05              jne         0000000140015C64
  0000000140015C5F: E9 F3 15 00 00     jmp         0000000140017257
  0000000140015C64: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140015C69: E8 C2 01 0A 00     call        00000001400B5E30
  0000000140015C6E: 8B D0              mov         edx,eax
  0000000140015C70: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140015C75: E8 76 1E 03 00     call        0000000140047AF0
  0000000140015C7A: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140015C7F: E8 AC A6 FF FF     call        0000000140010330
  0000000140015C84: 0F B6 C0           movzx       eax,al
  0000000140015C87: 85 C0              test        eax,eax
  0000000140015C89: 75 15              jne         0000000140015CA0
  0000000140015C8B: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140015C90: E8 CB A6 FF FF     call        0000000140010360
  0000000140015C95: 0F B6 C0           movzx       eax,al
  0000000140015C98: 85 C0              test        eax,eax
  0000000140015C9A: 0F 84 23 0A 00 00  je          00000001400166C3
  0000000140015CA0: 45 33 C0           xor         r8d,r8d
  0000000140015CA3: 33 D2              xor         edx,edx
  0000000140015CA5: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140015CAD: FF 15 B5 E4 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015CB3: 48 8D BC 24 30 07  lea         rdi,[rsp+0000000000000730h]
                    00 00
  0000000140015CBB: 48 8B F0           mov         rsi,rax
  0000000140015CBE: B9 10 00 00 00     mov         ecx,10h
  0000000140015CC3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015CC5: 45 33 C0           xor         r8d,r8d
  0000000140015CC8: 33 D2              xor         edx,edx
  0000000140015CCA: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140015CD2: FF 15 90 E4 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015CD8: 48 8D BC 24 20 07  lea         rdi,[rsp+0000000000000720h]
                    00 00
  0000000140015CE0: 48 8B F0           mov         rsi,rax
  0000000140015CE3: B9 10 00 00 00     mov         ecx,10h
  0000000140015CE8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015CEA: 45 33 C0           xor         r8d,r8d
  0000000140015CED: 33 D2              xor         edx,edx
  0000000140015CEF: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140015CF7: FF 15 6B E4 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015CFD: 48 8D BC 24 10 07  lea         rdi,[rsp+0000000000000710h]
                    00 00
  0000000140015D05: 48 8B F0           mov         rsi,rax
  0000000140015D08: B9 10 00 00 00     mov         ecx,10h
  0000000140015D0D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015D0F: 45 33 C0           xor         r8d,r8d
  0000000140015D12: 33 D2              xor         edx,edx
  0000000140015D14: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140015D1C: FF 15 46 E4 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015D22: 48 8D BC 24 00 07  lea         rdi,[rsp+0000000000000700h]
                    00 00
  0000000140015D2A: 48 8B F0           mov         rsi,rax
  0000000140015D2D: B9 10 00 00 00     mov         ecx,10h
  0000000140015D32: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015D34: 45 33 C0           xor         r8d,r8d
  0000000140015D37: 33 D2              xor         edx,edx
  0000000140015D39: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000140015D41: FF 15 21 E4 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015D47: 48 8D BC 24 F0 06  lea         rdi,[rsp+00000000000006F0h]
                    00 00
  0000000140015D4F: 48 8B F0           mov         rsi,rax
  0000000140015D52: B9 10 00 00 00     mov         ecx,10h
  0000000140015D57: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015D59: 45 33 C0           xor         r8d,r8d
  0000000140015D5C: 33 D2              xor         edx,edx
  0000000140015D5E: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140015D66: FF 15 FC E3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015D6C: 48 8D BC 24 E0 06  lea         rdi,[rsp+00000000000006E0h]
                    00 00
  0000000140015D74: 48 8B F0           mov         rsi,rax
  0000000140015D77: B9 10 00 00 00     mov         ecx,10h
  0000000140015D7C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015D7E: 45 33 C0           xor         r8d,r8d
  0000000140015D81: 33 D2              xor         edx,edx
  0000000140015D83: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000140015D8B: FF 15 D7 E3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015D91: 48 8D BC 24 D0 06  lea         rdi,[rsp+00000000000006D0h]
                    00 00
  0000000140015D99: 48 8B F0           mov         rsi,rax
  0000000140015D9C: B9 10 00 00 00     mov         ecx,10h
  0000000140015DA1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015DA3: 45 33 C0           xor         r8d,r8d
  0000000140015DA6: 33 D2              xor         edx,edx
  0000000140015DA8: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  0000000140015DB0: FF 15 B2 E3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015DB6: 48 8D BC 24 C0 06  lea         rdi,[rsp+00000000000006C0h]
                    00 00
  0000000140015DBE: 48 8B F0           mov         rsi,rax
  0000000140015DC1: B9 10 00 00 00     mov         ecx,10h
  0000000140015DC6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015DC8: 45 33 C0           xor         r8d,r8d
  0000000140015DCB: 33 D2              xor         edx,edx
  0000000140015DCD: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  0000000140015DD5: FF 15 8D E3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015DDB: 48 8D BC 24 B0 06  lea         rdi,[rsp+00000000000006B0h]
                    00 00
  0000000140015DE3: 48 8B F0           mov         rsi,rax
  0000000140015DE6: B9 10 00 00 00     mov         ecx,10h
  0000000140015DEB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015DED: 45 33 C0           xor         r8d,r8d
  0000000140015DF0: 33 D2              xor         edx,edx
  0000000140015DF2: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000140015DFA: FF 15 68 E3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015E00: 48 8D BC 24 A0 06  lea         rdi,[rsp+00000000000006A0h]
                    00 00
  0000000140015E08: 48 8B F0           mov         rsi,rax
  0000000140015E0B: B9 10 00 00 00     mov         ecx,10h
  0000000140015E10: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015E12: 48 8D 84 24 30 07  lea         rax,[rsp+0000000000000730h]
                    00 00
  0000000140015E1A: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140015E1F: 48 8D 84 24 20 07  lea         rax,[rsp+0000000000000720h]
                    00 00
  0000000140015E27: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140015E2C: 48 8D 84 24 10 07  lea         rax,[rsp+0000000000000710h]
                    00 00
  0000000140015E34: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140015E39: 48 8D 84 24 00 07  lea         rax,[rsp+0000000000000700h]
                    00 00
  0000000140015E41: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140015E46: 48 8D 84 24 F0 06  lea         rax,[rsp+00000000000006F0h]
                    00 00
  0000000140015E4E: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140015E53: 48 8D 84 24 E0 06  lea         rax,[rsp+00000000000006E0h]
                    00 00
  0000000140015E5B: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140015E60: 48 8D 84 24 D0 06  lea         rax,[rsp+00000000000006D0h]
                    00 00
  0000000140015E68: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140015E6D: 48 8D 84 24 C0 06  lea         rax,[rsp+00000000000006C0h]
                    00 00
  0000000140015E75: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140015E7A: 4C 8D 8C 24 B0 06  lea         r9,[rsp+00000000000006B0h]
                    00 00
  0000000140015E82: 4C 8D 84 24 A0 06  lea         r8,[rsp+00000000000006A0h]
                    00 00
  0000000140015E8A: 48 8D 15 CF 3E 0E  lea         rdx,[00000001420F9D60h]
                    02
  0000000140015E91: 48 8D 0D 68 08 D6  lea         rcx,[0000000143D76700h]
                    03
  0000000140015E98: E8 23 95 2B 00     call        00000001402CF3C0
  0000000140015E9D: 45 33 C0           xor         r8d,r8d
  0000000140015EA0: 33 D2              xor         edx,edx
  0000000140015EA2: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  0000000140015EAA: FF 15 B8 E2 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015EB0: 48 8D BC 24 D0 07  lea         rdi,[rsp+00000000000007D0h]
                    00 00
  0000000140015EB8: 48 8B F0           mov         rsi,rax
  0000000140015EBB: B9 10 00 00 00     mov         ecx,10h
  0000000140015EC0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015EC2: 45 33 C0           xor         r8d,r8d
  0000000140015EC5: 33 D2              xor         edx,edx
  0000000140015EC7: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  0000000140015ECF: FF 15 93 E2 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015ED5: 48 8D BC 24 C0 07  lea         rdi,[rsp+00000000000007C0h]
                    00 00
  0000000140015EDD: 48 8B F0           mov         rsi,rax
  0000000140015EE0: B9 10 00 00 00     mov         ecx,10h
  0000000140015EE5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015EE7: 45 33 C0           xor         r8d,r8d
  0000000140015EEA: 33 D2              xor         edx,edx
  0000000140015EEC: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  0000000140015EF4: FF 15 6E E2 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015EFA: 48 8D BC 24 B0 07  lea         rdi,[rsp+00000000000007B0h]
                    00 00
  0000000140015F02: 48 8B F0           mov         rsi,rax
  0000000140015F05: B9 10 00 00 00     mov         ecx,10h
  0000000140015F0A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015F0C: 45 33 C0           xor         r8d,r8d
  0000000140015F0F: 33 D2              xor         edx,edx
  0000000140015F11: 48 8D 8C 24 80 01  lea         rcx,[rsp+0000000000000180h]
                    00 00
  0000000140015F19: FF 15 49 E2 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015F1F: 48 8D BC 24 A0 07  lea         rdi,[rsp+00000000000007A0h]
                    00 00
  0000000140015F27: 48 8B F0           mov         rsi,rax
  0000000140015F2A: B9 10 00 00 00     mov         ecx,10h
  0000000140015F2F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015F31: 45 33 C0           xor         r8d,r8d
  0000000140015F34: 33 D2              xor         edx,edx
  0000000140015F36: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  0000000140015F3E: FF 15 24 E2 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015F44: 48 8D BC 24 90 07  lea         rdi,[rsp+0000000000000790h]
                    00 00
  0000000140015F4C: 48 8B F0           mov         rsi,rax
  0000000140015F4F: B9 10 00 00 00     mov         ecx,10h
  0000000140015F54: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015F56: 45 33 C0           xor         r8d,r8d
  0000000140015F59: 33 D2              xor         edx,edx
  0000000140015F5B: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  0000000140015F63: FF 15 FF E1 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015F69: 48 8D BC 24 80 07  lea         rdi,[rsp+0000000000000780h]
                    00 00
  0000000140015F71: 48 8B F0           mov         rsi,rax
  0000000140015F74: B9 10 00 00 00     mov         ecx,10h
  0000000140015F79: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015F7B: 45 33 C0           xor         r8d,r8d
  0000000140015F7E: 33 D2              xor         edx,edx
  0000000140015F80: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  0000000140015F88: FF 15 DA E1 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015F8E: 48 8D BC 24 70 07  lea         rdi,[rsp+0000000000000770h]
                    00 00
  0000000140015F96: 48 8B F0           mov         rsi,rax
  0000000140015F99: B9 10 00 00 00     mov         ecx,10h
  0000000140015F9E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015FA0: 45 33 C0           xor         r8d,r8d
  0000000140015FA3: 33 D2              xor         edx,edx
  0000000140015FA5: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  0000000140015FAD: FF 15 B5 E1 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015FB3: 48 8D BC 24 60 07  lea         rdi,[rsp+0000000000000760h]
                    00 00
  0000000140015FBB: 48 8B F0           mov         rsi,rax
  0000000140015FBE: B9 10 00 00 00     mov         ecx,10h
  0000000140015FC3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015FC5: 45 33 C0           xor         r8d,r8d
  0000000140015FC8: 33 D2              xor         edx,edx
  0000000140015FCA: 48 8D 8C 24 D0 01  lea         rcx,[rsp+00000000000001D0h]
                    00 00
  0000000140015FD2: FF 15 90 E1 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015FD8: 48 8D BC 24 50 07  lea         rdi,[rsp+0000000000000750h]
                    00 00
  0000000140015FE0: 48 8B F0           mov         rsi,rax
  0000000140015FE3: B9 10 00 00 00     mov         ecx,10h
  0000000140015FE8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140015FEA: 45 33 C0           xor         r8d,r8d
  0000000140015FED: 33 D2              xor         edx,edx
  0000000140015FEF: 48 8D 8C 24 E0 01  lea         rcx,[rsp+00000000000001E0h]
                    00 00
  0000000140015FF7: FF 15 6B E1 DE 01  call        qword ptr [0000000141E04168h]
  0000000140015FFD: 48 8D BC 24 40 07  lea         rdi,[rsp+0000000000000740h]
                    00 00
  0000000140016005: 48 8B F0           mov         rsi,rax
  0000000140016008: B9 10 00 00 00     mov         ecx,10h
  000000014001600D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001600F: 48 8D 84 24 D0 07  lea         rax,[rsp+00000000000007D0h]
                    00 00
  0000000140016017: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014001601C: 48 8D 84 24 C0 07  lea         rax,[rsp+00000000000007C0h]
                    00 00
  0000000140016024: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140016029: 48 8D 84 24 B0 07  lea         rax,[rsp+00000000000007B0h]
                    00 00
  0000000140016031: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140016036: 48 8D 84 24 A0 07  lea         rax,[rsp+00000000000007A0h]
                    00 00
  000000014001603E: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140016043: 48 8D 84 24 90 07  lea         rax,[rsp+0000000000000790h]
                    00 00
  000000014001604B: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140016050: 48 8D 84 24 80 07  lea         rax,[rsp+0000000000000780h]
                    00 00
  0000000140016058: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014001605D: 48 8D 84 24 70 07  lea         rax,[rsp+0000000000000770h]
                    00 00
  0000000140016065: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014001606A: 48 8D 84 24 60 07  lea         rax,[rsp+0000000000000760h]
                    00 00
  0000000140016072: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140016077: 4C 8D 8C 24 50 07  lea         r9,[rsp+0000000000000750h]
                    00 00
  000000014001607F: 4C 8D 84 24 40 07  lea         r8,[rsp+0000000000000740h]
                    00 00
  0000000140016087: 48 8D 15 F2 3C 0E  lea         rdx,[00000001420F9D80h]
                    02
  000000014001608E: 48 8D 0D 6B 06 D6  lea         rcx,[0000000143D76700h]
                    03
  0000000140016095: E8 26 93 2B 00     call        00000001402CF3C0
  000000014001609A: 45 33 C0           xor         r8d,r8d
  000000014001609D: 33 D2              xor         edx,edx
  000000014001609F: 48 8D 8C 24 F0 01  lea         rcx,[rsp+00000000000001F0h]
                    00 00
  00000001400160A7: FF 15 BB E0 DE 01  call        qword ptr [0000000141E04168h]
  00000001400160AD: 48 8D BC 24 70 08  lea         rdi,[rsp+0000000000000870h]
                    00 00
  00000001400160B5: 48 8B F0           mov         rsi,rax
  00000001400160B8: B9 10 00 00 00     mov         ecx,10h
  00000001400160BD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400160BF: 45 33 C0           xor         r8d,r8d
  00000001400160C2: 33 D2              xor         edx,edx
  00000001400160C4: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  00000001400160CC: FF 15 96 E0 DE 01  call        qword ptr [0000000141E04168h]
  00000001400160D2: 48 8D BC 24 60 08  lea         rdi,[rsp+0000000000000860h]
                    00 00
  00000001400160DA: 48 8B F0           mov         rsi,rax
  00000001400160DD: B9 10 00 00 00     mov         ecx,10h
  00000001400160E2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400160E4: 45 33 C0           xor         r8d,r8d
  00000001400160E7: 33 D2              xor         edx,edx
  00000001400160E9: 48 8D 8C 24 10 02  lea         rcx,[rsp+0000000000000210h]
                    00 00
  00000001400160F1: FF 15 71 E0 DE 01  call        qword ptr [0000000141E04168h]
  00000001400160F7: 48 8D BC 24 50 08  lea         rdi,[rsp+0000000000000850h]
                    00 00
  00000001400160FF: 48 8B F0           mov         rsi,rax
  0000000140016102: B9 10 00 00 00     mov         ecx,10h
  0000000140016107: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016109: 45 33 C0           xor         r8d,r8d
  000000014001610C: 33 D2              xor         edx,edx
  000000014001610E: 48 8D 8C 24 20 02  lea         rcx,[rsp+0000000000000220h]
                    00 00
  0000000140016116: FF 15 4C E0 DE 01  call        qword ptr [0000000141E04168h]
  000000014001611C: 48 8D BC 24 40 08  lea         rdi,[rsp+0000000000000840h]
                    00 00
  0000000140016124: 48 8B F0           mov         rsi,rax
  0000000140016127: B9 10 00 00 00     mov         ecx,10h
  000000014001612C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001612E: 45 33 C0           xor         r8d,r8d
  0000000140016131: 33 D2              xor         edx,edx
  0000000140016133: 48 8D 8C 24 30 02  lea         rcx,[rsp+0000000000000230h]
                    00 00
  000000014001613B: FF 15 27 E0 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016141: 48 8D BC 24 30 08  lea         rdi,[rsp+0000000000000830h]
                    00 00
  0000000140016149: 48 8B F0           mov         rsi,rax
  000000014001614C: B9 10 00 00 00     mov         ecx,10h
  0000000140016151: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016153: 45 33 C0           xor         r8d,r8d
  0000000140016156: 33 D2              xor         edx,edx
  0000000140016158: 48 8D 8C 24 40 02  lea         rcx,[rsp+0000000000000240h]
                    00 00
  0000000140016160: FF 15 02 E0 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016166: 48 8D BC 24 20 08  lea         rdi,[rsp+0000000000000820h]
                    00 00
  000000014001616E: 48 8B F0           mov         rsi,rax
  0000000140016171: B9 10 00 00 00     mov         ecx,10h
  0000000140016176: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016178: 45 33 C0           xor         r8d,r8d
  000000014001617B: 33 D2              xor         edx,edx
  000000014001617D: 48 8D 8C 24 50 02  lea         rcx,[rsp+0000000000000250h]
                    00 00
  0000000140016185: FF 15 DD DF DE 01  call        qword ptr [0000000141E04168h]
  000000014001618B: 48 8D BC 24 10 08  lea         rdi,[rsp+0000000000000810h]
                    00 00
  0000000140016193: 48 8B F0           mov         rsi,rax
  0000000140016196: B9 10 00 00 00     mov         ecx,10h
  000000014001619B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001619D: 45 33 C0           xor         r8d,r8d
  00000001400161A0: 33 D2              xor         edx,edx
  00000001400161A2: 48 8D 8C 24 60 02  lea         rcx,[rsp+0000000000000260h]
                    00 00
  00000001400161AA: FF 15 B8 DF DE 01  call        qword ptr [0000000141E04168h]
  00000001400161B0: 48 8D BC 24 00 08  lea         rdi,[rsp+0000000000000800h]
                    00 00
  00000001400161B8: 48 8B F0           mov         rsi,rax
  00000001400161BB: B9 10 00 00 00     mov         ecx,10h
  00000001400161C0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400161C2: 45 33 C0           xor         r8d,r8d
  00000001400161C5: 33 D2              xor         edx,edx
  00000001400161C7: 48 8D 8C 24 70 02  lea         rcx,[rsp+0000000000000270h]
                    00 00
  00000001400161CF: FF 15 93 DF DE 01  call        qword ptr [0000000141E04168h]
  00000001400161D5: 48 8D BC 24 F0 07  lea         rdi,[rsp+00000000000007F0h]
                    00 00
  00000001400161DD: 48 8B F0           mov         rsi,rax
  00000001400161E0: B9 10 00 00 00     mov         ecx,10h
  00000001400161E5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400161E7: 45 33 C0           xor         r8d,r8d
  00000001400161EA: 33 D2              xor         edx,edx
  00000001400161EC: 48 8D 8C 24 80 02  lea         rcx,[rsp+0000000000000280h]
                    00 00
  00000001400161F4: FF 15 6E DF DE 01  call        qword ptr [0000000141E04168h]
  00000001400161FA: 48 8D BC 24 E0 07  lea         rdi,[rsp+00000000000007E0h]
                    00 00
  0000000140016202: 48 8B F0           mov         rsi,rax
  0000000140016205: B9 10 00 00 00     mov         ecx,10h
  000000014001620A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001620C: 48 8D 84 24 70 08  lea         rax,[rsp+0000000000000870h]
                    00 00
  0000000140016214: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140016219: 48 8D 84 24 60 08  lea         rax,[rsp+0000000000000860h]
                    00 00
  0000000140016221: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140016226: 48 8D 84 24 50 08  lea         rax,[rsp+0000000000000850h]
                    00 00
  000000014001622E: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140016233: 48 8D 84 24 40 08  lea         rax,[rsp+0000000000000840h]
                    00 00
  000000014001623B: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140016240: 48 8D 84 24 30 08  lea         rax,[rsp+0000000000000830h]
                    00 00
  0000000140016248: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014001624D: 48 8D 84 24 20 08  lea         rax,[rsp+0000000000000820h]
                    00 00
  0000000140016255: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014001625A: 48 8D 84 24 10 08  lea         rax,[rsp+0000000000000810h]
                    00 00
  0000000140016262: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140016267: 48 8D 84 24 00 08  lea         rax,[rsp+0000000000000800h]
                    00 00
  000000014001626F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140016274: 4C 8D 8C 24 F0 07  lea         r9,[rsp+00000000000007F0h]
                    00 00
  000000014001627C: 4C 8D 84 24 E0 07  lea         r8,[rsp+00000000000007E0h]
                    00 00
  0000000140016284: 48 8D 15 15 3B 0E  lea         rdx,[00000001420F9DA0h]
                    02
  000000014001628B: 48 8D 0D 6E 04 D6  lea         rcx,[0000000143D76700h]
                    03
  0000000140016292: E8 29 91 2B 00     call        00000001402CF3C0
  0000000140016297: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014001629C: E8 EF F7 09 00     call        00000001400B5A90
  00000001400162A1: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400162A9: 48 83 BC 24 80 00  cmp         qword ptr [rsp+0000000000000080h],0
                    00 00 00
  00000001400162B2: 0F 84 06 04 00 00  je          00000001400166BE
  00000001400162B8: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400162C0: E8 1B 97 17 00     call        000000014018F9E0
  00000001400162C5: 90                 nop
  00000001400162C6: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400162CE: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400162D1: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400162D9: FF 90 78 01 00 00  call        qword ptr [rax+0000000000000178h]
  00000001400162DF: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  00000001400162E6: C7 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00
  00000001400162F1: EB 11              jmp         0000000140016304
  00000001400162F3: 8B 84 24 88 00 00  mov         eax,dword ptr [rsp+0000000000000088h]
                    00
  00000001400162FA: 83 C0 01           add         eax,1
  00000001400162FD: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  0000000140016304: 8B 84 24 98 00 00  mov         eax,dword ptr [rsp+0000000000000098h]
                    00
  000000014001630B: 39 84 24 88 00 00  cmp         dword ptr [rsp+0000000000000088h],eax
                    00
  0000000140016312: 0F 8D DA 00 00 00  jge         00000001400163F2
  0000000140016318: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140016320: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140016323: 8B 94 24 88 00 00  mov         edx,dword ptr [rsp+0000000000000088h]
                    00
  000000014001632A: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140016332: FF 90 A0 01 00 00  call        qword ptr [rax+00000000000001A0h]
  0000000140016338: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140016340: 48 83 BC 24 A0 00  cmp         qword ptr [rsp+00000000000000A0h],0
                    00 00 00
  0000000140016349: 0F 84 9E 00 00 00  je          00000001400163ED
  000000014001634F: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140016357: E8 A4 1B 0C 00     call        00000001400D7F00
  000000014001635C: 89 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],eax
                    00
  0000000140016363: C7 84 24 9C 00 00  mov         dword ptr [rsp+000000000000009Ch],0
                    00 00 00 00 00
  000000014001636E: EB 11              jmp         0000000140016381
  0000000140016370: 8B 84 24 9C 00 00  mov         eax,dword ptr [rsp+000000000000009Ch]
                    00
  0000000140016377: 83 C0 01           add         eax,1
  000000014001637A: 89 84 24 9C 00 00  mov         dword ptr [rsp+000000000000009Ch],eax
                    00
  0000000140016381: 8B 84 24 A8 00 00  mov         eax,dword ptr [rsp+00000000000000A8h]
                    00
  0000000140016388: 39 84 24 9C 00 00  cmp         dword ptr [rsp+000000000000009Ch],eax
                    00
  000000014001638F: 7D 5C              jge         00000001400163ED
  0000000140016391: 8B 94 24 9C 00 00  mov         edx,dword ptr [rsp+000000000000009Ch]
                    00
  0000000140016398: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400163A0: E8 3B 07 80 00     call        0000000140816AE0
  00000001400163A5: 89 84 24 AC 00 00  mov         dword ptr [rsp+00000000000000ACh],eax
                    00
  00000001400163AC: 4C 8D 84 24 AC 00  lea         r8,[rsp+00000000000000ACh]
                    00 00
  00000001400163B4: 48 8D 94 24 90 02  lea         rdx,[rsp+0000000000000290h]
                    00 00
  00000001400163BC: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400163C4: E8 07 EE 00 00     call        00000001400251D0
  00000001400163C9: 48 8B C8           mov         rcx,rax
  00000001400163CC: E8 4F E6 FE FF     call        0000000140004A20
  00000001400163D1: 48 85 C0           test        rax,rax
  00000001400163D4: 75 15              jne         00000001400163EB
  00000001400163D6: 48 8D 94 24 AC 00  lea         rdx,[rsp+00000000000000ACh]
                    00 00
  00000001400163DE: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400163E6: E8 15 ED 00 00     call        0000000140025100
  00000001400163EB: EB 83              jmp         0000000140016370
  00000001400163ED: E9 01 FF FF FF     jmp         00000001400162F3
  00000001400163F2: C7 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00
  00000001400163FD: EB 11              jmp         0000000140016410
  00000001400163FF: 8B 84 24 88 00 00  mov         eax,dword ptr [rsp+0000000000000088h]
                    00
  0000000140016406: 83 C0 01           add         eax,1
  0000000140016409: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  0000000140016410: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140016418: E8 C3 E6 FE FF     call        0000000140004AE0
  000000014001641D: 39 84 24 88 00 00  cmp         dword ptr [rsp+0000000000000088h],eax
                    00
  0000000140016424: 0F 8D 87 02 00 00  jge         00000001400166B1
  000000014001642A: C6 84 24 38 03 00  mov         byte ptr [rsp+0000000000000338h],1
                    00 01
  0000000140016432: 4C 8D 84 24 38 03  lea         r8,[rsp+0000000000000338h]
                    00 00
  000000014001643A: 48 8D 15 7B 39 0E  lea         rdx,[00000001420F9DBCh]
                    02
  0000000140016441: 48 8D 8C 24 28 03  lea         rcx,[rsp+0000000000000328h]
                    00 00
  0000000140016449: E8 32 EC 00 00     call        0000000140025080
  000000014001644E: 48 8D BC 24 60 0C  lea         rdi,[rsp+0000000000000C60h]
                    00 00
  0000000140016456: 48 8B F0           mov         rsi,rax
  0000000140016459: B9 10 00 00 00     mov         ecx,10h
  000000014001645E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016460: 48 8D BC 24 18 03  lea         rdi,[rsp+0000000000000318h]
                    00 00
  0000000140016468: 48 8D B4 24 60 0C  lea         rsi,[rsp+0000000000000C60h]
                    00 00
  0000000140016470: B9 10 00 00 00     mov         ecx,10h
  0000000140016475: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016477: 8B 94 24 88 00 00  mov         edx,dword ptr [rsp+0000000000000088h]
                    00
  000000014001647E: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140016486: E8 35 EC 00 00     call        00000001400250C0
  000000014001648B: 4C 8B C0           mov         r8,rax
  000000014001648E: 48 8D 15 47 EA 0D  lea         rdx,[00000001420F4EDCh]
                    02
  0000000140016495: 48 8D 8C 24 50 03  lea         rcx,[rsp+0000000000000350h]
                    00 00
  000000014001649D: E8 DE EB 00 00     call        0000000140025080
  00000001400164A2: 48 8D BC 24 50 0C  lea         rdi,[rsp+0000000000000C50h]
                    00 00
  00000001400164AA: 48 8B F0           mov         rsi,rax
  00000001400164AD: B9 10 00 00 00     mov         ecx,10h
  00000001400164B2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400164B4: 48 8D BC 24 40 03  lea         rdi,[rsp+0000000000000340h]
                    00 00
  00000001400164BC: 48 8D B4 24 50 0C  lea         rsi,[rsp+0000000000000C50h]
                    00 00
  00000001400164C4: B9 10 00 00 00     mov         ecx,10h
  00000001400164C9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400164CB: 45 33 C0           xor         r8d,r8d
  00000001400164CE: 33 D2              xor         edx,edx
  00000001400164D0: 48 8D 8C 24 98 02  lea         rcx,[rsp+0000000000000298h]
                    00 00
  00000001400164D8: FF 15 8A DC DE 01  call        qword ptr [0000000141E04168h]
  00000001400164DE: 48 8D BC 24 10 09  lea         rdi,[rsp+0000000000000910h]
                    00 00
  00000001400164E6: 48 8B F0           mov         rsi,rax
  00000001400164E9: B9 10 00 00 00     mov         ecx,10h
  00000001400164EE: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400164F0: 45 33 C0           xor         r8d,r8d
  00000001400164F3: 33 D2              xor         edx,edx
  00000001400164F5: 48 8D 8C 24 A8 02  lea         rcx,[rsp+00000000000002A8h]
                    00 00
  00000001400164FD: FF 15 65 DC DE 01  call        qword ptr [0000000141E04168h]
  0000000140016503: 48 8D BC 24 00 09  lea         rdi,[rsp+0000000000000900h]
                    00 00
  000000014001650B: 48 8B F0           mov         rsi,rax
  000000014001650E: B9 10 00 00 00     mov         ecx,10h
  0000000140016513: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016515: 45 33 C0           xor         r8d,r8d
  0000000140016518: 33 D2              xor         edx,edx
  000000014001651A: 48 8D 8C 24 B8 02  lea         rcx,[rsp+00000000000002B8h]
                    00 00
  0000000140016522: FF 15 40 DC DE 01  call        qword ptr [0000000141E04168h]
  0000000140016528: 48 8D BC 24 F0 08  lea         rdi,[rsp+00000000000008F0h]
                    00 00
  0000000140016530: 48 8B F0           mov         rsi,rax
  0000000140016533: B9 10 00 00 00     mov         ecx,10h
  0000000140016538: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001653A: 45 33 C0           xor         r8d,r8d
  000000014001653D: 33 D2              xor         edx,edx
  000000014001653F: 48 8D 8C 24 C8 02  lea         rcx,[rsp+00000000000002C8h]
                    00 00
  0000000140016547: FF 15 1B DC DE 01  call        qword ptr [0000000141E04168h]
  000000014001654D: 48 8D BC 24 E0 08  lea         rdi,[rsp+00000000000008E0h]
                    00 00
  0000000140016555: 48 8B F0           mov         rsi,rax
  0000000140016558: B9 10 00 00 00     mov         ecx,10h
  000000014001655D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001655F: 45 33 C0           xor         r8d,r8d
  0000000140016562: 33 D2              xor         edx,edx
  0000000140016564: 48 8D 8C 24 D8 02  lea         rcx,[rsp+00000000000002D8h]
                    00 00
  000000014001656C: FF 15 F6 DB DE 01  call        qword ptr [0000000141E04168h]
  0000000140016572: 48 8D BC 24 D0 08  lea         rdi,[rsp+00000000000008D0h]
                    00 00
  000000014001657A: 48 8B F0           mov         rsi,rax
  000000014001657D: B9 10 00 00 00     mov         ecx,10h
  0000000140016582: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016584: 45 33 C0           xor         r8d,r8d
  0000000140016587: 33 D2              xor         edx,edx
  0000000140016589: 48 8D 8C 24 E8 02  lea         rcx,[rsp+00000000000002E8h]
                    00 00
  0000000140016591: FF 15 D1 DB DE 01  call        qword ptr [0000000141E04168h]
  0000000140016597: 48 8D BC 24 C0 08  lea         rdi,[rsp+00000000000008C0h]
                    00 00
  000000014001659F: 48 8B F0           mov         rsi,rax
  00000001400165A2: B9 10 00 00 00     mov         ecx,10h
  00000001400165A7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400165A9: 45 33 C0           xor         r8d,r8d
  00000001400165AC: 33 D2              xor         edx,edx
  00000001400165AE: 48 8D 8C 24 F8 02  lea         rcx,[rsp+00000000000002F8h]
                    00 00
  00000001400165B6: FF 15 AC DB DE 01  call        qword ptr [0000000141E04168h]
  00000001400165BC: 48 8D BC 24 B0 08  lea         rdi,[rsp+00000000000008B0h]
                    00 00
  00000001400165C4: 48 8B F0           mov         rsi,rax
  00000001400165C7: B9 10 00 00 00     mov         ecx,10h
  00000001400165CC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400165CE: 45 33 C0           xor         r8d,r8d
  00000001400165D1: 33 D2              xor         edx,edx
  00000001400165D3: 48 8D 8C 24 08 03  lea         rcx,[rsp+0000000000000308h]
                    00 00
  00000001400165DB: FF 15 87 DB DE 01  call        qword ptr [0000000141E04168h]
  00000001400165E1: 48 8D BC 24 A0 08  lea         rdi,[rsp+00000000000008A0h]
                    00 00
  00000001400165E9: 48 8B F0           mov         rsi,rax
  00000001400165EC: B9 10 00 00 00     mov         ecx,10h
  00000001400165F1: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400165F3: 48 8D BC 24 90 08  lea         rdi,[rsp+0000000000000890h]
                    00 00
  00000001400165FB: 48 8D B4 24 18 03  lea         rsi,[rsp+0000000000000318h]
                    00 00
  0000000140016603: B9 10 00 00 00     mov         ecx,10h
  0000000140016608: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001660A: 48 8D BC 24 80 08  lea         rdi,[rsp+0000000000000880h]
                    00 00
  0000000140016612: 48 8D B4 24 40 03  lea         rsi,[rsp+0000000000000340h]
                    00 00
  000000014001661A: B9 10 00 00 00     mov         ecx,10h
  000000014001661F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016621: 48 8D 84 24 10 09  lea         rax,[rsp+0000000000000910h]
                    00 00
  0000000140016629: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014001662E: 48 8D 84 24 00 09  lea         rax,[rsp+0000000000000900h]
                    00 00
  0000000140016636: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014001663B: 48 8D 84 24 F0 08  lea         rax,[rsp+00000000000008F0h]
                    00 00
  0000000140016643: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140016648: 48 8D 84 24 E0 08  lea         rax,[rsp+00000000000008E0h]
                    00 00
  0000000140016650: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140016655: 48 8D 84 24 D0 08  lea         rax,[rsp+00000000000008D0h]
                    00 00
  000000014001665D: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140016662: 48 8D 84 24 C0 08  lea         rax,[rsp+00000000000008C0h]
                    00 00
  000000014001666A: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014001666F: 48 8D 84 24 B0 08  lea         rax,[rsp+00000000000008B0h]
                    00 00
  0000000140016677: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014001667C: 48 8D 84 24 A0 08  lea         rax,[rsp+00000000000008A0h]
                    00 00
  0000000140016684: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140016689: 4C 8D 8C 24 90 08  lea         r9,[rsp+0000000000000890h]
                    00 00
  0000000140016691: 4C 8D 84 24 80 08  lea         r8,[rsp+0000000000000880h]
                    00 00
  0000000140016699: 48 8D 15 28 37 0E  lea         rdx,[00000001420F9DC8h]
                    02
  00000001400166A0: 48 8D 0D 59 00 D6  lea         rcx,[0000000143D76700h]
                    03
  00000001400166A7: E8 14 8D 2B 00     call        00000001402CF3C0
  00000001400166AC: E9 4E FD FF FF     jmp         00000001400163FF
  00000001400166B1: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400166B9: E8 E2 1A 1F 00     call        00000001402081A0
  00000001400166BE: E9 FD 01 00 00     jmp         00000001400168C0
  00000001400166C3: 45 33 C0           xor         r8d,r8d
  00000001400166C6: 33 D2              xor         edx,edx
  00000001400166C8: 48 8D 8C 24 60 03  lea         rcx,[rsp+0000000000000360h]
                    00 00
  00000001400166D0: FF 15 92 DA DE 01  call        qword ptr [0000000141E04168h]
  00000001400166D6: 48 8D BC 24 B0 09  lea         rdi,[rsp+00000000000009B0h]
                    00 00
  00000001400166DE: 48 8B F0           mov         rsi,rax
  00000001400166E1: B9 10 00 00 00     mov         ecx,10h
  00000001400166E6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400166E8: 45 33 C0           xor         r8d,r8d
  00000001400166EB: 33 D2              xor         edx,edx
  00000001400166ED: 48 8D 8C 24 70 03  lea         rcx,[rsp+0000000000000370h]
                    00 00
  00000001400166F5: FF 15 6D DA DE 01  call        qword ptr [0000000141E04168h]
  00000001400166FB: 48 8D BC 24 A0 09  lea         rdi,[rsp+00000000000009A0h]
                    00 00
  0000000140016703: 48 8B F0           mov         rsi,rax
  0000000140016706: B9 10 00 00 00     mov         ecx,10h
  000000014001670B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001670D: 45 33 C0           xor         r8d,r8d
  0000000140016710: 33 D2              xor         edx,edx
  0000000140016712: 48 8D 8C 24 80 03  lea         rcx,[rsp+0000000000000380h]
                    00 00
  000000014001671A: FF 15 48 DA DE 01  call        qword ptr [0000000141E04168h]
  0000000140016720: 48 8D BC 24 90 09  lea         rdi,[rsp+0000000000000990h]
                    00 00
  0000000140016728: 48 8B F0           mov         rsi,rax
  000000014001672B: B9 10 00 00 00     mov         ecx,10h
  0000000140016730: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016732: 45 33 C0           xor         r8d,r8d
  0000000140016735: 33 D2              xor         edx,edx
  0000000140016737: 48 8D 8C 24 90 03  lea         rcx,[rsp+0000000000000390h]
                    00 00
  000000014001673F: FF 15 23 DA DE 01  call        qword ptr [0000000141E04168h]
  0000000140016745: 48 8D BC 24 80 09  lea         rdi,[rsp+0000000000000980h]
                    00 00
  000000014001674D: 48 8B F0           mov         rsi,rax
  0000000140016750: B9 10 00 00 00     mov         ecx,10h
  0000000140016755: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016757: 45 33 C0           xor         r8d,r8d
  000000014001675A: 33 D2              xor         edx,edx
  000000014001675C: 48 8D 8C 24 A0 03  lea         rcx,[rsp+00000000000003A0h]
                    00 00
  0000000140016764: FF 15 FE D9 DE 01  call        qword ptr [0000000141E04168h]
  000000014001676A: 48 8D BC 24 70 09  lea         rdi,[rsp+0000000000000970h]
                    00 00
  0000000140016772: 48 8B F0           mov         rsi,rax
  0000000140016775: B9 10 00 00 00     mov         ecx,10h
  000000014001677A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001677C: 45 33 C0           xor         r8d,r8d
  000000014001677F: 33 D2              xor         edx,edx
  0000000140016781: 48 8D 8C 24 B0 03  lea         rcx,[rsp+00000000000003B0h]
                    00 00
  0000000140016789: FF 15 D9 D9 DE 01  call        qword ptr [0000000141E04168h]
  000000014001678F: 48 8D BC 24 60 09  lea         rdi,[rsp+0000000000000960h]
                    00 00
  0000000140016797: 48 8B F0           mov         rsi,rax
  000000014001679A: B9 10 00 00 00     mov         ecx,10h
  000000014001679F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400167A1: 45 33 C0           xor         r8d,r8d
  00000001400167A4: 33 D2              xor         edx,edx
  00000001400167A6: 48 8D 8C 24 C0 03  lea         rcx,[rsp+00000000000003C0h]
                    00 00
  00000001400167AE: FF 15 B4 D9 DE 01  call        qword ptr [0000000141E04168h]
  00000001400167B4: 48 8D BC 24 50 09  lea         rdi,[rsp+0000000000000950h]
                    00 00
  00000001400167BC: 48 8B F0           mov         rsi,rax
  00000001400167BF: B9 10 00 00 00     mov         ecx,10h
  00000001400167C4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400167C6: 45 33 C0           xor         r8d,r8d
  00000001400167C9: 33 D2              xor         edx,edx
  00000001400167CB: 48 8D 8C 24 D0 03  lea         rcx,[rsp+00000000000003D0h]
                    00 00
  00000001400167D3: FF 15 8F D9 DE 01  call        qword ptr [0000000141E04168h]
  00000001400167D9: 48 8D BC 24 40 09  lea         rdi,[rsp+0000000000000940h]
                    00 00
  00000001400167E1: 48 8B F0           mov         rsi,rax
  00000001400167E4: B9 10 00 00 00     mov         ecx,10h
  00000001400167E9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400167EB: 45 33 C0           xor         r8d,r8d
  00000001400167EE: 33 D2              xor         edx,edx
  00000001400167F0: 48 8D 8C 24 E0 03  lea         rcx,[rsp+00000000000003E0h]
                    00 00
  00000001400167F8: FF 15 6A D9 DE 01  call        qword ptr [0000000141E04168h]
  00000001400167FE: 48 8D BC 24 30 09  lea         rdi,[rsp+0000000000000930h]
                    00 00
  0000000140016806: 48 8B F0           mov         rsi,rax
  0000000140016809: B9 10 00 00 00     mov         ecx,10h
  000000014001680E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016810: 45 33 C0           xor         r8d,r8d
  0000000140016813: 33 D2              xor         edx,edx
  0000000140016815: 48 8D 8C 24 F0 03  lea         rcx,[rsp+00000000000003F0h]
                    00 00
  000000014001681D: FF 15 45 D9 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016823: 48 8D BC 24 20 09  lea         rdi,[rsp+0000000000000920h]
                    00 00
  000000014001682B: 48 8B F0           mov         rsi,rax
  000000014001682E: B9 10 00 00 00     mov         ecx,10h
  0000000140016833: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016835: 48 8D 84 24 B0 09  lea         rax,[rsp+00000000000009B0h]
                    00 00
  000000014001683D: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140016842: 48 8D 84 24 A0 09  lea         rax,[rsp+00000000000009A0h]
                    00 00
  000000014001684A: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014001684F: 48 8D 84 24 90 09  lea         rax,[rsp+0000000000000990h]
                    00 00
  0000000140016857: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014001685C: 48 8D 84 24 80 09  lea         rax,[rsp+0000000000000980h]
                    00 00
  0000000140016864: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140016869: 48 8D 84 24 70 09  lea         rax,[rsp+0000000000000970h]
                    00 00
  0000000140016871: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140016876: 48 8D 84 24 60 09  lea         rax,[rsp+0000000000000960h]
                    00 00
  000000014001687E: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140016883: 48 8D 84 24 50 09  lea         rax,[rsp+0000000000000950h]
                    00 00
  000000014001688B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140016890: 48 8D 84 24 40 09  lea         rax,[rsp+0000000000000940h]
                    00 00
  0000000140016898: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014001689D: 4C 8D 8C 24 30 09  lea         r9,[rsp+0000000000000930h]
                    00 00
  00000001400168A5: 4C 8D 84 24 20 09  lea         r8,[rsp+0000000000000920h]
                    00 00
  00000001400168AD: 48 8D 15 2C 35 0E  lea         rdx,[00000001420F9DE0h]
                    02
  00000001400168B4: 48 8D 0D 45 FE D5  lea         rcx,[0000000143D76700h]
                    03
  00000001400168BB: E8 00 8B 2B 00     call        00000001402CF3C0
  00000001400168C0: 48 8D 0D F1 FD D5  lea         rcx,[0000000143D766B8h]
                    03
  00000001400168C7: E8 D4 4E 2B 00     call        00000001402CB7A0
  00000001400168CC: 83 F8 05           cmp         eax,5
  00000001400168CF: 0F 85 31 02 00 00  jne         0000000140016B06
  00000001400168D5: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  00000001400168DA: 48 8D 15 1F 35 0E  lea         rdx,[00000001420F9E00h]
                    02
  00000001400168E1: 48 8D 8C 24 A0 04  lea         rcx,[rsp+00000000000004A0h]
                    00 00
  00000001400168E9: E8 92 E7 00 00     call        0000000140025080
  00000001400168EE: 48 8D BC 24 40 0C  lea         rdi,[rsp+0000000000000C40h]
                    00 00
  00000001400168F6: 48 8B F0           mov         rsi,rax
  00000001400168F9: B9 10 00 00 00     mov         ecx,10h
  00000001400168FE: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016900: 48 8D BC 24 90 04  lea         rdi,[rsp+0000000000000490h]
                    00 00
  0000000140016908: 48 8D B4 24 40 0C  lea         rsi,[rsp+0000000000000C40h]
                    00 00
  0000000140016910: B9 10 00 00 00     mov         ecx,10h
  0000000140016915: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016917: 45 33 C0           xor         r8d,r8d
  000000014001691A: 33 D2              xor         edx,edx
  000000014001691C: 48 8D 8C 24 00 04  lea         rcx,[rsp+0000000000000400h]
                    00 00
  0000000140016924: FF 15 3E D8 DE 01  call        qword ptr [0000000141E04168h]
  000000014001692A: 48 8D BC 24 50 0A  lea         rdi,[rsp+0000000000000A50h]
                    00 00
  0000000140016932: 48 8B F0           mov         rsi,rax
  0000000140016935: B9 10 00 00 00     mov         ecx,10h
  000000014001693A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001693C: 45 33 C0           xor         r8d,r8d
  000000014001693F: 33 D2              xor         edx,edx
  0000000140016941: 48 8D 8C 24 10 04  lea         rcx,[rsp+0000000000000410h]
                    00 00
  0000000140016949: FF 15 19 D8 DE 01  call        qword ptr [0000000141E04168h]
  000000014001694F: 48 8D BC 24 40 0A  lea         rdi,[rsp+0000000000000A40h]
                    00 00
  0000000140016957: 48 8B F0           mov         rsi,rax
  000000014001695A: B9 10 00 00 00     mov         ecx,10h
  000000014001695F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016961: 45 33 C0           xor         r8d,r8d
  0000000140016964: 33 D2              xor         edx,edx
  0000000140016966: 48 8D 8C 24 20 04  lea         rcx,[rsp+0000000000000420h]
                    00 00
  000000014001696E: FF 15 F4 D7 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016974: 48 8D BC 24 30 0A  lea         rdi,[rsp+0000000000000A30h]
                    00 00
  000000014001697C: 48 8B F0           mov         rsi,rax
  000000014001697F: B9 10 00 00 00     mov         ecx,10h
  0000000140016984: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016986: 45 33 C0           xor         r8d,r8d
  0000000140016989: 33 D2              xor         edx,edx
  000000014001698B: 48 8D 8C 24 30 04  lea         rcx,[rsp+0000000000000430h]
                    00 00
  0000000140016993: FF 15 CF D7 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016999: 48 8D BC 24 20 0A  lea         rdi,[rsp+0000000000000A20h]
                    00 00
  00000001400169A1: 48 8B F0           mov         rsi,rax
  00000001400169A4: B9 10 00 00 00     mov         ecx,10h
  00000001400169A9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400169AB: 45 33 C0           xor         r8d,r8d
  00000001400169AE: 33 D2              xor         edx,edx
  00000001400169B0: 48 8D 8C 24 40 04  lea         rcx,[rsp+0000000000000440h]
                    00 00
  00000001400169B8: FF 15 AA D7 DE 01  call        qword ptr [0000000141E04168h]
  00000001400169BE: 48 8D BC 24 10 0A  lea         rdi,[rsp+0000000000000A10h]
                    00 00
  00000001400169C6: 48 8B F0           mov         rsi,rax
  00000001400169C9: B9 10 00 00 00     mov         ecx,10h
  00000001400169CE: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400169D0: 45 33 C0           xor         r8d,r8d
  00000001400169D3: 33 D2              xor         edx,edx
  00000001400169D5: 48 8D 8C 24 50 04  lea         rcx,[rsp+0000000000000450h]
                    00 00
  00000001400169DD: FF 15 85 D7 DE 01  call        qword ptr [0000000141E04168h]
  00000001400169E3: 48 8D BC 24 00 0A  lea         rdi,[rsp+0000000000000A00h]
                    00 00
  00000001400169EB: 48 8B F0           mov         rsi,rax
  00000001400169EE: B9 10 00 00 00     mov         ecx,10h
  00000001400169F3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400169F5: 45 33 C0           xor         r8d,r8d
  00000001400169F8: 33 D2              xor         edx,edx
  00000001400169FA: 48 8D 8C 24 60 04  lea         rcx,[rsp+0000000000000460h]
                    00 00
  0000000140016A02: FF 15 60 D7 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016A08: 48 8D BC 24 F0 09  lea         rdi,[rsp+00000000000009F0h]
                    00 00
  0000000140016A10: 48 8B F0           mov         rsi,rax
  0000000140016A13: B9 10 00 00 00     mov         ecx,10h
  0000000140016A18: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016A1A: 45 33 C0           xor         r8d,r8d
  0000000140016A1D: 33 D2              xor         edx,edx
  0000000140016A1F: 48 8D 8C 24 70 04  lea         rcx,[rsp+0000000000000470h]
                    00 00
  0000000140016A27: FF 15 3B D7 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016A2D: 48 8D BC 24 E0 09  lea         rdi,[rsp+00000000000009E0h]
                    00 00
  0000000140016A35: 48 8B F0           mov         rsi,rax
  0000000140016A38: B9 10 00 00 00     mov         ecx,10h
  0000000140016A3D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016A3F: 45 33 C0           xor         r8d,r8d
  0000000140016A42: 33 D2              xor         edx,edx
  0000000140016A44: 48 8D 8C 24 80 04  lea         rcx,[rsp+0000000000000480h]
                    00 00
  0000000140016A4C: FF 15 16 D7 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016A52: 48 8D BC 24 D0 09  lea         rdi,[rsp+00000000000009D0h]
                    00 00
  0000000140016A5A: 48 8B F0           mov         rsi,rax
  0000000140016A5D: B9 10 00 00 00     mov         ecx,10h
  0000000140016A62: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016A64: 48 8D BC 24 C0 09  lea         rdi,[rsp+00000000000009C0h]
                    00 00
  0000000140016A6C: 48 8D B4 24 90 04  lea         rsi,[rsp+0000000000000490h]
                    00 00
  0000000140016A74: B9 10 00 00 00     mov         ecx,10h
  0000000140016A79: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016A7B: 48 8D 84 24 50 0A  lea         rax,[rsp+0000000000000A50h]
                    00 00
  0000000140016A83: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140016A88: 48 8D 84 24 40 0A  lea         rax,[rsp+0000000000000A40h]
                    00 00
  0000000140016A90: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140016A95: 48 8D 84 24 30 0A  lea         rax,[rsp+0000000000000A30h]
                    00 00
  0000000140016A9D: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140016AA2: 48 8D 84 24 20 0A  lea         rax,[rsp+0000000000000A20h]
                    00 00
  0000000140016AAA: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140016AAF: 48 8D 84 24 10 0A  lea         rax,[rsp+0000000000000A10h]
                    00 00
  0000000140016AB7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140016ABC: 48 8D 84 24 00 0A  lea         rax,[rsp+0000000000000A00h]
                    00 00
  0000000140016AC4: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140016AC9: 48 8D 84 24 F0 09  lea         rax,[rsp+00000000000009F0h]
                    00 00
  0000000140016AD1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140016AD6: 48 8D 84 24 E0 09  lea         rax,[rsp+00000000000009E0h]
                    00 00
  0000000140016ADE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140016AE3: 4C 8D 8C 24 D0 09  lea         r9,[rsp+00000000000009D0h]
                    00 00
  0000000140016AEB: 4C 8D 84 24 C0 09  lea         r8,[rsp+00000000000009C0h]
                    00 00
  0000000140016AF3: 48 8D 15 16 33 0E  lea         rdx,[00000001420F9E10h]
                    02
  0000000140016AFA: 48 8D 0D FF FB D5  lea         rcx,[0000000143D76700h]
                    03
  0000000140016B01: E8 BA 88 2B 00     call        00000001402CF3C0
  0000000140016B06: 45 33 C0           xor         r8d,r8d
  0000000140016B09: 33 D2              xor         edx,edx
  0000000140016B0B: 48 8D 8C 24 B0 04  lea         rcx,[rsp+00000000000004B0h]
                    00 00
  0000000140016B13: FF 15 4F D6 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016B19: 48 8D BC 24 F0 0A  lea         rdi,[rsp+0000000000000AF0h]
                    00 00
  0000000140016B21: 48 8B F0           mov         rsi,rax
  0000000140016B24: B9 10 00 00 00     mov         ecx,10h
  0000000140016B29: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016B2B: 45 33 C0           xor         r8d,r8d
  0000000140016B2E: 33 D2              xor         edx,edx
  0000000140016B30: 48 8D 8C 24 C0 04  lea         rcx,[rsp+00000000000004C0h]
                    00 00
  0000000140016B38: FF 15 2A D6 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016B3E: 48 8D BC 24 E0 0A  lea         rdi,[rsp+0000000000000AE0h]
                    00 00
  0000000140016B46: 48 8B F0           mov         rsi,rax
  0000000140016B49: B9 10 00 00 00     mov         ecx,10h
  0000000140016B4E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016B50: 45 33 C0           xor         r8d,r8d
  0000000140016B53: 33 D2              xor         edx,edx
  0000000140016B55: 48 8D 8C 24 D0 04  lea         rcx,[rsp+00000000000004D0h]
                    00 00
  0000000140016B5D: FF 15 05 D6 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016B63: 48 8D BC 24 D0 0A  lea         rdi,[rsp+0000000000000AD0h]
                    00 00
  0000000140016B6B: 48 8B F0           mov         rsi,rax
  0000000140016B6E: B9 10 00 00 00     mov         ecx,10h
  0000000140016B73: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016B75: 45 33 C0           xor         r8d,r8d
  0000000140016B78: 33 D2              xor         edx,edx
  0000000140016B7A: 48 8D 8C 24 E0 04  lea         rcx,[rsp+00000000000004E0h]
                    00 00
  0000000140016B82: FF 15 E0 D5 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016B88: 48 8D BC 24 C0 0A  lea         rdi,[rsp+0000000000000AC0h]
                    00 00
  0000000140016B90: 48 8B F0           mov         rsi,rax
  0000000140016B93: B9 10 00 00 00     mov         ecx,10h
  0000000140016B98: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016B9A: 45 33 C0           xor         r8d,r8d
  0000000140016B9D: 33 D2              xor         edx,edx
  0000000140016B9F: 48 8D 8C 24 F0 04  lea         rcx,[rsp+00000000000004F0h]
                    00 00
  0000000140016BA7: FF 15 BB D5 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016BAD: 48 8D BC 24 B0 0A  lea         rdi,[rsp+0000000000000AB0h]
                    00 00
  0000000140016BB5: 48 8B F0           mov         rsi,rax
  0000000140016BB8: B9 10 00 00 00     mov         ecx,10h
  0000000140016BBD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016BBF: 45 33 C0           xor         r8d,r8d
  0000000140016BC2: 33 D2              xor         edx,edx
  0000000140016BC4: 48 8D 8C 24 00 05  lea         rcx,[rsp+0000000000000500h]
                    00 00
  0000000140016BCC: FF 15 96 D5 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016BD2: 48 8D BC 24 A0 0A  lea         rdi,[rsp+0000000000000AA0h]
                    00 00
  0000000140016BDA: 48 8B F0           mov         rsi,rax
  0000000140016BDD: B9 10 00 00 00     mov         ecx,10h
  0000000140016BE2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016BE4: 45 33 C0           xor         r8d,r8d
  0000000140016BE7: 33 D2              xor         edx,edx
  0000000140016BE9: 48 8D 8C 24 10 05  lea         rcx,[rsp+0000000000000510h]
                    00 00
  0000000140016BF1: FF 15 71 D5 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016BF7: 48 8D BC 24 90 0A  lea         rdi,[rsp+0000000000000A90h]
                    00 00
  0000000140016BFF: 48 8B F0           mov         rsi,rax
  0000000140016C02: B9 10 00 00 00     mov         ecx,10h
  0000000140016C07: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016C09: 45 33 C0           xor         r8d,r8d
  0000000140016C0C: 33 D2              xor         edx,edx
  0000000140016C0E: 48 8D 8C 24 20 05  lea         rcx,[rsp+0000000000000520h]
                    00 00
  0000000140016C16: FF 15 4C D5 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016C1C: 48 8D BC 24 80 0A  lea         rdi,[rsp+0000000000000A80h]
                    00 00
  0000000140016C24: 48 8B F0           mov         rsi,rax
  0000000140016C27: B9 10 00 00 00     mov         ecx,10h
  0000000140016C2C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016C2E: 45 33 C0           xor         r8d,r8d
  0000000140016C31: 33 D2              xor         edx,edx
  0000000140016C33: 48 8D 8C 24 30 05  lea         rcx,[rsp+0000000000000530h]
                    00 00
  0000000140016C3B: FF 15 27 D5 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016C41: 48 8D BC 24 70 0A  lea         rdi,[rsp+0000000000000A70h]
                    00 00
  0000000140016C49: 48 8B F0           mov         rsi,rax
  0000000140016C4C: B9 10 00 00 00     mov         ecx,10h
  0000000140016C51: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016C53: 45 33 C0           xor         r8d,r8d
  0000000140016C56: 33 D2              xor         edx,edx
  0000000140016C58: 48 8D 8C 24 40 05  lea         rcx,[rsp+0000000000000540h]
                    00 00
  0000000140016C60: FF 15 02 D5 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016C66: 48 8D BC 24 60 0A  lea         rdi,[rsp+0000000000000A60h]
                    00 00
  0000000140016C6E: 48 8B F0           mov         rsi,rax
  0000000140016C71: B9 10 00 00 00     mov         ecx,10h
  0000000140016C76: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016C78: 48 8D 84 24 F0 0A  lea         rax,[rsp+0000000000000AF0h]
                    00 00
  0000000140016C80: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140016C85: 48 8D 84 24 E0 0A  lea         rax,[rsp+0000000000000AE0h]
                    00 00
  0000000140016C8D: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140016C92: 48 8D 84 24 D0 0A  lea         rax,[rsp+0000000000000AD0h]
                    00 00
  0000000140016C9A: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140016C9F: 48 8D 84 24 C0 0A  lea         rax,[rsp+0000000000000AC0h]
                    00 00
  0000000140016CA7: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140016CAC: 48 8D 84 24 B0 0A  lea         rax,[rsp+0000000000000AB0h]
                    00 00
  0000000140016CB4: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140016CB9: 48 8D 84 24 A0 0A  lea         rax,[rsp+0000000000000AA0h]
                    00 00
  0000000140016CC1: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140016CC6: 48 8D 84 24 90 0A  lea         rax,[rsp+0000000000000A90h]
                    00 00
  0000000140016CCE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140016CD3: 48 8D 84 24 80 0A  lea         rax,[rsp+0000000000000A80h]
                    00 00
  0000000140016CDB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140016CE0: 4C 8D 8C 24 70 0A  lea         r9,[rsp+0000000000000A70h]
                    00 00
  0000000140016CE8: 4C 8D 84 24 60 0A  lea         r8,[rsp+0000000000000A60h]
                    00 00
  0000000140016CF0: 48 8D 15 31 31 0E  lea         rdx,[00000001420F9E28h]
                    02
  0000000140016CF7: 48 8D 0D 02 FA D5  lea         rcx,[0000000143D76700h]
                    03
  0000000140016CFE: E8 BD 86 2B 00     call        00000001402CF3C0
  0000000140016D03: 45 33 C0           xor         r8d,r8d
  0000000140016D06: 33 D2              xor         edx,edx
  0000000140016D08: 48 8D 8C 24 50 05  lea         rcx,[rsp+0000000000000550h]
                    00 00
  0000000140016D10: FF 15 52 D4 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016D16: 48 8D BC 24 90 0B  lea         rdi,[rsp+0000000000000B90h]
                    00 00
  0000000140016D1E: 48 8B F0           mov         rsi,rax
  0000000140016D21: B9 10 00 00 00     mov         ecx,10h
  0000000140016D26: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016D28: 45 33 C0           xor         r8d,r8d
  0000000140016D2B: 33 D2              xor         edx,edx
  0000000140016D2D: 48 8D 8C 24 60 05  lea         rcx,[rsp+0000000000000560h]
                    00 00
  0000000140016D35: FF 15 2D D4 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016D3B: 48 8D BC 24 80 0B  lea         rdi,[rsp+0000000000000B80h]
                    00 00
  0000000140016D43: 48 8B F0           mov         rsi,rax
  0000000140016D46: B9 10 00 00 00     mov         ecx,10h
  0000000140016D4B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016D4D: 45 33 C0           xor         r8d,r8d
  0000000140016D50: 33 D2              xor         edx,edx
  0000000140016D52: 48 8D 8C 24 70 05  lea         rcx,[rsp+0000000000000570h]
                    00 00
  0000000140016D5A: FF 15 08 D4 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016D60: 48 8D BC 24 70 0B  lea         rdi,[rsp+0000000000000B70h]
                    00 00
  0000000140016D68: 48 8B F0           mov         rsi,rax
  0000000140016D6B: B9 10 00 00 00     mov         ecx,10h
  0000000140016D70: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016D72: 45 33 C0           xor         r8d,r8d
  0000000140016D75: 33 D2              xor         edx,edx
  0000000140016D77: 48 8D 8C 24 80 05  lea         rcx,[rsp+0000000000000580h]
                    00 00
  0000000140016D7F: FF 15 E3 D3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016D85: 48 8D BC 24 60 0B  lea         rdi,[rsp+0000000000000B60h]
                    00 00
  0000000140016D8D: 48 8B F0           mov         rsi,rax
  0000000140016D90: B9 10 00 00 00     mov         ecx,10h
  0000000140016D95: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016D97: 45 33 C0           xor         r8d,r8d
  0000000140016D9A: 33 D2              xor         edx,edx
  0000000140016D9C: 48 8D 8C 24 90 05  lea         rcx,[rsp+0000000000000590h]
                    00 00
  0000000140016DA4: FF 15 BE D3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016DAA: 48 8D BC 24 50 0B  lea         rdi,[rsp+0000000000000B50h]
                    00 00
  0000000140016DB2: 48 8B F0           mov         rsi,rax
  0000000140016DB5: B9 10 00 00 00     mov         ecx,10h
  0000000140016DBA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016DBC: 45 33 C0           xor         r8d,r8d
  0000000140016DBF: 33 D2              xor         edx,edx
  0000000140016DC1: 48 8D 8C 24 A0 05  lea         rcx,[rsp+00000000000005A0h]
                    00 00
  0000000140016DC9: FF 15 99 D3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016DCF: 48 8D BC 24 40 0B  lea         rdi,[rsp+0000000000000B40h]
                    00 00
  0000000140016DD7: 48 8B F0           mov         rsi,rax
  0000000140016DDA: B9 10 00 00 00     mov         ecx,10h
  0000000140016DDF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016DE1: 45 33 C0           xor         r8d,r8d
  0000000140016DE4: 33 D2              xor         edx,edx
  0000000140016DE6: 48 8D 8C 24 B0 05  lea         rcx,[rsp+00000000000005B0h]
                    00 00
  0000000140016DEE: FF 15 74 D3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016DF4: 48 8D BC 24 30 0B  lea         rdi,[rsp+0000000000000B30h]
                    00 00
  0000000140016DFC: 48 8B F0           mov         rsi,rax
  0000000140016DFF: B9 10 00 00 00     mov         ecx,10h
  0000000140016E04: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016E06: 45 33 C0           xor         r8d,r8d
  0000000140016E09: 33 D2              xor         edx,edx
  0000000140016E0B: 48 8D 8C 24 C0 05  lea         rcx,[rsp+00000000000005C0h]
                    00 00
  0000000140016E13: FF 15 4F D3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016E19: 48 8D BC 24 20 0B  lea         rdi,[rsp+0000000000000B20h]
                    00 00
  0000000140016E21: 48 8B F0           mov         rsi,rax
  0000000140016E24: B9 10 00 00 00     mov         ecx,10h
  0000000140016E29: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016E2B: 45 33 C0           xor         r8d,r8d
  0000000140016E2E: 33 D2              xor         edx,edx
  0000000140016E30: 48 8D 8C 24 D0 05  lea         rcx,[rsp+00000000000005D0h]
                    00 00
  0000000140016E38: FF 15 2A D3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016E3E: 48 8D BC 24 10 0B  lea         rdi,[rsp+0000000000000B10h]
                    00 00
  0000000140016E46: 48 8B F0           mov         rsi,rax
  0000000140016E49: B9 10 00 00 00     mov         ecx,10h
  0000000140016E4E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016E50: 45 33 C0           xor         r8d,r8d
  0000000140016E53: 33 D2              xor         edx,edx
  0000000140016E55: 48 8D 8C 24 E0 05  lea         rcx,[rsp+00000000000005E0h]
                    00 00
  0000000140016E5D: FF 15 05 D3 DE 01  call        qword ptr [0000000141E04168h]
  0000000140016E63: 48 8D BC 24 00 0B  lea         rdi,[rsp+0000000000000B00h]
                    00 00
  0000000140016E6B: 48 8B F0           mov         rsi,rax
  0000000140016E6E: B9 10 00 00 00     mov         ecx,10h
  0000000140016E73: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140016E75: 48 8D 84 24 90 0B  lea         rax,[rsp+0000000000000B90h]
                    00 00
  0000000140016E7D: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140016E82: 48 8D 84 24 80 0B  lea         rax,[rsp+0000000000000B80h]
                    00 00
  0000000140016E8A: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140016E8F: 48 8D 84 24 70 0B  lea         rax,[rsp+0000000000000B70h]
                    00 00
  0000000140016E97: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140016E9C: 48 8D 84 24 60 0B  lea         rax,[rsp+0000000000000B60h]
                    00 00
  0000000140016EA4: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140016EA9: 48 8D 84 24 50 0B  lea         rax,[rsp+0000000000000B50h]
                    00 00
  0000000140016EB1: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140016EB6: 48 8D 84 24 40 0B  lea         rax,[rsp+0000000000000B40h]
                    00 00
  0000000140016EBE: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140016EC3: 48 8D 84 24 30 0B  lea         rax,[rsp+0000000000000B30h]
                    00 00
  0000000140016ECB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140016ED0: 48 8D 84 24 20 0B  lea         rax,[rsp+0000000000000B20h]
                    00 00
  0000000140016ED8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140016EDD: 4C 8D 8C 24 10 0B  lea         r9,[rsp+0000000000000B10h]
                    00 00
  0000000140016EE5: 4C 8D 84 24 00 0B  lea         r8,[rsp+0000000000000B00h]
                    00 00
  0000000140016EED: 48 8D 15 54 2F 0E  lea         rdx,[00000001420F9E48h]
                    02
  0000000140016EF4: 48 8D 0D 05 F8 D5  lea         rcx,[0000000143D76700h]
                    03
  0000000140016EFB: E8 C0 84 2B 00     call        00000001402CF3C0
  0000000140016F00: 48 8B 8C 24 A0 0C  mov         rcx,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140016F08: E8 43 C8 FF FF     call        0000000140013750
  0000000140016F0D: 48 8B C8           mov         rcx,rax
  0000000140016F10: E8 FB DF 00 00     call        0000000140024F10
  0000000140016F15: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140016F1A: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140016F20: 74 0A              je          0000000140016F2C
  0000000140016F22: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140016F27: E8 B4 B1 32 01     call        00000001413420E0
  0000000140016F2C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140016F31: E8 7A D1 08 00     call        00000001400A40B0
  0000000140016F36: 0F B6 C0           movzx       eax,al
  0000000140016F39: 85 C0              test        eax,eax
  0000000140016F3B: 75 15              jne         0000000140016F52
  0000000140016F3D: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140016F42: E8 89 D1 08 00     call        00000001400A40D0
  0000000140016F47: 0F B6 C0           movzx       eax,al
  0000000140016F4A: 85 C0              test        eax,eax
  0000000140016F4C: 0F 84 9E 00 00 00  je          0000000140016FF0
  0000000140016F52: 48 8B 84 24 A0 0C  mov         rax,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140016F5A: 48 83 B8 C8 01 00  cmp         qword ptr [rax+00000000000001C8h],0
                    00 00
  0000000140016F62: 74 62              je          0000000140016FC6
  0000000140016F64: 33 D2              xor         edx,edx
  0000000140016F66: 48 8D 8C 24 F0 05  lea         rcx,[rsp+00000000000005F0h]
                    00 00
  0000000140016F6E: FF 15 0C D5 DE 01  call        qword ptr [0000000141E04480h]
  0000000140016F74: 90                 nop
  0000000140016F75: 48 8D 94 24 F0 05  lea         rdx,[rsp+00000000000005F0h]
                    00 00
  0000000140016F7D: 48 8B 8C 24 A0 0C  mov         rcx,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140016F85: 48 8B 89 C8 01 00  mov         rcx,qword ptr [rcx+00000000000001C8h]
                    00
  0000000140016F8C: E8 2F 77 CD 00     call        0000000140CEE6C0
  0000000140016F91: 90                 nop
  0000000140016F92: 48 8D 8C 24 F0 05  lea         rcx,[rsp+00000000000005F0h]
                    00 00
  0000000140016F9A: FF 15 40 EA DE 01  call        qword ptr [0000000141E059E0h]
  0000000140016FA0: 48 8B 84 24 A0 0C  mov         rax,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140016FA8: 48 8B 80 C8 01 00  mov         rax,qword ptr [rax+00000000000001C8h]
                    00
  0000000140016FAF: 48 8B 8C 24 A0 0C  mov         rcx,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140016FB7: 48 8B 89 C8 01 00  mov         rcx,qword ptr [rcx+00000000000001C8h]
                    00
  0000000140016FBE: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140016FC1: 33 D2              xor         edx,edx
  0000000140016FC3: FF 50 58           call        qword ptr [rax+58h]
  0000000140016FC6: 48 8B 84 24 A0 0C  mov         rax,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140016FCE: 48 83 B8 50 02 00  cmp         qword ptr [rax+0000000000000250h],0
                    00 00
  0000000140016FD6: 74 16              je          0000000140016FEE
  0000000140016FD8: 33 D2              xor         edx,edx
  0000000140016FDA: 48 8B 8C 24 A0 0C  mov         rcx,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140016FE2: 48 8B 89 50 02 00  mov         rcx,qword ptr [rcx+0000000000000250h]
                    00
  0000000140016FE9: E8 32 35 0D 00     call        00000001400EA520
  0000000140016FEE: EB 60              jmp         0000000140017050
  0000000140016FF0: 48 8B 84 24 A0 0C  mov         rax,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140016FF8: 48 83 B8 C8 01 00  cmp         qword ptr [rax+00000000000001C8h],0
                    00 00
  0000000140017000: 74 26              je          0000000140017028
  0000000140017002: 48 8B 84 24 A0 0C  mov         rax,qword ptr [rsp+0000000000000CA0h]
                    00 00
  000000014001700A: 48 8B 80 C8 01 00  mov         rax,qword ptr [rax+00000000000001C8h]
                    00
  0000000140017011: 48 8B 8C 24 A0 0C  mov         rcx,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140017019: 48 8B 89 C8 01 00  mov         rcx,qword ptr [rcx+00000000000001C8h]
                    00
  0000000140017020: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140017023: B2 01              mov         dl,1
  0000000140017025: FF 50 58           call        qword ptr [rax+58h]
  0000000140017028: 48 8B 84 24 A0 0C  mov         rax,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140017030: 48 83 B8 50 02 00  cmp         qword ptr [rax+0000000000000250h],0
                    00 00
  0000000140017038: 74 16              je          0000000140017050
  000000014001703A: B2 01              mov         dl,1
  000000014001703C: 48 8B 8C 24 A0 0C  mov         rcx,qword ptr [rsp+0000000000000CA0h]
                    00 00
  0000000140017044: 48 8B 89 50 02 00  mov         rcx,qword ptr [rcx+0000000000000250h]
                    00
  000000014001704B: E8 D0 34 0D 00     call        00000001400EA520
  0000000140017050: 45 33 C0           xor         r8d,r8d
  0000000140017053: 33 D2              xor         edx,edx
  0000000140017055: 48 8D 8C 24 F8 05  lea         rcx,[rsp+00000000000005F8h]
                    00 00
  000000014001705D: FF 15 05 D1 DE 01  call        qword ptr [0000000141E04168h]
  0000000140017063: 48 8D BC 24 30 0C  lea         rdi,[rsp+0000000000000C30h]
                    00 00
  000000014001706B: 48 8B F0           mov         rsi,rax
  000000014001706E: B9 10 00 00 00     mov         ecx,10h
  0000000140017073: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140017075: 45 33 C0           xor         r8d,r8d
  0000000140017078: 33 D2              xor         edx,edx
  000000014001707A: 48 8D 8C 24 08 06  lea         rcx,[rsp+0000000000000608h]
                    00 00
  0000000140017082: FF 15 E0 D0 DE 01  call        qword ptr [0000000141E04168h]
  0000000140017088: 48 8D BC 24 20 0C  lea         rdi,[rsp+0000000000000C20h]
                    00 00
  0000000140017090: 48 8B F0           mov         rsi,rax
  0000000140017093: B9 10 00 00 00     mov         ecx,10h
  0000000140017098: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001709A: 45 33 C0           xor         r8d,r8d
  000000014001709D: 33 D2              xor         edx,edx
  000000014001709F: 48 8D 8C 24 18 06  lea         rcx,[rsp+0000000000000618h]
                    00 00
  00000001400170A7: FF 15 BB D0 DE 01  call        qword ptr [0000000141E04168h]
  00000001400170AD: 48 8D BC 24 10 0C  lea         rdi,[rsp+0000000000000C10h]
                    00 00
  00000001400170B5: 48 8B F0           mov         rsi,rax
  00000001400170B8: B9 10 00 00 00     mov         ecx,10h
  00000001400170BD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400170BF: 45 33 C0           xor         r8d,r8d
  00000001400170C2: 33 D2              xor         edx,edx
  00000001400170C4: 48 8D 8C 24 28 06  lea         rcx,[rsp+0000000000000628h]
                    00 00
  00000001400170CC: FF 15 96 D0 DE 01  call        qword ptr [0000000141E04168h]
  00000001400170D2: 48 8D BC 24 00 0C  lea         rdi,[rsp+0000000000000C00h]
                    00 00
  00000001400170DA: 48 8B F0           mov         rsi,rax
  00000001400170DD: B9 10 00 00 00     mov         ecx,10h
  00000001400170E2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400170E4: 45 33 C0           xor         r8d,r8d
  00000001400170E7: 33 D2              xor         edx,edx
  00000001400170E9: 48 8D 8C 24 38 06  lea         rcx,[rsp+0000000000000638h]
                    00 00
  00000001400170F1: FF 15 71 D0 DE 01  call        qword ptr [0000000141E04168h]
  00000001400170F7: 48 8D BC 24 F0 0B  lea         rdi,[rsp+0000000000000BF0h]
                    00 00
  00000001400170FF: 48 8B F0           mov         rsi,rax
  0000000140017102: B9 10 00 00 00     mov         ecx,10h
  0000000140017107: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140017109: 45 33 C0           xor         r8d,r8d
  000000014001710C: 33 D2              xor         edx,edx
  000000014001710E: 48 8D 8C 24 48 06  lea         rcx,[rsp+0000000000000648h]
                    00 00
  0000000140017116: FF 15 4C D0 DE 01  call        qword ptr [0000000141E04168h]
  000000014001711C: 48 8D BC 24 E0 0B  lea         rdi,[rsp+0000000000000BE0h]
                    00 00
  0000000140017124: 48 8B F0           mov         rsi,rax
  0000000140017127: B9 10 00 00 00     mov         ecx,10h
  000000014001712C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001712E: 45 33 C0           xor         r8d,r8d
  0000000140017131: 33 D2              xor         edx,edx
  0000000140017133: 48 8D 8C 24 58 06  lea         rcx,[rsp+0000000000000658h]
                    00 00
  000000014001713B: FF 15 27 D0 DE 01  call        qword ptr [0000000141E04168h]
  0000000140017141: 48 8D BC 24 D0 0B  lea         rdi,[rsp+0000000000000BD0h]
                    00 00
  0000000140017149: 48 8B F0           mov         rsi,rax
  000000014001714C: B9 10 00 00 00     mov         ecx,10h
  0000000140017151: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140017153: 45 33 C0           xor         r8d,r8d
  0000000140017156: 33 D2              xor         edx,edx
  0000000140017158: 48 8D 8C 24 68 06  lea         rcx,[rsp+0000000000000668h]
                    00 00
  0000000140017160: FF 15 02 D0 DE 01  call        qword ptr [0000000141E04168h]
  0000000140017166: 48 8D BC 24 C0 0B  lea         rdi,[rsp+0000000000000BC0h]
                    00 00
  000000014001716E: 48 8B F0           mov         rsi,rax
  0000000140017171: B9 10 00 00 00     mov         ecx,10h
  0000000140017176: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140017178: 45 33 C0           xor         r8d,r8d
  000000014001717B: 33 D2              xor         edx,edx
  000000014001717D: 48 8D 8C 24 78 06  lea         rcx,[rsp+0000000000000678h]
                    00 00
  0000000140017185: FF 15 DD CF DE 01  call        qword ptr [0000000141E04168h]
  000000014001718B: 48 8D BC 24 B0 0B  lea         rdi,[rsp+0000000000000BB0h]
                    00 00
  0000000140017193: 48 8B F0           mov         rsi,rax
  0000000140017196: B9 10 00 00 00     mov         ecx,10h
  000000014001719B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014001719D: 45 33 C0           xor         r8d,r8d
  00000001400171A0: 33 D2              xor         edx,edx
  00000001400171A2: 48 8D 8C 24 88 06  lea         rcx,[rsp+0000000000000688h]
                    00 00
  00000001400171AA: FF 15 B8 CF DE 01  call        qword ptr [0000000141E04168h]
  00000001400171B0: 48 8D BC 24 A0 0B  lea         rdi,[rsp+0000000000000BA0h]
                    00 00
  00000001400171B8: 48 8B F0           mov         rsi,rax
  00000001400171BB: B9 10 00 00 00     mov         ecx,10h
  00000001400171C0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400171C2: 48 8D 84 24 30 0C  lea         rax,[rsp+0000000000000C30h]
                    00 00
  00000001400171CA: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400171CF: 48 8D 84 24 20 0C  lea         rax,[rsp+0000000000000C20h]
                    00 00
  00000001400171D7: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400171DC: 48 8D 84 24 10 0C  lea         rax,[rsp+0000000000000C10h]
                    00 00
  00000001400171E4: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400171E9: 48 8D 84 24 00 0C  lea         rax,[rsp+0000000000000C00h]
                    00 00
  00000001400171F1: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400171F6: 48 8D 84 24 F0 0B  lea         rax,[rsp+0000000000000BF0h]
                    00 00
  00000001400171FE: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140017203: 48 8D 84 24 E0 0B  lea         rax,[rsp+0000000000000BE0h]
                    00 00
  000000014001720B: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140017210: 48 8D 84 24 D0 0B  lea         rax,[rsp+0000000000000BD0h]
                    00 00
  0000000140017218: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014001721D: 48 8D 84 24 C0 0B  lea         rax,[rsp+0000000000000BC0h]
                    00 00
  0000000140017225: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014001722A: 4C 8D 8C 24 B0 0B  lea         r9,[rsp+0000000000000BB0h]
                    00 00
  0000000140017232: 4C 8D 84 24 A0 0B  lea         r8,[rsp+0000000000000BA0h]
                    00 00
  000000014001723A: 48 8D 15 27 2C 0E  lea         rdx,[00000001420F9E68h]
                    02
  0000000140017241: 48 8D 0D B8 F4 D5  lea         rcx,[0000000143D76700h]
                    03
  0000000140017248: E8 73 81 2B 00     call        00000001402CF3C0
  000000014001724D: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140017252: E8 29 53 0A 00     call        00000001400BC580
  0000000140017257: 48 81 C4 88 0C 00  add         rsp,0C88h
                    00
  000000014001725E: 5F                 pop         rdi
  000000014001725F: 5E                 pop         rsi
  0000000140017260: C3                 ret

  Summary

     1E02000 .text

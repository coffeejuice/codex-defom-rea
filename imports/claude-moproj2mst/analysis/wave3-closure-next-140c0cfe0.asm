Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C0CFE0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C0CFE5: 56                 push        rsi
  0000000140C0CFE6: 57                 push        rdi
  0000000140C0CFE7: 48 81 EC C8 01 00  sub         rsp,1C8h
                    00
  0000000140C0CFEE: 48 8D 0D C3 96 16  lea         rcx,[0000000143D766B8h]
                    03
  0000000140C0CFF5: E8 96 B0 6B FF     call        00000001402C8090
  0000000140C0CFFA: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  0000000140C0D002: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140C0D00A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C0D00D: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140C0D015: FF 90 80 07 00 00  call        qword ptr [rax+0000000000000780h]
  0000000140C0D01B: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140C0D020: 48 8D 0D 91 96 16  lea         rcx,[0000000143D766B8h]
                    03
  0000000140C0D027: E8 64 B0 6B FF     call        00000001402C8090
  0000000140C0D02C: 48 89 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],rax
                    00 00
  0000000140C0D034: 48 8B 84 24 B8 01  mov         rax,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140C0D03C: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C0D03F: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140C0D047: FF 90 88 07 00 00  call        qword ptr [rax+0000000000000788h]
  0000000140C0D04D: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140C0D052: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140C0D058: 74 1B              je          0000000140C0D075
  0000000140C0D05A: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C0D062: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C0D065: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140C0D06A: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C0D072: FF 50 70           call        qword ptr [rax+70h]
  0000000140C0D075: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140C0D07B: 74 1B              je          0000000140C0D098
  0000000140C0D07D: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C0D085: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C0D088: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  0000000140C0D08D: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C0D095: FF 50 70           call        qword ptr [rax+70h]
  0000000140C0D098: 45 33 C0           xor         r8d,r8d
  0000000140C0D09B: 33 D2              xor         edx,edx
  0000000140C0D09D: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140C0D0A2: FF 15 C0 70 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0D0A8: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  0000000140C0D0B0: 48 8B F0           mov         rsi,rax
  0000000140C0D0B3: B9 10 00 00 00     mov         ecx,10h
  0000000140C0D0B8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0D0BA: 45 33 C0           xor         r8d,r8d
  0000000140C0D0BD: 33 D2              xor         edx,edx
  0000000140C0D0BF: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140C0D0C7: FF 15 9B 70 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0D0CD: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140C0D0D5: 48 8B F0           mov         rsi,rax
  0000000140C0D0D8: B9 10 00 00 00     mov         ecx,10h
  0000000140C0D0DD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0D0DF: 45 33 C0           xor         r8d,r8d
  0000000140C0D0E2: 33 D2              xor         edx,edx
  0000000140C0D0E4: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140C0D0EC: FF 15 76 70 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0D0F2: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140C0D0FA: 48 8B F0           mov         rsi,rax
  0000000140C0D0FD: B9 10 00 00 00     mov         ecx,10h
  0000000140C0D102: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0D104: 45 33 C0           xor         r8d,r8d
  0000000140C0D107: 33 D2              xor         edx,edx
  0000000140C0D109: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140C0D111: FF 15 51 70 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0D117: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140C0D11F: 48 8B F0           mov         rsi,rax
  0000000140C0D122: B9 10 00 00 00     mov         ecx,10h
  0000000140C0D127: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0D129: 45 33 C0           xor         r8d,r8d
  0000000140C0D12C: 33 D2              xor         edx,edx
  0000000140C0D12E: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140C0D136: FF 15 2C 70 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0D13C: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  0000000140C0D144: 48 8B F0           mov         rsi,rax
  0000000140C0D147: B9 10 00 00 00     mov         ecx,10h
  0000000140C0D14C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0D14E: 45 33 C0           xor         r8d,r8d
  0000000140C0D151: 33 D2              xor         edx,edx
  0000000140C0D153: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140C0D15B: FF 15 07 70 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0D161: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  0000000140C0D169: 48 8B F0           mov         rsi,rax
  0000000140C0D16C: B9 10 00 00 00     mov         ecx,10h
  0000000140C0D171: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0D173: 45 33 C0           xor         r8d,r8d
  0000000140C0D176: 33 D2              xor         edx,edx
  0000000140C0D178: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140C0D180: FF 15 E2 6F 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0D186: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  0000000140C0D18E: 48 8B F0           mov         rsi,rax
  0000000140C0D191: B9 10 00 00 00     mov         ecx,10h
  0000000140C0D196: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0D198: 45 33 C0           xor         r8d,r8d
  0000000140C0D19B: 33 D2              xor         edx,edx
  0000000140C0D19D: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140C0D1A5: FF 15 BD 6F 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0D1AB: 48 8D BC 24 30 01  lea         rdi,[rsp+0000000000000130h]
                    00 00
  0000000140C0D1B3: 48 8B F0           mov         rsi,rax
  0000000140C0D1B6: B9 10 00 00 00     mov         ecx,10h
  0000000140C0D1BB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0D1BD: 45 33 C0           xor         r8d,r8d
  0000000140C0D1C0: 33 D2              xor         edx,edx
  0000000140C0D1C2: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000140C0D1CA: FF 15 98 6F 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0D1D0: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  0000000140C0D1D8: 48 8B F0           mov         rsi,rax
  0000000140C0D1DB: B9 10 00 00 00     mov         ecx,10h
  0000000140C0D1E0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0D1E2: 45 33 C0           xor         r8d,r8d
  0000000140C0D1E5: 33 D2              xor         edx,edx
  0000000140C0D1E7: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140C0D1EF: FF 15 73 6F 1F 01  call        qword ptr [0000000141E04168h]
  0000000140C0D1F5: 48 8D BC 24 10 01  lea         rdi,[rsp+0000000000000110h]
                    00 00
  0000000140C0D1FD: 48 8B F0           mov         rsi,rax
  0000000140C0D200: B9 10 00 00 00     mov         ecx,10h
  0000000140C0D205: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C0D207: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140C0D20F: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C0D214: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140C0D21C: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C0D221: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  0000000140C0D229: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140C0D22E: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  0000000140C0D236: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C0D23B: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  0000000140C0D243: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C0D248: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  0000000140C0D250: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C0D255: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  0000000140C0D25D: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C0D262: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  0000000140C0D26A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C0D26F: 4C 8D 8C 24 20 01  lea         r9,[rsp+0000000000000120h]
                    00 00
  0000000140C0D277: 4C 8D 84 24 10 01  lea         r8,[rsp+0000000000000110h]
                    00 00
  0000000140C0D27F: 48 8D 15 FA ED C0  lea         rdx,[000000014281C080h]
                    01
  0000000140C0D286: 48 8D 0D 73 94 16  lea         rcx,[0000000143D76700h]
                    03
  0000000140C0D28D: E8 2E 21 6C FF     call        00000001402CF3C0
  0000000140C0D292: 48 81 C4 C8 01 00  add         rsp,1C8h
                    00
  0000000140C0D299: 5F                 pop         rdi
  0000000140C0D29A: 5E                 pop         rsi
  0000000140C0D29B: C3                 ret

  Summary

     1E02000 .text

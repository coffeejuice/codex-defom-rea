Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140038950: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140038954: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140038959: 56                 push        rsi
  000000014003895A: 57                 push        rdi
  000000014003895B: 48 81 EC B8 01 00  sub         rsp,1B8h
                    00
  0000000140038962: C6 44 24 68 00     mov         byte ptr [rsp+68h],0
  0000000140038967: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  000000014003896F: E8 5C 4E FE FF     call        000000014001D7D0
  0000000140038974: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140038979: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  000000014003897F: 0F 84 3D 02 00 00  je          0000000140038BC2
  0000000140038985: 8B 94 24 D8 01 00  mov         edx,dword ptr [rsp+00000000000001D8h]
                    00
  000000014003898C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140038991: E8 EA F8 00 00     call        0000000140048280
  0000000140038996: 88 44 24 68        mov         byte ptr [rsp+68h],al
  000000014003899A: 0F B6 44 24 68     movzx       eax,byte ptr [rsp+68h]
  000000014003899F: 85 C0              test        eax,eax
  00000001400389A1: 0F 84 1B 02 00 00  je          0000000140038BC2
  00000001400389A7: 45 33 C0           xor         r8d,r8d
  00000001400389AA: 33 D2              xor         edx,edx
  00000001400389AC: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001400389B1: FF 15 B1 B7 DC 01  call        qword ptr [0000000141E04168h]
  00000001400389B7: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  00000001400389BF: 48 8B F0           mov         rsi,rax
  00000001400389C2: B9 10 00 00 00     mov         ecx,10h
  00000001400389C7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400389C9: 45 33 C0           xor         r8d,r8d
  00000001400389CC: 33 D2              xor         edx,edx
  00000001400389CE: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000001400389D6: FF 15 8C B7 DC 01  call        qword ptr [0000000141E04168h]
  00000001400389DC: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  00000001400389E4: 48 8B F0           mov         rsi,rax
  00000001400389E7: B9 10 00 00 00     mov         ecx,10h
  00000001400389EC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400389EE: 45 33 C0           xor         r8d,r8d
  00000001400389F1: 33 D2              xor         edx,edx
  00000001400389F3: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400389FB: FF 15 67 B7 DC 01  call        qword ptr [0000000141E04168h]
  0000000140038A01: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140038A09: 48 8B F0           mov         rsi,rax
  0000000140038A0C: B9 10 00 00 00     mov         ecx,10h
  0000000140038A11: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140038A13: 45 33 C0           xor         r8d,r8d
  0000000140038A16: 33 D2              xor         edx,edx
  0000000140038A18: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140038A20: FF 15 42 B7 DC 01  call        qword ptr [0000000141E04168h]
  0000000140038A26: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140038A2E: 48 8B F0           mov         rsi,rax
  0000000140038A31: B9 10 00 00 00     mov         ecx,10h
  0000000140038A36: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140038A38: 45 33 C0           xor         r8d,r8d
  0000000140038A3B: 33 D2              xor         edx,edx
  0000000140038A3D: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140038A45: FF 15 1D B7 DC 01  call        qword ptr [0000000141E04168h]
  0000000140038A4B: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  0000000140038A53: 48 8B F0           mov         rsi,rax
  0000000140038A56: B9 10 00 00 00     mov         ecx,10h
  0000000140038A5B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140038A5D: 45 33 C0           xor         r8d,r8d
  0000000140038A60: 33 D2              xor         edx,edx
  0000000140038A62: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140038A6A: FF 15 F8 B6 DC 01  call        qword ptr [0000000141E04168h]
  0000000140038A70: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  0000000140038A78: 48 8B F0           mov         rsi,rax
  0000000140038A7B: B9 10 00 00 00     mov         ecx,10h
  0000000140038A80: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140038A82: 45 33 C0           xor         r8d,r8d
  0000000140038A85: 33 D2              xor         edx,edx
  0000000140038A87: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140038A8F: FF 15 D3 B6 DC 01  call        qword ptr [0000000141E04168h]
  0000000140038A95: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  0000000140038A9D: 48 8B F0           mov         rsi,rax
  0000000140038AA0: B9 10 00 00 00     mov         ecx,10h
  0000000140038AA5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140038AA7: 45 33 C0           xor         r8d,r8d
  0000000140038AAA: 33 D2              xor         edx,edx
  0000000140038AAC: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140038AB4: FF 15 AE B6 DC 01  call        qword ptr [0000000141E04168h]
  0000000140038ABA: 48 8D BC 24 30 01  lea         rdi,[rsp+0000000000000130h]
                    00 00
  0000000140038AC2: 48 8B F0           mov         rsi,rax
  0000000140038AC5: B9 10 00 00 00     mov         ecx,10h
  0000000140038ACA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140038ACC: 45 33 C0           xor         r8d,r8d
  0000000140038ACF: 33 D2              xor         edx,edx
  0000000140038AD1: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000140038AD9: FF 15 89 B6 DC 01  call        qword ptr [0000000141E04168h]
  0000000140038ADF: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  0000000140038AE7: 48 8B F0           mov         rsi,rax
  0000000140038AEA: B9 10 00 00 00     mov         ecx,10h
  0000000140038AEF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140038AF1: 45 33 C0           xor         r8d,r8d
  0000000140038AF4: 33 D2              xor         edx,edx
  0000000140038AF6: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140038AFE: FF 15 64 B6 DC 01  call        qword ptr [0000000141E04168h]
  0000000140038B04: 48 8D BC 24 10 01  lea         rdi,[rsp+0000000000000110h]
                    00 00
  0000000140038B0C: 48 8B F0           mov         rsi,rax
  0000000140038B0F: B9 10 00 00 00     mov         ecx,10h
  0000000140038B14: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140038B16: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140038B1E: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140038B23: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140038B2B: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140038B30: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  0000000140038B38: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140038B3D: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  0000000140038B45: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140038B4A: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  0000000140038B52: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140038B57: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  0000000140038B5F: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140038B64: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  0000000140038B6C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140038B71: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  0000000140038B79: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140038B7E: 4C 8D 8C 24 20 01  lea         r9,[rsp+0000000000000120h]
                    00 00
  0000000140038B86: 4C 8D 84 24 10 01  lea         r8,[rsp+0000000000000110h]
                    00 00
  0000000140038B8E: 48 8D 15 8B CB 0C  lea         rdx,[0000000142105720h]
                    02
  0000000140038B95: 48 8D 0D 64 DB D3  lea         rcx,[0000000143D76700h]
                    03
  0000000140038B9C: E8 1F 68 29 00     call        00000001402CF3C0
  0000000140038BA1: 48 8B 84 24 D0 01  mov         rax,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140038BA9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140038BAC: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140038BB4: FF 90 A0 02 00 00  call        qword ptr [rax+00000000000002A0h]
  0000000140038BBA: 48 8B C8           mov         rcx,rax
  0000000140038BBD: E8 3E 27 D0 00     call        0000000140D3B300
  0000000140038BC2: 8A 44 24 68        mov         al,byte ptr [rsp+68h]
  0000000140038BC6: 48 81 C4 B8 01 00  add         rsp,1B8h
                    00
  0000000140038BCD: 5F                 pop         rdi
  0000000140038BCE: 5E                 pop         rsi
  0000000140038BCF: C3                 ret

  Summary

     1E02000 .text

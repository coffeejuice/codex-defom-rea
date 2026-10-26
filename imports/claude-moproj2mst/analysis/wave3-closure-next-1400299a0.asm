Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400299A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400299A5: 56                 push        rsi
  00000001400299A6: 57                 push        rdi
  00000001400299A7: 48 81 EC F8 01 00  sub         rsp,1F8h
                    00
  00000001400299AE: 48 C7 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400299BA: C6 44 24 60 01     mov         byte ptr [rsp+60h],1
  00000001400299BF: 0F B6 54 24 60     movzx       edx,byte ptr [rsp+60h]
  00000001400299C4: 48 8D 0D ED CC D4  lea         rcx,[0000000143D766B8h]
                    03
  00000001400299CB: E8 50 0F 2A 00     call        00000001402CA920
  00000001400299D0: 0F B6 C0           movzx       eax,al
  00000001400299D3: 85 C0              test        eax,eax
  00000001400299D5: 74 07              je          00000001400299DE
  00000001400299D7: 32 C0              xor         al,al
  00000001400299D9: E9 1C 03 00 00     jmp         0000000140029CFA
  00000001400299DE: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001400299E6: E8 D5 1E C7 00     call        0000000140C9B8C0
  00000001400299EB: 0F B6 C0           movzx       eax,al
  00000001400299EE: 85 C0              test        eax,eax
  00000001400299F0: 75 59              jne         0000000140029A4B
  00000001400299F2: 45 33 C0           xor         r8d,r8d
  00000001400299F5: 48 8D 15 94 66 0D  lea         rdx,[0000000142100090h]
                    02
  00000001400299FC: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140029A01: E8 7A B3 FF FF     call        0000000140024D80
  0000000140029A06: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  0000000140029A0E: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000140029A16: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
  0000000140029A1E: 45 33 C9           xor         r9d,r9d
  0000000140029A21: 45 33 C0           xor         r8d,r8d
  0000000140029A24: 48 8B 94 24 D0 01  mov         rdx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140029A2C: 48 8D 0D 1D C9 D4  lea         rcx,[0000000143D76350h]
                    03
  0000000140029A33: E8 98 71 2A 00     call        00000001402D0BD0
  0000000140029A38: 90                 nop
  0000000140029A39: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140029A3E: FF 15 9C BF DD 01  call        qword ptr [0000000141E059E0h]
  0000000140029A44: 32 C0              xor         al,al
  0000000140029A46: E9 AF 02 00 00     jmp         0000000140029CFA
  0000000140029A4B: B2 01              mov         dl,1
  0000000140029A4D: 48 8D 0D 64 CC D4  lea         rcx,[0000000143D766B8h]
                    03
  0000000140029A54: E8 97 0F 2A 00     call        00000001402CA9F0
  0000000140029A59: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  0000000140029A61: E8 7A 10 00 00     call        000000014002AAE0
  0000000140029A66: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  0000000140029A6E: E8 2D 99 FE FF     call        00000001400133A0
  0000000140029A73: 48 8B C8           mov         rcx,rax
  0000000140029A76: E8 85 E8 C4 00     call        0000000140C78300
  0000000140029A7B: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  0000000140029A83: E8 88 3A FF FF     call        000000014001D510
  0000000140029A88: 48 8B D0           mov         rdx,rax
  0000000140029A8B: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  0000000140029A93: E8 48 06 00 00     call        000000014002A0E0
  0000000140029A98: 45 33 C0           xor         r8d,r8d
  0000000140029A9B: 33 D2              xor         edx,edx
  0000000140029A9D: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140029AA2: FF 15 C0 A6 DD 01  call        qword ptr [0000000141E04168h]
  0000000140029AA8: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  0000000140029AB0: 48 8B F0           mov         rsi,rax
  0000000140029AB3: B9 10 00 00 00     mov         ecx,10h
  0000000140029AB8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140029ABA: 45 33 C0           xor         r8d,r8d
  0000000140029ABD: 33 D2              xor         edx,edx
  0000000140029ABF: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140029AC7: FF 15 9B A6 DD 01  call        qword ptr [0000000141E04168h]
  0000000140029ACD: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  0000000140029AD5: 48 8B F0           mov         rsi,rax
  0000000140029AD8: B9 10 00 00 00     mov         ecx,10h
  0000000140029ADD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140029ADF: 45 33 C0           xor         r8d,r8d
  0000000140029AE2: 33 D2              xor         edx,edx
  0000000140029AE4: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140029AEC: FF 15 76 A6 DD 01  call        qword ptr [0000000141E04168h]
  0000000140029AF2: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140029AFA: 48 8B F0           mov         rsi,rax
  0000000140029AFD: B9 10 00 00 00     mov         ecx,10h
  0000000140029B02: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140029B04: 45 33 C0           xor         r8d,r8d
  0000000140029B07: 33 D2              xor         edx,edx
  0000000140029B09: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140029B11: FF 15 51 A6 DD 01  call        qword ptr [0000000141E04168h]
  0000000140029B17: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140029B1F: 48 8B F0           mov         rsi,rax
  0000000140029B22: B9 10 00 00 00     mov         ecx,10h
  0000000140029B27: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140029B29: 45 33 C0           xor         r8d,r8d
  0000000140029B2C: 33 D2              xor         edx,edx
  0000000140029B2E: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140029B36: FF 15 2C A6 DD 01  call        qword ptr [0000000141E04168h]
  0000000140029B3C: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140029B44: 48 8B F0           mov         rsi,rax
  0000000140029B47: B9 10 00 00 00     mov         ecx,10h
  0000000140029B4C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140029B4E: 45 33 C0           xor         r8d,r8d
  0000000140029B51: 33 D2              xor         edx,edx
  0000000140029B53: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140029B5B: FF 15 07 A6 DD 01  call        qword ptr [0000000141E04168h]
  0000000140029B61: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  0000000140029B69: 48 8B F0           mov         rsi,rax
  0000000140029B6C: B9 10 00 00 00     mov         ecx,10h
  0000000140029B71: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140029B73: 45 33 C0           xor         r8d,r8d
  0000000140029B76: 33 D2              xor         edx,edx
  0000000140029B78: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140029B80: FF 15 E2 A5 DD 01  call        qword ptr [0000000141E04168h]
  0000000140029B86: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  0000000140029B8E: 48 8B F0           mov         rsi,rax
  0000000140029B91: B9 10 00 00 00     mov         ecx,10h
  0000000140029B96: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140029B98: 45 33 C0           xor         r8d,r8d
  0000000140029B9B: 33 D2              xor         edx,edx
  0000000140029B9D: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140029BA5: FF 15 BD A5 DD 01  call        qword ptr [0000000141E04168h]
  0000000140029BAB: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  0000000140029BB3: 48 8B F0           mov         rsi,rax
  0000000140029BB6: B9 10 00 00 00     mov         ecx,10h
  0000000140029BBB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140029BBD: 45 33 C0           xor         r8d,r8d
  0000000140029BC0: 33 D2              xor         edx,edx
  0000000140029BC2: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000140029BCA: FF 15 98 A5 DD 01  call        qword ptr [0000000141E04168h]
  0000000140029BD0: 48 8D BC 24 30 01  lea         rdi,[rsp+0000000000000130h]
                    00 00
  0000000140029BD8: 48 8B F0           mov         rsi,rax
  0000000140029BDB: B9 10 00 00 00     mov         ecx,10h
  0000000140029BE0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140029BE2: 45 33 C0           xor         r8d,r8d
  0000000140029BE5: 33 D2              xor         edx,edx
  0000000140029BE7: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140029BEF: FF 15 73 A5 DD 01  call        qword ptr [0000000141E04168h]
  0000000140029BF5: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  0000000140029BFD: 48 8B F0           mov         rsi,rax
  0000000140029C00: B9 10 00 00 00     mov         ecx,10h
  0000000140029C05: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140029C07: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  0000000140029C0F: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140029C14: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140029C1C: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140029C21: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140029C29: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140029C2E: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  0000000140029C36: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140029C3B: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  0000000140029C43: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140029C48: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  0000000140029C50: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140029C55: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  0000000140029C5D: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140029C62: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  0000000140029C6A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140029C6F: 4C 8D 8C 24 30 01  lea         r9,[rsp+0000000000000130h]
                    00 00
  0000000140029C77: 4C 8D 84 24 20 01  lea         r8,[rsp+0000000000000120h]
                    00 00
  0000000140029C7F: 48 8D 15 2A 64 0D  lea         rdx,[00000001421000B0h]
                    02
  0000000140029C86: 48 8D 0D 73 CA D4  lea         rcx,[0000000143D76700h]
                    03
  0000000140029C8D: E8 2E 57 2A 00     call        00000001402CF3C0
  0000000140029C92: 33 D2              xor         edx,edx
  0000000140029C94: 48 8D 0D 1D CA D4  lea         rcx,[0000000143D766B8h]
                    03
  0000000140029C9B: E8 50 0D 2A 00     call        00000001402CA9F0
  0000000140029CA0: 45 33 C0           xor         r8d,r8d
  0000000140029CA3: 48 8D 15 2E 64 0D  lea         rdx,[00000001421000D8h]
                    02
  0000000140029CAA: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000140029CB2: E8 C9 B0 FF FF     call        0000000140024D80
  0000000140029CB7: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  0000000140029CBF: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140029CC7: 48 89 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],rax
                    00 00
  0000000140029CCF: 45 33 C9           xor         r9d,r9d
  0000000140029CD2: 45 33 C0           xor         r8d,r8d
  0000000140029CD5: 48 8B 94 24 E0 01  mov         rdx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140029CDD: 48 8D 0D 6C C6 D4  lea         rcx,[0000000143D76350h]
                    03
  0000000140029CE4: E8 E7 6E 2A 00     call        00000001402D0BD0
  0000000140029CE9: 90                 nop
  0000000140029CEA: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000140029CF2: FF 15 E8 BC DD 01  call        qword ptr [0000000141E059E0h]
  0000000140029CF8: B0 01              mov         al,1
  0000000140029CFA: 48 81 C4 F8 01 00  add         rsp,1F8h
                    00
  0000000140029D01: 5F                 pop         rdi
  0000000140029D02: 5E                 pop         rsi
  0000000140029D03: C3                 ret

  Summary

     1E02000 .text

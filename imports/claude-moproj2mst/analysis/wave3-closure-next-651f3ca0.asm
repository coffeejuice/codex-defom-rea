Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000651F3CA0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000000651F3CA5: 55                 push        rbp
  00000000651F3CA6: 56                 push        rsi
  00000000651F3CA7: 57                 push        rdi
  00000000651F3CA8: 41 54              push        r12
  00000000651F3CAA: 41 55              push        r13
  00000000651F3CAC: 48 83 EC 40        sub         rsp,40h
  00000000651F3CB0: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000651F3CB9: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  00000000651F3CBE: 4D 8B E0           mov         r12,r8
  00000000651F3CC1: 48 8B FA           mov         rdi,rdx
  00000000651F3CC4: 48 8B F1           mov         rsi,rcx
  00000000651F3CC7: 33 ED              xor         ebp,ebp
  00000000651F3CC9: 89 AC 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],ebp
                    00
  00000000651F3CD0: 49 8B D0           mov         rdx,r8
  00000000651F3CD3: E8 68 E8 FF FF     call        00000000651F2540
  00000000651F3CD8: 84 C0              test        al,al
  00000000651F3CDA: 0F 84 81 00 00 00  je          00000000651F3D61
  00000000651F3CE0: 8B 46 08           mov         eax,dword ptr [rsi+8]
  00000000651F3CE3: 41 3B 44 24 08     cmp         eax,dword ptr [r12+8]
  00000000651F3CE8: 74 04              je          00000000651F3CEE
  00000000651F3CEA: 85 C0              test        eax,eax
  00000000651F3CEC: 75 73              jne         00000000651F3D61
  00000000651F3CEE: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000651F3CF1: 49 8B 1C 24        mov         rbx,qword ptr [r12]
  00000000651F3CF5: 8B 43 38           mov         eax,dword ptr [rbx+38h]
  00000000651F3CF8: 39 41 38           cmp         dword ptr [rcx+38h],eax
  00000000651F3CFB: 75 64              jne         00000000651F3D61
  00000000651F3CFD: 48 89 5C 24 30     mov         qword ptr [rsp+30h],rbx
  00000000651F3D02: 48 85 DB           test        rbx,rbx
  00000000651F3D05: 74 0A              je          00000000651F3D11
  00000000651F3D07: 48 8B CB           mov         rcx,rbx
  00000000651F3D0A: FF 15 B0 2B 4A 00  call        qword ptr [00000000656968C0h]
  00000000651F3D10: 90                 nop
  00000000651F3D11: 8B 76 08           mov         esi,dword ptr [rsi+8]
  00000000651F3D14: 89 74 24 38        mov         dword ptr [rsp+38h],esi
  00000000651F3D18: 48 89 1F           mov         qword ptr [rdi],rbx
  00000000651F3D1B: 48 85 DB           test        rbx,rbx
  00000000651F3D1E: 74 09              je          00000000651F3D29
  00000000651F3D20: 48 8B CB           mov         rcx,rbx
  00000000651F3D23: FF 15 97 2B 4A 00  call        qword ptr [00000000656968C0h]
  00000000651F3D29: 89 77 08           mov         dword ptr [rdi+8],esi
  00000000651F3D2C: C7 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],1
                    00 01 00 00 00
  00000000651F3D37: 48 85 DB           test        rbx,rbx
  00000000651F3D3A: 74 1D              je          00000000651F3D59
  00000000651F3D3C: 48 8B CB           mov         rcx,rbx
  00000000651F3D3F: FF 15 33 2C 4A 00  call        qword ptr [0000000065696978h]
  00000000651F3D45: 84 C0              test        al,al
  00000000651F3D47: 75 10              jne         00000000651F3D59
  00000000651F3D49: 48 8B CB           mov         rcx,rbx
  00000000651F3D4C: E8 EF E2 FF FF     call        00000000651F2040
  00000000651F3D51: 48 8B CB           mov         rcx,rbx
  00000000651F3D54: E8 55 D3 3E 00     call        00000000655E10AE
  00000000651F3D59: 48 8B C7           mov         rax,rdi
  00000000651F3D5C: E9 19 01 00 00     jmp         00000000651F3E7A
  00000000651F3D61: 48 8B 1E           mov         rbx,qword ptr [rsi]
  00000000651F3D64: 48 89 5C 24 30     mov         qword ptr [rsp+30h],rbx
  00000000651F3D69: 48 85 DB           test        rbx,rbx
  00000000651F3D6C: 74 09              je          00000000651F3D77
  00000000651F3D6E: 48 8B CB           mov         rcx,rbx
  00000000651F3D71: FF 15 49 2B 4A 00  call        qword ptr [00000000656968C0h]
  00000000651F3D77: 44 8B 6E 08        mov         r13d,dword ptr [rsi+8]
  00000000651F3D7B: 44 89 6C 24 38     mov         dword ptr [rsp+38h],r13d
  00000000651F3D80: BA 01 00 00 00     mov         edx,1
  00000000651F3D85: 48 8B CB           mov         rcx,rbx
  00000000651F3D88: FF 15 C2 2D 4A 00  call        qword ptr [0000000065696B50h]
  00000000651F3D8E: 84 C0              test        al,al
  00000000651F3D90: 74 2D              je          00000000651F3DBF
  00000000651F3D92: 48 8B 4B 30        mov         rcx,qword ptr [rbx+30h]
  00000000651F3D96: 48 85 C9           test        rcx,rcx
  00000000651F3D99: 74 06              je          00000000651F3DA1
  00000000651F3D9B: FF 15 D7 2B 4A 00  call        qword ptr [0000000065696978h]
  00000000651F3DA1: 48 89 6B 30        mov         qword ptr [rbx+30h],rbp
  00000000651F3DA5: 48 8B 4B 58        mov         rcx,qword ptr [rbx+58h]
  00000000651F3DA9: 48 85 C9           test        rcx,rcx
  00000000651F3DAC: 74 0B              je          00000000651F3DB9
  00000000651F3DAE: 48 3B CB           cmp         rcx,rbx
  00000000651F3DB1: 74 06              je          00000000651F3DB9
  00000000651F3DB3: FF 15 BF 2B 4A 00  call        qword ptr [0000000065696978h]
  00000000651F3DB9: 48 89 6B 58        mov         qword ptr [rbx+58h],rbp
  00000000651F3DBD: EB 67              jmp         00000000651F3E26
  00000000651F3DBF: 48 85 DB           test        rbx,rbx
  00000000651F3DC2: 74 62              je          00000000651F3E26
  00000000651F3DC4: BA 01 00 00 00     mov         edx,1
  00000000651F3DC9: 48 8B CB           mov         rcx,rbx
  00000000651F3DCC: FF 15 9E 2C 4A 00  call        qword ptr [0000000065696A70h]
  00000000651F3DD2: 84 C0              test        al,al
  00000000651F3DD4: 74 50              je          00000000651F3E26
  00000000651F3DD6: B9 60 00 00 00     mov         ecx,60h
  00000000651F3DDB: E8 92 D3 3E 00     call        00000000655E1172
  00000000651F3DE0: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000000651F3DE5: 48 85 C0           test        rax,rax
  00000000651F3DE8: 74 0E              je          00000000651F3DF8
  00000000651F3DEA: 48 8B D3           mov         rdx,rbx
  00000000651F3DED: 48 8B C8           mov         rcx,rax
  00000000651F3DF0: E8 5B E1 FF FF     call        00000000651F1F50
  00000000651F3DF5: 48 8B E8           mov         rbp,rax
  00000000651F3DF8: 48 8B CD           mov         rcx,rbp
  00000000651F3DFB: FF 15 BF 2A 4A 00  call        qword ptr [00000000656968C0h]
  00000000651F3E01: 48 8B CB           mov         rcx,rbx
  00000000651F3E04: FF 15 6E 2B 4A 00  call        qword ptr [0000000065696978h]
  00000000651F3E0A: 84 C0              test        al,al
  00000000651F3E0C: 75 10              jne         00000000651F3E1E
  00000000651F3E0E: 48 8B CB           mov         rcx,rbx
  00000000651F3E11: E8 2A E2 FF FF     call        00000000651F2040
  00000000651F3E16: 48 8B CB           mov         rcx,rbx
  00000000651F3E19: E8 90 D2 3E 00     call        00000000655E10AE
  00000000651F3E1E: 48 8B DD           mov         rbx,rbp
  00000000651F3E21: 48 89 5C 24 30     mov         qword ptr [rsp+30h],rbx
  00000000651F3E26: 4D 8B 04 24        mov         r8,qword ptr [r12]
  00000000651F3E2A: 8B 56 08           mov         edx,dword ptr [rsi+8]
  00000000651F3E2D: 48 8B CB           mov         rcx,rbx
  00000000651F3E30: E8 DB E2 FF FF     call        00000000651F2110
  00000000651F3E35: 48 89 1F           mov         qword ptr [rdi],rbx
  00000000651F3E38: 48 85 DB           test        rbx,rbx
  00000000651F3E3B: 74 09              je          00000000651F3E46
  00000000651F3E3D: 48 8B CB           mov         rcx,rbx
  00000000651F3E40: FF 15 7A 2A 4A 00  call        qword ptr [00000000656968C0h]
  00000000651F3E46: 44 89 6F 08        mov         dword ptr [rdi+8],r13d
  00000000651F3E4A: C7 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],1
                    00 01 00 00 00
  00000000651F3E55: 48 85 DB           test        rbx,rbx
  00000000651F3E58: 74 1D              je          00000000651F3E77
  00000000651F3E5A: 48 8B CB           mov         rcx,rbx
  00000000651F3E5D: FF 15 15 2B 4A 00  call        qword ptr [0000000065696978h]
  00000000651F3E63: 84 C0              test        al,al
  00000000651F3E65: 75 10              jne         00000000651F3E77
  00000000651F3E67: 48 8B CB           mov         rcx,rbx
  00000000651F3E6A: E8 D1 E1 FF FF     call        00000000651F2040
  00000000651F3E6F: 48 8B CB           mov         rcx,rbx
  00000000651F3E72: E8 37 D2 3E 00     call        00000000655E10AE
  00000000651F3E77: 48 8B C7           mov         rax,rdi
  00000000651F3E7A: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  00000000651F3E7F: 48 83 C4 40        add         rsp,40h
  00000000651F3E83: 41 5D              pop         r13
  00000000651F3E85: 41 5C              pop         r12
  00000000651F3E87: 5F                 pop         rdi
  00000000651F3E88: 5E                 pop         rsi
  00000000651F3E89: 5D                 pop         rbp
  00000000651F3E8A: C3                 ret
  00000000651F3E8B: CC                 int         3

  Summary

      695000 .text

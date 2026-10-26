Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001416B7BD0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001416B7BD5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001416B7BDA: 57                 push        rdi
  00000001416B7BDB: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  00000001416B7BE2: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001416B7BEB: 48 8D 44 24 30     lea         rax,[rsp+30h]
  00000001416B7BF0: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001416B7BF5: 33 D2              xor         edx,edx
  00000001416B7BF7: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001416B7BFC: E8 6F F0 E8 FE     call        0000000140546C70
  00000001416B7C01: 4C 8B C0           mov         r8,rax
  00000001416B7C04: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001416B7C0C: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7C14: FF 15 66 3E 75 00  call        qword ptr [0000000141E0BA80h]
  00000001416B7C1A: 90                 nop
  00000001416B7C1B: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7C23: 48 8D 05 EE 51 90  lea         rax,[0000000142FBCE18h]
                    01
  00000001416B7C2A: 48 89 01           mov         qword ptr [rcx],rax
  00000001416B7C2D: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7C35: 48 8D 05 A4 51 90  lea         rax,[0000000142FBCDE0h]
                    01
  00000001416B7C3C: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001416B7C40: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7C48: 48 83 C1 30        add         rcx,30h
  00000001416B7C4C: E8 8F 7D AD FE     call        000000014018F9E0
  00000001416B7C51: 90                 nop
  00000001416B7C52: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7C5A: C7 40 54 FF FF FF  mov         dword ptr [rax+54h],0FFFFFFFFh
                    FF
  00000001416B7C61: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7C69: C7 40 58 FF FF FF  mov         dword ptr [rax+58h],0FFFFFFFFh
                    FF
  00000001416B7C70: 48 C7 44 24 40 A8  mov         qword ptr [rsp+40h],0A8h
                    00 00 00
  00000001416B7C79: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001416B7C7E: E8 39 6E DB FF     call        000000014146EABC
  00000001416B7C83: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001416B7C88: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  00000001416B7C8E: 74 1A              je          00000001416B7CAA
  00000001416B7C90: 48 8B 7C 24 48     mov         rdi,qword ptr [rsp+48h]
  00000001416B7C95: 33 C0              xor         eax,eax
  00000001416B7C97: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001416B7C9C: F3 AA              rep stos    byte ptr [rdi]
  00000001416B7C9E: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001416B7CA3: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001416B7CA8: EB 09              jmp         00000001416B7CB3
  00000001416B7CAA: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  00000001416B7CB3: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7CBB: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  00000001416B7CC0: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  00000001416B7CC4: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7CCC: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7CD4: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001416B7CD8: E8 83 42 00 00     call        00000001416BBF60
  00000001416B7CDD: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7CE5: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001416B7CE9: BA 01 00 00 00     mov         edx,1
  00000001416B7CEE: 48 8B 49 78        mov         rcx,qword ptr [rcx+78h]
  00000001416B7CF2: FF 15 90 F3 74 00  call        qword ptr [0000000141E07088h]
  00000001416B7CF8: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7D00: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001416B7D04: BA 02 00 00 00     mov         edx,2
  00000001416B7D09: 48 8B 49 78        mov         rcx,qword ptr [rcx+78h]
  00000001416B7D0D: FF 15 05 3B 75 00  call        qword ptr [0000000141E0B818h]
  00000001416B7D13: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7D1B: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001416B7D1F: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  00000001416B7D26: FF 15 84 3E 75 00  call        qword ptr [0000000141E0BBB0h]
  00000001416B7D2C: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001416B7D34: 4C 8D 0D 45 15 EB  lea         r9,[0000000143569280h]
                    01
  00000001416B7D3B: 4C 8D 05 16 F8 EA  lea         r8,[0000000143567558h]
                    01
  00000001416B7D42: 33 D2              xor         edx,edx
  00000001416B7D44: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001416B7D4C: E8 AB 7B DB FF     call        000000014146F8FC
  00000001416B7D51: 48 8B D0           mov         rdx,rax
  00000001416B7D54: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7D5C: 48 89 50 48        mov         qword ptr [rax+48h],rdx
  00000001416B7D60: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7D68: 48 C7 40 40 00 00  mov         qword ptr [rax+40h],0
                    00 00
  00000001416B7D70: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7D78: E8 33 31 00 00     call        00000001416BAEB0
  00000001416B7D7D: B9 38 00 00 00     mov         ecx,38h
  00000001416B7D82: E8 35 6D DB FF     call        000000014146EABC
  00000001416B7D87: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001416B7D8C: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  00000001416B7D92: 74 39              je          00000001416B7DCD
  00000001416B7D94: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  00000001416B7D9C: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7DA4: 44 8B 48 58        mov         r9d,dword ptr [rax+58h]
  00000001416B7DA8: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7DB0: 44 8B 40 54        mov         r8d,dword ptr [rax+54h]
  00000001416B7DB4: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7DBC: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001416B7DC1: E8 DA 41 FD FF     call        000000014168BFA0
  00000001416B7DC6: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001416B7DCB: EB 09              jmp         00000001416B7DD6
  00000001416B7DCD: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  00000001416B7DD6: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  00000001416B7DDB: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001416B7DE0: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7DE8: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001416B7DED: 48 89 41 38        mov         qword ptr [rcx+38h],rax
  00000001416B7DF1: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7DF9: C6 40 60 00        mov         byte ptr [rax+60h],0
  00000001416B7DFD: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7E05: C7 40 5C FF FF FF  mov         dword ptr [rax+5Ch],0FFFFFFFFh
                    FF
  00000001416B7E0C: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7E14: E8 27 1C 00 00     call        00000001416B9A40
  00000001416B7E19: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7E21: E8 7A 27 00 00     call        00000001416BA5A0
  00000001416B7E26: 90                 nop
  00000001416B7E27: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001416B7E2F: 48 81 C4 80 00 00  add         rsp,80h
                    00
  00000001416B7E36: 5F                 pop         rdi
  00000001416B7E37: C3                 ret

  Summary

     1E02000 .text

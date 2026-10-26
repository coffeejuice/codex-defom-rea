Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140304EF0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140304EF5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140304EFA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140304EFF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140304F04: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  0000000140304F0B: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140304F14: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140304F1C: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  0000000140304F21: 74 07              je          0000000140304F2A
  0000000140304F23: 32 C0              xor         al,al
  0000000140304F25: E9 D7 01 00 00     jmp         0000000140305101
  0000000140304F2A: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140304F32: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140304F3A: 48 89 41 28        mov         qword ptr [rcx+28h],rax
  0000000140304F3E: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140304F46: 48 83 C1 30        add         rcx,30h
  0000000140304F4A: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140304F52: FF 15 18 F5 AF 01  call        qword ptr [0000000141E04470h]
  0000000140304F58: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140304F60: 48 83 C1 38        add         rcx,38h
  0000000140304F64: E8 67 7F DA FF     call        00000001400ACED0
  0000000140304F69: 48 83 BC 24 A8 00  cmp         qword ptr [rsp+00000000000000A8h],0
                    00 00 00
  0000000140304F72: 74 19              je          0000000140304F8D
  0000000140304F74: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140304F7C: 48 83 C1 38        add         rcx,38h
  0000000140304F80: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140304F88: E8 A3 24 00 00     call        0000000140307430
  0000000140304F8D: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140304F95: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140304F9D: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140304FA1: FF 15 61 06 B0 01  call        qword ptr [0000000141E05608h]
  0000000140304FA7: B9 10 00 00 00     mov         ecx,10h
  0000000140304FAC: E8 0B 9B 16 01     call        000000014146EABC
  0000000140304FB1: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140304FB6: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140304FBC: 74 2E              je          0000000140304FEC
  0000000140304FBE: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140304FC6: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140304FCB: FF 15 4F 70 B0 01  call        qword ptr [0000000141E0C020h]
  0000000140304FD1: 4C 8B 5C 24 38     mov         r11,qword ptr [rsp+38h]
  0000000140304FD6: 48 8D 05 3B FC FF  lea         rax,[0000000142304C18h]
                    01
  0000000140304FDD: 49 89 03           mov         qword ptr [r11],rax
  0000000140304FE0: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140304FE5: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140304FEA: EB 09              jmp         0000000140304FF5
  0000000140304FEC: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  0000000140304FF5: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  0000000140304FFA: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140304FFF: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140305007: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014030500C: 48 89 41 40        mov         qword ptr [rcx+40h],rax
  0000000140305010: BA 04 00 00 00     mov         edx,4
  0000000140305015: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000014030501A: FF 15 F8 6F B0 01  call        qword ptr [0000000141E0C018h]
  0000000140305020: 90                 nop
  0000000140305021: 45 33 C0           xor         r8d,r8d
  0000000140305024: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140305029: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140305031: 48 8B 49 40        mov         rcx,qword ptr [rcx+40h]
  0000000140305035: FF 15 3D 70 B0 01  call        qword ptr [0000000141E0C078h]
  000000014030503B: 0F B6 C0           movzx       eax,al
  000000014030503E: 85 C0              test        eax,eax
  0000000140305040: 75 0A              jne         000000014030504C
  0000000140305042: C7 44 24 70 01 00  mov         dword ptr [rsp+70h],1
                    00 00
  000000014030504A: EB 08              jmp         0000000140305054
  000000014030504C: C7 44 24 70 00 00  mov         dword ptr [rsp+70h],0
                    00 00
  0000000140305054: 0F B6 44 24 70     movzx       eax,byte ptr [rsp+70h]
  0000000140305059: 88 44 24 40        mov         byte ptr [rsp+40h],al
  000000014030505D: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140305062: FF 15 08 70 B0 01  call        qword ptr [0000000141E0C070h]
  0000000140305068: 44 0F B6 5C 24 40  movzx       r11d,byte ptr [rsp+40h]
  000000014030506E: 45 85 DB           test        r11d,r11d
  0000000140305071: 74 5C              je          00000001403050CF
  0000000140305073: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014030507B: 48 8B 40 40        mov         rax,qword ptr [rax+40h]
  000000014030507F: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140305084: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140305089: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014030508E: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  0000000140305094: 74 1C              je          00000001403050B2
  0000000140305096: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014030509B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014030509E: BA 01 00 00 00     mov         edx,1
  00000001403050A3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001403050A8: FF 50 18           call        qword ptr [rax+18h]
  00000001403050AB: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001403050B0: EB 09              jmp         00000001403050BB
  00000001403050B2: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  00000001403050BB: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001403050C3: 48 C7 40 40 00 00  mov         qword ptr [rax+40h],0
                    00 00
  00000001403050CB: 32 C0              xor         al,al
  00000001403050CD: EB 32              jmp         0000000140305101
  00000001403050CF: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001403050D7: 4C 8D 0D F2 F3 FF  lea         r9,[00000001423044D0h]
                    01
  00000001403050DE: 4C 8B 84 24 90 00  mov         r8,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001403050E6: 48 8D 15 FB F3 FF  lea         rdx,[00000001423044E8h]
                    01
  00000001403050ED: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001403050F5: 48 8B 49 40        mov         rcx,qword ptr [rcx+40h]
  00000001403050F9: FF 15 D1 F0 AF 01  call        qword ptr [0000000141E041D0h]
  00000001403050FF: B0 01              mov         al,1
  0000000140305101: 48 81 C4 88 00 00  add         rsp,88h
                    00
  0000000140305108: C3                 ret

  Summary

     1E02000 .text

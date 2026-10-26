Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C2F50: 40 53              push        rbx
  00000000670C2F52: 55                 push        rbp
  00000000670C2F53: 56                 push        rsi
  00000000670C2F54: 57                 push        rdi
  00000000670C2F55: 41 54              push        r12
  00000000670C2F57: 48 83 EC 40        sub         rsp,40h
  00000000670C2F5B: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C2F64: 48 8B F1           mov         rsi,rcx
  00000000670C2F67: 48 83 79 28 00     cmp         qword ptr [rcx+28h],0
  00000000670C2F6C: 0F 85 61 01 00 00  jne         00000000670C30D3
  00000000670C2F72: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000670C2F75: 48 85 C9           test        rcx,rcx
  00000000670C2F78: 0F 84 55 01 00 00  je          00000000670C30D3
  00000000670C2F7E: 83 BE CC 00 00 00  cmp         dword ptr [rsi+00000000000000CCh],0
                    00
  00000000670C2F85: 0F 85 48 01 00 00  jne         00000000670C30D3
  00000000670C2F8B: 48 8B 86 90 00 00  mov         rax,qword ptr [rsi+0000000000000090h]
                    00
  00000000670C2F92: 83 78 08 00        cmp         dword ptr [rax+8],0
  00000000670C2F96: 0F 84 37 01 00 00  je          00000000670C30D3
  00000000670C2F9C: E8 2F 74 FF FF     call        00000000670BA3D0
  00000000670C2FA1: 44 0F B6 E0        movzx       r12d,al
  00000000670C2FA5: 84 C0              test        al,al
  00000000670C2FA7: 74 3E              je          00000000670C2FE7
  00000000670C2FA9: 33 D2              xor         edx,edx
  00000000670C2FAB: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000670C2FAE: E8 8D 92 FF FF     call        00000000670BC240
  00000000670C2FB3: 4C 8D 1D D6 7E 16  lea         r11,[000000006722AE90h]
                    00
  00000000670C2FBA: 4C 89 9C 24 80 00  mov         qword ptr [rsp+0000000000000080h],r11
                    00 00
  00000000670C2FC2: B8 0A 00 00 00     mov         eax,0Ah
  00000000670C2FC7: 66 89 44 24 70     mov         word ptr [rsp+70h],ax
  00000000670C2FCC: 44 8D 48 F7        lea         r9d,[rax-9]
  00000000670C2FD0: 4C 8D 84 24 80 00  lea         r8,[rsp+0000000000000080h]
                    00 00
  00000000670C2FD8: 0F B7 D0           movzx       edx,ax
  00000000670C2FDB: 48 8D 8E 90 00 00  lea         rcx,[rsi+0000000000000090h]
                    00
  00000000670C2FE2: E8 49 0C FB FF     call        0000000067073C30
  00000000670C2FE7: 48 83 7E 38 00     cmp         qword ptr [rsi+38h],0
  00000000670C2FEC: 75 09              jne         00000000670C2FF7
  00000000670C2FEE: E8 7D AF 08 00     call        000000006714DF70
  00000000670C2FF3: 48 89 46 38        mov         qword ptr [rsi+38h],rax
  00000000670C2FF7: 48 8B 86 90 00 00  mov         rax,qword ptr [rsi+0000000000000090h]
                    00
  00000000670C2FFE: 8B 78 08           mov         edi,dword ptr [rax+8]
  00000000670C3001: 48 8D 5E 60        lea         rbx,[rsi+60h]
  00000000670C3005: 48 8D 8E 90 00 00  lea         rcx,[rsi+0000000000000090h]
                    00
  00000000670C300C: E8 FF 29 0B 00     call        0000000067175A10
  00000000670C3011: 4C 8B C0           mov         r8,rax
  00000000670C3014: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  00000000670C3019: 44 8B CF           mov         r9d,edi
  00000000670C301C: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  00000000670C3021: 48 8B 4E 38        mov         rcx,qword ptr [rsi+38h]
  00000000670C3025: E8 E6 A3 FA FF     call        000000006706D410
  00000000670C302A: 90                 nop
  00000000670C302B: 48 8D 8E 90 00 00  lea         rcx,[rsi+0000000000000090h]
                    00
  00000000670C3032: E8 19 E6 F5 FF     call        0000000067021650
  00000000670C3037: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000000670C303C: 4C 63 40 08        movsxd      r8,dword ptr [rax+8]
  00000000670C3040: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  00000000670C3044: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000670C3047: E8 04 87 FF FF     call        00000000670BB750
  00000000670C304C: 48 8B D8           mov         rbx,rax
  00000000670C304F: 48 85 C0           test        rax,rax
  00000000670C3052: 7F 21              jg          00000000670C3075
  00000000670C3054: C7 86 CC 00 00 00  mov         dword ptr [rsi+00000000000000CCh],3
                    03 00 00 00
  00000000670C305E: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000000670C3063: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670C3067: 75 6A              jne         00000000670C30D3
  00000000670C3069: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000000670C306E: E8 3D D3 F5 FF     call        00000000670203B0
  00000000670C3073: EB 5E              jmp         00000000670C30D3
  00000000670C3075: 45 84 E4           test        r12b,r12b
  00000000670C3078: 74 0A              je          00000000670C3084
  00000000670C307A: B2 01              mov         dl,1
  00000000670C307C: 48 8B 0E           mov         rcx,qword ptr [rsi]
  00000000670C307F: E8 BC 91 FF FF     call        00000000670BC240
  00000000670C3084: 48 8B 16           mov         rdx,qword ptr [rsi]
  00000000670C3087: 48 8D 0D 82 32 19  lea         rcx,[0000000067256310h]
                    00
  00000000670C308E: E8 3D AD 05 00     call        000000006711DDD0
  00000000670C3093: 48 85 C0           test        rax,rax
  00000000670C3096: 74 13              je          00000000670C30AB
  00000000670C3098: 48 8B C8           mov         rcx,rax
  00000000670C309B: E8 60 2D FF FF     call        00000000670B5E00
  00000000670C30A0: 84 C0              test        al,al
  00000000670C30A2: 75 07              jne         00000000670C30AB
  00000000670C30A4: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000000670C30A9: EB 0E              jmp         00000000670C30B9
  00000000670C30AB: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000000670C30B0: 48 63 48 08        movsxd      rcx,dword ptr [rax+8]
  00000000670C30B4: 48 3B D9           cmp         rbx,rcx
  00000000670C30B7: 74 0A              je          00000000670C30C3
  00000000670C30B9: C7 86 CC 00 00 00  mov         dword ptr [rsi+00000000000000CCh],3
                    03 00 00 00
  00000000670C30C3: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670C30C7: 75 0A              jne         00000000670C30D3
  00000000670C30C9: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000000670C30CE: E8 DD D2 F5 FF     call        00000000670203B0
  00000000670C30D3: 48 83 C4 40        add         rsp,40h
  00000000670C30D7: 41 5C              pop         r12
  00000000670C30D9: 5F                 pop         rdi
  00000000670C30DA: 5E                 pop         rsi
  00000000670C30DB: 5D                 pop         rbp
  00000000670C30DC: 5B                 pop         rbx
  00000000670C30DD: C3                 ret

  Summary

      1A7000 .text

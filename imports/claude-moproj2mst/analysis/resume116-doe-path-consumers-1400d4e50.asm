Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D4E50: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001400D4E55: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001400D4E5A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400D4E5F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D4E64: 56                 push        rsi
  00000001400D4E65: 57                 push        rdi
  00000001400D4E66: 48 81 EC 18 02 00  sub         rsp,218h
                    00
  00000001400D4E6D: 48 C7 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400D4E79: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D4E81: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D4E88: E8 53 FC F2 FF     call        0000000140004AE0
  00000001400D4E8D: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  00000001400D4E91: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000001400D4E99: EB 0B              jmp         00000001400D4EA6
  00000001400D4E9B: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  00000001400D4E9F: 83 C0 01           add         eax,1
  00000001400D4EA2: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  00000001400D4EA6: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  00000001400D4EAA: 39 44 24 30        cmp         dword ptr [rsp+30h],eax
  00000001400D4EAE: 0F 8D CD 09 00 00  jge         00000001400D5881
  00000001400D4EB4: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D4EBC: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D4EC3: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D4EC7: E8 44 BD 08 00     call        0000000140160C10
  00000001400D4ECC: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D4ECF: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D4ED2: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001400D4ED7: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  00000001400D4EDF: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400D4EE4: E8 D7 F8 22 00     call        00000001403047C0
  00000001400D4EE9: 90                 nop
  00000001400D4EEA: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001400D4EEF: FF 15 CB F5 D2 01  call        qword ptr [0000000141E044C0h]
  00000001400D4EF5: 90                 nop
  00000001400D4EF6: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400D4EFE: FF 15 BC F5 D2 01  call        qword ptr [0000000141E044C0h]
  00000001400D4F04: 90                 nop
  00000001400D4F05: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400D4F0A: FF 15 B0 F5 D2 01  call        qword ptr [0000000141E044C0h]
  00000001400D4F10: 90                 nop
  00000001400D4F11: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D4F16: FF 15 A4 F5 D2 01  call        qword ptr [0000000141E044C0h]
  00000001400D4F1C: 90                 nop
  00000001400D4F1D: 4C 8D 8C 24 90 00  lea         r9,[rsp+0000000000000090h]
                    00 00
  00000001400D4F25: 4C 8D 44 24 78     lea         r8,[rsp+78h]
  00000001400D4F2A: 33 D2              xor         edx,edx
  00000001400D4F2C: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400D4F31: E8 0A 2A 74 00     call        0000000140817940
  00000001400D4F36: 33 D2              xor         edx,edx
  00000001400D4F38: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400D4F3D: E8 9E 37 74 00     call        00000001408186E0
  00000001400D4F42: 48 8B 8C 24 38 02  mov         rcx,qword ptr [rsp+0000000000000238h]
                    00 00
  00000001400D4F4A: E8 11 18 F9 FF     call        0000000140066760
  00000001400D4F4F: 8B D0              mov         edx,eax
  00000001400D4F51: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400D4F56: E8 35 29 74 00     call        0000000140817890
  00000001400D4F5B: 48 8D 84 24 B8 00  lea         rax,[rsp+00000000000000B8h]
                    00 00
  00000001400D4F63: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  00000001400D4F6B: 48 8D 84 24 C8 00  lea         rax,[rsp+00000000000000C8h]
                    00 00
  00000001400D4F73: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  00000001400D4F7B: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  00000001400D4F83: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000001400D4F8B: FF 15 47 0A D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D4F91: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  00000001400D4F99: 48 8B 84 24 98 01  mov         rax,qword ptr [rsp+0000000000000198h]
                    00 00
  00000001400D4FA1: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  00000001400D4FA9: 48 8B 94 24 48 02  mov         rdx,qword ptr [rsp+0000000000000248h]
                    00 00
  00000001400D4FB1: 48 81 C2 28 08 00  add         rdx,828h
                    00
  00000001400D4FB8: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400D4FC0: FF 15 BA F4 D2 01  call        qword ptr [0000000141E04480h]
  00000001400D4FC6: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  00000001400D4FCE: 48 8B 84 24 A0 01  mov         rax,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001400D4FD6: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400D4FDB: 4C 8B 8C 24 A8 01  mov         r9,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001400D4FE3: 44 8B 84 24 40 02  mov         r8d,dword ptr [rsp+0000000000000240h]
                    00 00
  00000001400D4FEB: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001400D4FF0: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D4FF8: E8 73 09 00 00     call        00000001400D5970
  00000001400D4FFD: 90                 nop
  00000001400D4FFE: 33 D2              xor         edx,edx
  00000001400D5000: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001400D5005: E8 C6 E7 81 00     call        00000001408F37D0
  00000001400D500A: 90                 nop
  00000001400D500B: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  00000001400D5013: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001400D5018: FF 15 32 EF D2 01  call        qword ptr [0000000141E03F50h]
  00000001400D501E: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  00000001400D5026: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001400D502E: 48 89 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],rax
                    00 00
  00000001400D5036: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  00000001400D503E: FF 15 14 EF D2 01  call        qword ptr [0000000141E03F58h]
  00000001400D5044: 45 33 C9           xor         r9d,r9d
  00000001400D5047: 4C 8B 84 24 48 02  mov         r8,qword ptr [rsp+0000000000000248h]
                    00 00
  00000001400D504F: 48 8B D0           mov         rdx,rax
  00000001400D5052: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001400D5057: E8 04 F7 81 00     call        00000001408F4760
  00000001400D505C: 88 84 24 80 00 00  mov         byte ptr [rsp+0000000000000080h],al
                    00
  00000001400D5063: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  00000001400D506B: FF 15 1F F2 D2 01  call        qword ptr [0000000141E04290h]
  00000001400D5071: 33 D2              xor         edx,edx
  00000001400D5073: 48 8B 8C 24 48 02  mov         rcx,qword ptr [rsp+0000000000000248h]
                    00 00
  00000001400D507B: E8 60 70 79 00     call        000000014086C0E0
  00000001400D5080: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001400D5085: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D508D: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D5094: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D5098: E8 73 BB 08 00     call        0000000140160C10
  00000001400D509D: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D50A0: 48 83 78 08 00     cmp         qword ptr [rax+8],0
  00000001400D50A5: 0F 84 32 03 00 00  je          00000001400D53DD
  00000001400D50AB: 48 8B 84 24 30 02  mov         rax,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D50B3: 48 83 B8 B0 00 00  cmp         qword ptr [rax+00000000000000B0h],0
                    00 00
  00000001400D50BB: 0F 84 1C 03 00 00  je          00000001400D53DD
  00000001400D50C1: C6 84 24 98 00 00  mov         byte ptr [rsp+0000000000000098h],1
                    00 01
  00000001400D50C9: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  00000001400D50CF: 74 63              je          00000001400D5134
  00000001400D50D1: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D50D9: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D50E0: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D50E4: E8 27 BB 08 00     call        0000000140160C10
  00000001400D50E9: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D50EC: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001400D50F0: E8 CB 2F 00 00     call        00000001400D80C0
  00000001400D50F5: 0F B6 C0           movzx       eax,al
  00000001400D50F8: 85 C0              test        eax,eax
  00000001400D50FA: 74 30              je          00000001400D512C
  00000001400D50FC: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D5104: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D510B: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D510F: E8 FC BA 08 00     call        0000000140160C10
  00000001400D5114: 48 8B 10           mov         rdx,qword ptr [rax]
  00000001400D5117: 48 8B 52 08        mov         rdx,qword ptr [rdx+8]
  00000001400D511B: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400D5120: E8 4B 93 81 00     call        00000001408EE470
  00000001400D5125: 0F B6 C0           movzx       eax,al
  00000001400D5128: 85 C0              test        eax,eax
  00000001400D512A: 74 08              je          00000001400D5134
  00000001400D512C: C6 84 24 98 00 00  mov         byte ptr [rsp+0000000000000098h],0
                    00 00
  00000001400D5134: 44 0F B6 84 24 98  movzx       r8d,byte ptr [rsp+0000000000000098h]
                    00 00 00
  00000001400D513D: 33 D2              xor         edx,edx
  00000001400D513F: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400D5144: E8 F7 2D 74 00     call        0000000140817F40
  00000001400D5149: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  00000001400D5151: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001400D5156: FF 15 F4 ED D2 01  call        qword ptr [0000000141E03F50h]
  00000001400D515C: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  00000001400D5164: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001400D516C: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  00000001400D5174: 48 8B 8C 24 C8 01  mov         rcx,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000001400D517C: FF 15 D6 ED D2 01  call        qword ptr [0000000141E03F58h]
  00000001400D5182: 48 8B F8           mov         rdi,rax
  00000001400D5185: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D518D: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D5194: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D5198: E8 73 BA 08 00     call        0000000140160C10
  00000001400D519D: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D51A0: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  00000001400D51A9: 41 B9 02 00 00 00  mov         r9d,2
  00000001400D51AF: 41 B8 02 00 00 00  mov         r8d,2
  00000001400D51B5: 48 8B D7           mov         rdx,rdi
  00000001400D51B8: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001400D51BC: E8 1F 96 81 00     call        00000001408EE7E0
  00000001400D51C1: 89 84 24 9C 00 00  mov         dword ptr [rsp+000000000000009Ch],eax
                    00
  00000001400D51C8: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  00000001400D51D0: FF 15 BA F0 D2 01  call        qword ptr [0000000141E04290h]
  00000001400D51D6: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D51DE: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D51E5: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D51E9: E8 22 BA 08 00     call        0000000140160C10
  00000001400D51EE: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D51F1: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  00000001400D51F5: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000001400D51FD: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400D5205: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  00000001400D520D: 48 83 BC 24 E8 00  cmp         qword ptr [rsp+00000000000000E8h],0
                    00 00 00
  00000001400D5216: 74 24              je          00000001400D523C
  00000001400D5218: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001400D5220: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D5223: BA 01 00 00 00     mov         edx,1
  00000001400D5228: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001400D5230: FF 10              call        qword ptr [rax]
  00000001400D5232: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
  00000001400D523A: EB 0C              jmp         00000001400D5248
  00000001400D523C: 48 C7 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],0
                    00 00 00 00 00 00
  00000001400D5248: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D5250: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D5257: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D525B: E8 B0 B9 08 00     call        0000000140160C10
  00000001400D5260: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D5263: 48 83 C1 18        add         rcx,18h
  00000001400D5267: E8 A4 21 23 00     call        0000000140307410
  00000001400D526C: 0F B6 C0           movzx       eax,al
  00000001400D526F: 85 C0              test        eax,eax
  00000001400D5271: 0F 85 66 01 00 00  jne         00000001400D53DD
  00000001400D5277: C7 84 24 A4 00 00  mov         dword ptr [rsp+00000000000000A4h],0
                    00 00 00 00 00
  00000001400D5282: EB 11              jmp         00000001400D5295
  00000001400D5284: 8B 84 24 A4 00 00  mov         eax,dword ptr [rsp+00000000000000A4h]
                    00
  00000001400D528B: 83 C0 01           add         eax,1
  00000001400D528E: 89 84 24 A4 00 00  mov         dword ptr [rsp+00000000000000A4h],eax
                    00
  00000001400D5295: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D529D: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D52A4: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D52A8: E8 63 B9 08 00     call        0000000140160C10
  00000001400D52AD: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D52B0: 48 83 C1 18        add         rcx,18h
  00000001400D52B4: E8 27 F8 F2 FF     call        0000000140004AE0
  00000001400D52B9: 39 84 24 A4 00 00  cmp         dword ptr [rsp+00000000000000A4h],eax
                    00
  00000001400D52C0: 0F 8D 9C 00 00 00  jge         00000001400D5362
  00000001400D52C6: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D52CE: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D52D5: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D52D9: E8 32 B9 08 00     call        0000000140160C10
  00000001400D52DE: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D52E1: 48 83 C1 18        add         rcx,18h
  00000001400D52E5: 8B 94 24 A4 00 00  mov         edx,dword ptr [rsp+00000000000000A4h]
                    00
  00000001400D52EC: E8 8F 2F 00 00     call        00000001400D8280
  00000001400D52F1: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  00000001400D52F9: 48 8D 70 10        lea         rsi,[rax+10h]
  00000001400D52FD: B9 10 00 00 00     mov         ecx,10h
  00000001400D5302: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D5304: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D530C: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D5313: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D5317: E8 F4 B8 08 00     call        0000000140160C10
  00000001400D531C: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D531F: 48 83 C1 18        add         rcx,18h
  00000001400D5323: 8B 94 24 A4 00 00  mov         edx,dword ptr [rsp+00000000000000A4h]
                    00
  00000001400D532A: E8 51 2F 00 00     call        00000001400D8280
  00000001400D532F: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  00000001400D5337: 48 8B F0           mov         rsi,rax
  00000001400D533A: B9 10 00 00 00     mov         ecx,10h
  00000001400D533F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D5341: 4C 8D 8C 24 60 01  lea         r9,[rsp+0000000000000160h]
                    00 00
  00000001400D5349: 4C 8D 84 24 50 01  lea         r8,[rsp+0000000000000150h]
                    00 00
  00000001400D5351: 33 D2              xor         edx,edx
  00000001400D5353: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400D5358: E8 A3 2C 74 00     call        0000000140818000
  00000001400D535D: E9 22 FF FF FF     jmp         00000001400D5284
  00000001400D5362: 48 8D 15 9F 18 09  lea         rdx,[0000000142166C08h]
                    02
  00000001400D5369: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  00000001400D5371: FF 15 09 F1 D2 01  call        qword ptr [0000000141E04480h]
  00000001400D5377: 90                 nop
  00000001400D5378: 45 33 C9           xor         r9d,r9d
  00000001400D537B: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001400D5381: 48 8D 94 24 F8 00  lea         rdx,[rsp+00000000000000F8h]
                    00 00
  00000001400D5389: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001400D538E: FF 15 0C F3 D2 01  call        qword ptr [0000000141E046A0h]
  00000001400D5394: 89 84 24 A0 00 00  mov         dword ptr [rsp+00000000000000A0h],eax
                    00
  00000001400D539B: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  00000001400D53A3: FF 15 37 06 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D53A9: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  00000001400D53AE: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400D53B3: FF 15 B7 F0 D2 01  call        qword ptr [0000000141E04470h]
  00000001400D53B9: 8B 94 24 A0 00 00  mov         edx,dword ptr [rsp+00000000000000A0h]
                    00
  00000001400D53C0: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400D53C5: FF 15 DD F2 D2 01  call        qword ptr [0000000141E046A8h]
  00000001400D53CB: 48 8D 15 3E 18 09  lea         rdx,[0000000142166C10h]
                    02
  00000001400D53D2: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400D53D7: FF 15 EB F0 D2 01  call        qword ptr [0000000141E044C8h]
  00000001400D53DD: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D53E5: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D53EC: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D53F0: E8 1B B8 08 00     call        0000000140160C10
  00000001400D53F5: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D53F8: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  00000001400D53FD: 0F 84 3B 03 00 00  je          00000001400D573E
  00000001400D5403: 48 8B 84 24 30 02  mov         rax,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D540B: 48 83 B8 B0 00 00  cmp         qword ptr [rax+00000000000000B0h],0
                    00 00
  00000001400D5413: 0F 84 25 03 00 00  je          00000001400D573E
  00000001400D5419: C6 84 24 A8 00 00  mov         byte ptr [rsp+00000000000000A8h],1
                    00 01
  00000001400D5421: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  00000001400D5427: 74 63              je          00000001400D548C
  00000001400D5429: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D5431: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D5438: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D543C: E8 CF B7 08 00     call        0000000140160C10
  00000001400D5441: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D5444: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000001400D5448: E8 73 2C 00 00     call        00000001400D80C0
  00000001400D544D: 0F B6 C0           movzx       eax,al
  00000001400D5450: 85 C0              test        eax,eax
  00000001400D5452: 74 30              je          00000001400D5484
  00000001400D5454: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D545C: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D5463: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D5467: E8 A4 B7 08 00     call        0000000140160C10
  00000001400D546C: 48 8B 10           mov         rdx,qword ptr [rax]
  00000001400D546F: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  00000001400D5473: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400D5478: E8 F3 8F 81 00     call        00000001408EE470
  00000001400D547D: 0F B6 C0           movzx       eax,al
  00000001400D5480: 85 C0              test        eax,eax
  00000001400D5482: 74 08              je          00000001400D548C
  00000001400D5484: C6 84 24 A8 00 00  mov         byte ptr [rsp+00000000000000A8h],0
                    00 00
  00000001400D548C: 44 0F B6 84 24 A8  movzx       r8d,byte ptr [rsp+00000000000000A8h]
                    00 00 00
  00000001400D5495: 33 D2              xor         edx,edx
  00000001400D5497: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400D549C: E8 FF 2A 74 00     call        0000000140817FA0
  00000001400D54A1: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  00000001400D54A9: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400D54B1: FF 15 99 EA D2 01  call        qword ptr [0000000141E03F50h]
  00000001400D54B7: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  00000001400D54BF: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001400D54C7: 48 89 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],rax
                    00 00
  00000001400D54CF: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001400D54D7: FF 15 7B EA D2 01  call        qword ptr [0000000141E03F58h]
  00000001400D54DD: 48 8B F8           mov         rdi,rax
  00000001400D54E0: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D54E8: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D54EF: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D54F3: E8 18 B7 08 00     call        0000000140160C10
  00000001400D54F8: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D54FB: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  00000001400D5504: 41 B9 02 00 00 00  mov         r9d,2
  00000001400D550A: 41 B8 02 00 00 00  mov         r8d,2
  00000001400D5510: 48 8B D7           mov         rdx,rdi
  00000001400D5513: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000001400D5517: E8 C4 92 81 00     call        00000001408EE7E0
  00000001400D551C: 89 84 24 AC 00 00  mov         dword ptr [rsp+00000000000000ACh],eax
                    00
  00000001400D5523: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  00000001400D552B: FF 15 5F ED D2 01  call        qword ptr [0000000141E04290h]
  00000001400D5531: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D5539: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D5540: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D5544: E8 C7 B6 08 00     call        0000000140160C10
  00000001400D5549: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D554C: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001400D5550: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  00000001400D5558: 48 8B 84 24 10 01  mov         rax,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001400D5560: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  00000001400D5568: 48 83 BC 24 08 01  cmp         qword ptr [rsp+0000000000000108h],0
                    00 00 00
  00000001400D5571: 74 24              je          00000001400D5597
  00000001400D5573: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D557B: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D557E: BA 01 00 00 00     mov         edx,1
  00000001400D5583: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400D558B: FF 10              call        qword ptr [rax]
  00000001400D558D: 48 89 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],rax
                    00 00
  00000001400D5595: EB 0C              jmp         00000001400D55A3
  00000001400D5597: 48 C7 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],0
                    00 00 00 00 00 00
  00000001400D55A3: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D55AB: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D55B2: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D55B6: E8 55 B6 08 00     call        0000000140160C10
  00000001400D55BB: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D55BE: 48 83 C1 20        add         rcx,20h
  00000001400D55C2: E8 49 1E 23 00     call        0000000140307410
  00000001400D55C7: 0F B6 C0           movzx       eax,al
  00000001400D55CA: 85 C0              test        eax,eax
  00000001400D55CC: 0F 85 6C 01 00 00  jne         00000001400D573E
  00000001400D55D2: C7 84 24 B4 00 00  mov         dword ptr [rsp+00000000000000B4h],0
                    00 00 00 00 00
  00000001400D55DD: EB 11              jmp         00000001400D55F0
  00000001400D55DF: 8B 84 24 B4 00 00  mov         eax,dword ptr [rsp+00000000000000B4h]
                    00
  00000001400D55E6: 83 C0 01           add         eax,1
  00000001400D55E9: 89 84 24 B4 00 00  mov         dword ptr [rsp+00000000000000B4h],eax
                    00
  00000001400D55F0: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D55F8: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D55FF: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D5603: E8 08 B6 08 00     call        0000000140160C10
  00000001400D5608: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D560B: 48 83 C1 20        add         rcx,20h
  00000001400D560F: E8 CC F4 F2 FF     call        0000000140004AE0
  00000001400D5614: 39 84 24 B4 00 00  cmp         dword ptr [rsp+00000000000000B4h],eax
                    00
  00000001400D561B: 0F 8D 9C 00 00 00  jge         00000001400D56BD
  00000001400D5621: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D5629: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D5630: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D5634: E8 D7 B5 08 00     call        0000000140160C10
  00000001400D5639: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D563C: 48 83 C1 20        add         rcx,20h
  00000001400D5640: 8B 94 24 B4 00 00  mov         edx,dword ptr [rsp+00000000000000B4h]
                    00
  00000001400D5647: E8 34 2C 00 00     call        00000001400D8280
  00000001400D564C: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  00000001400D5654: 48 8D 70 10        lea         rsi,[rax+10h]
  00000001400D5658: B9 10 00 00 00     mov         ecx,10h
  00000001400D565D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D565F: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D5667: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D566E: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D5672: E8 99 B5 08 00     call        0000000140160C10
  00000001400D5677: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001400D567A: 48 83 C1 20        add         rcx,20h
  00000001400D567E: 8B 94 24 B4 00 00  mov         edx,dword ptr [rsp+00000000000000B4h]
                    00
  00000001400D5685: E8 F6 2B 00 00     call        00000001400D8280
  00000001400D568A: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  00000001400D5692: 48 8B F0           mov         rsi,rax
  00000001400D5695: B9 10 00 00 00     mov         ecx,10h
  00000001400D569A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D569C: 4C 8D 8C 24 80 01  lea         r9,[rsp+0000000000000180h]
                    00 00
  00000001400D56A4: 4C 8D 84 24 70 01  lea         r8,[rsp+0000000000000170h]
                    00 00
  00000001400D56AC: 33 D2              xor         edx,edx
  00000001400D56AE: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400D56B3: E8 C8 29 74 00     call        0000000140818080
  00000001400D56B8: E9 22 FF FF FF     jmp         00000001400D55DF
  00000001400D56BD: 48 8D 15 54 15 09  lea         rdx,[0000000142166C18h]
                    02
  00000001400D56C4: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  00000001400D56CC: FF 15 AE ED D2 01  call        qword ptr [0000000141E04480h]
  00000001400D56D2: 90                 nop
  00000001400D56D3: 45 33 C9           xor         r9d,r9d
  00000001400D56D6: 41 B8 FF FF FF FF  mov         r8d,0FFFFFFFFh
  00000001400D56DC: 48 8D 94 24 18 01  lea         rdx,[rsp+0000000000000118h]
                    00 00
  00000001400D56E4: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400D56EC: FF 15 AE EF D2 01  call        qword ptr [0000000141E046A0h]
  00000001400D56F2: 89 84 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],eax
                    00
  00000001400D56F9: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  00000001400D5701: FF 15 D9 02 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D5707: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  00000001400D570F: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D5714: FF 15 56 ED D2 01  call        qword ptr [0000000141E04470h]
  00000001400D571A: 8B 94 24 B0 00 00  mov         edx,dword ptr [rsp+00000000000000B0h]
                    00
  00000001400D5721: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D5726: FF 15 7C EF D2 01  call        qword ptr [0000000141E046A8h]
  00000001400D572C: 48 8D 15 ED 14 09  lea         rdx,[0000000142166C20h]
                    02
  00000001400D5733: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D5738: FF 15 8A ED D2 01  call        qword ptr [0000000141E044C8h]
  00000001400D573E: 48 8D 84 24 20 01  lea         rax,[rsp+0000000000000120h]
                    00 00
  00000001400D5746: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  00000001400D574E: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  00000001400D5756: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  00000001400D575E: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  00000001400D5763: 48 8B 8C 24 28 01  mov         rcx,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001400D576B: FF 15 67 02 D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D5771: 48 89 84 24 F0 01  mov         qword ptr [rsp+00000000000001F0h],rax
                    00 00
  00000001400D5779: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001400D5781: 48 89 84 24 F8 01  mov         qword ptr [rsp+00000000000001F8h],rax
                    00 00
  00000001400D5789: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000001400D578E: 48 8B 8C 24 38 01  mov         rcx,qword ptr [rsp+0000000000000138h]
                    00 00
  00000001400D5796: FF 15 3C 02 D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D579C: 48 89 84 24 00 02  mov         qword ptr [rsp+0000000000000200h],rax
                    00 00
  00000001400D57A4: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  00000001400D57A9: 48 8B 94 24 F8 01  mov         rdx,qword ptr [rsp+00000000000001F8h]
                    00 00
  00000001400D57B1: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  00000001400D57B9: E8 72 42 83 00     call        0000000140909A30
  00000001400D57BE: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D57C6: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D57CD: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000001400D57D1: E8 3A B4 08 00     call        0000000140160C10
  00000001400D57D6: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D57D9: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  00000001400D57E1: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001400D57E9: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  00000001400D57F1: 48 83 BC 24 40 01  cmp         qword ptr [rsp+0000000000000140h],0
                    00 00 00
  00000001400D57FA: 74 1C              je          00000001400D5818
  00000001400D57FC: BA 01 00 00 00     mov         edx,1
  00000001400D5801: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400D5809: E8 02 29 00 00     call        00000001400D8110
  00000001400D580E: 48 89 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],rax
                    00 00
  00000001400D5816: EB 0C              jmp         00000001400D5824
  00000001400D5818: 48 C7 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],0
                    00 00 00 00 00 00
  00000001400D5824: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001400D5829: E8 F2 DF 81 00     call        00000001408F3820
  00000001400D582E: 90                 nop
  00000001400D582F: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001400D5834: FF 15 A6 01 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D583A: 90                 nop
  00000001400D583B: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D5840: FF 15 9A 01 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D5846: 90                 nop
  00000001400D5847: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400D584C: FF 15 8E 01 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D5852: 90                 nop
  00000001400D5853: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400D585B: FF 15 7F 01 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D5861: 90                 nop
  00000001400D5862: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001400D5867: FF 15 73 01 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D586D: 90                 nop
  00000001400D586E: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000001400D5876: FF 15 64 01 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D587C: E9 1A F6 FF FF     jmp         00000001400D4E9B
  00000001400D5881: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D5889: 48 81 C1 A8 00 00  add         rcx,0A8h
                    00
  00000001400D5890: E8 FB 82 13 00     call        000000014020DB90
  00000001400D5895: 48 8B 94 24 30 02  mov         rdx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D589D: 48 C7 82 B0 00 00  mov         qword ptr [rdx+00000000000000B0h],0
                    00 00 00 00 00
  00000001400D58A8: B0 01              mov         al,1
  00000001400D58AA: 48 81 C4 18 02 00  add         rsp,218h
                    00
  00000001400D58B1: 5F                 pop         rdi
  00000001400D58B2: 5E                 pop         rsi
  00000001400D58B3: C3                 ret

  Summary

     1E02000 .text

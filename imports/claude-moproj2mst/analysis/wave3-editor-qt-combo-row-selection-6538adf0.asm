Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006538ADF0: 48 8B C4           mov         rax,rsp
  000000006538ADF3: 56                 push        rsi
  000000006538ADF4: 57                 push        rdi
  000000006538ADF5: 41 54              push        r12
  000000006538ADF7: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  000000006538ADFE: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006538AE07: 48 89 58 18        mov         qword ptr [rax+18h],rbx
  000000006538AE0B: 48 89 68 20        mov         qword ptr [rax+20h],rbp
  000000006538AE0F: 48 8B DA           mov         rbx,rdx
  000000006538AE12: 48 8B F9           mov         rdi,rcx
  000000006538AE15: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  000000006538AE19: C7 40 A0 FF FF FF  mov         dword ptr [rax-60h],0FFFFFFFFh
                    FF
  000000006538AE20: C7 40 A4 FF FF FF  mov         dword ptr [rax-5Ch],0FFFFFFFFh
                    FF
  000000006538AE27: 33 F6              xor         esi,esi
  000000006538AE29: 48 89 70 A8        mov         qword ptr [rax-58h],rsi
  000000006538AE2D: 48 89 70 B0        mov         qword ptr [rax-50h],rsi
  000000006538AE31: 8B 42 04           mov         eax,dword ptr [rdx+4]
  000000006538AE34: 3B 81 18 02 00 00  cmp         eax,dword ptr [rcx+0000000000000218h]
  000000006538AE3A: 74 66              je          000000006538AEA2
  000000006538AE3C: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006538AE41: 48 8B CB           mov         rcx,rbx
  000000006538AE44: FF 15 1E DA 30 00  call        qword ptr [0000000065698868h]
  000000006538AE4A: 90                 nop
  000000006538AE4B: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006538AE52: 4C 8B 11           mov         r10,qword ptr [rcx]
  000000006538AE55: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006538AE5A: 44 8B 8F 18 02 00  mov         r9d,dword ptr [rdi+0000000000000218h]
                    00
  000000006538AE61: 44 8B 03           mov         r8d,dword ptr [rbx]
  000000006538AE64: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000006538AE69: 41 FF 52 58        call        qword ptr [r10+58h]
  000000006538AE6D: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006538AE72: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006538AE75: 48 89 0A           mov         qword ptr [rdx],rcx
  000000006538AE78: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006538AE7C: 48 89 4A 08        mov         qword ptr [rdx+8],rcx
  000000006538AE80: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  000000006538AE84: 48 89 42 10        mov         qword ptr [rdx+10h],rax
  000000006538AE88: 39 74 24 38        cmp         dword ptr [rsp+38h],esi
  000000006538AE8C: 7C 14              jl          000000006538AEA2
  000000006538AE8E: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000006538AE93: 48 C1 E8 20        shr         rax,20h
  000000006538AE97: 85 C0              test        eax,eax
  000000006538AE99: 78 07              js          000000006538AEA2
  000000006538AE9B: 48 39 74 24 48     cmp         qword ptr [rsp+48h],rsi
  000000006538AEA0: 75 1B              jne         000000006538AEBD
  000000006538AEA2: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006538AEA7: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006538AEAA: 48 89 01           mov         qword ptr [rcx],rax
  000000006538AEAD: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006538AEB1: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000006538AEB5: 48 8B 43 10        mov         rax,qword ptr [rbx+10h]
  000000006538AEB9: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  000000006538AEBD: 48 8D 8F 48 02 00  lea         rcx,[rdi+0000000000000248h]
                    00
  000000006538AEC4: FF 15 86 D9 30 00  call        qword ptr [0000000065698850h]
  000000006538AECA: 44 8B 44 24 38     mov         r8d,dword ptr [rsp+38h]
  000000006538AECF: 44 39 00           cmp         dword ptr [rax],r8d
  000000006538AED2: 75 24              jne         000000006538AEF8
  000000006538AED4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006538AED9: 48 39 48 08        cmp         qword ptr [rax+8],rcx
  000000006538AEDD: 75 19              jne         000000006538AEF8
  000000006538AEDF: 8B 4C 24 3C        mov         ecx,dword ptr [rsp+3Ch]
  000000006538AEE3: 39 48 04           cmp         dword ptr [rax+4],ecx
  000000006538AEE6: 75 10              jne         000000006538AEF8
  000000006538AEE8: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006538AEED: 48 39 48 10        cmp         qword ptr [rax+10h],rcx
  000000006538AEF1: 75 05              jne         000000006538AEF8
  000000006538AEF3: BE 01 00 00 00     mov         esi,1
  000000006538AEF8: 40 84 F6           test        sil,sil
  000000006538AEFB: 40 0F 94 C6        sete        sil
  000000006538AEFF: 40 84 F6           test        sil,sil
  000000006538AF02: 74 38              je          000000006538AF3C
  000000006538AF04: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006538AF09: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006538AF11: FF 15 31 DC 30 00  call        qword ptr [0000000065698B48h]
  000000006538AF17: 90                 nop
  000000006538AF18: 48 8B D0           mov         rdx,rax
  000000006538AF1B: 48 8D 8F 48 02 00  lea         rcx,[rdi+0000000000000248h]
                    00
  000000006538AF22: FF 15 08 D9 30 00  call        qword ptr [0000000065698830h]
  000000006538AF28: 90                 nop
  000000006538AF29: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006538AF31: FF 15 E1 D8 30 00  call        qword ptr [0000000065698818h]
  000000006538AF37: 44 8B 44 24 38     mov         r8d,dword ptr [rsp+38h]
  000000006538AF3C: 48 83 BF E8 01 00  cmp         qword ptr [rdi+00000000000001E8h],0
                    00 00
  000000006538AF44: 74 77              je          000000006538AFBD
  000000006538AF46: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006538AF4E: 49 8B CC           mov         rcx,r12
  000000006538AF51: E8 0A F0 FF FF     call        0000000065389F60
  000000006538AF56: 90                 nop
  000000006538AF57: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  000000006538AF5F: 48 8B 8F E8 01 00  mov         rcx,qword ptr [rdi+00000000000001E8h]
                    00
  000000006538AF66: E8 45 4A 03 00     call        00000000653BF9B0
  000000006538AF6B: 90                 nop
  000000006538AF6C: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006538AF74: 48 8B C8           mov         rcx,rax
  000000006538AF77: FF 15 A3 C2 30 00  call        qword ptr [0000000065697220h]
  000000006538AF7D: 0F B6 D8           movzx       ebx,al
  000000006538AF80: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006538AF88: FF 15 D2 B7 30 00  call        qword ptr [0000000065696760h]
  000000006538AF8E: 84 DB              test        bl,bl
  000000006538AF90: 74 14              je          000000006538AFA6
  000000006538AF92: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006538AF9A: 48 8B 8F E8 01 00  mov         rcx,qword ptr [rdi+00000000000001E8h]
                    00
  000000006538AFA1: E8 3A 4A 03 00     call        00000000653BF9E0
  000000006538AFA6: 48 8B CF           mov         rcx,rdi
  000000006538AFA9: E8 72 F8 FF FF     call        000000006538A820
  000000006538AFAE: 90                 nop
  000000006538AFAF: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006538AFB7: FF 15 A3 B7 30 00  call        qword ptr [0000000065696760h]
  000000006538AFBD: 40 84 F6           test        sil,sil
  000000006538AFC0: 74 23              je          000000006538AFE5
  000000006538AFC2: 49 8B CC           mov         rcx,r12
  000000006538AFC5: E8 E6 A2 CD FF     call        00000000650652B0
  000000006538AFCA: 48 8D 8F 48 02 00  lea         rcx,[rdi+0000000000000248h]
                    00
  000000006538AFD1: FF 15 79 D8 30 00  call        qword ptr [0000000065698850h]
  000000006538AFD7: 48 8B D0           mov         rdx,rax
  000000006538AFDA: 48 8B CF           mov         rcx,rdi
  000000006538AFDD: E8 AE FB FF FF     call        000000006538AB90
  000000006538AFE2: 90                 nop
  000000006538AFE3: EB 00              jmp         000000006538AFE5
  000000006538AFE5: 4C 8D 9C 24 80 00  lea         r11,[rsp+0000000000000080h]
                    00 00
  000000006538AFED: 49 8B 5B 30        mov         rbx,qword ptr [r11+30h]
  000000006538AFF1: 49 8B 6B 38        mov         rbp,qword ptr [r11+38h]
  000000006538AFF5: 49 8B E3           mov         rsp,r11
  000000006538AFF8: 41 5C              pop         r12
  000000006538AFFA: 5F                 pop         rdi
  000000006538AFFB: 5E                 pop         rsi
  000000006538AFFC: C3                 ret

  Summary

      695000 .text

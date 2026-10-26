Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006509ADB0: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000006509ADB5: 55                 push        rbp
  000000006509ADB6: 56                 push        rsi
  000000006509ADB7: 57                 push        rdi
  000000006509ADB8: 41 54              push        r12
  000000006509ADBA: 41 55              push        r13
  000000006509ADBC: 41 56              push        r14
  000000006509ADBE: 41 57              push        r15
  000000006509ADC0: 48 83 EC 50        sub         rsp,50h
  000000006509ADC4: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006509ADCD: 48 89 9C 24 98 00  mov         qword ptr [rsp+0000000000000098h],rbx
                    00 00
  000000006509ADD5: 4C 8B E2           mov         r12,rdx
  000000006509ADD8: 48 8B E9           mov         rbp,rcx
  000000006509ADDB: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  000000006509ADDF: BA 3C 00 00 00     mov         edx,3Ch
  000000006509ADE4: 48 8B CE           mov         rcx,rsi
  000000006509ADE7: E8 24 32 FC FF     call        000000006505E010
  000000006509ADEC: 44 0F B6 F8        movzx       r15d,al
  000000006509ADF0: 4C 8B 4E 20        mov         r9,qword ptr [rsi+20h]
  000000006509ADF4: 45 8B 51 08        mov         r10d,dword ptr [r9+8]
  000000006509ADF8: 41 C1 EA 0F        shr         r10d,0Fh
  000000006509ADFC: 41 80 E2 01        and         r10b,1
  000000006509AE00: 74 49              je          000000006509AE4B
  000000006509AE02: 48 8B CE           mov         rcx,rsi
  000000006509AE05: FF 15 5D BA 5F 00  call        qword ptr [0000000065696868h]
  000000006509AE0B: 48 85 C0           test        rax,rax
  000000006509AE0E: 74 3B              je          000000006509AE4B
  000000006509AE10: 48 8B CE           mov         rcx,rsi
  000000006509AE13: FF 15 4F BA 5F 00  call        qword ptr [0000000065696868h]
  000000006509AE19: 4C 3B E0           cmp         r12,rax
  000000006509AE1C: 74 2D              je          000000006509AE4B
  000000006509AE1E: 48 8B 7E 20        mov         rdi,qword ptr [rsi+20h]
  000000006509AE22: 48 8B CE           mov         rcx,rsi
  000000006509AE25: FF 15 3D BA 5F 00  call        qword ptr [0000000065696868h]
  000000006509AE2B: 48 8B 58 08        mov         rbx,qword ptr [rax+8]
  000000006509AE2F: 4C 8D 47 14        lea         r8,[rdi+14h]
  000000006509AE33: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006509AE38: 48 8B CD           mov         rcx,rbp
  000000006509AE3B: E8 E0 C2 F6 FF     call        0000000065007120
  000000006509AE40: 48 8B D0           mov         rdx,rax
  000000006509AE43: 48 8B CB           mov         rcx,rbx
  000000006509AE46: E8 05 41 15 00     call        00000000651EEF50
  000000006509AE4B: 48 8B BD 48 01 00  mov         rdi,qword ptr [rbp+0000000000000148h]
                    00
  000000006509AE52: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006509AE56: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006509AE59: C1 E9 0F           shr         ecx,0Fh
  000000006509AE5C: 80 E1 01           and         cl,1
  000000006509AE5F: 74 1F              je          000000006509AE80
  000000006509AE61: 48 85 FF           test        rdi,rdi
  000000006509AE64: 74 1A              je          000000006509AE80
  000000006509AE66: 33 D2              xor         edx,edx
  000000006509AE68: 48 8B CF           mov         rcx,rdi
  000000006509AE6B: FF 15 8F EA 5F 00  call        qword ptr [0000000065699900h]
  000000006509AE71: 33 D2              xor         edx,edx
  000000006509AE73: 48 8B 8D 48 01 00  mov         rcx,qword ptr [rbp+0000000000000148h]
                    00
  000000006509AE7A: FF 15 30 EA 5F 00  call        qword ptr [00000000656998B0h]
  000000006509AE80: 45 32 F6           xor         r14b,r14b
  000000006509AE83: BA 4F 00 00 00     mov         edx,4Fh
  000000006509AE88: 48 8B CE           mov         rcx,rsi
  000000006509AE8B: E8 80 31 FC FF     call        000000006505E010
  000000006509AE90: 84 C0              test        al,al
  000000006509AE92: 74 12              je          000000006509AEA6
  000000006509AE94: 41 B6 01           mov         r14b,1
  000000006509AE97: 45 33 C0           xor         r8d,r8d
  000000006509AE9A: 41 8D 50 4F        lea         edx,[r8+4Fh]
  000000006509AE9E: 48 8B CE           mov         rcx,rsi
  000000006509AEA1: E8 3A E1 FC FF     call        0000000065068FE0
  000000006509AEA6: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006509AEAA: 33 C9              xor         ecx,ecx
  000000006509AEAC: 80 78 0C 11        cmp         byte ptr [rax+0Ch],11h
  000000006509AEB0: 48 0F 44 F9        cmove       rdi,rcx
  000000006509AEB4: 33 D2              xor         edx,edx
  000000006509AEB6: 48 8B CD           mov         rcx,rbp
  000000006509AEB9: E8 E2 AA FC FF     call        00000000650659A0
  000000006509AEBE: 49 8B D4           mov         rdx,r12
  000000006509AEC1: 48 8B CD           mov         rcx,rbp
  000000006509AEC4: FF 15 4E C9 5F 00  call        qword ptr [0000000065697818h]
  000000006509AECA: 4C 8B 5E 20        mov         r11,qword ptr [rsi+20h]
  000000006509AECE: 41 0F B6 43 0A     movzx       eax,byte ptr [r11+0Ah]
  000000006509AED3: 24 01              and         al,1
  000000006509AED5: 74 16              je          000000006509AEED
  000000006509AED7: BA 45 00 00 00     mov         edx,45h
  000000006509AEDC: 48 8B CE           mov         rcx,rsi
  000000006509AEDF: E8 2C 31 FC FF     call        000000006505E010
  000000006509AEE4: 84 C0              test        al,al
  000000006509AEE6: 74 05              je          000000006509AEED
  000000006509AEE8: 41 B5 01           mov         r13b,1
  000000006509AEEB: EB 03              jmp         000000006509AEF0
  000000006509AEED: 45 32 ED           xor         r13b,r13b
  000000006509AEF0: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006509AEF8: 8B 00              mov         eax,dword ptr [rax]
  000000006509AEFA: 89 85 54 01 00 00  mov         dword ptr [rbp+0000000000000154h],eax
  000000006509AF00: 81 8D 58 01 00 00  or          dword ptr [rbp+0000000000000158h],1000h
                    00 10 00 00
  000000006509AF0A: 45 33 C0           xor         r8d,r8d
  000000006509AF0D: 41 8D 50 3C        lea         edx,[r8+3Ch]
  000000006509AF11: 48 8B CE           mov         rcx,rsi
  000000006509AF14: E8 C7 E0 FC FF     call        0000000065068FE0
  000000006509AF19: 45 33 C0           xor         r8d,r8d
  000000006509AF1C: 41 8D 50 0F        lea         edx,[r8+0Fh]
  000000006509AF20: 48 8B CE           mov         rcx,rsi
  000000006509AF23: E8 B8 E0 FC FF     call        0000000065068FE0
  000000006509AF28: 45 33 C0           xor         r8d,r8d
  000000006509AF2B: 41 8D 50 10        lea         edx,[r8+10h]
  000000006509AF2F: 48 8B CE           mov         rcx,rsi
  000000006509AF32: E8 A9 E0 FC FF     call        0000000065068FE0
  000000006509AF37: 48 8B D6           mov         rdx,rsi
  000000006509AF3A: 48 8D 8D 54 01 00  lea         rcx,[rbp+0000000000000154h]
                    00
  000000006509AF41: E8 6A 19 FC FF     call        000000006505C8B0
  000000006509AF46: 45 84 FF           test        r15b,r15b
  000000006509AF49: 75 1D              jne         000000006509AF68
  000000006509AF4B: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006509AF4F: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006509AF53: A8 01              test        al,1
  000000006509AF55: 75 1B              jne         000000006509AF72
  000000006509AF57: BA 3C 00 00 00     mov         edx,3Ch
  000000006509AF5C: 49 8B CC           mov         rcx,r12
  000000006509AF5F: E8 AC 30 FC FF     call        000000006505E010
  000000006509AF64: 84 C0              test        al,al
  000000006509AF66: 74 0A              je          000000006509AF72
  000000006509AF68: 33 D2              xor         edx,edx
  000000006509AF6A: 48 8B CD           mov         rcx,rbp
  000000006509AF6D: E8 DE DE FC FF     call        0000000065068E50
  000000006509AF72: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006509AF76: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006509AF7A: A8 01              test        al,1
  000000006509AF7C: 75 1A              jne         000000006509AF98
  000000006509AF7E: 4D 85 E4           test        r12,r12
  000000006509AF81: 74 15              je          000000006509AF98
  000000006509AF83: 49 8B 44 24 20     mov         rax,qword ptr [r12+20h]
  000000006509AF88: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006509AF8B: C1 E9 0F           shr         ecx,0Fh
  000000006509AF8E: 80 E1 01           and         cl,1
  000000006509AF91: 75 05              jne         000000006509AF98
  000000006509AF93: 45 84 ED           test        r13b,r13b
  000000006509AF96: 74 10              je          000000006509AFA8
  000000006509AF98: 41 B0 01           mov         r8b,1
  000000006509AF9B: BA 10 00 00 00     mov         edx,10h
  000000006509AFA0: 48 8B CE           mov         rcx,rsi
  000000006509AFA3: E8 38 E0 FC FF     call        0000000065068FE0
  000000006509AFA8: 45 0F B6 C5        movzx       r8d,r13b
  000000006509AFAC: BA 45 00 00 00     mov         edx,45h
  000000006509AFB1: 48 8B CE           mov         rcx,rsi
  000000006509AFB4: E8 27 E0 FC FF     call        0000000065068FE0
  000000006509AFB9: 45 84 FF           test        r15b,r15b
  000000006509AFBC: 74 08              je          000000006509AFC6
  000000006509AFBE: 48 8B CD           mov         rcx,rbp
  000000006509AFC1: E8 3A FC FF FF     call        000000006509AC00
  000000006509AFC6: 48 8B 8D 88 00 00  mov         rcx,qword ptr [rbp+0000000000000088h]
                    00
  000000006509AFCD: 48 85 C9           test        rcx,rcx
  000000006509AFD0: 74 73              je          000000006509B045
  000000006509AFD2: 48 83 C1 30        add         rcx,30h
  000000006509AFD6: E8 45 D1 0E 00     call        0000000065188120
  000000006509AFDB: 84 C0              test        al,al
  000000006509AFDD: 75 66              jne         000000006509B045
  000000006509AFDF: 48 8B 95 88 00 00  mov         rdx,qword ptr [rbp+0000000000000088h]
                    00
  000000006509AFE6: 48 83 C2 30        add         rdx,30h
  000000006509AFEA: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006509AFF2: E8 99 66 0D 00     call        0000000065171690
  000000006509AFF7: 90                 nop
  000000006509AFF8: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000006509B000: E8 EB D0 0E 00     call        00000000651880F0
  000000006509B005: 90                 nop
  000000006509B006: 48 8B 8D 88 00 00  mov         rcx,qword ptr [rbp+0000000000000088h]
                    00
  000000006509B00D: 48 83 C1 30        add         rcx,30h
  000000006509B011: 48 8B D0           mov         rdx,rax
  000000006509B014: E8 37 E4 0E 00     call        0000000065189450
  000000006509B019: 90                 nop
  000000006509B01A: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000006509B022: E8 F9 E3 0E 00     call        0000000065189420
  000000006509B027: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  000000006509B02F: 48 8B CE           mov         rcx,rsi
  000000006509B032: E8 D9 A2 FC FF     call        0000000065065310
  000000006509B037: 90                 nop
  000000006509B038: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006509B040: E8 DB E3 0E 00     call        0000000065189420
  000000006509B045: 48 8B 85 88 00 00  mov         rax,qword ptr [rbp+0000000000000088h]
                    00
  000000006509B04C: 48 85 C0           test        rax,rax
  000000006509B04F: 74 38              je          000000006509B089
  000000006509B051: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006509B055: 48 85 C9           test        rcx,rcx
  000000006509B058: 74 2F              je          000000006509B089
  000000006509B05A: 48 83 C1 30        add         rcx,30h
  000000006509B05E: FF 15 84 B9 5F 00  call        qword ptr [00000000656969E8h]
  000000006509B064: 84 C0              test        al,al
  000000006509B066: 75 21              jne         000000006509B089
  000000006509B068: B2 01              mov         dl,1
  000000006509B06A: 48 8B CD           mov         rcx,rbp
  000000006509B06D: E8 DE C5 FF FF     call        0000000065097650
  000000006509B072: 4C 8B 9D 88 00 00  mov         r11,qword ptr [rbp+0000000000000088h]
                    00
  000000006509B079: 49 8B 53 08        mov         rdx,qword ptr [r11+8]
  000000006509B07D: 48 83 C2 30        add         rdx,30h
  000000006509B081: 48 8B CD           mov         rcx,rbp
  000000006509B084: E8 F7 6A FC FF     call        0000000065061B80
  000000006509B089: 48 85 FF           test        rdi,rdi
  000000006509B08C: 74 09              je          000000006509B097
  000000006509B08E: 48 8B CF           mov         rcx,rdi
  000000006509B091: FF 15 59 E8 5F 00  call        qword ptr [00000000656998F0h]
  000000006509B097: BA 4E 00 00 00     mov         edx,4Eh
  000000006509B09C: 48 8B CE           mov         rcx,rsi
  000000006509B09F: E8 6C 2F FC FF     call        000000006505E010
  000000006509B0A4: 84 C0              test        al,al
  000000006509B0A6: 75 37              jne         000000006509B0DF
  000000006509B0A8: 45 84 F6           test        r14b,r14b
  000000006509B0AB: 75 32              jne         000000006509B0DF
  000000006509B0AD: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006509B0B1: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  000000006509B0B5: 75 38              jne         000000006509B0EF
  000000006509B0B7: 48 8B CE           mov         rcx,rsi
  000000006509B0BA: FF 15 A8 B7 5F 00  call        qword ptr [0000000065696868h]
  000000006509B0C0: 48 85 C0           test        rax,rax
  000000006509B0C3: 74 2A              je          000000006509B0EF
  000000006509B0C5: 48 8B CE           mov         rcx,rsi
  000000006509B0C8: FF 15 9A B7 5F 00  call        qword ptr [0000000065696868h]
  000000006509B0CE: BA 4F 00 00 00     mov         edx,4Fh
  000000006509B0D3: 48 8B C8           mov         rcx,rax
  000000006509B0D6: E8 35 2F FC FF     call        000000006505E010
  000000006509B0DB: 84 C0              test        al,al
  000000006509B0DD: 74 10              je          000000006509B0EF
  000000006509B0DF: 41 B0 01           mov         r8b,1
  000000006509B0E2: BA 4F 00 00 00     mov         edx,4Fh
  000000006509B0E7: 48 8B CE           mov         rcx,rsi
  000000006509B0EA: E8 F1 DE FC FF     call        0000000065068FE0
  000000006509B0EF: 48 8B 4E 20        mov         rcx,qword ptr [rsi+20h]
  000000006509B0F3: 48 83 C1 14        add         rcx,14h
  000000006509B0F7: FF 15 2B B6 5F 00  call        qword ptr [0000000065696728h]
  000000006509B0FD: 8B D8              mov         ebx,eax
  000000006509B0FF: 48 8B 4E 20        mov         rcx,qword ptr [rsi+20h]
  000000006509B103: 48 83 C1 14        add         rcx,14h
  000000006509B107: FF 15 53 B7 5F 00  call        qword ptr [0000000065696860h]
  000000006509B10D: 44 8B C8           mov         r9d,eax
  000000006509B110: 89 5C 24 20        mov         dword ptr [rsp+20h],ebx
  000000006509B114: 45 33 C0           xor         r8d,r8d
  000000006509B117: 33 D2              xor         edx,edx
  000000006509B119: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006509B11E: FF 15 4C B6 5F 00  call        qword ptr [0000000065696770h]
  000000006509B124: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006509B129: 48 8B CD           mov         rcx,rbp
  000000006509B12C: E8 1F 3E 15 00     call        00000000651EEF50
  000000006509B131: 48 8B 9C 24 98 00  mov         rbx,qword ptr [rsp+0000000000000098h]
                    00 00
  000000006509B139: 48 83 C4 50        add         rsp,50h
  000000006509B13D: 41 5F              pop         r15
  000000006509B13F: 41 5E              pop         r14
  000000006509B141: 41 5D              pop         r13
  000000006509B143: 41 5C              pop         r12
  000000006509B145: 5F                 pop         rdi
  000000006509B146: 5E                 pop         rsi
  000000006509B147: 5D                 pop         rbp
  000000006509B148: C3                 ret

  Summary

      695000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006535AC50: 48 8B C4           mov         rax,rsp
  000000006535AC53: 57                 push        rdi
  000000006535AC54: 48 83 EC 70        sub         rsp,70h
  000000006535AC58: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006535AC61: 48 89 58 08        mov         qword ptr [rax+8],rbx
  000000006535AC65: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  000000006535AC69: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  000000006535AC6D: 49 8B F9           mov         rdi,r9
  000000006535AC70: 48 8B F1           mov         rsi,rcx
  000000006535AC73: 33 DB              xor         ebx,ebx
  000000006535AC75: 83 FA 51           cmp         edx,51h
  000000006535AC78: 0F 87 C7 02 00 00  ja          000000006535AF45
  000000006535AC7E: 48 63 C2           movsxd      rax,edx
  000000006535AC81: 48 8D 0D 78 53 CA  lea         rcx,[0000000065000000h]
                    FF
  000000006535AC88: 0F B6 84 01 9C AF  movzx       eax,byte ptr [rcx+rax+000000000035AF9Ch]
                    35 00
  000000006535AC90: 44 8B 94 81 74 AF  mov         r10d,dword ptr [rcx+rax*4+000000000035AF74h]
                    35 00
  000000006535AC98: 4C 03 D1           add         r10,rcx
  000000006535AC9B: 41 FF E2           jmp         r10
  000000006535AC9E: BB 01 00 00 00     mov         ebx,1
  000000006535ACA3: E9 B4 02 00 00     jmp         000000006535AF5C
  000000006535ACA8: 49 8B D1           mov         rdx,r9
  000000006535ACAB: 48 8D 0D F6 EE 3F  lea         rcx,[0000000065759BA8h]
                    00
  000000006535ACB2: FF 15 20 C5 33 00  call        qword ptr [00000000656971D8h]
  000000006535ACB8: 48 85 C0           test        rax,rax
  000000006535ACBB: 0F 84 9B 02 00 00  je          000000006535AF5C
  000000006535ACC1: BB 01 00 00 00     mov         ebx,1
  000000006535ACC6: E9 91 02 00 00     jmp         000000006535AF5C
  000000006535ACCB: BB 01 00 00 00     mov         ebx,1
  000000006535ACD0: E9 87 02 00 00     jmp         000000006535AF5C
  000000006535ACD5: 89 5C 24 38        mov         dword ptr [rsp+38h],ebx
  000000006535ACD9: 45 33 C9           xor         r9d,r9d
  000000006535ACDC: 4C 8D 44 24 38     lea         r8,[rsp+38h]
  000000006535ACE1: 33 D2              xor         edx,edx
  000000006535ACE3: B9 0A 10 00 00     mov         ecx,100Ah
  000000006535ACE8: FF 15 FA ED 33 00  call        qword ptr [0000000065699AE8h]
  000000006535ACEE: 8B 5C 24 38        mov         ebx,dword ptr [rsp+38h]
  000000006535ACF2: 48 8B 76 08        mov         rsi,qword ptr [rsi+8]
  000000006535ACF6: 85 DB              test        ebx,ebx
  000000006535ACF8: 0F 85 5E 02 00 00  jne         000000006535AF5C
  000000006535ACFE: 48 85 FF           test        rdi,rdi
  000000006535AD01: 0F 84 55 02 00 00  je          000000006535AF5C
  000000006535AD07: 48 85 F6           test        rsi,rsi
  000000006535AD0A: 0F 84 4C 02 00 00  je          000000006535AF5C
  000000006535AD10: 48 8B D7           mov         rdx,rdi
  000000006535AD13: 48 8D 0D 3E D2 3A  lea         rcx,[0000000065707F58h]
                    00
  000000006535AD1A: FF 15 B8 C4 33 00  call        qword ptr [00000000656971D8h]
  000000006535AD20: 48 8B E8           mov         rbp,rax
  000000006535AD23: 48 85 C0           test        rax,rax
  000000006535AD26: 75 76              jne         000000006535AD9E
  000000006535AD28: 48 8B D7           mov         rdx,rdi
  000000006535AD2B: 48 8D 0D D6 C1 3A  lea         rcx,[0000000065706F08h]
                    00
  000000006535AD32: FF 15 A0 C4 33 00  call        qword ptr [00000000656971D8h]
  000000006535AD38: 48 85 C0           test        rax,rax
  000000006535AD3B: 74 44              je          000000006535AD81
  000000006535AD3D: E8 5E C3 CB FF     call        00000000650170A0
  000000006535AD42: 48 8B E8           mov         rbp,rax
  000000006535AD45: 48 85 C0           test        rax,rax
  000000006535AD48: 74 37              je          000000006535AD81
  000000006535AD4A: 48 3B C7           cmp         rax,rdi
  000000006535AD4D: 74 32              je          000000006535AD81
  000000006535AD4F: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006535AD54: FF 15 0E BA 33 00  call        qword ptr [0000000065696768h]
  000000006535AD5A: 90                 nop
  000000006535AD5B: 4C 8D 05 F6 D1 3A  lea         r8,[0000000065707F58h]
                    00
  000000006535AD62: 48 8B D0           mov         rdx,rax
  000000006535AD65: 48 8B CD           mov         rcx,rbp
  000000006535AD68: FF 15 1A DB 33 00  call        qword ptr [0000000065698888h]
  000000006535AD6E: 48 8B E8           mov         rbp,rax
  000000006535AD71: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006535AD76: FF 15 E4 B9 33 00  call        qword ptr [0000000065696760h]
  000000006535AD7C: 48 85 ED           test        rbp,rbp
  000000006535AD7F: 75 1D              jne         000000006535AD9E
  000000006535AD81: 48 8B D7           mov         rdx,rdi
  000000006535AD84: 48 8B CE           mov         rcx,rsi
  000000006535AD87: E8 64 F4 FF FF     call        000000006535A1F0
  000000006535AD8C: 84 C0              test        al,al
  000000006535AD8E: 0F 84 C8 01 00 00  je          000000006535AF5C
  000000006535AD94: BB 01 00 00 00     mov         ebx,1
  000000006535AD99: E9 BE 01 00 00     jmp         000000006535AF5C
  000000006535AD9E: 48 8B 45 08        mov         rax,qword ptr [rbp+8]
  000000006535ADA2: F6 80 18 02 00 00  test        byte ptr [rax+0000000000000218h],1
                    01
  000000006535ADA9: 75 0D              jne         000000006535ADB8
  000000006535ADAB: 80 BE D8 00 00 00  cmp         byte ptr [rsi+00000000000000D8h],0
                    00
  000000006535ADB2: 0F 84 A4 01 00 00  je          000000006535AF5C
  000000006535ADB8: BB 01 00 00 00     mov         ebx,1
  000000006535ADBD: E9 9A 01 00 00     jmp         000000006535AF5C
  000000006535ADC2: 4D 85 C0           test        r8,r8
  000000006535ADC5: 0F 84 91 01 00 00  je          000000006535AF5C
  000000006535ADCB: 41 83 38 01        cmp         dword ptr [r8],1
  000000006535ADCF: 0F 8C 87 01 00 00  jl          000000006535AF5C
  000000006535ADD5: 41 83 78 04 0F     cmp         dword ptr [r8+4],0Fh
  000000006535ADDA: 0F 85 7C 01 00 00  jne         000000006535AF5C
  000000006535ADE0: 41 83 78 38 01     cmp         dword ptr [r8+38h],1
  000000006535ADE5: 0F 85 71 01 00 00  jne         000000006535AF5C
  000000006535ADEB: BB 01 00 00 00     mov         ebx,1
  000000006535ADF0: 48 8B B4 24 A0 00  mov         rsi,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006535ADF8: 48 85 F6           test        rsi,rsi
  000000006535ADFB: 0F 84 5B 01 00 00  je          000000006535AF5C
  000000006535AE01: 39 1E              cmp         dword ptr [rsi],ebx
  000000006535AE03: 0F 8F 53 01 00 00  jg          000000006535AF5C
  000000006535AE09: 81 7E 04 01 F0 00  cmp         dword ptr [rsi+4],0F001h
                    00
  000000006535AE10: 0F 85 46 01 00 00  jne         000000006535AF5C
  000000006535AE16: 49 8D 68 10        lea         rbp,[r8+10h]
  000000006535AE1A: 45 33 C0           xor         r8d,r8d
  000000006535AE1D: 48 8B D5           mov         rdx,rbp
  000000006535AE20: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006535AE25: E8 76 25 E3 FF     call        000000006518D3A0
  000000006535AE2A: 90                 nop
  000000006535AE2B: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006535AE30: 48 8D 4E 08        lea         rcx,[rsi+8]
  000000006535AE34: E8 17 E6 E2 FF     call        0000000065189450
  000000006535AE39: 90                 nop
  000000006535AE3A: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006535AE3F: E8 DC E5 E2 FF     call        0000000065189420
  000000006535AE44: 44 8B C3           mov         r8d,ebx
  000000006535AE47: 48 85 FF           test        rdi,rdi
  000000006535AE4A: 74 14              je          000000006535AE60
  000000006535AE4C: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006535AE50: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006535AE54: 41 84 C0           test        r8b,al
  000000006535AE57: B8 04 00 00 00     mov         eax,4
  000000006535AE5C: 44 0F 45 C0        cmovne      r8d,eax
  000000006535AE60: 41 8B C0           mov         eax,r8d
  000000006535AE63: F7 D8              neg         eax
  000000006535AE65: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  000000006535AE69: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000006535AE6D: 45 8B C8           mov         r9d,r8d
  000000006535AE70: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  000000006535AE75: 48 8B CD           mov         rcx,rbp
  000000006535AE78: FF 15 B2 BA 33 00  call        qword ptr [0000000065696930h]
  000000006535AE7E: 48 8B D0           mov         rdx,rax
  000000006535AE81: 45 33 C0           xor         r8d,r8d
  000000006535AE84: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006535AE89: E8 12 25 E3 FF     call        000000006518D3A0
  000000006535AE8E: 90                 nop
  000000006535AE8F: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006535AE94: 48 8D 4E 08        lea         rcx,[rsi+8]
  000000006535AE98: E8 C3 42 E3 FF     call        000000006518F160
  000000006535AE9D: 90                 nop
  000000006535AE9E: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006535AEA3: E8 78 E5 E2 FF     call        0000000065189420
  000000006535AEA8: E9 AF 00 00 00     jmp         000000006535AF5C
  000000006535AEAD: 4D 85 C9           test        r9,r9
  000000006535AEB0: 74 7E              je          000000006535AF30
  000000006535AEB2: 48 8B 05 FF C3 33  mov         rax,qword ptr [00000000656972B8h]
                    00
  000000006535AEB9: 8B 08              mov         ecx,dword ptr [rax]
  000000006535AEBB: 83 F9 30           cmp         ecx,30h
  000000006535AEBE: 7C 70              jl          000000006535AF30
  000000006535AEC0: F6 C1 F0           test        cl,0F0h
  000000006535AEC3: 74 6B              je          000000006535AF30
  000000006535AEC5: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006535AECA: 49 8B C9           mov         rcx,r9
  000000006535AECD: E8 6E 21 28 00     call        00000000655DD040
  000000006535AED2: 90                 nop
  000000006535AED3: BA CF 25 00 00     mov         edx,25CFh
  000000006535AED8: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000006535AEE0: FF 15 7A C4 33 00  call        qword ptr [0000000065697360h]
  000000006535AEE6: 0F B7 10           movzx       edx,word ptr [rax]
  000000006535AEE9: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006535AEEE: E8 3D AC EA FF     call        0000000065205B30
  000000006535AEF3: 84 C0              test        al,al
  000000006535AEF5: 74 07              je          000000006535AEFE
  000000006535AEF7: BB CF 25 00 00     mov         ebx,25CFh
  000000006535AEFC: EB 24              jmp         000000006535AF22
  000000006535AEFE: BF 22 20 00 00     mov         edi,2022h
  000000006535AF03: 8B D7              mov         edx,edi
  000000006535AF05: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006535AF0A: FF 15 50 C4 33 00  call        qword ptr [0000000065697360h]
  000000006535AF10: 0F B7 10           movzx       edx,word ptr [rax]
  000000006535AF13: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006535AF18: E8 13 AC EA FF     call        0000000065205B30
  000000006535AF1D: 84 C0              test        al,al
  000000006535AF1F: 0F 45 DF           cmovne      ebx,edi
  000000006535AF22: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006535AF27: E8 F4 BB EA FF     call        0000000065206B20
  000000006535AF2C: 85 DB              test        ebx,ebx
  000000006535AF2E: 75 2C              jne         000000006535AF5C
  000000006535AF30: BB 2A 00 00 00     mov         ebx,2Ah
  000000006535AF35: EB 25              jmp         000000006535AF5C
  000000006535AF37: BB 01 00 00 00     mov         ebx,1
  000000006535AF3C: EB 1E              jmp         000000006535AF5C
  000000006535AF3E: BB 01 00 00 00     mov         ebx,1
  000000006535AF43: EB 17              jmp         000000006535AF5C
  000000006535AF45: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006535AF4D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006535AF52: 48 8B CE           mov         rcx,rsi
  000000006535AF55: E8 86 A2 F5 FF     call        00000000652B51E0
  000000006535AF5A: 8B D8              mov         ebx,eax
  000000006535AF5C: 8B C3              mov         eax,ebx
  000000006535AF5E: 4C 8D 5C 24 70     lea         r11,[rsp+70h]
  000000006535AF63: 49 8B 5B 10        mov         rbx,qword ptr [r11+10h]
  000000006535AF67: 49 8B 6B 20        mov         rbp,qword ptr [r11+20h]
  000000006535AF6B: 49 8B 73 28        mov         rsi,qword ptr [r11+28h]
  000000006535AF6F: 49 8B E3           mov         rsp,r11
  000000006535AF72: 5F                 pop         rdi
  000000006535AF73: C3                 ret
  000000006535AF74: 9E                 sahf
  000000006535AF75: AC                 lods        byte ptr [rsi]
  000000006535AF76: 35 00 CB AC 35     xor         eax,35ACCB00h
  000000006535AF7B: 00 AD AE 35 00 5C  add         byte ptr [rbp+000000005C0035AEh],ch
  000000006535AF81: AF                 scas        dword ptr [rdi]
  000000006535AF82: 35 00 D5 AC 35     xor         eax,35ACD500h
  000000006535AF87: 00 C2              add         dl,al
  000000006535AF89: AD                 lods        dword ptr [rsi]
  000000006535AF8A: 35 00 A8 AC 35     xor         eax,35ACA800h
  000000006535AF8F: 00 37              add         byte ptr [rdi],dh
  000000006535AF91: AF                 scas        dword ptr [rdi]
  000000006535AF92: 35 00 3E AF 35     xor         eax,35AF3E00h
  000000006535AF97: 00 45 AF           add         byte ptr [rbp-51h],al
  000000006535AF9A: 35 00 00 09 09     xor         eax,9090000h
  000000006535AF9F: 09 09              or          dword ptr [rcx],ecx
  000000006535AFA1: 09 09              or          dword ptr [rcx],ecx
  000000006535AFA3: 00 09              add         byte ptr [rcx],cl
  000000006535AFA5: 09 09              or          dword ptr [rcx],ecx
  000000006535AFA7: 00 00              add         byte ptr [rax],al
  000000006535AFA9: 00 00              add         byte ptr [rax],al
  000000006535AFAB: 09 09              or          dword ptr [rcx],ecx
  000000006535AFAD: 09 00              or          dword ptr [rax],eax
  000000006535AFAF: 00 00              add         byte ptr [rax],al
  000000006535AFB1: 00 01              add         byte ptr [rcx],al
  000000006535AFB3: 09 09              or          dword ptr [rcx],ecx
  000000006535AFB5: 09 09              or          dword ptr [rcx],ecx
  000000006535AFB7: 00 09              add         byte ptr [rcx],cl
  000000006535AFB9: 09 09              or          dword ptr [rcx],ecx
  000000006535AFBB: 09 09              or          dword ptr [rcx],ecx
  000000006535AFBD: 09 09              or          dword ptr [rcx],ecx
  000000006535AFBF: 02 09              add         cl,byte ptr [rcx]
  000000006535AFC1: 03 09              add         ecx,dword ptr [rcx]
  000000006535AFC3: 09 09              or          dword ptr [rcx],ecx
  000000006535AFC5: 04 09              add         al,9
  000000006535AFC7: 09 09              or          dword ptr [rcx],ecx
  000000006535AFC9: 09 09              or          dword ptr [rcx],ecx
  000000006535AFCB: 09 09              or          dword ptr [rcx],ecx
  000000006535AFCD: 09 09              or          dword ptr [rcx],ecx
  000000006535AFCF: 09 09              or          dword ptr [rcx],ecx
  000000006535AFD1: 09 09              or          dword ptr [rcx],ecx
  000000006535AFD3: 05 09 09 09 09     add         eax,9090909h
  000000006535AFD8: 09 06              or          dword ptr [rsi],eax
  000000006535AFDA: 09 09              or          dword ptr [rcx],ecx
  000000006535AFDC: 09 09              or          dword ptr [rcx],ecx
  000000006535AFDE: 09 09              or          dword ptr [rcx],ecx
  000000006535AFE0: 09 09              or          dword ptr [rcx],ecx
  000000006535AFE2: 09 09              or          dword ptr [rcx],ecx
  000000006535AFE4: 03 09              add         ecx,dword ptr [rcx]
  000000006535AFE6: 09 09              or          dword ptr [rcx],ecx
  000000006535AFE8: 09 09              or          dword ptr [rcx],ecx
  000000006535AFEA: 09 09              or          dword ptr [rcx],ecx
  000000006535AFEC: 07
  000000006535AFED: 08

  Summary

      695000 .text

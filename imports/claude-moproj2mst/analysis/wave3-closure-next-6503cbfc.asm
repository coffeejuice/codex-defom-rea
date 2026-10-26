Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006503CBFC: 49 89 6B 20        mov         qword ptr [r11+20h],rbp
  000000006503CC00: 49 89 7B D8        mov         qword ptr [r11-28h],rdi
  000000006503CC04: 39 50 08           cmp         dword ptr [rax+8],edx
  000000006503CC07: 4D 89 63 D0        mov         qword ptr [r11-30h],r12
  000000006503CC0B: 41 8B F0           mov         esi,r8d
  000000006503CC0E: 4D 89 6B C8        mov         qword ptr [r11-38h],r13
  000000006503CC12: 44 8B FA           mov         r15d,edx
  000000006503CC15: 4C 8B F1           mov         r14,rcx
  000000006503CC18: 0F 8D CA 02 00 00  jge         000000006503CEE8
  000000006503CC1E: 8B 89 C0 00 00 00  mov         ecx,dword ptr [rcx+00000000000000C0h]
  000000006503CC24: 49 8D 43 08        lea         rax,[r11+8]
  000000006503CC28: 49 8D 53 10        lea         rdx,[r11+10h]
  000000006503CC2C: 03 C9              add         ecx,ecx
  000000006503CC2E: 44 3B F9           cmp         r15d,ecx
  000000006503CC31: 89 4C 24 60        mov         dword ptr [rsp+60h],ecx
  000000006503CC35: 48 0F 4D C2        cmovge      rax,rdx
  000000006503CC39: 48 63 38           movsxd      rdi,dword ptr [rax]
  000000006503CC3C: 49 8B 86 C8 00 00  mov         rax,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CC43: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CC47: 41 3B F8           cmp         edi,r8d
  000000006503CC4A: 7F 14              jg          000000006503CC60
  000000006503CC4C: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006503CC50: 75 26              jne         000000006503CC78
  000000006503CC52: 3B 78 08           cmp         edi,dword ptr [rax+8]
  000000006503CC55: 7D 21              jge         000000006503CC78
  000000006503CC57: 41 8B C0           mov         eax,r8d
  000000006503CC5A: D1 F8              sar         eax,1
  000000006503CC5C: 3B F8              cmp         edi,eax
  000000006503CC5E: 7D 18              jge         000000006503CC78
  000000006503CC60: 41 B8 24 00 00 00  mov         r8d,24h
  000000006503CC66: 41 B1 01           mov         r9b,1
  000000006503CC69: 8B D7              mov         edx,edi
  000000006503CC6B: 41 8D 48 10        lea         ecx,[r8+10h]
  000000006503CC6F: FF 15 73 A4 65 00  call        qword ptr [00000000656970E8h]
  000000006503CC75: 44 8B C0           mov         r8d,eax
  000000006503CC78: 49 8D 8E C8 00 00  lea         rcx,[r14+00000000000000C8h]
                    00
  000000006503CC7F: 8B D7              mov         edx,edi
  000000006503CC81: E8 5A D8 FE FF     call        000000006502A4E0
  000000006503CC86: 49 8B 86 E0 00 00  mov         rax,qword ptr [r14+00000000000000E0h]
                    00
  000000006503CC8D: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CC91: 41 3B F8           cmp         edi,r8d
  000000006503CC94: 7F 14              jg          000000006503CCAA
  000000006503CC96: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006503CC9A: 75 24              jne         000000006503CCC0
  000000006503CC9C: 3B 78 08           cmp         edi,dword ptr [rax+8]
  000000006503CC9F: 7D 1F              jge         000000006503CCC0
  000000006503CCA1: 41 8B C0           mov         eax,r8d
  000000006503CCA4: D1 F8              sar         eax,1
  000000006503CCA6: 3B F8              cmp         edi,eax
  000000006503CCA8: 7D 16              jge         000000006503CCC0
  000000006503CCAA: 45 33 C9           xor         r9d,r9d
  000000006503CCAD: 8B D7              mov         edx,edi
  000000006503CCAF: 45 8D 41 04        lea         r8d,[r9+4]
  000000006503CCB3: 41 8D 49 14        lea         ecx,[r9+14h]
  000000006503CCB7: FF 15 2B A4 65 00  call        qword ptr [00000000656970E8h]
  000000006503CCBD: 44 8B C0           mov         r8d,eax
  000000006503CCC0: 49 8D 8E E0 00 00  lea         rcx,[r14+00000000000000E0h]
                    00
  000000006503CCC7: 8B D7              mov         edx,edi
  000000006503CCC9: E8 92 F2 FF FF     call        000000006503BF60
  000000006503CCCE: 49 8B 86 F0 00 00  mov         rax,qword ptr [r14+00000000000000F0h]
                    00
  000000006503CCD5: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CCD9: 41 3B F8           cmp         edi,r8d
  000000006503CCDC: 7F 14              jg          000000006503CCF2
  000000006503CCDE: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006503CCE2: 75 24              jne         000000006503CD08
  000000006503CCE4: 3B 78 08           cmp         edi,dword ptr [rax+8]
  000000006503CCE7: 7D 1F              jge         000000006503CD08
  000000006503CCE9: 41 8B C0           mov         eax,r8d
  000000006503CCEC: D1 F8              sar         eax,1
  000000006503CCEE: 3B F8              cmp         edi,eax
  000000006503CCF0: 7D 16              jge         000000006503CD08
  000000006503CCF2: 45 33 C9           xor         r9d,r9d
  000000006503CCF5: 8B D7              mov         edx,edi
  000000006503CCF7: 45 8D 41 04        lea         r8d,[r9+4]
  000000006503CCFB: 41 8D 49 14        lea         ecx,[r9+14h]
  000000006503CCFF: FF 15 E3 A3 65 00  call        qword ptr [00000000656970E8h]
  000000006503CD05: 44 8B C0           mov         r8d,eax
  000000006503CD08: 49 8D 8E F0 00 00  lea         rcx,[r14+00000000000000F0h]
                    00
  000000006503CD0F: 8B D7              mov         edx,edi
  000000006503CD11: E8 4A F2 FF FF     call        000000006503BF60
  000000006503CD16: 4D 63 9E C0 00 00  movsxd      r11,dword ptr [r14+00000000000000C0h]
                    00
  000000006503CD1D: 44 3B DF           cmp         r11d,edi
  000000006503CD20: 0F 8D C2 01 00 00  jge         000000006503CEE8
  000000006503CD26: 4C 8B EF           mov         r13,rdi
  000000006503CD29: 4B 8D 3C DB        lea         rdi,[r11+r11*8]
  000000006503CD2D: 4A 8D 34 9D 10 00  lea         rsi,[r11*4+0000000000000010h]
                    00 00
  000000006503CD35: 48 C1 E7 02        shl         rdi,2
  000000006503CD39: 4D 2B EB           sub         r13,r11
  000000006503CD3C: 0F 1F 40 00        nop         dword ptr [rax]
  000000006503CD40: 49 8B 8E C8 00 00  mov         rcx,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CD47: BA 01 00 00 00     mov         edx,1
  000000006503CD4C: FF 15 1E 9D 65 00  call        qword ptr [0000000065696A70h]
  000000006503CD52: 84 C0              test        al,al
  000000006503CD54: 74 1A              je          000000006503CD70
  000000006503CD56: 49 8B 86 C8 00 00  mov         rax,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CD5D: 49 8D 8E C8 00 00  lea         rcx,[r14+00000000000000C8h]
                    00
  000000006503CD64: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CD68: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503CD6B: E8 70 D7 FE FF     call        000000006502A4E0
  000000006503CD70: 49 8B 86 C8 00 00  mov         rax,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CD77: 33 C9              xor         ecx,ecx
  000000006503CD79: 89 4C 07 10        mov         dword ptr [rdi+rax+10h],ecx
  000000006503CD7D: 89 4C 07 14        mov         dword ptr [rdi+rax+14h],ecx
  000000006503CD81: 89 4C 07 1C        mov         dword ptr [rdi+rax+1Ch],ecx
  000000006503CD85: C7 44 07 18 FF FF  mov         dword ptr [rdi+rax+18h],7FFFFh
                    07 00
  000000006503CD8D: 88 4C 07 20        mov         byte ptr [rdi+rax+20h],cl
  000000006503CD91: C6 44 07 21 01     mov         byte ptr [rdi+rax+21h],1
  000000006503CD96: 89 4C 07 24        mov         dword ptr [rdi+rax+24h],ecx
  000000006503CD9A: 8D 51 01           lea         edx,[rcx+1]
  000000006503CD9D: 49 8B 8E C8 00 00  mov         rcx,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CDA4: FF 15 C6 9C 65 00  call        qword ptr [0000000065696A70h]
  000000006503CDAA: 84 C0              test        al,al
  000000006503CDAC: 74 1A              je          000000006503CDC8
  000000006503CDAE: 49 8B 86 C8 00 00  mov         rax,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CDB5: 49 8D 8E C8 00 00  lea         rcx,[r14+00000000000000C8h]
                    00
  000000006503CDBC: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CDC0: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503CDC3: E8 18 D7 FE FF     call        000000006502A4E0
  000000006503CDC8: 49 8B 86 C8 00 00  mov         rax,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CDCF: BA 01 00 00 00     mov         edx,1
  000000006503CDD4: C7 44 38 18 00 00  mov         dword ptr [rax+rdi+18h],0
                    00 00
  000000006503CDDC: 49 8B 8E C8 00 00  mov         rcx,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CDE3: FF 15 87 9C 65 00  call        qword ptr [0000000065696A70h]
  000000006503CDE9: 84 C0              test        al,al
  000000006503CDEB: 74 1A              je          000000006503CE07
  000000006503CDED: 49 8B 86 C8 00 00  mov         rax,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CDF4: 49 8D 8E C8 00 00  lea         rcx,[r14+00000000000000C8h]
                    00
  000000006503CDFB: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CDFF: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503CE02: E8 D9 D6 FE FF     call        000000006502A4E0
  000000006503CE07: 49 8B 86 C8 00 00  mov         rax,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CE0E: BA 01 00 00 00     mov         edx,1
  000000006503CE13: C7 44 38 2C 00 00  mov         dword ptr [rax+rdi+2Ch],0
                    00 00
  000000006503CE1B: 49 8B 8E C8 00 00  mov         rcx,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CE22: FF 15 48 9C 65 00  call        qword ptr [0000000065696A70h]
  000000006503CE28: 84 C0              test        al,al
  000000006503CE2A: 74 1A              je          000000006503CE46
  000000006503CE2C: 49 8B 86 C8 00 00  mov         rax,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CE33: 49 8D 8E C8 00 00  lea         rcx,[r14+00000000000000C8h]
                    00
  000000006503CE3A: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CE3E: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503CE41: E8 9A D6 FE FF     call        000000006502A4E0
  000000006503CE46: 49 8B 86 C8 00 00  mov         rax,qword ptr [r14+00000000000000C8h]
                    00
  000000006503CE4D: BA 01 00 00 00     mov         edx,1
  000000006503CE52: C7 44 38 30 00 00  mov         dword ptr [rax+rdi+30h],0
                    00 00
  000000006503CE5A: 49 8B 8E E0 00 00  mov         rcx,qword ptr [r14+00000000000000E0h]
                    00
  000000006503CE61: FF 15 09 9C 65 00  call        qword ptr [0000000065696A70h]
  000000006503CE67: 84 C0              test        al,al
  000000006503CE69: 74 1A              je          000000006503CE85
  000000006503CE6B: 49 8B 86 E0 00 00  mov         rax,qword ptr [r14+00000000000000E0h]
                    00
  000000006503CE72: 49 8D 8E E0 00 00  lea         rcx,[r14+00000000000000E0h]
                    00
  000000006503CE79: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CE7D: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503CE80: E8 DB F0 FF FF     call        000000006503BF60
  000000006503CE85: 49 8B 86 E0 00 00  mov         rax,qword ptr [r14+00000000000000E0h]
                    00
  000000006503CE8C: BA 01 00 00 00     mov         edx,1
  000000006503CE91: C7 04 06 00 00 00  mov         dword ptr [rsi+rax],0
                    00
  000000006503CE98: 49 8B 8E F0 00 00  mov         rcx,qword ptr [r14+00000000000000F0h]
                    00
  000000006503CE9F: FF 15 CB 9B 65 00  call        qword ptr [0000000065696A70h]
  000000006503CEA5: 84 C0              test        al,al
  000000006503CEA7: 74 1A              je          000000006503CEC3
  000000006503CEA9: 49 8B 86 F0 00 00  mov         rax,qword ptr [r14+00000000000000F0h]
                    00
  000000006503CEB0: 49 8D 8E F0 00 00  lea         rcx,[r14+00000000000000F0h]
                    00
  000000006503CEB7: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CEBB: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503CEBE: E8 9D F0 FF FF     call        000000006503BF60
  000000006503CEC3: 49 8B 86 F0 00 00  mov         rax,qword ptr [r14+00000000000000F0h]
                    00
  000000006503CECA: 48 83 C6 04        add         rsi,4
  000000006503CECE: 48 83 C7 24        add         rdi,24h
  000000006503CED2: 49 83 ED 01        sub         r13,1
  000000006503CED6: C7 44 06 FC 00 00  mov         dword ptr [rsi+rax-4],0
                    00 00
  000000006503CEDE: 0F 85 5C FE FF FF  jne         000000006503CD40
  000000006503CEE4: 8B 74 24 70        mov         esi,dword ptr [rsp+70h]
  000000006503CEE8: 49 8B 86 D0 00 00  mov         rax,qword ptr [r14+00000000000000D0h]
                    00
  000000006503CEEF: 39 70 08           cmp         dword ptr [rax+8],esi
  000000006503CEF2: 0F 8D D0 02 00 00  jge         000000006503D1C8
  000000006503CEF8: 41 8B 8E C4 00 00  mov         ecx,dword ptr [r14+00000000000000C4h]
                    00
  000000006503CEFF: 48 8D 44 24 68     lea         rax,[rsp+68h]
  000000006503CF04: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  000000006503CF09: 03 C9              add         ecx,ecx
  000000006503CF0B: 3B F1              cmp         esi,ecx
  000000006503CF0D: 89 4C 24 68        mov         dword ptr [rsp+68h],ecx
  000000006503CF11: 48 0F 4D C2        cmovge      rax,rdx
  000000006503CF15: 48 63 38           movsxd      rdi,dword ptr [rax]
  000000006503CF18: 49 8B 86 D0 00 00  mov         rax,qword ptr [r14+00000000000000D0h]
                    00
  000000006503CF1F: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CF23: 41 3B F8           cmp         edi,r8d
  000000006503CF26: 7F 14              jg          000000006503CF3C
  000000006503CF28: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006503CF2C: 75 26              jne         000000006503CF54
  000000006503CF2E: 3B 78 08           cmp         edi,dword ptr [rax+8]
  000000006503CF31: 7D 21              jge         000000006503CF54
  000000006503CF33: 41 8B C0           mov         eax,r8d
  000000006503CF36: D1 F8              sar         eax,1
  000000006503CF38: 3B F8              cmp         edi,eax
  000000006503CF3A: 7D 18              jge         000000006503CF54
  000000006503CF3C: 41 B8 24 00 00 00  mov         r8d,24h
  000000006503CF42: 41 B1 01           mov         r9b,1
  000000006503CF45: 8B D7              mov         edx,edi
  000000006503CF47: 41 8D 48 10        lea         ecx,[r8+10h]
  000000006503CF4B: FF 15 97 A1 65 00  call        qword ptr [00000000656970E8h]
  000000006503CF51: 44 8B C0           mov         r8d,eax
  000000006503CF54: 49 8D 8E D0 00 00  lea         rcx,[r14+00000000000000D0h]
                    00
  000000006503CF5B: 8B D7              mov         edx,edi
  000000006503CF5D: E8 7E D5 FE FF     call        000000006502A4E0
  000000006503CF62: 49 8B 86 E8 00 00  mov         rax,qword ptr [r14+00000000000000E8h]
                    00
  000000006503CF69: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CF6D: 41 3B F8           cmp         edi,r8d
  000000006503CF70: 7F 14              jg          000000006503CF86
  000000006503CF72: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006503CF76: 75 24              jne         000000006503CF9C
  000000006503CF78: 3B 78 08           cmp         edi,dword ptr [rax+8]
  000000006503CF7B: 7D 1F              jge         000000006503CF9C
  000000006503CF7D: 41 8B C0           mov         eax,r8d
  000000006503CF80: D1 F8              sar         eax,1
  000000006503CF82: 3B F8              cmp         edi,eax
  000000006503CF84: 7D 16              jge         000000006503CF9C
  000000006503CF86: 45 33 C9           xor         r9d,r9d
  000000006503CF89: 8B D7              mov         edx,edi
  000000006503CF8B: 45 8D 41 04        lea         r8d,[r9+4]
  000000006503CF8F: 41 8D 49 14        lea         ecx,[r9+14h]
  000000006503CF93: FF 15 4F A1 65 00  call        qword ptr [00000000656970E8h]
  000000006503CF99: 44 8B C0           mov         r8d,eax
  000000006503CF9C: 49 8D 8E E8 00 00  lea         rcx,[r14+00000000000000E8h]
                    00
  000000006503CFA3: 8B D7              mov         edx,edi
  000000006503CFA5: E8 B6 EF FF FF     call        000000006503BF60
  000000006503CFAA: 49 8B 86 F8 00 00  mov         rax,qword ptr [r14+00000000000000F8h]
                    00
  000000006503CFB1: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503CFB5: 41 3B F8           cmp         edi,r8d
  000000006503CFB8: 7F 14              jg          000000006503CFCE
  000000006503CFBA: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006503CFBE: 75 24              jne         000000006503CFE4
  000000006503CFC0: 3B 78 08           cmp         edi,dword ptr [rax+8]
  000000006503CFC3: 7D 1F              jge         000000006503CFE4
  000000006503CFC5: 41 8B C0           mov         eax,r8d
  000000006503CFC8: D1 F8              sar         eax,1
  000000006503CFCA: 3B F8              cmp         edi,eax
  000000006503CFCC: 7D 16              jge         000000006503CFE4
  000000006503CFCE: 45 33 C9           xor         r9d,r9d
  000000006503CFD1: 8B D7              mov         edx,edi
  000000006503CFD3: 45 8D 41 04        lea         r8d,[r9+4]
  000000006503CFD7: 41 8D 49 14        lea         ecx,[r9+14h]
  000000006503CFDB: FF 15 07 A1 65 00  call        qword ptr [00000000656970E8h]
  000000006503CFE1: 44 8B C0           mov         r8d,eax
  000000006503CFE4: 49 8D 8E F8 00 00  lea         rcx,[r14+00000000000000F8h]
                    00
  000000006503CFEB: 8B D7              mov         edx,edi
  000000006503CFED: E8 6E EF FF FF     call        000000006503BF60
  000000006503CFF2: 4D 63 9E C4 00 00  movsxd      r11,dword ptr [r14+00000000000000C4h]
                    00
  000000006503CFF9: 44 3B DF           cmp         r11d,edi
  000000006503CFFC: 0F 8D C6 01 00 00  jge         000000006503D1C8
  000000006503D002: 4C 8B EF           mov         r13,rdi
  000000006503D005: 4B 8D 3C DB        lea         rdi,[r11+r11*8]
  000000006503D009: 4A 8D 34 9D 10 00  lea         rsi,[r11*4+0000000000000010h]
                    00 00
  000000006503D011: 48 C1 E7 02        shl         rdi,2
  000000006503D015: 4D 2B EB           sub         r13,r11
  000000006503D018: 0F 1F 84 00 00 00  nop         dword ptr [rax+rax+0000000000000000h]
                    00 00
  000000006503D020: 49 8B 8E D0 00 00  mov         rcx,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D027: BA 01 00 00 00     mov         edx,1
  000000006503D02C: FF 15 3E 9A 65 00  call        qword ptr [0000000065696A70h]
  000000006503D032: 84 C0              test        al,al
  000000006503D034: 74 1A              je          000000006503D050
  000000006503D036: 49 8B 86 D0 00 00  mov         rax,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D03D: 49 8D 8E D0 00 00  lea         rcx,[r14+00000000000000D0h]
                    00
  000000006503D044: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503D048: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503D04B: E8 90 D4 FE FF     call        000000006502A4E0
  000000006503D050: 49 8B 86 D0 00 00  mov         rax,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D057: 33 C9              xor         ecx,ecx
  000000006503D059: 89 4C 38 10        mov         dword ptr [rax+rdi+10h],ecx
  000000006503D05D: 89 4C 38 14        mov         dword ptr [rax+rdi+14h],ecx
  000000006503D061: 89 4C 38 1C        mov         dword ptr [rax+rdi+1Ch],ecx
  000000006503D065: C7 44 38 18 FF FF  mov         dword ptr [rax+rdi+18h],7FFFFh
                    07 00
  000000006503D06D: 88 4C 38 20        mov         byte ptr [rax+rdi+20h],cl
  000000006503D071: C6 44 38 21 01     mov         byte ptr [rax+rdi+21h],1
  000000006503D076: 89 4C 38 24        mov         dword ptr [rax+rdi+24h],ecx
  000000006503D07A: 8D 51 01           lea         edx,[rcx+1]
  000000006503D07D: 49 8B 8E D0 00 00  mov         rcx,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D084: FF 15 E6 99 65 00  call        qword ptr [0000000065696A70h]
  000000006503D08A: 84 C0              test        al,al
  000000006503D08C: 74 1A              je          000000006503D0A8
  000000006503D08E: 49 8B 86 D0 00 00  mov         rax,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D095: 49 8D 8E D0 00 00  lea         rcx,[r14+00000000000000D0h]
                    00
  000000006503D09C: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503D0A0: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503D0A3: E8 38 D4 FE FF     call        000000006502A4E0
  000000006503D0A8: 49 8B 86 D0 00 00  mov         rax,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D0AF: BA 01 00 00 00     mov         edx,1
  000000006503D0B4: C7 44 38 18 00 00  mov         dword ptr [rax+rdi+18h],0
                    00 00
  000000006503D0BC: 49 8B 8E D0 00 00  mov         rcx,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D0C3: FF 15 A7 99 65 00  call        qword ptr [0000000065696A70h]
  000000006503D0C9: 84 C0              test        al,al
  000000006503D0CB: 74 1A              je          000000006503D0E7
  000000006503D0CD: 49 8B 86 D0 00 00  mov         rax,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D0D4: 49 8D 8E D0 00 00  lea         rcx,[r14+00000000000000D0h]
                    00
  000000006503D0DB: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503D0DF: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503D0E2: E8 F9 D3 FE FF     call        000000006502A4E0
  000000006503D0E7: 49 8B 86 D0 00 00  mov         rax,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D0EE: BA 01 00 00 00     mov         edx,1
  000000006503D0F3: C7 44 07 2C 00 00  mov         dword ptr [rdi+rax+2Ch],0
                    00 00
  000000006503D0FB: 49 8B 8E D0 00 00  mov         rcx,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D102: FF 15 68 99 65 00  call        qword ptr [0000000065696A70h]
  000000006503D108: 84 C0              test        al,al
  000000006503D10A: 74 1A              je          000000006503D126
  000000006503D10C: 49 8B 86 D0 00 00  mov         rax,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D113: 49 8D 8E D0 00 00  lea         rcx,[r14+00000000000000D0h]
                    00
  000000006503D11A: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503D11E: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503D121: E8 BA D3 FE FF     call        000000006502A4E0
  000000006503D126: 49 8B 86 D0 00 00  mov         rax,qword ptr [r14+00000000000000D0h]
                    00
  000000006503D12D: BA 01 00 00 00     mov         edx,1
  000000006503D132: C7 44 07 30 00 00  mov         dword ptr [rdi+rax+30h],0
                    00 00
  000000006503D13A: 49 8B 8E E8 00 00  mov         rcx,qword ptr [r14+00000000000000E8h]
                    00
  000000006503D141: FF 15 29 99 65 00  call        qword ptr [0000000065696A70h]
  000000006503D147: 84 C0              test        al,al
  000000006503D149: 74 1A              je          000000006503D165
  000000006503D14B: 49 8B 86 E8 00 00  mov         rax,qword ptr [r14+00000000000000E8h]
                    00
  000000006503D152: 49 8D 8E E8 00 00  lea         rcx,[r14+00000000000000E8h]
                    00
  000000006503D159: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503D15D: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503D160: E8 FB ED FF FF     call        000000006503BF60
  000000006503D165: 49 8B 86 E8 00 00  mov         rax,qword ptr [r14+00000000000000E8h]
                    00
  000000006503D16C: BA 01 00 00 00     mov         edx,1
  000000006503D171: C7 04 06 00 00 00  mov         dword ptr [rsi+rax],0
                    00
  000000006503D178: 49 8B 8E F8 00 00  mov         rcx,qword ptr [r14+00000000000000F8h]
                    00
  000000006503D17F: FF 15 EB 98 65 00  call        qword ptr [0000000065696A70h]
  000000006503D185: 84 C0              test        al,al
  000000006503D187: 74 1A              je          000000006503D1A3
  000000006503D189: 49 8B 86 F8 00 00  mov         rax,qword ptr [r14+00000000000000F8h]
                    00
  000000006503D190: 49 8D 8E F8 00 00  lea         rcx,[r14+00000000000000F8h]
                    00
  000000006503D197: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006503D19B: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006503D19E: E8 BD ED FF FF     call        000000006503BF60
  000000006503D1A3: 49 8B 86 F8 00 00  mov         rax,qword ptr [r14+00000000000000F8h]
                    00
  000000006503D1AA: 48 83 C6 04        add         rsi,4
  000000006503D1AE: 48 83 C7 24        add         rdi,24h
  000000006503D1B2: 49 83 ED 01        sub         r13,1
  000000006503D1B6: C7 44 06 FC 00 00  mov         dword ptr [rsi+rax-4],0
                    00 00
  000000006503D1BE: 0F 85 5C FE FF FF  jne         000000006503D020
  000000006503D1C4: 8B 74 24 70        mov         esi,dword ptr [rsp+70h]
  000000006503D1C8: 49 8B 9E D8 00 00  mov         rbx,qword ptr [r14+00000000000000D8h]
                    00
  000000006503D1CF: 4C 8B 6C 24 20     mov         r13,qword ptr [rsp+20h]
  000000006503D1D4: 4C 8B 64 24 28     mov         r12,qword ptr [rsp+28h]
  000000006503D1D9: 48 8B 7C 24 30     mov         rdi,qword ptr [rsp+30h]
  000000006503D1DE: 48 8B 6C 24 78     mov         rbp,qword ptr [rsp+78h]
  000000006503D1E3: 48 85 DB           test        rbx,rbx
  000000006503D1E6: 74 47              je          000000006503D22F

  Summary

      695000 .text

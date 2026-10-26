Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146F100: 4C 8B D9           mov         r11,rcx
  000000014146F103: 48 2B D1           sub         rdx,rcx
  000000014146F106: 0F 82 9E 01 00 00  jb          000000014146F2AA
  000000014146F10C: 49 83 F8 08        cmp         r8,8
  000000014146F110: 72 61              jb          000000014146F173
  000000014146F112: F6 C1 07           test        cl,7
  000000014146F115: 74 36              je          000000014146F14D
  000000014146F117: F6 C1 01           test        cl,1
  000000014146F11A: 74 0B              je          000000014146F127
  000000014146F11C: 8A 04 0A           mov         al,byte ptr [rdx+rcx]
  000000014146F11F: 49 FF C8           dec         r8
  000000014146F122: 88 01              mov         byte ptr [rcx],al
  000000014146F124: 48 FF C1           inc         rcx
  000000014146F127: F6 C1 02           test        cl,2
  000000014146F12A: 74 0F              je          000000014146F13B
  000000014146F12C: 66 8B 04 0A        mov         ax,word ptr [rdx+rcx]
  000000014146F130: 49 83 E8 02        sub         r8,2
  000000014146F134: 66 89 01           mov         word ptr [rcx],ax
  000000014146F137: 48 83 C1 02        add         rcx,2
  000000014146F13B: F6 C1 04           test        cl,4
  000000014146F13E: 74 0D              je          000000014146F14D
  000000014146F140: 8B 04 0A           mov         eax,dword ptr [rdx+rcx]
  000000014146F143: 49 83 E8 04        sub         r8,4
  000000014146F147: 89 01              mov         dword ptr [rcx],eax
  000000014146F149: 48 83 C1 04        add         rcx,4
  000000014146F14D: 4D 8B C8           mov         r9,r8
  000000014146F150: 49 C1 E9 05        shr         r9,5
  000000014146F154: 75 51              jne         000000014146F1A7
  000000014146F156: 4D 8B C8           mov         r9,r8
  000000014146F159: 49 C1 E9 03        shr         r9,3
  000000014146F15D: 74 14              je          000000014146F173
  000000014146F15F: 48 8B 04 0A        mov         rax,qword ptr [rdx+rcx]
  000000014146F163: 48 89 01           mov         qword ptr [rcx],rax
  000000014146F166: 48 83 C1 08        add         rcx,8
  000000014146F16A: 49 FF C9           dec         r9
  000000014146F16D: 75 F0              jne         000000014146F15F
  000000014146F16F: 49 83 E0 07        and         r8,7
  000000014146F173: 4D 85 C0           test        r8,r8
  000000014146F176: 75 08              jne         000000014146F180
  000000014146F178: 49 8B C3           mov         rax,r11
  000000014146F17B: C3                 ret
  000000014146F17C: 0F 1F 40 00        nop         dword ptr [rax]
  000000014146F180: 8A 04 0A           mov         al,byte ptr [rdx+rcx]
  000000014146F183: 88 01              mov         byte ptr [rcx],al
  000000014146F185: 48 FF C1           inc         rcx
  000000014146F188: 49 FF C8           dec         r8
  000000014146F18B: 75 F3              jne         000000014146F180
  000000014146F18D: 49 8B C3           mov         rax,r11
  000000014146F190: C3                 ret
  000000014146F191: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    66 0F 1F 84 00 00
                    00 00 00
  000000014146F1A0: 66 66 66 90        nop
  000000014146F1A4: 66 66 90           nop
  000000014146F1A7: 49 81 F9 00 20 00  cmp         r9,2000h
                    00
  000000014146F1AE: 73 42              jae         000000014146F1F2
  000000014146F1B0: 48 8B 04 0A        mov         rax,qword ptr [rdx+rcx]
  000000014146F1B4: 4C 8B 54 0A 08     mov         r10,qword ptr [rdx+rcx+8]
  000000014146F1B9: 48 83 C1 20        add         rcx,20h
  000000014146F1BD: 48 89 41 E0        mov         qword ptr [rcx-20h],rax
  000000014146F1C1: 4C 89 51 E8        mov         qword ptr [rcx-18h],r10
  000000014146F1C5: 48 8B 44 0A F0     mov         rax,qword ptr [rdx+rcx-10h]
  000000014146F1CA: 4C 8B 54 0A F8     mov         r10,qword ptr [rdx+rcx-8]
  000000014146F1CF: 49 FF C9           dec         r9
  000000014146F1D2: 48 89 41 F0        mov         qword ptr [rcx-10h],rax
  000000014146F1D6: 4C 89 51 F8        mov         qword ptr [rcx-8],r10
  000000014146F1DA: 75 D4              jne         000000014146F1B0
  000000014146F1DC: 49 83 E0 1F        and         r8,1Fh
  000000014146F1E0: E9 71 FF FF FF     jmp         000000014146F156
  000000014146F1E5: 66 66 66 0F 1F 84  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00 00
  000000014146F1F0: 66 90              nop
  000000014146F1F2: 48 81 FA 00 10 00  cmp         rdx,1000h
                    00
  000000014146F1F9: 72 B5              jb          000000014146F1B0
  000000014146F1FB: B8 20 00 00 00     mov         eax,20h
  000000014146F200: 0F 18 04 0A        prefetchnta [rdx+rcx]
  000000014146F204: 0F 18 44 0A 40     prefetchnta [rdx+rcx+40h]
  000000014146F209: 48 81 C1 80 00 00  add         rcx,80h
                    00
  000000014146F210: FF C8              dec         eax
  000000014146F212: 75 EC              jne         000000014146F200
  000000014146F214: 48 81 E9 00 10 00  sub         rcx,1000h
                    00
  000000014146F21B: B8 40 00 00 00     mov         eax,40h
  000000014146F220: 4C 8B 0C 0A        mov         r9,qword ptr [rdx+rcx]
  000000014146F224: 4C 8B 54 0A 08     mov         r10,qword ptr [rdx+rcx+8]
  000000014146F229: 4C 0F C3 09        movnti      qword ptr [rcx],r9
  000000014146F22D: 4C 0F C3 51 08     movnti      qword ptr [rcx+8],r10
  000000014146F232: 4C 8B 4C 0A 10     mov         r9,qword ptr [rdx+rcx+10h]
  000000014146F237: 4C 8B 54 0A 18     mov         r10,qword ptr [rdx+rcx+18h]
  000000014146F23C: 4C 0F C3 49 10     movnti      qword ptr [rcx+10h],r9
  000000014146F241: 4C 0F C3 51 18     movnti      qword ptr [rcx+18h],r10
  000000014146F246: 4C 8B 4C 0A 20     mov         r9,qword ptr [rdx+rcx+20h]
  000000014146F24B: 4C 8B 54 0A 28     mov         r10,qword ptr [rdx+rcx+28h]
  000000014146F250: 48 83 C1 40        add         rcx,40h
  000000014146F254: 4C 0F C3 49 E0     movnti      qword ptr [rcx-20h],r9
  000000014146F259: 4C 0F C3 51 E8     movnti      qword ptr [rcx-18h],r10
  000000014146F25E: 4C 8B 4C 0A F0     mov         r9,qword ptr [rdx+rcx-10h]
  000000014146F263: 4C 8B 54 0A F8     mov         r10,qword ptr [rdx+rcx-8]
  000000014146F268: FF C8              dec         eax
  000000014146F26A: 4C 0F C3 49 F0     movnti      qword ptr [rcx-10h],r9
  000000014146F26F: 4C 0F C3 51 F8     movnti      qword ptr [rcx-8],r10
  000000014146F274: 75 AA              jne         000000014146F220
  000000014146F276: 49 81 E8 00 10 00  sub         r8,1000h
                    00
  000000014146F27D: 49 81 F8 00 10 00  cmp         r8,1000h
                    00
  000000014146F284: 0F 83 71 FF FF FF  jae         000000014146F1FB
  000000014146F28A: F0 80 0C 24 00     lock or     byte ptr [rsp],0
  000000014146F28F: E9 B9 FE FF FF     jmp         000000014146F14D
  000000014146F294: 66 66 66 66 0F 1F  nop         word ptr [rax+rax+0000000000000000h]
                    84 00 00 00 00 00
  000000014146F2A0: 66 66 66 90        nop
  000000014146F2A4: 66 66 66 90        nop
  000000014146F2A8: 66 90              nop
  000000014146F2AA: 49 03 C8           add         rcx,r8
  000000014146F2AD: 49 83 F8 08        cmp         r8,8
  000000014146F2B1: 72 61              jb          000000014146F314
  000000014146F2B3: F6 C1 07           test        cl,7
  000000014146F2B6: 74 36              je          000000014146F2EE
  000000014146F2B8: F6 C1 01           test        cl,1
  000000014146F2BB: 74 0B              je          000000014146F2C8
  000000014146F2BD: 48 FF C9           dec         rcx
  000000014146F2C0: 8A 04 0A           mov         al,byte ptr [rdx+rcx]
  000000014146F2C3: 49 FF C8           dec         r8
  000000014146F2C6: 88 01              mov         byte ptr [rcx],al
  000000014146F2C8: F6 C1 02           test        cl,2
  000000014146F2CB: 74 0F              je          000000014146F2DC
  000000014146F2CD: 48 83 E9 02        sub         rcx,2
  000000014146F2D1: 66 8B 04 0A        mov         ax,word ptr [rdx+rcx]
  000000014146F2D5: 49 83 E8 02        sub         r8,2
  000000014146F2D9: 66 89 01           mov         word ptr [rcx],ax
  000000014146F2DC: F6 C1 04           test        cl,4
  000000014146F2DF: 74 0D              je          000000014146F2EE
  000000014146F2E1: 48 83 E9 04        sub         rcx,4
  000000014146F2E5: 8B 04 0A           mov         eax,dword ptr [rdx+rcx]
  000000014146F2E8: 49 83 E8 04        sub         r8,4
  000000014146F2EC: 89 01              mov         dword ptr [rcx],eax
  000000014146F2EE: 4D 8B C8           mov         r9,r8
  000000014146F2F1: 49 C1 E9 05        shr         r9,5
  000000014146F2F5: 75 50              jne         000000014146F347
  000000014146F2F7: 4D 8B C8           mov         r9,r8
  000000014146F2FA: 49 C1 E9 03        shr         r9,3
  000000014146F2FE: 74 14              je          000000014146F314
  000000014146F300: 48 83 E9 08        sub         rcx,8
  000000014146F304: 48 8B 04 0A        mov         rax,qword ptr [rdx+rcx]
  000000014146F308: 49 FF C9           dec         r9
  000000014146F30B: 48 89 01           mov         qword ptr [rcx],rax
  000000014146F30E: 75 F0              jne         000000014146F300
  000000014146F310: 49 83 E0 07        and         r8,7
  000000014146F314: 4D 85 C0           test        r8,r8
  000000014146F317: 75 07              jne         000000014146F320
  000000014146F319: 49 8B C3           mov         rax,r11
  000000014146F31C: C3                 ret
  000000014146F31D: 0F 1F 00           nop         dword ptr [rax]
  000000014146F320: 48 FF C9           dec         rcx
  000000014146F323: 8A 04 0A           mov         al,byte ptr [rdx+rcx]
  000000014146F326: 49 FF C8           dec         r8
  000000014146F329: 88 01              mov         byte ptr [rcx],al
  000000014146F32B: 75 F3              jne         000000014146F320
  000000014146F32D: 49 8B C3           mov         rax,r11

  Summary

     1E02000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670744CE: 48 89 5C 24 38     mov         qword ptr [rsp+38h],rbx
  00000000670744D3: 49 8B 59 10        mov         rbx,qword ptr [r9+10h]
  00000000670744D7: 48 89 74 24 40     mov         qword ptr [rsp+40h],rsi
  00000000670744DC: 0F B7 03           movzx       eax,word ptr [rbx]
  00000000670744DF: 48 8D 35 1A BB F8  lea         rsi,[0000000067000000h]
                    FF
  00000000670744E6: 66 83 F8 09        cmp         ax,9
  00000000670744EA: 72 06              jb          00000000670744F2
  00000000670744EC: 66 83 F8 0D        cmp         ax,0Dh
  00000000670744F0: 76 6E              jbe         0000000067074560
  00000000670744F2: 0F B7 D0           movzx       edx,ax
  00000000670744F5: 48 8B C2           mov         rax,rdx
  00000000670744F8: 83 E2 1F           and         edx,1Fh
  00000000670744FB: 48 C1 E8 05        shr         rax,5
  00000000670744FF: 0F B7 8C 46 50 40  movzx       ecx,word ptr [rsi+rax*2+00000000001F4050h]
                    1F 00
  0000000067074507: 48 03 CA           add         rcx,rdx
  000000006707450A: 0F B7 84 4E 50 40  movzx       eax,word ptr [rsi+rcx*2+00000000001F4050h]
                    1F 00
  0000000067074512: 48 8D 0C 80        lea         rcx,[rax+rax*4]
  0000000067074516: B8 01 00 00 00     mov         eax,1
  000000006707451B: 0F B6 8C 8E B0 F7  movzx       ecx,byte ptr [rsi+rcx*4+00000000001FF7B0h]
                    1F 00
  0000000067074523: D3 E0              shl         eax,cl
  0000000067074525: A9 80 03 00 00     test        eax,380h
  000000006707452A: 0F 95 C0           setne       al
  000000006707452D: 84 C0              test        al,al
  000000006707452F: 75 2F              jne         0000000067074560
  0000000067074531: 41 8D 42 FF        lea         eax,[r10-1]
  0000000067074535: 48 63 C8           movsxd      rcx,eax
  0000000067074538: 48 8D 0C 4B        lea         rcx,[rbx+rcx*2]
  000000006707453C: E8 3F 57 FF FF     call        0000000067069C80
  0000000067074541: 84 C0              test        al,al
  0000000067074543: 75 1B              jne         0000000067074560
  0000000067074545: 4C 89 0F           mov         qword ptr [rdi],r9
  0000000067074548: F0 41 83 01 01     lock add    dword ptr [r9],1
  000000006707454D: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  0000000067074552: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  0000000067074557: 48 8B C7           mov         rax,rdi
  000000006707455A: 48 83 C4 20        add         rsp,20h
  000000006707455E: 5F                 pop         rdi
  000000006707455F: C3                 ret

  Summary

      1A7000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000651F2110: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000651F2115: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000651F211A: 57                 push        rdi
  00000000651F211B: 48 83 EC 20        sub         rsp,20h
  00000000651F211F: 41 8B 40 38        mov         eax,dword ptr [r8+38h]
  00000000651F2123: 49 8B F0           mov         rsi,r8
  00000000651F2126: 8B FA              mov         edi,edx
  00000000651F2128: 89 41 38           mov         dword ptr [rcx+38h],eax
  00000000651F212B: 8B C2              mov         eax,edx
  00000000651F212D: 48 8B D9           mov         rbx,rcx
  00000000651F2130: 25 FF FF 01 00     and         eax,1FFFFh
  00000000651F2135: 3D FF FF 01 00     cmp         eax,1FFFFh
  00000000651F213A: 0F 84 3B 01 00 00  je          00000000651F227B
  00000000651F2140: F6 C2 01           test        dl,1
  00000000651F2143: 75 0E              jne         00000000651F2153
  00000000651F2145: 49 8D 50 08        lea         rdx,[r8+8]
  00000000651F2149: 48 83 C1 08        add         rcx,8
  00000000651F214D: FF 15 2D 46 4A 00  call        qword ptr [0000000065696780h]
  00000000651F2153: 0F BA E7 10        bt          edi,10h
  00000000651F2157: 72 0E              jb          00000000651F2167
  00000000651F2159: 48 8D 56 10        lea         rdx,[rsi+10h]
  00000000651F215D: 48 8D 4B 10        lea         rcx,[rbx+10h]
  00000000651F2161: FF 15 19 46 4A 00  call        qword ptr [0000000065696780h]
  00000000651F2167: 40 F6 C7 02        test        dil,2
  00000000651F216B: 75 10              jne         00000000651F217D
  00000000651F216D: 48 8B 46 18        mov         rax,qword ptr [rsi+18h]
  00000000651F2171: 48 89 43 18        mov         qword ptr [rbx+18h],rax
  00000000651F2175: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  00000000651F2179: 48 89 43 20        mov         qword ptr [rbx+20h],rax
  00000000651F217D: 40 F6 C7 04        test        dil,4
  00000000651F2181: 75 07              jne         00000000651F218A
  00000000651F2183: 0F B6 46 2A        movzx       eax,byte ptr [rsi+2Ah]
  00000000651F2187: 88 43 2A           mov         byte ptr [rbx+2Ah],al
  00000000651F218A: 40 F6 C7 08        test        dil,8
  00000000651F218E: 75 08              jne         00000000651F2198
  00000000651F2190: 0F B7 46 28        movzx       eax,word ptr [rsi+28h]
  00000000651F2194: 66 89 43 28        mov         word ptr [rbx+28h],ax
  00000000651F2198: 40 F6 C7 10        test        dil,10h
  00000000651F219C: 75 0E              jne         00000000651F21AC
  00000000651F219E: 8B 46 28           mov         eax,dword ptr [rsi+28h]
  00000000651F21A1: 33 43 28           xor         eax,dword ptr [rbx+28h]
  00000000651F21A4: 25 00 00 00 7F     and         eax,7F000000h
  00000000651F21A9: 31 43 28           xor         dword ptr [rbx+28h],eax
  00000000651F21AC: 40 F6 C7 20        test        dil,20h
  00000000651F21B0: 75 0C              jne         00000000651F21BE
  00000000651F21B2: 8B 46 2C           mov         eax,dword ptr [rsi+2Ch]
  00000000651F21B5: 33 43 2C           xor         eax,dword ptr [rbx+2Ch]
  00000000651F21B8: 83 E0 03           and         eax,3
  00000000651F21BB: 31 43 2C           xor         dword ptr [rbx+2Ch],eax
  00000000651F21BE: 0F BA E7 09        bt          edi,9
  00000000651F21C2: 72 10              jb          00000000651F21D4
  00000000651F21C4: 8B 46 28           mov         eax,dword ptr [rsi+28h]
  00000000651F21C7: 33 43 28           xor         eax,dword ptr [rbx+28h]
  00000000651F21CA: 0F BA F0 1F        btr         eax,1Fh
  00000000651F21CE: 33 46 28           xor         eax,dword ptr [rsi+28h]
  00000000651F21D1: 89 43 28           mov         dword ptr [rbx+28h],eax
  00000000651F21D4: 0F BA E7 0A        bt          edi,0Ah
  00000000651F21D8: 72 0E              jb          00000000651F21E8
  00000000651F21DA: 8B 46 2C           mov         eax,dword ptr [rsi+2Ch]
  00000000651F21DD: 33 43 2C           xor         eax,dword ptr [rbx+2Ch]
  00000000651F21E0: 25 FC 3F 00 00     and         eax,3FFCh
  00000000651F21E5: 31 43 2C           xor         dword ptr [rbx+2Ch],eax
  00000000651F21E8: 0F BA E7 0F        bt          edi,0Fh
  00000000651F21EC: 72 0E              jb          00000000651F21FC
  00000000651F21EE: 8B 46 2C           mov         eax,dword ptr [rsi+2Ch]
  00000000651F21F1: 33 43 2C           xor         eax,dword ptr [rbx+2Ch]
  00000000651F21F4: 25 00 80 01 00     and         eax,18000h
  00000000651F21F9: 31 43 2C           xor         dword ptr [rbx+2Ch],eax
  00000000651F21FC: 40 F6 C7 40        test        dil,40h
  00000000651F2200: 75 0C              jne         00000000651F220E
  00000000651F2202: 8B 46 48           mov         eax,dword ptr [rsi+48h]
  00000000651F2205: 33 43 48           xor         eax,dword ptr [rbx+48h]
  00000000651F2208: 83 E0 02           and         eax,2
  00000000651F220B: 31 43 48           xor         dword ptr [rbx+48h],eax
  00000000651F220E: 40 84 FF           test        dil,dil
  00000000651F2211: 78 0C              js          00000000651F221F
  00000000651F2213: 8B 46 48           mov         eax,dword ptr [rsi+48h]
  00000000651F2216: 33 43 48           xor         eax,dword ptr [rbx+48h]
  00000000651F2219: 83 E0 04           and         eax,4
  00000000651F221C: 31 43 48           xor         dword ptr [rbx+48h],eax
  00000000651F221F: 0F BA E7 08        bt          edi,8
  00000000651F2223: 72 0C              jb          00000000651F2231
  00000000651F2225: 8B 46 48           mov         eax,dword ptr [rsi+48h]
  00000000651F2228: 33 43 48           xor         eax,dword ptr [rbx+48h]
  00000000651F222B: 83 E0 08           and         eax,8
  00000000651F222E: 31 43 48           xor         dword ptr [rbx+48h],eax
  00000000651F2231: 0F BA E7 0B        bt          edi,0Bh
  00000000651F2235: 72 0C              jb          00000000651F2243
  00000000651F2237: 8B 46 48           mov         eax,dword ptr [rsi+48h]
  00000000651F223A: 33 43 48           xor         eax,dword ptr [rbx+48h]
  00000000651F223D: 83 E0 10           and         eax,10h
  00000000651F2240: 31 43 48           xor         dword ptr [rbx+48h],eax
  00000000651F2243: 0F BA E7 0D        bt          edi,0Dh
  00000000651F2247: 72 12              jb          00000000651F225B
  00000000651F2249: 8B 46 50           mov         eax,dword ptr [rsi+50h]
  00000000651F224C: 89 43 50           mov         dword ptr [rbx+50h],eax
  00000000651F224F: 0F B6 46 4C        movzx       eax,byte ptr [rsi+4Ch]
  00000000651F2253: 32 43 4C           xor         al,byte ptr [rbx+4Ch]
  00000000651F2256: 24 01              and         al,1
  00000000651F2258: 30 43 4C           xor         byte ptr [rbx+4Ch],al
  00000000651F225B: 0F BA E7 0E        bt          edi,0Eh
  00000000651F225F: 72 06              jb          00000000651F2267
  00000000651F2261: 8B 46 54           mov         eax,dword ptr [rsi+54h]
  00000000651F2264: 89 43 54           mov         dword ptr [rbx+54h],eax
  00000000651F2267: 0F BA E7 0C        bt          edi,0Ch
  00000000651F226B: 72 0E              jb          00000000651F227B
  00000000651F226D: 8B 46 48           mov         eax,dword ptr [rsi+48h]
  00000000651F2270: 33 43 48           xor         eax,dword ptr [rbx+48h]
  00000000651F2273: 25 E0 00 00 00     and         eax,0E0h
  00000000651F2278: 31 43 48           xor         dword ptr [rbx+48h],eax
  00000000651F227B: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000651F2280: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  00000000651F2285: 48 83 C4 20        add         rsp,20h
  00000000651F2289: 5F                 pop         rdi
  00000000651F228A: C3                 ret

  Summary

      695000 .text

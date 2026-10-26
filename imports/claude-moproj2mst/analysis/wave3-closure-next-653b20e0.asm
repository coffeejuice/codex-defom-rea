Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653B20E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000653B20E5: 57                 push        rdi
  00000000653B20E6: 48 83 EC 40        sub         rsp,40h
  00000000653B20EA: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653B20F3: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000653B20F8: 48 8B D9           mov         rbx,rcx
  00000000653B20FB: BA 04 08 04 00     mov         edx,40804h
  00000000653B2100: E8 7B C6 CA FF     call        000000006505E780
  00000000653B2105: 90                 nop
  00000000653B2106: 48 8D 05 63 D8 34  lea         rax,[00000000656FF970h]
                    00
  00000000653B210D: 48 89 03           mov         qword ptr [rbx],rax
  00000000653B2110: 48 8D 8B E0 01 00  lea         rcx,[rbx+00000000000001E0h]
                    00
  00000000653B2117: 33 FF              xor         edi,edi
  00000000653B2119: 89 7C 24 20        mov         dword ptr [rsp+20h],edi
  00000000653B211D: 45 33 C9           xor         r9d,r9d
  00000000653B2120: 45 33 C0           xor         r8d,r8d
  00000000653B2123: 33 D2              xor         edx,edx
  00000000653B2125: FF 15 45 46 2E 00  call        qword ptr [0000000065696770h]
  00000000653B212B: C7 83 F0 01 00 00  mov         dword ptr [rbx+00000000000001F0h],10h
                    10 00 00 00
  00000000653B2135: B8 01 00 00 00     mov         eax,1
  00000000653B213A: 66 89 83 F4 01 00  mov         word ptr [rbx+00000000000001F4h],ax
                    00
  00000000653B2141: 66 89 BB F6 01 00  mov         word ptr [rbx+00000000000001F6h],di
                    00
  00000000653B2148: 66 89 BB F8 01 00  mov         word ptr [rbx+00000000000001F8h],di
                    00
  00000000653B214F: 66 89 BB FA 01 00  mov         word ptr [rbx+00000000000001FAh],di
                    00
  00000000653B2156: 66 89 BB FC 01 00  mov         word ptr [rbx+00000000000001FCh],di
                    00
  00000000653B215D: 66 89 BB FE 01 00  mov         word ptr [rbx+00000000000001FEh],di
                    00
  00000000653B2164: 66 89 BB 00 02 00  mov         word ptr [rbx+0000000000000200h],di
                    00
  00000000653B216B: 48 8B C3           mov         rax,rbx
  00000000653B216E: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000653B2173: 48 83 C4 40        add         rsp,40h
  00000000653B2177: 5F                 pop         rdi
  00000000653B2178: C3                 ret

  Summary

      695000 .text

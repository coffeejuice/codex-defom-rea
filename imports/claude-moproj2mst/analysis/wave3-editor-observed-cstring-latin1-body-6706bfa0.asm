Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006706BFA0: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  000000006706BFA5: 48 63 EF           movsxd      rbp,edi
  000000006706BFA8: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  000000006706BFAD: 48 8D 4C 2D 20     lea         rcx,[rbp+rbp+20h]
  000000006706BFB2: E8 E9 43 FB FF     call        00000000670203A0
  000000006706BFB7: 48 8B F0           mov         rsi,rax
  000000006706BFBA: 48 85 C0           test        rax,rax
  000000006706BFBD: 75 05              jne         000000006706BFC4
  000000006706BFBF: E8 EC FF FA FF     call        000000006701BFB0
  000000006706BFC4: 48 8D 4E 1A        lea         rcx,[rsi+1Ah]
  000000006706BFC8: 33 D2              xor         edx,edx
  000000006706BFCA: B8 E0 FF 00 00     mov         eax,0FFE0h
  000000006706BFCF: 66 21 46 18        and         word ptr [rsi+18h],ax
  000000006706BFD3: 83 FF 10           cmp         edi,10h
  000000006706BFD6: 48 89 4E 10        mov         qword ptr [rsi+10h],rcx
  000000006706BFDA: C7 06 01 00 00 00  mov         dword ptr [rsi],1
  000000006706BFE0: 89 7E 08           mov         dword ptr [rsi+8],edi
  000000006706BFE3: 89 7E 04           mov         dword ptr [rsi+4],edi
  000000006706BFE6: 66 89 54 6E 1A     mov         word ptr [rsi+rbp*2+1Ah],dx
  000000006706BFEB: 48 8B 4E 10        mov         rcx,qword ptr [rsi+10h]
  000000006706BFEF: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  000000006706BFF4: 7C 50              jl          000000006706C046

  Summary

      1A7000 .text

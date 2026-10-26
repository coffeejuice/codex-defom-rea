Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065473AD0: test        ecx,ecx
  0000000065473AD2: je          0000000065473B34
  0000000065473AD4: test        ecx,0FFFFFC00h
  0000000065473ADA: jne         0000000065473B34
  0000000065473ADC: movzx       eax,cl
  0000000065473ADF: dec         eax
  0000000065473AE1: cmp         eax,8
  0000000065473AE4: ja          0000000065473B31
  0000000065473AE6: lea         rdx,[0000000065000000h]
  0000000065473AED: cdqe
  0000000065473AEF: mov         ecx,dword ptr [rdx+rax*4+0000000000473B40h]
  0000000065473AF6: add         rcx,rdx
  0000000065473AF9: jmp         rcx
  0000000065473AFB: mov         eax,400h
  0000000065473B00: ret
  0000000065473B01: mov         eax,400000h
  0000000065473B06: ret
  0000000065473B07: mov         eax,4000h
  0000000065473B0C: ret
  0000000065473B0D: mov         eax,10000h
  0000000065473B12: ret
  0000000065473B13: mov         eax,40000h
  0000000065473B18: ret
  0000000065473B19: mov         eax,80000h
  0000000065473B1E: ret
  0000000065473B1F: mov         eax,100000h
  0000000065473B24: ret
  0000000065473B25: mov         eax,8000h
  0000000065473B2A: ret
  0000000065473B2B: mov         eax,20000h
  0000000065473B30: ret

  Summary

      695000 .text

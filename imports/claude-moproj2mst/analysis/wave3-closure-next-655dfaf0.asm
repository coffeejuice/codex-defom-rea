Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000655DFAF0: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  00000000655DFAF4: 8B 40 08           mov         eax,dword ptr [rax+8]
  00000000655DFAF7: C1 E8 0F           shr         eax,0Fh
  00000000655DFAFA: 24 01              and         al,1
  00000000655DFAFC: C3                 ret
  00000000655DFAFD: CC                 int         3
  00000000655DFAFE: CC                 int         3
  00000000655DFAFF: CC                 int         3
  00000000655DFB00: 84 D2              test        dl,dl
  00000000655DFB02: 74 0D              je          00000000655DFB11
  00000000655DFB04: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  00000000655DFB08: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  00000000655DFB0C: 74 03              je          00000000655DFB11
  00000000655DFB0E: 33 C0              xor         eax,eax

  Summary

      695000 .text

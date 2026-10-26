Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650CE980: 48 8B 51 10        mov         rdx,qword ptr [rcx+10h]
  00000000650CE984: 83 7A 20 01        cmp         dword ptr [rdx+20h],1
  00000000650CE988: 0F 94 C0           sete        al
  00000000650CE98B: C3                 ret
  00000000650CE98C: CC                 int         3
  00000000650CE98D: CC                 int         3
  00000000650CE98E: CC                 int         3
  00000000650CE98F: CC                 int         3
  00000000650CE990: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000650CE994: 48 85 C0           test        rax,rax
  00000000650CE997: 74 09              je          00000000650CE9A2
  00000000650CE999: 80 78 14 00        cmp         byte ptr [rax+14h],0
  00000000650CE99D: 75 03              jne         00000000650CE9A2
  00000000650CE99F: 32 C0              xor         al,al
  00000000650CE9A1: C3                 ret
  00000000650CE9A2: B0 01              mov         al,1
  00000000650CE9A4: C3                 ret
  00000000650CE9A5: CC                 int         3
  00000000650CE9A6: CC                 int         3
  00000000650CE9A7: CC                 int         3
  00000000650CE9A8: CC                 int         3
  00000000650CE9A9: CC                 int         3
  00000000650CE9AA: CC                 int         3
  00000000650CE9AB: CC                 int         3
  00000000650CE9AC: CC                 int         3
  00000000650CE9AD: CC                 int         3
  00000000650CE9AE: CC                 int         3
  00000000650CE9AF: CC                 int         3
  00000000650CE9B0: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000650CE9B4: 48 85 C0           test        rax,rax
  00000000650CE9B7: 74 04              je          00000000650CE9BD
  00000000650CE9B9: 8B 40 08           mov         eax,dword ptr [rax+8]
  00000000650CE9BC: C3                 ret
  00000000650CE9BD: 33 C0              xor         eax,eax
  00000000650CE9BF: C3                 ret
  00000000650CE9C0: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000000650CE9C4: 48 85 C0           test        rax,rax
  00000000650CE9C7: 74 04              je          00000000650CE9CD
  00000000650CE9C9: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  00000000650CE9CC: C3                 ret
  00000000650CE9CD: 33 C0              xor         eax,eax
  00000000650CE9CF: C3                 ret
  00000000650CE9D0: 40 53              push        rbx
  00000000650CE9D2: 48 83 EC 20        sub         rsp,20h
  00000000650CE9D6: 48 83 79 10 00     cmp         qword ptr [rcx+10h],0
  00000000650CE9DB: 48 8B DA           mov         rbx,rdx
  00000000650CE9DE: 74 12              je          00000000650CE9F2

  Summary

      695000 .text

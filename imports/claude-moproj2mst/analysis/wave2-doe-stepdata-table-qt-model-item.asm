
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065529390: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  0000000065529394: 48 8B 88 C8 00 00  mov         rcx,qword ptr [rax+00000000000000C8h]
                    00
  000000006552939B: 4C 8B 49 08        mov         r9,qword ptr [rcx+8]
  000000006552939F: 85 D2              test        edx,edx
  00000000655293A1: 78 2C              js          00000000655293CF
  00000000655293A3: 45 85 C0           test        r8d,r8d
  00000000655293A6: 78 27              js          00000000655293CF
  00000000655293A8: 41 3B 51 28        cmp         edx,dword ptr [r9+28h]
  00000000655293AC: 7D 21              jge         00000000655293CF
  00000000655293AE: 41 8B 41 2C        mov         eax,dword ptr [r9+2Ch]
  00000000655293B2: 44 3B C0           cmp         r8d,eax
  00000000655293B5: 7D 18              jge         00000000655293CF
  00000000655293B7: 0F AF C2           imul        eax,edx
  00000000655293BA: 41 03 C0           add         eax,r8d
  00000000655293BD: 83 F8 FF           cmp         eax,0FFFFFFFFh
  00000000655293C0: 74 0D              je          00000000655293CF
  00000000655293C2: 48 63 C8           movsxd      rcx,eax
  00000000655293C5: 49 8B 41 20        mov         rax,qword ptr [r9+20h]
  00000000655293C9: 48

  Summary

      695000 .text

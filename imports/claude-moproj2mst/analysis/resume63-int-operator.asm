Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C60E0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  00000000670C60E5: 57                 push        rdi
  00000000670C60E6: 48 83 EC 20        sub         rsp,20h
  00000000670C60EA: 48 8B D9           mov         rbx,rcx
  00000000670C60ED: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000000670C60F1: 48 8B FA           mov         rdi,rdx
  00000000670C60F4: 48 83 79 28 00     cmp         qword ptr [rcx+28h],0
  00000000670C60F9: 75 20              jne         00000000670C611B
  00000000670C60FB: 48 83 39 00        cmp         qword ptr [rcx],0
  00000000670C60FF: 75 1A              jne         00000000670C611B
  00000000670C6101: 48 8D 0D C0 62 16  lea         rcx,[000000006722C3C8h]
                    00
  00000000670C6108: E8 D3 6D F5 FF     call        000000006701CEE0
  00000000670C610D: 48 8B C3           mov         rax,rbx
  00000000670C6110: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670C6115: 48 83 C4 20        add         rsp,20h
  00000000670C6119: 5F                 pop         rdi
  00000000670C611A: C3                 ret
  00000000670C611B: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000670C6120: E8 EB F0 FF FF     call        00000000670C5210
  00000000670C6125: 85 C0              test        eax,eax
  00000000670C6127: 74 3D              je          00000000670C6166
  00000000670C6129: FF C8              dec         eax
  00000000670C612B: 83 F8 01           cmp         eax,1
  00000000670C612E: 77 3C              ja          00000000670C616C
  00000000670C6130: 48 8B CB           mov         rcx,rbx
  00000000670C6133: C7 07 00 00 00 00  mov         dword ptr [rdi],0
  00000000670C6139: E8 22 C5 FF FF     call        00000000670C2660
  00000000670C613E: F6 D8              neg         al
  00000000670C6140: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  00000000670C6144: 1B C9              sbb         ecx,ecx
  00000000670C6146: 83 C1 02           add         ecx,2
  00000000670C6149: 83 B8 CC 00 00 00  cmp         dword ptr [rax+00000000000000CCh],0
                    00
  00000000670C6150: 75 1A              jne         00000000670C616C
  00000000670C6152: 89 88 CC 00 00 00  mov         dword ptr [rax+00000000000000CCh],ecx
  00000000670C6158: 48 8B C3           mov         rax,rbx
  00000000670C615B: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670C6160: 48 83 C4 20        add         rsp,20h
  00000000670C6164: 5F                 pop         rdi
  00000000670C6165: C3                 ret
  00000000670C6166: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  00000000670C616A: 89 07              mov         dword ptr [rdi],eax
  00000000670C616C: 48 8B C3           mov         rax,rbx
  00000000670C616F: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670C6174: 48 83 C4 20        add         rsp,20h
  00000000670C6178: 5F                 pop         rdi
  00000000670C6179: C3                 ret
  00000000670C617A: CC                 int         3
  00000000670C617B: CC                 int         3
  00000000670C617C: CC                 int         3
  00000000670C617D: CC                 int         3
  00000000670C617E: CC                 int         3
  00000000670C617F: CC                 int         3
  00000000670C6180: 48

  Summary

      1A7000 .text

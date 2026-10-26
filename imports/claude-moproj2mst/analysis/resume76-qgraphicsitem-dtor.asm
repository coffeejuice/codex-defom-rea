Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065558950: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065558955: 53                 push        rbx
  0000000065558956: 55                 push        rbp
  0000000065558957: 56                 push        rsi
  0000000065558958: 57                 push        rdi
  0000000065558959: 41 54              push        r12
  000000006555895B: 41 55              push        r13
  000000006555895D: 41 56              push        r14
  000000006555895F: 41 57              push        r15
  0000000065558961: 48 83 EC 68        sub         rsp,68h
  0000000065558965: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006555896E: 4C 8B E9           mov         r13,rcx
  0000000065558971: 33 ED              xor         ebp,ebp
  0000000065558973: 8B FD              mov         edi,ebp
  0000000065558975: 89 AC 24 B8 00 00  mov         dword ptr [rsp+00000000000000B8h],ebp
                    00
  000000006555897C: 48 8D 05 65 B1 1E  lea         rax,[0000000065743AE8h]
                    00
  0000000065558983: 48 89 01           mov         qword ptr [rcx],rax
  0000000065558986: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006555898A: 0F BA A0 3C 01 00  bt          dword ptr [rax+000000000000013Ch],17h
                    00 17
  0000000065558992: 73 2C              jae         00000000655589C0
  0000000065558994: 48 83 C1 F0        add         rcx,0FFFFFFFFFFFFFFF0h
  0000000065558998: FF 15 D2 01 14 00  call        qword ptr [0000000065698B70h]
  000000006555899E: 48 8B D8           mov         rbx,rax
  00000000655589A1: 83 48 20 08        or          dword ptr [rax+20h],8
  00000000655589A5: 48 8B 48 78        mov         rcx,qword ptr [rax+78h]
  00000000655589A9: 48 85 C9           test        rcx,rcx
  00000000655589AC: 74 12              je          00000000655589C0
  00000000655589AE: 49 8D 55 F0        lea         rdx,[r13-10h]
  00000000655589B2: 4C 8B 05 2F EB 13  mov         r8,qword ptr [00000000656974E8h]
                    00
  00000000655589B9: 41 FF 10           call        qword ptr [r8]
  00000000655589BC: 48 89 6B 78        mov         qword ptr [rbx+78h],rbp
  00000000655589C0: 49 8B 45 08        mov         rax,qword ptr [r13+8]
  00000000655589C4: 81 88 3C 01 00 00  or          dword ptr [rax+000000000000013Ch],400000h
                    00 00 40 00
  00000000655589CE: 49 8B 4D 08        mov         rcx,qword ptr [r13+8]
  00000000655589D2: E8 A9 CB FF FF     call        0000000065555580
  00000000655589D7: 49 8B 45 08        mov         rax,qword ptr [r13+8]
  00000000655589DB: 0F BA A0 3C 01 00  bt          dword ptr [rax+000000000000013Ch],17h
                    00 17
  00000000655589E3: 0F 83 1A 03 00 00  jae         0000000065558D03
  00000000655589E9: 48 8B 80 30 01 00  mov         rax,qword ptr [rax+0000000000000130h]
                    00
  00000000655589F0: 83 78 70 00        cmp         dword ptr [rax+70h],0
  00000000655589F4: 0F 84 09 03 00 00  je          0000000065558D03
  00000000655589FA: 49 8D 45 F0        lea         rax,[r13-10h]
  00000000655589FE: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000065558A06: E8 A5 E9 AB FF     call        00000000650173B0
  0000000065558A0B: 4C 8B F8           mov         r15,rax
  0000000065558A0E: 48 85 C0           test        rax,rax
  0000000065558A11: 0F 84 EC 02 00 00  je          0000000065558D03
  0000000065558A17: 49 8B 4D 08        mov         rcx,qword ptr [r13+8]
  0000000065558A1B: 48
  0000000065558A1C: 81
  0000000065558A1D: C1 30 01           sal         dword ptr [rax],1
  0000000065558A20: 00

  Summary

      695000 .text

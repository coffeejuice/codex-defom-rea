Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411E1820: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411E1825: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  00000001411E182C: 48 C7 44 24 68 FE  mov         qword ptr [rsp+68h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001411E1835: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001411E183A: FF 15 80 2C C2 00  call        qword ptr [0000000141E044C0h]
  00000001411E1840: 90                 nop
  00000001411E1841: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411E1849: 48 83 78 10 00     cmp         qword ptr [rax+10h],0
  00000001411E184E: 74 16              je          00000001411E1866
  00000001411E1850: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411E1858: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000001411E185C: E8 2F 75 41 FF     call        00000001405F8D90
  00000001411E1861: 48 85 C0           test        rax,rax
  00000001411E1864: 75 1C              jne         00000001411E1882
  00000001411E1866: C7 44 24 30 FF FF  mov         dword ptr [rsp+30h],0FFFFFFFFh
                    FF FF
  00000001411E186E: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001411E1873: FF 15 67 41 C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E1879: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  00000001411E187D: E9 D2 00 00 00     jmp         00000001411E1954
  00000001411E1882: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411E188A: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000001411E188E: E8 FD 74 41 FF     call        00000001405F8D90
  00000001411E1893: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001411E1898: 48 8B C8           mov         rcx,rax
  00000001411E189B: E8 50 F5 E6 FE     call        0000000140050DF0
  00000001411E18A0: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001411E18A5: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  00000001411E18AA: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001411E18AF: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  00000001411E18B4: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001411E18B9: FF 15 B1 2B C2 00  call        qword ptr [0000000141E04470h]
  00000001411E18BF: 90                 nop
  00000001411E18C0: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001411E18C5: FF 15 15 41 C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E18CB: 48 8D 44 24 40     lea         rax,[rsp+40h]
  00000001411E18D0: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001411E18D5: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001411E18DA: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411E18DF: FF 15 F3 40 C2 00  call        qword ptr [0000000141E059D8h]
  00000001411E18E5: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001411E18ED: 48 8B 94 24 80 00  mov         rdx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001411E18F5: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411E18FD: E8 6E 00 00 00     call        00000001411E1970
  00000001411E1902: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001411E1906: 48 8D 44 24 50     lea         rax,[rsp+50h]
  00000001411E190B: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001411E1910: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001411E1915: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001411E191A: FF 15 B8 40 C2 00  call        qword ptr [0000000141E059D8h]
  00000001411E1920: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001411E1928: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001411E1930: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411E1938: E8 03 F5 FF FF     call        00000001411E0E40
  00000001411E193D: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  00000001411E1941: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000001411E1945: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001411E194A: FF 15 90 40 C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E1950: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  00000001411E1954: 48 81 C4 98 00 00  add         rsp,98h
                    00
  00000001411E195B: C3                 ret

  Summary

     1E02000 .text

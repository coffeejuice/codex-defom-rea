Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711AAE0: 48 85 C9           test        rcx,rcx
  000000006711AAE3: 0F 84 9C 00 00 00  je          000000006711AB85
  000000006711AAE9: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006711AAEE: 48 83 EC 48        sub         rsp,48h
  000000006711AAF2: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711AAFB: 48 8B 0D 1E 97 1D  mov         rcx,qword ptr [00000000672F4220h]
                    00
  000000006711AB02: 48 85 C9           test        rcx,rcx
  000000006711AB05: 0F 95 C0           setne       al
  000000006711AB08: 84 C0              test        al,al
  000000006711AB0A: 75 15              jne         000000006711AB21
  000000006711AB0C: 48 8D 15 FD F3 11  lea         rdx,[0000000067239F10h]
                    00
  000000006711AB13: 48 8D 0D F6 EC 11  lea         rcx,[0000000067239810h]
                    00
  000000006711AB1A: E8 C1 23 F0 FF     call        000000006701CEE0
  000000006711AB1F: EB 60              jmp         000000006711AB81
  000000006711AB21: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006711AB25: 48 81 C1 98 00 00  add         rcx,98h
                    00
  000000006711AB2C: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006711AB31: E8 FA 05 F1 FF     call        000000006702B130
  000000006711AB36: 85 C0              test        eax,eax
  000000006711AB38: 74 47              je          000000006711AB81
  000000006711AB3A: 80 3D BC 96 1D 00  cmp         byte ptr [00000000672F41FDh],0
                    00
  000000006711AB41: 75 3E              jne         000000006711AB81
  000000006711AB43: BA 59 00 00 00     mov         edx,59h
  000000006711AB48: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006711AB4D: E8 FE 29 00 00     call        000000006711D550
  000000006711AB52: 90                 nop
  000000006711AB53: B8 FD FF 00 00     mov         eax,0FFFDh
  000000006711AB58: 66 21 44 24 3A     and         word ptr [rsp+3Ah],ax
  000000006711AB5D: 48 8B 0D BC 96 1D  mov         rcx,qword ptr [00000000672F4220h]
                    00
  000000006711AB64: 48 85 C9           test        rcx,rcx
  000000006711AB67: 74 0E              je          000000006711AB77
  000000006711AB69: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  000000006711AB6E: 48 8B D1           mov         rdx,rcx
  000000006711AB71: E8 FA E9 FF FF     call        0000000067119570
  000000006711AB76: 90                 nop
  000000006711AB77: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006711AB7C: E8 FF 29 00 00     call        000000006711D580
  000000006711AB81: 48 83 C4 48        add         rsp,48h
  000000006711AB85: F3 C3              rep ret

  Summary

      1A7000 .text

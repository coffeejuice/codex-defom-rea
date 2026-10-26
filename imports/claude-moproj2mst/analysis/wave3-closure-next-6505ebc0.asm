Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505EBC0: 40 53              push        rbx
  000000006505EBC2: 48 83 EC 20        sub         rsp,20h
  000000006505EBC6: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006505EBCA: 81 89 D8 01 00 00  or          dword ptr [rcx+00000000000001D8h],10000h
                    00 00 01 00
  000000006505EBD4: E8 77 FF FF FF     call        000000006505EB50
  000000006505EBD9: 4C 8B 5B 20        mov         r11,qword ptr [rbx+20h]
  000000006505EBDD: 41 F6 43 0C 01     test        byte ptr [r11+0Ch],1
  000000006505EBE2: 75 3C              jne         000000006505EC20
  000000006505EBE4: 48 8B CB           mov         rcx,rbx
  000000006505EBE7: FF 15 7B 7C 63 00  call        qword ptr [0000000065696868h]
  000000006505EBED: 48 85 C0           test        rax,rax
  000000006505EBF0: 74 2E              je          000000006505EC20
  000000006505EBF2: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006505EBF6: 8B 81 D8 01 00 00  mov         eax,dword ptr [rcx+00000000000001D8h]
  000000006505EBFC: 0F BA E0 10        bt          eax,10h
  000000006505EC00: 72 1E              jb          000000006505EC20
  000000006505EC02: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006505EC06: 0F BA E8 10        bts         eax,10h
  000000006505EC0A: 89 81 D8 01 00 00  mov         dword ptr [rcx+00000000000001D8h],eax
  000000006505EC10: E8 3B FF FF FF     call        000000006505EB50
  000000006505EC15: 4C 8B 5B 20        mov         r11,qword ptr [rbx+20h]
  000000006505EC19: 41 F6 43 0C 01     test        byte ptr [r11+0Ch],1
  000000006505EC1E: 74 C4              je          000000006505EBE4
  000000006505EC20: 48 83 C4 20        add         rsp,20h
  000000006505EC24: 5B                 pop         rbx
  000000006505EC25: C3                 ret

  Summary

      695000 .text

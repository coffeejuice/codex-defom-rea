Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711D640: B8 AA 00 00 00     mov         eax,0AAh
  000000006711D645: 48 C7 41 08 00 00  mov         qword ptr [rcx+8],0
                    00 00
  000000006711D64D: 66 89 41 10        mov         word ptr [rcx+10h],ax
  000000006711D651: B8 FC FF 00 00     mov         eax,0FFFCh
  000000006711D656: 66 21 41 12        and         word ptr [rcx+12h],ax
  000000006711D65A: 66 83 49 12 04     or          word ptr [rcx+12h],4
  000000006711D65F: 48 8D 05 7A CE 11  lea         rax,[000000006723A4E0h]
                    00
  000000006711D666: 48 89 01           mov         qword ptr [rcx],rax
  000000006711D669: 48 8B 02           mov         rax,qword ptr [rdx]
  000000006711D66C: 48 89 41 18        mov         qword ptr [rcx+18h],rax
  000000006711D670: F0 83 00 01        lock add    dword ptr [rax],1
  000000006711D674: 48 8B C1           mov         rax,rcx
  000000006711D677: C3                 ret
  000000006711D678: CC                 int         3
  000000006711D679: CC                 int         3
  000000006711D67A: CC                 int         3
  000000006711D67B: CC                 int         3
  000000006711D67C: CC                 int         3
  000000006711D67D: CC                 int         3
  000000006711D67E: CC                 int         3
  000000006711D67F: CC                 int         3
  000000006711D680: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006711D685: 53                 push        rbx
  000000006711D686: 48 83 EC 30        sub         rsp,30h
  000000006711D68A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711D693: 48 8B D9           mov         rbx,rcx
  000000006711D696: 48 8D 05 43 CE 11  lea         rax,[000000006723A4E0h]
                    00
  000000006711D69D: 48 89 01           mov         qword ptr [rcx],rax
  000000006711D6A0: 48 8B 51 18        mov         rdx,qword ptr [rcx+18h]
  000000006711D6A4: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  000000006711D6A8: 75 0A              jne         000000006711D6B4
  000000006711D6AA: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  000000006711D6AE: E8 FD 2C F0 FF     call        00000000670203B0
  000000006711D6B3: 90                 nop
  000000006711D6B4: 48 8D 05 25 BB 08  lea         rax,[00000000671A91E0h]
                    00
  000000006711D6BB: 48 89 03           mov         qword ptr [rbx],rax
  000000006711D6BE: F6
  000000006711D6BF: 43

  Summary

      1A7000 .text

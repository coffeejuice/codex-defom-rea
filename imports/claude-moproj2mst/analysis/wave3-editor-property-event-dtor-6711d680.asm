Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

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
  000000006711D6BE: F6 43 12 01        test        byte ptr [rbx+12h],1
  000000006711D6C2: 74 12              je          000000006711D6D6
  000000006711D6C4: 48 83 3D 54 6B 1D  cmp         qword ptr [00000000672F4220h],0
                    00 00
  000000006711D6CC: 74 08              je          000000006711D6D6
  000000006711D6CE: 48 8B CB           mov         rcx,rbx
  000000006711D6D1: E8 9A C2 FF FF     call        0000000067119970
  000000006711D6D6: 48 83 C4 30        add         rsp,30h
  000000006711D6DA: 5B                 pop         rbx
  000000006711D6DB: C3                 ret

  Summary

      1A7000 .text

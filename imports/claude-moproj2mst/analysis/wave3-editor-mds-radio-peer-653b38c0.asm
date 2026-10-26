Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653B38C0: 40 53              push        rbx
  00000000653B38C2: 48 83 EC 20        sub         rsp,20h
  00000000653B38C6: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000653B38CA: 33 C0              xor         eax,eax
  00000000653B38CC: C7 81 E8 01 00 00  mov         dword ptr [rcx+00000000000001E8h],1
                    01 00 00 00
  00000000653B38D6: 89 81 EC 01 00 00  mov         dword ptr [rcx+00000000000001ECh],eax
  00000000653B38DC: 88 81 F0 01 00 00  mov         byte ptr [rcx+00000000000001F0h],al
  00000000653B38E2: 88 81 F1 01 00 00  mov         byte ptr [rcx+00000000000001F1h],al
  00000000653B38E8: 88 81 F3 01 00 00  mov         byte ptr [rcx+00000000000001F3h],al
  00000000653B38EE: 88 81 F4 01 00 00  mov         byte ptr [rcx+00000000000001F4h],al
  00000000653B38F4: 89 81 F8 01 00 00  mov         dword ptr [rcx+00000000000001F8h],eax
  00000000653B38FA: C6 81 F2 01 00 00  mov         byte ptr [rcx+00000000000001F2h],1
                    01
  00000000653B3901: E8 9A F9 FF FF     call        00000000653B32A0
  00000000653B3906: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000653B390B: BA 20 00 00 00     mov         edx,20h
  00000000653B3910: C7 44 24 30 55 00  mov         dword ptr [rsp+30h],55h
                    00 00
  00000000653B3918: E8 73 03 C9 FF     call        0000000065043C90
  00000000653B391D: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  00000000653B3921: 48 8B CB           mov         rcx,rbx
  00000000653B3924: E8 F7 A1 CB FF     call        000000006506DB20
  00000000653B3929: 48 83 C4 20        add         rsp,20h
  00000000653B392D: 5B                 pop         rbx
  00000000653B392E: C3                 ret

  Summary

      695000 .text

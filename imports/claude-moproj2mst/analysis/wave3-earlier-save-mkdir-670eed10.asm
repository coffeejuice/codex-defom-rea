Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670EED10: 40 53              push        rbx
  00000000670EED12: 48 83 EC 20        sub         rsp,20h
  00000000670EED16: 48 8B D9           mov         rbx,rcx
  00000000670EED19: E8 E2 E9 FB FF     call        00000000670AD700
  00000000670EED1E: 48 8D 05 0B 1E 20  lea         rax,[00000000672F0B30h]
                    00
  00000000670EED25: 48 89 43 08        mov         qword ptr [rbx+8],rax
  00000000670EED29: F0 83 05 FF 1D 20  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670EED31: B8 FE FF FF FF     mov         eax,0FFFFFFFEh
  00000000670EED36: 66 89 43 10        mov         word ptr [rbx+10h],ax
  00000000670EED3A: 66 89 43 12        mov         word ptr [rbx+12h],ax
  00000000670EED3E: 33 C0              xor         eax,eax
  00000000670EED40: 66 89 43 14        mov         word ptr [rbx+14h],ax
  00000000670EED44: 48 8B C3           mov         rax,rbx
  00000000670EED47: 48 83 C4 20        add         rsp,20h
  00000000670EED4B: 5B                 pop         rbx
  00000000670EED4C: C3                 ret

  Summary

      1A7000 .text

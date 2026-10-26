Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670A7C83: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  00000000670A7C88: E8 F9 0D 0D 00     call        0000000067178A86
  00000000670A7C8D: 48 8B D8           mov         rbx,rax
  00000000670A7C90: 48 85 C0           test        rax,rax
  00000000670A7C93: 74 14              je          00000000670A7CA9
  00000000670A7C95: 48 8D 05 B4 8D 24  lea         rax,[00000000672F0A50h]
                    00
  00000000670A7C9C: 48 89 03           mov         qword ptr [rbx],rax
  00000000670A7C9F: F0 83 05 A9 8D 24  lock add    dword ptr [00000000672F0A50h],1
                    00 01
  00000000670A7CA7: EB 02              jmp         00000000670A7CAB
  00000000670A7CA9: 33 DB              xor         ebx,ebx
  00000000670A7CAB: 33 C0              xor         eax,eax
  00000000670A7CAD: F0 48 0F B1 1D 52  lock cmpxchg qword ptr [00000000672F3F08h],rbx
                    C2 24 00
  00000000670A7CB6: 74 26              je          00000000670A7CDE
  00000000670A7CB8: 48 85 DB           test        rbx,rbx
  00000000670A7CBB: 74 4E              je          00000000670A7D0B
  00000000670A7CBD: 48 8B CB           mov         rcx,rbx
  00000000670A7CC0: E8 1B FF FF FF     call        00000000670A7BE0
  00000000670A7CC5: 48 8B CB           mov         rcx,rbx
  00000000670A7CC8: E8 89 0C 0D 00     call        0000000067178956
  00000000670A7CCD: 48 8B 05 34 C2 24  mov         rax,qword ptr [00000000672F3F08h]
                    00
  00000000670A7CD4: 48 8B 5C 24 20     mov         rbx,qword ptr [rsp+20h]
  00000000670A7CD9: 48 83 C4 28        add         rsp,28h
  00000000670A7CDD: C3                 ret

  Summary

      1A7000 .text

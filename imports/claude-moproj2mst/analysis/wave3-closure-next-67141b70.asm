Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067141B70: 48 83 EC 28        sub         rsp,28h
  0000000067141B74: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000067141B78: F0 83 81 A8 00 00  lock add    dword ptr [rcx+00000000000000A8h],1
                    00 01
  0000000067141B80: 48 83 B9 98 00 00  cmp         qword ptr [rcx+0000000000000098h],0
                    00 00
  0000000067141B88: 74 29              je          0000000067141BB3
  0000000067141B8A: BA 01 00 00 00     mov         edx,1
  0000000067141B8F: 33 C0              xor         eax,eax
  0000000067141B91: F0 0F B1 91 B4 00  lock cmpxchg dword ptr [rcx+00000000000000B4h],edx
                    00 00
  0000000067141B99: 75 18              jne         0000000067141BB3
  0000000067141B9B: 48 8B 89 98 00 00  mov         rcx,qword ptr [rcx+0000000000000098h]
                    00
  0000000067141BA2: 45 33 C9           xor         r9d,r9d
  0000000067141BA5: 45 33 C0           xor         r8d,r8d
  0000000067141BA8: BA 01 04 00 00     mov         edx,401h
  0000000067141BAD: FF 15 A5 6C 06 00  call        qword ptr [00000000671A8858h]
  0000000067141BB3: 48 83 C4 28        add         rsp,28h
  0000000067141BB7: C3                 ret

  Summary

      1A7000 .text

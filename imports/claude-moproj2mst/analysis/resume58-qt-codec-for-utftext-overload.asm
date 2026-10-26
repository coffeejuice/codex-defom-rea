Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714E050: 4C 8B 09           mov         r9,qword ptr [rcx]
  000000006714E053: 45 8B 41 08        mov         r8d,dword ptr [r9+8]
  000000006714E057: 41 83 F8 03        cmp         r8d,3
  000000006714E05B: 7E 47              jle         000000006714E0A4
  000000006714E05D: 49 8B 41 10        mov         rax,qword ptr [r9+10h]
  000000006714E061: 0F B6 08           movzx       ecx,byte ptr [rax]
  000000006714E064: 84 C9              test        cl,cl
  000000006714E066: 75 1B              jne         000000006714E083
  000000006714E068: 38 48 01           cmp         byte ptr [rax+1],cl
  000000006714E06B: 75 16              jne         000000006714E083
  000000006714E06D: 80 78 02 FE        cmp         byte ptr [rax+2],0FEh
  000000006714E071: 75 10              jne         000000006714E083
  000000006714E073: 80 78 03 FF        cmp         byte ptr [rax+3],0FFh
  000000006714E077: 75 0A              jne         000000006714E083
  000000006714E079: B9 FA 03 00 00     mov         ecx,3FAh
  000000006714E07E: E9 4D F0 FF FF     jmp         000000006714D0D0
  000000006714E083: 80 F9 FF           cmp         cl,0FFh
  000000006714E086: 75 1C              jne         000000006714E0A4
  000000006714E088: 80 78 01 FE        cmp         byte ptr [rax+1],0FEh
  000000006714E08C: 75 16              jne         000000006714E0A4
  000000006714E08E: 80 78 02 00        cmp         byte ptr [rax+2],0
  000000006714E092: 75 10              jne         000000006714E0A4
  000000006714E094: 80 78 03 00        cmp         byte ptr [rax+3],0
  000000006714E098: 75 0A              jne         000000006714E0A4
  000000006714E09A: B9 FB 03 00 00     mov         ecx,3FBh
  000000006714E09F: E9 2C F0 FF FF     jmp         000000006714D0D0
  000000006714E0A4: 41 83 F8 02        cmp         r8d,2
  000000006714E0A8: 7C 52              jl          000000006714E0FC
  000000006714E0AA: 49 8B 41 10        mov         rax,qword ptr [r9+10h]
  000000006714E0AE: 0F B6 08           movzx       ecx,byte ptr [rax]
  000000006714E0B1: 80 F9 FE           cmp         cl,0FEh
  000000006714E0B4: 75 10              jne         000000006714E0C6
  000000006714E0B6: 80 78 01 FF        cmp         byte ptr [rax+1],0FFh
  000000006714E0BA: 75 0A              jne         000000006714E0C6
  000000006714E0BC: B9 F5 03 00 00     mov         ecx,3F5h
  000000006714E0C1: E9 0A F0 FF FF     jmp         000000006714D0D0
  000000006714E0C6: 80 F9 FF           cmp         cl,0FFh
  000000006714E0C9: 75 10              jne         000000006714E0DB
  000000006714E0CB: 80 78 01 FE        cmp         byte ptr [rax+1],0FEh
  000000006714E0CF: 75 0A              jne         000000006714E0DB
  000000006714E0D1: B9 F6 03 00 00     mov         ecx,3F6h
  000000006714E0D6: E9 F5 EF FF FF     jmp         000000006714D0D0
  000000006714E0DB: 41 83 F8 03        cmp         r8d,3
  000000006714E0DF: 7C 1B              jl          000000006714E0FC
  000000006714E0E1: 80 F9 EF           cmp         cl,0EFh
  000000006714E0E4: 75 16              jne         000000006714E0FC
  000000006714E0E6: 80 78 01 BB        cmp         byte ptr [rax+1],0BBh
  000000006714E0EA: 75 10              jne         000000006714E0FC
  000000006714E0EC: 80 78 02 BF        cmp         byte ptr [rax+2],0BFh
  000000006714E0F0: 75 0A              jne         000000006714E0FC
  000000006714E0F2: B9 6A 00 00 00     mov         ecx,6Ah
  000000006714E0F7: E9 D4 EF FF FF     jmp         000000006714D0D0
  000000006714E0FC: 48 8B C2           mov         rax,rdx
  000000006714E0FF: C3                 ret
  000000006714E100: 40

  Summary

      1A7000 .text

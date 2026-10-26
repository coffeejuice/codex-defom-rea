Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091C7A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091C7A5: 48 83 EC 48        sub         rsp,48h
  000000014091C7A9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091C7B2: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  000000014091C7B7: 48 83 C2 38        add         rdx,38h
  000000014091C7BB: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C7C0: E8 0B D3 93 FF     call        0000000140259AD0
  000000014091C7C5: 90                 nop
  000000014091C7C6: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091C7CE: EB 0B              jmp         000000014091C7DB
  000000014091C7D0: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091C7D4: 83 C0 01           add         eax,1
  000000014091C7D7: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091C7DB: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C7E0: E8 FB 82 6E FF     call        0000000140004AE0
  000000014091C7E5: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014091C7E9: 7D 1A              jge         000000014091C805
  000000014091C7EB: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091C7EF: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C7F4: E8 17 44 84 FF     call        0000000140160C10
  000000014091C7F9: 48 8B 08           mov         rcx,qword ptr [rax]
  000000014091C7FC: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091C800: 89 41 18           mov         dword ptr [rcx+18h],eax
  000000014091C803: EB CB              jmp         000000014091C7D0
  000000014091C805: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C80A: E8 91 B9 8E FF     call        00000001402081A0
  000000014091C80F: 48 83 C4 48        add         rsp,48h
  000000014091C813: C3                 ret

  Summary

     1E02000 .text

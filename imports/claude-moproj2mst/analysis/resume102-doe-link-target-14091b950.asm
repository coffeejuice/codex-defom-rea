Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091B950: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014091B954: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091B959: 48 83 EC 38        sub         rsp,38h
  000000014091B95D: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  000000014091B966: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  000000014091B96B: 7E 33              jle         000000014091B9A0
  000000014091B96D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014091B972: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091B975: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091B97A: FF 90 78 01 00 00  call        qword ptr [rax+0000000000000178h]
  000000014091B980: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  000000014091B984: 7D 1A              jge         000000014091B9A0
  000000014091B986: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091B98B: 48 83 C1 38        add         rcx,38h
  000000014091B98F: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  000000014091B993: E8 78 52 84 FF     call        0000000140160C10
  000000014091B998: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091B99B: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014091B9A0: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014091B9A5: 48 83 C4 38        add         rsp,38h
  000000014091B9A9: C3                 ret

  Summary

     1E02000 .text

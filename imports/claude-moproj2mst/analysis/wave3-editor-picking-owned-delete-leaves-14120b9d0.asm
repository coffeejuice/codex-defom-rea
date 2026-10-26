Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014120B9D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014120B9D5: 48 83 EC 48        sub         rsp,48h
  000000014120B9D9: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014120B9DE: 48 83 B8 20 44 00  cmp         qword ptr [rax+0000000000004420h],0
                    00 00
  000000014120B9E6: 74 52              je          000000014120BA3A
  000000014120B9E8: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014120B9ED: 48 8B 80 20 44 00  mov         rax,qword ptr [rax+0000000000004420h]
                    00
  000000014120B9F4: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014120B9F9: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014120B9FE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014120BA03: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014120BA09: 74 16              je          000000014120BA21
  000000014120BA0B: BA 01 00 00 00     mov         edx,1
  000000014120BA10: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014120BA15: E8 66 5F A8 FF     call        0000000140C91980
  000000014120BA1A: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014120BA1F: EB 09              jmp         000000014120BA2A
  000000014120BA21: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014120BA2A: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014120BA2F: 48 C7 80 20 44 00  mov         qword ptr [rax+0000000000004420h],0
                    00 00 00 00 00
  000000014120BA3A: 48 83 C4 48        add         rsp,48h
  000000014120BA3E: C3                 ret

  Summary

     1E02000 .text

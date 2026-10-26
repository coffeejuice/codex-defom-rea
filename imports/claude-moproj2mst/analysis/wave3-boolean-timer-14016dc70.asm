Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014016DC70: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014016DC75: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014016DC7A: 48 83 EC 48        sub         rsp,48h
  000000014016DC7E: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014016DC83: 48 8B 40 68        mov         rax,qword ptr [rax+68h]
  000000014016DC87: 48 83 B8 20 01 00  cmp         qword ptr [rax+0000000000000120h],0
                    00 00
  000000014016DC8F: 74 46              je          000000014016DCD7
  000000014016DC91: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014016DC96: 48 8B 40 68        mov         rax,qword ptr [rax+68h]
  000000014016DC9A: 48 8B 80 20 01 00  mov         rax,qword ptr [rax+0000000000000120h]
                    00
  000000014016DCA1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014016DCA6: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014016DCAB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014016DCB0: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014016DCB6: 74 16              je          000000014016DCCE
  000000014016DCB8: BA 01 00 00 00     mov         edx,1
  000000014016DCBD: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014016DCC2: E8 09 0C 00 00     call        000000014016E8D0
  000000014016DCC7: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014016DCCC: EB 09              jmp         000000014016DCD7
  000000014016DCCE: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014016DCD7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014016DCDC: 48 8B 49 68        mov         rcx,qword ptr [rcx+68h]
  000000014016DCE0: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014016DCE5: 48 89 81 20 01 00  mov         qword ptr [rcx+0000000000000120h],rax
                    00
  000000014016DCEC: 48 83 C4 48        add         rsp,48h
  000000014016DCF0: C3                 ret

  Summary

     1E02000 .text

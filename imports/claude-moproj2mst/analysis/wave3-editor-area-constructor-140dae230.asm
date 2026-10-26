Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DAE230: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  0000000140DAE235: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140DAE23A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DAE23F: 48 83 EC 38        sub         rsp,38h
  0000000140DAE243: 0F B6 44 24 50     movzx       eax,byte ptr [rsp+50h]
  0000000140DAE248: 85 C0              test        eax,eax
  0000000140DAE24A: 74 41              je          0000000140DAE28D
  0000000140DAE24C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DAE251: 48 83 C1 18        add         rcx,18h
  0000000140DAE255: E8 96 CC E6 FF     call        0000000140C1AEF0
  0000000140DAE25A: 48 85 C0           test        rax,rax
  0000000140DAE25D: 74 2E              je          0000000140DAE28D
  0000000140DAE25F: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140DAE264: 48 83 C2 18        add         rdx,18h
  0000000140DAE268: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DAE26D: 48 83 C1 20        add         rcx,20h
  0000000140DAE271: E8 8A D7 EA FF     call        0000000140C5BA00
  0000000140DAE276: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DAE27B: 48 83 C1 20        add         rcx,20h
  0000000140DAE27F: E8 6C CC E6 FF     call        0000000140C1AEF0
  0000000140DAE284: 48 8B C8           mov         rcx,rax
  0000000140DAE287: FF 15 23 D9 05 01  call        qword ptr [0000000141E0BBB0h]
  0000000140DAE28D: 0F B6 44 24 50     movzx       eax,byte ptr [rsp+50h]
  0000000140DAE292: 85 C0              test        eax,eax
  0000000140DAE294: 75 0A              jne         0000000140DAE2A0
  0000000140DAE296: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140DAE29E: EB 08              jmp         0000000140DAE2A8
  0000000140DAE2A0: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140DAE2A8: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140DAE2AD: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000140DAE2B2: 88 41 10           mov         byte ptr [rcx+10h],al
  0000000140DAE2B5: 48 83 C4 38        add         rsp,38h
  0000000140DAE2B9: C3                 ret

  Summary

     1E02000 .text

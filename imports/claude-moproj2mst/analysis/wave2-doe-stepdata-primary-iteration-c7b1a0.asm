Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C7B1A0: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140C7B1A4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C7B1A9: 48 83 EC 38        sub         rsp,38h
  0000000140C7B1AD: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C7B1B5: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C7B1BA: E8 11 FB FF FF     call        0000000140C7ACD0
  0000000140C7B1BF: 48 85 C0           test        rax,rax
  0000000140C7B1C2: 74 1B              je          0000000140C7B1DF
  0000000140C7B1C4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C7B1C9: E8 02 FB FF FF     call        0000000140C7ACD0
  0000000140C7B1CE: 0F B6 54 24 48     movzx       edx,byte ptr [rsp+48h]
  0000000140C7B1D3: 48 8B C8           mov         rcx,rax
  0000000140C7B1D6: E8 C5 8A BA FF     call        0000000140823CA0
  0000000140C7B1DB: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140C7B1DF: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140C7B1E3: 48 83 C4 38        add         rsp,38h
  0000000140C7B1E7: C3                 ret

  Summary

     1E02000 .text

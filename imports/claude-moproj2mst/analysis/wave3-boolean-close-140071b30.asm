Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140071B30: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140071B35: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140071B3A: 48 83 EC 38        sub         rsp,38h
  0000000140071B3E: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140071B44: 74 2A              je          0000000140071B70
  0000000140071B46: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  0000000140071B4B: 49 83 E8 10        sub         r8,10h
  0000000140071B4F: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140071B54: 4C 8D 0D 4D 7B 0C  lea         r9,[00000001421396A8h]
                    02
  0000000140071B5B: 48 8D 15 66 7B 0C  lea         rdx,[00000001421396C8h]
                    02
  0000000140071B62: 48 8D 0D 97 4B D0  lea         rcx,[0000000143D76700h]
                    03
  0000000140071B69: E8 72 E0 25 00     call        00000001402CFBE0
  0000000140071B6E: EB 23              jmp         0000000140071B93
  0000000140071B70: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  0000000140071B75: 49 83 E8 10        sub         r8,10h
  0000000140071B79: 4C 8D 0D 68 7B 0C  lea         r9,[00000001421396E8h]
                    02
  0000000140071B80: 48 8D 15 81 7B 0C  lea         rdx,[0000000142139708h]
                    02
  0000000140071B87: 48 8D 0D 72 4B D0  lea         rcx,[0000000143D76700h]
                    03
  0000000140071B8E: E8 3D E4 25 00     call        00000001402CFFD0
  0000000140071B93: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140071B98: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140071B9D: E8 4E A8 0D 00     call        000000014014C3F0
  0000000140071BA2: 48 83 C4 38        add         rsp,38h
  0000000140071BA6: C3                 ret

  Summary

     1E02000 .text

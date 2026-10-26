Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412EC7B0: 48 83 EC 28        sub         rsp,28h
  00000001412EC7B4: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  00000001412EC7B9: 45 33 C9           xor         r9d,r9d
  00000001412EC7BC: C6 44 24 40 00     mov         byte ptr [rsp+40h],0
  00000001412EC7C1: E8 FA AE FF FF     call        00000001412E76C0
  00000001412EC7C6: 44 38 4C 24 40     cmp         byte ptr [rsp+40h],r9b
  00000001412EC7CB: 75 07              jne         00000001412EC7D4
  00000001412EC7CD: 32 C0              xor         al,al
  00000001412EC7CF: 48 83 C4 28        add         rsp,28h
  00000001412EC7D3: C3                 ret
  00000001412EC7D4: 8A 00              mov         al,byte ptr [rax]
  00000001412EC7D6: 48 83 C4 28        add         rsp,28h
  00000001412EC7DA: C3                 ret

  Summary

     1E02000 .text

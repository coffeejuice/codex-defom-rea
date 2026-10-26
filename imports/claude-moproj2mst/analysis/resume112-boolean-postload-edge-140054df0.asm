Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140054DF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140054DF5: 48 83 EC 48        sub         rsp,48h
  0000000140054DF9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140054DFE: E8 2D C4 FF FF     call        0000000140051230
  0000000140054E03: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140054E08: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140054E0E: 74 10              je          0000000140054E20
  0000000140054E10: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140054E15: 48 83 C0 10        add         rax,10h
  0000000140054E19: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140054E1E: EB 09              jmp         0000000140054E29
  0000000140054E20: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140054E29: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140054E2E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140054E33: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140054E38: 48 8D 0D C1 18 D2  lea         rcx,[0000000143D76700h]
                    03
  0000000140054E3F: E8 7C 9B 27 00     call        00000001402CE9C0
  0000000140054E44: 48 83 C4 48        add         rsp,48h
  0000000140054E48: C3                 ret

  Summary

     1E02000 .text

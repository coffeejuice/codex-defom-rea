Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141202A30: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141202A35: 48 83 EC 38        sub         rsp,38h
  0000000141202A39: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141202A3E: 48 83 C1 08        add         rcx,8
  0000000141202A42: E8 09 60 ED FE     call        00000001400D8A50
  0000000141202A47: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  0000000141202A4C: 74 19              je          0000000141202A67
  0000000141202A4E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000141202A53: 48 83 C1 08        add         rcx,8
  0000000141202A57: E8 F4 5F ED FE     call        00000001400D8A50
  0000000141202A5C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000141202A60: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000141202A65: EB 0C              jmp         0000000141202A73
  0000000141202A67: 48 8D 05 B2 29 4E  lea         rax,[00000001446E5420h]
                    03
  0000000141202A6E: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000141202A73: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000141202A78: 48 83 C4 38        add         rsp,38h
  0000000141202A7C: C3                 ret

  Summary

     1E02000 .text

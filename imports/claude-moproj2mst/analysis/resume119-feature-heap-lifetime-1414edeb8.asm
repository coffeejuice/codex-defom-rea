Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414EDEB8: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001414EDEBD: 57                 push        rdi
  00000001414EDEBE: 48 83 EC 20        sub         rsp,20h
  00000001414EDEC2: 80 79 10 00        cmp         byte ptr [rcx+10h],0
  00000001414EDEC6: 48 8D 05 53 CA A2  lea         rax,[0000000142F1A920h]
                    01
  00000001414EDECD: 8B FA              mov         edi,edx
  00000001414EDECF: 48 89 01           mov         qword ptr [rcx],rax
  00000001414EDED2: 48 8B D9           mov         rbx,rcx
  00000001414EDED5: 74 0F              je          00000001414EDEE6
  00000001414EDED7: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  00000001414EDEDB: 48 85 C9           test        rcx,rcx
  00000001414EDEDE: 74 06              je          00000001414EDEE6
  00000001414EDEE0: FF 15 B2 55 91 00  call        qword ptr [0000000141E03498h]
  00000001414EDEE6: 40 F6 C7 01        test        dil,1
  00000001414EDEEA: 74 08              je          00000001414EDEF4
  00000001414EDEEC: 48 8B CB           mov         rcx,rbx
  00000001414EDEEF: E8 74 1D FF FF     call        00000001414DFC68
  00000001414EDEF4: 48 8B C3           mov         rax,rbx
  00000001414EDEF7: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000001414EDEFC: 48 83 C4 20        add         rsp,20h
  00000001414EDF00: 5F                 pop         rdi
  00000001414EDF01: C3                 ret

  Summary

     1E02000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141481CE8: 40 53              push        rbx
  0000000141481CEA: 48 83 EC 20        sub         rsp,20h
  0000000141481CEE: 48 8B D9           mov         rbx,rcx
  0000000141481CF1: 48 8B 0D E8 E9 27  mov         rcx,qword ptr [00000001447006E0h]
                    03
  0000000141481CF8: E8 EF F6 FF FF     call        00000001414813EC
  0000000141481CFD: 48 85 C0           test        rax,rax
  0000000141481D00: 74 10              je          0000000141481D12
  0000000141481D02: 48 8B CB           mov         rcx,rbx
  0000000141481D05: FF D0              call        rax
  0000000141481D07: 85 C0              test        eax,eax
  0000000141481D09: 74 07              je          0000000141481D12
  0000000141481D0B: B8 01 00 00 00     mov         eax,1
  0000000141481D10: EB 02              jmp         0000000141481D14
  0000000141481D12: 33 C0              xor         eax,eax
  0000000141481D14: 48 83 C4 20        add         rsp,20h
  0000000141481D18: 5B                 pop         rbx
  0000000141481D19: C3                 ret

  Summary

     1E02000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141481708: 40 53              push        rbx
  000000014148170A: 48 83 EC 20        sub         rsp,20h
  000000014148170E: E8 E1 D2 FE FF     call        000000014146E9F4
  0000000141481713: E8 B4 E2 FF FF     call        000000014147F9CC
  0000000141481718: 85 C0              test        eax,eax
  000000014148171A: 74 60              je          000000014148177C
  000000014148171C: 48 8D 0D 6D FE FF  lea         rcx,[0000000141481590h]
                    FF
  0000000141481723: FF 15 27 1D 98 00  call        qword ptr [0000000141E03450h]
  0000000141481729: 89 05 61 D2 6D 02  mov         dword ptr [0000000143B5E990h],eax
  000000014148172F: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141481732: 74 48              je          000000014148177C
  0000000141481734: BA C8 02 00 00     mov         edx,2C8h
  0000000141481739: B9 01 00 00 00     mov         ecx,1
  000000014148173E: E8 FD CC FF FF     call        000000014147E440
  0000000141481743: 48 8B D8           mov         rbx,rax
  0000000141481746: 48 85 C0           test        rax,rax
  0000000141481749: 74 31              je          000000014148177C
  000000014148174B: 8B 0D 3F D2 6D 02  mov         ecx,dword ptr [0000000143B5E990h]
  0000000141481751: 48 8B D0           mov         rdx,rax
  0000000141481754: FF 15 DE 1C 98 00  call        qword ptr [0000000141E03438h]
  000000014148175A: 85 C0              test        eax,eax
  000000014148175C: 74 1E              je          000000014148177C
  000000014148175E: 33 D2              xor         edx,edx
  0000000141481760: 48 8B CB           mov         rcx,rbx
  0000000141481763: E8 CC FC FF FF     call        0000000141481434
  0000000141481768: FF 15 7A 1C 98 00  call        qword ptr [0000000141E033E8h]
  000000014148176E: 48 83 4B 08 FF     or          qword ptr [rbx+8],0FFFFFFFFFFFFFFFFh
  0000000141481773: 89 03              mov         dword ptr [rbx],eax
  0000000141481775: B8 01 00 00 00     mov         eax,1
  000000014148177A: EB 07              jmp         0000000141481783
  000000014148177C: E8 8B FC FF FF     call        000000014148140C
  0000000141481781: 33 C0              xor         eax,eax
  0000000141481783: 48 83 C4 20        add         rsp,20h
  0000000141481787: 5B                 pop         rbx
  0000000141481788: C3                 ret

  Summary

     1E02000 .text

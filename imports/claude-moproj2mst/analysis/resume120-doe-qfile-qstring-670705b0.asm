Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670705B0: 66 89 54 24 10     mov         word ptr [rsp+10h],dx
  00000000670705B5: 53                 push        rbx
  00000000670705B6: 48 83 EC 20        sub         rsp,20h
  00000000670705BA: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670705BD: 48 8B D9           mov         rbx,rcx
  00000000670705C0: 8B 10              mov         edx,dword ptr [rax]
  00000000670705C2: 83 FA 01           cmp         edx,1
  00000000670705C5: 75 0D              jne         00000000670705D4
  00000000670705C7: 48 8B 11           mov         rdx,qword ptr [rcx]
  00000000670705CA: 8B 42 08           mov         eax,dword ptr [rdx+8]
  00000000670705CD: FF C0              inc         eax
  00000000670705CF: 3B 42 04           cmp         eax,dword ptr [rdx+4]
  00000000670705D2: 7E 22              jle         00000000670705F6
  00000000670705D4: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670705D7: BA 20 00 00 00     mov         edx,20h
  00000000670705DC: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000670705DF: FF C1              inc         ecx
  00000000670705E1: 03 C9              add         ecx,ecx
  00000000670705E3: E8 B8 C4 FB FF     call        000000006702CAA0
  00000000670705E8: 48 8B CB           mov         rcx,rbx
  00000000670705EB: 48 63 D0           movsxd      rdx,eax
  00000000670705EE: 48 D1 EA           shr         rdx,1
  00000000670705F1: E8 AA FB FF FF     call        00000000670701A0
  00000000670705F6: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670705F9: 48 63 50 08        movsxd      rdx,dword ptr [rax+8]
  00000000670705FD: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  0000000067070601: 0F B7 44 24 38     movzx       eax,word ptr [rsp+38h]
  0000000067070606: 66 89 04 51        mov         word ptr [rcx+rdx*2],ax
  000000006707060A: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006707060D: FF 40 08           inc         dword ptr [rax+8]
  0000000067070610: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000067070613: 48 63 50 08        movsxd      rdx,dword ptr [rax+8]
  0000000067070617: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  000000006707061B: 33 C0              xor         eax,eax
  000000006707061D: 66 89 04 51        mov         word ptr [rcx+rdx*2],ax
  0000000067070621: 48 8B C3           mov         rax,rbx
  0000000067070624: 48 83 C4 20        add         rsp,20h
  0000000067070628: 5B                 pop         rbx
  0000000067070629: C3                 ret

  Summary

      1A7000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140817940: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140817945: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000014081794A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014081794E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140817953: 48 83 EC 38        sub         rsp,38h
  0000000140817957: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014081795C: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  0000000140817961: 7E 5C              jle         00000001408179BF
  0000000140817963: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140817968: 48 83 C1 28        add         rcx,28h
  000000014081796C: E8 6F D1 7E FF     call        0000000140004AE0
  0000000140817971: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  0000000140817975: 7D 48              jge         00000001408179BF
  0000000140817977: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014081797C: 48 83 C1 28        add         rcx,28h
  0000000140817980: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  0000000140817984: E8 87 74 AD FF     call        00000001402EEE10
  0000000140817989: 48 8B 10           mov         rdx,qword ptr [rax]
  000000014081798C: 48 83 C2 08        add         rdx,8
  0000000140817990: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140817995: FF 15 D5 CA 5E 01  call        qword ptr [0000000141E04470h]
  000000014081799B: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408179A0: 48 83 C1 28        add         rcx,28h
  00000001408179A4: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  00000001408179A8: E8 63 74 AD FF     call        00000001402EEE10
  00000001408179AD: 48 8B 10           mov         rdx,qword ptr [rax]
  00000001408179B0: 48 83 C2 10        add         rdx,10h
  00000001408179B4: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000001408179B9: FF 15 B1 CA 5E 01  call        qword ptr [0000000141E04470h]
  00000001408179BF: 8A 44 24 20        mov         al,byte ptr [rsp+20h]
  00000001408179C3: 48 83 C4 38        add         rsp,38h
  00000001408179C7: C3                 ret

  Summary

     1E02000 .text

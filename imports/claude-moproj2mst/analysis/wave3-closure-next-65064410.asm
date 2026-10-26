Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065064410: 40 53              push        rbx
  0000000065064412: 48 83 EC 20        sub         rsp,20h
  0000000065064416: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006506441A: 48 8B D9           mov         rbx,rcx
  000000006506441D: F6 40 08 01        test        byte ptr [rax+8],1
  0000000065064421: 74 16              je          0000000065064439
  0000000065064423: 83 60 30 F1        and         dword ptr [rax+30h],0FFFFFFF1h
  0000000065064427: 83 48 30 01        or          dword ptr [rax+30h],1
  000000006506442B: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006506442F: 48 83 C0 28        add         rax,28h
  0000000065064433: 48 83 C4 20        add         rsp,20h
  0000000065064437: 5B                 pop         rbx
  0000000065064438: C3                 ret
  0000000065064439: 8B 40 08           mov         eax,dword ptr [rax+8]
  000000006506443C: C1 E8 0F           shr         eax,0Fh
  000000006506443F: A8 01              test        al,1
  0000000065064441: 74 09              je          000000006506444C
  0000000065064443: E8 88 D8 FF FF     call        0000000065061CD0
  0000000065064448: 84 C0              test        al,al
  000000006506444A: 74 22              je          000000006506446E
  000000006506444C: 48 8B CB           mov         rcx,rbx
  000000006506444F: E8 EC DB FB FF     call        0000000065022040
  0000000065064454: 84 C0              test        al,al
  0000000065064456: 75 16              jne         000000006506446E
  0000000065064458: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006506445C: 83 60 30 F0        and         dword ptr [rax+30h],0FFFFFFF0h
  0000000065064460: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  0000000065064464: 48 83 C0 28        add         rax,28h
  0000000065064468: 48 83 C4 20        add         rsp,20h
  000000006506446C: 5B                 pop         rbx
  000000006506446D: C3                 ret
  000000006506446E: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  0000000065064472: 83 60 30 F2        and         dword ptr [rax+30h],0FFFFFFF2h
  0000000065064476: 83 48 30 02        or          dword ptr [rax+30h],2
  000000006506447A: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006506447E: 48 83 C0 28        add         rax,28h
  0000000065064482: 48 83 C4 20        add         rsp,20h
  0000000065064486: 5B                 pop         rbx
  0000000065064487: C3                 ret

  Summary

      695000 .text

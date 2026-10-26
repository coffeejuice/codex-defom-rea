Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650A1430: 40 53              push        rbx
  00000000650A1432: 48 83 EC 20        sub         rsp,20h
  00000000650A1436: 48 8B D9           mov         rbx,rcx
  00000000650A1439: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000650A143C: 48 85 C9           test        rcx,rcx
  00000000650A143F: 74 73              je          00000000650A14B4
  00000000650A1441: 48 83 C1 08        add         rcx,8
  00000000650A1445: FF 15 2D 55 5F 00  call        qword ptr [0000000065696978h]
  00000000650A144B: 84 C0              test        al,al
  00000000650A144D: 75 65              jne         00000000650A14B4
  00000000650A144F: 48 8B 1B           mov         rbx,qword ptr [rbx]
  00000000650A1452: 48 85 DB           test        rbx,rbx
  00000000650A1455: 74 5D              je          00000000650A14B4
  00000000650A1457: 8B 43 14           mov         eax,dword ptr [rbx+14h]
  00000000650A145A: 83 F8 01           cmp         eax,1
  00000000650A145D: 75 36              jne         00000000650A1495
  00000000650A145F: 48 8B 4B 18        mov         rcx,qword ptr [rbx+18h]
  00000000650A1463: FF 15 0F 55 5F 00  call        qword ptr [0000000065696978h]
  00000000650A1469: 84 C0              test        al,al
  00000000650A146B: 75 3F              jne         00000000650A14AC
  00000000650A146D: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000650A1470: 48 85 C9           test        rcx,rcx
  00000000650A1473: 74 0A              je          00000000650A147F
  00000000650A1475: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000650A1478: BA 01 00 00 00     mov         edx,1
  00000000650A147D: FF 10              call        qword ptr [rax]
  00000000650A147F: 48 8B 4B 18        mov         rcx,qword ptr [rbx+18h]
  00000000650A1483: E8 26 FC 53 00     call        00000000655E10AE
  00000000650A1488: 48 8B CB           mov         rcx,rbx
  00000000650A148B: 48 83 C4 20        add         rsp,20h
  00000000650A148F: 5B                 pop         rbx
  00000000650A1490: E9 19 FC 53 00     jmp         00000000655E10AE
  00000000650A1495: 83 F8 02           cmp         eax,2
  00000000650A1498: 75 12              jne         00000000650A14AC
  00000000650A149A: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000650A149D: 48 85 C9           test        rcx,rcx
  00000000650A14A0: 74 0A              je          00000000650A14AC
  00000000650A14A2: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000650A14A5: BA 01 00 00 00     mov         edx,1
  00000000650A14AA: FF 10              call        qword ptr [rax]
  00000000650A14AC: 48 8B CB           mov         rcx,rbx
  00000000650A14AF: E8 FA FB 53 00     call        00000000655E10AE
  00000000650A14B4: 48 83 C4 20        add         rsp,20h
  00000000650A14B8: 5B                 pop         rbx
  00000000650A14B9: C3                 ret

  Summary

      695000 .text

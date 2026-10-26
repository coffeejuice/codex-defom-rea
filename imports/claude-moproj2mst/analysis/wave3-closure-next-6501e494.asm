Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006501E494: 48 89 74 24 40     mov         qword ptr [rsp+40h],rsi
  000000006501E499: 48 89 7C 24 48     mov         qword ptr [rsp+48h],rdi
  000000006501E49E: FF 15 A4 84 67 00  call        qword ptr [0000000065696948h]
  000000006501E4A4: 49 8B CC           mov         rcx,r12
  000000006501E4A7: 48 8B D8           mov         rbx,rax
  000000006501E4AA: FF 15 A0 84 67 00  call        qword ptr [0000000065696950h]
  000000006501E4B0: 48 3B D8           cmp         rbx,rax
  000000006501E4B3: 0F 84 D9 00 00 00  je          000000006501E592
  000000006501E4B9: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006501E4C0: 48 8B 33           mov         rsi,qword ptr [rbx]
  000000006501E4C3: 4C 39 2E           cmp         qword ptr [rsi],r13
  000000006501E4C6: 0F 85 B0 00 00 00  jne         000000006501E57C
  000000006501E4CC: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006501E4D0: 48 85 C9           test        rcx,rcx
  000000006501E4D3: 0F 84 A3 00 00 00  je          000000006501E57C
  000000006501E4D9: FF 15 89 85 67 00  call        qword ptr [0000000065696A68h]
  000000006501E4DF: 48 8B CD           mov         rcx,rbp
  000000006501E4E2: 8B F8              mov         edi,eax
  000000006501E4E4: FF 15 7E 85 67 00  call        qword ptr [0000000065696A68h]
  000000006501E4EA: 3B F8              cmp         edi,eax
  000000006501E4EC: 0F 85 8A 00 00 00  jne         000000006501E57C
  000000006501E4F2: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006501E4F6: FF 15 6C 85 67 00  call        qword ptr [0000000065696A68h]
  000000006501E4FC: 83 F8 4C           cmp         eax,4Ch
  000000006501E4FF: 0F 84 D0 00 00 00  je          000000006501E5D5
  000000006501E505: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006501E509: FF 15 59 85 67 00  call        qword ptr [0000000065696A68h]
  000000006501E50F: 83 F8 48           cmp         eax,48h
  000000006501E512: 0F 84 BD 00 00 00  je          000000006501E5D5
  000000006501E518: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006501E51C: FF 15 46 85 67 00  call        qword ptr [0000000065696A68h]
  000000006501E522: 83 F8 4D           cmp         eax,4Dh
  000000006501E525: 0F 84 AA 00 00 00  je          000000006501E5D5
  000000006501E52B: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006501E52F: FF 15 33 85 67 00  call        qword ptr [0000000065696A68h]
  000000006501E535: 83 F8 0E           cmp         eax,0Eh
  000000006501E538: 0F 84 8B 00 00 00  je          000000006501E5C9
  000000006501E53E: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006501E542: FF 15 20 85 67 00  call        qword ptr [0000000065696A68h]
  000000006501E548: 83 F8 0D           cmp         eax,0Dh
  000000006501E54B: 74 6E              je          000000006501E5BB
  000000006501E54D: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006501E551: FF 15 11 85 67 00  call        qword ptr [0000000065696A68h]
  000000006501E557: 83 F8 59           cmp         eax,59h
  000000006501E55A: 74 79              je          000000006501E5D5
  000000006501E55C: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006501E560: FF 15 02 85 67 00  call        qword ptr [0000000065696A68h]
  000000006501E566: 3D C9 00 00 00     cmp         eax,0C9h
  000000006501E56B: 74 68              je          000000006501E5D5
  000000006501E56D: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006501E571: FF 15 F1 84 67 00  call        qword ptr [0000000065696A68h]
  000000006501E577: 83 F8 53           cmp         eax,53h
  000000006501E57A: 74 31              je          000000006501E5AD
  000000006501E57C: 49 8B CC           mov         rcx,r12
  000000006501E57F: 48 83 C3 08        add         rbx,8
  000000006501E583: FF 15 C7 83 67 00  call        qword ptr [0000000065696950h]
  000000006501E589: 48 3B D8           cmp         rbx,rax
  000000006501E58C: 0F 85 2E FF FF FF  jne         000000006501E4C0
  000000006501E592: 32 C0              xor         al,al
  000000006501E594: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  000000006501E599: 48 8B 7C 24 48     mov         rdi,qword ptr [rsp+48h]
  000000006501E59E: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  000000006501E5A3: 48 83 C4 20        add         rsp,20h
  000000006501E5A7: 41 5D              pop         r13
  000000006501E5A9: 41 5C              pop         r12
  000000006501E5AB: 5D                 pop         rbp
  000000006501E5AC: C3                 ret

  Summary

      695000 .text

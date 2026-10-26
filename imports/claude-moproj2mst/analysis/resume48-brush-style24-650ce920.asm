Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650CE920: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000650CE925: 53                 push        rbx
  00000000650CE926: 48 83 EC 30        sub         rsp,30h
  00000000650CE92A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650CE933: 48 8B D9           mov         rbx,rcx
  00000000650CE936: 48 8D 05 F3 6D 5E  lea         rax,[00000000656B5730h]
                    00
  00000000650CE93D: 48 89 01           mov         qword ptr [rcx],rax
  00000000650CE940: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  00000000650CE944: 48 85 C9           test        rcx,rcx
  00000000650CE947: 74 22              je          00000000650CE96B
  00000000650CE949: 48 83 C1 18        add         rcx,18h
  00000000650CE94D: FF 15 25 80 5C 00  call        qword ptr [0000000065696978h]
  00000000650CE953: 84 C0              test        al,al
  00000000650CE955: 75 14              jne         00000000650CE96B
  00000000650CE957: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  00000000650CE95B: 48 85 C9           test        rcx,rcx
  00000000650CE95E: 74 0B              je          00000000650CE96B
  00000000650CE960: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000650CE963: BA 01 00 00 00     mov         edx,1
  00000000650CE968: FF 10              call        qword ptr [rax]
  00000000650CE96A: 90                 nop
  00000000650CE96B: 48 8B CB           mov         rcx,rbx
  00000000650CE96E: 48 83 C4 30        add         rsp,30h
  00000000650CE972: 5B                 pop         rbx
  00000000650CE973: E9 18 98 06 00     jmp         0000000065138190

  Summary

      695000 .text

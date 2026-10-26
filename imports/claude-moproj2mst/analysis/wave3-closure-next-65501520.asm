Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065501520: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000065501525: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  000000006550152A: 57                 push        rdi
  000000006550152B: 48 83 EC 20        sub         rsp,20h
  000000006550152F: 48 8B D9           mov         rbx,rcx
  0000000065501532: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000065501535: 8B F2              mov         esi,edx
  0000000065501537: BA 01 00 00 00     mov         edx,1
  000000006550153C: 41 8B F8           mov         edi,r8d
  000000006550153F: FF 15 2B 55 19 00  call        qword ptr [0000000065696A70h]
  0000000065501545: 84 C0              test        al,al
  0000000065501547: 74 0E              je          0000000065501557
  0000000065501549: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006550154C: 48 8B CB           mov         rcx,rbx
  000000006550154F: 8B 50 04           mov         edx,dword ptr [rax+4]
  0000000065501552: E8 19 DF 00 00     call        000000006550F470
  0000000065501557: 44 8B C7           mov         r8d,edi
  000000006550155A: 8B D6              mov         edx,esi
  000000006550155C: 48 8B CB           mov         rcx,rbx
  000000006550155F: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065501564: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000065501569: 48 83 C4 20        add         rsp,20h
  000000006550156D: 5F                 pop         rdi
  000000006550156E: 48 FF 25 AB 5A 19  jmp         qword ptr [0000000065697020h]
                    00

  Summary

      695000 .text

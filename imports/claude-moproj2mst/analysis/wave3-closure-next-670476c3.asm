Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670476C3: 48 89 6C 24 48     mov         qword ptr [rsp+48h],rbp
  00000000670476C8: 8B 69 20           mov         ebp,dword ptr [rcx+20h]
  00000000670476CB: 4C 89 64 24 20     mov         qword ptr [rsp+20h],r12
  00000000670476D0: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  00000000670476D4: 85 ED              test        ebp,ebp
  00000000670476D6: 74 5F              je          0000000067047737
  00000000670476D8: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000670476DD: 48 89 7C 24 50     mov         qword ptr [rsp+50h],rdi
  00000000670476E2: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    0F 1F 84 00 00 00
                    00 00
  00000000670476F0: 49 8B 1C 24        mov         rbx,qword ptr [r12]
  00000000670476F4: FF CD              dec         ebp
  00000000670476F6: 49 83 C4 08        add         r12,8
  00000000670476FA: 48 3B DE           cmp         rbx,rsi
  00000000670476FD: 74 2A              je          0000000067047729
  00000000670476FF: 90                 nop
  0000000067047700: 48 8B 3B           mov         rdi,qword ptr [rbx]
  0000000067047703: 48 8B CB           mov         rcx,rbx
  0000000067047706: 41 FF D5           call        r13
  0000000067047709: F6 46 24 02        test        byte ptr [rsi+24h],2
  000000006704770D: 74 0A              je          0000000067047719
  000000006704770F: 48 8B CB           mov         rcx,rbx
  0000000067047712: E8 69 8D FD FF     call        0000000067020480
  0000000067047717: EB 08              jmp         0000000067047721
  0000000067047719: 48 8B CB           mov         rcx,rbx
  000000006704771C: E8 8F 8C FD FF     call        00000000670203B0
  0000000067047721: 48 8B DF           mov         rbx,rdi
  0000000067047724: 48 3B FE           cmp         rdi,rsi
  0000000067047727: 75 D7              jne         0000000067047700
  0000000067047729: 85 ED              test        ebp,ebp
  000000006704772B: 75 C3              jne         00000000670476F0
  000000006704772D: 48 8B 7C 24 50     mov         rdi,qword ptr [rsp+50h]
  0000000067047732: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  0000000067047737: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  000000006704773C: 4C 8B 64 24 20     mov         r12,qword ptr [rsp+20h]
  0000000067047741: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  0000000067047745: E8 12 12 13 00     call        000000006717895C
  000000006704774A: 48 8B CE           mov         rcx,rsi
  000000006704774D: 48 83 C4 28        add         rsp,28h
  0000000067047751: 41 5D              pop         r13
  0000000067047753: 5E                 pop         rsi
  0000000067047754: E9 FD 11 13 00     jmp         0000000067178956

  Summary

      1A7000 .text

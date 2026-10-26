Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650668F0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000650668F5: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  00000000650668FA: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  00000000650668FF: 57                 push        rdi
  0000000065066900: 48 83 EC 40        sub         rsp,40h
  0000000065066904: 48 8B 69 08        mov         rbp,qword ptr [rcx+8]
  0000000065066908: 48 8B F9           mov         rdi,rcx
  000000006506690B: 48 81 C1 70 01 00  add         rcx,170h
                    00
  0000000065066912: 48 8B DA           mov         rbx,rdx
  0000000065066915: E8 26 34 FE FF     call        0000000065049D40
  000000006506691A: 84 C0              test        al,al
  000000006506691C: 74 10              je          000000006506692E
  000000006506691E: 8B 87 78 01 00 00  mov         eax,dword ptr [rdi+0000000000000178h]
  0000000065066924: 33 43 08           xor         eax,dword ptr [rbx+8]
  0000000065066927: A9 F0 FF FF FF     test        eax,0FFFFFFF0h
  000000006506692C: 74 71              je          000000006506699F
  000000006506692E: 48 8D 8F 70 01 00  lea         rcx,[rdi+0000000000000170h]
                    00
  0000000065066935: 48 8B D3           mov         rdx,rbx
  0000000065066938: E8 23 32 FE FF     call        0000000065049B60
  000000006506693D: 48 8B CF           mov         rcx,rdi
  0000000065066940: E8 6B 89 03 00     call        000000006509F2B0
  0000000065066945: 48 8B CF           mov         rcx,rdi
  0000000065066948: E8 C3 F3 FF FF     call        0000000065065D10
  000000006506694D: 48 8B CF           mov         rcx,rdi
  0000000065066950: E8 8B F5 FF FF     call        0000000065065EE0
  0000000065066955: 48 8B 4D 20        mov         rcx,qword ptr [rbp+20h]
  0000000065066959: 48 83 C1 14        add         rcx,14h
  000000006506695D: FF 15 C5 FD 62 00  call        qword ptr [0000000065696728h]
  0000000065066963: 48 8B 4D 20        mov         rcx,qword ptr [rbp+20h]
  0000000065066967: 48 83 C1 14        add         rcx,14h
  000000006506696B: 8B D8              mov         ebx,eax
  000000006506696D: FF 15 ED FE 62 00  call        qword ptr [0000000065696860h]
  0000000065066973: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000065066978: 45 33 C0           xor         r8d,r8d
  000000006506697B: 33 D2              xor         edx,edx
  000000006506697D: 44 8B C8           mov         r9d,eax
  0000000065066980: 89 5C 24 20        mov         dword ptr [rsp+20h],ebx
  0000000065066984: FF 15 E6 FD 62 00  call        qword ptr [0000000065696770h]
  000000006506698A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006506698F: 48 8B CD           mov         rcx,rbp
  0000000065066992: E8 09 BC FF FF     call        00000000650625A0
  0000000065066997: 48 8B CF           mov         rcx,rdi
  000000006506699A: E8 41 F5 FF FF     call        0000000065065EE0
  000000006506699F: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000650669A4: 48 8B 6C 24 58     mov         rbp,qword ptr [rsp+58h]
  00000000650669A9: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  00000000650669AE: 48 83 C4 40        add         rsp,40h
  00000000650669B2: 5F                 pop         rdi
  00000000650669B3: C3                 ret

  Summary

      695000 .text

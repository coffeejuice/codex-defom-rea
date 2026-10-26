Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067119570: 40 56              push        rsi
  0000000067119572: 57                 push        rdi
  0000000067119573: 41 54              push        r12
  0000000067119575: 48 83 EC 40        sub         rsp,40h
  0000000067119579: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067119582: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  0000000067119587: 4D 8B E0           mov         r12,r8
  000000006711958A: 48 8B FA           mov         rdi,rdx
  000000006711958D: 48 8B F1           mov         rsi,rcx
  0000000067119590: C6 44 24 68 00     mov         byte ptr [rsp+68h],0
  0000000067119595: 48 89 54 24 28     mov         qword ptr [rsp+28h],rdx
  000000006711959A: 4C 89 44 24 30     mov         qword ptr [rsp+30h],r8
  000000006711959F: 48 8D 44 24 68     lea         rax,[rsp+68h]
  00000000671195A4: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000000671195A9: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000671195AE: B9 03 00 00 00     mov         ecx,3
  00000000671195B3: E8 C8 46 F0 FF     call        000000006701DC80
  00000000671195B8: 84 C0              test        al,al
  00000000671195BA: 74 06              je          00000000671195C2
  00000000671195BC: 8A 44 24 68        mov         al,byte ptr [rsp+68h]
  00000000671195C0: EB 23              jmp         00000000671195E5
  00000000671195C2: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  00000000671195C6: 48 8B 58 40        mov         rbx,qword ptr [rax+40h]
  00000000671195CA: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  00000000671195CF: FF 43 1C           inc         dword ptr [rbx+1Ch]
  00000000671195D2: 48 8B 06           mov         rax,qword ptr [rsi]
  00000000671195D5: 4D 8B C4           mov         r8,r12
  00000000671195D8: 48 8B D7           mov         rdx,rdi
  00000000671195DB: 48 8B CE           mov         rcx,rsi
  00000000671195DE: FF 50 58           call        qword ptr [rax+58h]
  00000000671195E1: 90                 nop
  00000000671195E2: FF 4B 1C           dec         dword ptr [rbx+1Ch]
  00000000671195E5: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  00000000671195EA: 48 83 C4 40        add         rsp,40h
  00000000671195EE: 41 5C              pop         r12
  00000000671195F0: 5F                 pop         rdi
  00000000671195F1: 5E                 pop         rsi
  00000000671195F2: C3                 ret

  Summary

      1A7000 .text

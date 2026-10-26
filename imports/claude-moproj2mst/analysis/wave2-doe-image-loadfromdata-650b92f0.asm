Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650B92F0: 40 53              push        rbx
  00000000650B92F2: 48 83 EC 40        sub         rsp,40h
  00000000650B92F6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650B92FF: 48 8B D9           mov         rbx,rcx
  00000000650B9302: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650B9307: E8 04 C1 FF FF     call        00000000650B5410
  00000000650B930C: 90                 nop
  00000000650B930D: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000000650B9313: 74 57              je          00000000650B936C
  00000000650B9315: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000650B931A: 48 8B CB           mov         rcx,rbx
  00000000650B931D: E8 FE FB FF FF     call        00000000650B8F20
  00000000650B9322: 90                 nop
  00000000650B9323: 48 8D 05 2E 94 5F  lea         rax,[00000000656B2758h]
                    00
  00000000650B932A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000000650B932F: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000000650B9334: 48 85 C9           test        rcx,rcx
  00000000650B9337: 74 25              je          00000000650B935E
  00000000650B9339: FF 15 39 D6 5D 00  call        qword ptr [0000000065696978h]
  00000000650B933F: 84 C0              test        al,al
  00000000650B9341: 75 1B              jne         00000000650B935E
  00000000650B9343: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000650B9348: 48 85 DB           test        rbx,rbx
  00000000650B934B: 74 11              je          00000000650B935E
  00000000650B934D: 48 8B CB           mov         rcx,rbx
  00000000650B9350: E8 EB 94 FF FF     call        00000000650B2840
  00000000650B9355: 48 8B CB           mov         rcx,rbx
  00000000650B9358: E8 51 7D 52 00     call        00000000655E10AE
  00000000650B935D: 90                 nop
  00000000650B935E: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650B9363: E8 28 EE 07 00     call        0000000065138190
  00000000650B9368: B0 01              mov         al,1
  00000000650B936A: EB 18              jmp         00000000650B9384
  00000000650B936C: 48 8D 05 E5 93 5F  lea         rax,[00000000656B2758h]
                    00
  00000000650B9373: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000000650B9378: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650B937D: E8 0E EE 07 00     call        0000000065138190
  00000000650B9382: 32 C0              xor         al,al
  00000000650B9384: 48 83 C4 40        add         rsp,40h
  00000000650B9388: 5B                 pop         rbx
  00000000650B9389: C3                 ret

  Summary

      695000 .text

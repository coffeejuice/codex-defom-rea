Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B7880: 40 57              push        rdi
  00000000670B7882: 48 83 EC 40        sub         rsp,40h
  00000000670B7886: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B788F: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000670B7894: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  00000000670B7899: 48 8B DA           mov         rbx,rdx
  00000000670B789C: 48 8B F9           mov         rdi,rcx
  00000000670B789F: B9 30 01 00 00     mov         ecx,130h
  00000000670B78A4: E8 DD 11 0C 00     call        0000000067178A86
  00000000670B78A9: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000000670B78AE: 48 85 C0           test        rax,rax
  00000000670B78B1: 74 0A              je          00000000670B78BD
  00000000670B78B3: 48 8B C8           mov         rcx,rax
  00000000670B78B6: E8 75 E2 FF FF     call        00000000670B5B30
  00000000670B78BB: EB 02              jmp         00000000670B78BF
  00000000670B78BD: 33 C0              xor         eax,eax
  00000000670B78BF: 45 33 C0           xor         r8d,r8d
  00000000670B78C2: 48 8B D0           mov         rdx,rax
  00000000670B78C5: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670B78CA: E8 B1 26 00 00     call        00000000670B9F80
  00000000670B78CF: 90                 nop
  00000000670B78D0: 48 8D 35 41 24 17  lea         rsi,[0000000067229D18h]
                    00
  00000000670B78D7: 48 89 74 24 28     mov         qword ptr [rsp+28h],rsi
  00000000670B78DC: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670B78E1: 48 81 C1 E8 00 00  add         rcx,0E8h
                    00
  00000000670B78E8: 48 8B D7           mov         rdx,rdi
  00000000670B78EB: E8 E0 8A FB FF     call        00000000670703D0
  00000000670B78F0: 90                 nop
  00000000670B78F1: 48 8B D3           mov         rdx,rbx
  00000000670B78F4: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670B78F9: E8 E2 F8 FF FF     call        00000000670B71E0
  00000000670B78FE: 0F B6 D8           movzx       ebx,al
  00000000670B7901: 48 89 74 24 28     mov         qword ptr [rsp+28h],rsi
  00000000670B7906: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670B790B: E8 20 E6 FF FF     call        00000000670B5F30
  00000000670B7910: 90                 nop
  00000000670B7911: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670B7916: E8 95 26 00 00     call        00000000670B9FB0
  00000000670B791B: 8A C3              mov         al,bl
  00000000670B791D: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000670B7922: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  00000000670B7927: 48 83 C4 40        add         rsp,40h
  00000000670B792B: 5F                 pop         rdi
  00000000670B792C: C3                 ret

  Summary

      1A7000 .text

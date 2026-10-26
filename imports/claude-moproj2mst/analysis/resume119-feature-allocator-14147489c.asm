Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014147489C: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001414748A1: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000001414748A6: 57                 push        rdi
  00000001414748A7: 48 83 EC 20        sub         rsp,20h
  00000001414748AB: 48 8B D9           mov         rbx,rcx
  00000001414748AE: 48 83 F9 E0        cmp         rcx,0FFFFFFFFFFFFFFE0h
  00000001414748B2: 77 7C              ja          0000000141474930
  00000001414748B4: BF 01 00 00 00     mov         edi,1
  00000001414748B9: 48 85 C9           test        rcx,rcx
  00000001414748BC: 48 0F 45 F9        cmovne      rdi,rcx
  00000001414748C0: 48 8B 0D 49 BF 28  mov         rcx,qword ptr [0000000144700810h]
                    03
  00000001414748C7: 48 85 C9           test        rcx,rcx
  00000001414748CA: 75 20              jne         00000001414748EC
  00000001414748CC: E8 CF A0 00 00     call        000000014147E9A0
  00000001414748D1: B9 1E 00 00 00     mov         ecx,1Eh
  00000001414748D6: E8 9D 9E 00 00     call        000000014147E778
  00000001414748DB: B9 FF 00 00 00     mov         ecx,0FFh
  00000001414748E0: E8 FB 9D FF FF     call        000000014146E6E0
  00000001414748E5: 48 8B 0D 24 BF 28  mov         rcx,qword ptr [0000000144700810h]
                    03
  00000001414748EC: 4C 8B C7           mov         r8,rdi
  00000001414748EF: 33 D2              xor         edx,edx
  00000001414748F1: FF 15 89 EA 98 00  call        qword ptr [0000000141E03380h]
  00000001414748F7: 48 8B F0           mov         rsi,rax
  00000001414748FA: 48 85 C0           test        rax,rax
  00000001414748FD: 75 2C              jne         000000014147492B
  00000001414748FF: 39 05 53 BF 28 03  cmp         dword ptr [0000000144700858h],eax
  0000000141474905: 74 0E              je          0000000141474915
  0000000141474907: 48 8B CB           mov         rcx,rbx
  000000014147490A: E8 D9 D3 00 00     call        0000000141481CE8
  000000014147490F: 85 C0              test        eax,eax
  0000000141474911: 74 0D              je          0000000141474920
  0000000141474913: EB AB              jmp         00000001414748C0
  0000000141474915: E8 1A F8 FF FF     call        0000000141474134
  000000014147491A: C7 00 0C 00 00 00  mov         dword ptr [rax],0Ch
  0000000141474920: E8 0F F8 FF FF     call        0000000141474134
  0000000141474925: C7 00 0C 00 00 00  mov         dword ptr [rax],0Ch
  000000014147492B: 48 8B C6           mov         rax,rsi
  000000014147492E: EB 12              jmp         0000000141474942
  0000000141474930: E8 B3 D3 00 00     call        0000000141481CE8
  0000000141474935: E8 FA F7 FF FF     call        0000000141474134
  000000014147493A: C7 00 0C 00 00 00  mov         dword ptr [rax],0Ch
  0000000141474940: 33 C0              xor         eax,eax
  0000000141474942: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000141474947: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000014147494C: 48 83 C4 20        add         rsp,20h
  0000000141474950: 5F                 pop         rdi
  0000000141474951: C3                 ret

  Summary

     1E02000 .text

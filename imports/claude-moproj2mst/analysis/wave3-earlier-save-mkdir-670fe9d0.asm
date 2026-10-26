Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670FE9D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670FE9D5: 53                 push        rbx
  00000000670FE9D6: 56                 push        rsi
  00000000670FE9D7: 57                 push        rdi
  00000000670FE9D8: 48 83 EC 30        sub         rsp,30h
  00000000670FE9DC: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670FE9E5: 48 8B FA           mov         rdi,rdx
  00000000670FE9E8: 48 8B D9           mov         rbx,rcx
  00000000670FE9EB: BE 5C 00 00 00     mov         esi,5Ch
  00000000670FE9F0: 66 89 74 24 60     mov         word ptr [rsp+60h],si
  00000000670FE9F5: 44 8D 46 A5        lea         r8d,[rsi-5Bh]
  00000000670FE9F9: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000670FE9FE: E8 8D D4 F6 FF     call        000000006706BE90
  00000000670FEA03: 84 C0              test        al,al
  00000000670FEA05: 74 32              je          00000000670FEA39
  00000000670FEA07: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  00000000670FEA10: BA 01 00 00 00     mov         edx,1
  00000000670FEA15: 48 8B CB           mov         rcx,rbx
  00000000670FEA18: E8 93 5C F7 FF     call        00000000670746B0
  00000000670FEA1D: 66 89 74 24 60     mov         word ptr [rsp+60h],si
  00000000670FEA22: 41 B8 01 00 00 00  mov         r8d,1
  00000000670FEA28: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000670FEA2D: 48 8B CB           mov         rcx,rbx
  00000000670FEA30: E8 5B D4 F6 FF     call        000000006706BE90
  00000000670FEA35: 84 C0              test        al,al
  00000000670FEA37: 75 D7              jne         00000000670FEA10
  00000000670FEA39: B8 3A 00 00 00     mov         eax,3Ah
  00000000670FEA3E: 66 89 44 24 68     mov         word ptr [rsp+68h],ax
  00000000670FEA43: 44 8D 40 C7        lea         r8d,[rax-39h]
  00000000670FEA47: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000000670FEA4C: 48 8B CB           mov         rcx,rbx
  00000000670FEA4F: E8 3C D4 F6 FF     call        000000006706BE90
  00000000670FEA54: 84 C0              test        al,al
  00000000670FEA56: 75 39              jne         00000000670FEA91
  00000000670FEA58: 48 8B CB           mov         rcx,rbx
  00000000670FEA5B: E8 F0 1F F7 FF     call        0000000067070A50
  00000000670FEA60: 48 8B C8           mov         rcx,rax
  00000000670FEA63: 48 8B D7           mov         rdx,rdi
  00000000670FEA66: FF 15 AC 97 0A 00  call        qword ptr [00000000671A8218h]
  00000000670FEA6C: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  00000000670FEA70: 74 1F              je          00000000670FEA91
  00000000670FEA72: 48 8B C8           mov         rcx,rax
  00000000670FEA75: FF 15 95 97 0A 00  call        qword ptr [00000000671A8210h]
  00000000670FEA7B: 90                 nop
  00000000670FEA7C: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670FEA7F: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FEA83: 75 08              jne         00000000670FEA8D
  00000000670FEA85: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670FEA88: E8 43 16 F7 FF     call        00000000670700D0
  00000000670FEA8D: B0 01              mov         al,1
  00000000670FEA8F: EB 13              jmp         00000000670FEAA4
  00000000670FEA91: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000670FEA94: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FEA98: 75 08              jne         00000000670FEAA2
  00000000670FEA9A: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000670FEA9D: E8 2E 16 F7 FF     call        00000000670700D0
  00000000670FEAA2: 32 C0              xor         al,al
  00000000670FEAA4: 48 83 C4 30        add         rsp,30h
  00000000670FEAA8: 5F                 pop         rdi
  00000000670FEAA9: 5E                 pop         rsi
  00000000670FEAAA: 5B                 pop         rbx
  00000000670FEAAB: C3                 ret

  Summary

      1A7000 .text

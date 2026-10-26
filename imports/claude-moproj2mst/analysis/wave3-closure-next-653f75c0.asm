Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653F75C0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000653F75C5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000653F75CA: 57                 push        rdi
  00000000653F75CB: 48 83 EC 20        sub         rsp,20h
  00000000653F75CF: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  00000000653F75D3: 48 8B D9           mov         rbx,rcx
  00000000653F75D6: 48 8B CA           mov         rcx,rdx
  00000000653F75D9: 48 8B FA           mov         rdi,rdx
  00000000653F75DC: E8 7F D6 C1 FF     call        0000000065014C60
  00000000653F75E1: 83 F8 04           cmp         eax,4
  00000000653F75E4: 74 48              je          00000000653F762E
  00000000653F75E6: 48 8B CB           mov         rcx,rbx
  00000000653F75E9: E8 22 FE FF FF     call        00000000653F7410
  00000000653F75EE: 84 C0              test        al,al
  00000000653F75F0: 74 3C              je          00000000653F762E
  00000000653F75F2: 8B 86 38 02 00 00  mov         eax,dword ptr [rsi+0000000000000238h]
  00000000653F75F8: A8 04              test        al,4
  00000000653F75FA: 75 32              jne         00000000653F762E
  00000000653F75FC: 83 C8 04           or          eax,4
  00000000653F75FF: 48 8B CB           mov         rcx,rbx
  00000000653F7602: 89 86 38 02 00 00  mov         dword ptr [rsi+0000000000000238h],eax
  00000000653F7608: E8 53 55 C6 FF     call        000000006505CB60
  00000000653F760D: 48 8D 0D 94 38 32  lea         rcx,[000000006571AEA8h]
                    00
  00000000653F7614: 48 8B D0           mov         rdx,rax
  00000000653F7617: FF 15 EB F3 29 00  call        qword ptr [0000000065696A08h]
  00000000653F761D: 48 85 C0           test        rax,rax
  00000000653F7620: 74 0C              je          00000000653F762E
  00000000653F7622: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000000653F7626: 48 8B D3           mov         rdx,rbx
  00000000653F7629: E8 32 3A 06 00     call        000000006545B060
  00000000653F762E: 48 8B D7           mov         rdx,rdi
  00000000653F7631: 48 8B CB           mov         rcx,rbx
  00000000653F7634: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000653F7639: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  00000000653F763E: 48 83 C4 20        add         rsp,20h
  00000000653F7642: 5F                 pop         rdi
  00000000653F7643: E9 18 C3 F7 FF     jmp         0000000065373960

  Summary

      695000 .text

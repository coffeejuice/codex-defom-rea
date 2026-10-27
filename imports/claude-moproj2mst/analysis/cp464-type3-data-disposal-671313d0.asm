Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000671313D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000671313D5: 56                 push        rsi
  00000000671313D6: 57                 push        rdi
  00000000671313D7: 41 54              push        r12
  00000000671313D9: 48 83 EC 30        sub         rsp,30h
  00000000671313DD: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000671313E6: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  00000000671313EB: 48 8B FA           mov         rdi,rdx
  00000000671313EE: 48 8B D9           mov         rbx,rcx
  00000000671313F1: 48 8D 05 D0 B6 10  lea         rax,[000000006723CAC8h]
                    00
  00000000671313F8: 48 89 01           mov         qword ptr [rcx],rax
  00000000671313FB: B9 88 00 00 00     mov         ecx,88h
  0000000067131400: E8 81 76 04 00     call        0000000067178A86
  0000000067131405: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000006713140A: 48 85 C0           test        rax,rax
  000000006713140D: 74 11              je          0000000067131420
  000000006713140F: BA 04 08 04 00     mov         edx,40804h
  0000000067131414: 48 8B C8           mov         rcx,rax
  0000000067131417: E8 44 FD FF FF     call        0000000067131160
  000000006713141C: 33 F6              xor         esi,esi
  000000006713141E: EB 05              jmp         0000000067131425
  0000000067131420: 33 F6              xor         esi,esi
  0000000067131422: 48 8B C6           mov         rax,rsi
  0000000067131425: 48 89 43 08        mov         qword ptr [rbx+8],rax
  0000000067131429: 4C 8B E0           mov         r12,rax
  000000006713142C: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000067131431: 48 89 58 08        mov         qword ptr [rax+8],rbx
  0000000067131435: 48 85 FF           test        rdi,rdi
  0000000067131438: 74 0F              je          0000000067131449
  000000006713143A: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006713143E: 48 8B 41 40        mov         rax,qword ptr [rcx+40h]
  0000000067131442: 48 83 78 08 00     cmp         qword ptr [rax+8],0
  0000000067131447: 74 05              je          000000006713144E
  0000000067131449: E8 52 98 EF FF     call        000000006702ACA0
  000000006713144E: 49 89 44 24 40     mov         qword ptr [r12+40h],rax
  0000000067131453: 48 8B C8           mov         rcx,rax
  0000000067131456: E8 D5 61 EF FF     call        0000000067027630
  000000006713145B: 48 85 FF           test        rdi,rdi
  000000006713145E: 74 28              je          0000000067131488
  0000000067131460: 48 8B 57 08        mov         rdx,qword ptr [rdi+8]
  0000000067131464: 4D 8B 44 24 40     mov         r8,qword ptr [r12+40h]
  0000000067131469: 48 8B 52 40        mov         rdx,qword ptr [rdx+40h]
  000000006713146D: 48 8B CF           mov         rcx,rdi
  0000000067131470: E8 DB 85 FF FF     call        0000000067129A50
  0000000067131475: 84 C0              test        al,al
  0000000067131477: 48 0F 44 FE        cmove       rdi,rsi
  000000006713147B: 48 8B D7           mov         rdx,rdi
  000000006713147E: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  0000000067131482: E8 69 BF FF FF     call        000000006712D3F0
  0000000067131487: 90                 nop
  0000000067131488: F0 83 05 B4 2E 1C  lock add    dword ptr [00000000672F4344h],1
                    00 01
  0000000067131490: 48 8B C3           mov         rax,rbx
  0000000067131493: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  0000000067131498: 48 83 C4 30        add         rsp,30h
  000000006713149C: 41 5C              pop         r12
  000000006713149E: 5F                 pop         rdi
  000000006713149F: 5E                 pop         rsi
  00000000671314A0: C3                 ret

  Summary

      1A7000 .text

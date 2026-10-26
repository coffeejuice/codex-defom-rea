Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006506A6F0: 40 56              push        rsi
  000000006506A6F2: 57                 push        rdi
  000000006506A6F3: 41 54              push        r12
  000000006506A6F5: 48 83 EC 40        sub         rsp,40h
  000000006506A6F9: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006506A702: 48 89 5C 24 68     mov         qword ptr [rsp+68h],rbx
  000000006506A707: 48 89 6C 24 70     mov         qword ptr [rsp+70h],rbp
  000000006506A70C: 48 8B EA           mov         rbp,rdx
  000000006506A70F: 4C 8B E1           mov         r12,rcx
  000000006506A712: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006506A716: 48 8B CA           mov         rcx,rdx
  000000006506A719: E8 52 48 FC FF     call        000000006502EF70
  000000006506A71E: 85 C0              test        eax,eax
  000000006506A720: 75 13              jne         000000006506A735
  000000006506A722: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  000000006506A729: 48 85 C0           test        rax,rax
  000000006506A72C: 74 60              je          000000006506A78E
  000000006506A72E: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  000000006506A733: 74 59              je          000000006506A78E
  000000006506A735: 48 8B CB           mov         rcx,rbx
  000000006506A738: E8 C3 87 FF FF     call        0000000065062F00
  000000006506A73D: B9 08 00 00 00     mov         ecx,8
  000000006506A742: E8 2B 6A 57 00     call        00000000655E1172
  000000006506A747: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006506A74C: 48 85 C0           test        rax,rax
  000000006506A74F: 74 10              je          000000006506A761
  000000006506A751: 48 8B D5           mov         rdx,rbp
  000000006506A754: 48 8B C8           mov         rcx,rax
  000000006506A757: E8 04 4A FC FF     call        000000006502F160
  000000006506A75C: 48 8B F0           mov         rsi,rax
  000000006506A75F: EB 02              jmp         000000006506A763
  000000006506A761: 33 F6              xor         esi,esi
  000000006506A763: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  000000006506A76A: 48 8B 78 18        mov         rdi,qword ptr [rax+18h]
  000000006506A76E: 48 85 FF           test        rdi,rdi
  000000006506A771: 74 10              je          000000006506A783
  000000006506A773: 48 8B CF           mov         rcx,rdi
  000000006506A776: E8 25 4A FC FF     call        000000006502F1A0
  000000006506A77B: 48 8B CF           mov         rcx,rdi
  000000006506A77E: E8 2B 69 57 00     call        00000000655E10AE
  000000006506A783: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  000000006506A78A: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  000000006506A78E: 41 B0 01           mov         r8b,1
  000000006506A791: BA 26 00 00 00     mov         edx,26h
  000000006506A796: 49 8B CC           mov         rcx,r12
  000000006506A799: E8 42 E8 FF FF     call        0000000065068FE0
  000000006506A79E: 48 8B D5           mov         rdx,rbp
  000000006506A7A1: 48 8B CB           mov         rcx,rbx
  000000006506A7A4: E8 67 C6 02 00     call        0000000065096E10
  000000006506A7A9: BA B7 00 00 00     mov         edx,0B7h
  000000006506A7AE: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006506A7B3: FF 15 5F C0 62 00  call        qword ptr [0000000065696818h]
  000000006506A7B9: 90                 nop
  000000006506A7BA: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006506A7BF: 49 8B CC           mov         rcx,r12
  000000006506A7C2: FF 15 48 C0 62 00  call        qword ptr [0000000065696810h]
  000000006506A7C8: 90                 nop
  000000006506A7C9: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006506A7CE: FF 15 34 C0 62 00  call        qword ptr [0000000065696808h]
  000000006506A7D4: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  000000006506A7D9: 48 8B 6C 24 70     mov         rbp,qword ptr [rsp+70h]
  000000006506A7DE: 48 83 C4 40        add         rsp,40h
  000000006506A7E2: 41 5C              pop         r12
  000000006506A7E4: 5F                 pop         rdi
  000000006506A7E5: 5E                 pop         rsi
  000000006506A7E6: C3                 ret

  Summary

      695000 .text

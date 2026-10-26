Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006706C120: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006706C125: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  000000006706C12A: 57                 push        rdi
  000000006706C12B: 48 83 EC 40        sub         rsp,40h
  000000006706C12F: 48 8B F1           mov         rsi,rcx
  000000006706C132: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006706C137: 41 8B F8           mov         edi,r8d
  000000006706C13A: 48 8B DA           mov         rbx,rdx
  000000006706C13D: E8 AE 13 FE FF     call        000000006704D4F0
  000000006706C142: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006706C147: E8 F4 D1 FD FF     call        0000000067049340
  000000006706C14C: 4C 8D 4C 24 68     lea         r9,[rsp+68h]
  000000006706C151: 44 8B C7           mov         r8d,edi
  000000006706C154: 48 8B D6           mov         rdx,rsi
  000000006706C157: 48 8B C8           mov         rcx,rax
  000000006706C15A: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000006706C162: E8 F9 02 FE FF     call        000000006704C460
  000000006706C167: 80 7C 24 68 00     cmp         byte ptr [rsp+68h],0
  000000006706C16C: 74 18              je          000000006706C186
  000000006706C16E: 48 85 DB           test        rbx,rbx
  000000006706C171: 74 47              je          000000006706C1BA
  000000006706C173: C6 03 01           mov         byte ptr [rbx],1
  000000006706C176: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  000000006706C17B: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  000000006706C180: 48 83 C4 40        add         rsp,40h
  000000006706C184: 5F                 pop         rdi
  000000006706C185: C3                 ret
  000000006706C186: 45 33 C0           xor         r8d,r8d
  000000006706C189: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006706C18E: 41 8D 50 01        lea         edx,[r8+1]
  000000006706C192: E8 09 14 FE FF     call        000000006704D5A0
  000000006706C197: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006706C19C: E8 9F D1 FD FF     call        0000000067049340
  000000006706C1A1: 4C 8B CB           mov         r9,rbx
  000000006706C1A4: 44 8B C7           mov         r8d,edi
  000000006706C1A7: 48 8B D6           mov         rdx,rsi
  000000006706C1AA: 48 8B C8           mov         rcx,rax
  000000006706C1AD: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000006706C1B5: E8 A6 02 FE FF     call        000000006704C460
  000000006706C1BA: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  000000006706C1BF: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  000000006706C1C4: 48 83 C4 40        add         rsp,40h
  000000006706C1C8: 5F                 pop         rdi
  000000006706C1C9: C3                 ret

  Summary

      1A7000 .text

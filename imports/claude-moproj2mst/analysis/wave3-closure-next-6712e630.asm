Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712E630: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006712E635: 53                 push        rbx
  000000006712E636: 56                 push        rsi
  000000006712E637: 57                 push        rdi
  000000006712E638: 48 83 EC 30        sub         rsp,30h
  000000006712E63C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006712E645: 49 8B D8           mov         rbx,r8
  000000006712E648: 48 8B F9           mov         rdi,rcx
  000000006712E64B: 48 8D 05 76 E4 10  lea         rax,[000000006723CAC8h]
                    00
  000000006712E652: 48 89 01           mov         qword ptr [rcx],rax
  000000006712E655: 48 89 51 08        mov         qword ptr [rcx+8],rdx
  000000006712E659: 48 8B F2           mov         rsi,rdx
  000000006712E65C: 48 89 54 24 60     mov         qword ptr [rsp+60h],rdx
  000000006712E661: 48 89 4A 08        mov         qword ptr [rdx+8],rcx
  000000006712E665: 4D 85 C0           test        r8,r8
  000000006712E668: 74 0F              je          000000006712E679
  000000006712E66A: 49 8B 48 08        mov         rcx,qword ptr [r8+8]
  000000006712E66E: 48 8B 41 40        mov         rax,qword ptr [rcx+40h]
  000000006712E672: 48 83 78 08 00     cmp         qword ptr [rax+8],0
  000000006712E677: 74 05              je          000000006712E67E
  000000006712E679: E8 22 C6 EF FF     call        000000006702ACA0
  000000006712E67E: 48 89 46 40        mov         qword ptr [rsi+40h],rax
  000000006712E682: 48 8B C8           mov         rcx,rax
  000000006712E685: E8 A6 8F EF FF     call        0000000067027630
  000000006712E68A: 48 85 DB           test        rbx,rbx
  000000006712E68D: 74 51              je          000000006712E6E0
  000000006712E68F: 48 8B 53 08        mov         rdx,qword ptr [rbx+8]
  000000006712E693: 4C 8B 46 40        mov         r8,qword ptr [rsi+40h]
  000000006712E697: 48 8B 52 40        mov         rdx,qword ptr [rdx+40h]
  000000006712E69B: 48 8B CB           mov         rcx,rbx
  000000006712E69E: E8 AD B3 FF FF     call        0000000067129A50
  000000006712E6A3: 33 C9              xor         ecx,ecx
  000000006712E6A5: 84 C0              test        al,al
  000000006712E6A7: 48 0F 44 D9        cmove       rbx,rcx
  000000006712E6AB: F6 46 20 01        test        byte ptr [rsi+20h],1
  000000006712E6AF: 74 22              je          000000006712E6D3
  000000006712E6B1: 48 85 DB           test        rbx,rbx
  000000006712E6B4: 74 2A              je          000000006712E6E0
  000000006712E6B6: 48 89 5E 10        mov         qword ptr [rsi+10h],rbx
  000000006712E6BA: 48 89 7C 24 58     mov         qword ptr [rsp+58h],rdi
  000000006712E6BF: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006712E6C3: 48 83 C1 18        add         rcx,18h
  000000006712E6C7: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006712E6CC: E8 7F 58 FD FF     call        0000000067103F50
  000000006712E6D1: EB 0D              jmp         000000006712E6E0
  000000006712E6D3: 48 8B D3           mov         rdx,rbx
  000000006712E6D6: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006712E6DA: E8 11 ED FF FF     call        000000006712D3F0
  000000006712E6DF: 90                 nop
  000000006712E6E0: F0 83 05 5C 5C 1C  lock add    dword ptr [00000000672F4344h],1
                    00 01
  000000006712E6E8: 48 8B C7           mov         rax,rdi
  000000006712E6EB: 48 83 C4 30        add         rsp,30h
  000000006712E6EF: 5F                 pop         rdi
  000000006712E6F0: 5E                 pop         rsi
  000000006712E6F1: 5B                 pop         rbx
  000000006712E6F2: C3                 ret

  Summary

      1A7000 .text

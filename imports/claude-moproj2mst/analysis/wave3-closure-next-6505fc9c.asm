Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505FC9C: 48 89 6C 24 40     mov         qword ptr [rsp+40h],rbp
  000000006505FCA1: 4C 89 64 24 48     mov         qword ptr [rsp+48h],r12
  000000006505FCA6: 4C 89 6C 24 50     mov         qword ptr [rsp+50h],r13
  000000006505FCAB: 48 85 C9           test        rcx,rcx
  000000006505FCAE: 74 05              je          000000006505FCB5
  000000006505FCB0: E8 BB FE FF FF     call        000000006505FB70
  000000006505FCB5: 49 8B 9E 90 00 00  mov         rbx,qword ptr [r14+0000000000000090h]
                    00
  000000006505FCBC: 45 33 ED           xor         r13d,r13d
  000000006505FCBF: 4C 8B E6           mov         r12,rsi
  000000006505FCC2: 49 8B FD           mov         rdi,r13
  000000006505FCC5: 40 B5 01           mov         bpl,1
  000000006505FCC8: 48 3B DE           cmp         rbx,rsi
  000000006505FCCB: 0F 84 AF 00 00 00  je          000000006505FD80
  000000006505FCD1: 48 8B CB           mov         rcx,rbx
  000000006505FCD4: 48 85 DB           test        rbx,rbx
  000000006505FCD7: 74 24              je          000000006505FCFD
  000000006505FCD9: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006505FCE0: 48 3B CE           cmp         rcx,rsi
  000000006505FCE3: 74 40              je          000000006505FD25
  000000006505FCE5: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006505FCE9: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  000000006505FCED: 75 32              jne         000000006505FD21
  000000006505FCEF: FF 15 73 6B 63 00  call        qword ptr [0000000065696868h]
  000000006505FCF5: 48 8B C8           mov         rcx,rax
  000000006505FCF8: 48 85 C0           test        rax,rax
  000000006505FCFB: 75 E3              jne         000000006505FCE0
  000000006505FCFD: 32 C9              xor         cl,cl
  000000006505FCFF: 40 84 ED           test        bpl,bpl
  000000006505FD02: 74 47              je          000000006505FD4B
  000000006505FD04: 48 85 FF           test        rdi,rdi
  000000006505FD07: 74 3F              je          000000006505FD48
  000000006505FD09: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006505FD0D: 48 89 98 90 00 00  mov         qword ptr [rax+0000000000000090h],rbx
                    00
  000000006505FD14: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006505FD18: 48 89 B8 98 00 00  mov         qword ptr [rax+0000000000000098h],rdi
                    00
  000000006505FD1F: EB 2A              jmp         000000006505FD4B
  000000006505FD21: 32 C9              xor         cl,cl
  000000006505FD23: EB DA              jmp         000000006505FCFF
  000000006505FD25: B1 01              mov         cl,1
  000000006505FD27: 40 84 ED           test        bpl,bpl
  000000006505FD2A: 75 17              jne         000000006505FD43
  000000006505FD2C: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  000000006505FD31: 48 89 98 90 00 00  mov         qword ptr [rax+0000000000000090h],rbx
                    00
  000000006505FD38: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006505FD3C: 4C 89 A0 98 00 00  mov         qword ptr [rax+0000000000000098h],r12
                    00
  000000006505FD43: 4C 8B E3           mov         r12,rbx
  000000006505FD46: EB 06              jmp         000000006505FD4E
  000000006505FD48: 4C 8B EB           mov         r13,rbx
  000000006505FD4B: 48 8B FB           mov         rdi,rbx
  000000006505FD4E: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006505FD52: 0F B6 E9           movzx       ebp,cl
  000000006505FD55: 48 8B 98 90 00 00  mov         rbx,qword ptr [rax+0000000000000090h]
                    00
  000000006505FD5C: 48 3B DE           cmp         rbx,rsi
  000000006505FD5F: 0F 85 6C FF FF FF  jne         000000006505FCD1
  000000006505FD65: 4D 85 ED           test        r13,r13
  000000006505FD68: 74 16              je          000000006505FD80
  000000006505FD6A: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006505FD6E: 4C 89 A8 90 00 00  mov         qword ptr [rax+0000000000000090h],r13
                    00
  000000006505FD75: 49 8B 45 08        mov         rax,qword ptr [r13+8]
  000000006505FD79: 48 89 B8 98 00 00  mov         qword ptr [rax+0000000000000098h],rdi
                    00
  000000006505FD80: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  000000006505FD84: 4C 8B 6C 24 50     mov         r13,qword ptr [rsp+50h]
  000000006505FD89: 48 8B 6C 24 40     mov         rbp,qword ptr [rsp+40h]
  000000006505FD8E: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  000000006505FD92: 75 6C              jne         000000006505FE00

  Summary

      695000 .text

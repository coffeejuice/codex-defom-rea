Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065404D00: 40 56              push        rsi
  0000000065404D02: 57                 push        rdi
  0000000065404D03: 41 54              push        r12
  0000000065404D05: 41 55              push        r13
  0000000065404D07: 41 56              push        r14
  0000000065404D09: 48 83 EC 40        sub         rsp,40h
  0000000065404D0D: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065404D16: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  0000000065404D1B: 48 89 6C 24 78     mov         qword ptr [rsp+78h],rbp
  0000000065404D20: 49 8B D8           mov         rbx,r8
  0000000065404D23: 44 8B F2           mov         r14d,edx
  0000000065404D26: 4C 8B E1           mov         r12,rcx
  0000000065404D29: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  0000000065404D2D: 48 8D A9 24 02 00  lea         rbp,[rcx+0000000000000224h]
                    00
  0000000065404D34: 48 89 6C 24 28     mov         qword ptr [rsp+28h],rbp
  0000000065404D39: 44 0F B6 6D 00     movzx       r13d,byte ptr [rbp]
  0000000065404D3E: 44 88 6C 24 30     mov         byte ptr [rsp+30h],r13b
  0000000065404D43: C6 45 00 01        mov         byte ptr [rbp],1
  0000000065404D47: 45 84 C9           test        r9b,r9b
  0000000065404D4A: 74 32              je          0000000065404D7E
  0000000065404D4C: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  0000000065404D50: 8B 50 08           mov         edx,dword ptr [rax+8]
  0000000065404D53: C1 EA 0F           shr         edx,0Fh
  0000000065404D56: 80 E2 01           and         dl,1
  0000000065404D59: 74 23              je          0000000065404D7E
  0000000065404D5B: 49 8B 40 20        mov         rax,qword ptr [r8+20h]
  0000000065404D5F: 0F B6 48 0A        movzx       ecx,byte ptr [rax+0Ah]
  0000000065404D63: 80 E1 01           and         cl,1
  0000000065404D66: 74 11              je          0000000065404D79
  0000000065404D68: BA 45 00 00 00     mov         edx,45h
  0000000065404D6D: 48 8B CB           mov         rcx,rbx
  0000000065404D70: E8 9B 92 C5 FF     call        000000006505E010
  0000000065404D75: 84 C0              test        al,al
  0000000065404D77: 75 05              jne         0000000065404D7E
  0000000065404D79: 40 B7 01           mov         dil,1
  0000000065404D7C: EB 03              jmp         0000000065404D81
  0000000065404D7E: 40 32 FF           xor         dil,dil
  0000000065404D81: 48 8B CB           mov         rcx,rbx
  0000000065404D84: FF 15 DE 1A 29 00  call        qword ptr [0000000065696868h]
  0000000065404D8A: 48 3B C6           cmp         rax,rsi
  0000000065404D8D: 74 0B              je          0000000065404D9A
  0000000065404D8F: 48 8B D6           mov         rdx,rsi
  0000000065404D92: 48 8B CB           mov         rcx,rbx
  0000000065404D95: E8 C6 BC C6 FF     call        0000000065070A60
  0000000065404D9A: 40 84 FF           test        dil,dil
  0000000065404D9D: 74 0B              je          0000000065404DAA
  0000000065404D9F: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065404DA2: B2 01              mov         dl,1
  0000000065404DA4: 48 8B CB           mov         rcx,rbx
  0000000065404DA7: FF 50 58           call        qword ptr [rax+58h]
  0000000065404DAA: 4C 8B C3           mov         r8,rbx
  0000000065404DAD: 41 8B D6           mov         edx,r14d
  0000000065404DB0: 49 8B CC           mov         rcx,r12
  0000000065404DB3: E8 E8 EB FF FF     call        00000000654039A0
  0000000065404DB8: 4C 8B 5E 20        mov         r11,qword ptr [rsi+20h]
  0000000065404DBC: 41 8B 53 08        mov         edx,dword ptr [r11+8]
  0000000065404DC0: C1 EA 0F           shr         edx,0Fh
  0000000065404DC3: 80 E2 01           and         dl,1
  0000000065404DC6: 49 8B CC           mov         rcx,r12
  0000000065404DC9: E8 52 FB FF FF     call        0000000065404920
  0000000065404DCE: 90                 nop
  0000000065404DCF: 44 88 6D 00        mov         byte ptr [rbp],r13b
  0000000065404DD3: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  0000000065404DD8: 48 8B 6C 24 78     mov         rbp,qword ptr [rsp+78h]
  0000000065404DDD: 48 83 C4 40        add         rsp,40h
  0000000065404DE1: 41 5E              pop         r14
  0000000065404DE3: 41 5D              pop         r13
  0000000065404DE5: 41 5C              pop         r12
  0000000065404DE7: 5F                 pop         rdi
  0000000065404DE8: 5E                 pop         rsi
  0000000065404DE9: C3                 ret

  Summary

      695000 .text

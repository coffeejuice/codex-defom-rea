Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C83080: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C83085: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C8308A: 48 81 EC A8 00 00  sub         rsp,0A8h
                    00
  0000000140C83091: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C8309A: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C830A2: 48 83 BC 24 B8 00  cmp         qword ptr [rsp+00000000000000B8h],0
                    00 00 00
  0000000140C830AB: 0F 84 0B 02 00 00  je          0000000140C832BC
  0000000140C830B1: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C830B9: 48 83 C1 18        add         rcx,18h
  0000000140C830BD: E8 4E 43 68 FF     call        0000000140307410
  0000000140C830C2: 0F B6 C0           movzx       eax,al
  0000000140C830C5: 85 C0              test        eax,eax
  0000000140C830C7: 0F 85 EF 01 00 00  jne         0000000140C832BC
  0000000140C830CD: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140C830D5: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  0000000140C830DE: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C830E6: 48 83 C2 18        add         rdx,18h
  0000000140C830EA: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140C830EF: E8 FC A9 3E FF     call        000000014006DAF0
  0000000140C830F4: 90                 nop
  0000000140C830F5: 48 8D 44 24 38     lea         rax,[rsp+38h]
  0000000140C830FA: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C830FF: EB 43              jmp         0000000140C83144
  0000000140C83101: 33 C0              xor         eax,eax
  0000000140C83103: 83 F8 01           cmp         eax,1
  0000000140C83106: 74 0B              je          0000000140C83113
  0000000140C83108: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  0000000140C83111: EB 16              jmp         0000000140C83129
  0000000140C83113: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C8311B: 48 83 C1 18        add         rcx,18h
  0000000140C8311F: E8 2C 1B 50 FF     call        0000000140184C50
  0000000140C83124: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140C83129: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  0000000140C8312E: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C83133: E8 48 92 17 00     call        0000000140DFC380
  0000000140C83138: 48 8B C8           mov         rcx,rax
  0000000140C8313B: 48 83 C1 10        add         rcx,10h
  0000000140C8313F: E8 BC 66 16 00     call        0000000140DE9800
  0000000140C83144: 33 C0              xor         eax,eax
  0000000140C83146: 83 F8 01           cmp         eax,1
  0000000140C83149: 74 0B              je          0000000140C83156
  0000000140C8314B: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  0000000140C83154: EB 16              jmp         0000000140C8316C
  0000000140C83156: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C8315E: 48 83 C1 18        add         rcx,18h
  0000000140C83162: E8 E9 1A 50 FF     call        0000000140184C50
  0000000140C83167: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140C8316C: 48 8B 54 24 70     mov         rdx,qword ptr [rsp+70h]
  0000000140C83171: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C83176: E8 05 92 17 00     call        0000000140DFC380
  0000000140C8317B: 48 8B C8           mov         rcx,rax
  0000000140C8317E: E8 FD 8E 17 00     call        0000000140DFC080
  0000000140C83183: 0F B6 C0           movzx       eax,al
  0000000140C83186: 85 C0              test        eax,eax
  0000000140C83188: 0F 84 24 01 00 00  je          0000000140C832B2
  0000000140C8318E: 33 C0              xor         eax,eax
  0000000140C83190: 83 F8 01           cmp         eax,1
  0000000140C83193: 74 0B              je          0000000140C831A0
  0000000140C83195: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  0000000140C8319E: EB 16              jmp         0000000140C831B6
  0000000140C831A0: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C831A8: 48 83 C1 18        add         rcx,18h
  0000000140C831AC: E8 9F 1A 50 FF     call        0000000140184C50
  0000000140C831B1: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140C831B6: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140C831BB: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C831C0: E8 BB 91 17 00     call        0000000140DFC380
  0000000140C831C5: 48 8B C8           mov         rcx,rax
  0000000140C831C8: 48 83 C1 10        add         rcx,10h
  0000000140C831CC: E8 2F C2 9C FF     call        000000014064F400
  0000000140C831D1: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C831D4: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C831D9: EB 63              jmp         0000000140C8323E
  0000000140C831DB: 33 C0              xor         eax,eax
  0000000140C831DD: 83 F8 01           cmp         eax,1
  0000000140C831E0: 74 0E              je          0000000140C831F0
  0000000140C831E2: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00 00
  0000000140C831EE: EB 19              jmp         0000000140C83209
  0000000140C831F0: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C831F8: 48 83 C1 18        add         rcx,18h
  0000000140C831FC: E8 4F 1A 50 FF     call        0000000140184C50
  0000000140C83201: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140C83209: 48 8B 94 24 80 00  mov         rdx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C83211: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C83216: E8 65 91 17 00     call        0000000140DFC380
  0000000140C8321B: 48 83 C0 08        add         rax,8
  0000000140C8321F: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140C83227: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140C8322F: 8B 08              mov         ecx,dword ptr [rax]
  0000000140C83231: 83 E9 01           sub         ecx,1
  0000000140C83234: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140C8323C: 89 08              mov         dword ptr [rax],ecx
  0000000140C8323E: 33 C0              xor         eax,eax
  0000000140C83240: 83 F8 01           cmp         eax,1
  0000000140C83243: 74 0E              je          0000000140C83253
  0000000140C83245: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00 00
  0000000140C83251: EB 19              jmp         0000000140C8326C
  0000000140C83253: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C8325B: 48 83 C1 18        add         rcx,18h
  0000000140C8325F: E8 EC 19 50 FF     call        0000000140184C50
  0000000140C83264: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140C8326C: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140C83274: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C83279: E8 02 91 17 00     call        0000000140DFC380
  0000000140C8327E: 83 78 08 00        cmp         dword ptr [rax+8],0
  0000000140C83282: 74 29              je          0000000140C832AD
  0000000140C83284: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140C83288: 83 C0 01           add         eax,1
  0000000140C8328B: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140C8328F: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140C83297: 48 39 44 24 28     cmp         qword ptr [rsp+28h],rax
  0000000140C8329C: 75 0A              jne         0000000140C832A8
  0000000140C8329E: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140C832A2: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140C832A6: EB 05              jmp         0000000140C832AD
  0000000140C832A8: E9 2E FF FF FF     jmp         0000000140C831DB
  0000000140C832AD: E9 4F FE FF FF     jmp         0000000140C83101
  0000000140C832B2: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140C832B7: E8 64 E2 FC FF     call        0000000140C51520
  0000000140C832BC: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140C832C0: 48 81 C4 A8 00 00  add         rsp,0A8h
                    00
  0000000140C832C7: C3                 ret

  Summary

     1E02000 .text

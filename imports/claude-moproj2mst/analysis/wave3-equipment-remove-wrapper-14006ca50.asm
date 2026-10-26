Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014006CA50: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014006CA54: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014006CA59: 48 81 EC B8 00 00  sub         rsp,0B8h
                    00
  000000014006CA60: 48 C7 44 24 70 FE  mov         qword ptr [rsp+70h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014006CA69: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014006CA71: 48 8B 00           mov         rax,qword ptr [rax]
  000000014006CA74: 8B 94 24 C8 00 00  mov         edx,dword ptr [rsp+00000000000000C8h]
                    00
  000000014006CA7B: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014006CA83: FF 90 F0 01 00 00  call        qword ptr [rax+00000000000001F0h]
  000000014006CA89: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  000000014006CA8D: 83 7C 24 30 00     cmp         dword ptr [rsp+30h],0
  000000014006CA92: 7D 05              jge         000000014006CA99
  000000014006CA94: E9 3D 02 00 00     jmp         000000014006CCD6
  000000014006CA99: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014006CAA1: 48 8B 00           mov         rax,qword ptr [rax]
  000000014006CAA4: 8B 94 24 C8 00 00  mov         edx,dword ptr [rsp+00000000000000C8h]
                    00
  000000014006CAAB: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014006CAB3: FF 90 A8 01 00 00  call        qword ptr [rax+00000000000001A8h]
  000000014006CAB9: 44 8B 44 24 30     mov         r8d,dword ptr [rsp+30h]
  000000014006CABE: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000014006CAC3: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  000000014006CACB: FF 15 DF EA D9 01  call        qword ptr [0000000141E0B5B0h]
  000000014006CAD1: 90                 nop
  000000014006CAD2: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  000000014006CADB: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000014006CAE0: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000014006CAE5: E8 06 10 00 00     call        000000014006DAF0
  000000014006CAEA: 90                 nop
  000000014006CAEB: 48 8D 44 24 38     lea         rax,[rsp+38h]
  000000014006CAF0: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014006CAF5: EB 3C              jmp         000000014006CB33
  000000014006CAF7: 33 C0              xor         eax,eax
  000000014006CAF9: 83 F8 01           cmp         eax,1
  000000014006CAFC: 74 0B              je          000000014006CB09
  000000014006CAFE: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  000000014006CB07: EB 0F              jmp         000000014006CB18
  000000014006CB09: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014006CB0E: E8 3D 81 11 00     call        0000000140184C50
  000000014006CB13: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000014006CB18: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  000000014006CB1D: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014006CB22: E8 59 F8 D8 00     call        0000000140DFC380
  000000014006CB27: 48 8B C8           mov         rcx,rax
  000000014006CB2A: 48 83 C1 10        add         rcx,10h
  000000014006CB2E: E8 CD CC D7 00     call        0000000140DE9800
  000000014006CB33: 33 C0              xor         eax,eax
  000000014006CB35: 83 F8 01           cmp         eax,1
  000000014006CB38: 74 0E              je          000000014006CB48
  000000014006CB3A: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00 00
  000000014006CB46: EB 12              jmp         000000014006CB5A
  000000014006CB48: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014006CB4D: E8 FE 80 11 00     call        0000000140184C50
  000000014006CB52: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014006CB5A: 48 8B 94 24 80 00  mov         rdx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014006CB62: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014006CB67: E8 14 F8 D8 00     call        0000000140DFC380
  000000014006CB6C: 48 8B C8           mov         rcx,rax
  000000014006CB6F: E8 0C F5 D8 00     call        0000000140DFC080
  000000014006CB74: 0F B6 C0           movzx       eax,al
  000000014006CB77: 85 C0              test        eax,eax
  000000014006CB79: 0F 84 42 01 00 00  je          000000014006CCC1
  000000014006CB7F: 33 C0              xor         eax,eax
  000000014006CB81: 83 F8 01           cmp         eax,1
  000000014006CB84: 74 0E              je          000000014006CB94
  000000014006CB86: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  000000014006CB92: EB 12              jmp         000000014006CBA6
  000000014006CB94: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014006CB99: E8 B2 80 11 00     call        0000000140184C50
  000000014006CB9E: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  000000014006CBA6: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014006CBAE: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014006CBB3: E8 C8 F7 D8 00     call        0000000140DFC380
  000000014006CBB8: 48 8B C8           mov         rcx,rax
  000000014006CBBB: 48 83 C1 10        add         rcx,10h
  000000014006CBBF: E8 3C 28 5E 00     call        000000014064F400
  000000014006CBC4: 48 8B 00           mov         rax,qword ptr [rax]
  000000014006CBC7: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014006CBCC: EB 5C              jmp         000000014006CC2A
  000000014006CBCE: 33 C0              xor         eax,eax
  000000014006CBD0: 83 F8 01           cmp         eax,1
  000000014006CBD3: 74 0E              je          000000014006CBE3
  000000014006CBD5: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00 00
  000000014006CBE1: EB 12              jmp         000000014006CBF5
  000000014006CBE3: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014006CBE8: E8 63 80 11 00     call        0000000140184C50
  000000014006CBED: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  000000014006CBF5: 48 8B 94 24 90 00  mov         rdx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014006CBFD: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014006CC02: E8 79 F7 D8 00     call        0000000140DFC380
  000000014006CC07: 48 83 C0 08        add         rax,8
  000000014006CC0B: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  000000014006CC13: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  000000014006CC1B: 8B 08              mov         ecx,dword ptr [rax]
  000000014006CC1D: 83 E9 01           sub         ecx,1
  000000014006CC20: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  000000014006CC28: 89 08              mov         dword ptr [rax],ecx
  000000014006CC2A: 33 C0              xor         eax,eax
  000000014006CC2C: 83 F8 01           cmp         eax,1
  000000014006CC2F: 74 0E              je          000000014006CC3F
  000000014006CC31: 48 C7 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],0
                    00 00 00 00 00 00
  000000014006CC3D: EB 12              jmp         000000014006CC51
  000000014006CC3F: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014006CC44: E8 07 80 11 00     call        0000000140184C50
  000000014006CC49: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  000000014006CC51: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014006CC59: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014006CC5E: E8 1D F7 D8 00     call        0000000140DFC380
  000000014006CC63: 83 78 08 00        cmp         dword ptr [rax+8],0
  000000014006CC67: 74 53              je          000000014006CCBC
  000000014006CC69: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014006CC6F: 74 46              je          000000014006CCB7
  000000014006CC71: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014006CC76: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014006CC7B: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  000000014006CC80: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014006CC85: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  000000014006CC8B: 74 1E              je          000000014006CCAB
  000000014006CC8D: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014006CC92: 48 8B 00           mov         rax,qword ptr [rax]
  000000014006CC95: BA 01 00 00 00     mov         edx,1
  000000014006CC9A: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014006CC9F: FF 10              call        qword ptr [rax]
  000000014006CCA1: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  000000014006CCA9: EB 0C              jmp         000000014006CCB7
  000000014006CCAB: 48 C7 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],0
                    00 00 00 00 00 00
  000000014006CCB7: E9 12 FF FF FF     jmp         000000014006CBCE
  000000014006CCBC: E9 36 FE FF FF     jmp         000000014006CAF7
  000000014006CCC1: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000014006CCC6: E8 55 48 BE 00     call        0000000140C51520
  000000014006CCCB: 90                 nop
  000000014006CCCC: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014006CCD1: E8 CA B4 19 00     call        00000001402081A0
  000000014006CCD6: 48 81 C4 B8 00 00  add         rsp,0B8h
                    00
  000000014006CCDD: C3                 ret

  Summary

     1E02000 .text

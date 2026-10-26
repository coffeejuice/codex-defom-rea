Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CFAC50: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CFAC55: 48 83 EC 68        sub         rsp,68h
  0000000140CFAC59: 48 C7 44 24 50 FE  mov         qword ptr [rsp+50h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140CFAC62: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140CFAC6B: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140CFAC70: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140CFAC75: 0F B6 80 A8 01 00  movzx       eax,byte ptr [rax+00000000000001A8h]
                    00
  0000000140CFAC7C: 85 C0              test        eax,eax
  0000000140CFAC7E: 74 36              je          0000000140CFACB6
  0000000140CFAC80: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CFAC85: 48 81 C1 A0 01 00  add         rcx,1A0h
                    00
  0000000140CFAC8C: E8 5F 02 F2 FF     call        0000000140C1AEF0
  0000000140CFAC91: 48 85 C0           test        rax,rax
  0000000140CFAC94: 74 20              je          0000000140CFACB6
  0000000140CFAC96: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CFAC9B: 48 81 C1 A0 01 00  add         rcx,1A0h
                    00
  0000000140CFACA2: E8 49 02 F2 FF     call        0000000140C1AEF0
  0000000140CFACA7: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CFACAC: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140CFACB1: E9 D5 00 00 00     jmp         0000000140CFAD8B
  0000000140CFACB6: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140CFACBB: 0F B6 80 D0 01 00  movzx       eax,byte ptr [rax+00000000000001D0h]
                    00
  0000000140CFACC2: 85 C0              test        eax,eax
  0000000140CFACC4: 74 36              je          0000000140CFACFC
  0000000140CFACC6: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CFACCB: 48 81 C1 C8 01 00  add         rcx,1C8h
                    00
  0000000140CFACD2: E8 19 02 F2 FF     call        0000000140C1AEF0
  0000000140CFACD7: 48 85 C0           test        rax,rax
  0000000140CFACDA: 74 20              je          0000000140CFACFC
  0000000140CFACDC: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CFACE1: 48 81 C1 C8 01 00  add         rcx,1C8h
                    00
  0000000140CFACE8: E8 03 02 F2 FF     call        0000000140C1AEF0
  0000000140CFACED: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CFACF2: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140CFACF7: E9 8F 00 00 00     jmp         0000000140CFAD8B
  0000000140CFACFC: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CFAD01: 48 81 C1 D8 01 00  add         rcx,1D8h
                    00
  0000000140CFAD08: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140CFAD0D: E8 1E 7D 01 00     call        0000000140D12A30
  0000000140CFAD12: 90                 nop
  0000000140CFAD13: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140CFAD18: E8 C3 9D 30 FF     call        0000000140004AE0
  0000000140CFAD1D: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140CFAD21: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140CFAD25: 83 E8 01           sub         eax,1
  0000000140CFAD28: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000140CFAD2C: EB 0B              jmp         0000000140CFAD39
  0000000140CFAD2E: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140CFAD32: 83 E8 01           sub         eax,1
  0000000140CFAD35: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000140CFAD39: 83 7C 24 3C FF     cmp         dword ptr [rsp+3Ch],0FFFFFFFFh
  0000000140CFAD3E: 7E 41              jle         0000000140CFAD81
  0000000140CFAD40: 8B 54 24 3C        mov         edx,dword ptr [rsp+3Ch]
  0000000140CFAD44: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140CFAD49: E8 C2 5E 46 FF     call        0000000140160C10
  0000000140CFAD4E: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140CFAD51: E8 AA 2E A6 FF     call        000000014075DC00
  0000000140CFAD56: 0F B6 C0           movzx       eax,al
  0000000140CFAD59: 85 C0              test        eax,eax
  0000000140CFAD5B: 74 22              je          0000000140CFAD7F
  0000000140CFAD5D: 8B 54 24 3C        mov         edx,dword ptr [rsp+3Ch]
  0000000140CFAD61: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140CFAD66: E8 A5 5E 46 FF     call        0000000140160C10
  0000000140CFAD6B: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140CFAD6E: E8 8D 74 01 00     call        0000000140D12200
  0000000140CFAD73: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CFAD78: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140CFAD7D: EB 02              jmp         0000000140CFAD81
  0000000140CFAD7F: EB AD              jmp         0000000140CFAD2E
  0000000140CFAD81: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140CFAD86: E8 15 D4 50 FF     call        00000001402081A0
  0000000140CFAD8B: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000140CFAD90: 85 C0              test        eax,eax
  0000000140CFAD92: 75 3C              jne         0000000140CFADD0
  0000000140CFAD94: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140CFAD99: 0F B6 80 C0 01 00  movzx       eax,byte ptr [rax+00000000000001C0h]
                    00
  0000000140CFADA0: 85 C0              test        eax,eax
  0000000140CFADA2: 74 2C              je          0000000140CFADD0
  0000000140CFADA4: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CFADA9: 48 81 C1 B8 01 00  add         rcx,1B8h
                    00
  0000000140CFADB0: E8 3B 01 F2 FF     call        0000000140C1AEF0
  0000000140CFADB5: 48 85 C0           test        rax,rax
  0000000140CFADB8: 74 16              je          0000000140CFADD0
  0000000140CFADBA: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CFADBF: 48 81 C1 B8 01 00  add         rcx,1B8h
                    00
  0000000140CFADC6: E8 25 01 F2 FF     call        0000000140C1AEF0
  0000000140CFADCB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CFADD0: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CFADD5: E8 86 EB FF FF     call        0000000140CF9960
  0000000140CFADDA: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CFADDF: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CFADE4: E8 37 1B 01 00     call        0000000140D0C920
  0000000140CFADE9: 88 44 24 38        mov         byte ptr [rsp+38h],al
  0000000140CFADED: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140CFADF2: 85 C0              test        eax,eax
  0000000140CFADF4: 74 3C              je          0000000140CFAE32
  0000000140CFADF6: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140CFADFC: 74 34              je          0000000140CFAE32
  0000000140CFADFE: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CFAE03: E8 D8 12 F8 FF     call        0000000140C7C0E0
  0000000140CFAE08: 0F B6 C0           movzx       eax,al
  0000000140CFAE0B: 85 C0              test        eax,eax
  0000000140CFAE0D: 74 23              je          0000000140CFAE32
  0000000140CFAE0F: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140CFAE15: 74 1B              je          0000000140CFAE32
  0000000140CFAE17: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140CFAE1C: E8 7F 12 F8 FF     call        0000000140C7C0A0
  0000000140CFAE21: 0F B6 C0           movzx       eax,al
  0000000140CFAE24: 85 C0              test        eax,eax
  0000000140CFAE26: 74 0A              je          0000000140CFAE32
  0000000140CFAE28: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140CFAE2D: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CFAE32: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140CFAE37: 48 83 C4 68        add         rsp,68h
  0000000140CFAE3B: C3                 ret

  Summary

     1E02000 .text

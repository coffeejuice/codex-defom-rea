Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CDEAB0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140CDEAB5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140CDEABA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CDEABF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CDEAC4: 48 83 EC 68        sub         rsp,68h
  0000000140CDEAC8: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140CDEAD1: C7 44 24 4C 00 00  mov         dword ptr [rsp+4Ch],0
                    00 00
  0000000140CDEAD9: C7 44 24 3C 00 00  mov         dword ptr [rsp+3Ch],0
                    00 00
  0000000140CDEAE1: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140CDEAE9: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  0000000140CDEAF1: C7 44 24 34 00 00  mov         dword ptr [rsp+34h],0
                    00 00
  0000000140CDEAF9: C7 44 24 48 01 00  mov         dword ptr [rsp+48h],1
                    00 00
  0000000140CDEB01: 48 8D 15 8C 36 46  lea         rdx,[0000000142142194h]
                    01
  0000000140CDEB08: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CDEB10: E8 C3 11 79 00     call        000000014146FCD8
  0000000140CDEB15: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CDEB1A: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140CDEB20: 75 07              jne         0000000140CDEB29
  0000000140CDEB22: 32 C0              xor         al,al
  0000000140CDEB24: E9 63 07 00 00     jmp         0000000140CDF28C
  0000000140CDEB29: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  0000000140CDEB31: EB 0B              jmp         0000000140CDEB3E
  0000000140CDEB33: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  0000000140CDEB37: 83 C0 01           add         eax,1
  0000000140CDEB3A: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  0000000140CDEB3E: 83 7C 24 50 64     cmp         dword ptr [rsp+50h],64h
  0000000140CDEB43: 0F 8D E2 06 00 00  jge         0000000140CDF22B
  0000000140CDEB49: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEB4E: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEB55: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  0000000140CDEB5D: 4C 8D 4C 24 4C     lea         r9,[rsp+4Ch]
  0000000140CDEB62: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEB68: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEB6D: E8 7E 7D C2 FF     call        00000001409068F0
  0000000140CDEB72: 89 44 24 54        mov         dword ptr [rsp+54h],eax
  0000000140CDEB76: 83 7C 24 4C 00     cmp         dword ptr [rsp+4Ch],0
  0000000140CDEB7B: 7E 07              jle         0000000140CDEB84
  0000000140CDEB7D: 83 7C 24 4C 64     cmp         dword ptr [rsp+4Ch],64h
  0000000140CDEB82: 7E 02              jle         0000000140CDEB86
  0000000140CDEB84: EB AD              jmp         0000000140CDEB33
  0000000140CDEB86: 83 7C 24 54 0D     cmp         dword ptr [rsp+54h],0Dh
  0000000140CDEB8B: 75 0A              jne         0000000140CDEB97
  0000000140CDEB8D: C7 44 24 48 01 00  mov         dword ptr [rsp+48h],1
                    00 00
  0000000140CDEB95: EB 08              jmp         0000000140CDEB9F
  0000000140CDEB97: C7 44 24 48 02 00  mov         dword ptr [rsp+48h],2
                    00 00
  0000000140CDEB9F: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEBA4: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEBAB: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEBB3: 4C 8D 4C 24 3C     lea         r9,[rsp+3Ch]
  0000000140CDEBB8: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEBBE: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEBC3: E8 28 7D C2 FF     call        00000001409068F0
  0000000140CDEBC8: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEBCD: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEBD4: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEBDC: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  0000000140CDEBE1: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEBE7: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEBEC: E8 FF 7C C2 FF     call        00000001409068F0
  0000000140CDEBF1: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEBF6: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEBFD: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEC05: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  0000000140CDEC0A: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEC10: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEC15: E8 D6 7C C2 FF     call        00000001409068F0
  0000000140CDEC1A: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEC1E: 83 E8 01           sub         eax,1
  0000000140CDEC21: 48 63 D0           movsxd      rdx,eax
  0000000140CDEC24: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEC29: 0F B6 44 24 3C     movzx       eax,byte ptr [rsp+3Ch]
  0000000140CDEC2E: 88 84 91 60 07 00  mov         byte ptr [rcx+rdx*4+0000000000000760h],al
                    00
  0000000140CDEC35: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEC39: 83 E8 01           sub         eax,1
  0000000140CDEC3C: 48 63 D0           movsxd      rdx,eax
  0000000140CDEC3F: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEC44: 0F B6 44 24 30     movzx       eax,byte ptr [rsp+30h]
  0000000140CDEC49: 88 84 91 61 07 00  mov         byte ptr [rcx+rdx*4+0000000000000761h],al
                    00
  0000000140CDEC50: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEC54: 83 E8 01           sub         eax,1
  0000000140CDEC57: 48 63 D0           movsxd      rdx,eax
  0000000140CDEC5A: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEC5F: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140CDEC64: 88 84 91 62 07 00  mov         byte ptr [rcx+rdx*4+0000000000000762h],al
                    00
  0000000140CDEC6B: 83 7C 24 48 01     cmp         dword ptr [rsp+48h],1
  0000000140CDEC70: 7E 44              jle         0000000140CDECB6
  0000000140CDEC72: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEC77: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEC7E: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEC86: 4C 8D 4C 24 34     lea         r9,[rsp+34h]
  0000000140CDEC8B: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEC91: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEC96: E8 55 7C C2 FF     call        00000001409068F0
  0000000140CDEC9B: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEC9F: 83 E8 01           sub         eax,1
  0000000140CDECA2: 48 63 D0           movsxd      rdx,eax
  0000000140CDECA5: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDECAA: 0F B6 44 24 34     movzx       eax,byte ptr [rsp+34h]
  0000000140CDECAF: 88 84 91 63 07 00  mov         byte ptr [rcx+rdx*4+0000000000000763h],al
                    00
  0000000140CDECB6: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDECBB: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDECC2: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDECCA: 4C 8D 4C 24 3C     lea         r9,[rsp+3Ch]
  0000000140CDECCF: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDECD5: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDECDA: E8 11 7C C2 FF     call        00000001409068F0
  0000000140CDECDF: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDECE4: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDECEB: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDECF3: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  0000000140CDECF8: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDECFE: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDED03: E8 E8 7B C2 FF     call        00000001409068F0
  0000000140CDED08: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDED0D: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDED14: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDED1C: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  0000000140CDED21: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDED27: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDED2C: E8 BF 7B C2 FF     call        00000001409068F0
  0000000140CDED31: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDED35: 83 E8 01           sub         eax,1
  0000000140CDED38: 48 63 D0           movsxd      rdx,eax
  0000000140CDED3B: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDED40: 0F B6 44 24 3C     movzx       eax,byte ptr [rsp+3Ch]
  0000000140CDED45: 88 84 91 F0 08 00  mov         byte ptr [rcx+rdx*4+00000000000008F0h],al
                    00
  0000000140CDED4C: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDED50: 83 E8 01           sub         eax,1
  0000000140CDED53: 48 63 D0           movsxd      rdx,eax
  0000000140CDED56: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDED5B: 0F B6 44 24 30     movzx       eax,byte ptr [rsp+30h]
  0000000140CDED60: 88 84 91 F1 08 00  mov         byte ptr [rcx+rdx*4+00000000000008F1h],al
                    00
  0000000140CDED67: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDED6B: 83 E8 01           sub         eax,1
  0000000140CDED6E: 48 63 D0           movsxd      rdx,eax
  0000000140CDED71: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDED76: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140CDED7B: 88 84 91 F2 08 00  mov         byte ptr [rcx+rdx*4+00000000000008F2h],al
                    00
  0000000140CDED82: 83 7C 24 48 01     cmp         dword ptr [rsp+48h],1
  0000000140CDED87: 7E 44              jle         0000000140CDEDCD
  0000000140CDED89: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDED8E: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDED95: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDED9D: 4C 8D 4C 24 34     lea         r9,[rsp+34h]
  0000000140CDEDA2: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEDA8: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEDAD: E8 3E 7B C2 FF     call        00000001409068F0
  0000000140CDEDB2: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEDB6: 83 E8 01           sub         eax,1
  0000000140CDEDB9: 48 63 D0           movsxd      rdx,eax
  0000000140CDEDBC: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEDC1: 0F B6 44 24 34     movzx       eax,byte ptr [rsp+34h]
  0000000140CDEDC6: 88 84 91 F3 08 00  mov         byte ptr [rcx+rdx*4+00000000000008F3h],al
                    00
  0000000140CDEDCD: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEDD2: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEDD9: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEDE1: 4C 8D 4C 24 3C     lea         r9,[rsp+3Ch]
  0000000140CDEDE6: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEDEC: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEDF1: E8 FA 7A C2 FF     call        00000001409068F0
  0000000140CDEDF6: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEDFB: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEE02: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEE0A: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  0000000140CDEE0F: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEE15: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEE1A: E8 D1 7A C2 FF     call        00000001409068F0
  0000000140CDEE1F: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEE24: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEE2B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEE33: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  0000000140CDEE38: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEE3E: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEE43: E8 A8 7A C2 FF     call        00000001409068F0
  0000000140CDEE48: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEE4C: 83 E8 01           sub         eax,1
  0000000140CDEE4F: 48 63 D0           movsxd      rdx,eax
  0000000140CDEE52: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEE57: 0F B6 44 24 3C     movzx       eax,byte ptr [rsp+3Ch]
  0000000140CDEE5C: 88 84 91 80 0A 00  mov         byte ptr [rcx+rdx*4+0000000000000A80h],al
                    00
  0000000140CDEE63: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEE67: 83 E8 01           sub         eax,1
  0000000140CDEE6A: 48 63 D0           movsxd      rdx,eax
  0000000140CDEE6D: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEE72: 0F B6 44 24 30     movzx       eax,byte ptr [rsp+30h]
  0000000140CDEE77: 88 84 91 81 0A 00  mov         byte ptr [rcx+rdx*4+0000000000000A81h],al
                    00
  0000000140CDEE7E: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEE82: 83 E8 01           sub         eax,1
  0000000140CDEE85: 48 63 D0           movsxd      rdx,eax
  0000000140CDEE88: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEE8D: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140CDEE92: 88 84 91 82 0A 00  mov         byte ptr [rcx+rdx*4+0000000000000A82h],al
                    00
  0000000140CDEE99: 83 7C 24 48 01     cmp         dword ptr [rsp+48h],1
  0000000140CDEE9E: 7E 44              jle         0000000140CDEEE4
  0000000140CDEEA0: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEEA5: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEEAC: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEEB4: 4C 8D 4C 24 34     lea         r9,[rsp+34h]
  0000000140CDEEB9: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEEBF: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEEC4: E8 27 7A C2 FF     call        00000001409068F0
  0000000140CDEEC9: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEECD: 83 E8 01           sub         eax,1
  0000000140CDEED0: 48 63 D0           movsxd      rdx,eax
  0000000140CDEED3: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEED8: 0F B6 44 24 34     movzx       eax,byte ptr [rsp+34h]
  0000000140CDEEDD: 88 84 91 83 0A 00  mov         byte ptr [rcx+rdx*4+0000000000000A83h],al
                    00
  0000000140CDEEE4: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEEE9: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEEF0: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEEF8: 4C 8D 4C 24 3C     lea         r9,[rsp+3Ch]
  0000000140CDEEFD: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEF03: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEF08: E8 E3 79 C2 FF     call        00000001409068F0
  0000000140CDEF0D: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEF12: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEF19: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEF21: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  0000000140CDEF26: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEF2C: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEF31: E8 BA 79 C2 FF     call        00000001409068F0
  0000000140CDEF36: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEF3B: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEF42: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEF4A: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  0000000140CDEF4F: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEF55: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEF5A: E8 91 79 C2 FF     call        00000001409068F0
  0000000140CDEF5F: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEF63: 83 E8 01           sub         eax,1
  0000000140CDEF66: 48 63 D0           movsxd      rdx,eax
  0000000140CDEF69: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEF6E: 0F B6 44 24 3C     movzx       eax,byte ptr [rsp+3Ch]
  0000000140CDEF73: 88 84 91 10 0C 00  mov         byte ptr [rcx+rdx*4+0000000000000C10h],al
                    00
  0000000140CDEF7A: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEF7E: 83 E8 01           sub         eax,1
  0000000140CDEF81: 48 63 D0           movsxd      rdx,eax
  0000000140CDEF84: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEF89: 0F B6 44 24 30     movzx       eax,byte ptr [rsp+30h]
  0000000140CDEF8E: 88 84 91 11 0C 00  mov         byte ptr [rcx+rdx*4+0000000000000C11h],al
                    00
  0000000140CDEF95: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEF99: 83 E8 01           sub         eax,1
  0000000140CDEF9C: 48 63 D0           movsxd      rdx,eax
  0000000140CDEF9F: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEFA4: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140CDEFA9: 88 84 91 12 0C 00  mov         byte ptr [rcx+rdx*4+0000000000000C12h],al
                    00
  0000000140CDEFB0: 83 7C 24 48 01     cmp         dword ptr [rsp+48h],1
  0000000140CDEFB5: 7E 44              jle         0000000140CDEFFB
  0000000140CDEFB7: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEFBC: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDEFC3: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDEFCB: 4C 8D 4C 24 34     lea         r9,[rsp+34h]
  0000000140CDEFD0: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDEFD6: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDEFDB: E8 10 79 C2 FF     call        00000001409068F0
  0000000140CDEFE0: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDEFE4: 83 E8 01           sub         eax,1
  0000000140CDEFE7: 48 63 D0           movsxd      rdx,eax
  0000000140CDEFEA: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDEFEF: 0F B6 44 24 34     movzx       eax,byte ptr [rsp+34h]
  0000000140CDEFF4: 88 84 91 13 0C 00  mov         byte ptr [rcx+rdx*4+0000000000000C13h],al
                    00
  0000000140CDEFFB: 83 7C 24 48 01     cmp         dword ptr [rsp+48h],1
  0000000140CDF000: 0F 8E 20 02 00 00  jle         0000000140CDF226
  0000000140CDF006: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF00B: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDF012: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDF01A: 4C 8D 4C 24 3C     lea         r9,[rsp+3Ch]
  0000000140CDF01F: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDF025: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDF02A: E8 C1 78 C2 FF     call        00000001409068F0
  0000000140CDF02F: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF034: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDF03B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDF043: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  0000000140CDF048: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDF04E: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDF053: E8 98 78 C2 FF     call        00000001409068F0
  0000000140CDF058: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF05D: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDF064: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDF06C: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  0000000140CDF071: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDF077: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDF07C: E8 6F 78 C2 FF     call        00000001409068F0
  0000000140CDF081: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF086: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDF08D: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDF095: 4C 8D 4C 24 34     lea         r9,[rsp+34h]
  0000000140CDF09A: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDF0A0: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDF0A5: E8 46 78 C2 FF     call        00000001409068F0
  0000000140CDF0AA: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDF0AE: 83 E8 01           sub         eax,1
  0000000140CDF0B1: 48 63 D0           movsxd      rdx,eax
  0000000140CDF0B4: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF0B9: 0F B6 44 24 3C     movzx       eax,byte ptr [rsp+3Ch]
  0000000140CDF0BE: 88 84 91 A0 0D 00  mov         byte ptr [rcx+rdx*4+0000000000000DA0h],al
                    00
  0000000140CDF0C5: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDF0C9: 83 E8 01           sub         eax,1
  0000000140CDF0CC: 48 63 D0           movsxd      rdx,eax
  0000000140CDF0CF: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF0D4: 0F B6 44 24 30     movzx       eax,byte ptr [rsp+30h]
  0000000140CDF0D9: 88 84 91 A1 0D 00  mov         byte ptr [rcx+rdx*4+0000000000000DA1h],al
                    00
  0000000140CDF0E0: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDF0E4: 83 E8 01           sub         eax,1
  0000000140CDF0E7: 48 63 D0           movsxd      rdx,eax
  0000000140CDF0EA: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF0EF: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140CDF0F4: 88 84 91 A2 0D 00  mov         byte ptr [rcx+rdx*4+0000000000000DA2h],al
                    00
  0000000140CDF0FB: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDF0FF: 83 E8 01           sub         eax,1
  0000000140CDF102: 48 63 D0           movsxd      rdx,eax
  0000000140CDF105: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF10A: 0F B6 44 24 34     movzx       eax,byte ptr [rsp+34h]
  0000000140CDF10F: 88 84 91 A3 0D 00  mov         byte ptr [rcx+rdx*4+0000000000000DA3h],al
                    00
  0000000140CDF116: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF11B: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDF122: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDF12A: 4C 8D 4C 24 3C     lea         r9,[rsp+3Ch]
  0000000140CDF12F: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDF135: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDF13A: E8 B1 77 C2 FF     call        00000001409068F0
  0000000140CDF13F: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF144: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDF14B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDF153: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  0000000140CDF158: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDF15E: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDF163: E8 88 77 C2 FF     call        00000001409068F0
  0000000140CDF168: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF16D: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDF174: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDF17C: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  0000000140CDF181: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDF187: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDF18C: E8 5F 77 C2 FF     call        00000001409068F0
  0000000140CDF191: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF196: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDF19D: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDF1A5: 4C 8D 4C 24 34     lea         r9,[rsp+34h]
  0000000140CDF1AA: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDF1B0: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDF1B5: E8 36 77 C2 FF     call        00000001409068F0
  0000000140CDF1BA: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDF1BE: 83 E8 01           sub         eax,1
  0000000140CDF1C1: 48 63 D0           movsxd      rdx,eax
  0000000140CDF1C4: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF1C9: 0F B6 44 24 3C     movzx       eax,byte ptr [rsp+3Ch]
  0000000140CDF1CE: 88 84 91 30 0F 00  mov         byte ptr [rcx+rdx*4+0000000000000F30h],al
                    00
  0000000140CDF1D5: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDF1D9: 83 E8 01           sub         eax,1
  0000000140CDF1DC: 48 63 D0           movsxd      rdx,eax
  0000000140CDF1DF: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF1E4: 0F B6 44 24 30     movzx       eax,byte ptr [rsp+30h]
  0000000140CDF1E9: 88 84 91 31 0F 00  mov         byte ptr [rcx+rdx*4+0000000000000F31h],al
                    00
  0000000140CDF1F0: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDF1F4: 83 E8 01           sub         eax,1
  0000000140CDF1F7: 48 63 D0           movsxd      rdx,eax
  0000000140CDF1FA: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF1FF: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140CDF204: 88 84 91 32 0F 00  mov         byte ptr [rcx+rdx*4+0000000000000F32h],al
                    00
  0000000140CDF20B: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000140CDF20F: 83 E8 01           sub         eax,1
  0000000140CDF212: 48 63 D0           movsxd      rdx,eax
  0000000140CDF215: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF21A: 0F B6 44 24 34     movzx       eax,byte ptr [rsp+34h]
  0000000140CDF21F: 88 84 91 33 0F 00  mov         byte ptr [rcx+rdx*4+0000000000000F33h],al
                    00
  0000000140CDF226: E9 08 F9 FF FF     jmp         0000000140CDEB33
  0000000140CDF22B: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF230: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDF237: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  0000000140CDF23F: 4C 8B 4C 24 78     mov         r9,qword ptr [rsp+78h]
  0000000140CDF244: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDF24A: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDF24F: E8 9C 76 C2 FF     call        00000001409068F0
  0000000140CDF254: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CDF259: 48 81 C1 5C 07 00  add         rcx,75Ch
                    00
  0000000140CDF260: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CDF268: 4C 8B 8C 24 80 00  mov         r9,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CDF270: 41 B8 05 00 00 00  mov         r8d,5
  0000000140CDF276: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  0000000140CDF27B: E8 70 76 C2 FF     call        00000001409068F0
  0000000140CDF280: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140CDF285: E8 9E 08 79 00     call        000000014146FB28
  0000000140CDF28A: B0 01              mov         al,1
  0000000140CDF28C: 48 83 C4 68        add         rsp,68h
  0000000140CDF290: C3                 ret

  Summary

     1E02000 .text

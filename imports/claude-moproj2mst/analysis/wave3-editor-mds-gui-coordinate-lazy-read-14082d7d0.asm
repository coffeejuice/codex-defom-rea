Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014082D7D0: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  000000014082D7D5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014082D7DA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014082D7DF: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  000000014082D7E6: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D7EE: E8 8D 31 8D FF     call        0000000140100980
  000000014082D7F3: 89 44 24 70        mov         dword ptr [rsp+70h],eax
  000000014082D7F7: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000014082D7FC: E8 DF 37 FF FF     call        0000000140820FE0
  000000014082D801: C7 44 24 54 00 00  mov         dword ptr [rsp+54h],0
                    00 00
  000000014082D809: 48 63 0D D8 02 59  movsxd      rcx,dword ptr [0000000143DBDAE8h]
                    03
  000000014082D810: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D818: 8B 84 88 90 1B 00  mov         eax,dword ptr [rax+rcx*4+0000000000001B90h]
                    00
  000000014082D81F: 83 E8 01           sub         eax,1
  000000014082D822: 48 63 C8           movsxd      rcx,eax
  000000014082D825: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D82D: 48 8B 80 88 1B 00  mov         rax,qword ptr [rax+0000000000001B88h]
                    00
  000000014082D834: 48 8D 04 88        lea         rax,[rax+rcx*4]
  000000014082D838: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014082D83D: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082D845: 83 38 00           cmp         dword ptr [rax],0
  000000014082D848: 7C 12              jl          000000014082D85C
  000000014082D84A: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082D852: 8B 05 58 05 59 03  mov         eax,dword ptr [0000000143DBDDB0h]
  000000014082D858: 39 01              cmp         dword ptr [rcx],eax
  000000014082D85A: 7C 07              jl          000000014082D863
  000000014082D85C: 32 C0              xor         al,al
  000000014082D85E: E9 20 04 00 00     jmp         000000014082DC83
  000000014082D863: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082D86B: 48 63 08           movsxd      rcx,dword ptr [rax]
  000000014082D86E: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D876: 83 BC 88 C4 26 00  cmp         dword ptr [rax+rcx*4+00000000000026C4h],0
                    00 00
  000000014082D87E: 74 07              je          000000014082D887
  000000014082D880: 32 C0              xor         al,al
  000000014082D882: E9 FC 03 00 00     jmp         000000014082DC83
  000000014082D887: 48 8D 15 DE 95 F7  lea         rdx,[00000001427A6E6Ch]
                    01
  000000014082D88E: 48 8D 0D 6B 07 59  lea         rcx,[0000000143DBE000h]
                    03
  000000014082D895: E8 A6 4A 00 00     call        0000000140832340
  000000014082D89A: 45 33 C0           xor         r8d,r8d
  000000014082D89D: 48 8D 15 D4 95 F7  lea         rdx,[00000001427A6E78h]
                    01
  000000014082D8A4: 48 8D 0D 55 07 59  lea         rcx,[0000000143DBE000h]
                    03
  000000014082D8AB: E8 C0 4A 00 00     call        0000000140832370
  000000014082D8B0: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082D8B8: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D8C0: E8 9B A7 FF FF     call        0000000140828060
  000000014082D8C5: 45 33 C0           xor         r8d,r8d
  000000014082D8C8: 48 8D 15 C1 95 F7  lea         rdx,[00000001427A6E90h]
                    01
  000000014082D8CF: 48 8D 0D 0A 02 5A  lea         rcx,[0000000143DCDAE0h]
                    03
  000000014082D8D6: E8 95 4A 00 00     call        0000000140832370
  000000014082D8DB: C6 44 24 50 00     mov         byte ptr [rsp+50h],0
  000000014082D8E0: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082D8E8: 8B 05 2E 03 59 03  mov         eax,dword ptr [0000000143DBDC1Ch]
  000000014082D8EE: 39 01              cmp         dword ptr [rcx],eax
  000000014082D8F0: 75 31              jne         000000014082D923
  000000014082D8F2: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D8FA: 83 B8 60 16 00 00  cmp         dword ptr [rax+0000000000001660h],0
                    00
  000000014082D901: 7E 0A              jle         000000014082D90D
  000000014082D903: C7 44 24 78 01 00  mov         dword ptr [rsp+78h],1
                    00 00
  000000014082D90B: EB 08              jmp         000000014082D915
  000000014082D90D: C7 44 24 78 00 00  mov         dword ptr [rsp+78h],0
                    00 00
  000000014082D915: 0F B6 44 24 78     movzx       eax,byte ptr [rsp+78h]
  000000014082D91A: 88 44 24 50        mov         byte ptr [rsp+50h],al
  000000014082D91E: E9 87 00 00 00     jmp         000000014082D9AA
  000000014082D923: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082D92B: 8B 05 FF 02 59 03  mov         eax,dword ptr [0000000143DBDC30h]
  000000014082D931: 39 01              cmp         dword ptr [rcx],eax
  000000014082D933: 75 2E              jne         000000014082D963
  000000014082D935: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D93D: 83 B8 64 16 00 00  cmp         dword ptr [rax+0000000000001664h],0
                    00
  000000014082D944: 7E 0A              jle         000000014082D950
  000000014082D946: C7 44 24 7C 01 00  mov         dword ptr [rsp+7Ch],1
                    00 00
  000000014082D94E: EB 08              jmp         000000014082D958
  000000014082D950: C7 44 24 7C 00 00  mov         dword ptr [rsp+7Ch],0
                    00 00
  000000014082D958: 0F B6 44 24 7C     movzx       eax,byte ptr [rsp+7Ch]
  000000014082D95D: 88 44 24 50        mov         byte ptr [rsp+50h],al
  000000014082D961: EB 47              jmp         000000014082D9AA
  000000014082D963: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082D96B: 8B 05 CB 02 59 03  mov         eax,dword ptr [0000000143DBDC3Ch]
  000000014082D971: 39 01              cmp         dword ptr [rcx],eax
  000000014082D973: 75 35              jne         000000014082D9AA
  000000014082D975: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D97D: 83 B8 68 16 00 00  cmp         dword ptr [rax+0000000000001668h],0
                    00
  000000014082D984: 7E 0D              jle         000000014082D993
  000000014082D986: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],1
                    00 01 00 00 00
  000000014082D991: EB 0B              jmp         000000014082D99E
  000000014082D993: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00
  000000014082D99E: 0F B6 84 24 80 00  movzx       eax,byte ptr [rsp+0000000000000080h]
                    00 00
  000000014082D9A6: 88 44 24 50        mov         byte ptr [rsp+50h],al
  000000014082D9AA: C7 44 24 54 00 00  mov         dword ptr [rsp+54h],0
                    00 00
  000000014082D9B2: EB 0B              jmp         000000014082D9BF
  000000014082D9B4: 8B 44 24 54        mov         eax,dword ptr [rsp+54h]
  000000014082D9B8: 83 C0 01           add         eax,1
  000000014082D9BB: 89 44 24 54        mov         dword ptr [rsp+54h],eax
  000000014082D9BF: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D9C7: 8B 80 78 13 00 00  mov         eax,dword ptr [rax+0000000000001378h]
  000000014082D9CD: 39 44 24 54        cmp         dword ptr [rsp+54h],eax
  000000014082D9D1: 0F 8D 94 02 00 00  jge         000000014082DC6B
  000000014082D9D7: 8B 54 24 54        mov         edx,dword ptr [rsp+54h]
  000000014082D9DB: 83 C2 01           add         edx,1
  000000014082D9DE: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D9E6: E8 B5 28 00 00     call        00000001408302A0
  000000014082D9EB: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014082D9F0: 48 63 0D 51 01 59  movsxd      rcx,dword ptr [0000000143DBDB48h]
                    03
  000000014082D9F7: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082D9FF: 8B 84 88 90 1B 00  mov         eax,dword ptr [rax+rcx*4+0000000000001B90h]
                    00
  000000014082DA06: 0F AF 44 24 54     imul        eax,dword ptr [rsp+54h]
  000000014082DA0B: 48 63 C8           movsxd      rcx,eax
  000000014082DA0E: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000014082DA13: 48 8D 04 88        lea         rax,[rax+rcx*4]
  000000014082DA17: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014082DA1C: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  000000014082DA22: 74 0B              je          000000014082DA2F
  000000014082DA24: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000014082DA29: 83 78 0C 07        cmp         dword ptr [rax+0Ch],7
  000000014082DA2D: 75 02              jne         000000014082DA31
  000000014082DA2F: EB 83              jmp         000000014082D9B4
  000000014082DA31: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014082DA36: E8 B5 76 00 00     call        00000001408350F0
  000000014082DA3B: 0F B6 C0           movzx       eax,al
  000000014082DA3E: 85 C0              test        eax,eax
  000000014082DA40: 74 05              je          000000014082DA47
  000000014082DA42: E9 6D FF FF FF     jmp         000000014082D9B4
  000000014082DA47: 0F B6 44 24 50     movzx       eax,byte ptr [rsp+50h]
  000000014082DA4C: 85 C0              test        eax,eax
  000000014082DA4E: 0F 84 01 01 00 00  je          000000014082DB55
  000000014082DA54: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DA5C: 48 81 C2 F8 15 00  add         rdx,15F8h
                    00
  000000014082DA63: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DA6B: 48 81 C1 C0 1D 00  add         rcx,1DC0h
                    00
  000000014082DA72: 4C 8B 8C 24 A0 00  mov         r9,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DA7A: 49 81 C1 90 1B 00  add         r9,1B90h
                    00
  000000014082DA81: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  000000014082DA89: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082DA91: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014082DA96: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DA9E: 8B 80 E8 13 00 00  mov         eax,dword ptr [rax+00000000000013E8h]
  000000014082DAA4: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  000000014082DAA8: 48 89 54 24 28     mov         qword ptr [rsp+28h],rdx
  000000014082DAAD: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  000000014082DAB2: 4C 8B 84 24 A0 00  mov         r8,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DABA: 4D 8B 80 78 1B 00  mov         r8,qword ptr [r8+0000000000001B78h]
                    00
  000000014082DAC1: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DAC9: 48 8B 92 88 1B 00  mov         rdx,qword ptr [rdx+0000000000001B88h]
                    00
  000000014082DAD0: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014082DAD5: E8 F6 46 01 00     call        00000001408421D0
  000000014082DADA: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DAE2: 48 81 C2 F8 15 00  add         rdx,15F8h
                    00
  000000014082DAE9: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DAF1: 48 81 C1 C0 1D 00  add         rcx,1DC0h
                    00
  000000014082DAF8: 4C 8B 8C 24 A0 00  mov         r9,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DB00: 49 81 C1 90 1B 00  add         r9,1B90h
                    00
  000000014082DB07: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082DB0F: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014082DB14: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  000000014082DB19: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014082DB1E: 48 89 54 24 28     mov         qword ptr [rsp+28h],rdx
  000000014082DB23: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  000000014082DB28: 4C 8B 84 24 A0 00  mov         r8,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DB30: 4D 8B 80 78 1B 00  mov         r8,qword ptr [r8+0000000000001B78h]
                    00
  000000014082DB37: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DB3F: 48 8B 92 88 1B 00  mov         rdx,qword ptr [rdx+0000000000001B88h]
                    00
  000000014082DB46: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014082DB4B: E8 A0 75 01 00     call        00000001408450F0
  000000014082DB50: E9 11 01 00 00     jmp         000000014082DC66
  000000014082DB55: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014082DB5D: 0F B6 00           movzx       eax,byte ptr [rax]
  000000014082DB60: 85 C0              test        eax,eax
  000000014082DB62: 0F 84 88 00 00 00  je          000000014082DBF0
  000000014082DB68: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DB70: 48 81 C2 F8 15 00  add         rdx,15F8h
                    00
  000000014082DB77: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DB7F: 48 81 C1 C0 1D 00  add         rcx,1DC0h
                    00
  000000014082DB86: 4C 8B 8C 24 A0 00  mov         r9,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DB8E: 49 81 C1 90 1B 00  add         r9,1B90h
                    00
  000000014082DB95: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  000000014082DB9D: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082DBA5: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014082DBAA: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DBB2: 8B 80 E8 13 00 00  mov         eax,dword ptr [rax+00000000000013E8h]
  000000014082DBB8: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  000000014082DBBC: 48 89 54 24 28     mov         qword ptr [rsp+28h],rdx
  000000014082DBC1: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  000000014082DBC6: 4C 8B 84 24 A0 00  mov         r8,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DBCE: 4D 8B 80 78 1B 00  mov         r8,qword ptr [r8+0000000000001B78h]
                    00
  000000014082DBD5: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DBDD: 48 8B 92 88 1B 00  mov         rdx,qword ptr [rdx+0000000000001B88h]
                    00
  000000014082DBE4: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014082DBE9: E8 E2 45 01 00     call        00000001408421D0
  000000014082DBEE: EB 76              jmp         000000014082DC66
  000000014082DBF0: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DBF8: 48 81 C2 F8 15 00  add         rdx,15F8h
                    00
  000000014082DBFF: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DC07: 48 81 C1 C0 1D 00  add         rcx,1DC0h
                    00
  000000014082DC0E: 4C 8B 8C 24 A0 00  mov         r9,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DC16: 49 81 C1 90 1B 00  add         r9,1B90h
                    00
  000000014082DC1D: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014082DC25: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014082DC2A: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  000000014082DC2F: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014082DC34: 48 89 54 24 28     mov         qword ptr [rsp+28h],rdx
  000000014082DC39: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  000000014082DC3E: 4C 8B 84 24 A0 00  mov         r8,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DC46: 4D 8B 80 78 1B 00  mov         r8,qword ptr [r8+0000000000001B78h]
                    00
  000000014082DC4D: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014082DC55: 48 8B 92 88 1B 00  mov         rdx,qword ptr [rdx+0000000000001B88h]
                    00
  000000014082DC5C: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014082DC61: E8 8A 74 01 00     call        00000001408450F0
  000000014082DC66: E9 49 FD FF FF     jmp         000000014082D9B4
  000000014082DC6B: C6 44 24 74 01     mov         byte ptr [rsp+74h],1
  000000014082DC70: 48 8D 54 24 74     lea         rdx,[rsp+74h]
  000000014082DC75: 48 8D 0D 84 03 59  lea         rcx,[0000000143DBE000h]
                    03
  000000014082DC7C: E8 AF 47 00 00     call        0000000140832430
  000000014082DC81: B0 01              mov         al,1
  000000014082DC83: 48 81 C4 98 00 00  add         rsp,98h
                    00
  000000014082DC8A: C3                 ret

  Summary

     1E02000 .text

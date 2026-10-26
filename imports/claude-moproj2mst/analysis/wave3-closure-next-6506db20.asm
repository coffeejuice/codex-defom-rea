Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006506DB20: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006506DB25: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000006506DB29: 57                 push        rdi
  000000006506DB2A: 48 83 EC 20        sub         rsp,20h
  000000006506DB2E: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006506DB32: 41 B0 01           mov         r8b,1
  000000006506DB35: BA 44 00 00 00     mov         edx,44h
  000000006506DB3A: 48 8B F9           mov         rdi,rcx
  000000006506DB3D: E8 9E B4 FF FF     call        0000000065068FE0
  000000006506DB42: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000006506DB46: 3B 83 A0 01 00 00  cmp         eax,dword ptr [rbx+00000000000001A0h]
  000000006506DB4C: 74 6C              je          000000006506DBBA
  000000006506DB4E: 48 8B 8B 88 00 00  mov         rcx,qword ptr [rbx+0000000000000088h]
                    00
  000000006506DB55: 89 83 A0 01 00 00  mov         dword ptr [rbx+00000000000001A0h],eax
  000000006506DB5B: 48 85 C9           test        rcx,rcx
  000000006506DB5E: 74 17              je          000000006506DB77
  000000006506DB60: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  000000006506DB64: 48 85 C9           test        rcx,rcx
  000000006506DB67: 74 0E              je          000000006506DB77
  000000006506DB69: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006506DB6E: 48 83 C1 20        add         rcx,20h
  000000006506DB72: E8 D9 04 4F 00     call        000000006555E050
  000000006506DB77: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006506DB7B: 33 D2              xor         edx,edx
  000000006506DB7D: E8 DE 48 FF FF     call        0000000065062460
  000000006506DB82: 4C 8B 5F 20        mov         r11,qword ptr [rdi+20h]
  000000006506DB86: 41 F6 43 0C 01     test        byte ptr [r11+0Ch],1
  000000006506DB8B: 74 2D              je          000000006506DBBA
  000000006506DB8D: 48 8B 83 88 00 00  mov         rax,qword ptr [rbx+0000000000000088h]
                    00
  000000006506DB94: 48 85 C0           test        rax,rax
  000000006506DB97: 74 21              je          000000006506DBBA
  000000006506DB99: 48 83 78 08 00     cmp         qword ptr [rax+8],0
  000000006506DB9E: 74 1A              je          000000006506DBBA
  000000006506DBA0: 48 8B CB           mov         rcx,rbx
  000000006506DBA3: E8 D8 7E FF FF     call        0000000065065A80
  000000006506DBA8: 4C 8B 9B 88 00 00  mov         r11,qword ptr [rbx+0000000000000088h]
                    00
  000000006506DBAF: 49 8B 43 08        mov         rax,qword ptr [r11+8]
  000000006506DBB3: 81 60 7C FF FD FF  and         dword ptr [rax+7Ch],0FFFFFDFFh
                    FF
  000000006506DBBA: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006506DBBF: 48 83 C4 20        add         rsp,20h
  000000006506DBC3: 5F                 pop         rdi
  000000006506DBC4: C3                 ret

  Summary

      695000 .text

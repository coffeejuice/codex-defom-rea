Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714BB40: 40 53              push        rbx
  000000006714BB42: 48 83 EC 30        sub         rsp,30h
  000000006714BB46: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006714BB4F: 48 8B 05 2A 89 1A  mov         rax,qword ptr [00000000672F4480h]
                    00
  000000006714BB56: 48 85 C0           test        rax,rax
  000000006714BB59: 0F 85 AE 00 00 00  jne         000000006714BC0D
  000000006714BB5F: 38 05 23 89 1A 00  cmp         byte ptr [00000000672F4488h],al
  000000006714BB65: 0F 85 99 00 00 00  jne         000000006714BC04
  000000006714BB6B: 8D 48 08           lea         ecx,[rax+8]
  000000006714BB6E: E8 13 CF 02 00     call        0000000067178A86
  000000006714BB73: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006714BB78: 48 85 C0           test        rax,rax
  000000006714BB7B: 74 12              je          000000006714BB8F
  000000006714BB7D: BA 01 00 00 00     mov         edx,1
  000000006714BB82: 48 8B C8           mov         rcx,rax
  000000006714BB85: E8 36 9E ED FF     call        00000000670259C0
  000000006714BB8A: 48 8B D8           mov         rbx,rax
  000000006714BB8D: EB 02              jmp         000000006714BB91
  000000006714BB8F: 33 DB              xor         ebx,ebx
  000000006714BB91: 33 C0              xor         eax,eax
  000000006714BB93: F0 48 0F B1 1D E4  lock cmpxchg qword ptr [00000000672F4480h],rbx
                    88 1A 00
  000000006714BB9C: 74 27              je          000000006714BBC5
  000000006714BB9E: 48 85 DB           test        rbx,rbx
  000000006714BBA1: 74 19              je          000000006714BBBC
  000000006714BBA3: 48 8B CB           mov         rcx,rbx
  000000006714BBA6: E8 D5 9F ED FF     call        0000000067025B80
  000000006714BBAB: 48 8B CB           mov         rcx,rbx
  000000006714BBAE: E8 A3 CD 02 00     call        0000000067178956
  000000006714BBB3: 48 8B 05 C6 88 1A  mov         rax,qword ptr [00000000672F4480h]
                    00
  000000006714BBBA: EB 58              jmp         000000006714BC14
  000000006714BBBC: 48 8B 05 BD 88 1A  mov         rax,qword ptr [00000000672F4480h]
                    00
  000000006714BBC3: EB 4F              jmp         000000006714BC14
  000000006714BBC5: 8B 05 AD 88 1A 00  mov         eax,dword ptr [00000000672F4478h]
  000000006714BBCB: A8 01              test        al,1
  000000006714BBCD: 75 2C              jne         000000006714BBFB
  000000006714BBCF: 83 C8 01           or          eax,1
  000000006714BBD2: 89 05 A0 88 1A 00  mov         dword ptr [00000000672F4478h],eax
  000000006714BBD8: 48 8D 05 A1 88 1A  lea         rax,[00000000672F4480h]
                    00
  000000006714BBDF: 48 89 05 8A 88 1A  mov         qword ptr [00000000672F4470h],rax
                    00
  000000006714BBE6: 48 8D 0D 63 B4 05  lea         rcx,[00000000671A7050h]
                    00
  000000006714BBED: E8 62 CF 02 00     call        0000000067178B54
  000000006714BBF2: 48 8B 05 87 88 1A  mov         rax,qword ptr [00000000672F4480h]
                    00
  000000006714BBF9: EB 19              jmp         000000006714BC14
  000000006714BBFB: 48 8B 05 7E 88 1A  mov         rax,qword ptr [00000000672F4480h]
                    00
  000000006714BC02: EB 10              jmp         000000006714BC14
  000000006714BC04: 48 8B 05 75 88 1A  mov         rax,qword ptr [00000000672F4480h]
                    00
  000000006714BC0B: EB 07              jmp         000000006714BC14
  000000006714BC0D: 48 8B 05 6C 88 1A  mov         rax,qword ptr [00000000672F4480h]
                    00
  000000006714BC14: 48 83 C4 30        add         rsp,30h
  000000006714BC18: 5B                 pop         rbx
  000000006714BC19: C3                 ret
  000000006714BC1A: CC                 int         3
  000000006714BC1B: CC                 int         3
  000000006714BC1C: CC                 int         3
  000000006714BC1D: CC                 int         3
  000000006714BC1E: CC                 int         3
  000000006714BC1F: CC                 int         3
  000000006714BC20: 40

  Summary

      1A7000 .text

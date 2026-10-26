Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670FF860: 40 57              push        rdi
  00000000670FF862: 48 83 EC 30        sub         rsp,30h
  00000000670FF866: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670FF86F: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000670FF874: 48 8B FA           mov         rdi,rdx
  00000000670FF877: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000670FF87C: E8 9F 02 FF FF     call        00000000670EFB20
  00000000670FF881: 90                 nop
  00000000670FF882: 48 8B C8           mov         rcx,rax
  00000000670FF885: E8 C6 11 F7 FF     call        0000000067070A50
  00000000670FF88A: 48 8B C8           mov         rcx,rax
  00000000670FF88D: FF 15 CD 89 0A 00  call        qword ptr [00000000671A8260h]
  00000000670FF893: 85 C0              test        eax,eax
  00000000670FF895: 0F 95 C3           setne       bl
  00000000670FF898: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000000670FF89D: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FF8A1: 75 0A              jne         00000000670FF8AD
  00000000670FF8A3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000670FF8A8: E8 23 08 F7 FF     call        00000000670700D0
  00000000670FF8AD: 84 DB              test        bl,bl
  00000000670FF8AF: 75 1E              jne         00000000670FF8CF
  00000000670FF8B1: FF 15 29 8B 0A 00  call        qword ptr [00000000671A83E0h]
  00000000670FF8B7: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  00000000670FF8BB: C7 44 24 54 02 00  mov         dword ptr [rsp+54h],2
                    00 00
  00000000670FF8C3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000670FF8C8: 48 89 0F           mov         qword ptr [rdi],rcx
  00000000670FF8CB: 8A C3              mov         al,bl
  00000000670FF8CD: EB 02              jmp         00000000670FF8D1
  00000000670FF8CF: 8A C3              mov         al,bl
  00000000670FF8D1: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000670FF8D6: 48 83 C4 30        add         rsp,30h
  00000000670FF8DA: 5F                 pop         rdi
  00000000670FF8DB: C3                 ret
  00000000670FF8DC: CC                 int         3
  00000000670FF8DD: CC                 int         3
  00000000670FF8DE: CC                 int         3
  00000000670FF8DF: CC                 int         3
  00000000670FF8E0: 40 57              push        rdi
  00000000670FF8E2: 48 83 EC 30        sub         rsp,30h
  00000000670FF8E6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670FF8EF: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000670FF8F4: 49 8B F8           mov         rdi,r8
  00000000670FF8F7: 33 DB              xor         ebx,ebx
  00000000670FF8F9: 44 8B 0A           mov         r9d,dword ptr [rdx]
  00000000670FF8FC: 41 8B C1           mov         eax,r9d
  00000000670FF8FF: 25

  Summary

      1A7000 .text

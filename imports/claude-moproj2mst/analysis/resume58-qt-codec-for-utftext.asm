Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714E100: 40 53              push        rbx
  000000006714E102: 48 83 EC 20        sub         rsp,20h
  000000006714E106: 48 8B D9           mov         rbx,rcx
  000000006714E109: B9 04 00 00 00     mov         ecx,4
  000000006714E10E: E8 BD EF FF FF     call        000000006714D0D0
  000000006714E113: 48 8B CB           mov         rcx,rbx
  000000006714E116: 48 8B D0           mov         rdx,rax
  000000006714E119: 48 83 C4 20        add         rsp,20h
  000000006714E11D: 5B                 pop         rbx
  000000006714E11E: E9 2D FF FF FF     jmp         000000006714E050
  000000006714E123: CC                 int         3
  000000006714E124: CC                 int         3
  000000006714E125: CC                 int         3
  000000006714E126: CC                 int         3
  000000006714E127: CC                 int         3
  000000006714E128: CC                 int         3
  000000006714E129: CC                 int         3
  000000006714E12A: CC                 int         3
  000000006714E12B: CC                 int         3
  000000006714E12C: CC                 int         3
  000000006714E12D: CC                 int         3
  000000006714E12E: CC                 int         3
  000000006714E12F: CC                 int         3
  000000006714E130: 40 55              push        rbp
  000000006714E132: 56                 push        rsi
  000000006714E133: 57                 push        rdi
  000000006714E134: 48 83 EC 30        sub         rsp,30h
  000000006714E138: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006714E141: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  000000006714E146: 48 8B EA           mov         rbp,rdx
  000000006714E149: 48 8B F9           mov         rdi,rcx
  000000006714E14C: 33 D2              xor         edx,edx
  000000006714E14E: E8 FD FE FF FF     call        000000006714E050
  000000006714E153: 48 8B D8           mov         rbx,rax
  000000006714E156: 48 85 C0           test        rax,rax
  000000006714E159: 0F 85 F3 00 00 00  jne         000000006714E252
  000000006714E15F: 41 B8 00 02 00 00  mov         r8d,200h
  000000006714E165: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000006714E16A: 48 8B CF           mov         rcx,rdi
  000000006714E16D: E8 9E FD ED FF     call        000000006702DF10
  000000006714E172: 90                 nop
  000000006714E173: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  000000006714E178: 48 8B C8           mov         rcx,rax
  000000006714E17B: E8 40 13 EE FF     call        000000006702F4C0
  000000006714E180: 90                 nop
  000000006714E181: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714E186: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  000000006714E18A: 75 0A              jne         000000006714E196
  000000006714E18C: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006714E191: E8 1A 22 ED FF     call        00000000670203B0
  000000006714E196: 45 33 C0           xor         r8d,r8d
  000000006714E199: 48 8D 15 D8 7B 0F  lea         rdx,[0000000067245D78h]
                    00
  000000006714E1A0: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006714E1A5: E8 A6 10 EE FF     call        000000006702F250
  000000006714E1AA: 83 F8 FF           cmp         eax,0FFFFFFFFh
  000000006714E1AD: 0F 84 80 00 00 00  je          000000006714E233
  000000006714E1B3: 44 8B C0           mov         r8d,eax
  000000006714E1B6: 48 8D 15 AF 7B 0F  lea         rdx,[0000000067245D6Ch]
                    00
  000000006714E1BD: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006714E1C2: E8 99 FC ED FF     call        000000006702DE60
  000000006714E1C7: 83 F8 FF           cmp         eax,0FFFFFFFFh
  000000006714E1CA: 74 67              je          000000006714E233
  000000006714E1CC: 44 8B C0           mov         r8d,eax
  000000006714E1CF: 48 8D 15 8A 7B 0F  lea         rdx,[0000000067245D60h]
                    00
  000000006714E1D6: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006714E1DB: E8 70 10 EE FF     call        000000006702F250
  000000006714E1E0: 8D 70 08           lea         esi,[rax+8]
  000000006714E1E3: 83 FE FF           cmp         esi,0FFFFFFFFh
  000000006714E1E6: 74 4B              je          000000006714E233
  000000006714E1E8: 44 8D 46 01        lea         r8d,[rsi+1]
  000000006714E1EC: B2 22              mov         dl,22h
  000000006714E1EE: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006714E1F3: E8 68 FB ED FF     call        000000006702DD60
  000000006714E1F8: 2B C6              sub         eax,esi
  000000006714E1FA: 44 8B C8           mov         r9d,eax
  000000006714E1FD: 44 8B C6           mov         r8d,esi
  000000006714E200: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000006714E205: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006714E20A: E8 A1 FD ED FF     call        000000006702DFB0
  000000006714E20F: 90                 nop
  000000006714E210: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006714E215: E8 06 E9 FF FF     call        000000006714CB20
  000000006714E21A: 48 8B D8           mov         rbx,rax
  000000006714E21D: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  000000006714E222: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006714E226: 75 0B              jne         000000006714E233
  000000006714E228: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  000000006714E22D: E8 7E 21 ED FF     call        00000000670203B0
  000000006714E232: 90                 nop
  000000006714E233: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006714E238: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  000000006714E23C: 75 0A              jne         000000006714E248
  000000006714E23E: 48
  000000006714E23F: 8B
  000000006714E240: 4C

  Summary

      1A7000 .text

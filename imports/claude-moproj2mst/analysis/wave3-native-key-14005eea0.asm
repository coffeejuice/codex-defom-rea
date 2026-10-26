Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014005EEA0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014005EEA4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014005EEA9: 48 83 EC 68        sub         rsp,68h
  000000014005EEAD: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014005EEB6: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014005EEBB: E8 E0 08 00 00     call        000000014005F7A0
  000000014005EEC0: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014005EEC5: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014005EECB: 75 07              jne         000000014005EED4
  000000014005EECD: 32 C0              xor         al,al
  000000014005EECF: E9 9C 00 00 00     jmp         000000014005EF70
  000000014005EED4: 44 8B 44 24 78     mov         r8d,dword ptr [rsp+78h]
  000000014005EED9: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000014005EEDE: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014005EEE3: E8 28 FC FF FF     call        000000014005EB10
  000000014005EEE8: 90                 nop
  000000014005EEE9: 8B 44 24 78        mov         eax,dword ptr [rsp+78h]
  000000014005EEED: 83 C0 01           add         eax,1
  000000014005EEF0: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  000000014005EEF4: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000014005EEF9: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014005EEFE: FF 15 4C 50 DA 01  call        qword ptr [0000000141E03F50h]
  000000014005EF04: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014005EF09: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014005EF0E: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000014005EF13: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000014005EF18: FF 15 3A 50 DA 01  call        qword ptr [0000000141E03F58h]
  000000014005EF1E: 44 8B 44 24 34     mov         r8d,dword ptr [rsp+34h]
  000000014005EF23: 48 8B D0           mov         rdx,rax
  000000014005EF26: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014005EF2B: E8 50 73 83 00     call        0000000140896280
  000000014005EF30: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  000000014005EF34: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000014005EF39: FF 15 51 53 DA 01  call        qword ptr [0000000141E04290h]
  000000014005EF3F: 83 7C 24 30 00     cmp         dword ptr [rsp+30h],0
  000000014005EF44: 74 16              je          000000014005EF5C
  000000014005EF46: C6 44 24 40 00     mov         byte ptr [rsp+40h],0
  000000014005EF4B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014005EF50: FF 15 8A 6A DA 01  call        qword ptr [0000000141E059E0h]
  000000014005EF56: 8A 44 24 40        mov         al,byte ptr [rsp+40h]
  000000014005EF5A: EB 14              jmp         000000014005EF70
  000000014005EF5C: C6 44 24 41 01     mov         byte ptr [rsp+41h],1
  000000014005EF61: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014005EF66: FF 15 74 6A DA 01  call        qword ptr [0000000141E059E0h]
  000000014005EF6C: 8A 44 24 41        mov         al,byte ptr [rsp+41h]
  000000014005EF70: 48 83 C4 68        add         rsp,68h
  000000014005EF74: C3                 ret

  Summary

     1E02000 .text

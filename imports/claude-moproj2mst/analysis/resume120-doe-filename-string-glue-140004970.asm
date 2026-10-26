Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140004970: 66 44 89 44 24 18  mov         word ptr [rsp+18h],r8w
  0000000140004976: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014000497B: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140004980: 48 83 EC 48        sub         rsp,48h
  0000000140004984: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014000498D: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140004995: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  000000014000499A: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000014000499F: FF 15 33 10 E0 01  call        qword ptr [0000000141E059D8h]
  00000001400049A5: 90                 nop
  00000001400049A6: 0F B7 54 24 60     movzx       edx,word ptr [rsp+60h]
  00000001400049AB: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001400049B0: FF 15 92 F8 DF 01  call        qword ptr [0000000141E04248h]
  00000001400049B6: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001400049BB: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400049C0: FF 15 12 10 E0 01  call        qword ptr [0000000141E059D8h]
  00000001400049C6: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  00000001400049CA: 83 C8 01           or          eax,1
  00000001400049CD: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001400049D1: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001400049D6: FF 15 04 10 E0 01  call        qword ptr [0000000141E059E0h]
  00000001400049DC: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400049E1: 48 83 C4 48        add         rsp,48h
  00000001400049E5: C3                 ret
  00000001400049E6: CC                 int         3

  Summary

     1E02000 .text

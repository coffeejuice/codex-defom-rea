Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411F19F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411F19F5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001411F19FA: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  00000001411F19FE: C3                 ret
  00000001411F19FF: CC                 int         3
  00000001411F1A00: CC                 int         3
  00000001411F1A01: CC                 int         3
  00000001411F1A02: CC                 int         3
  00000001411F1A03: CC                 int         3
  00000001411F1A04: CC                 int         3
  00000001411F1A05: CC                 int         3
  00000001411F1A06: CC                 int         3
  00000001411F1A07: CC                 int         3
  00000001411F1A08: CC                 int         3
  00000001411F1A09: CC                 int         3
  00000001411F1A0A: CC                 int         3
  00000001411F1A0B: CC                 int         3
  00000001411F1A0C: CC                 int         3
  00000001411F1A0D: CC                 int         3
  00000001411F1A0E: CC                 int         3
  00000001411F1A0F: CC                 int         3
  00000001411F1A10: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411F1A15: 48 83 EC 38        sub         rsp,38h
  00000001411F1A19: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  00000001411F1A22: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F1A27: E8 54 FF FF FF     call        00000001411F1980
  00000001411F1A2C: 48 85 C0           test        rax,rax
  00000001411F1A2F: 75 04              jne         00000001411F1A35
  00000001411F1A31: 33 C0              xor         eax,eax
  00000001411F1A33: EB 6E              jmp         00000001411F1AA3
  00000001411F1A35: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F1A3A: E8 71 00 00 00     call        00000001411F1AB0
  00000001411F1A3F: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001411F1A43: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F1A48: E8 33 FF FF FF     call        00000001411F1980
  00000001411F1A4D: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001411F1A52: 48 8B C8           mov         rcx,rax
  00000001411F1A55: E8 76 18 A9 FF     call        0000000140C832D0
  00000001411F1A5A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411F1A5F: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001411F1A65: 75 37              jne         00000001411F1A9E
  00000001411F1A67: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F1A6C: E8
  00000001411F1A6D: 0F
  00000001411F1A6E: FF
  00000001411F1A6F: FF

  Summary

     1E02000 .text

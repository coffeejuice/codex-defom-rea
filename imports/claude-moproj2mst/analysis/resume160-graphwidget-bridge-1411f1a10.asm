Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

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
  00000001411F1A6C: E8 0F FF FF FF     call        00000001411F1980
  00000001411F1A71: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  00000001411F1A76: 48 8B C8           mov         rcx,rax
  00000001411F1A79: E8 02 16 A9 FF     call        0000000140C83080
  00000001411F1A7E: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001411F1A82: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001411F1A87: E8 F4 FE FF FF     call        00000001411F1980
  00000001411F1A8C: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001411F1A91: 48 8B C8           mov         rcx,rax
  00000001411F1A94: E8 37 18 A9 FF     call        0000000140C832D0
  00000001411F1A99: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411F1A9E: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001411F1AA3: 48 83 C4 38        add         rsp,38h
  00000001411F1AA7: C3                 ret

  Summary

     1E02000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D7790: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001400D7795: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400D7799: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D779E: 48 83 EC 38        sub         rsp,38h
  00000001400D77A2: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400D77AB: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001400D77B0: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  00000001400D77B5: 7E 3B              jle         00000001400D77F2
  00000001400D77B7: 83 7C 24 48 03     cmp         dword ptr [rsp+48h],3
  00000001400D77BC: 7D 34              jge         00000001400D77F2
  00000001400D77BE: 48 63 4C 24 48     movsxd      rcx,dword ptr [rsp+48h]
  00000001400D77C3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400D77C8: 0F B6 44 08 70     movzx       eax,byte ptr [rax+rcx+70h]
  00000001400D77CD: 85 C0              test        eax,eax
  00000001400D77CF: 75 21              jne         00000001400D77F2
  00000001400D77D1: 48 63 4C 24 48     movsxd      rcx,dword ptr [rsp+48h]
  00000001400D77D6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400D77DB: 48 8D 54 C8 58     lea         rdx,[rax+rcx*8+58h]
  00000001400D77E0: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  00000001400D77E5: 8B 4C 24 48        mov         ecx,dword ptr [rsp+48h]
  00000001400D77E9: E8 72 DB 83 00     call        0000000140915360
  00000001400D77EE: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000001400D77F2: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001400D77F7: 88 44 24 21        mov         byte ptr [rsp+21h],al
  00000001400D77FB: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400D7800: FF 15 DA E1 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7806: 8A 44 24 21        mov         al,byte ptr [rsp+21h]
  00000001400D780A: 48 83 C4 38        add         rsp,38h
  00000001400D780E: C3                 ret

  Summary

     1E02000 .text

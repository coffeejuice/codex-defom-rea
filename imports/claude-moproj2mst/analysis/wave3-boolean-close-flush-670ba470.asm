Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670BA470: 40 57              push        rdi
  00000000670BA472: 48 83 EC 30        sub         rsp,30h
  00000000670BA476: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670BA47F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670BA484: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000670BA488: 83 BB 88 00 00 00  cmp         dword ptr [rbx+0000000000000088h],0
                    00
  00000000670BA48F: 74 76              je          00000000670BA507
  00000000670BA491: E8 DA 6D 0B 00     call        0000000067171270
  00000000670BA496: 33 FF              xor         edi,edi
  00000000670BA498: 89 BB 88 00 00 00  mov         dword ptr [rbx+0000000000000088h],edi
  00000000670BA49E: 48 8D 8B 90 00 00  lea         rcx,[rbx+0000000000000090h]
                    00
  00000000670BA4A5: 48 8D 05 84 66 23  lea         rax,[00000000672F0B30h]
                    00
  00000000670BA4AC: 48 39 01           cmp         qword ptr [rcx],rax
  00000000670BA4AF: 74 2D              je          00000000670BA4DE
  00000000670BA4B1: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000000670BA4B6: F0 83 05 72 66 23  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670BA4BE: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670BA4C3: E8 08 5F FB FF     call        00000000670703D0
  00000000670BA4C8: 90                 nop
  00000000670BA4C9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670BA4CE: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670BA4D2: 75 0A              jne         00000000670BA4DE
  00000000670BA4D4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670BA4D9: E8 F2 5B FB FF     call        00000000670700D0
  00000000670BA4DE: 48 89 BB B8 00 00  mov         qword ptr [rbx+00000000000000B8h],rdi
                    00
  00000000670BA4E5: 48 89 BB C8 00 00  mov         qword ptr [rbx+00000000000000C8h],rdi
                    00
  00000000670BA4EC: 48 8B 83 A8 00 00  mov         rax,qword ptr [rbx+00000000000000A8h]
                    00
  00000000670BA4F3: 48 89 83 A0 00 00  mov         qword ptr [rbx+00000000000000A0h],rax
                    00
  00000000670BA4FA: 89 BB 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],edi
  00000000670BA500: C6 83 E1 00 00 00  mov         byte ptr [rbx+00000000000000E1h],1
                    01
  00000000670BA507: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670BA50C: 48 83 C4 30        add         rsp,30h
  00000000670BA510: 5F                 pop         rdi
  00000000670BA511: C3                 ret

  Summary

      1A7000 .text

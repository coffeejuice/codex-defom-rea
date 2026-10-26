Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B5F30: 40 57              push        rdi
  00000000670B5F32: 48 83 EC 30        sub         rsp,30h
  00000000670B5F36: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B5F3F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000670B5F44: 48 89 74 24 50     mov         qword ptr [rsp+50h],rsi
  00000000670B5F49: 48 8B D9           mov         rbx,rcx
  00000000670B5F4C: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  00000000670B5F50: E8 8B 44 00 00     call        00000000670BA3E0
  00000000670B5F55: 84 C0              test        al,al
  00000000670B5F57: 0F 84 96 00 00 00  je          00000000670B5FF3
  00000000670B5F5D: 48 8B CB           mov         rcx,rbx
  00000000670B5F60: E8 9B FE FF FF     call        00000000670B5E00
  00000000670B5F65: 0F B6 F8           movzx       edi,al
  00000000670B5F68: 48 8B CB           mov         rcx,rbx
  00000000670B5F6B: E8 00 45 00 00     call        00000000670BA470
  00000000670B5F70: C6 86 F8 00 00 00  mov         byte ptr [rsi+00000000000000F8h],0
                    00
  00000000670B5F77: 48 8D 8E 00 01 00  lea         rcx,[rsi+0000000000000100h]
                    00
  00000000670B5F7E: E8 2D F9 FF FF     call        00000000670B58B0
  00000000670B5F83: 48 8B 8E F0 00 00  mov         rcx,qword ptr [rsi+00000000000000F0h]
                    00
  00000000670B5F8A: 48 8B 11           mov         rdx,qword ptr [rcx]
  00000000670B5F8D: FF 52 10           call        qword ptr [rdx+10h]
  00000000670B5F90: 84 C0              test        al,al
  00000000670B5F92: 74 0F              je          00000000670B5FA3
  00000000670B5F94: 40 84 FF           test        dil,dil
  00000000670B5F97: 74 5A              je          00000000670B5FF3
  00000000670B5F99: 48 8B CB           mov         rcx,rbx
  00000000670B5F9C: E8 AF E8 FF FF     call        00000000670B4850
  00000000670B5FA1: EB 50              jmp         00000000670B5FF3
  00000000670B5FA3: 40 84 FF           test        dil,dil
  00000000670B5FA6: 74 4B              je          00000000670B5FF3
  00000000670B5FA8: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670B5FAD: 48 8B 8E F0 00 00  mov         rcx,qword ptr [rsi+00000000000000F0h]
                    00
  00000000670B5FB4: E8 67 1A FF FF     call        00000000670A7A20
  00000000670B5FB9: 48 8B D8           mov         rbx,rax
  00000000670B5FBC: 48 8B 8E F0 00 00  mov         rcx,qword ptr [rsi+00000000000000F0h]
                    00
  00000000670B5FC3: E8 48 1A FF FF     call        00000000670A7A10
  00000000670B5FC8: 89 86 20 01 00 00  mov         dword ptr [rsi+0000000000000120h],eax
  00000000670B5FCE: 48 8D 8E 90 00 00  lea         rcx,[rsi+0000000000000090h]
                    00
  00000000670B5FD5: 48 8B D3           mov         rdx,rbx
  00000000670B5FD8: E8 F3 A3 FB FF     call        00000000670703D0
  00000000670B5FDD: 90                 nop
  00000000670B5FDE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670B5FE3: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B5FE7: 75 0A              jne         00000000670B5FF3
  00000000670B5FE9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670B5FEE: E8 DD A0 FB FF     call        00000000670700D0
  00000000670B5FF3: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000670B5FF8: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  00000000670B5FFD: 48 83 C4 30        add         rsp,30h
  00000000670B6001: 5F                 pop         rdi
  00000000670B6002: C3                 ret

  Summary

      1A7000 .text

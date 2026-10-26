Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408BDE10: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001408BDE14: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408BDE19: 48 83 EC 38        sub         rsp,38h
  00000001408BDE1D: 83 7C 24 48 00     cmp         dword ptr [rsp+48h],0
  00000001408BDE22: 7F 07              jg          00000001408BDE2B
  00000001408BDE24: 33 C0              xor         eax,eax
  00000001408BDE26: E9 CE 00 00 00     jmp         00000001408BDEF9
  00000001408BDE2B: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408BDE30: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  00000001408BDE37: 8B 54 24 48        mov         edx,dword ptr [rsp+48h]
  00000001408BDE3B: E8 D0 0E F9 FF     call        000000014084ED10
  00000001408BDE40: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001408BDE45: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001408BDE4B: 0F 85 A3 00 00 00  jne         00000001408BDEF4
  00000001408BDE51: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408BDE56: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  00000001408BDE5D: E8 0E 52 F1 FF     call        00000001407D3070
  00000001408BDE62: 44 8B D8           mov         r11d,eax
  00000001408BDE65: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  00000001408BDE69: 83 E8 01           sub         eax,1
  00000001408BDE6C: 44 3B D8           cmp         r11d,eax
  00000001408BDE6F: 7D 41              jge         00000001408BDEB2
  00000001408BDE71: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408BDE76: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  00000001408BDE7D: E8 1E 0C F9 FF     call        000000014084EAA0
  00000001408BDE82: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001408BDE87: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408BDE8C: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  00000001408BDE93: E8 D8 51 F1 FF     call        00000001407D3070
  00000001408BDE98: 44 8B D8           mov         r11d,eax
  00000001408BDE9B: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001408BDEA0: 44 89 58 08        mov         dword ptr [rax+8],r11d
  00000001408BDEA4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001408BDEA9: C7 40 0C 07 00 00  mov         dword ptr [rax+0Ch],7
                    00
  00000001408BDEB0: EB 9F              jmp         00000001408BDE51
  00000001408BDEB2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408BDEB7: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  00000001408BDEBE: E8 DD 0B F9 FF     call        000000014084EAA0
  00000001408BDEC3: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001408BDEC8: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001408BDECD: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  00000001408BDED1: 89 41 08           mov         dword ptr [rcx+8],eax
  00000001408BDED4: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001408BDED9: 48 81 C1 80 13 00  add         rcx,1380h
                    00
  00000001408BDEE0: E8 8B 51 F1 FF     call        00000001407D3070
  00000001408BDEE5: 44 8B D8           mov         r11d,eax
  00000001408BDEE8: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001408BDEED: 44 89 98 78 13 00  mov         dword ptr [rax+0000000000001378h],r11d
                    00
  00000001408BDEF4: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001408BDEF9: 48 83 C4 38        add         rsp,38h
  00000001408BDEFD: C3                 ret

  Summary

     1E02000 .text

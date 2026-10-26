Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140817D10: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140817D15: 53                 push        rbx
  0000000140817D16: 48 83 EC 70        sub         rsp,70h
  0000000140817D1A: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140817D22: 48 83 C1 28        add         rcx,28h
  0000000140817D26: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140817D2B: E8 30 2C 92 FF     call        000000014013A960
  0000000140817D30: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140817D38: 48 83 C1 28        add         rcx,28h
  0000000140817D3C: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140817D41: E8 6A 0B 00 00     call        00000001408188B0
  0000000140817D46: 48 8B D0           mov         rdx,rax
  0000000140817D49: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140817D4E: E8 1D E7 90 FF     call        0000000140126470
  0000000140817D53: 0F B6 C0           movzx       eax,al
  0000000140817D56: 85 C0              test        eax,eax
  0000000140817D58: 0F 84 A3 00 00 00  je          0000000140817E01
  0000000140817D5E: 48 8D 44 24 30     lea         rax,[rsp+30h]
  0000000140817D63: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140817D68: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140817D6D: E8 8E 76 E3 FF     call        000000014064F400
  0000000140817D72: 48 8B 18           mov         rbx,qword ptr [rax]
  0000000140817D75: 48 8D 15 5C CD F8  lea         rdx,[00000001427A4AD8h]
                    01
  0000000140817D7C: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140817D81: FF 15 F9 C6 5E 01  call        qword ptr [0000000141E04480h]
  0000000140817D87: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140817D8C: 44 8B 4B 1C        mov         r9d,dword ptr [rbx+1Ch]
  0000000140817D90: 4C 8B 44 24 58     mov         r8,qword ptr [rsp+58h]
  0000000140817D95: BA 31 01 00 00     mov         edx,131h
  0000000140817D9A: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140817DA2: E8 39 F1 FF FF     call        0000000140816EE0
  0000000140817DA7: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140817DAC: E8 4F 76 E3 FF     call        000000014064F400
  0000000140817DB1: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140817DB4: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140817DB9: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140817DBE: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140817DC3: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140817DC9: 74 16              je          0000000140817DE1
  0000000140817DCB: BA 01 00 00 00     mov         edx,1
  0000000140817DD0: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140817DD5: E8 36 0A 00 00     call        0000000140818810
  0000000140817DDA: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140817DDF: EB 09              jmp         0000000140817DEA
  0000000140817DE1: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000140817DEA: 45 33 C0           xor         r8d,r8d
  0000000140817DED: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000140817DF2: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140817DF7: E8 24 FE BB 00     call        00000001413D7C20
  0000000140817DFC: E9 2F FF FF FF     jmp         0000000140817D30
  0000000140817E01: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140817E09: 48 83 C1 28        add         rcx,28h
  0000000140817E0D: E8 6E 58 94 FF     call        000000014015D680
  0000000140817E12: 48 83 C4 70        add         rsp,70h
  0000000140817E16: 5B                 pop         rbx
  0000000140817E17: C3                 ret

  Summary

     1E02000 .text

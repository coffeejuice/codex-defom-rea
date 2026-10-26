Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006701DC80: 40 57              push        rdi
  000000006701DC82: 48 83 EC 30        sub         rsp,30h
  000000006701DC86: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006701DC8F: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006701DC94: 48 89 6C 24 48     mov         qword ptr [rsp+48h],rbp
  000000006701DC99: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  000000006701DC9E: 48 8B EA           mov         rbp,rdx
  000000006701DCA1: 48 63 F9           movsxd      rdi,ecx
  000000006701DCA4: E8 17 FE FF FF     call        000000006701DAC0
  000000006701DCA9: 48 8B D8           mov         rbx,rax
  000000006701DCAC: 48 85 C0           test        rax,rax
  000000006701DCAF: 0F 84 A4 00 00 00  je          000000006701DD59
  000000006701DCB5: 48 8B 00           mov         rax,qword ptr [rax]
  000000006701DCB8: 3B 78 08           cmp         edi,dword ptr [rax+8]
  000000006701DCBB: 0F 8D 98 00 00 00  jge         000000006701DD59
  000000006701DCC1: 44 8B 00           mov         r8d,dword ptr [rax]
  000000006701DCC4: 41 83 F8 01        cmp         r8d,1
  000000006701DCC8: 74 12              je          000000006701DCDC
  000000006701DCCA: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006701DCCD: 44 8B 41 04        mov         r8d,dword ptr [rcx+4]
  000000006701DCD1: 8B 51 08           mov         edx,dword ptr [rcx+8]
  000000006701DCD4: 48 8B CB           mov         rcx,rbx
  000000006701DCD7: E8 64 FB FF FF     call        000000006701D840
  000000006701DCDC: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006701DCDF: 48 8B 44 F8 10     mov         rax,qword ptr [rax+rdi*8+10h]
  000000006701DCE4: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000006701DCE9: F0 83 00 01        lock add    dword ptr [rax],1
  000000006701DCED: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000006701DCF2: F6 40 10 01        test        byte ptr [rax+10h],1
  000000006701DCF6: 75 12              jne         000000006701DD0A
  000000006701DCF8: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006701DCFB: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006701DD00: E8 6B 59 0E 00     call        0000000067103670
  000000006701DD05: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000006701DD0A: 32 DB              xor         bl,bl
  000000006701DD0C: 33 FF              xor         edi,edi
  000000006701DD0E: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006701DD11: 48 8B F7           mov         rsi,rdi
  000000006701DD14: 8B 48 0C           mov         ecx,dword ptr [rax+0Ch]
  000000006701DD17: 2B CA              sub         ecx,edx
  000000006701DD19: 85 C9              test        ecx,ecx
  000000006701DD1B: 7E 28              jle         000000006701DD45
  000000006701DD1D: 0F 1F 00           nop         dword ptr [rax]
  000000006701DD20: 48 63 D2           movsxd      rdx,edx
  000000006701DD23: 48 03 D6           add         rdx,rsi
  000000006701DD26: 48 8B CD           mov         rcx,rbp
  000000006701DD29: FF 54 D0 18        call        qword ptr [rax+rdx*8+18h]
  000000006701DD2D: 0A D8              or          bl,al
  000000006701DD2F: FF C7              inc         edi
  000000006701DD31: 48 FF C6           inc         rsi
  000000006701DD34: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000006701DD39: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006701DD3C: 8B 48 0C           mov         ecx,dword ptr [rax+0Ch]
  000000006701DD3F: 2B CA              sub         ecx,edx
  000000006701DD41: 3B F9              cmp         edi,ecx
  000000006701DD43: 7C DB              jl          000000006701DD20
  000000006701DD45: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006701DD49: 75 0A              jne         000000006701DD55
  000000006701DD4B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000006701DD50: E8 5B 26 00 00     call        00000000670203B0
  000000006701DD55: 8A C3              mov         al,bl
  000000006701DD57: EB 02              jmp         000000006701DD5B
  000000006701DD59: 32 C0              xor         al,al
  000000006701DD5B: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  000000006701DD60: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  000000006701DD65: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  000000006701DD6A: 48 83 C4 30        add         rsp,30h
  000000006701DD6E: 5F                 pop         rdi
  000000006701DD6F: C3                 ret

  Summary

      1A7000 .text

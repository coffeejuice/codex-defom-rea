Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670FFBF0: 4C 8B DC           mov         r11,rsp
  00000000670FFBF3: 49 89 4B 08        mov         qword ptr [r11+8],rcx
  00000000670FFBF7: 53                 push        rbx
  00000000670FFBF8: 57                 push        rdi
  00000000670FFBF9: 48 81 EC 68 02 00  sub         rsp,268h
                    00
  00000000670FFC00: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670FFC09: 48 8B FA           mov         rdi,rdx
  00000000670FFC0C: 48 8B D9           mov         rbx,rcx
  00000000670FFC0F: 41 C7 43 10 00 00  mov         dword ptr [r11+10h],0
                    00 00
  00000000670FFC17: 48 8D 05 12 0F 1F  lea         rax,[00000000672F0B30h]
                    00
  00000000670FFC1E: 48 89 01           mov         qword ptr [rcx],rax
  00000000670FFC21: F0 83 05 07 0F 1F  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670FFC29: 41 C7 43 10 01 00  mov         dword ptr [r11+10h],1
                    00 00
  00000000670FFC31: 48 8B 02           mov         rax,qword ptr [rdx]
  00000000670FFC34: 44 8B 40 08        mov         r8d,dword ptr [rax+8]
  00000000670FFC38: 41 FF C0           inc         r8d
  00000000670FFC3B: 45 89 43 18        mov         dword ptr [r11+18h],r8d
  00000000670FFC3F: 41 C7 43 20 04 01  mov         dword ptr [r11+20h],104h
                    00 00
  00000000670FFC47: 49 8D 43 18        lea         rax,[r11+18h]
  00000000670FFC4B: 49 8D 4B 20        lea         rcx,[r11+20h]
  00000000670FFC4F: 41 81 F8 04 01 00  cmp         r8d,104h
                    00
  00000000670FFC56: 48 0F 4E C1        cmovle      rax,rcx
  00000000670FFC5A: 48 63 08           movsxd      rcx,dword ptr [rax]
  00000000670FFC5D: 89 4C 24 44        mov         dword ptr [rsp+44h],ecx
  00000000670FFC61: 81 F9 04 01 00 00  cmp         ecx,104h
  00000000670FFC67: 7E 21              jle         00000000670FFC8A
  00000000670FFC69: 48 03 C9           add         rcx,rcx
  00000000670FFC6C: E8 2F 07 F2 FF     call        00000000670203A0
  00000000670FFC71: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000000670FFC76: 48 85 C0           test        rax,rax
  00000000670FFC79: 75 05              jne         00000000670FFC80
  00000000670FFC7B: E8 30 C3 F1 FF     call        000000006701BFB0
  00000000670FFC80: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  00000000670FFC84: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  00000000670FFC88: EB 12              jmp         00000000670FFC9C
  00000000670FFC8A: 48 8D 44 24 50     lea         rax,[rsp+50h]
  00000000670FFC8F: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000000670FFC94: C7 44 24 40 04 01  mov         dword ptr [rsp+40h],104h
                    00 00
  00000000670FFC9C: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000000670FFCA5: 48 8B CF           mov         rcx,rdi
  00000000670FFCA8: E8 A3 0D F7 FF     call        0000000067070A50
  00000000670FFCAD: 48 8B C8           mov         rcx,rax
  00000000670FFCB0: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  00000000670FFCB5: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000000670FFCBA: 8B 54 24 44        mov         edx,dword ptr [rsp+44h]
  00000000670FFCBE: FF 15 A4 85 0A 00  call        qword ptr [00000000671A8268h]
  00000000670FFCC4: 3B 44 24 44        cmp         eax,dword ptr [rsp+44h]
  00000000670FFCC8: 76 44              jbe         00000000670FFD0E
  00000000670FFCCA: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000000670FFCCE: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670FFCD3: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670FFCD8: 3B 44 24 40        cmp         eax,dword ptr [rsp+40h]
  00000000670FFCDC: 48 0F 4D CA        cmovge      rcx,rdx
  00000000670FFCE0: 44 8B 01           mov         r8d,dword ptr [rcx]
  00000000670FFCE3: 8B D0              mov         edx,eax
  00000000670FFCE5: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670FFCEA: E8 31 AA 04 00     call        000000006714A720
  00000000670FFCEF: 48 8B CF           mov         rcx,rdi
  00000000670FFCF2: E8 59 0D F7 FF     call        0000000067070A50
  00000000670FFCF7: 48 8B C8           mov         rcx,rax
  00000000670FFCFA: 4C 8D 4C 24 30     lea         r9,[rsp+30h]
  00000000670FFCFF: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  00000000670FFD04: 8B 54 24 44        mov         edx,dword ptr [rsp+44h]
  00000000670FFD08: FF 15 5A 85 0A 00  call        qword ptr [00000000671A8268h]
  00000000670FFD0E: 85 C0              test        eax,eax
  00000000670FFD10: 74 34              je          00000000670FFD46
  00000000670FFD12: 44 8B C0           mov         r8d,eax
  00000000670FFD15: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  00000000670FFD1A: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670FFD1F: E8 7C 80 07 00     call        0000000067177DA0
  00000000670FFD24: 90                 nop
  00000000670FFD25: 48 8B D0           mov         rdx,rax
  00000000670FFD28: 48 8B CB           mov         rcx,rbx
  00000000670FFD2B: E8 A0 06 F7 FF     call        00000000670703D0
  00000000670FFD30: 90                 nop
  00000000670FFD31: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670FFD36: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FFD3A: 75 0A              jne         00000000670FFD46
  00000000670FFD3C: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670FFD41: E8 8A 03 F7 FF     call        00000000670700D0
  00000000670FFD46: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670FFD49: 48 63 48 08        movsxd      rcx,dword ptr [rax+8]
  00000000670FFD4D: 85 C9              test        ecx,ecx
  00000000670FFD4F: 74 1A              je          00000000670FFD6B
  00000000670FFD51: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000000670FFD55: 66 83 7C 48 FE 20  cmp         word ptr [rax+rcx*2-2],20h
  00000000670FFD5B: 75 0E              jne         00000000670FFD6B
  00000000670FFD5D: BA 20 00 00 00     mov         edx,20h
  00000000670FFD62: 48 8B CB           mov         rcx,rbx
  00000000670FFD65: E8 46 08 F7 FF     call        00000000670705B0
  00000000670FFD6A: 90                 nop
  00000000670FFD6B: 48 8D 44 24 50     lea         rax,[rsp+50h]
  00000000670FFD70: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000000670FFD75: 48 3B C8           cmp         rcx,rax
  00000000670FFD78: 74 0A              je          00000000670FFD84
  00000000670FFD7A: E8 31 06 F2 FF     call        00000000670203B0
  00000000670FFD7F: 48 8B C3           mov         rax,rbx
  00000000670FFD82: EB 03              jmp         00000000670FFD87
  00000000670FFD84: 48 8B C3           mov         rax,rbx
  00000000670FFD87: 48 81 C4 68 02 00  add         rsp,268h
                    00
  00000000670FFD8E: 5F                 pop         rdi
  00000000670FFD8F: 5B                 pop         rbx
  00000000670FFD90: C3                 ret
  00000000670FFD91: CC                 int         3

  Summary

      1A7000 .text

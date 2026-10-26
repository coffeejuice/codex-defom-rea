Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653FE680: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000653FE685: 57                 push        rdi
  00000000653FE686: 48 83 EC 30        sub         rsp,30h
  00000000653FE68A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653FE693: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000653FE698: 48 8B DA           mov         rbx,rdx
  00000000653FE69B: 48 8B F9           mov         rdi,rcx
  00000000653FE69E: B9 B8 02 00 00     mov         ecx,2B8h
  00000000653FE6A3: E8 CA 2A 1E 00     call        00000000655E1172
  00000000653FE6A8: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000000653FE6AD: 48 85 C0           test        rax,rax
  00000000653FE6B0: 74 0A              je          00000000653FE6BC
  00000000653FE6B2: 48 8B C8           mov         rcx,rax
  00000000653FE6B5: E8 86 FA FF FF     call        00000000653FE140
  00000000653FE6BA: EB 02              jmp         00000000653FE6BE
  00000000653FE6BC: 33 C0              xor         eax,eax
  00000000653FE6BE: 4C 8B C3           mov         r8,rbx
  00000000653FE6C1: 48 8B D0           mov         rdx,rax
  00000000653FE6C4: 48 8B CF           mov         rcx,rdi
  00000000653FE6C7: E8 A4 C2 F7 FF     call        000000006537A970
  00000000653FE6CC: 90                 nop
  00000000653FE6CD: 48 8D 05 14 C0 30  lea         rax,[000000006570A6E8h]
                    00
  00000000653FE6D4: 48 89 07           mov         qword ptr [rdi],rax
  00000000653FE6D7: 48 8D 05 D2 BF 30  lea         rax,[000000006570A6B0h]
                    00
  00000000653FE6DE: 48 89 47 10        mov         qword ptr [rdi+10h],rax
  00000000653FE6E2: 48 8B 5F 08        mov         rbx,qword ptr [rdi+8]
  00000000653FE6E6: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],10000h
                    01 00
  00000000653FE6EE: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000653FE6F3: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  00000000653FE6F7: E8 34 17 C6 FF     call        000000006505FE30
  00000000653FE6FC: 45 33 C0           xor         r8d,r8d
  00000000653FE6FF: 41 8D 50 33        lea         edx,[r8+33h]
  00000000653FE703: 48 8B CB           mov         rcx,rbx
  00000000653FE706: E8 15 FD C5 FF     call        000000006505E420
  00000000653FE70B: 90                 nop
  00000000653FE70C: 48 8B C7           mov         rax,rdi
  00000000653FE70F: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000653FE714: 48 83 C4 30        add         rsp,30h
  00000000653FE718: 5F                 pop         rdi
  00000000653FE719: C3                 ret

  Summary

      695000 .text

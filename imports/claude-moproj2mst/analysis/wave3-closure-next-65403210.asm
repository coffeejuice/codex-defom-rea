Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065403210: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065403215: 53                 push        rbx
  0000000065403216: 56                 push        rsi
  0000000065403217: 57                 push        rdi
  0000000065403218: 48 83 EC 30        sub         rsp,30h
  000000006540321C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065403225: 49 8B D8           mov         rbx,r8
  0000000065403228: 8B F2              mov         esi,edx
  000000006540322A: 48 8B F9           mov         rdi,rcx
  000000006540322D: B9 28 02 00 00     mov         ecx,228h
  0000000065403232: E8 3B DF 1D 00     call        00000000655E1172
  0000000065403237: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000006540323C: 48 85 C0           test        rax,rax
  000000006540323F: 74 0A              je          000000006540324B
  0000000065403241: 48 8B C8           mov         rcx,rax
  0000000065403244: E8 27 F8 FF FF     call        0000000065402A70
  0000000065403249: EB 02              jmp         000000006540324D
  000000006540324B: 33 C0              xor         eax,eax
  000000006540324D: C7 44 24 58 00 00  mov         dword ptr [rsp+58h],0
                    00 00
  0000000065403255: 4C 8D 4C 24 58     lea         r9,[rsp+58h]
  000000006540325A: 4C 8B C3           mov         r8,rbx
  000000006540325D: 48 8B D0           mov         rdx,rax
  0000000065403260: 48 8B CF           mov         rcx,rdi
  0000000065403263: E8 08 F2 FA FF     call        00000000653B2470
  0000000065403268: 90                 nop
  0000000065403269: 48 8D 05 68 82 30  lea         rax,[000000006570B4D8h]
                    00
  0000000065403270: 48 89 07           mov         qword ptr [rdi],rax
  0000000065403273: 48 8D 05 1E 82 30  lea         rax,[000000006570B498h]
                    00
  000000006540327A: 48 89 47 10        mov         qword ptr [rdi+10h],rax
  000000006540327E: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065403282: 89 B0 18 02 00 00  mov         dword ptr [rax+0000000000000218h],esi
  0000000065403288: 48 8B 58 08        mov         rbx,qword ptr [rax+8]
  000000006540328C: BA 57 00 00 00     mov         edx,57h
  0000000065403291: B8 75 00 00 00     mov         eax,75h
  0000000065403296: 83 FE 02           cmp         esi,2
  0000000065403299: 0F 44 D0           cmove       edx,eax
  000000006540329C: 48 8B CB           mov         rcx,rbx
  000000006540329F: E8 7C A8 C6 FF     call        000000006506DB20
  00000000654032A4: 45 33 C0           xor         r8d,r8d
  00000000654032A7: 41 8D 50 44        lea         edx,[r8+44h]
  00000000654032AB: 48 8B CB           mov         rcx,rbx
  00000000654032AE: E8 2D 5D C6 FF     call        0000000065068FE0
  00000000654032B3: 90                 nop
  00000000654032B4: 48 8B C7           mov         rax,rdi
  00000000654032B7: 48 83 C4 30        add         rsp,30h
  00000000654032BB: 5F                 pop         rdi
  00000000654032BC: 5E                 pop         rsi
  00000000654032BD: 5B                 pop         rbx
  00000000654032BE: C3                 ret

  Summary

      695000 .text

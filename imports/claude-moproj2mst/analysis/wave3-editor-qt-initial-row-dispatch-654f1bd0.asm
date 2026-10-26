Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654F1BD0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000654F1BD5: 57                 push        rdi
  00000000654F1BD6: 48 83 EC 30        sub         rsp,30h
  00000000654F1BDA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654F1BE3: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000654F1BE8: 48 89 74 24 50     mov         qword ptr [rsp+50h],rsi
  00000000654F1BED: 49 8B F8           mov         rdi,r8
  00000000654F1BF0: 48 8B F2           mov         rsi,rdx
  00000000654F1BF3: 48 8B D9           mov         rbx,rcx
  00000000654F1BF6: B9 D8 00 00 00     mov         ecx,0D8h
  00000000654F1BFB: E8 72 F5 0E 00     call        00000000655E1172
  00000000654F1C00: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000000654F1C05: 48 85 C0           test        rax,rax
  00000000654F1C08: 74 0A              je          00000000654F1C14
  00000000654F1C0A: 48 8B C8           mov         rcx,rax
  00000000654F1C0D: E8 EE FB FF FF     call        00000000654F1800
  00000000654F1C12: EB 02              jmp         00000000654F1C16
  00000000654F1C14: 33 C0              xor         eax,eax
  00000000654F1C16: 4C 8B C7           mov         r8,rdi
  00000000654F1C19: 48 8B D0           mov         rdx,rax
  00000000654F1C1C: 48 8B CB           mov         rcx,rbx
  00000000654F1C1F: FF 15 0B 4E 1A 00  call        qword ptr [0000000065696A30h]
  00000000654F1C25: 90                 nop
  00000000654F1C26: 48 8D 05 AB 14 24  lea         rax,[00000000657330D8h]
                    00
  00000000654F1C2D: 48 89 03           mov         qword ptr [rbx],rax
  00000000654F1C30: 48 8B D6           mov         rdx,rsi
  00000000654F1C33: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  00000000654F1C37: E8 C4 EF FF FF     call        00000000654F0C00
  00000000654F1C3C: 90                 nop
  00000000654F1C3D: 48 8B C3           mov         rax,rbx
  00000000654F1C40: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  00000000654F1C45: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  00000000654F1C4A: 48 83 C4 30        add         rsp,30h
  00000000654F1C4E: 5F                 pop         rdi
  00000000654F1C4F: C3                 ret

  Summary

      695000 .text

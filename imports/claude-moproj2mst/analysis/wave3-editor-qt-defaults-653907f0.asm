Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653907F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000653907F5: 57                 push        rdi
  00000000653907F6: 48 83 EC 30        sub         rsp,30h
  00000000653907FA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065390803: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000065390808: 48 8B FA           mov         rdi,rdx
  000000006539080B: 48 8B D9           mov         rbx,rcx
  000000006539080E: B9 68 02 00 00     mov         ecx,268h
  0000000065390813: E8 5A 09 25 00     call        00000000655E1172
  0000000065390818: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000006539081D: 48 85 C0           test        rax,rax
  0000000065390820: 74 0A              je          000000006539082C
  0000000065390822: 48 8B C8           mov         rcx,rax
  0000000065390825: E8 66 5A FF FF     call        0000000065386290
  000000006539082A: EB 02              jmp         000000006539082E
  000000006539082C: 33 C0              xor         eax,eax
  000000006539082E: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  0000000065390836: 4C 8D 4C 24 50     lea         r9,[rsp+50h]
  000000006539083B: 4C 8B C7           mov         r8,rdi
  000000006539083E: 48 8B D0           mov         rdx,rax
  0000000065390841: 48 8B CB           mov         rcx,rbx
  0000000065390844: E8 B7 06 CE FF     call        0000000065070F00
  0000000065390849: 90                 nop
  000000006539084A: 48 8D 05 47 81 36  lea         rax,[00000000656F8998h]
                    00
  0000000065390851: 48 89 03           mov         qword ptr [rbx],rax
  0000000065390854: 48 8D 05 FD 80 36  lea         rax,[00000000656F8958h]
                    00
  000000006539085B: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  000000006539085F: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  0000000065390863: E8 38 FE FF FF     call        00000000653906A0
  0000000065390868: 90                 nop
  0000000065390869: 48 8B C3           mov         rax,rbx
  000000006539086C: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065390871: 48 83 C4 30        add         rsp,30h
  0000000065390875: 5F                 pop         rdi
  0000000065390876: C3                 ret

  Summary

      695000 .text

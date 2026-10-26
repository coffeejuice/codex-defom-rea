Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670EF4B0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  00000000670EF4B5: 57                 push        rdi
  00000000670EF4B6: 48 83 EC 20        sub         rsp,20h
  00000000670EF4BA: 48 8B DA           mov         rbx,rdx
  00000000670EF4BD: 48 8B F9           mov         rdi,rcx
  00000000670EF4C0: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000000670EF4C8: E8 13 FD FF FF     call        00000000670EF1E0
  00000000670EF4CD: 44 0F B7 5F 10     movzx       r11d,word ptr [rdi+10h]
  00000000670EF4D2: 66 41 83 FB FF     cmp         r11w,0FFFFh
  00000000670EF4D7: 75 37              jne         00000000670EF510
  00000000670EF4D9: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670EF4DC: 83 78 08 02        cmp         dword ptr [rax+8],2
  00000000670EF4E0: 7C 13              jl          00000000670EF4F5
  00000000670EF4E2: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000000670EF4E6: 66 83 78 02 3A     cmp         word ptr [rax+2],3Ah
  00000000670EF4EB: 75 08              jne         00000000670EF4F5
  00000000670EF4ED: 41 B8 02 00 00 00  mov         r8d,2
  00000000670EF4F3: EB 5B              jmp         00000000670EF550
  00000000670EF4F5: BA 2E 00 00 00     mov         edx,2Eh
  00000000670EF4FA: 48 8B CB           mov         rcx,rbx
  00000000670EF4FD: E8 1E C2 F7 FF     call        000000006706B720
  00000000670EF502: 48 8B C3           mov         rax,rbx
  00000000670EF505: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670EF50A: 48 83 C4 20        add         rsp,20h
  00000000670EF50E: 5F                 pop         rdi
  00000000670EF50F: C3                 ret
  00000000670EF510: 66 45 85 DB        test        r11w,r11w
  00000000670EF514: 75 1B              jne         00000000670EF531
  00000000670EF516: BA 2F 00 00 00     mov         edx,2Fh
  00000000670EF51B: 48 8B CB           mov         rcx,rbx
  00000000670EF51E: E8 FD C1 F7 FF     call        000000006706B720
  00000000670EF523: 48 8B C3           mov         rax,rbx
  00000000670EF526: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670EF52B: 48 83 C4 20        add         rsp,20h
  00000000670EF52F: 5F                 pop         rdi
  00000000670EF530: C3                 ret
  00000000670EF531: 66 41 83 FB 02     cmp         r11w,2
  00000000670EF536: 75 14              jne         00000000670EF54C
  00000000670EF538: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670EF53B: 41 B8 03 00 00 00  mov         r8d,3
  00000000670EF541: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000000670EF545: 66 83 79 02 3A     cmp         word ptr [rcx+2],3Ah
  00000000670EF54A: 74 04              je          00000000670EF550
  00000000670EF54C: 45 0F BF C3        movsx       r8d,r11w
  00000000670EF550: 48 8B D3           mov         rdx,rbx
  00000000670EF553: 48 8B CF           mov         rcx,rdi
  00000000670EF556: E8 95 49 F8 FF     call        0000000067073EF0
  00000000670EF55B: 48 8B C3           mov         rax,rbx
  00000000670EF55E: 48 8B 5C 24 38     mov         rbx,qword ptr [rsp+38h]
  00000000670EF563: 48 83 C4 20        add         rsp,20h
  00000000670EF567: 5F                 pop         rdi
  00000000670EF568: C3                 ret

  Summary

      1A7000 .text

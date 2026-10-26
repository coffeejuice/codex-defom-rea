Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411E2480: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001411E2485: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411E248A: 57                 push        rdi
  00000001411E248B: 48 83 EC 40        sub         rsp,40h
  00000001411E248F: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001411E2498: 45 33 C0           xor         r8d,r8d
  00000001411E249B: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  00000001411E24A0: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411E24A5: E8 96 E2 F4 FE     call        0000000140130740
  00000001411E24AA: 90                 nop
  00000001411E24AB: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411E24B0: 48 8D 05 61 76 AF  lea         rax,[0000000142CD9B18h]
                    01
  00000001411E24B7: 48 89 01           mov         qword ptr [rcx],rax
  00000001411E24BA: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411E24BF: 48 8D 05 12 76 AF  lea         rax,[0000000142CD9AD8h]
                    01
  00000001411E24C6: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001411E24CA: 48 C7 44 24 20 40  mov         qword ptr [rsp+20h],40h
                    00 00 00
  00000001411E24D3: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001411E24D8: E8 DF C5 28 00     call        000000014146EABC
  00000001411E24DD: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001411E24E2: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001411E24E8: 74 1A              je          00000001411E2504
  00000001411E24EA: 48 8B 7C 24 28     mov         rdi,qword ptr [rsp+28h]
  00000001411E24EF: 33 C0              xor         eax,eax
  00000001411E24F1: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001411E24F6: F3 AA              rep stos    byte ptr [rdi]
  00000001411E24F8: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001411E24FD: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001411E2502: EB 09              jmp         00000001411E250D
  00000001411E2504: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001411E250D: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411E2512: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001411E2517: 48 89 41 48        mov         qword ptr [rcx+48h],rax
  00000001411E251B: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  00000001411E2520: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411E2525: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  00000001411E2529: E8 A2 08 00 00     call        00000001411E2DD0
  00000001411E252E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411E2533: E8 78 00 00 00     call        00000001411E25B0
  00000001411E2538: 90                 nop
  00000001411E2539: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001411E253E: 48 83 C4 40        add         rsp,40h
  00000001411E2542: 5F                 pop         rdi
  00000001411E2543: C3                 ret

  Summary

     1E02000 .text

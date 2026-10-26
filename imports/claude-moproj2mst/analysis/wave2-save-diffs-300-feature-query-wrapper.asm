Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412EEF80: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001412EEF85: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  00000001412EEF8A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  00000001412EEF8F: 57                 push        rdi
  00000001412EEF90: 48 83 EC 30        sub         rsp,30h
  00000001412EEF94: 48 8B F1           mov         rsi,rcx
  00000001412EEF97: B9 A0 3E 00 00     mov         ecx,3EA0h
  00000001412EEF9C: 49 8B E9           mov         rbp,r9
  00000001412EEF9F: 41 8B D8           mov         ebx,r8d
  00000001412EEFA2: 8B FA              mov         edi,edx
  00000001412EEFA4: E8 13 FB 17 00     call        000000014146EABC
  00000001412EEFA9: 48 85 C0           test        rax,rax
  00000001412EEFAC: 74 2A              je          00000001412EEFD8
  00000001412EEFAE: 48 8D 0D 33 5D AC  lea         rcx,[0000000142DB4CE8h]
                    01
  00000001412EEFB5: C7 44 24 28 01 00  mov         dword ptr [rsp+28h],1
                    00 00
  00000001412EEFBD: 44 8B CB           mov         r9d,ebx
  00000001412EEFC0: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  00000001412EEFC5: 44 8B C7           mov         r8d,edi
  00000001412EEFC8: 48 8B D6           mov         rdx,rsi
  00000001412EEFCB: 48 8B C8           mov         rcx,rax
  00000001412EEFCE: E8 CD F6 FF FF     call        00000001412EE6A0
  00000001412EEFD3: 48 8B D8           mov         rbx,rax
  00000001412EEFD6: EB 02              jmp         00000001412EEFDA
  00000001412EEFD8: 33 DB              xor         ebx,ebx
  00000001412EEFDA: 4C 8D 0D 1F 5B AC  lea         r9,[0000000142DB4B00h]
                    01
  00000001412EEFE1: 48 8D 0D A8 0E 40  lea         rcx,[00000001446EFE90h]
                    03
  00000001412EEFE8: 41 B8 A0 3E 00 00  mov         r8d,3EA0h
  00000001412EEFEE: 8B D3              mov         edx,ebx
  00000001412EEFF0: C7 44 24 20 9F 11  mov         dword ptr [rsp+20h],119Fh
                    00 00
  00000001412EEFF8: E8 73 65 00 00     call        00000001412F5570
  00000001412EEFFD: 48 8B D5           mov         rdx,rbp
  00000001412EF000: 48 8B CB           mov         rcx,rbx
  00000001412EF003: E8 C8 77 FF FF     call        00000001412E67D0
  00000001412EF008: 48 8B CB           mov         rcx,rbx
  00000001412EF00B: E8 80 5B FF FF     call        00000001412E4B90
  00000001412EF010: 48 8D 0D 79 0E 40  lea         rcx,[00000001446EFE90h]
                    03
  00000001412EF017: 8B D3              mov         edx,ebx
  00000001412EF019: 8B F8              mov         edi,eax
  00000001412EF01B: E8 90 61 00 00     call        00000001412F51B0
  00000001412EF020: 48 85 DB           test        rbx,rbx
  00000001412EF023: 74 0E              je          00000001412EF033
  00000001412EF025: 4C 8B 03           mov         r8,qword ptr [rbx]
  00000001412EF028: BA 01 00 00 00     mov         edx,1
  00000001412EF02D: 48 8B CB           mov         rcx,rbx
  00000001412EF030: 41 FF 10           call        qword ptr [r8]
  00000001412EF033: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000001412EF038: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  00000001412EF03D: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  00000001412EF042: 8B C7              mov         eax,edi
  00000001412EF044: 48 83 C4 30        add         rsp,30h
  00000001412EF048: 5F                 pop         rdi
  00000001412EF049: C3                 ret
  00000001412EF04A: CC                 int         3

  Summary

     1E02000 .text

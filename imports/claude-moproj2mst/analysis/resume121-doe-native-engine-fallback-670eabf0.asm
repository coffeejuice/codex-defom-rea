Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670EABF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670EABF5: 57                 push        rdi
  00000000670EABF6: 48 83 EC 40        sub         rsp,40h
  00000000670EABFA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670EAC03: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000670EAC08: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  00000000670EAC0D: 48 8B FA           mov         rdi,rdx
  00000000670EAC10: 48 8B F1           mov         rsi,rcx
  00000000670EAC13: B9 A8 00 00 00     mov         ecx,0A8h
  00000000670EAC18: E8 69 DE 08 00     call        0000000067178A86
  00000000670EAC1D: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000000670EAC22: 48 85 C0           test        rax,rax
  00000000670EAC25: 74 0A              je          00000000670EAC31
  00000000670EAC27: 48 8B C8           mov         rcx,rax
  00000000670EAC2A: E8 41 FE FF FF     call        00000000670EAA70
  00000000670EAC2F: EB 02              jmp         00000000670EAC33
  00000000670EAC31: 33 C0              xor         eax,eax
  00000000670EAC33: 48 8B D0           mov         rdx,rax
  00000000670EAC36: 48 8B CE           mov         rcx,rsi
  00000000670EAC39: E8 E2 CB FB FF     call        00000000670A7820
  00000000670EAC3E: 90                 nop
  00000000670EAC3F: 48 8D 05 72 9A 14  lea         rax,[00000000672346B8h]
                    00
  00000000670EAC46: 48 89 06           mov         qword ptr [rsi],rax
  00000000670EAC49: 48 8B 5E 08        mov         rbx,qword ptr [rsi+8]
  00000000670EAC4D: 48 8B D7           mov         rdx,rdi
  00000000670EAC50: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670EAC55: E8 B6 40 00 00     call        00000000670EED10
  00000000670EAC5A: 48 8B F8           mov         rdi,rax
  00000000670EAC5D: 48 8B D0           mov         rdx,rax
  00000000670EAC60: 48 8D 4B 20        lea         rcx,[rbx+20h]
  00000000670EAC64: E8 67 57 F8 FF     call        00000000670703D0
  00000000670EAC69: 48 8D 57 08        lea         rdx,[rdi+8]
  00000000670EAC6D: 48 8D 4B 28        lea         rcx,[rbx+28h]
  00000000670EAC71: E8 5A 57 F8 FF     call        00000000670703D0
  00000000670EAC76: 44 0F B7 5F 10     movzx       r11d,word ptr [rdi+10h]
  00000000670EAC7B: 66 44 89 5B 30     mov         word ptr [rbx+30h],r11w
  00000000670EAC80: 0F B7 4F 12        movzx       ecx,word ptr [rdi+12h]
  00000000670EAC84: 66 89 4B 32        mov         word ptr [rbx+32h],cx
  00000000670EAC88: 0F B7 47 14        movzx       eax,word ptr [rdi+14h]
  00000000670EAC8C: 66 89 43 34        mov         word ptr [rbx+34h],ax
  00000000670EAC90: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670EAC95: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670EAC99: 75 0B              jne         00000000670EACA6
  00000000670EAC9B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670EACA0: E8 2B 54 F8 FF     call        00000000670700D0
  00000000670EACA5: 90                 nop
  00000000670EACA6: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670EACAB: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EACAF: 75 10              jne         00000000670EACC1
  00000000670EACB1: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670EACB6: E8 15 54 F8 FF     call        00000000670700D0
  00000000670EACBB: 90                 nop
  00000000670EACBC: 48 8B C6           mov         rax,rsi
  00000000670EACBF: EB 03              jmp         00000000670EACC4
  00000000670EACC1: 48 8B C6           mov         rax,rsi
  00000000670EACC4: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670EACC9: 48 8B 74 24 68     mov         rsi,qword ptr [rsp+68h]
  00000000670EACCE: 48 83 C4 40        add         rsp,40h
  00000000670EACD2: 5F                 pop         rdi
  00000000670EACD3: C3                 ret

  Summary

      1A7000 .text

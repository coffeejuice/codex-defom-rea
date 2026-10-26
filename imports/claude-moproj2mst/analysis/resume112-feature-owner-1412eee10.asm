Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412EEE10: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001412EEE15: 57                 push        rdi
  00000001412EEE16: 48 83 EC 30        sub         rsp,30h
  00000001412EEE1A: B9 A0 3E 00 00     mov         ecx,3EA0h
  00000001412EEE1F: E8 98 FC 17 00     call        000000014146EABC
  00000001412EEE24: 48 85 C0           test        rax,rax
  00000001412EEE27: 74 29              je          00000001412EEE52
  00000001412EEE29: 48 8D 0D B8 5E AC  lea         rcx,[0000000142DB4CE8h]
                    01
  00000001412EEE30: C7 44 24 28 01 00  mov         dword ptr [rsp+28h],1
                    00 00
  00000001412EEE38: 45 33 C9           xor         r9d,r9d
  00000001412EEE3B: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  00000001412EEE40: 45 33 C0           xor         r8d,r8d
  00000001412EEE43: 33 D2              xor         edx,edx
  00000001412EEE45: 48 8B C8           mov         rcx,rax
  00000001412EEE48: E8 23 F7 FF FF     call        00000001412EE570
  00000001412EEE4D: 48 8B F8           mov         rdi,rax
  00000001412EEE50: EB 02              jmp         00000001412EEE54
  00000001412EEE52: 33 FF              xor         edi,edi
  00000001412EEE54: 4C 8D 0D A5 5C AC  lea         r9,[0000000142DB4B00h]
                    01
  00000001412EEE5B: 48 8D 0D 2E 10 40  lea         rcx,[00000001446EFE90h]
                    03
  00000001412EEE62: 41 B8 A0 3E 00 00  mov         r8d,3EA0h
  00000001412EEE68: 8B D7              mov         edx,edi
  00000001412EEE6A: C7 44 24 20 7C 11  mov         dword ptr [rsp+20h],117Ch
                    00 00
  00000001412EEE72: E8 F9 66 00 00     call        00000001412F5570
  00000001412EEE77: 33 D2              xor         edx,edx
  00000001412EEE79: 48 8B CF           mov         rcx,rdi
  00000001412EEE7C: C6 87 04 0D 00 00  mov         byte ptr [rdi+0000000000000D04h],0
                    00
  00000001412EEE83: E8 C8 8F 00 00     call        00000001412F7E50
  00000001412EEE88: 48 8B CF           mov         rcx,rdi
  00000001412EEE8B: E8 50 BC FF FF     call        00000001412EAAE0
  00000001412EEE90: 4C 8B 07           mov         r8,qword ptr [rdi]
  00000001412EEE93: BA 01 00 00 00     mov         edx,1
  00000001412EEE98: 48 8B CF           mov         rcx,rdi
  00000001412EEE9B: 8B D8              mov         ebx,eax
  00000001412EEE9D: 41 FF 10           call        qword ptr [r8]
  00000001412EEEA0: 8B C3              mov         eax,ebx
  00000001412EEEA2: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000001412EEEA7: 48 83 C4 30        add         rsp,30h
  00000001412EEEAB: 5F                 pop         rdi
  00000001412EEEAC: C3                 ret

  Summary

     1E02000 .text

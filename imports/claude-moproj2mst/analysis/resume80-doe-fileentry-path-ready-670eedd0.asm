Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670EEDD0: 40 53              push        rbx
  00000000670EEDD2: 48 83 EC 30        sub         rsp,30h
  00000000670EEDD6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670EEDDF: 48 8B D9           mov         rbx,rcx
  00000000670EEDE2: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670EEDE5: 83 78 08 00        cmp         dword ptr [rax+8],0
  00000000670EEDE9: 0F 85 B8 00 00 00  jne         00000000670EEEA7
  00000000670EEDEF: 48 8D 51 08        lea         rdx,[rcx+8]
  00000000670EEDF3: 48 8B 02           mov         rax,qword ptr [rdx]
  00000000670EEDF6: 83 78 08 00        cmp         dword ptr [rax+8],0
  00000000670EEDFA: 0F 84 A7 00 00 00  je          00000000670EEEA7
  00000000670EEE00: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670EEE05: E8 F6 E8 FB FF     call        00000000670AD700
  00000000670EEE0A: 90                 nop
  00000000670EEE0B: 48 8B D0           mov         rdx,rax
  00000000670EEE0E: 48 8B CB           mov         rcx,rbx
  00000000670EEE11: E8 BA 15 F8 FF     call        00000000670703D0
  00000000670EEE16: 90                 nop
  00000000670EEE17: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670EEE1C: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670EEE20: 75 0A              jne         00000000670EEE2C
  00000000670EEE22: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670EEE27: E8 A4 12 F8 FF     call        00000000670700D0
  00000000670EEE2C: 48 8D 05 DD 68 14  lea         rax,[0000000067235710h]
                    00
  00000000670EEE33: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000000670EEE38: 41 B8 01 00 00 00  mov         r8d,1
  00000000670EEE3E: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000000670EEE43: 48 8B CB           mov         rcx,rbx
  00000000670EEE46: E8 B5 F2 F7 FF     call        000000006706E100
  00000000670EEE4B: 84 C0              test        al,al
  00000000670EEE4D: 74 1C              je          00000000670EEE6B
  00000000670EEE4F: BA 02 00 00 00     mov         edx,2
  00000000670EEE54: 44 8D 42 04        lea         r8d,[rdx+4]
  00000000670EEE58: 48 8B CB           mov         rcx,rbx
  00000000670EEE5B: E8 00 40 F8 FF     call        0000000067072E60
  00000000670EEE60: 48 8B D0           mov         rdx,rax
  00000000670EEE63: 48 8B CB           mov         rcx,rbx
  00000000670EEE66: E8 65 15 F8 FF     call        00000000670703D0
  00000000670EEE6B: 48 8D 05 96 68 14  lea         rax,[0000000067235708h]
                    00
  00000000670EEE72: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000000670EEE77: 41 B8 01 00 00 00  mov         r8d,1
  00000000670EEE7D: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000670EEE82: 48 8B CB           mov         rcx,rbx
  00000000670EEE85: E8 76 F2 F7 FF     call        000000006706E100
  00000000670EEE8A: 84 C0              test        al,al
  00000000670EEE8C: 74 19              je          00000000670EEEA7
  00000000670EEE8E: 33 D2              xor         edx,edx
  00000000670EEE90: 44 8D 42 04        lea         r8d,[rdx+4]
  00000000670EEE94: 48 8B CB           mov         rcx,rbx
  00000000670EEE97: E8 C4 3F F8 FF     call        0000000067072E60
  00000000670EEE9C: 48 8B D0           mov         rdx,rax
  00000000670EEE9F: 48 8B CB           mov         rcx,rbx
  00000000670EEEA2: E8 29 15 F8 FF     call        00000000670703D0
  00000000670EEEA7: 48 83 C4 30        add         rsp,30h
  00000000670EEEAB: 5B                 pop         rbx
  00000000670EEEAC: C3                 ret

  Summary

      1A7000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EC98E0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140EC98E5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140EC98EA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140EC98EF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EC98F4: 48 83 EC 48        sub         rsp,48h
  0000000140EC98F8: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140EC9901: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140EC9909: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140EC990E: FF 15 AC AB F3 00  call        qword ptr [0000000141E044C0h]
  0000000140EC9914: 90                 nop
  0000000140EC9915: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140EC991A: 44 8B 00           mov         r8d,dword ptr [rax]
  0000000140EC991D: 48 8D 15 D4 B6 C4  lea         rdx,[0000000142B14FF8h]
                    01
  0000000140EC9924: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140EC9929: FF 15 E9 A5 F3 00  call        qword ptr [0000000141E03F18h]
  0000000140EC992F: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  0000000140EC9934: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140EC9939: FF 15 F9 AA F3 00  call        qword ptr [0000000141E04438h]
  0000000140EC993F: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140EC9944: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140EC9949: FF 15 89 C0 F3 00  call        qword ptr [0000000141E059D8h]
  0000000140EC994F: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140EC9953: 83 C8 01           or          eax,1
  0000000140EC9956: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140EC995A: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140EC995F: FF 15 7B C0 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140EC9965: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  0000000140EC996A: 48 83 C4 48        add         rsp,48h
  0000000140EC996E: C3                 ret
  0000000140EC996F: CC                 int         3

  Summary

     1E02000 .text

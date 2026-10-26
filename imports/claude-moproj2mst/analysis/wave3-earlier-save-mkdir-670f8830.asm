Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670F8830: 40 57              push        rdi
  00000000670F8832: 48 83 EC 40        sub         rsp,40h
  00000000670F8836: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670F883F: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000670F8844: 41 0F B6 D8        movzx       ebx,r8b
  00000000670F8848: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670F884D: E8 BE 64 FF FF     call        00000000670EED10
  00000000670F8852: 90                 nop
  00000000670F8853: 0F B6 D3           movzx       edx,bl
  00000000670F8856: 48 8B C8           mov         rcx,rax
  00000000670F8859: E8 52 66 00 00     call        00000000670FEEB0
  00000000670F885E: 0F B6 F8           movzx       edi,al
  00000000670F8861: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000000670F8866: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  00000000670F886A: 75 0B              jne         00000000670F8877
  00000000670F886C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670F8871: E8 5A 78 F7 FF     call        00000000670700D0
  00000000670F8876: 90                 nop
  00000000670F8877: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670F887C: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F8880: 75 0F              jne         00000000670F8891
  00000000670F8882: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670F8887: E8 44 78 F7 FF     call        00000000670700D0
  00000000670F888C: 40 8A C7           mov         al,dil
  00000000670F888F: EB 03              jmp         00000000670F8894
  00000000670F8891: 40 8A C7           mov         al,dil
  00000000670F8894: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000670F8899: 48 83 C4 40        add         rsp,40h
  00000000670F889D: 5F                 pop         rdi
  00000000670F889E: C3                 ret

  Summary

      1A7000 .text

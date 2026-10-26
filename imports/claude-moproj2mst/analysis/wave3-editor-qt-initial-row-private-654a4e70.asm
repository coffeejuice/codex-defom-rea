Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654A4E70: 40 53              push        rbx
  00000000654A4E72: 48 83 EC 40        sub         rsp,40h
  00000000654A4E76: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000654A4E7A: 48 8B DA           mov         rbx,rdx
  00000000654A4E7D: 33 D2              xor         edx,edx
  00000000654A4E7F: 48 8B 88 A0 02 00  mov         rcx,qword ptr [rax+00000000000002A0h]
                    00
  00000000654A4E86: 89 54 24 50        mov         dword ptr [rsp+50h],edx
  00000000654A4E8A: 48 85 C9           test        rcx,rcx
  00000000654A4E8D: 74 0F              je          00000000654A4E9E
  00000000654A4E8F: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000654A4E94: E8 77 EF 08 00     call        0000000065533E10
  00000000654A4E99: 4C 8B D8           mov         r11,rax
  00000000654A4E9C: EB 1F              jmp         00000000654A4EBD
  00000000654A4E9E: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  00000000654A4EA6: C7 44 24 24 FF FF  mov         dword ptr [rsp+24h],0FFFFFFFFh
                    FF FF
  00000000654A4EAE: 48 89 54 24 28     mov         qword ptr [rsp+28h],rdx
  00000000654A4EB3: 48 89 54 24 30     mov         qword ptr [rsp+30h],rdx
  00000000654A4EB8: 4C 8D 5C 24 20     lea         r11,[rsp+20h]
  00000000654A4EBD: 41 8B 03           mov         eax,dword ptr [r11]
  00000000654A4EC0: 89 03              mov         dword ptr [rbx],eax
  00000000654A4EC2: 41 8B 43 04        mov         eax,dword ptr [r11+4]
  00000000654A4EC6: 89 43 04           mov         dword ptr [rbx+4],eax
  00000000654A4EC9: 49 8B 43 08        mov         rax,qword ptr [r11+8]
  00000000654A4ECD: 48 89 43 08        mov         qword ptr [rbx+8],rax
  00000000654A4ED1: 49 8B 43 10        mov         rax,qword ptr [r11+10h]
  00000000654A4ED5: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  00000000654A4ED9: 48 8B C3           mov         rax,rbx
  00000000654A4EDC: 48 83 C4 40        add         rsp,40h
  00000000654A4EE0: 5B                 pop         rbx
  00000000654A4EE1: C3                 ret

  Summary

      695000 .text

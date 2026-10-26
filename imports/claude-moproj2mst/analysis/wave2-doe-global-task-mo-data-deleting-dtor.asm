
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001401385D0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001401385D4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401385D9: 48 83 EC 28        sub         rsp,28h
  00000001401385DD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001401385E2: E8 29 00 00 00     call        0000000140138610
  00000001401385E7: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001401385EB: 83 E0 01           and         eax,1
  00000001401385EE: 85 C0              test        eax,eax
  00000001401385F0: 74 0A              je          00000001401385FC
  00000001401385F2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001401385F7: E8 6C 76 3A 01     call        00000001414DFC68
  00000001401385FC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140138601: 48 83 C4 28        add         rsp,28h
  0000000140138605: C3                 ret

  Summary

     1E02000 .text


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407C7E60: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001407C7E64: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407C7E69: 48 83 EC 28        sub         rsp,28h
  00000001407C7E6D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407C7E72: E8 69 FB FF FF     call        00000001407C79E0
  00000001407C7E77: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001407C7E7B: 83 E0 01           and         eax,1
  00000001407C7E7E: 85 C0              test        eax,eax
  00000001407C7E80: 74 0A              je          00000001407C7E8C
  00000001407C7E82: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407C7E87: E8 DC 7D D1 00     call        00000001414DFC68
  00000001407C7E8C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001407C7E91: 48 83 C4 28        add         rsp,28h
  00000001407C7E95: C3                 ret

  Summary

     1E02000 .text

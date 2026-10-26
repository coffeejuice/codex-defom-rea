
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407C8210: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001407C8214: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407C8219: 48 83 EC 28        sub         rsp,28h
  00000001407C821D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407C8222: E8 B9 FD FF FF     call        00000001407C7FE0
  00000001407C8227: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001407C822B: 83 E0 01           and         eax,1
  00000001407C822E: 85 C0              test        eax,eax
  00000001407C8230: 74 0A              je          00000001407C823C
  00000001407C8232: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407C8237: E8 2C 7A D1 00     call        00000001414DFC68
  00000001407C823C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001407C8241: 48 83 C4 28        add         rsp,28h
  00000001407C8245: C3                 ret

  Summary

     1E02000 .text

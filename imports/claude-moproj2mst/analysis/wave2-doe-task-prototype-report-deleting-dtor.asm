
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407B9560: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001407B9564: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407B9569: 48 83 EC 28        sub         rsp,28h
  00000001407B956D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407B9572: E8 B9 F4 FF FF     call        00000001407B8A30
  00000001407B9577: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001407B957B: 83 E0 01           and         eax,1
  00000001407B957E: 85 C0              test        eax,eax
  00000001407B9580: 74 0A              je          00000001407B958C
  00000001407B9582: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407B9587: E8 DC 66 D2 00     call        00000001414DFC68
  00000001407B958C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001407B9591: 48 83 C4 28        add         rsp,28h
  00000001407B9595: C3                 ret

  Summary

     1E02000 .text

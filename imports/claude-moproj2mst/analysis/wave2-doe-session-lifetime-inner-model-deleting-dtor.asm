
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140657890: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140657894: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140657899: 48 83 EC 28        sub         rsp,28h
  000000014065789D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001406578A2: E8 79 7C FF FF     call        000000014064F520
  00000001406578A7: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001406578AB: 83 E0 01           and         eax,1
  00000001406578AE: 85 C0              test        eax,eax
  00000001406578B0: 74 0A              je          00000001406578BC
  00000001406578B2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001406578B7: E8 AC 83 E8 00     call        00000001414DFC68
  00000001406578BC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001406578C1: 48 83 C4 28        add         rsp,28h
  00000001406578C5: C3                 ret

  Summary

     1E02000 .text

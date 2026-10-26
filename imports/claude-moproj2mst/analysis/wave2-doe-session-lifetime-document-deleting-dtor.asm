
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014064F060: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014064F064: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014064F069: 48 83 EC 28        sub         rsp,28h
  000000014064F06D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014064F072: E8 79 42 FF FF     call        00000001406432F0
  000000014064F077: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014064F07B: 83 E0 01           and         eax,1
  000000014064F07E: 85 C0              test        eax,eax
  000000014064F080: 74 0A              je          000000014064F08C
  000000014064F082: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014064F087: E8 DC 0B E9 00     call        00000001414DFC68
  000000014064F08C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014064F091: 48 83 C4 28        add         rsp,28h
  000000014064F095: C3                 ret

  Summary

     1E02000 .text


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014065AF80: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014065AF84: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014065AF89: 48 83 EC 28        sub         rsp,28h
  000000014065AF8D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014065AF92: E8 59 D8 FF FF     call        00000001406587F0
  000000014065AF97: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014065AF9B: 83 E0 01           and         eax,1
  000000014065AF9E: 85 C0              test        eax,eax
  000000014065AFA0: 74 0A              je          000000014065AFAC
  000000014065AFA2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014065AFA7: E8 BC 4C E8 00     call        00000001414DFC68
  000000014065AFAC: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000014065AFB1: 48 83 C4 28        add         rsp,28h
  000000014065AFB5: C3                 ret

  Summary

     1E02000 .text

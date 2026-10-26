
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014093A8A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014093A8A5: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  000000014093A8AA: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014093A8AF: F2 0F 10 01        movsd       xmm0,mmword ptr [rcx]
  000000014093A8B3: F2 0F 59 00        mulsd       xmm0,mmword ptr [rax]
  000000014093A8B7: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  000000014093A8BC: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014093A8C1: F2 0F 10 49 08     movsd       xmm1,mmword ptr [rcx+8]
  000000014093A8C6: F2 0F 59 48 08     mulsd       xmm1,mmword ptr [rax+8]
  000000014093A8CB: F2 0F 58 C1        addsd       xmm0,xmm1
  000000014093A8CF: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  000000014093A8D4: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  000000014093A8D9: F2 0F 10 49 10     movsd       xmm1,mmword ptr [rcx+10h]
  000000014093A8DE: F2 0F 59 48 10     mulsd       xmm1,mmword ptr [rax+10h]
  000000014093A8E3: F2 0F 58 C1        addsd       xmm0,xmm1
  000000014093A8E7: C3                 ret
  000000014093A8E8: CC                 int         3
  000000014093A8E9: CC                 int         3
  000000014093A8EA: CC                 int         3
  000000014093A8EB: CC                 int         3
  000000014093A8EC: CC                 int         3
  000000014093A8ED: CC                 int         3
  000000014093A8EE: CC                 int         3
  000000014093A8EF: CC                 int         3
  000000014093A8F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014093A8F5: 48 83 EC 28        sub         rsp,28h
  000000014093A8F9: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014093A8FE: E8 4D FF FF FF     call        000000014093A850
  000000014093A903: 0F 14 C0           unpcklps    xmm0,xmm0
  000000014093A906: 0F 5A C0           cvtps2pd    xmm0,xmm0
  000000014093A909: E8 AE 72 B3 00     call        0000000141471BBC
  000000014093A90E: F2 0F 5A C0        cvtsd2ss    xmm0,xmm0
  000000014093A912: 48 83 C4 28        add         rsp,28h
  000000014093A916: C3                 ret
  000000014093A917: CC                 int         3
  000000014093A918: CC                 int         3
  000000014093A919: CC                 int         3
  000000014093A91A: CC                 int         3
  000000014093A91B: CC                 int         3
  000000014093A91C: CC                 int         3
  000000014093A91D: CC                 int         3
  000000014093A91E: CC                 int         3
  000000014093A91F: CC                 int         3

  Summary

     1E02000 .text

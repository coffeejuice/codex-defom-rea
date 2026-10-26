
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140304800: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140304805: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014030480A: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  000000014030480F: 48 8B 44 24 10     mov         rax,qword ptr [rsp+10h]
  0000000140304814: 0F B6 00           movzx       eax,byte ptr [rax]
  0000000140304817: 88 41 0E           mov         byte ptr [rcx+0Eh],al
  000000014030481A: C3                 ret
  000000014030481B: CC                 int         3
  000000014030481C: CC                 int         3
  000000014030481D: CC                 int         3
  000000014030481E: CC                 int         3
  000000014030481F: CC                 int         3

  Summary

     1E02000 .text


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411FD540: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411FD545: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001411FD54A: 48 C7 80 38 03 00  mov         qword ptr [rax+0000000000000338h],0
                    00 00 00 00 00
  00000001411FD555: C3                 ret
  00000001411FD556: CC                 int         3
  00000001411FD557: CC                 int         3
  00000001411FD558: CC                 int         3
  00000001411FD559: CC                 int         3
  00000001411FD55A: CC                 int         3
  00000001411FD55B: CC                 int         3
  00000001411FD55C: CC                 int         3
  00000001411FD55D: CC                 int         3
  00000001411FD55E: CC                 int         3
  00000001411FD55F: CC                 int         3
  00000001411FD560: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411FD565: 48 83 EC 28        sub         rsp,28h
  00000001411FD569: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001411FD56E: 48 83 B8 38 03 00  cmp         qword ptr [rax+0000000000000338h],0
                    00 00
  00000001411FD576: 75 04              jne         00000001411FD57C
  00000001411FD578: 32 C0              xor         al,al
  00000001411FD57A: EB 11              jmp         00000001411FD58D
  00000001411FD57C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001411FD581: 48 8B 89 38 03 00  mov         rcx,qword ptr [rcx+0000000000000338h]
                    00
  00000001411FD588: E8 33 B4 BF FF     call        0000000140DF89C0
  00000001411FD58D: 48 83 C4 28        add         rsp,28h
  00000001411FD591: C3                 ret

  Summary

     1E02000 .text


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D4F550: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140D4F555: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140D4F55A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140D4F55E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D4F563: 48 83 EC 28        sub         rsp,28h
  0000000140D4F567: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140D4F56C: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140D4F571: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140D4F575: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D4F57A: FF 15 F8 50 0B 01  call        qword ptr [0000000141E04678h]
  0000000140D4F580: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140D4F584: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000140D4F589: 7D 06              jge         0000000140D4F591
  0000000140D4F58B: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D4F58F: EB 35              jmp         0000000140D4F5C6
  0000000140D4F591: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140D4F596: 75 2A              jne         0000000140D4F5C2
  0000000140D4F598: 83 7C 24 40 05     cmp         dword ptr [rsp+40h],5
  0000000140D4F59D: 7D 18              jge         0000000140D4F5B7
  0000000140D4F59F: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140D4F5A4: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140D4F5A9: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140D4F5AD: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D4F5B2: E8 09 FE FF FF     call        0000000140D4F3C0
  0000000140D4F5B7: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D4F5BB: 83 E8 05           sub         eax,5
  0000000140D4F5BE: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140D4F5C2: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D4F5C6: 48 83 C4 28        add         rsp,28h
  0000000140D4F5CA: C3                 ret
  0000000140D4F5CB: CC                 int         3
  0000000140D4F5CC: CC                 int         3

  Summary

     1E02000 .text

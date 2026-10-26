
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140DD6390: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140DD6395: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140DD639A: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140DD639E: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140DD63A3: 48 83 EC 28        sub         rsp,28h
  0000000140DD63A7: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140DD63AC: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140DD63B1: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140DD63B5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DD63BA: FF 15 90 FE 02 01  call        qword ptr [0000000141E06250h]
  0000000140DD63C0: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140DD63C4: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000140DD63C9: 7D 06              jge         0000000140DD63D1
  0000000140DD63CB: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD63CF: EB 35              jmp         0000000140DD6406
  0000000140DD63D1: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140DD63D6: 75 2A              jne         0000000140DD6402
  0000000140DD63D8: 83 7C 24 40 07     cmp         dword ptr [rsp+40h],7
  0000000140DD63DD: 7D 18              jge         0000000140DD63F7
  0000000140DD63DF: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140DD63E4: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140DD63E9: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140DD63ED: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140DD63F2: E8 D9 FC FF FF     call        0000000140DD60D0
  0000000140DD63F7: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD63FB: 83 E8 07           sub         eax,7
  0000000140DD63FE: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140DD6402: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140DD6406: 48 83 C4 28        add         rsp,28h
  0000000140DD640A: C3                 ret

  Summary

     1E02000 .text

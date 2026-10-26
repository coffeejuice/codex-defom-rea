
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CC75A0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140CC75A5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140CC75AA: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140CC75AE: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CC75B3: 48 83 EC 28        sub         rsp,28h
  0000000140CC75B7: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140CC75BC: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140CC75C1: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140CC75C5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CC75CA: FF 15 08 23 14 01  call        qword ptr [0000000141E098D8h]
  0000000140CC75D0: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140CC75D4: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  0000000140CC75D9: 7D 06              jge         0000000140CC75E1
  0000000140CC75DB: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140CC75DF: EB 3A              jmp         0000000140CC761B
  0000000140CC75E1: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140CC75E6: 75 2F              jne         0000000140CC7617
  0000000140CC75E8: 81 7C 24 40 B4 00  cmp         dword ptr [rsp+40h],0B4h
                    00 00
  0000000140CC75F0: 7D 18              jge         0000000140CC760A
  0000000140CC75F2: 4C 8B 4C 24 48     mov         r9,qword ptr [rsp+48h]
  0000000140CC75F7: 44 8B 44 24 40     mov         r8d,dword ptr [rsp+40h]
  0000000140CC75FC: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140CC7600: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CC7605: E8 66 E3 FF FF     call        0000000140CC5970
  0000000140CC760A: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140CC760E: 2D B4 00 00 00     sub         eax,0B4h
  0000000140CC7613: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140CC7617: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]

  Summary

     1E02000 .text

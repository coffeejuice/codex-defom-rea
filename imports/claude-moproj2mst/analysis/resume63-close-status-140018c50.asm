
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140018C50: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140018C55: 48 83 EC 48        sub         rsp,48h
  0000000140018C59: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140018C62: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140018C67: 48 83 B8 E8 03 00  cmp         qword ptr [rax+00000000000003E8h],0
                    00 00
  0000000140018C6F: 75 4E              jne         0000000140018CBF
  0000000140018C71: B9 28 00 00 00     mov         ecx,28h
  0000000140018C76: E8 41 5E 45 01     call        000000014146EABC
  0000000140018C7B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140018C80: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140018C86: 74 13              je          0000000140018C9B
  0000000140018C88: 33 D2              xor         edx,edx
  0000000140018C8A: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140018C8F: E8 BC C3 CF 00     call        0000000140D15050
  0000000140018C94: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140018C99: EB 09              jmp         0000000140018CA4
  0000000140018C9B: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140018CA4: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140018CA9: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140018CAE: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140018CB3: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140018CB8: 48 89 81 E8 03 00  mov         qword ptr [rcx+00000000000003E8h],rax
                    00
  0000000140018CBF: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140018CC4: 48 8B 80 E8 03 00  mov         rax,qword ptr [rax+00000000000003E8h]
                    00
  0000000140018CCB: 48 83 C4 48        add         rsp,48h
  0000000140018CCF: C3                 ret

  Summary

     1E02000 .text

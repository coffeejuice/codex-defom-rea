
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CA6240: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CA6245: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CA624A: 48 83 EC 28        sub         rsp,28h
  0000000140CA624E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140CA6253: 48 81 C1 48 01 00  add         rcx,148h
                    00
  0000000140CA625A: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  0000000140CA625F: E8 BC 66 01 00     call        0000000140CBC920
  0000000140CA6264: 48 83 C4 28        add         rsp,28h
  0000000140CA6268: C3                 ret
  0000000140CA6269: CC                 int         3
  0000000140CA626A: CC                 int         3
  0000000140CA626B: CC                 int         3
  0000000140CA626C: CC                 int         3
  0000000140CA626D: CC                 int         3
  0000000140CA626E: CC                 int         3
  0000000140CA626F: CC                 int         3
  0000000140CA6270: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CA6275: 48 83 EC 48        sub         rsp,48h
  0000000140CA6279: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140CA6282: B9 10 00 00 00     mov         ecx,10h
  0000000140CA6287: E8 30 88 7C 00     call        000000014146EABC
  0000000140CA628C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CA6291: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140CA6297: 74 13              je          0000000140CA62AC
  0000000140CA6299: 33 D2              xor         edx,edx
  0000000140CA629B: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140CA62A0: E8 DB 41 0E 00     call        0000000140D8A480
  0000000140CA62A5: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CA62AA: EB 09              jmp         0000000140CA62B5
  0000000140CA62AC: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140CA62B5: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140CA62BA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CA62BF: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CA62C4: 48 81 C1 50 01 00  add         rcx,150h
                    00
  0000000140CA62CB: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140CA62D0: E8 4B 66 01 00     call        0000000140CBC920
  0000000140CA62D5: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140CA62DA: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CA62DD: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CA62E2: FF 90 D8 07 00 00  call        qword ptr [rax+00000000000007D8h]
  0000000140CA62E8: 48 83 C4 48        add         rsp,48h
  0000000140CA62EC: C3                 ret

  Summary

     1E02000 .text

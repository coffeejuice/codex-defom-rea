
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407B8A90: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407B8A95: 48 83 EC 48        sub         rsp,48h
  00000001407B8A99: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001407B8AA2: B9 10 00 00 00     mov         ecx,10h
  00000001407B8AA7: E8 10 60 CB 00     call        000000014146EABC
  00000001407B8AAC: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001407B8AB1: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001407B8AB7: 74 11              je          00000001407B8ACA
  00000001407B8AB9: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001407B8ABE: E8 DD 2C 00 00     call        00000001407BB7A0
  00000001407B8AC3: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001407B8AC8: EB 09              jmp         00000001407B8AD3
  00000001407B8ACA: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001407B8AD3: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001407B8AD8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001407B8ADD: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001407B8AE2: 48 83 C4 48        add         rsp,48h
  00000001407B8AE6: C3                 ret

  Summary

     1E02000 .text

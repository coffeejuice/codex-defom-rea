
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014064BE50: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014064BE55: 48 83 EC 48        sub         rsp,48h
  000000014064BE59: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014064BE5E: 48 83 78 48 00     cmp         qword ptr [rax+48h],0
  000000014064BE63: 74 4C              je          000000014064BEB1
  000000014064BE65: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014064BE6A: 48 8B 40 48        mov         rax,qword ptr [rax+48h]
  000000014064BE6E: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014064BE73: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000014064BE78: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014064BE7D: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  000000014064BE83: 74 16              je          000000014064BE9B
  000000014064BE85: BA 01 00 00 00     mov         edx,1
  000000014064BE8A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000014064BE8F: E8 2C 6D C5 FF     call        00000001402A2BC0
  000000014064BE94: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014064BE99: EB 09              jmp         000000014064BEA4
  000000014064BE9B: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  000000014064BEA4: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014064BEA9: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  000000014064BEB1: 48 83 C4 48        add         rsp,48h
  000000014064BEB5: C3                 ret

  Summary

     1E02000 .text

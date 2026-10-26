
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014140E090: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014140E095: 48 83 EC 48        sub         rsp,48h
  000000014140E099: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014140E0A2: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014140E0A7: 48 8D 05 8A 5F A8  lea         rax,[0000000142E94038h]
                    01
  000000014140E0AE: 48 89 01           mov         qword ptr [rcx],rax
  000000014140E0B1: B9 80 00 00 00     mov         ecx,80h
  000000014140E0B6: E8 01 0A 06 00     call        000000014146EABC
  000000014140E0BB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014140E0C0: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  000000014140E0C6: 74 11              je          000000014140E0D9
  000000014140E0C8: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014140E0CD: E8 3E 08 00 00     call        000000014140E910
  000000014140E0D2: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014140E0D7: EB 09              jmp         000000014140E0E2
  000000014140E0D9: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  000000014140E0E2: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014140E0E7: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014140E0EC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014140E0F1: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014140E0F6: 48 89 41 08        mov         qword ptr [rcx+8],rax
  000000014140E0FA: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014140E0FF: 48 83 C4 48        add         rsp,48h
  000000014140E103: C3                 ret

  Summary

     1E02000 .text

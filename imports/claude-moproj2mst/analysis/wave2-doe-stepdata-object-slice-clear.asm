
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411FD600: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411FD605: 48 83 EC 48        sub         rsp,48h
  00000001411FD609: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001411FD612: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001411FD617: 48 83 B8 38 03 00  cmp         qword ptr [rax+0000000000000338h],0
                    00 00
  00000001411FD61F: 75 51              jne         00000001411FD672
  00000001411FD621: B9 20 40 00 00     mov         ecx,4020h
  00000001411FD626: E8 91 14 27 00     call        000000014146EABC
  00000001411FD62B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001411FD630: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001411FD636: 74 16              je          00000001411FD64E
  00000001411FD638: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  00000001411FD63D: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001411FD642: E8 C9 AA BF FF     call        0000000140DF8110
  00000001411FD647: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001411FD64C: EB 09              jmp         00000001411FD657
  00000001411FD64E: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001411FD657: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001411FD65C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411FD661: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411FD666: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001411FD66B: 48 89 81 38 03 00  mov         qword ptr [rcx+0000000000000338h],rax
                    00
  00000001411FD672: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001411FD677: 48 8B 80 38 03 00  mov         rax,qword ptr [rax+0000000000000338h]
                    00
  00000001411FD67E: 48 83 C4 48        add         rsp,48h
  00000001411FD682: C3                 ret

  Summary

     1E02000 .text

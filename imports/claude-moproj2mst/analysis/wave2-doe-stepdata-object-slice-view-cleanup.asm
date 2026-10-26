
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411FDC40: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411FDC45: 48 83 EC 48        sub         rsp,48h
  00000001411FDC49: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411FDC4E: E8 2D 3D FF FF     call        00000001411F1980
  00000001411FDC53: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411FDC58: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001411FDC5D: 48 83 B8 38 03 00  cmp         qword ptr [rax+0000000000000338h],0
                    00 00
  00000001411FDC65: 74 63              je          00000001411FDCCA
  00000001411FDC67: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411FDC6C: 48 8B 89 38 03 00  mov         rcx,qword ptr [rcx+0000000000000338h]
                    00
  00000001411FDC73: E8 E8 A7 BF FF     call        0000000140DF8460
  00000001411FDC78: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001411FDC7D: 48 8B 80 38 03 00  mov         rax,qword ptr [rax+0000000000000338h]
                    00
  00000001411FDC84: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001411FDC89: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001411FDC8E: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001411FDC93: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001411FDC99: 74 16              je          00000001411FDCB1
  00000001411FDC9B: BA 01 00 00 00     mov         edx,1
  00000001411FDCA0: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001411FDCA5: E8 B6 05 36 FF     call        000000014055E260
  00000001411FDCAA: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001411FDCAF: EB 09              jmp         00000001411FDCBA
  00000001411FDCB1: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001411FDCBA: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001411FDCBF: 48 C7 80 38 03 00  mov         qword ptr [rax+0000000000000338h],0
                    00 00 00 00 00
  00000001411FDCCA: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001411FDCD0: 74 0A              je          00000001411FDCDC
  00000001411FDCD2: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001411FDCD7: E8 84 28 A8 FF     call        0000000140C80560
  00000001411FDCDC: 48 83 C4 48        add         rsp,48h
  00000001411FDCE0: C3                 ret

  Summary

     1E02000 .text

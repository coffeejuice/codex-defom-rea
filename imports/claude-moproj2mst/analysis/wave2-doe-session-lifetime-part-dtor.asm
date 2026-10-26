
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001406587F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001406587F5: 48 83 EC 48        sub         rsp,48h
  00000001406587F9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140658802: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140658807: 48 8D 05 FA 36 E9  lea         rax,[00000001424EBF08h]
                    01
  000000014065880E: 48 89 01           mov         qword ptr [rcx],rax
  0000000140658811: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140658816: E8 85 01 00 00     call        00000001406589A0
  000000014065881B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140658820: 48 83 C1 28        add         rcx,28h
  0000000140658824: E8 C7 26 5C 00     call        0000000140C1AEF0
  0000000140658829: 48 85 C0           test        rax,rax
  000000014065882C: 74 5B              je          0000000140658889
  000000014065882E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140658833: 48 83 C1 28        add         rcx,28h
  0000000140658837: E8 B4 26 5C 00     call        0000000140C1AEF0
  000000014065883C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140658841: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140658846: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014065884B: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140658851: 74 1C              je          000000014065886F
  0000000140658853: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140658858: 48 8B 00           mov         rax,qword ptr [rax]
  000000014065885B: BA 01 00 00 00     mov         edx,1
  0000000140658860: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140658865: FF 50 18           call        qword ptr [rax+18h]
  0000000140658868: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014065886D: EB 09              jmp         0000000140658878
  000000014065886F: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140658878: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014065887D: 48 83 C1 28        add         rcx,28h
  0000000140658881: 33 D2              xor         edx,edx
  0000000140658883: E8 98 40 66 00     call        0000000140CBC920
  0000000140658888: 90                 nop
  0000000140658889: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014065888E: 48 83 C1 38        add         rcx,38h
  0000000140658892: E8 99 30 00 00     call        000000014065B930
  0000000140658897: 90                 nop
  0000000140658898: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014065889D: 48 83 C1 30        add         rcx,30h
  00000001406588A1: E8 5A 40 66 00     call        0000000140CBC900
  00000001406588A6: 90                 nop
  00000001406588A7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001406588AC: 48 83 C1 28        add         rcx,28h
  00000001406588B0: E8 4B 40 66 00     call        0000000140CBC900
  00000001406588B5: 90                 nop
  00000001406588B6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001406588BB: E8 30 26 00 00     call        000000014065AEF0
  00000001406588C0: 48 83 C4 48        add         rsp,48h
  00000001406588C4: C3                 ret

  Summary

     1E02000 .text

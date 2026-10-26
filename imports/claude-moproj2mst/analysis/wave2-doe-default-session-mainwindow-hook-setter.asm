
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CA6110: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CA6115: 53                 push        rbx
  0000000140CA6116: 48 83 EC 40        sub         rsp,40h
  0000000140CA611A: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140CA6123: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CA6128: 48 81 C1 40 01 00  add         rcx,140h
                    00
  0000000140CA612F: E8 BC 4D F7 FF     call        0000000140C1AEF0
  0000000140CA6134: 48 85 C0           test        rax,rax
  0000000140CA6137: 74 05              je          0000000140CA613E
  0000000140CA6139: E9 B8 00 00 00     jmp         0000000140CA61F6
  0000000140CA613E: B9 38 00 00 00     mov         ecx,38h
  0000000140CA6143: E8 74 89 7C 00     call        000000014146EABC
  0000000140CA6148: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CA614D: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  0000000140CA6153: 74 13              je          0000000140CA6168
  0000000140CA6155: 33 D2              xor         edx,edx
  0000000140CA6157: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140CA615C: E8 BF 3E 0E 00     call        0000000140D8A020
  0000000140CA6161: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CA6166: EB 09              jmp         0000000140CA6171
  0000000140CA6168: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  0000000140CA6171: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140CA6176: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CA617B: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CA6180: 48 81 C1 40 01 00  add         rcx,140h
                    00
  0000000140CA6187: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140CA618C: E8 8F 67 01 00     call        0000000140CBC920
  0000000140CA6191: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CA6196: 48 81 C1 40 01 00  add         rcx,140h
                    00
  0000000140CA619D: E8 4E 4D F7 FF     call        0000000140C1AEF0
  0000000140CA61A2: 33 D2              xor         edx,edx
  0000000140CA61A4: 48 8B C8           mov         rcx,rax
  0000000140CA61A7: FF 15 03 37 16 01  call        qword ptr [0000000141E098B0h]
  0000000140CA61AD: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CA61B2: 48 81 C1 40 01 00  add         rcx,140h
                    00
  0000000140CA61B9: E8 32 4D F7 FF     call        0000000140C1AEF0
  0000000140CA61BE: 48 8B C8           mov         rcx,rax
  0000000140CA61C1: E8 EA 40 0E 00     call        0000000140D8A2B0
  0000000140CA61C6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CA61CB: 48 81 C1 28 01 00  add         rcx,128h
                    00
  0000000140CA61D2: E8 19 4D F7 FF     call        0000000140C1AEF0
  0000000140CA61D7: 48 8B D8           mov         rbx,rax
  0000000140CA61DA: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140CA61DF: 48 81 C1 40 01 00  add         rcx,140h
                    00
  0000000140CA61E6: E8 05 4D F7 FF     call        0000000140C1AEF0
  0000000140CA61EB: 48 8B D3           mov         rdx,rbx
  0000000140CA61EE: 48 8B C8           mov         rcx,rax
  0000000140CA61F1: E8 4A 3F 0E 00     call        0000000140D8A140
  0000000140CA61F6: 48 83 C4 40        add         rsp,40h
  0000000140CA61FA: 5B                 pop         rbx
  0000000140CA61FB: C3                 ret

  Summary

     1E02000 .text


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407C8060: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407C8065: 48 83 EC 58        sub         rsp,58h
  00000001407C8069: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001407C806E: 48 83 C1 10        add         rcx,10h
  00000001407C8072: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001407C8077: E8 14 0E BD 00     call        0000000141398E90
  00000001407C807C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001407C8081: 48 83 C1 10        add         rcx,10h
  00000001407C8085: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001407C808A: E8 41 FB C0 00     call        00000001413D7BD0
  00000001407C808F: 48 8B D0           mov         rdx,rax
  00000001407C8092: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407C8097: E8 D4 E3 95 FF     call        0000000140126470
  00000001407C809C: 0F B6 C0           movzx       eax,al
  00000001407C809F: 85 C0              test        eax,eax
  00000001407C80A1: 74 6A              je          00000001407C810D
  00000001407C80A3: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407C80A8: E8 63 20 BB 00     call        000000014137A110
  00000001407C80AD: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407C80B0: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001407C80B5: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001407C80BB: 74 41              je          00000001407C80FE
  00000001407C80BD: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000001407C80C2: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001407C80C7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001407C80CC: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001407C80D1: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001407C80D7: 74 1C              je          00000001407C80F5
  00000001407C80D9: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001407C80DE: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407C80E1: BA 01 00 00 00     mov         edx,1
  00000001407C80E6: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001407C80EB: FF 50 18           call        qword ptr [rax+18h]
  00000001407C80EE: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001407C80F3: EB 09              jmp         00000001407C80FE
  00000001407C80F5: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  00000001407C80FE: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407C8103: E8 68 0D BD 00     call        0000000141398E70
  00000001407C8108: E9 6F FF FF FF     jmp         00000001407C807C
  00000001407C810D: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001407C8112: 48 83 C1 10        add         rcx,10h
  00000001407C8116: E8 55 88 8A FF     call        0000000140070970
  00000001407C811B: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001407C8120: 48 83 C1 08        add         rcx,8
  00000001407C8124: E8 47 88 8A FF     call        0000000140070970
  00000001407C8129: 48 83 C4 58        add         rsp,58h
  00000001407C812D: C3                 ret

  Summary

     1E02000 .text

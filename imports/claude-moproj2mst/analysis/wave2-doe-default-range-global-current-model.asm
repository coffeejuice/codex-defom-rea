
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402C80F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402C80F5: 48 83 EC 28        sub         rsp,28h
  00000001402C80F9: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402C80FE: 48 83 38 00        cmp         qword ptr [rax],0
  00000001402C8102: 75 04              jne         00000001402C8108
  00000001402C8104: 33 C0              xor         eax,eax
  00000001402C8106: EB 19              jmp         00000001402C8121
  00000001402C8108: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  00000001402C810D: 48 8B 00           mov         rax,qword ptr [rax]
  00000001402C8110: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001402C8115: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000001402C8118: 48 8B 00           mov         rax,qword ptr [rax]
  00000001402C811B: FF 90 68 02 00 00  call        qword ptr [rax+0000000000000268h]
  00000001402C8121: 48 83 C4 28        add         rsp,28h
  00000001402C8125: C3                 ret

  Summary

     1E02000 .text

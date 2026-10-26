Bounded extraction from retained DUMPBIN captures; target SHA-256 fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc.
Factory virtual caller c36b80 dispatch and return:
  0000000140C36B80: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C36B85: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C36B8A: 48 83 EC 38        sub         rsp,38h
  0000000140C36B8E: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
  0000000140C36B97: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140C36B9C: E8 8F 9B 68 00     call        00000001412C0730
  0000000140C36BA1: 48 8B D0           mov         rdx,rax
  0000000140C36BA4: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C36BA9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C36BAC: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140C36BB1: FF 50 58           call        qword ptr [rax+58h]
  0000000140C36BB4: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C36BB9: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140C36BBE: 48 83 C4 38        add         rsp,38h
  0000000140C36BC2: C3                 ret

Histogram construction function c36bd0: item construction return-value staging and function epilogue:
  0000000140C36D9D: E8 0E 49 FF FF     call        0000000140C2B6B0
  0000000140C36DA2: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
  0000000140C36DAA: EB 0C              jmp         0000000140C36DB8
  0000000140C36DB8: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
  0000000140C36DC5: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140C36DCF: E9 B9 00 00 00     jmp         0000000140C36E8D
  0000000140C36E55: E8 56 48 FF FF     call        0000000140C2B6B0
  0000000140C36E5A: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
  0000000140C36E62: EB 0C              jmp         0000000140C36E70
  0000000140C36E70: 48 8B 84 24 D8 00  mov         rax,qword ptr [rsp+00000000000000D8h]
  0000000140C36E80: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
  0000000140C36E88: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C3711F: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140C37124: 48 81 C4 08 01 00  add         rsp,108h
  0000000140C3712B: 5F                 pop         rdi
  0000000140C3712C: 5B                 pop         rbx
  0000000140C3712D: C3                 ret

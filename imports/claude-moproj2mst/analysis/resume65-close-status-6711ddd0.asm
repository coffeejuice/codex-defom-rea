
Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711DDD0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006711DDD5: 57                 push        rdi
  000000006711DDD6: 48 83 EC 20        sub         rsp,20h
  000000006711DDDA: 48 8B FA           mov         rdi,rdx
  000000006711DDDD: 48 8B D9           mov         rbx,rcx
  000000006711DDE0: 48 85 D2           test        rdx,rdx
  000000006711DDE3: 74 18              je          000000006711DDFD
  000000006711DDE5: 48 8B 02           mov         rax,qword ptr [rdx]
  000000006711DDE8: 48 8B CA           mov         rcx,rdx
  000000006711DDEB: FF 10              call        qword ptr [rax]
  000000006711DDED: 0F 1F 00           nop         dword ptr [rax]
  000000006711DDF0: 48 3B C3           cmp         rax,rbx
  000000006711DDF3: 74 15              je          000000006711DE0A
  000000006711DDF5: 48 8B 00           mov         rax,qword ptr [rax]
  000000006711DDF8: 48 85 C0           test        rax,rax
  000000006711DDFB: 75 F3              jne         000000006711DDF0
  000000006711DDFD: 33 C0              xor         eax,eax
  000000006711DDFF: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006711DE04: 48 83 C4 20        add         rsp,20h
  000000006711DE08: 5F                 pop         rdi
  000000006711DE09: C3                 ret
  000000006711DE0A: 48 8B C7           mov         rax,rdi
  000000006711DE0D: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006711DE12: 48 83 C4 20        add         rsp,20h
  000000006711DE16: 5F                 pop         rdi
  000000006711DE17: C3                 ret

  Summary

      1A7000 .text


Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065406AC0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000065406AC5: 57                 push        rdi
  0000000065406AC6: 48 83 EC 20        sub         rsp,20h
  0000000065406ACA: 8B DA              mov         ebx,edx
  0000000065406ACC: 48 8B F9           mov         rdi,rcx
  0000000065406ACF: FF 15 A3 11 29 00  call        qword ptr [0000000065697C78h]
  0000000065406AD5: F6 C3 01           test        bl,1
  0000000065406AD8: 74 08              je          0000000065406AE2
  0000000065406ADA: 48 8B CF           mov         rcx,rdi
  0000000065406ADD: E8 CC A5 1D 00     call        00000000655E10AE
  0000000065406AE2: 48 8B C7           mov         rax,rdi
  0000000065406AE5: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065406AEA: 48 83 C4 20        add         rsp,20h
  0000000065406AEE: 5F                 pop         rdi
  0000000065406AEF: C3                 ret

  Summary

      695000 .text

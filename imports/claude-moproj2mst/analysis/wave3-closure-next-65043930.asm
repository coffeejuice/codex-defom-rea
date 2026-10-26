Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065043930: 40 53              push        rbx
  0000000065043932: 48 83 EC 30        sub         rsp,30h
  0000000065043936: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006504393F: 48 8B DA           mov         rbx,rdx
  0000000065043942: 48 8B 05 97 5F 98  mov         rax,qword ptr [00000000659C98E0h]
                    00
  0000000065043949: 48 85 C0           test        rax,rax
  000000006504394C: 74 07              je          0000000065043955
  000000006504394E: FF D0              call        rax
  0000000065043950: 48 85 C0           test        rax,rax
  0000000065043953: 75 23              jne         0000000065043978
  0000000065043955: B9 58 00 00 00     mov         ecx,58h
  000000006504395A: E8 13 D8 59 00     call        00000000655E1172
  000000006504395F: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000065043964: 48 85 C0           test        rax,rax
  0000000065043967: 74 0D              je          0000000065043976
  0000000065043969: 48 8B D3           mov         rdx,rbx
  000000006504396C: 48 8B C8           mov         rcx,rax
  000000006504396F: E8 3C 5B 00 00     call        00000000650494B0
  0000000065043974: EB 02              jmp         0000000065043978
  0000000065043976: 33 C0              xor         eax,eax
  0000000065043978: 48 83 C4 30        add         rsp,30h
  000000006504397C: 5B                 pop         rbx
  000000006504397D: C3                 ret

  Summary

      695000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670038F0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000670038F5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000670038FA: 57                 push        rdi
  00000000670038FB: 48 83 EC 20        sub         rsp,20h
  00000000670038FF: 48 8B F1           mov         rsi,rcx
  0000000067003902: 48 8B CA           mov         rcx,rdx
  0000000067003905: 48 8B FA           mov         rdi,rdx
  0000000067003908: E8 33 5A 04 00     call        0000000067049340
  000000006700390D: 48 8B CE           mov         rcx,rsi
  0000000067003910: 48 8B D8           mov         rbx,rax
  0000000067003913: E8 28 5A 04 00     call        0000000067049340
  0000000067003918: 48 3B C3           cmp         rax,rbx
  000000006700391B: 75 32              jne         000000006700394F
  000000006700391D: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000067003922: 48 8B CE           mov         rcx,rsi
  0000000067003925: E8 16 6A 04 00     call        000000006704A340
  000000006700392A: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006700392F: 48 8B CF           mov         rcx,rdi
  0000000067003932: 8B 18              mov         ebx,dword ptr [rax]
  0000000067003934: E8 07 6A 04 00     call        000000006704A340
  0000000067003939: 3B 18              cmp         ebx,dword ptr [rax]
  000000006700393B: 75 12              jne         000000006700394F
  000000006700393D: B0 01              mov         al,1
  000000006700393F: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000067003944: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000067003949: 48 83 C4 20        add         rsp,20h
  000000006700394D: 5F                 pop         rdi
  000000006700394E: C3                 ret
  000000006700394F: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000067003954: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000067003959: 32 C0              xor         al,al
  000000006700395B: 48 83 C4 20        add         rsp,20h
  000000006700395F: 5F                 pop         rdi
  0000000067003960: C3                 ret

  Summary

      1A7000 .text

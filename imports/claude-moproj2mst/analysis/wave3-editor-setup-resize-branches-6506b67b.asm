Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006506B67B: 48 83 B8 88 00 00  cmp         qword ptr [rax+0000000000000088h],0
                    00 00
  000000006506B683: 74 1E              je          000000006506B6A3
  000000006506B685: 48 8B 80 88 00 00  mov         rax,qword ptr [rax+0000000000000088h]
                    00
  000000006506B68C: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006506B694: 44 8B 40 44        mov         r8d,dword ptr [rax+44h]
  000000006506B698: 8B 50 40           mov         edx,dword ptr [rax+40h]
  000000006506B69B: FF 15 7F B3 62 00  call        qword ptr [0000000065696A20h]
  000000006506B6A1: EB 10              jmp         000000006506B6B3
  000000006506B6A3: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006506B6A8: 45 33 C0           xor         r8d,r8d
  000000006506B6AB: 33 D2              xor         edx,edx
  000000006506B6AD: FF 15 6D B3 62 00  call        qword ptr [0000000065696A20h]
  000000006506B6B3: 48 8B 00           mov         rax,qword ptr [rax]
  000000006506B6B6: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  000000006506B6BE: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006506B6C2: 48 83 B8 88 00 00  cmp         qword ptr [rax+0000000000000088h],0
                    00 00
  000000006506B6CA: 74 15              je          000000006506B6E1
  000000006506B6CC: 48 8B 80 88 00 00  mov         rax,qword ptr [rax+0000000000000088h]
                    00
  000000006506B6D3: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006506B6D8: 44 8B 40 4C        mov         r8d,dword ptr [rax+4Ch]
  000000006506B6DC: 8B 50 48           mov         edx,dword ptr [rax+48h]
  000000006506B6DF: EB 0D              jmp         000000006506B6EE
  000000006506B6E1: BA FF FF FF 00     mov         edx,0FFFFFFh
  000000006506B6E6: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006506B6EB: 44 8B C2           mov         r8d,edx
  000000006506B6EE: FF 15 2C B3 62 00  call        qword ptr [0000000065696A20h]
  000000006506B6F4: 4C 8D 84 24 90 00  lea         r8,[rsp+0000000000000090h]
                    00 00
  000000006506B6FC: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006506B701: 48 8B 00           mov         rax,qword ptr [rax]
  000000006506B704: 48 8B CF           mov         rcx,rdi
  000000006506B707: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  000000006506B70F: FF 15 5B B7 62 00  call        qword ptr [0000000065696E70h]
  000000006506B715: 4C 8D 84 24 A0 00  lea         r8,[rsp+00000000000000A0h]
                    00 00
  000000006506B71D: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006506B722: 48 8B C8           mov         rcx,rax
  000000006506B725: FF 15 4D B7 62 00  call        qword ptr [0000000065696E78h]
  000000006506B72B: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  000000006506B72F: 48 83 C1 14        add         rcx,14h
  000000006506B733: 48 8B D0           mov         rdx,rax
  000000006506B736: FF 15 84 BB 62 00  call        qword ptr [00000000656972C0h]
  000000006506B73C: 41 B0 01           mov         r8b,1
  000000006506B73F: BA 23 00 00 00     mov         edx,23h
  000000006506B744: 48 8B CB           mov         rcx,rbx
  000000006506B747: 48 83 C4 70        add         rsp,70h
  000000006506B74B: 41 5C              pop         r12
  000000006506B74D: 5F                 pop         rdi
  000000006506B74E: 5B                 pop         rbx
  000000006506B74F: E9 8C D8 FF FF     jmp         0000000065068FE0

  Summary

      695000 .text

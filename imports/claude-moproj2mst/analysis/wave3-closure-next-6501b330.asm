Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006501B330: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006501B335: 53                 push        rbx
  000000006501B336: 56                 push        rsi
  000000006501B337: 57                 push        rdi
  000000006501B338: 48 83 EC 50        sub         rsp,50h
  000000006501B33C: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006501B345: 48 8B F9           mov         rdi,rcx
  000000006501B348: 33 DB              xor         ebx,ebx
  000000006501B34A: 89 5C 24 78        mov         dword ptr [rsp+78h],ebx
  000000006501B34E: E8 FD FE FF FF     call        000000006501B250
  000000006501B353: 48 8B D0           mov         rdx,rax
  000000006501B356: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000006501B35E: FF 15 AC B8 67 00  call        qword ptr [0000000065696C10h]
  000000006501B364: 90                 nop
  000000006501B365: 48 8B 05 4C E3 9A  mov         rax,qword ptr [00000000659C96B8h]
                    00
  000000006501B36C: 48 85 C0           test        rax,rax
  000000006501B36F: 0F 85 8B 00 00 00  jne         000000006501B400
  000000006501B375: 8D 4B 10           lea         ecx,[rbx+10h]
  000000006501B378: E8 F5 5D 5C 00     call        00000000655E1172
  000000006501B37D: 48 8B F0           mov         rsi,rax
  000000006501B380: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000006501B385: 48 85 C0           test        rax,rax
  000000006501B388: 74 4B              je          000000006501B3D5
  000000006501B38A: 48 8D 15 B7 33 68  lea         rdx,[000000006569E748h]
                    00
  000000006501B391: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006501B396: FF 15 44 B6 67 00  call        qword ptr [00000000656969E0h]
  000000006501B39C: 48 8B D0           mov         rdx,rax
  000000006501B39F: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006501B3A7: FF 15 5B B8 67 00  call        qword ptr [0000000065696C08h]
  000000006501B3AD: 90                 nop
  000000006501B3AE: BB 01 00 00 00     mov         ebx,1
  000000006501B3B3: 89 5C 24 78        mov         dword ptr [rsp+78h],ebx
  000000006501B3B7: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000006501B3BC: 41 83 C9 FF        or          r9d,0FFFFFFFFh
  000000006501B3C0: 45 0B C1           or          r8d,r9d
  000000006501B3C3: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000006501B3CB: 48 8B CE           mov         rcx,rsi
  000000006501B3CE: E8 7D 7B 1D 00     call        00000000651F2F50
  000000006501B3D3: EB 03              jmp         000000006501B3D8
  000000006501B3D5: 48 8B C3           mov         rax,rbx
  000000006501B3D8: 48 89 05 D9 E2 9A  mov         qword ptr [00000000659C96B8h],rax
                    00
  000000006501B3DF: F6 C3 01           test        bl,1
  000000006501B3E2: 74 1C              je          000000006501B400
  000000006501B3E4: 83 E3 FE           and         ebx,0FFFFFFFEh
  000000006501B3E7: 89 5C 24 78        mov         dword ptr [rsp+78h],ebx
  000000006501B3EB: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006501B3F3: FF 15 67 B3 67 00  call        qword ptr [0000000065696760h]
  000000006501B3F9: 48 8B 05 B8 E2 9A  mov         rax,qword ptr [00000000659C96B8h]
                    00
  000000006501B400: 48 8B D0           mov         rdx,rax
  000000006501B403: 48 8B CF           mov         rcx,rdi
  000000006501B406: E8 B5 DC 21 00     call        00000000652390C0
  000000006501B40B: 83 CB 02           or          ebx,2
  000000006501B40E: 89 5C 24 78        mov         dword ptr [rsp+78h],ebx
  000000006501B412: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000006501B41A: FF 15 E0 B7 67 00  call        qword ptr [0000000065696C00h]
  000000006501B420: 48 8B C7           mov         rax,rdi
  000000006501B423: 48 83 C4 50        add         rsp,50h
  000000006501B427: 5F                 pop         rdi
  000000006501B428: 5E                 pop         rsi
  000000006501B429: 5B                 pop         rbx
  000000006501B42A: C3                 ret
  000000006501B42B: CC                 int         3

  Summary

      695000 .text

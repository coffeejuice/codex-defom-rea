Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006503F7C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006503F7C5: 53                 push        rbx
  000000006503F7C6: 48 83 EC 30        sub         rsp,30h
  000000006503F7CA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006503F7D3: 48 8B D9           mov         rbx,rcx
  000000006503F7D6: B9 38 01 00 00     mov         ecx,138h
  000000006503F7DB: E8 92 19 5A 00     call        00000000655E1172
  000000006503F7E0: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000006503F7E5: 48 85 C0           test        rax,rax
  000000006503F7E8: 74 0A              je          000000006503F7F4
  000000006503F7EA: 48 8B C8           mov         rcx,rax
  000000006503F7ED: E8 AE CD FF FF     call        000000006503C5A0
  000000006503F7F2: EB 02              jmp         000000006503F7F6
  000000006503F7F4: 33 C0              xor         eax,eax
  000000006503F7F6: 45 33 C9           xor         r9d,r9d
  000000006503F7F9: 45 33 C0           xor         r8d,r8d
  000000006503F7FC: 48 8B D0           mov         rdx,rax
  000000006503F7FF: 48 8B CB           mov         rcx,rbx
  000000006503F802: E8 09 73 00 00     call        0000000065046B10
  000000006503F807: 90                 nop
  000000006503F808: 48 8D 05 D9 30 66  lea         rax,[00000000656A28E8h]
                    00
  000000006503F80F: 48 89 03           mov         qword ptr [rbx],rax
  000000006503F812: 48 8D 05 4F 30 66  lea         rax,[00000000656A2868h]
                    00
  000000006503F819: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  000000006503F81D: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006503F821: C7 44 24 48 01 00  mov         dword ptr [rsp+48h],1
                    00 00
  000000006503F829: C7 44 24 50 01 00  mov         dword ptr [rsp+50h],1
                    00 00
  000000006503F831: 48 8D 81 C4 00 00  lea         rax,[rcx+00000000000000C4h]
                    00
  000000006503F838: 4C 8D 44 24 48     lea         r8,[rsp+48h]
  000000006503F83D: 83 38 01           cmp         dword ptr [rax],1
  000000006503F840: 4C 0F 4F C0        cmovg       r8,rax
  000000006503F844: 48 8D 81 C0 00 00  lea         rax,[rcx+00000000000000C0h]
                    00
  000000006503F84B: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006503F850: 83 38 01           cmp         dword ptr [rax],1
  000000006503F853: 48 0F 4F D0        cmovg       rdx,rax
  000000006503F857: 45 8B 00           mov         r8d,dword ptr [r8]
  000000006503F85A: 8B 12              mov         edx,dword ptr [rdx]
  000000006503F85C: E8 7F D3 FF FF     call        000000006503CBE0
  000000006503F861: 90                 nop
  000000006503F862: 48 8B C3           mov         rax,rbx
  000000006503F865: 48 83 C4 30        add         rsp,30h
  000000006503F869: 5B                 pop         rbx
  000000006503F86A: C3                 ret

  Summary

      695000 .text

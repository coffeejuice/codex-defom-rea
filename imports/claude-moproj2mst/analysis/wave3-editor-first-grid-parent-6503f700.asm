Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006503F700: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006503F705: 57                 push        rdi
  000000006503F706: 48 83 EC 30        sub         rsp,30h
  000000006503F70A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006503F713: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006503F718: 48 8B FA           mov         rdi,rdx
  000000006503F71B: 48 8B D9           mov         rbx,rcx
  000000006503F71E: B9 38 01 00 00     mov         ecx,138h
  000000006503F723: E8 4A 1A 5A 00     call        00000000655E1172
  000000006503F728: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000006503F72D: 48 85 C0           test        rax,rax
  000000006503F730: 74 0A              je          000000006503F73C
  000000006503F732: 48 8B C8           mov         rcx,rax
  000000006503F735: E8 66 CE FF FF     call        000000006503C5A0
  000000006503F73A: EB 02              jmp         000000006503F73E
  000000006503F73C: 33 C0              xor         eax,eax
  000000006503F73E: 4C 8B CF           mov         r9,rdi
  000000006503F741: 45 33 C0           xor         r8d,r8d
  000000006503F744: 48 8B D0           mov         rdx,rax
  000000006503F747: 48 8B CB           mov         rcx,rbx
  000000006503F74A: E8 C1 73 00 00     call        0000000065046B10
  000000006503F74F: 90                 nop
  000000006503F750: 48 8D 05 91 31 66  lea         rax,[00000000656A28E8h]
                    00
  000000006503F757: 48 89 03           mov         qword ptr [rbx],rax
  000000006503F75A: 48 8D 05 07 31 66  lea         rax,[00000000656A2868h]
                    00
  000000006503F761: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  000000006503F765: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006503F769: C7 44 24 50 01 00  mov         dword ptr [rsp+50h],1
                    00 00
  000000006503F771: C7 44 24 58 01 00  mov         dword ptr [rsp+58h],1
                    00 00
  000000006503F779: 48 8D 81 C4 00 00  lea         rax,[rcx+00000000000000C4h]
                    00
  000000006503F780: 4C 8D 44 24 50     lea         r8,[rsp+50h]
  000000006503F785: 83 38 01           cmp         dword ptr [rax],1
  000000006503F788: 4C 0F 4F C0        cmovg       r8,rax
  000000006503F78C: 48 8D 81 C0 00 00  lea         rax,[rcx+00000000000000C0h]
                    00
  000000006503F793: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  000000006503F798: 83 38 01           cmp         dword ptr [rax],1
  000000006503F79B: 48 0F 4F D0        cmovg       rdx,rax
  000000006503F79F: 45 8B 00           mov         r8d,dword ptr [r8]
  000000006503F7A2: 8B 12              mov         edx,dword ptr [rdx]
  000000006503F7A4: E8 37 D4 FF FF     call        000000006503CBE0
  000000006503F7A9: 90                 nop
  000000006503F7AA: 48 8B C3           mov         rax,rbx
  000000006503F7AD: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006503F7B2: 48 83 C4 30        add         rsp,30h
  000000006503F7B6: 5F                 pop         rdi
  000000006503F7B7: C3                 ret

  Summary

      695000 .text

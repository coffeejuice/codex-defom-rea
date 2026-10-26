Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067131160: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000067131165: 57                 push        rdi
  0000000067131166: 48 83 EC 30        sub         rsp,30h
  000000006713116A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067131173: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  0000000067131178: 48 8B D9           mov         rbx,rcx
  000000006713117B: 48 8D 05 AE 88 07  lea         rax,[00000000671A9A30h]
                    00
  0000000067131182: 48 89 01           mov         qword ptr [rcx],rax
  0000000067131185: 48 8D 0D C4 F8 1B  lea         rcx,[00000000672F0A50h]
                    00
  000000006713118C: 48 89 4B 18        mov         qword ptr [rbx+18h],rcx
  0000000067131190: F0 83 05 B8 F8 1B  lock add    dword ptr [00000000672F0A50h],1
                    00 01
  0000000067131198: 48 8D 05 71 8B 07  lea         rax,[00000000671A9D10h]
                    00
  000000006713119F: 48 89 03           mov         qword ptr [rbx],rax
  00000000671311A2: 48 8D 05 87 F9 1B  lea         rax,[00000000672F0B30h]
                    00
  00000000671311A9: 48 89 43 30        mov         qword ptr [rbx+30h],rax
  00000000671311AD: F0 83 05 7B F9 1B  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000671311B5: 33 FF              xor         edi,edi
  00000000671311B7: 48 89 7B 40        mov         qword ptr [rbx+40h],rdi
  00000000671311BB: 48 89 7B 48        mov         qword ptr [rbx+48h],rdi
  00000000671311BF: 48 89 7B 50        mov         qword ptr [rbx+50h],rdi
  00000000671311C3: 48 89 7B 58        mov         qword ptr [rbx+58h],rdi
  00000000671311C7: 48 8D 05 EA F9 1B  lea         rax,[00000000672F0BB8h]
                    00
  00000000671311CE: 48 89 43 68        mov         qword ptr [rbx+68h],rax
  00000000671311D2: F0 83 05 DE F9 1B  lock add    dword ptr [00000000672F0BB8h],1
                    00 01
  00000000671311DA: 48 89 4B 70        mov         qword ptr [rbx+70h],rcx
  00000000671311DE: F0 83 05 6A F8 1B  lock add    dword ptr [00000000672F0A50h],1
                    00 01
  00000000671311E6: 48 89 7B 78        mov         qword ptr [rbx+78h],rdi
  00000000671311EA: 48 89 BB 80 00 00  mov         qword ptr [rbx+0000000000000080h],rdi
                    00
  00000000671311F1: 81 FA 04 08 04 00  cmp         edx,40804h
  00000000671311F7: 74 12              je          000000006713120B
  00000000671311F9: 41 B8 04 08 04 00  mov         r8d,40804h
  00000000671311FF: 48 8D 0D 3A BE 10  lea         rcx,[000000006723D040h]
                    00
  0000000067131206: E8 B5 BF EE FF     call        000000006701D1C0
  000000006713120B: 48 89 7B 08        mov         qword ptr [rbx+8],rdi
  000000006713120F: 48 89 7B 10        mov         qword ptr [rbx+10h],rdi
  0000000067131213: 89 7B 24           mov         dword ptr [rbx+24h],edi
  0000000067131216: 48 89 7B 38        mov         qword ptr [rbx+38h],rdi
  000000006713121A: 89 7B 64           mov         dword ptr [rbx+64h],edi
  000000006713121D: 89 7B 60           mov         dword ptr [rbx+60h],edi
  0000000067131220: 48 89 7B 28        mov         qword ptr [rbx+28h],rdi
  0000000067131224: 8B 43 20           mov         eax,dword ptr [rbx+20h]
  0000000067131227: 25 F0 FC FF FF     and         eax,0FFFFFCF0h
  000000006713122C: 83 C8 60           or          eax,60h
  000000006713122F: 89 43 20           mov         dword ptr [rbx+20h],eax
  0000000067131232: 48 8B C3           mov         rax,rbx
  0000000067131235: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006713123A: 48 83 C4 30        add         rsp,30h
  000000006713123E: 5F                 pop         rdi
  000000006713123F: C3                 ret

  Summary

      1A7000 .text

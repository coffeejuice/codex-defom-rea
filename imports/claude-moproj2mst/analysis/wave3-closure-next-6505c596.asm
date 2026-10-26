Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505C596: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  000000006505C59B: 48 8D 3D 0E D1 64  lea         rdi,[00000000656A96B0h]
                    00
  000000006505C5A2: 48 8B C2           mov         rax,rdx
  000000006505C5A5: 48 2B FA           sub         rdi,rdx
  000000006505C5A8: 0F 1F 84 00 00 00  nop         dword ptr [rax+rax+0000000000000000h]
                    00 00
  000000006505C5B0: 0F B6 10           movzx       edx,byte ptr [rax]
  000000006505C5B3: 0F B6 0C 38        movzx       ecx,byte ptr [rax+rdi]
  000000006505C5B7: 2B D1              sub         edx,ecx
  000000006505C5B9: 75 07              jne         000000006505C5C2
  000000006505C5BB: 48 FF C0           inc         rax
  000000006505C5BE: 85 C9              test        ecx,ecx
  000000006505C5C0: 75 EE              jne         000000006505C5B0
  000000006505C5C2: 85 D2              test        edx,edx
  000000006505C5C4: 75 0D              jne         000000006505C5D3
  000000006505C5C6: 49 8B C0           mov         rax,r8
  000000006505C5C9: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  000000006505C5CE: 48 83 C4 28        add         rsp,28h
  000000006505C5D2: C3                 ret
  000000006505C5D3: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  000000006505C5D8: 48 8D 3D 19 D8 64  lea         rdi,[00000000656A9DF8h]
                    00
  000000006505C5DF: B9 0D 00 00 00     mov         ecx,0Dh
  000000006505C5E4: 49 8B F1           mov         rsi,r9
  000000006505C5E7: F3 A6              repe cmps   byte ptr [rsi],byte ptr [rdi]
  000000006505C5E9: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  000000006505C5EE: 75 1F              jne         000000006505C60F
  000000006505C5F0: 4D 85 C0           test        r8,r8
  000000006505C5F3: 74 0E              je          000000006505C603
  000000006505C5F5: 49 8D 40 10        lea         rax,[r8+10h]
  000000006505C5F9: 48 8B 7C 24 20     mov         rdi,qword ptr [rsp+20h]
  000000006505C5FE: 48
  000000006505C5FF: 83

  Summary

      695000 .text

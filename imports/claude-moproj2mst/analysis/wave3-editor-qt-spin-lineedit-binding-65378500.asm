Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065378500: 48 85 D2           test        rdx,rdx
  0000000065378503: 0F 84 54 01 00 00  je          000000006537865D
  0000000065378509: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  000000006537850E: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  0000000065378513: 57                 push        rdi
  0000000065378514: 48 83 EC 30        sub         rsp,30h
  0000000065378518: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  000000006537851C: 48 8B F9           mov         rdi,rcx
  000000006537851F: 48 8B F2           mov         rsi,rdx
  0000000065378522: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  0000000065378529: 48 85 C9           test        rcx,rcx
  000000006537852C: 74 0B              je          0000000065378539
  000000006537852E: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065378531: BA 01 00 00 00     mov         edx,1
  0000000065378536: FF 50 18           call        qword ptr [rax+18h]
  0000000065378539: 48 8B CE           mov         rcx,rsi
  000000006537853C: 48 89 B3 E0 01 00  mov         qword ptr [rbx+00000000000001E0h],rsi
                    00
  0000000065378543: E8 B8 76 04 00     call        00000000653BFC00
  0000000065378548: 48 85 C0           test        rax,rax
  000000006537854B: 75 13              jne         0000000065378560
  000000006537854D: 48 8B 93 B0 02 00  mov         rdx,qword ptr [rbx+00000000000002B0h]
                    00
  0000000065378554: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  000000006537855B: E8 B0 76 04 00     call        00000000653BFC10
  0000000065378560: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  0000000065378567: FF 15 FB E2 31 00  call        qword ptr [0000000065696868h]
  000000006537856D: 48 3B C7           cmp         rax,rdi
  0000000065378570: 74 0F              je          0000000065378581
  0000000065378572: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  0000000065378579: 48 8B D7           mov         rdx,rdi
  000000006537857C: E8 DF 84 CF FF     call        0000000065070A60
  0000000065378581: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  0000000065378588: 33 D2              xor         edx,edx
  000000006537858A: E8 61 75 04 00     call        00000000653BFAF0
  000000006537858F: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  0000000065378596: 48 8B D7           mov         rdx,rdi
  0000000065378599: E8 C2 C0 CE FF     call        0000000065064660
  000000006537859E: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  00000000653785A5: 33 D2              xor         edx,edx
  00000000653785A7: E8 74 1C CF FF     call        000000006506A220
  00000000653785AC: 83 BB 40 02 00 00  cmp         dword ptr [rbx+0000000000000240h],0
                    00
  00000000653785B3: 74 4C              je          0000000065378601
  00000000653785B5: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  00000000653785BC: 4C 8D 0D 2D C3 37  lea         r9,[00000000656F48F0h]
                    00
  00000000653785C3: 48 8D 15 0E C3 37  lea         rdx,[00000000656F48D8h]
                    00
  00000000653785CA: 4C 8B C7           mov         r8,rdi
  00000000653785CD: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  00000000653785D5: FF 15 3D E1 31 00  call        qword ptr [0000000065696718h]
  00000000653785DB: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  00000000653785E2: 4C 8D 0D BF C2 37  lea         r9,[00000000656F48A8h]
                    00
  00000000653785E9: 48 8D 15 98 C2 37  lea         rdx,[00000000656F4888h]
                    00
  00000000653785F0: 4C 8B C7           mov         r8,rdi
  00000000653785F3: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  00000000653785FB: FF 15 17 E1 31 00  call        qword ptr [0000000065696718h]
  0000000065378601: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065378604: 48 8B CB           mov         rcx,rbx
  0000000065378607: FF 50 70           call        qword ptr [rax+70h]
  000000006537860A: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  0000000065378611: 33 D2              xor         edx,edx
  0000000065378613: E8 68 3D CE FF     call        000000006505C380
  0000000065378618: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006537861C: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006537861F: C1 E9 0F           shr         ecx,0Fh
  0000000065378622: F6 C1 01           test        cl,1
  0000000065378625: 74 0F              je          0000000065378636
  0000000065378627: 48 8B 8B E0 01 00  mov         rcx,qword ptr [rbx+00000000000001E0h]
                    00
  000000006537862E: B2 01              mov         dl,1
  0000000065378630: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065378633: FF 50 58           call        qword ptr [rax+58h]
  0000000065378636: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006537863A: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006537863D: C1 EA 0F           shr         edx,0Fh
  0000000065378640: F6 C2 01           test        dl,1
  0000000065378643: 74 09              je          000000006537864E
  0000000065378645: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065378648: 48 8B CB           mov         rcx,rbx
  000000006537864B: FF 50 30           call        qword ptr [rax+30h]
  000000006537864E: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  0000000065378653: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  0000000065378658: 48 83 C4 30        add         rsp,30h
  000000006537865C: 5F                 pop         rdi
  000000006537865D: F3 C3              rep ret

  Summary

      695000 .text

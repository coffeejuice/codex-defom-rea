Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006539B500: 40 53              push        rbx
  000000006539B502: 56                 push        rsi
  000000006539B503: 57                 push        rdi
  000000006539B504: 48 83 EC 40        sub         rsp,40h
  000000006539B508: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006539B511: 48 8B D9           mov         rbx,rcx
  000000006539B514: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  000000006539B518: 48 8B CF           mov         rcx,rdi
  000000006539B51B: E8 40 21 CC FF     call        000000006505D660
  000000006539B520: 4C 8B 10           mov         r10,qword ptr [rax]
  000000006539B523: 33 F6              xor         esi,esi
  000000006539B525: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  000000006539B52A: 4C 8B CF           mov         r9,rdi
  000000006539B52D: 45 33 C0           xor         r8d,r8d
  000000006539B530: 8D 56 45           lea         edx,[rsi+45h]
  000000006539B533: 48 8B C8           mov         rcx,rax
  000000006539B536: 41 FF 92 E8 00 00  call        qword ptr [r10+00000000000000E8h]
                    00
  000000006539B53D: 89 83 34 02 00 00  mov         dword ptr [rbx+0000000000000234h],eax
  000000006539B543: 48 8B 93 38 02 00  mov         rdx,qword ptr [rbx+0000000000000238h]
                    00
  000000006539B54A: 48 85 D2           test        rdx,rdx
  000000006539B54D: 74 41              je          000000006539B590
  000000006539B54F: 83 BB 30 02 00 00  cmp         dword ptr [rbx+0000000000000230h],1
                    01
  000000006539B556: 75 12              jne         000000006539B56A
  000000006539B558: 48 8D 0D E9 58 3B  lea         rcx,[0000000065750E48h]
                    00
  000000006539B55F: FF 15 A3 B4 2F 00  call        qword ptr [0000000065696A08h]
  000000006539B565: 48 85 C0           test        rax,rax
  000000006539B568: 75 26              jne         000000006539B590
  000000006539B56A: 83 BB 30 02 00 00  cmp         dword ptr [rbx+0000000000000230h],2
                    02
  000000006539B571: 0F 85 8D 00 00 00  jne         000000006539B604
  000000006539B577: 48 8B 93 38 02 00  mov         rdx,qword ptr [rbx+0000000000000238h]
                    00
  000000006539B57E: 48 8D 0D 43 58 3B  lea         rcx,[0000000065750DC8h]
                    00
  000000006539B585: FF 15 7D B4 2F 00  call        qword ptr [0000000065696A08h]
  000000006539B58B: 48 85 C0           test        rax,rax
  000000006539B58E: 74 74              je          000000006539B604
  000000006539B590: 48 8B 8B 38 02 00  mov         rcx,qword ptr [rbx+0000000000000238h]
                    00
  000000006539B597: 48 85 C9           test        rcx,rcx
  000000006539B59A: 74 0B              je          000000006539B5A7
  000000006539B59C: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006539B59F: BA 01 00 00 00     mov         edx,1
  000000006539B5A4: FF 50 18           call        qword ptr [rax+18h]
  000000006539B5A7: 83 BB 30 02 00 00  cmp         dword ptr [rbx+0000000000000230h],1
                    01
  000000006539B5AE: 75 2B              jne         000000006539B5DB
  000000006539B5B0: B9 20 00 00 00     mov         ecx,20h
  000000006539B5B5: E8 B8 5B 24 00     call        00000000655E1172
  000000006539B5BA: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006539B5BF: 48 85 C0           test        rax,rax
  000000006539B5C2: 74 0E              je          000000006539B5D2
  000000006539B5C4: 48 8B D7           mov         rdx,rdi
  000000006539B5C7: 48 8B C8           mov         rcx,rax
  000000006539B5CA: E8 01 1A C9 FF     call        000000006502CFD0
  000000006539B5CF: 48 8B F0           mov         rsi,rax
  000000006539B5D2: 48 89 B3 38 02 00  mov         qword ptr [rbx+0000000000000238h],rsi
                    00
  000000006539B5D9: EB 29              jmp         000000006539B604
  000000006539B5DB: B9 20 00 00 00     mov         ecx,20h
  000000006539B5E0: E8 8D 5B 24 00     call        00000000655E1172
  000000006539B5E5: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006539B5EA: 48 85 C0           test        rax,rax
  000000006539B5ED: 74 0E              je          000000006539B5FD
  000000006539B5EF: 48 8B D7           mov         rdx,rdi
  000000006539B5F2: 48 8B C8           mov         rcx,rax
  000000006539B5F5: E8 86 1C C9 FF     call        000000006502D280
  000000006539B5FA: 48 8B F0           mov         rsi,rax
  000000006539B5FD: 48 89 B3 38 02 00  mov         qword ptr [rbx+0000000000000238h],rsi
                    00
  000000006539B604: 45 33 C0           xor         r8d,r8d
  000000006539B607: 41 8D 50 30        lea         edx,[r8+30h]
  000000006539B60B: 48 8B CB           mov         rcx,rbx
  000000006539B60E: E8 0D 2E CC FF     call        000000006505E420
  000000006539B613: 4C 8D 5C 24 68     lea         r11,[rsp+68h]
  000000006539B618: 4C 89 5C 24 20     mov         qword ptr [rsp+20h],r11
  000000006539B61D: 4C 8D 4C 24 70     lea         r9,[rsp+70h]
  000000006539B622: 4C 8D 44 24 78     lea         r8,[rsp+78h]
  000000006539B627: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006539B62C: 48 8B CB           mov         rcx,rbx
  000000006539B62F: E8 CC 0E CC FF     call        000000006505C500
  000000006539B634: 44 8B 5C 24 68     mov         r11d,dword ptr [rsp+68h]
  000000006539B639: 41 F7 DB           neg         r11d
  000000006539B63C: 44 8B 4C 24 70     mov         r9d,dword ptr [rsp+70h]
  000000006539B641: 41 F7 D9           neg         r9d
  000000006539B644: 44 8B 44 24 78     mov         r8d,dword ptr [rsp+78h]
  000000006539B649: 41 F7 D8           neg         r8d
  000000006539B64C: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  000000006539B650: F7 DA              neg         edx
  000000006539B652: 44 89 5C 24 20     mov         dword ptr [rsp+20h],r11d
  000000006539B657: 48 8B 8B 38 02 00  mov         rcx,qword ptr [rbx+0000000000000238h]
                    00
  000000006539B65E: E8 5D 93 CA FF     call        00000000650449C0
  000000006539B663: BA 44 00 00 00     mov         edx,44h
  000000006539B668: 48 8B CF           mov         rcx,rdi
  000000006539B66B: E8 A0 29 CC FF     call        000000006505E010
  000000006539B670: 84 C0              test        al,al
  000000006539B672: 0F 85 81 00 00 00  jne         000000006539B6F9
  000000006539B678: C7 44 24 60 07 00  mov         dword ptr [rsp+60h],7
                    00 00
  000000006539B680: BA 02 00 00 00     mov         edx,2
  000000006539B685: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006539B68A: E8 01 86 CA FF     call        0000000065043C90
  000000006539B68F: 83 BB 30 02 00 00  cmp         dword ptr [rbx+0000000000000230h],2
                    02
  000000006539B696: 75 46              jne         000000006539B6DE
  000000006539B698: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  000000006539B69C: 8B D0              mov         edx,eax
  000000006539B69E: C1 EA 04           shr         edx,4
  000000006539B6A1: 83 E2 0F           and         edx,0Fh
  000000006539B6A4: 8B C8              mov         ecx,eax
  000000006539B6A6: 83 E1 0F           and         ecx,0Fh
  000000006539B6A9: C1 E1 04           shl         ecx,4
  000000006539B6AC: 0B D1              or          edx,ecx
  000000006539B6AE: 8B C8              mov         ecx,eax
  000000006539B6B0: C1 E9 10           shr         ecx,10h
  000000006539B6B3: 44 0F B6 C1        movzx       r8d,cl
  000000006539B6B7: 41 C1 E0 18        shl         r8d,18h
  000000006539B6BB: 8B C8              mov         ecx,eax
  000000006539B6BD: 81 E1 00 FF 00 00  and         ecx,0FF00h
  000000006539B6C3: 44 0B C1           or          r8d,ecx
  000000006539B6C6: 41 81 E0 FF FF 00  and         r8d,0FF00FFFFh
                    FF
  000000006539B6CD: 41 0B D0           or          edx,r8d
  000000006539B6D0: C1 E8 18           shr         eax,18h
  000000006539B6D3: C1 E0 10           shl         eax,10h
  000000006539B6D6: 0B D0              or          edx,eax
  000000006539B6D8: 89 54 24 60        mov         dword ptr [rsp+60h],edx
  000000006539B6DC: EB 04              jmp         000000006539B6E2
  000000006539B6DE: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  000000006539B6E2: 48 8B CF           mov         rcx,rdi
  000000006539B6E5: E8 36 24 CD FF     call        000000006506DB20
  000000006539B6EA: 45 33 C0           xor         r8d,r8d
  000000006539B6ED: 41 8D 50 44        lea         edx,[r8+44h]
  000000006539B6F1: 48 8B CF           mov         rcx,rdi
  000000006539B6F4: E8 E7 D8 CC FF     call        0000000065068FE0
  000000006539B6F9: BA 01 00 00 00     mov         edx,1
  000000006539B6FE: 48 8B CF           mov         rcx,rdi
  000000006539B701: E8 5A 26 CC FF     call        000000006505DD60
  000000006539B706: 48 83 C4 40        add         rsp,40h
  000000006539B70A: 5F                 pop         rdi
  000000006539B70B: 5E                 pop         rsi
  000000006539B70C: 5B                 pop         rbx
  000000006539B70D: C3                 ret

  Summary

      695000 .text

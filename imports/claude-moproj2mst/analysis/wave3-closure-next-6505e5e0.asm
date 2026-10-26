Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505E5E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006505E5E5: 57                 push        rdi
  000000006505E5E6: 48 83 EC 40        sub         rsp,40h
  000000006505E5EA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006505E5F3: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  000000006505E5F8: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  000000006505E5FD: 48 8B F2           mov         rsi,rdx
  000000006505E600: 48 8B F9           mov         rdi,rcx
  000000006505E603: BA 01 00 00 00     mov         edx,1
  000000006505E608: 48 8B 09           mov         rcx,qword ptr [rcx]
  000000006505E60B: FF 15 5F 84 63 00  call        qword ptr [0000000065696A70h]
  000000006505E611: 84 C0              test        al,al
  000000006505E613: 75 2E              jne         000000006505E643
  000000006505E615: 48 8B 17           mov         rdx,qword ptr [rdi]
  000000006505E618: 48 63 4A 08        movsxd      rcx,dword ptr [rdx+8]
  000000006505E61C: 8D 41 01           lea         eax,[rcx+1]
  000000006505E61F: 3B 42 04           cmp         eax,dword ptr [rdx+4]
  000000006505E622: 7F 1F              jg          000000006505E643
  000000006505E624: 48 FF C1           inc         rcx
  000000006505E627: 48 C1 E1 04        shl         rcx,4
  000000006505E62B: 48 03 CA           add         rcx,rdx
  000000006505E62E: 48 89 4C 24 60     mov         qword ptr [rsp+60h],rcx
  000000006505E633: 74 09              je          000000006505E63E
  000000006505E635: 48 8B D6           mov         rdx,rsi
  000000006505E638: E8 23 E0 FF FF     call        000000006505C660
  000000006505E63D: 90                 nop
  000000006505E63E: E9 9A 00 00 00     jmp         000000006505E6DD
  000000006505E643: 48 8B 1E           mov         rbx,qword ptr [rsi]
  000000006505E646: 48 89 5C 24 28     mov         qword ptr [rsp+28h],rbx
  000000006505E64B: 48 8B 76 08        mov         rsi,qword ptr [rsi+8]
  000000006505E64F: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  000000006505E654: 48 85 DB           test        rbx,rbx
  000000006505E657: 74 0B              je          000000006505E664
  000000006505E659: 48 8D 4B 08        lea         rcx,[rbx+8]
  000000006505E65D: FF 15 5D 82 63 00  call        qword ptr [00000000656968C0h]
  000000006505E663: 90                 nop
  000000006505E664: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006505E667: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006505E66A: FF C2              inc         edx
  000000006505E66C: 45 33 C9           xor         r9d,r9d
  000000006505E66F: 41 8D 49 20        lea         ecx,[r9+20h]
  000000006505E673: 45 8D 41 10        lea         r8d,[r9+10h]
  000000006505E677: FF 15 6B 8A 63 00  call        qword ptr [00000000656970E8h]
  000000006505E67D: 44 8B C0           mov         r8d,eax
  000000006505E680: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006505E683: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006505E686: 48 8B CF           mov         rcx,rdi
  000000006505E689: E8 B2 E7 FF FF     call        000000006505CE40
  000000006505E68E: 4C 8B 1F           mov         r11,qword ptr [rdi]
  000000006505E691: 49 63 43 08        movsxd      rax,dword ptr [r11+8]
  000000006505E695: 48 FF C0           inc         rax
  000000006505E698: 48 C1 E0 04        shl         rax,4
  000000006505E69C: 49 03 C3           add         rax,r11
  000000006505E69F: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006505E6A4: 74 17              je          000000006505E6BD
  000000006505E6A6: 48 89 18           mov         qword ptr [rax],rbx
  000000006505E6A9: 48 89 70 08        mov         qword ptr [rax+8],rsi
  000000006505E6AD: 48 85 DB           test        rbx,rbx
  000000006505E6B0: 74 0B              je          000000006505E6BD
  000000006505E6B2: 48 8D 4B 08        lea         rcx,[rbx+8]
  000000006505E6B6: FF 15 04 82 63 00  call        qword ptr [00000000656968C0h]
  000000006505E6BC: 90                 nop
  000000006505E6BD: 48 85 DB           test        rbx,rbx
  000000006505E6C0: 74 1B              je          000000006505E6DD
  000000006505E6C2: 48 8D 4B 08        lea         rcx,[rbx+8]
  000000006505E6C6: FF 15 AC 82 63 00  call        qword ptr [0000000065696978h]
  000000006505E6CC: 84 C0              test        al,al
  000000006505E6CE: 75 0D              jne         000000006505E6DD
  000000006505E6D0: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006505E6D3: BA 01 00 00 00     mov         edx,1
  000000006505E6D8: 48 8B CB           mov         rcx,rbx
  000000006505E6DB: FF 10              call        qword ptr [rax]
  000000006505E6DD: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006505E6E0: FF 40 08           inc         dword ptr [rax+8]
  000000006505E6E3: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  000000006505E6E8: 48 8B 74 24 68     mov         rsi,qword ptr [rsp+68h]
  000000006505E6ED: 48 83 C4 40        add         rsp,40h
  000000006505E6F1: 5F                 pop         rdi
  000000006505E6F2: C3                 ret

  Summary

      695000 .text

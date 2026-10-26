Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006507E340: 40 55              push        rbp
  000000006507E342: 56                 push        rsi
  000000006507E343: 57                 push        rdi
  000000006507E344: 41 54              push        r12
  000000006507E346: 41 55              push        r13
  000000006507E348: 41 56              push        r14
  000000006507E34A: 41 57              push        r15
  000000006507E34C: 48 81 EC B0 00 00  sub         rsp,0B0h
                    00
  000000006507E353: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006507E35C: 48 89 9C 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rbx
                    00 00
  000000006507E364: 4D 8B F0           mov         r14,r8
  000000006507E367: 4C 8B E2           mov         r12,rdx
  000000006507E36A: 4C 8B F9           mov         r15,rcx
  000000006507E36D: 48 8B 35 7C 85 61  mov         rsi,qword ptr [00000000656968F0h]
                    00
  000000006507E374: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  000000006507E379: 48 8D 4E 68        lea         rcx,[rsi+68h]
  000000006507E37D: FF 15 3D 85 61 00  call        qword ptr [00000000656968C0h]
  000000006507E383: 90                 nop
  000000006507E384: 48 8B 2D 65 85 61  mov         rbp,qword ptr [00000000656968F0h]
                    00
  000000006507E38B: 48 89 AC 24 08 01  mov         qword ptr [rsp+0000000000000108h],rbp
                    00 00
  000000006507E393: 48 8D 4D 68        lea         rcx,[rbp+68h]
  000000006507E397: FF 15 23 85 61 00  call        qword ptr [00000000656968C0h]
  000000006507E39D: 90                 nop
  000000006507E39E: 49 8B 5C 24 08     mov         rbx,qword ptr [r12+8]
  000000006507E3A3: 48 8B 83 C0 01 00  mov         rax,qword ptr [rbx+00000000000001C0h]
                    00
  000000006507E3AA: 83 78 70 00        cmp         dword ptr [rax+70h],0
  000000006507E3AE: 0F 84 A6 01 00 00  je          000000006507E55A
  000000006507E3B4: 48 8D 48 68        lea         rcx,[rax+68h]
  000000006507E3B8: BA 01 00 00 00     mov         edx,1
  000000006507E3BD: FF 15 AD 86 61 00  call        qword ptr [0000000065696A70h]
  000000006507E3C3: 84 C0              test        al,al
  000000006507E3C5: 74 0C              je          000000006507E3D3
  000000006507E3C7: 48 8D 8B C0 01 00  lea         rcx,[rbx+00000000000001C0h]
                    00
  000000006507E3CE: E8 6D FC F8 FF     call        000000006500E040
  000000006507E3D3: 48 8B 9B C0 01 00  mov         rbx,qword ptr [rbx+00000000000001C0h]
                    00
  000000006507E3DA: 48 8B 5B 08        mov         rbx,qword ptr [rbx+8]
  000000006507E3DE: 49 8B 7C 24 08     mov         rdi,qword ptr [r12+8]
  000000006507E3E3: 48 8B 8F C0 01 00  mov         rcx,qword ptr [rdi+00000000000001C0h]
                    00
  000000006507E3EA: 48 83 C1 68        add         rcx,68h
  000000006507E3EE: BA 01 00 00 00     mov         edx,1
  000000006507E3F3: FF 15 77 86 61 00  call        qword ptr [0000000065696A70h]
  000000006507E3F9: 84 C0              test        al,al
  000000006507E3FB: 74 0C              je          000000006507E409
  000000006507E3FD: 48 8D 8F C0 01 00  lea         rcx,[rdi+00000000000001C0h]
                    00
  000000006507E404: E8 37 FC F8 FF     call        000000006500E040
  000000006507E409: 48 8B BF C0 01 00  mov         rdi,qword ptr [rdi+00000000000001C0h]
                    00
  000000006507E410: 48 3B DF           cmp         rbx,rdi
  000000006507E413: 0F 84 41 01 00 00  je          000000006507E55A
  000000006507E419: C7 84 24 F8 00 00  mov         dword ptr [rsp+00000000000000F8h],0
                    00 00 00 00 00
  000000006507E424: 4C 89 64 24 38     mov         qword ptr [rsp+38h],r12
  000000006507E429: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006507E430: 4C 8D 6B F8        lea         r13,[rbx-8]
  000000006507E434: 48 8D 4E 68        lea         rcx,[rsi+68h]
  000000006507E438: BA 01 00 00 00     mov         edx,1
  000000006507E43D: FF 15 2D 86 61 00  call        qword ptr [0000000065696A70h]
  000000006507E443: 84 C0              test        al,al
  000000006507E445: 74 0F              je          000000006507E456
  000000006507E447: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006507E44C: E8 9F 90 FF FF     call        00000000650774F0
  000000006507E451: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  000000006507E456: 48 8B D6           mov         rdx,rsi
  000000006507E459: 48 63 46 6C        movsxd      rax,dword ptr [rsi+6Ch]
  000000006507E45D: 48 85 C0           test        rax,rax
  000000006507E460: 78 43              js          000000006507E4A5
  000000006507E462: 48 8B 4C C2 08     mov         rcx,qword ptr [rdx+rax*8+8]
  000000006507E467: 48 3B CE           cmp         rcx,rsi
  000000006507E46A: 74 17              je          000000006507E483
  000000006507E46C: 45 8B 45 00        mov         r8d,dword ptr [r13]
  000000006507E470: 44 39 41 F8        cmp         dword ptr [rcx-8],r8d
  000000006507E474: 7D 0D              jge         000000006507E483
  000000006507E476: 48 8B D1           mov         rdx,rcx
  000000006507E479: 48 8B 4C C1 08     mov         rcx,qword ptr [rcx+rax*8+8]
  000000006507E47E: 48 3B CE           cmp         rcx,rsi
  000000006507E481: 75 ED              jne         000000006507E470
  000000006507E483: 48 89 54 C4 50     mov         qword ptr [rsp+rax*8+50h],rdx
  000000006507E488: 48 83 E8 01        sub         rax,1
  000000006507E48C: 79 D4              jns         000000006507E462
  000000006507E48E: 48 3B CE           cmp         rcx,rsi
  000000006507E491: 74 12              je          000000006507E4A5
  000000006507E493: 8B 41 F8           mov         eax,dword ptr [rcx-8]
  000000006507E496: 41 39 45 00        cmp         dword ptr [r13],eax
  000000006507E49A: 7C 09              jl          000000006507E4A5
  000000006507E49C: C7 41 FC 00 00 00  mov         dword ptr [rcx-4],0
                    00
  000000006507E4A3: EB 22              jmp         000000006507E4C7
  000000006507E4A5: 48 8D 84 24 F8 00  lea         rax,[rsp+00000000000000F8h]
                    00 00
  000000006507E4AD: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006507E4B2: 4D 8B CD           mov         r9,r13
  000000006507E4B5: 4C 8D 44 24 50     lea         r8,[rsp+50h]
  000000006507E4BA: 48 8B D6           mov         rdx,rsi
  000000006507E4BD: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006507E4C2: E8 09 24 1F 00     call        00000000652708D0
  000000006507E4C7: 48 8D 4D 68        lea         rcx,[rbp+68h]
  000000006507E4CB: BA 01 00 00 00     mov         edx,1
  000000006507E4D0: FF 15 9A 85 61 00  call        qword ptr [0000000065696A70h]
  000000006507E4D6: 84 C0              test        al,al
  000000006507E4D8: 74 15              je          000000006507E4EF
  000000006507E4DA: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  000000006507E4E2: E8 39 8F FF FF     call        0000000065077420
  000000006507E4E7: 48 8B AC 24 08 01  mov         rbp,qword ptr [rsp+0000000000000108h]
                    00 00
  000000006507E4EF: 48 8B D5           mov         rdx,rbp
  000000006507E4F2: 48 63 4D 6C        movsxd      rcx,dword ptr [rbp+6Ch]
  000000006507E4F6: 48 85 C9           test        rcx,rcx
  000000006507E4F9: 78 33              js          000000006507E52E
  000000006507E4FB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  000000006507E500: 48 8B 44 CA 08     mov         rax,qword ptr [rdx+rcx*8+8]
  000000006507E505: 48 3B C5           cmp         rax,rbp
  000000006507E508: 74 19              je          000000006507E523
  000000006507E50A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006507E510: 4C 39 60 F0        cmp         qword ptr [rax-10h],r12
  000000006507E514: 73 0D              jae         000000006507E523
  000000006507E516: 48 8B D0           mov         rdx,rax
  000000006507E519: 48 8B 44 C8 08     mov         rax,qword ptr [rax+rcx*8+8]
  000000006507E51E: 48 3B C5           cmp         rax,rbp
  000000006507E521: 75 ED              jne         000000006507E510
  000000006507E523: 48 89 54 CC 50     mov         qword ptr [rsp+rcx*8+50h],rdx
  000000006507E528: 48 83 E9 01        sub         rcx,1
  000000006507E52C: 79 D2              jns         000000006507E500
  000000006507E52E: 4C 89 6C 24 20     mov         qword ptr [rsp+20h],r13
  000000006507E533: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  000000006507E538: 4C 8D 44 24 50     lea         r8,[rsp+50h]
  000000006507E53D: 48 8B D5           mov         rdx,rbp
  000000006507E540: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  000000006507E548: E8 93 8D FF FF     call        00000000650772E0
  000000006507E54D: 48 8B 5B 08        mov         rbx,qword ptr [rbx+8]
  000000006507E551: 48 3B DF           cmp         rbx,rdi
  000000006507E554: 0F 85 D6 FE FF FF  jne         000000006507E430
  000000006507E55A: 49 8B 44 24 20     mov         rax,qword ptr [r12+20h]
  000000006507E55F: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006507E563: A8 01              test        al,1
  000000006507E565: 0F 85 34 02 00 00  jne         000000006507E79F
  000000006507E56B: 49 8B CC           mov         rcx,r12
  000000006507E56E: FF 15 F4 82 61 00  call        qword ptr [0000000065696868h]
  000000006507E574: 4C 8B E0           mov         r12,rax
  000000006507E577: 48 85 C0           test        rax,rax
  000000006507E57A: 0F 84 1F 02 00 00  je          000000006507E79F
  000000006507E580: 49 8B 5C 24 08     mov         rbx,qword ptr [r12+8]
  000000006507E585: 48 8B 8B C0 01 00  mov         rcx,qword ptr [rbx+00000000000001C0h]
                    00
  000000006507E58C: 48 83 C1 68        add         rcx,68h
  000000006507E590: BA 01 00 00 00     mov         edx,1
  000000006507E595: FF 15 D5 84 61 00  call        qword ptr [0000000065696A70h]
  000000006507E59B: 84 C0              test        al,al
  000000006507E59D: 74 0C              je          000000006507E5AB
  000000006507E59F: 48 8D 8B C0 01 00  lea         rcx,[rbx+00000000000001C0h]
                    00
  000000006507E5A6: E8 95 FA F8 FF     call        000000006500E040
  000000006507E5AB: 48 8B 9B C0 01 00  mov         rbx,qword ptr [rbx+00000000000001C0h]
                    00
  000000006507E5B2: 48 8B 5B 08        mov         rbx,qword ptr [rbx+8]
  000000006507E5B6: 49 8B 7C 24 08     mov         rdi,qword ptr [r12+8]
  000000006507E5BB: 48 8B 8F C0 01 00  mov         rcx,qword ptr [rdi+00000000000001C0h]
                    00
  000000006507E5C2: 48 83 C1 68        add         rcx,68h
  000000006507E5C6: BA 01 00 00 00     mov         edx,1
  000000006507E5CB: FF 15 9F 84 61 00  call        qword ptr [0000000065696A70h]
  000000006507E5D1: 84 C0              test        al,al
  000000006507E5D3: 74 0C              je          000000006507E5E1
  000000006507E5D5: 48 8D 8F C0 01 00  lea         rcx,[rdi+00000000000001C0h]
                    00
  000000006507E5DC: E8 5F FA F8 FF     call        000000006500E040
  000000006507E5E1: 48 8B BF C0 01 00  mov         rdi,qword ptr [rdi+00000000000001C0h]
                    00
  000000006507E5E8: 48 3B DF           cmp         rbx,rdi
  000000006507E5EB: 0F 84 8C 01 00 00  je          000000006507E77D
  000000006507E5F1: 8B 43 FC           mov         eax,dword ptr [rbx-4]
  000000006507E5F4: 83 E0 01           and         eax,1
  000000006507E5F7: 0F 85 73 01 00 00  jne         000000006507E770
  000000006507E5FD: 4C 8D 6B F8        lea         r13,[rbx-8]
  000000006507E601: 4C 8B C6           mov         r8,rsi
  000000006507E604: 48 63 4E 6C        movsxd      rcx,dword ptr [rsi+6Ch]
  000000006507E608: 48 85 C9           test        rcx,rcx
  000000006507E60B: 78 3D              js          000000006507E64A
  000000006507E60D: 0F 1F 00           nop         dword ptr [rax]
  000000006507E610: 49 8B 44 C8 08     mov         rax,qword ptr [r8+rcx*8+8]
  000000006507E615: 48 3B C6           cmp         rax,rsi
  000000006507E618: 74 18              je          000000006507E632
  000000006507E61A: 41 8B 55 00        mov         edx,dword ptr [r13]
  000000006507E61E: 66 90              nop
  000000006507E620: 39 50 F8           cmp         dword ptr [rax-8],edx
  000000006507E623: 7D 0D              jge         000000006507E632
  000000006507E625: 4C 8B C0           mov         r8,rax
  000000006507E628: 48 8B 44 C8 08     mov         rax,qword ptr [rax+rcx*8+8]
  000000006507E62D: 48 3B C6           cmp         rax,rsi
  000000006507E630: 75 EE              jne         000000006507E620
  000000006507E632: 48 83 E9 01        sub         rcx,1
  000000006507E636: 79 D8              jns         000000006507E610
  000000006507E638: 48 3B C6           cmp         rax,rsi
  000000006507E63B: 74 0D              je          000000006507E64A
  000000006507E63D: 8B 40 F8           mov         eax,dword ptr [rax-8]
  000000006507E640: 41 39 45 00        cmp         dword ptr [r13],eax
  000000006507E644: 0F 8D 26 01 00 00  jge         000000006507E770
  000000006507E64A: C7 84 24 F8 00 00  mov         dword ptr [rsp+00000000000000F8h],0
                    00 00 00 00 00
  000000006507E655: 48 8D 4E 68        lea         rcx,[rsi+68h]
  000000006507E659: BA 01 00 00 00     mov         edx,1
  000000006507E65E: FF 15 0C 84 61 00  call        qword ptr [0000000065696A70h]
  000000006507E664: 84 C0              test        al,al
  000000006507E666: 74 0F              je          000000006507E677
  000000006507E668: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006507E66D: E8 7E 8E FF FF     call        00000000650774F0
  000000006507E672: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  000000006507E677: 48 8B D6           mov         rdx,rsi
  000000006507E67A: 48 63 46 6C        movsxd      rax,dword ptr [rsi+6Ch]
  000000006507E67E: 48 85 C0           test        rax,rax
  000000006507E681: 78 43              js          000000006507E6C6
  000000006507E683: 48 8B 4C C2 08     mov         rcx,qword ptr [rdx+rax*8+8]
  000000006507E688: 48 3B CE           cmp         rcx,rsi
  000000006507E68B: 74 17              je          000000006507E6A4
  000000006507E68D: 45 8B 45 00        mov         r8d,dword ptr [r13]
  000000006507E691: 44 39 41 F8        cmp         dword ptr [rcx-8],r8d
  000000006507E695: 7D 0D              jge         000000006507E6A4
  000000006507E697: 48 8B D1           mov         rdx,rcx
  000000006507E69A: 48 8B 4C C1 08     mov         rcx,qword ptr [rcx+rax*8+8]
  000000006507E69F: 48 3B CE           cmp         rcx,rsi
  000000006507E6A2: 75 ED              jne         000000006507E691
  000000006507E6A4: 48 89 54 C4 50     mov         qword ptr [rsp+rax*8+50h],rdx
  000000006507E6A9: 48 83 E8 01        sub         rax,1
  000000006507E6AD: 79 D4              jns         000000006507E683
  000000006507E6AF: 48 3B CE           cmp         rcx,rsi
  000000006507E6B2: 74 12              je          000000006507E6C6
  000000006507E6B4: 8B 41 F8           mov         eax,dword ptr [rcx-8]
  000000006507E6B7: 41 39 45 00        cmp         dword ptr [r13],eax
  000000006507E6BB: 7C 09              jl          000000006507E6C6
  000000006507E6BD: C7 41 FC 00 00 00  mov         dword ptr [rcx-4],0
                    00
  000000006507E6C4: EB 22              jmp         000000006507E6E8
  000000006507E6C6: 48 8D 84 24 F8 00  lea         rax,[rsp+00000000000000F8h]
                    00 00
  000000006507E6CE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006507E6D3: 4D 8B CD           mov         r9,r13
  000000006507E6D6: 4C 8D 44 24 50     lea         r8,[rsp+50h]
  000000006507E6DB: 48 8B D6           mov         rdx,rsi
  000000006507E6DE: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006507E6E3: E8 E8 21 1F 00     call        00000000652708D0
  000000006507E6E8: 4C 89 A4 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],r12
                    00 00
  000000006507E6F0: 48 8D 4D 68        lea         rcx,[rbp+68h]
  000000006507E6F4: BA 01 00 00 00     mov         edx,1
  000000006507E6F9: FF 15 71 83 61 00  call        qword ptr [0000000065696A70h]
  000000006507E6FF: 84 C0              test        al,al
  000000006507E701: 74 15              je          000000006507E718
  000000006507E703: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  000000006507E70B: E8 10 8D FF FF     call        0000000065077420
  000000006507E710: 48 8B AC 24 08 01  mov         rbp,qword ptr [rsp+0000000000000108h]
                    00 00
  000000006507E718: 48 8B D5           mov         rdx,rbp
  000000006507E71B: 48 63 4D 6C        movsxd      rcx,dword ptr [rbp+6Ch]
  000000006507E71F: 48 85 C9           test        rcx,rcx
  000000006507E722: 78 2A              js          000000006507E74E
  000000006507E724: 48 8B 44 CA 08     mov         rax,qword ptr [rdx+rcx*8+8]
  000000006507E729: 48 3B C5           cmp         rax,rbp
  000000006507E72C: 74 15              je          000000006507E743
  000000006507E72E: 66 90              nop
  000000006507E730: 4C 39 60 F0        cmp         qword ptr [rax-10h],r12
  000000006507E734: 73 0D              jae         000000006507E743
  000000006507E736: 48 8B D0           mov         rdx,rax
  000000006507E739: 48 8B 44 C8 08     mov         rax,qword ptr [rax+rcx*8+8]
  000000006507E73E: 48 3B C5           cmp         rax,rbp
  000000006507E741: 75 ED              jne         000000006507E730
  000000006507E743: 48 89 54 CC 50     mov         qword ptr [rsp+rcx*8+50h],rdx
  000000006507E748: 48 83 E9 01        sub         rcx,1
  000000006507E74C: 79 D6              jns         000000006507E724
  000000006507E74E: 4C 89 6C 24 20     mov         qword ptr [rsp+20h],r13
  000000006507E753: 4C 8D 8C 24 F8 00  lea         r9,[rsp+00000000000000F8h]
                    00 00
  000000006507E75B: 4C 8D 44 24 50     lea         r8,[rsp+50h]
  000000006507E760: 48 8B D5           mov         rdx,rbp
  000000006507E763: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  000000006507E76B: E8 70 8B FF FF     call        00000000650772E0
  000000006507E770: 48 8B 5B 08        mov         rbx,qword ptr [rbx+8]
  000000006507E774: 48 3B DF           cmp         rbx,rdi
  000000006507E777: 0F 85 74 FE FF FF  jne         000000006507E5F1
  000000006507E77D: 49 8B 44 24 20     mov         rax,qword ptr [r12+20h]
  000000006507E782: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006507E786: A8 01              test        al,1
  000000006507E788: 75 15              jne         000000006507E79F
  000000006507E78A: 49 8B CC           mov         rcx,r12
  000000006507E78D: FF 15 D5 80 61 00  call        qword ptr [0000000065696868h]
  000000006507E793: 4C 8B E0           mov         r12,rax
  000000006507E796: 48 85 C0           test        rax,rax
  000000006507E799: 0F 85 E1 FD FF FF  jne         000000006507E580
  000000006507E79F: 83 7D 70 00        cmp         dword ptr [rbp+70h],0
  000000006507E7A3: 75 04              jne         000000006507E7A9
  000000006507E7A5: 32 DB              xor         bl,bl
  000000006507E7A7: EB 16              jmp         000000006507E7BF
  000000006507E7A9: 4D 8B C6           mov         r8,r14
  000000006507E7AC: 48 8D 94 24 08 01  lea         rdx,[rsp+0000000000000108h]
                    00 00
  000000006507E7B4: 49 8B CF           mov         rcx,r15
  000000006507E7B7: E8 84 E6 FF FF     call        000000006507CE40
  000000006507E7BC: 0F B6 D8           movzx       ebx,al
  000000006507E7BF: 48 8D 4D 68        lea         rcx,[rbp+68h]
  000000006507E7C3: FF 15 AF 81 61 00  call        qword ptr [0000000065696978h]
  000000006507E7C9: 84 C0              test        al,al
  000000006507E7CB: 75 21              jne         000000006507E7EE
  000000006507E7CD: 48 8B 4D 08        mov         rcx,qword ptr [rbp+8]
  000000006507E7D1: 48 3B CD           cmp         rcx,rbp
  000000006507E7D4: 74 09              je          000000006507E7DF
  000000006507E7D6: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006507E7DA: 48 3B CD           cmp         rcx,rbp
  000000006507E7DD: 75 F7              jne         000000006507E7D6
  000000006507E7DF: BA 10 00 00 00     mov         edx,10h
  000000006507E7E4: 48 8B CD           mov         rcx,rbp
  000000006507E7E7: FF 15 6B 81 61 00  call        qword ptr [0000000065696958h]
  000000006507E7ED: 90                 nop
  000000006507E7EE: 48 85 F6           test        rsi,rsi
  000000006507E7F1: 74 3C              je          000000006507E82F
  000000006507E7F3: 48 8D 4E 68        lea         rcx,[rsi+68h]
  000000006507E7F7: FF 15 7B 81 61 00  call        qword ptr [0000000065696978h]
  000000006507E7FD: 84 C0              test        al,al
  000000006507E7FF: 75 2A              jne         000000006507E82B
  000000006507E801: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  000000006507E805: 48 3B C6           cmp         rax,rsi
  000000006507E808: 74 0F              je          000000006507E819
  000000006507E80A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006507E810: 48 8B 40 08        mov         rax,qword ptr [rax+8]
  000000006507E814: 48 3B C6           cmp         rax,rsi
  000000006507E817: 75 F7              jne         000000006507E810
  000000006507E819: BA 08 00 00 00     mov         edx,8
  000000006507E81E: 48 8B CE           mov         rcx,rsi
  000000006507E821: FF 15 31 81 61 00  call        qword ptr [0000000065696958h]
  000000006507E827: 8A C3              mov         al,bl
  000000006507E829: EB 06              jmp         000000006507E831
  000000006507E82B: 8A C3              mov         al,bl
  000000006507E82D: EB 02              jmp         000000006507E831
  000000006507E82F: 8A C3              mov         al,bl
  000000006507E831: 48 8B 9C 24 F0 00  mov         rbx,qword ptr [rsp+00000000000000F0h]
                    00 00
  000000006507E839: 48 81 C4 B0 00 00  add         rsp,0B0h
                    00
  000000006507E840: 41 5F              pop         r15
  000000006507E842: 41 5E              pop         r14
  000000006507E844: 41 5D              pop         r13
  000000006507E846: 41 5C              pop         r12
  000000006507E848: 5F                 pop         rdi
  000000006507E849: 5E                 pop         rsi
  000000006507E84A: 5D                 pop         rbp
  000000006507E84B: C3                 ret

  Summary

      695000 .text

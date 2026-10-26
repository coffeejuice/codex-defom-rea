Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065068490: 40 55              push        rbp
  0000000065068492: 56                 push        rsi
  0000000065068493: 57                 push        rdi
  0000000065068494: 48 83 EC 30        sub         rsp,30h
  0000000065068498: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650684A1: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000650684A6: 48 8B FA           mov         rdi,rdx
  00000000650684A9: 48 8B E9           mov         rbp,rcx
  00000000650684AC: 48 85 D2           test        rdx,rdx
  00000000650684AF: 75 12              jne         00000000650684C3
  00000000650684B1: 48 8D 0D 08 2D 64  lea         rcx,[00000000656AB1C0h]
                    00
  00000000650684B8: FF 15 DA E4 62 00  call        qword ptr [0000000065696998h]
  00000000650684BE: E9 2F 02 00 00     jmp         00000000650686F2
  00000000650684C3: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000000650684C7: 48 83 B8 A8 00 00  cmp         qword ptr [rax+00000000000000A8h],0
                    00 00
  00000000650684CF: 0F 84 BF 00 00 00  je          0000000065068594
  00000000650684D5: 48 39 90 A8 00 00  cmp         qword ptr [rax+00000000000000A8h],rdx
                    00
  00000000650684DC: 0F 84 10 02 00 00  je          00000000650686F2
  00000000650684E2: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000650684E7: FF 15 63 E9 62 00  call        qword ptr [0000000065696E50h]
  00000000650684ED: 90                 nop
  00000000650684EE: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000000650684F3: 48 8B C8           mov         rcx,rax
  00000000650684F6: FF 15 4C E9 62 00  call        qword ptr [0000000065696E48h]
  00000000650684FC: 48 8B D8           mov         rbx,rax
  00000000650684FF: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000065068504: 48 8B CF           mov         rcx,rdi
  0000000065068507: FF 15 43 E9 62 00  call        qword ptr [0000000065696E50h]
  000000006506850D: 90                 nop
  000000006506850E: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  0000000065068513: 48 8B C8           mov         rcx,rax
  0000000065068516: FF 15 2C E9 62 00  call        qword ptr [0000000065696E48h]
  000000006506851C: 48 8B F0           mov         rsi,rax
  000000006506851F: 48 8B CB           mov         rcx,rbx
  0000000065068522: FF 15 18 E9 62 00  call        qword ptr [0000000065696E40h]
  0000000065068528: 48 8B F8           mov         rdi,rax
  000000006506852B: 48 8B 55 00        mov         rdx,qword ptr [rbp]
  000000006506852F: 48 8B CD           mov         rcx,rbp
  0000000065068532: FF 12              call        qword ptr [rdx]
  0000000065068534: 48 8B C8           mov         rcx,rax
  0000000065068537: FF 15 2B E8 62 00  call        qword ptr [0000000065696D68h]
  000000006506853D: 48 8B D8           mov         rbx,rax
  0000000065068540: 48 8B CE           mov         rcx,rsi
  0000000065068543: FF 15 F7 E8 62 00  call        qword ptr [0000000065696E40h]
  0000000065068549: 48 8B D0           mov         rdx,rax
  000000006506854C: 4C 8B CF           mov         r9,rdi
  000000006506854F: 4C 8B C3           mov         r8,rbx
  0000000065068552: 48 8D 0D 07 2C 64  lea         rcx,[00000000656AB160h]
                    00
  0000000065068559: FF 15 39 E4 62 00  call        qword ptr [0000000065696998h]
  000000006506855F: 90                 nop
  0000000065068560: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000065068565: FF 15 E5 E4 62 00  call        qword ptr [0000000065696A50h]
  000000006506856B: 90                 nop
  000000006506856C: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000065068571: FF 15 E9 E1 62 00  call        qword ptr [0000000065696760h]
  0000000065068577: 90                 nop
  0000000065068578: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006506857D: FF 15 CD E4 62 00  call        qword ptr [0000000065696A50h]
  0000000065068583: 90                 nop
  0000000065068584: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000065068589: FF 15 D1 E1 62 00  call        qword ptr [0000000065696760h]
  000000006506858F: E9 5E 01 00 00     jmp         00000000650686F2
  0000000065068594: 48 8B CA           mov         rcx,rdx
  0000000065068597: FF 15 CB E2 62 00  call        qword ptr [0000000065696868h]
  000000006506859D: 48 8B D8           mov         rbx,rax
  00000000650685A0: 48 85 C0           test        rax,rax
  00000000650685A3: 0F 84 D5 00 00 00  je          000000006506867E
  00000000650685A9: 48 3B C5           cmp         rax,rbp
  00000000650685AC: 0F 84 CC 00 00 00  je          000000006506867E
  00000000650685B2: 48 8B C8           mov         rcx,rax
  00000000650685B5: FF 15 9D E2 62 00  call        qword ptr [0000000065696858h]
  00000000650685BB: 84 C0              test        al,al
  00000000650685BD: 74 0D              je          00000000650685CC
  00000000650685BF: 48 8B CB           mov         rcx,rbx
  00000000650685C2: E8 69 59 FF FF     call        000000006505DF30
  00000000650685C7: E9 B2 00 00 00     jmp         000000006506867E
  00000000650685CC: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000650685D1: 48 8B CD           mov         rcx,rbp
  00000000650685D4: FF 15 76 E8 62 00  call        qword ptr [0000000065696E50h]
  00000000650685DA: 90                 nop
  00000000650685DB: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000000650685E0: 48 8B C8           mov         rcx,rax
  00000000650685E3: FF 15 5F E8 62 00  call        qword ptr [0000000065696E48h]
  00000000650685E9: 48 8B D8           mov         rbx,rax
  00000000650685EC: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  00000000650685F1: 48 8B CF           mov         rcx,rdi
  00000000650685F4: FF 15 56 E8 62 00  call        qword ptr [0000000065696E50h]
  00000000650685FA: 90                 nop
  00000000650685FB: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  0000000065068600: 48 8B C8           mov         rcx,rax
  0000000065068603: FF 15 3F E8 62 00  call        qword ptr [0000000065696E48h]
  0000000065068609: 48 8B F0           mov         rsi,rax
  000000006506860C: 48 8B CB           mov         rcx,rbx
  000000006506860F: FF 15 2B E8 62 00  call        qword ptr [0000000065696E40h]
  0000000065068615: 48 8B F8           mov         rdi,rax
  0000000065068618: 48 8B 55 00        mov         rdx,qword ptr [rbp]
  000000006506861C: 48 8B CD           mov         rcx,rbp
  000000006506861F: FF 12              call        qword ptr [rdx]
  0000000065068621: 48 8B C8           mov         rcx,rax
  0000000065068624: FF 15 3E E7 62 00  call        qword ptr [0000000065696D68h]
  000000006506862A: 48 8B D8           mov         rbx,rax
  000000006506862D: 48 8B CE           mov         rcx,rsi
  0000000065068630: FF 15 0A E8 62 00  call        qword ptr [0000000065696E40h]
  0000000065068636: 48 8B D0           mov         rdx,rax
  0000000065068639: 4C 8B CF           mov         r9,rdi
  000000006506863C: 4C 8B C3           mov         r8,rbx
  000000006506863F: 48 8D 0D AA 2A 64  lea         rcx,[00000000656AB0F0h]
                    00
  0000000065068646: FF 15 4C E3 62 00  call        qword ptr [0000000065696998h]
  000000006506864C: 90                 nop
  000000006506864D: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000065068652: FF 15 F8 E3 62 00  call        qword ptr [0000000065696A50h]
  0000000065068658: 90                 nop
  0000000065068659: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006506865E: FF 15 FC E0 62 00  call        qword ptr [0000000065696760h]
  0000000065068664: 90                 nop
  0000000065068665: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006506866A: FF 15 E0 E3 62 00  call        qword ptr [0000000065696A50h]
  0000000065068670: 90                 nop
  0000000065068671: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000065068676: FF 15 E4 E0 62 00  call        qword ptr [0000000065696760h]
  000000006506867C: EB 74              jmp         00000000650686F2
  000000006506867E: 48 8B 75 08        mov         rsi,qword ptr [rbp+8]
  0000000065068682: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065068686: 83 88 9C 00 00 00  or          dword ptr [rax+000000000000009Ch],1
                    01
  000000006506868D: 48 89 BE A8 00 00  mov         qword ptr [rsi+00000000000000A8h],rdi
                    00
  0000000065068694: 48 3B DD           cmp         rbx,rbp
  0000000065068697: 74 22              je          00000000650686BB
  0000000065068699: 48 8B D5           mov         rdx,rbp
  000000006506869C: 48 8B CF           mov         rcx,rdi
  000000006506869F: FF 15 6B E4 62 00  call        qword ptr [0000000065696B10h]
  00000000650686A5: 48 8B D5           mov         rdx,rbp
  00000000650686A8: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  00000000650686AC: E8 0F CB FD FF     call        00000000650451C0
  00000000650686B1: 48 8D 4F 10        lea         rcx,[rdi+10h]
  00000000650686B5: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000650686B8: FF 50 58           call        qword ptr [rax+58h]
  00000000650686BB: 48 8B 45 20        mov         rax,qword ptr [rbp+20h]
  00000000650686BF: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  00000000650686C3: 74 2D              je          00000000650686F2
  00000000650686C5: 48 8B 86 88 00 00  mov         rax,qword ptr [rsi+0000000000000088h]
                    00
  00000000650686CC: 48 85 C0           test        rax,rax
  00000000650686CF: 74 21              je          00000000650686F2
  00000000650686D1: 48 83 78 08 00     cmp         qword ptr [rax+8],0
  00000000650686D6: 74 1A              je          00000000650686F2
  00000000650686D8: 48 8B CE           mov         rcx,rsi
  00000000650686DB: E8 A0 D3 FF FF     call        0000000065065A80
  00000000650686E0: 4C 8B 9E 88 00 00  mov         r11,qword ptr [rsi+0000000000000088h]
                    00
  00000000650686E7: 49 8B 43 08        mov         rax,qword ptr [r11+8]
  00000000650686EB: 81 60 7C FF FD FF  and         dword ptr [rax+7Ch],0FFFFFDFFh
                    FF
  00000000650686F2: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000650686F7: 48 83 C4 30        add         rsp,30h
  00000000650686FB: 5F                 pop         rdi
  00000000650686FC: 5E                 pop         rsi
  00000000650686FD: 5D                 pop         rbp
  00000000650686FE: C3                 ret

  Summary

      695000 .text

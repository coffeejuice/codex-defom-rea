Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140895350: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140895355: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014089535A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089535F: B8 88 34 00 00     mov         eax,3488h
  0000000140895364: E8 F7 04 F6 00     call        00000001417F5860
  0000000140895369: 48 2B E0           sub         rsp,rax
  000000014089536C: 48 C7 84 24 68 34  mov         qword ptr [rsp+0000000000003468h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140895378: 48 8B 05 B1 8C 2C  mov         rax,qword ptr [0000000143B5E030h]
                    03
  000000014089537F: 48 33 C4           xor         rax,rsp
  0000000140895382: 48 89 84 24 70 34  mov         qword ptr [rsp+0000000000003470h],rax
                    00 00
  000000014089538A: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140895393: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  000000014089539B: 48 83 BC 24 98 34  cmp         qword ptr [rsp+0000000000003498h],0
                    00 00 00
  00000001408953A4: 74 13              je          00000001408953B9
  00000001408953A6: 48 8B 8C 24 98 34  mov         rcx,qword ptr [rsp+0000000000003498h]
                    00 00
  00000001408953AE: E8 CD B7 BD 00     call        0000000141470B80
  00000001408953B3: 48 83 F8 01        cmp         rax,1
  00000001408953B7: 73 0A              jae         00000001408953C3
  00000001408953B9: B8 01 00 00 00     mov         eax,1
  00000001408953BE: E9 7B 01 00 00     jmp         000000014089553E
  00000001408953C3: 48 8B 8C 24 90 34  mov         rcx,qword ptr [rsp+0000000000003490h]
                    00 00
  00000001408953CB: E8 B0 15 00 00     call        0000000140896980
  00000001408953D0: 0F B6 C0           movzx       eax,al
  00000001408953D3: 85 C0              test        eax,eax
  00000001408953D5: 0F 84 D4 00 00 00  je          00000001408954AF
  00000001408953DB: BA 03 00 00 00     mov         edx,3
  00000001408953E0: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001408953E5: E8 06 C8 F8 FF     call        0000000140821BF0
  00000001408953EA: 90                 nop
  00000001408953EB: C6 84 24 64 34 00  mov         byte ptr [rsp+0000000000003464h],1
                    00 01
  00000001408953F3: 48 8D 94 24 64 34  lea         rdx,[rsp+0000000000003464h]
                    00 00
  00000001408953FB: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140895400: E8 3B 15 00 00     call        0000000140896940
  0000000140895405: C6 84 24 65 34 00  mov         byte ptr [rsp+0000000000003465h],1
                    00 01
  000000014089540D: 48 8D 94 24 65 34  lea         rdx,[rsp+0000000000003465h]
                    00 00
  0000000140895415: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014089541A: E8 41 15 00 00     call        0000000140896960
  000000014089541F: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140895424: 45 33 C9           xor         r9d,r9d
  0000000140895427: 48 8B 84 24 90 34  mov         rax,qword ptr [rsp+0000000000003490h]
                    00 00
  000000014089542F: 44 8B 80 44 07 00  mov         r8d,dword ptr [rax+0000000000000744h]
                    00
  0000000140895436: 48 8B 94 24 90 34  mov         rdx,qword ptr [rsp+0000000000003490h]
                    00 00
  000000014089543E: 48 8B 92 90 33 00  mov         rdx,qword ptr [rdx+0000000000003390h]
                    00
  0000000140895445: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014089544A: E8 F1 00 F9 FF     call        0000000140825540
  000000014089544F: 89 84 24 60 34 00  mov         dword ptr [rsp+0000000000003460h],eax
                    00
  0000000140895456: 83 BC 24 60 34 00  cmp         dword ptr [rsp+0000000000003460h],0
                    00 00
  000000014089545E: 75 38              jne         0000000140895498
  0000000140895460: 48 8D 15 25 6C F1  lea         rdx,[00000001427AC08Ch]
                    01
  0000000140895467: 48 8B 8C 24 98 34  mov         rcx,qword ptr [rsp+0000000000003498h]
                    00 00
  000000014089546F: E8 64 A8 BD 00     call        000000014146FCD8
  0000000140895474: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140895479: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  000000014089547E: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140895483: E8 A8 01 00 00     call        0000000140895630
  0000000140895488: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  000000014089548C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895491: E8 92 A6 BD 00     call        000000014146FB28
  0000000140895496: EB 08              jmp         00000001408954A0
  0000000140895498: C7 44 24 38 02 00  mov         dword ptr [rsp+38h],2
                    00 00
  00000001408954A0: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001408954A5: E8 B6 F0 F8 FF     call        0000000140824560
  00000001408954AA: E9 8B 00 00 00     jmp         000000014089553A
  00000001408954AF: 48 8B 8C 24 90 34  mov         rcx,qword ptr [rsp+0000000000003490h]
                    00 00
  00000001408954B7: 48 8B 84 24 A0 34  mov         rax,qword ptr [rsp+00000000000034A0h]
                    00 00
  00000001408954BF: 8B 00              mov         eax,dword ptr [rax]
  00000001408954C1: 89 81 44 17 00 00  mov         dword ptr [rcx+0000000000001744h],eax
  00000001408954C7: 48 8D 15 B6 6C F1  lea         rdx,[00000001427AC184h]
                    01
  00000001408954CE: 48 8B 8C 24 98 34  mov         rcx,qword ptr [rsp+0000000000003498h]
                    00 00
  00000001408954D6: E8 FD A7 BD 00     call        000000014146FCD8
  00000001408954DB: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001408954E0: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001408954E6: 75 07              jne         00000001408954EF
  00000001408954E8: B8 01 00 00 00     mov         eax,1
  00000001408954ED: EB 4F              jmp         000000014089553E
  00000001408954EF: 48 8B 84 24 A0 34  mov         rax,qword ptr [rsp+00000000000034A0h]
                    00 00
  00000001408954F7: 8B 00              mov         eax,dword ptr [rax]
  00000001408954F9: 25 00 00 00 80     and         eax,80000000h
  00000001408954FE: 85 C0              test        eax,eax
  0000000140895500: 75 18              jne         000000014089551A
  0000000140895502: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140895507: 48 8B 8C 24 90 34  mov         rcx,qword ptr [rsp+0000000000003490h]
                    00 00
  000000014089550F: E8 1C 01 00 00     call        0000000140895630
  0000000140895514: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140895518: EB 16              jmp         0000000140895530
  000000014089551A: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  000000014089551F: 48 8B 8C 24 90 34  mov         rcx,qword ptr [rsp+0000000000003490h]
                    00 00
  0000000140895527: E8 34 03 00 00     call        0000000140895860
  000000014089552C: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140895530: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895535: E8 EE A5 BD 00     call        000000014146FB28
  000000014089553A: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  000000014089553E: 48 8B 8C 24 70 34  mov         rcx,qword ptr [rsp+0000000000003470h]
                    00 00
  0000000140895546: 48 33 CC           xor         rcx,rsp
  0000000140895549: E8 C2 89 BD 00     call        000000014146DF10
  000000014089554E: 48 81 C4 88 34 00  add         rsp,3488h
                    00
  0000000140895555: C3                 ret

  Summary

     1E02000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065503610: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065503615: 57                 push        rdi
  0000000065503616: 48 83 EC 40        sub         rsp,40h
  000000006550361A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065503623: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  0000000065503628: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  000000006550362D: 48 8B F2           mov         rsi,rdx
  0000000065503630: 48 8B D9           mov         rbx,rcx
  0000000065503633: 48 8D 05 4E 24 23  lea         rax,[0000000065735A88h]
                    00
  000000006550363A: 48 89 01           mov         qword ptr [rcx],rax
  000000006550363D: 44 89 41 08        mov         dword ptr [rcx+8],r8d
  0000000065503641: 48 8B 05 F8 34 19  mov         rax,qword ptr [0000000065696B40h]
                    00
  0000000065503648: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  000000006550364C: 48 8B 0D ED 34 19  mov         rcx,qword ptr [0000000065696B40h]
                    00
  0000000065503653: FF 15 67 32 19 00  call        qword ptr [00000000656968C0h]
  0000000065503659: 90                 nop
  000000006550365A: 48 89 73 18        mov         qword ptr [rbx+18h],rsi
  000000006550365E: B9 18 00 00 00     mov         ecx,18h
  0000000065503663: E8 0A DB 0D 00     call        00000000655E1172
  0000000065503668: 48 8B F8           mov         rdi,rax
  000000006550366B: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000065503670: 48 85 C0           test        rax,rax
  0000000065503673: 74 24              je          0000000065503699
  0000000065503675: 48 89 18           mov         qword ptr [rax],rbx
  0000000065503678: 48 8B 0D C1 34 19  mov         rcx,qword ptr [0000000065696B40h]
                    00
  000000006550367F: 48 89 48 08        mov         qword ptr [rax+8],rcx
  0000000065503683: 48 8B 0D B6 34 19  mov         rcx,qword ptr [0000000065696B40h]
                    00
  000000006550368A: FF 15 30 32 19 00  call        qword ptr [00000000656968C0h]
  0000000065503690: C7 47 10 FF FF FF  mov         dword ptr [rdi+10h],0FFFFFFFFh
                    FF
  0000000065503697: EB 02              jmp         000000006550369B
  0000000065503699: 33 FF              xor         edi,edi
  000000006550369B: 48 89 7B 20        mov         qword ptr [rbx+20h],rdi
  000000006550369F: C7 43 28 35 00 00  mov         dword ptr [rbx+28h],35h
                    00
  00000000655036A6: 48 85 F6           test        rsi,rsi
  00000000655036A9: 74 69              je          0000000065503714
  00000000655036AB: 48 8B CE           mov         rcx,rsi
  00000000655036AE: E8 1D 16 FA FF     call        00000000654A4CD0
  00000000655036B3: 48 8B D0           mov         rdx,rax
  00000000655036B6: 48 8D 0D CB 5C 4C  lea         rcx,[00000000659C9388h]
                    00
  00000000655036BD: FF 15 45 33 19 00  call        qword ptr [0000000065696A08h]
  00000000655036C3: 48 8B F8           mov         rdi,rax
  00000000655036C6: 48 85 C0           test        rax,rax
  00000000655036C9: 74 44              je          000000006550370F
  00000000655036CB: C7 44 24 28 FF FF  mov         dword ptr [rsp+28h],0FFFFFFFFh
                    FF FF
  00000000655036D3: C7 44 24 2C FF FF  mov         dword ptr [rsp+2Ch],0FFFFFFFFh
                    FF FF
  00000000655036DB: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000000655036E4: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000000655036ED: 4C 8B 00           mov         r8,qword ptr [rax]
  00000000655036F0: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000655036F5: 48 8B C8           mov         rcx,rax
  00000000655036F8: 41 FF 50 68        call        qword ptr [r8+68h]
  00000000655036FC: 8B D0              mov         edx,eax
  00000000655036FE: 4C 8B C3           mov         r8,rbx
  0000000065503701: 48 8B CF           mov         rcx,rdi
  0000000065503704: E8 B7 F1 FF FF     call        00000000655028C0
  0000000065503709: 90                 nop
  000000006550370A: 48 8B C3           mov         rax,rbx
  000000006550370D: EB 08              jmp         0000000065503717
  000000006550370F: 48 8B C3           mov         rax,rbx
  0000000065503712: EB 03              jmp         0000000065503717
  0000000065503714: 48 8B C3           mov         rax,rbx
  0000000065503717: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  000000006550371C: 48 8B 74 24 68     mov         rsi,qword ptr [rsp+68h]
  0000000065503721: 48 83 C4 40        add         rsp,40h
  0000000065503725: 5F                 pop         rdi
  0000000065503726: C3                 ret

  Summary

      695000 .text

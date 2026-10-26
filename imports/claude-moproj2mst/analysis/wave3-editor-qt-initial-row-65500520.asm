Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065500520: 48 8B C4           mov         rax,rsp
  0000000065500523: 57                 push        rdi
  0000000065500524: 48 83 EC 70        sub         rsp,70h
  0000000065500528: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065500531: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  0000000065500535: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  0000000065500539: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  000000006550053D: 8B DA              mov         ebx,edx
  000000006550053F: 48 8B F1           mov         rsi,rcx
  0000000065500542: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  0000000065500546: 48 8B 97 80 02 00  mov         rdx,qword ptr [rdi+0000000000000280h]
                    00
  000000006550054D: 48 8D 0D 34 8E 4C  lea         rcx,[00000000659C9388h]
                    00
  0000000065500554: FF 15 AE 64 19 00  call        qword ptr [0000000065696A08h]
  000000006550055A: C7 44 24 38 FF FF  mov         dword ptr [rsp+38h],0FFFFFFFFh
                    FF FF
  0000000065500562: C7 44 24 3C FF FF  mov         dword ptr [rsp+3Ch],0FFFFFFFFh
                    FF FF
  000000006550056A: 33 ED              xor         ebp,ebp
  000000006550056C: 48 89 6C 24 40     mov         qword ptr [rsp+40h],rbp
  0000000065500571: 48 89 6C 24 48     mov         qword ptr [rsp+48h],rbp
  0000000065500576: 4C 8B 10           mov         r10,qword ptr [rax]
  0000000065500579: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006550057E: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  0000000065500583: 45 33 C9           xor         r9d,r9d
  0000000065500586: 44 8B C3           mov         r8d,ebx
  0000000065500589: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006550058E: 48 8B C8           mov         rcx,rax
  0000000065500591: 41 FF 52 58        call        qword ptr [r10+58h]
  0000000065500595: 90                 nop
  0000000065500596: 48 89 6C 24 40     mov         qword ptr [rsp+40h],rbp
  000000006550059B: 48 89 6C 24 48     mov         qword ptr [rsp+48h],rbp
  00000000655005A0: 8B 87 B0 02 00 00  mov         eax,dword ptr [rdi+00000000000002B0h]
  00000000655005A6: 83 F8 01           cmp         eax,1
  00000000655005A9: 75 2B              jne         00000000655005D6
  00000000655005AB: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],3
                    00 03 00 00 00
  00000000655005B6: 48 8B CE           mov         rcx,rsi
  00000000655005B9: E8 52 48 FA FF     call        00000000654A4E10
  00000000655005BE: 48 8B C8           mov         rcx,rax
  00000000655005C1: 4C 8D 84 24 80 00  lea         r8,[rsp+0000000000000080h]
                    00 00
  00000000655005C9: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000655005CE: E8 FD 0A FF FF     call        00000000654F10D0
  00000000655005D3: 90                 nop
  00000000655005D4: EB 54              jmp         000000006550062A
  00000000655005D6: 85 C0              test        eax,eax
  00000000655005D8: 75 27              jne         0000000065500601
  00000000655005DA: 89 AC 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],ebp
                    00
  00000000655005E1: 48 8B CE           mov         rcx,rsi
  00000000655005E4: E8 27 48 FA FF     call        00000000654A4E10
  00000000655005E9: 48 8B C8           mov         rcx,rax
  00000000655005EC: 4C 8D 84 24 80 00  lea         r8,[rsp+0000000000000080h]
                    00 00
  00000000655005F4: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000655005F9: E8 D2 0A FF FF     call        00000000654F10D0
  00000000655005FE: 90                 nop
  00000000655005FF: EB 29              jmp         000000006550062A
  0000000065500601: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],12h
                    00 12 00 00 00
  000000006550060C: 48 8B CE           mov         rcx,rsi
  000000006550060F: E8 FC 47 FA FF     call        00000000654A4E10
  0000000065500614: 48 8B C8           mov         rcx,rax
  0000000065500617: 4C 8D 84 24 80 00  lea         r8,[rsp+0000000000000080h]
                    00 00
  000000006550061F: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000065500624: E8 A7 0A FF FF     call        00000000654F10D0
  0000000065500629: 90                 nop
  000000006550062A: 4C 8D 5C 24 70     lea         r11,[rsp+70h]
  000000006550062F: 49 8B 5B 18        mov         rbx,qword ptr [r11+18h]
  0000000065500633: 49 8B 6B 20        mov         rbp,qword ptr [r11+20h]
  0000000065500637: 49 8B 73 28        mov         rsi,qword ptr [r11+28h]
  000000006550063B: 49 8B E3           mov         rsp,r11
  000000006550063E: 5F                 pop         rdi
  000000006550063F: C3                 ret

  Summary

      695000 .text

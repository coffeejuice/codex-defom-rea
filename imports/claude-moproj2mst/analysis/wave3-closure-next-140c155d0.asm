Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C155D0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C155D5: 48 81 EC C8 00 00  sub         rsp,0C8h
                    00
  0000000140C155DC: 48 C7 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140C155E8: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C155F0: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  0000000140C155F5: 74 5E              je          0000000140C15655
  0000000140C155F7: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C155FF: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  0000000140C15603: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C15608: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140C1560D: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C15612: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140C15618: 74 1F              je          0000000140C15639
  0000000140C1561A: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140C1561F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C15622: BA 01 00 00 00     mov         edx,1
  0000000140C15627: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140C1562C: FF 50 18           call        qword ptr [rax+18h]
  0000000140C1562F: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140C15637: EB 0C              jmp         0000000140C15645
  0000000140C15639: 48 C7 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],0
                    00 00 00 00 00 00
  0000000140C15645: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C1564D: 48 C7 40 18 00 00  mov         qword ptr [rax+18h],0
                    00 00
  0000000140C15655: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C1565D: 48 83 C1 10        add         rcx,10h
  0000000140C15661: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140C15666: E8 35 89 FF FF     call        0000000140C0DFA0
  0000000140C1566B: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C15673: 48 83 C1 10        add         rcx,10h
  0000000140C15677: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140C1567C: E8 5F 89 FF FF     call        0000000140C0DFE0
  0000000140C15681: 48 8B D0           mov         rdx,rax
  0000000140C15684: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140C15689: E8 E2 0D 51 FF     call        0000000140126470
  0000000140C1568E: 0F B6 C0           movzx       eax,al
  0000000140C15691: 85 C0              test        eax,eax
  0000000140C15693: 0F 84 48 01 00 00  je          0000000140C157E1
  0000000140C15699: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140C1569E: E8 0D 1F 1C 00     call        0000000140DD75B0
  0000000140C156A3: 48 8B C8           mov         rcx,rax
  0000000140C156A6: E8 45 58 00 00     call        0000000140C1AEF0
  0000000140C156AB: 48 85 C0           test        rax,rax
  0000000140C156AE: 75 55              jne         0000000140C15705
  0000000140C156B0: 48 8D 44 24 50     lea         rax,[rsp+50h]
  0000000140C156B5: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C156BA: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140C156BF: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C156C4: E8 97 64 04 00     call        0000000140C5BB60
  0000000140C156C9: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C156D1: 48 83 C1 10        add         rcx,10h
  0000000140C156D5: 4C 8B C0           mov         r8,rax
  0000000140C156D8: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000140C156DD: E8 3E 89 FF FF     call        0000000140C0E020
  0000000140C156E2: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C156EA: 48 83 C1 10        add         rcx,10h
  0000000140C156EE: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  0000000140C156F3: E8 A8 88 FF FF     call        0000000140C0DFA0
  0000000140C156F8: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C156FB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C15700: E9 66 FF FF FF     jmp         0000000140C1566B
  0000000140C15705: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140C1570A: E8 A1 1E 1C 00     call        0000000140DD75B0
  0000000140C1570F: 48 8B D0           mov         rdx,rax
  0000000140C15712: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140C15717: E8 E4 57 00 00     call        0000000140C1AF00
  0000000140C1571C: 90                 nop
  0000000140C1571D: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140C15722: E8 C9 57 00 00     call        0000000140C1AEF0
  0000000140C15727: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C1572C: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C15731: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140C15736: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  0000000140C1573B: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140C15740: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140C15746: 74 1F              je          0000000140C15767
  0000000140C15748: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140C1574D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C15750: BA 01 00 00 00     mov         edx,1
  0000000140C15755: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140C1575A: FF 50 18           call        qword ptr [rax+18h]
  0000000140C1575D: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140C15765: EB 0C              jmp         0000000140C15773
  0000000140C15767: 48 C7 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],0
                    00 00 00 00 00 00
  0000000140C15773: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  0000000140C1577B: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140C15783: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140C15788: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140C15790: E8 CB 63 04 00     call        0000000140C5BB60
  0000000140C15795: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C1579D: 48 83 C1 10        add         rcx,10h
  0000000140C157A1: 4C 8B C0           mov         r8,rax
  0000000140C157A4: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140C157AC: E8 6F 88 FF FF     call        0000000140C0E020
  0000000140C157B1: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C157B9: 48 83 C1 10        add         rcx,10h
  0000000140C157BD: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  0000000140C157C5: E8 D6 87 FF FF     call        0000000140C0DFA0
  0000000140C157CA: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C157CD: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C157D2: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140C157D7: E8 24 71 0A 00     call        0000000140CBC900
  0000000140C157DC: E9 8A FE FF FF     jmp         0000000140C1566B
  0000000140C157E1: 48 81 C4 C8 00 00  add         rsp,0C8h
                    00
  0000000140C157E8: C3                 ret

  Summary

     1E02000 .text

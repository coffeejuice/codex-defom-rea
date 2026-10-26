Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704C460: 48 8B C4           mov         rax,rsp
  000000006704C463: 56                 push        rsi
  000000006704C464: 57                 push        rdi
  000000006704C465: 41 54              push        r12
  000000006704C467: 48 81 EC 60 01 00  sub         rsp,160h
                    00
  000000006704C46E: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006704C477: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006704C47B: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  000000006704C47F: 49 8B D9           mov         rbx,r9
  000000006704C482: 45 8B E0           mov         r12d,r8d
  000000006704C485: 48 8B C2           mov         rax,rdx
  000000006704C488: 48 8B E9           mov         rbp,rcx
  000000006704C48B: 33 FF              xor         edi,edi
  000000006704C48D: 8B F7              mov         esi,edi
  000000006704C48F: 89 7C 24 20        mov         dword ptr [rsp+20h],edi
  000000006704C493: 89 7C 24 44        mov         dword ptr [rsp+44h],edi
  000000006704C497: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006704C49C: 48 89 4C 24 48     mov         qword ptr [rsp+48h],rcx
  000000006704C4A1: C7 44 24 40 00 01  mov         dword ptr [rsp+40h],100h
                    00 00
  000000006704C4A9: B9 A0 00 00 00     mov         ecx,0A0h
  000000006704C4AE: 66 39 4D 08        cmp         word ptr [rbp+8],cx
  000000006704C4B2: 75 17              jne         000000006704C4CB
  000000006704C4B4: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006704C4B9: 48 8B C8           mov         rcx,rax
  000000006704C4BC: E8 DF 7F 02 00     call        00000000670744A0
  000000006704C4C1: 90                 nop
  000000006704C4C2: BE 01 00 00 00     mov         esi,1
  000000006704C4C7: 89 74 24 20        mov         dword ptr [rsp+20h],esi
  000000006704C4CB: 4C 8D 4C 24 40     lea         r9,[rsp+40h]
  000000006704C4D0: 44 8B 84 24 A0 01  mov         r8d,dword ptr [rsp+00000000000001A0h]
                    00 00
  000000006704C4D8: 48 8B D0           mov         rdx,rax
  000000006704C4DB: 48 8B CD           mov         rcx,rbp
  000000006704C4DE: E8 AD FC FF FF     call        000000006704C190
  000000006704C4E3: 84 C0              test        al,al
  000000006704C4E5: 40 0F 94 C5        sete        bpl
  000000006704C4E9: 40 F6 C6 01        test        sil,1
  000000006704C4ED: 74 15              je          000000006704C504
  000000006704C4EF: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  000000006704C4F4: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006704C4F8: 75 0A              jne         000000006704C504
  000000006704C4FA: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000006704C4FF: E8 CC 3B 02 00     call        00000000670700D0
  000000006704C504: 40 84 ED           test        bpl,bpl
  000000006704C507: 74 23              je          000000006704C52C
  000000006704C509: 48 85 DB           test        rbx,rbx
  000000006704C50C: 74 03              je          000000006704C511
  000000006704C50E: C6 03 00           mov         byte ptr [rbx],0
  000000006704C511: 48 8D 44 24 50     lea         rax,[rsp+50h]
  000000006704C516: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006704C51B: 48 3B C8           cmp         rcx,rax
  000000006704C51E: 74 05              je          000000006704C525
  000000006704C520: E8 8B 3E FD FF     call        00000000670203B0
  000000006704C525: 33 C0              xor         eax,eax
  000000006704C527: E9 80 00 00 00     jmp         000000006704C5AC
  000000006704C52C: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006704C531: 80 39 00           cmp         byte ptr [rcx],0
  000000006704C534: 75 0A              jne         000000006704C540
  000000006704C536: 48 85 DB           test        rbx,rbx
  000000006704C539: 74 5F              je          000000006704C59A
  000000006704C53B: C6 03 00           mov         byte ptr [rbx],0
  000000006704C53E: EB 5A              jmp         000000006704C59A
  000000006704C540: 4C 8D 8C 24 80 01  lea         r9,[rsp+0000000000000180h]
                    00 00
  000000006704C548: 45 8B C4           mov         r8d,r12d
  000000006704C54B: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006704C550: E8 CB 6D 00 00     call        0000000067053320
  000000006704C555: 4C 8B D8           mov         r11,rax
  000000006704C558: 80 BC 24 80 01 00  cmp         byte ptr [rsp+0000000000000180h],0
                    00 00
  000000006704C560: 75 0F              jne         000000006704C571
  000000006704C562: 48 85 DB           test        rbx,rbx
  000000006704C565: 74 03              je          000000006704C56A
  000000006704C567: C6 03 00           mov         byte ptr [rbx],0
  000000006704C56A: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006704C56F: EB 29              jmp         000000006704C59A
  000000006704C571: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000006704C576: 80 38 00           cmp         byte ptr [rax],0
  000000006704C579: 74 0F              je          000000006704C58A
  000000006704C57B: 48 85 DB           test        rbx,rbx
  000000006704C57E: 74 03              je          000000006704C583
  000000006704C580: C6 03 00           mov         byte ptr [rbx],0
  000000006704C583: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006704C588: EB 10              jmp         000000006704C59A
  000000006704C58A: 48 85 DB           test        rbx,rbx
  000000006704C58D: 74 03              je          000000006704C592
  000000006704C58F: C6 03 01           mov         byte ptr [rbx],1
  000000006704C592: 49 8B FB           mov         rdi,r11
  000000006704C595: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006704C59A: 48 8D 44 24 50     lea         rax,[rsp+50h]
  000000006704C59F: 48 3B C8           cmp         rcx,rax
  000000006704C5A2: 74 05              je          000000006704C5A9
  000000006704C5A4: E8 07 3E FD FF     call        00000000670203B0
  000000006704C5A9: 48 8B C7           mov         rax,rdi
  000000006704C5AC: 4C 8D 9C 24 60 01  lea         r11,[rsp+0000000000000160h]
                    00 00
  000000006704C5B4: 49 8B 5B 28        mov         rbx,qword ptr [r11+28h]
  000000006704C5B8: 49 8B 6B 30        mov         rbp,qword ptr [r11+30h]
  000000006704C5BC: 49 8B E3           mov         rsp,r11
  000000006704C5BF: 41 5C              pop         r12
  000000006704C5C1: 5F                 pop         rdi
  000000006704C5C2: 5E                 pop         rsi
  000000006704C5C3: C3                 ret
  000000006704C5C4: CC                 int         3
  000000006704C5C5: CC                 int         3
  000000006704C5C6: CC                 int         3
  000000006704C5C7: CC                 int         3
  000000006704C5C8: CC                 int         3
  000000006704C5C9: CC                 int         3
  000000006704C5CA: CC                 int         3
  000000006704C5CB: CC                 int         3
  000000006704C5CC: CC                 int         3
  000000006704C5CD: CC                 int         3
  000000006704C5CE: CC                 int         3
  000000006704C5CF: CC                 int         3
  000000006704C5D0: 48 8B C4           mov         rax,rsp
  000000006704C5D3: 57                 push        rdi
  000000006704C5D4: 48 81 EC 60 01 00  sub         rsp,160h
                    00
  000000006704C5DB: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006704C5E4: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006704C5E8: 48 89 68 18        mov         qword ptr [rax+18h],rbp
  000000006704C5EC: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  000000006704C5F0: 49 8B D9           mov         rbx,r9
  000000006704C5F3: 41 8B E8           mov         ebp,r8d
  000000006704C5F6: 48 8B C2           mov         rax,rdx
  000000006704C5F9: 48 8B F1           mov         rsi,rcx
  000000006704C5FC: 33 FF              xor         edi,edi
  000000006704C5FE: 89 7C 24 20        mov         dword ptr [rsp+20h],edi
  000000006704C602: 89 7C 24 44        mov         dword ptr [rsp+44h],edi
  000000006704C606: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006704C60B: 48 89 4C 24 48     mov         qword ptr [rsp+48h],rcx
  000000006704C610: C7 44 24 40 00 01  mov         dword ptr [rsp+40h],100h
                    00 00
  000000006704C618: B9 A0 00 00 00     mov         ecx,0A0h
  000000006704C61D: 66 39 4E 08        cmp         word ptr [rsi+8],cx

  Summary

      1A7000 .text

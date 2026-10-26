Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  000000006100C760: 40 56              push        rsi
  000000006100C762: 57                 push        rdi
  000000006100C763: 41 54              push        r12
  000000006100C765: 48 83 EC 50        sub         rsp,50h
  000000006100C769: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006100C772: 48 89 5C 24 78     mov         qword ptr [rsp+78h],rbx
  000000006100C777: 48 89 AC 24 80 00  mov         qword ptr [rsp+0000000000000080h],rbp
                    00 00
  000000006100C77F: 48 8B E9           mov         rbp,rcx
  000000006100C782: 80 79 61 00        cmp         byte ptr [rcx+61h],0
  000000006100C786: 74 11              je          000000006100C799
  000000006100C788: 4D 8B C1           mov         r8,r9
  000000006100C78B: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  000000006100C78F: E8 CC E5 FF FF     call        000000006100AD60
  000000006100C794: 48 8B F8           mov         rdi,rax
  000000006100C797: EB 0F              jmp         000000006100C7A8
  000000006100C799: 49 8B D1           mov         rdx,r9
  000000006100C79C: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  000000006100C7A0: E8 DB E4 FF FF     call        000000006100AC80
  000000006100C7A5: 48 8B F8           mov         rdi,rax
  000000006100C7A8: 48 85 FF           test        rdi,rdi
  000000006100C7AB: 75 07              jne         000000006100C7B4
  000000006100C7AD: 32 C0              xor         al,al
  000000006100C7AF: E9 20 01 00 00     jmp         000000006100C8D4
  000000006100C7B4: 48 8B 4D 68        mov         rcx,qword ptr [rbp+68h]
  000000006100C7B8: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006100C7BB: FF 50 08           call        qword ptr [rax+8]
  000000006100C7BE: 8B D8              mov         ebx,eax
  000000006100C7C0: 48 8B 4D 68        mov         rcx,qword ptr [rbp+68h]
  000000006100C7C4: 48 8B 11           mov         rdx,qword ptr [rcx]
  000000006100C7C7: FF 52 10           call        qword ptr [rdx+10h]
  000000006100C7CA: 89 47 5C           mov         dword ptr [rdi+5Ch],eax
  000000006100C7CD: 89 5F 60           mov         dword ptr [rdi+60h],ebx
  000000006100C7D0: 48 8B 4D 50        mov         rcx,qword ptr [rbp+50h]
  000000006100C7D4: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006100C7D7: 48 8B D7           mov         rdx,rdi
  000000006100C7DA: FF 50 30           call        qword ptr [rax+30h]
  000000006100C7DD: 48 89 7D 50        mov         qword ptr [rbp+50h],rdi
  000000006100C7E1: 33 F6              xor         esi,esi
  000000006100C7E3: 4C 8B A4 24 90 00  mov         r12,qword ptr [rsp+0000000000000090h]
                    00 00
  000000006100C7EB: 49 8B CC           mov         rcx,r12
  000000006100C7EE: E8 1D 3F 00 00     call        0000000061010710
  000000006100C7F3: 85 C0              test        eax,eax
  000000006100C7F5: 0F 8E D7 00 00 00  jle         000000006100C8D2
  000000006100C7FB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  000000006100C800: 80 7D 61 00        cmp         byte ptr [rbp+61h],0
  000000006100C804: 74 6F              je          000000006100C875
  000000006100C806: 44 8B C6           mov         r8d,esi
  000000006100C809: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006100C80E: 49 8B CC           mov         rcx,r12
  000000006100C811: E8 6A 4B 00 00     call        0000000061011380
  000000006100C816: 48 8B F8           mov         rdi,rax
  000000006100C819: 44 8B C6           mov         r8d,esi
  000000006100C81C: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000006100C821: 49 8B CC           mov         rcx,r12
  000000006100C824: E8 D7 4A 00 00     call        0000000061011300
  000000006100C829: 48 8B D8           mov         rbx,rax
  000000006100C82C: 44 8B C6           mov         r8d,esi
  000000006100C82F: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  000000006100C834: 49 8B CC           mov         rcx,r12
  000000006100C837: E8 04 4B 00 00     call        0000000061011340
  000000006100C83C: 90                 nop
  000000006100C83D: 4C 8B CF           mov         r9,rdi
  000000006100C840: 4C 8B C3           mov         r8,rbx
  000000006100C843: 48 8B D0           mov         rdx,rax
  000000006100C846: 48 8B 4D 50        mov         rcx,qword ptr [rbp+50h]
  000000006100C84A: E8 C1 F5 FF FF     call        000000006100BE10
  000000006100C84F: 90                 nop
  000000006100C850: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000006100C855: FF 15 85 70 03 00  call        qword ptr [00000000610438E0h]
  000000006100C85B: 90                 nop
  000000006100C85C: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006100C861: FF 15 79 70 03 00  call        qword ptr [00000000610438E0h]
  000000006100C867: 90                 nop
  000000006100C868: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006100C86D: FF 15 6D 70 03 00  call        qword ptr [00000000610438E0h]
  000000006100C873: EB 4B              jmp         000000006100C8C0
  000000006100C875: 44 8B C6           mov         r8d,esi
  000000006100C878: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006100C87D: 49 8B CC           mov         rcx,r12
  000000006100C880: E8 FB 4A 00 00     call        0000000061011380
  000000006100C885: 48 8B D8           mov         rbx,rax
  000000006100C888: 44 8B C6           mov         r8d,esi
  000000006100C88B: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006100C890: 49 8B CC           mov         rcx,r12
  000000006100C893: E8 68 4A 00 00     call        0000000061011300
  000000006100C898: 90                 nop
  000000006100C899: 4C 8B C3           mov         r8,rbx
  000000006100C89C: 48 8B D0           mov         rdx,rax
  000000006100C89F: 48 8B 4D 50        mov         rcx,qword ptr [rbp+50h]
  000000006100C8A3: E8 78 F4 FF FF     call        000000006100BD20
  000000006100C8A8: 90                 nop
  000000006100C8A9: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006100C8AE: FF 15 2C 70 03 00  call        qword ptr [00000000610438E0h]
  000000006100C8B4: 90                 nop
  000000006100C8B5: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006100C8BA: FF 15 20 70 03 00  call        qword ptr [00000000610438E0h]
  000000006100C8C0: FF C6              inc         esi
  000000006100C8C2: 49 8B CC           mov         rcx,r12
  000000006100C8C5: E8 46 3E 00 00     call        0000000061010710
  000000006100C8CA: 3B F0              cmp         esi,eax
  000000006100C8CC: 0F 8C 2E FF FF FF  jl          000000006100C800
  000000006100C8D2: B0 01              mov         al,1
  000000006100C8D4: 48 8B 5C 24 78     mov         rbx,qword ptr [rsp+78h]
  000000006100C8D9: 48 8B AC 24 80 00  mov         rbp,qword ptr [rsp+0000000000000080h]
                    00 00
  000000006100C8E1: 48 83 C4 50        add         rsp,50h
  000000006100C8E5: 41 5C              pop         r12
  000000006100C8E7: 5F                 pop         rdi
  000000006100C8E8: 5E                 pop         rsi
  000000006100C8E9: C3                 ret

  Summary

       42000 .text

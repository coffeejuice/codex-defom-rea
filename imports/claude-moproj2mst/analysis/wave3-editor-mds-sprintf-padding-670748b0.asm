Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670748B0: 66 44 89 4C 24 20  mov         word ptr [rsp+20h],r9w
  00000000670748B6: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000000670748BB: 53                 push        rbx
  00000000670748BC: 55                 push        rbp
  00000000670748BD: 56                 push        rsi
  00000000670748BE: 57                 push        rdi
  00000000670748BF: 48 83 EC 38        sub         rsp,38h
  00000000670748C3: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670748CC: 48 8B DA           mov         rbx,rdx
  00000000670748CF: 48 8B E9           mov         rbp,rcx
  00000000670748D2: C7 44 24 70 00 00  mov         dword ptr [rsp+70h],0
                    00 00
  00000000670748DA: 48 8D 05 4F C2 27  lea         rax,[00000000672F0B30h]
                    00
  00000000670748E1: 48 89 02           mov         qword ptr [rdx],rax
  00000000670748E4: F0 83 05 44 C2 27  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670748EC: C7 44 24 70 01 00  mov         dword ptr [rsp+70h],1
                    00 00
  00000000670748F4: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670748F7: 48 63 70 08        movsxd      rsi,dword ptr [rax+8]
  00000000670748FB: 41 8B F8           mov         edi,r8d
  00000000670748FE: 2B FE              sub         edi,esi
  0000000067074900: 85 FF              test        edi,edi
  0000000067074902: 7E 4E              jle         0000000067074952
  0000000067074904: 8D 14 37           lea         edx,[rdi+rsi]
  0000000067074907: 48 8B CB           mov         rcx,rbx
  000000006707490A: E8 B1 E0 FF FF     call        00000000670729C0
  000000006707490F: 4C 8B 1B           mov         r11,qword ptr [rbx]
  0000000067074912: 49 8B 4B 10        mov         rcx,qword ptr [r11+10h]
  0000000067074916: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  0000000067074920: 0F B7 44 24 78     movzx       eax,word ptr [rsp+78h]
  0000000067074925: 66 89 01           mov         word ptr [rcx],ax
  0000000067074928: 48 83 C1 02        add         rcx,2
  000000006707492C: 83 EF 01           sub         edi,1
  000000006707492F: 75 EF              jne         0000000067074920
  0000000067074931: 85 F6              test        esi,esi
  0000000067074933: 74 18              je          000000006707494D
  0000000067074935: 4C 8B C6           mov         r8,rsi
  0000000067074938: 4D 03 C0           add         r8,r8
  000000006707493B: 48 8B 55 00        mov         rdx,qword ptr [rbp]
  000000006707493F: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  0000000067074943: E8 56 41 10 00     call        0000000067178A9E
  0000000067074948: 48 8B C3           mov         rax,rbx
  000000006707494B: EB 78              jmp         00000000670749C5
  000000006707494D: 48 8B C3           mov         rax,rbx
  0000000067074950: EB 73              jmp         00000000670749C5
  0000000067074952: 80 BC 24 80 00 00  cmp         byte ptr [rsp+0000000000000080h],0
                    00 00
  000000006707495A: 74 4A              je          00000000670749A6
  000000006707495C: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000067074961: E8 8A F5 FF FF     call        0000000067073EF0
  0000000067074966: 48 8B F0           mov         rsi,rax
  0000000067074969: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006707496C: F0 83 01 01        lock add    dword ptr [rcx],1
  0000000067074970: 48 8B 13           mov         rdx,qword ptr [rbx]
  0000000067074973: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  0000000067074977: 75 08              jne         0000000067074981
  0000000067074979: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006707497C: E8 4F B7 FF FF     call        00000000670700D0
  0000000067074981: 48 8B 06           mov         rax,qword ptr [rsi]
  0000000067074984: 48 89 03           mov         qword ptr [rbx],rax
  0000000067074987: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000006707498C: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067074990: 75 0F              jne         00000000670749A1
  0000000067074992: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000067074997: E8 34 B7 FF FF     call        00000000670700D0
  000000006707499C: 48 8B C3           mov         rax,rbx
  000000006707499F: EB 24              jmp         00000000670749C5
  00000000670749A1: 48 8B C3           mov         rax,rbx
  00000000670749A4: EB 1F              jmp         00000000670749C5
  00000000670749A6: F0 83 00 01        lock add    dword ptr [rax],1
  00000000670749AA: 48 8B 02           mov         rax,qword ptr [rdx]
  00000000670749AD: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670749B1: 75 08              jne         00000000670749BB
  00000000670749B3: 48 8B 0A           mov         rcx,qword ptr [rdx]
  00000000670749B6: E8 15 B7 FF FF     call        00000000670700D0
  00000000670749BB: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000000670749BF: 48 89 03           mov         qword ptr [rbx],rax
  00000000670749C2: 48 8B C3           mov         rax,rbx
  00000000670749C5: 48 83 C4 38        add         rsp,38h
  00000000670749C9: 5F                 pop         rdi
  00000000670749CA: 5E                 pop         rsi
  00000000670749CB: 5D                 pop         rbp
  00000000670749CC: 5B                 pop         rbx
  00000000670749CD: C3                 ret
  00000000670749CE: CC                 int         3

  Summary

      1A7000 .text

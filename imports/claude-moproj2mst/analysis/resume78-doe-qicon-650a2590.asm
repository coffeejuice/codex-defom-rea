Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650A2590: 40 57              push        rdi
  00000000650A2592: 41 54              push        r12
  00000000650A2594: 41 55              push        r13
  00000000650A2596: 48 83 EC 50        sub         rsp,50h
  00000000650A259A: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650A25A3: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  00000000650A25A8: 48 89 6C 24 78     mov         qword ptr [rsp+78h],rbp
  00000000650A25AD: 48 89 B4 24 80 00  mov         qword ptr [rsp+0000000000000080h],rsi
                    00 00
  00000000650A25B5: 45 8B E1           mov         r12d,r9d
  00000000650A25B8: 4D 8B E8           mov         r13,r8
  00000000650A25BB: 48 8B EA           mov         rbp,rdx
  00000000650A25BE: 48 8B D9           mov         rbx,rcx
  00000000650A25C1: 48 8B CA           mov         rcx,rdx
  00000000650A25C4: FF 15 1E 44 5F 00  call        qword ptr [00000000656969E8h]
  00000000650A25CA: 84 C0              test        al,al
  00000000650A25CC: 0F 85 EB 01 00 00  jne         00000000650A27BD
  00000000650A25D2: 48 83 3B 00        cmp         qword ptr [rbx],0
  00000000650A25D6: 0F 85 B8 01 00 00  jne         00000000650A2794
  00000000650A25DC: 48 8B D5           mov         rdx,rbp
  00000000650A25DF: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000650A25E4: FF 15 8E 4D 5F 00  call        qword ptr [0000000065697378h]
  00000000650A25EA: 90                 nop
  00000000650A25EB: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000650A25F0: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000650A25F5: FF 15 95 52 5F 00  call        qword ptr [0000000065697890h]
  00000000650A25FB: 90                 nop
  00000000650A25FC: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000650A2601: FF 15 E1 43 5F 00  call        qword ptr [00000000656969E8h]
  00000000650A2607: 33 FF              xor         edi,edi
  00000000650A2609: 84 C0              test        al,al
  00000000650A260B: 0F 85 1B 01 00 00  jne         00000000650A272C
  00000000650A2611: E8 6A F8 FF FF     call        00000000650A1E80
  00000000650A2616: 48 8B C8           mov         rcx,rax
  00000000650A2619: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000650A261E: FF 15 54 4F 5F 00  call        qword ptr [0000000065697578h]
  00000000650A2624: 48 85 C0           test        rax,rax
  00000000650A2627: 74 50              je          00000000650A2679
  00000000650A2629: 4C 8B 00           mov         r8,qword ptr [rax]
  00000000650A262C: 48 8D 15 2D EC 60  lea         rdx,[00000000656B1260h]
                    00
  00000000650A2633: 48 8B C8           mov         rcx,rax
  00000000650A2636: 41 FF 50 08        call        qword ptr [r8+8]
  00000000650A263A: 48 85 C0           test        rax,rax
  00000000650A263D: 74 3A              je          00000000650A2679
  00000000650A263F: 4C 8B 00           mov         r8,qword ptr [rax]
  00000000650A2642: 48 8B D5           mov         rdx,rbp
  00000000650A2645: 48 8B C8           mov         rcx,rax
  00000000650A2648: 41 FF 50 10        call        qword ptr [r8+10h]
  00000000650A264C: 48 8B F0           mov         rsi,rax
  00000000650A264F: 48 85 C0           test        rax,rax
  00000000650A2652: 74 25              je          00000000650A2679
  00000000650A2654: 8D 4F 20           lea         ecx,[rdi+20h]
  00000000650A2657: E8 16 EB 53 00     call        00000000655E1172
  00000000650A265C: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000000650A2661: 48 85 C0           test        rax,rax
  00000000650A2664: 74 0A              je          00000000650A2670
  00000000650A2666: 48 8B C8           mov         rcx,rax
  00000000650A2669: E8 E2 DE FF FF     call        00000000650A0550
  00000000650A266E: EB 03              jmp         00000000650A2673
  00000000650A2670: 48 8B C7           mov         rax,rdi
  00000000650A2673: 48 89 03           mov         qword ptr [rbx],rax
  00000000650A2676: 48 89 30           mov         qword ptr [rax],rsi
  00000000650A2679: 48 83 3B 00        cmp         qword ptr [rbx],0
  00000000650A267D: 0F 85 F8 00 00 00  jne         00000000650A277B
  00000000650A2683: E8 88 F6 FF FF     call        00000000650A1D10
  00000000650A2688: 48 8B C8           mov         rcx,rax
  00000000650A268B: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000000650A2690: FF 15 E2 4E 5F 00  call        qword ptr [0000000065697578h]
  00000000650A2696: 48 85 C0           test        rax,rax
  00000000650A2699: 0F 84 8D 00 00 00  je          00000000650A272C
  00000000650A269F: 4C 8B 00           mov         r8,qword ptr [rax]
  00000000650A26A2: 48 8D 15 67 EB 60  lea         rdx,[00000000656B1210h]
                    00
  00000000650A26A9: 48 8B C8           mov         rcx,rax
  00000000650A26AC: 41 FF 50 08        call        qword ptr [r8+8]
  00000000650A26B0: 48 85 C0           test        rax,rax
  00000000650A26B3: 74 77              je          00000000650A272C
  00000000650A26B5: 4C 8B 00           mov         r8,qword ptr [rax]
  00000000650A26B8: 48 8B D5           mov         rdx,rbp
  00000000650A26BB: 48 8B C8           mov         rcx,rax
  00000000650A26BE: 41 FF 50 10        call        qword ptr [r8+10h]
  00000000650A26C2: 48 8B F0           mov         rsi,rax
  00000000650A26C5: 48 85 C0           test        rax,rax
  00000000650A26C8: 74 62              je          00000000650A272C
  00000000650A26CA: B9 20 00 00 00     mov         ecx,20h
  00000000650A26CF: E8 9E EA 53 00     call        00000000655E1172
  00000000650A26D4: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000000650A26D9: 48 85 C0           test        rax,rax
  00000000650A26DC: 74 0A              je          00000000650A26E8
  00000000650A26DE: 48 8B C8           mov         rcx,rax
  00000000650A26E1: E8 6A DE FF FF     call        00000000650A0550
  00000000650A26E6: EB 03              jmp         00000000650A26EB
  00000000650A26E8: 48 8B C7           mov         rax,rdi
  00000000650A26EB: 48 89 03           mov         qword ptr [rbx],rax
  00000000650A26EE: 48 89 30           mov         qword ptr [rax],rsi
  00000000650A26F1: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650A26F4: C7 40 14 01 00 00  mov         dword ptr [rax+14h],1
                    00
  00000000650A26FB: B9 04 00 00 00     mov         ecx,4
  00000000650A2700: E8 6D EA 53 00     call        00000000655E1172
  00000000650A2705: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000000650A270A: 48 85 C0           test        rax,rax
  00000000650A270D: 74 13              je          00000000650A2722
  00000000650A270F: BA 01 00 00 00     mov         edx,1
  00000000650A2714: 48 8B C8           mov         rcx,rax
  00000000650A2717: FF 15 5B 49 5F 00  call        qword ptr [0000000065697078h]
  00000000650A271D: 4C 8B D8           mov         r11,rax
  00000000650A2720: EB 03              jmp         00000000650A2725
  00000000650A2722: 4C 8B DF           mov         r11,rdi
  00000000650A2725: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650A2728: 4C 89 58 18        mov         qword ptr [rax+18h],r11
  00000000650A272C: 48 83 3B 00        cmp         qword ptr [rbx],0
  00000000650A2730: 75 49              jne         00000000650A277B
  00000000650A2732: B9 20 00 00 00     mov         ecx,20h
  00000000650A2737: E8 36 EA 53 00     call        00000000655E1172
  00000000650A273C: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000000650A2741: 48 85 C0           test        rax,rax
  00000000650A2744: 74 0A              je          00000000650A2750
  00000000650A2746: 48 8B C8           mov         rcx,rax
  00000000650A2749: E8 02 DE FF FF     call        00000000650A0550
  00000000650A274E: EB 03              jmp         00000000650A2753
  00000000650A2750: 48 8B C7           mov         rax,rdi
  00000000650A2753: 48 89 03           mov         qword ptr [rbx],rax
  00000000650A2756: B9 10 00 00 00     mov         ecx,10h
  00000000650A275B: E8 12 EA 53 00     call        00000000655E1172
  00000000650A2760: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000000650A2765: 48 85 C0           test        rax,rax
  00000000650A2768: 74 0B              je          00000000650A2775
  00000000650A276A: 48 8B C8           mov         rcx,rax
  00000000650A276D: E8 FE FC FF FF     call        00000000650A2470
  00000000650A2772: 48 8B F8           mov         rdi,rax
  00000000650A2775: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650A2778: 48 89 38           mov         qword ptr [rax],rdi
  00000000650A277B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000650A2780: FF 15 DA 3F 5F 00  call        qword ptr [0000000065696760h]
  00000000650A2786: 90                 nop
  00000000650A2787: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000650A278C: FF 15 C6 4B 5F 00  call        qword ptr [0000000065697358h]
  00000000650A2792: EB 08              jmp         00000000650A279C
  00000000650A2794: 48 8B CB           mov         rcx,rbx
  00000000650A2797: E8 84 E2 FF FF     call        00000000650A0A20
  00000000650A279C: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650A279F: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000650A27A2: 4C 8B 11           mov         r10,qword ptr [rcx]
  00000000650A27A5: 8B 84 24 90 00 00  mov         eax,dword ptr [rsp+0000000000000090h]
                    00
  00000000650A27AC: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000000650A27B0: 45 8B CC           mov         r9d,r12d
  00000000650A27B3: 4D 8B C5           mov         r8,r13
  00000000650A27B6: 48 8B D5           mov         rdx,rbp
  00000000650A27B9: 41 FF 52 28        call        qword ptr [r10+28h]
  00000000650A27BD: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  00000000650A27C2: 48 8B 6C 24 78     mov         rbp,qword ptr [rsp+78h]
  00000000650A27C7: 48 8B B4 24 80 00  mov         rsi,qword ptr [rsp+0000000000000080h]
                    00 00
  00000000650A27CF: 48 83 C4 50        add         rsp,50h
  00000000650A27D3: 41 5D              pop         r13
  00000000650A27D5: 41 5C              pop         r12
  00000000650A27D7: 5F                 pop         rdi
  00000000650A27D8: C3                 ret

  Summary

      695000 .text

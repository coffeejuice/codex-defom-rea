Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067079640: 48 8B C4           mov         rax,rsp
  0000000067079643: 48 89 50 10        mov         qword ptr [rax+10h],rdx
  0000000067079647: 56                 push        rsi
  0000000067079648: 57                 push        rdi
  0000000067079649: 41 54              push        r12
  000000006707964B: 41 55              push        r13
  000000006707964D: 41 56              push        r14
  000000006707964F: 48 83 EC 40        sub         rsp,40h
  0000000067079653: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006707965C: 48 89 58 18        mov         qword ptr [rax+18h],rbx
  0000000067079660: 48 89 68 20        mov         qword ptr [rax+20h],rbp
  0000000067079664: 45 8B E1           mov         r12d,r9d
  0000000067079667: 4C 8B F2           mov         r14,rdx
  000000006707966A: 48 8B E9           mov         rbp,rcx
  000000006707966D: C7 40 08 00 00 00  mov         dword ptr [rax+8],0
                    00
  0000000067079674: 49 8B D0           mov         rdx,r8
  0000000067079677: 48 8D 48 B8        lea         rcx,[rax-48h]
  000000006707967B: E8 50 F7 FE FF     call        0000000067068DD0
  0000000067079680: 90                 nop
  0000000067079681: 48 8D 05 C8 73 27  lea         rax,[00000000672F0A50h]
                    00
  0000000067079688: 49 89 06           mov         qword ptr [r14],rax
  000000006707968B: F0 83 05 BD 73 27  lock add    dword ptr [00000000672F0A50h],1
                    00 01
  0000000067079693: C7 44 24 70 01 00  mov         dword ptr [rsp+70h],1
                    00 00
  000000006707969B: 33 DB              xor         ebx,ebx
  000000006707969D: 45 33 C9           xor         r9d,r9d
  00000000670796A0: 45 33 C0           xor         r8d,r8d
  00000000670796A3: 48 8B D5           mov         rdx,rbp
  00000000670796A6: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670796AB: E8 F0 EF FE FF     call        00000000670686A0
  00000000670796B0: 8B F8              mov         edi,eax
  00000000670796B2: 41 83 CD FF        or          r13d,0FFFFFFFFh
  00000000670796B6: 41 3B C5           cmp         eax,r13d
  00000000670796B9: 74 77              je          0000000067079732
  00000000670796BB: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  00000000670796C0: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000670796C5: E8 06 FD FD FF     call        00000000670593D0
  00000000670796CA: 8B F0              mov         esi,eax
  00000000670796CC: 3B DF              cmp         ebx,edi
  00000000670796CE: 75 05              jne         00000000670796D5
  00000000670796D0: 45 85 E4           test        r12d,r12d
  00000000670796D3: 75 38              jne         000000006707970D
  00000000670796D5: 44 8B CF           mov         r9d,edi
  00000000670796D8: 44 2B CB           sub         r9d,ebx
  00000000670796DB: 44 8B C3           mov         r8d,ebx
  00000000670796DE: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000670796E3: 48 8B CD           mov         rcx,rbp
  00000000670796E6: E8 B5 A8 FF FF     call        0000000067073FA0
  00000000670796EB: 90                 nop
  00000000670796EC: 48 8B D0           mov         rdx,rax
  00000000670796EF: 49 8B CE           mov         rcx,r14
  00000000670796F2: E8 99 BC FA FF     call        0000000067025390
  00000000670796F7: 90                 nop
  00000000670796F8: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670796FD: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067079701: 75 0A              jne         000000006707970D
  0000000067079703: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000067079708: E8 C3 69 FF FF     call        00000000670700D0
  000000006707970D: 8D 1C 3E           lea         ebx,[rsi+rdi]
  0000000067079710: 33 C0              xor         eax,eax
  0000000067079712: 85 F6              test        esi,esi
  0000000067079714: 0F 94 C0           sete        al
  0000000067079717: 44 8D 04 18        lea         r8d,[rax+rbx]
  000000006707971B: 45 33 C9           xor         r9d,r9d
  000000006707971E: 48 8B D5           mov         rdx,rbp
  0000000067079721: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000067079726: E8 75 EF FE FF     call        00000000670686A0
  000000006707972B: 8B F8              mov         edi,eax
  000000006707972D: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000067079730: 75 8E              jne         00000000670796C0
  0000000067079732: 48 8B 45 00        mov         rax,qword ptr [rbp]
  0000000067079736: 3B 58 08           cmp         ebx,dword ptr [rax+8]
  0000000067079739: 75 05              jne         0000000067079740
  000000006707973B: 45 85 E4           test        r12d,r12d
  000000006707973E: 75 36              jne         0000000067079776
  0000000067079740: 45 8B CD           mov         r9d,r13d
  0000000067079743: 44 8B C3           mov         r8d,ebx
  0000000067079746: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006707974B: 48 8B CD           mov         rcx,rbp
  000000006707974E: E8 4D A8 FF FF     call        0000000067073FA0
  0000000067079753: 90                 nop
  0000000067079754: 48 8B D0           mov         rdx,rax
  0000000067079757: 49 8B CE           mov         rcx,r14
  000000006707975A: E8 31 BC FA FF     call        0000000067025390
  000000006707975F: 90                 nop
  0000000067079760: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000067079765: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067079769: 75 0B              jne         0000000067079776
  000000006707976B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000067079770: E8 5B 69 FF FF     call        00000000670700D0
  0000000067079775: 90                 nop
  0000000067079776: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006707977B: E8 C0 E6 FE FF     call        0000000067067E40
  0000000067079780: 49 8B C6           mov         rax,r14
  0000000067079783: 4C 8D 5C 24 40     lea         r11,[rsp+40h]
  0000000067079788: 49 8B 5B 40        mov         rbx,qword ptr [r11+40h]
  000000006707978C: 49 8B 6B 48        mov         rbp,qword ptr [r11+48h]
  0000000067079790: 49 8B E3           mov         rsp,r11
  0000000067079793: 41 5E              pop         r14
  0000000067079795: 41 5D              pop         r13
  0000000067079797: 41 5C              pop         r12
  0000000067079799: 5F                 pop         rdi
  000000006707979A: 5E                 pop         rsi
  000000006707979B: C3                 ret
  000000006707979C: CC                 int         3
  000000006707979D: CC                 int         3
  000000006707979E: CC                 int         3
  000000006707979F: CC                 int         3
  00000000670797A0: 4C 8B DC           mov         r11,rsp
  00000000670797A3: 4D 89 43 18        mov         qword ptr [r11+18h],r8
  00000000670797A7: 49 89 53 10        mov         qword ptr [r11+10h],rdx
  00000000670797AB: 55                 push        rbp
  00000000670797AC: 56                 push        rsi
  00000000670797AD: 57                 push        rdi
  00000000670797AE: 41 54              push        r12
  00000000670797B0: 41 55              push        r13
  00000000670797B2: 41 56              push        r14
  00000000670797B4: 41 57              push        r15
  00000000670797B6: 48 83 EC 70        sub         rsp,70h
  00000000670797BA: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670797C3: 49 89 5B 08        mov         qword ptr [r11+8],rbx
  00000000670797C7: 45 8B E9           mov         r13d,r9d
  00000000670797CA: 4D 8B E0           mov         r12,r8
  00000000670797CD: 48 8B EA           mov         rbp,rdx
  00000000670797D0: 45 33 F6           xor         r14d,r14d
  00000000670797D3: 45 89 73 A4        mov         dword ptr [r11-5Ch],r14d
  00000000670797D7: 48 8B 9C 24 D8 00  mov         rbx,qword ptr [rsp+00000000000000D8h]
                    00 00
  00000000670797DF: 8B 03              mov         eax,dword ptr [rbx]
  00000000670797E1: 83 E0 08           and         eax,8
  00000000670797E4: 41 8B C6           mov         eax,r14d
  00000000670797E7: 0F 94 C0           sete        al
  00000000670797EA: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000000670797EE: 45 33 C9           xor         r9d,r9d
  00000000670797F1: 49 8D 53 98        lea         rdx,[r11-68h]
  00000000670797F5: E8 66 FB FF FF     call        0000000067079360
  00000000670797FA: 90                 nop
  00000000670797FB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000067079800: 44 8B 40 0C        mov         r8d,dword ptr [rax+0Ch]
  0000000067079804: 48 63 50 08        movsxd      rdx,dword ptr [rax+8]
  0000000067079808: 44 3B C2           cmp         r8d,edx
  000000006707980B: 75 6D              jne         000000006707987A
  000000006707980D: 4C 8D 3D 1C 73 27  lea         r15,[00000000672F0B30h]
                    00
  0000000067079814: 4C 89 7D 00        mov         qword ptr [rbp],r15
  0000000067079818: F0 83 05 10 73 27  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  0000000067079820: C7 44 24 4C 01 00  mov         dword ptr [rsp+4Ch],1
                    00 00
  0000000067079828: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006707982D: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  0000000067079831: 75 3F              jne         0000000067079872
  0000000067079833: 48 8B 74 24 40     mov         rsi,qword ptr [rsp+40h]
  0000000067079838: 48 63 46 0C        movsxd      rax,dword ptr [rsi+0Ch]
  000000006707983C: 48 8D 5C C6 18     lea         rbx,[rsi+rax*8+18h]
  0000000067079841: 48 63 46 08        movsxd      rax,dword ptr [rsi+8]
  0000000067079845: 48 8D 7C C6 18     lea         rdi,[rsi+rax*8+18h]
  000000006707984A: 48 3B FB           cmp         rdi,rbx
  000000006707984D: 74 1B              je          000000006707986A
  000000006707984F: 90                 nop
  0000000067079850: 48 83 EB 08        sub         rbx,8
  0000000067079854: 48 8B 13           mov         rdx,qword ptr [rbx]
  0000000067079857: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  000000006707985B: 75 08              jne         0000000067079865
  000000006707985D: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067079860: E8 6B 68 FF FF     call        00000000670700D0
  0000000067079865: 48 3B DF           cmp         rbx,rdi
  0000000067079868: 75 E6              jne         0000000067079850
  000000006707986A: 48 8B CE           mov         rcx,rsi
  000000006707986D: E8 3E 6B FA FF     call        00000000670203B0
  0000000067079872: 48 8B C5           mov         rax,rbp
  0000000067079875: E9 0B 03 00 00     jmp         0000000067079B85
  000000006707987A: 8B 0B              mov         ecx,dword ptr [rbx]
  000000006707987C: 83 E1 01           and         ecx,1
  000000006707987F: 75 20              jne         00000000670798A1
  0000000067079881: 45 85 ED           test        r13d,r13d
  0000000067079884: 79 08              jns         000000006707988E
  0000000067079886: 41 8B C0           mov         eax,r8d
  0000000067079889: 2B C2              sub         eax,edx
  000000006707988B: 44 03 E8           add         r13d,eax
  000000006707988E: 8B B4 24 D0 00 00  mov         esi,dword ptr [rsp+00000000000000D0h]
                    00
  0000000067079895: 85 F6              test        esi,esi
  0000000067079897: 79 50              jns         00000000670798E9
  0000000067079899: 44 2B C2           sub         r8d,edx
  000000006707989C: 41 03 F0           add         esi,r8d
  000000006707989F: EB 48              jmp         00000000670798E9
  00000000670798A1: 45 8B CE           mov         r9d,r14d
  00000000670798A4: 44 2B C2           sub         r8d,edx
  00000000670798A7: 45 85 C0           test        r8d,r8d
  00000000670798AA: 7E 1E              jle         00000000670798CA
  00000000670798AC: 48 8D 54 D0 18     lea         rdx,[rax+rdx*8+18h]
  00000000670798B1: 41 8B C8           mov         ecx,r8d
  00000000670798B4: 48 8B 02           mov         rax,qword ptr [rdx]
  00000000670798B7: 83 78 08 00        cmp         dword ptr [rax+8],0
  00000000670798BB: 75 03              jne         00000000670798C0
  00000000670798BD: 41 FF C1           inc         r9d
  00000000670798C0: 48 83 C2 08        add         rdx,8
  00000000670798C4: 48 83 E9 01        sub         rcx,1
  00000000670798C8: 75 EA              jne         00000000670798B4
  00000000670798CA: 45 85 ED           test        r13d,r13d
  00000000670798CD: 79 09              jns         00000000670798D8
  00000000670798CF: 41 8B C0           mov         eax,r8d
  00000000670798D2: 41 2B C1           sub         eax,r9d
  00000000670798D5: 44 03 E8           add         r13d,eax
  00000000670798D8: 8B B4 24 D0 00 00  mov         esi,dword ptr [rsp+00000000000000D0h]
                    00
  00000000670798DF: 85

  Summary

      1A7000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140AF73F0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140AF73F5: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140AF73FA: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140AF73FF: 48 81 EC 78 04 00  sub         rsp,478h
                    00
  0000000140AF7406: 48 8B 05 23 6C 06  mov         rax,qword ptr [0000000143B5E030h]
                    03
  0000000140AF740D: 48 33 C4           xor         rax,rsp
  0000000140AF7410: 48 89 84 24 60 04  mov         qword ptr [rsp+0000000000000460h],rax
                    00 00
  0000000140AF7418: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140AF7420: C7 84 24 30 04 00  mov         dword ptr [rsp+0000000000000430h],0
                    00 00 00 00 00
  0000000140AF742B: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF7433: 83 78 04 00        cmp         dword ptr [rax+4],0
  0000000140AF7437: 75 15              jne         0000000140AF744E
  0000000140AF7439: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF7441: 83 78 40 01        cmp         dword ptr [rax+40h],1
  0000000140AF7445: 7F 07              jg          0000000140AF744E
  0000000140AF7447: 33 C0              xor         eax,eax
  0000000140AF7449: E9 94 03 00 00     jmp         0000000140AF77E2
  0000000140AF744E: 48 8B 8C 24 88 04  mov         rcx,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF7456: E8 E5 43 DA FF     call        000000014089B840
  0000000140AF745B: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF7463: 8B 40 04           mov         eax,dword ptr [rax+4]
  0000000140AF7466: 89 84 24 58 04 00  mov         dword ptr [rsp+0000000000000458h],eax
                    00
  0000000140AF746D: 83 BC 24 58 04 00  cmp         dword ptr [rsp+0000000000000458h],0
                    00 00
  0000000140AF7475: 74 2F              je          0000000140AF74A6
  0000000140AF7477: 83 BC 24 58 04 00  cmp         dword ptr [rsp+0000000000000458h],1
                    00 01
  0000000140AF747F: 0F 84 AC 00 00 00  je          0000000140AF7531
  0000000140AF7485: 83 BC 24 58 04 00  cmp         dword ptr [rsp+0000000000000458h],1
                    00 01
  0000000140AF748D: 0F 8E 25 03 00 00  jle         0000000140AF77B8
  0000000140AF7493: 83 BC 24 58 04 00  cmp         dword ptr [rsp+0000000000000458h],3
                    00 03
  0000000140AF749B: 0F 8E 87 01 00 00  jle         0000000140AF7628
  0000000140AF74A1: E9 12 03 00 00     jmp         0000000140AF77B8
  0000000140AF74A6: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF74AE: 83 78 40 01        cmp         dword ptr [rax+40h],1
  0000000140AF74B2: 7E 71              jle         0000000140AF7525
  0000000140AF74B4: C7 84 24 30 04 00  mov         dword ptr [rsp+0000000000000430h],0
                    00 00 00 00 00
  0000000140AF74BF: EB 11              jmp         0000000140AF74D2
  0000000140AF74C1: 8B 84 24 30 04 00  mov         eax,dword ptr [rsp+0000000000000430h]
                    00
  0000000140AF74C8: 83 C0 01           add         eax,1
  0000000140AF74CB: 89 84 24 30 04 00  mov         dword ptr [rsp+0000000000000430h],eax
                    00
  0000000140AF74D2: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF74DA: 8B 40 40           mov         eax,dword ptr [rax+40h]
  0000000140AF74DD: 39 84 24 30 04 00  cmp         dword ptr [rsp+0000000000000430h],eax
                    00
  0000000140AF74E4: 7D 3D              jge         0000000140AF7523
  0000000140AF74E6: 48 63 8C 24 30 04  movsxd      rcx,dword ptr [rsp+0000000000000430h]
                    00 00
  0000000140AF74EE: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF74F6: 48 8B 40 48        mov         rax,qword ptr [rax+48h]
  0000000140AF74FA: 4C 8D 04 C8        lea         r8,[rax+rcx*8]
  0000000140AF74FE: 48 8D 15 D7 14 CF  lea         rdx,[00000001427E89DCh]
                    01
  0000000140AF7505: 48 8B 8C 24 80 04  mov         rcx,qword ptr [rsp+0000000000000480h]
                    00 00
  0000000140AF750D: E8 B6 B5 97 00     call        0000000141472AC8
  0000000140AF7512: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000140AF7515: 75 0A              jne         0000000140AF7521
  0000000140AF7517: B8 01 00 00 00     mov         eax,1
  0000000140AF751C: E9 C1 02 00 00     jmp         0000000140AF77E2
  0000000140AF7521: EB 9E              jmp         0000000140AF74C1
  0000000140AF7523: EB 07              jmp         0000000140AF752C
  0000000140AF7525: 33 C0              xor         eax,eax
  0000000140AF7527: E9 B6 02 00 00     jmp         0000000140AF77E2
  0000000140AF752C: E9 87 02 00 00     jmp         0000000140AF77B8
  0000000140AF7531: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140AF7539: EB 0B              jmp         0000000140AF7546
  0000000140AF753B: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140AF753F: 83 C0 01           add         eax,1
  0000000140AF7542: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140AF7546: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF754E: 8B 40 1C           mov         eax,dword ptr [rax+1Ch]
  0000000140AF7551: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  0000000140AF7555: 0F 8D C8 00 00 00  jge         0000000140AF7623
  0000000140AF755B: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  0000000140AF7560: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF7568: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140AF756C: 4C 8D 04 C8        lea         r8,[rax+rcx*8]
  0000000140AF7570: 48 8D 15 6D 14 CF  lea         rdx,[00000001427E89E4h]
                    01
  0000000140AF7577: 48 8B 8C 24 80 04  mov         rcx,qword ptr [rsp+0000000000000480h]
                    00 00
  0000000140AF757F: E8 44 B5 97 00     call        0000000141472AC8
  0000000140AF7584: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000140AF7587: 75 0A              jne         0000000140AF7593
  0000000140AF7589: B8 01 00 00 00     mov         eax,1
  0000000140AF758E: E9 4F 02 00 00     jmp         0000000140AF77E2
  0000000140AF7593: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF759B: 8B 40 40           mov         eax,dword ptr [rax+40h]
  0000000140AF759E: 89 84 24 34 04 00  mov         dword ptr [rsp+0000000000000434h],eax
                    00
  0000000140AF75A5: C7 84 24 30 04 00  mov         dword ptr [rsp+0000000000000430h],0
                    00 00 00 00 00
  0000000140AF75B0: EB 11              jmp         0000000140AF75C3
  0000000140AF75B2: 8B 84 24 30 04 00  mov         eax,dword ptr [rsp+0000000000000430h]
                    00
  0000000140AF75B9: 83 C0 01           add         eax,1
  0000000140AF75BC: 89 84 24 30 04 00  mov         dword ptr [rsp+0000000000000430h],eax
                    00
  0000000140AF75C3: 8B 84 24 34 04 00  mov         eax,dword ptr [rsp+0000000000000434h]
                    00
  0000000140AF75CA: 39 84 24 30 04 00  cmp         dword ptr [rsp+0000000000000430h],eax
                    00
  0000000140AF75D1: 7D 4B              jge         0000000140AF761E
  0000000140AF75D3: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140AF75D7: 0F AF 84 24 34 04  imul        eax,dword ptr [rsp+0000000000000434h]
                    00 00
  0000000140AF75DF: 03 84 24 30 04 00  add         eax,dword ptr [rsp+0000000000000430h]
                    00
  0000000140AF75E6: 48 63 C8           movsxd      rcx,eax
  0000000140AF75E9: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF75F1: 48 8B 40 48        mov         rax,qword ptr [rax+48h]
  0000000140AF75F5: 4C 8D 04 C8        lea         r8,[rax+rcx*8]
  0000000140AF75F9: 48 8D 15 EC 13 CF  lea         rdx,[00000001427E89ECh]
                    01
  0000000140AF7600: 48 8B 8C 24 80 04  mov         rcx,qword ptr [rsp+0000000000000480h]
                    00 00
  0000000140AF7608: E8 BB B4 97 00     call        0000000141472AC8
  0000000140AF760D: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000140AF7610: 75 0A              jne         0000000140AF761C
  0000000140AF7612: B8 01 00 00 00     mov         eax,1
  0000000140AF7617: E9 C6 01 00 00     jmp         0000000140AF77E2
  0000000140AF761C: EB 94              jmp         0000000140AF75B2
  0000000140AF761E: E9 18 FF FF FF     jmp         0000000140AF753B
  0000000140AF7623: E9 90 01 00 00     jmp         0000000140AF77B8
  0000000140AF7628: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140AF7630: EB 0B              jmp         0000000140AF763D
  0000000140AF7632: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140AF7636: 83 C0 01           add         eax,1
  0000000140AF7639: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140AF763D: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF7645: 8B 40 04           mov         eax,dword ptr [rax+4]
  0000000140AF7648: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  0000000140AF764C: 0F 8D 80 00 00 00  jge         0000000140AF76D2
  0000000140AF7652: C7 84 24 30 04 00  mov         dword ptr [rsp+0000000000000430h],0
                    00 00 00 00 00
  0000000140AF765D: EB 11              jmp         0000000140AF7670
  0000000140AF765F: 8B 84 24 30 04 00  mov         eax,dword ptr [rsp+0000000000000430h]
                    00
  0000000140AF7666: 83 C0 01           add         eax,1
  0000000140AF7669: 89 84 24 30 04 00  mov         dword ptr [rsp+0000000000000430h],eax
                    00
  0000000140AF7670: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  0000000140AF7675: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF767D: 8B 44 88 1C        mov         eax,dword ptr [rax+rcx*4+1Ch]
  0000000140AF7681: 39 84 24 30 04 00  cmp         dword ptr [rsp+0000000000000430h],eax
                    00
  0000000140AF7688: 7D 43              jge         0000000140AF76CD
  0000000140AF768A: 48 63 54 24 20     movsxd      rdx,dword ptr [rsp+20h]
  0000000140AF768F: 48 63 8C 24 30 04  movsxd      rcx,dword ptr [rsp+0000000000000430h]
                    00 00
  0000000140AF7697: 48 8B 84 24 88 04  mov         rax,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF769F: 48 8B 44 D0 28     mov         rax,qword ptr [rax+rdx*8+28h]
  0000000140AF76A4: 4C 8D 04 C8        lea         r8,[rax+rcx*8]
  0000000140AF76A8: 48 8D 15 45 13 CF  lea         rdx,[00000001427E89F4h]
                    01
  0000000140AF76AF: 48 8B 8C 24 80 04  mov         rcx,qword ptr [rsp+0000000000000480h]
                    00 00
  0000000140AF76B7: E8 0C B4 97 00     call        0000000141472AC8
  0000000140AF76BC: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000140AF76BF: 75 0A              jne         0000000140AF76CB
  0000000140AF76C1: B8 01 00 00 00     mov         eax,1
  0000000140AF76C6: E9 17 01 00 00     jmp         0000000140AF77E2
  0000000140AF76CB: EB 92              jmp         0000000140AF765F
  0000000140AF76CD: E9 60 FF FF FF     jmp         0000000140AF7632
  0000000140AF76D2: 48 8B 8C 24 88 04  mov         rcx,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF76DA: E8 B1 45 DA FF     call        000000014089BC90
  0000000140AF76DF: 89 84 24 40 04 00  mov         dword ptr [rsp+0000000000000440h],eax
                    00
  0000000140AF76E6: 48 63 8C 24 40 04  movsxd      rcx,dword ptr [rsp+0000000000000440h]
                    00 00
  0000000140AF76EE: B8 08 00 00 00     mov         eax,8
  0000000140AF76F3: 48 F7 E1           mul         rax,rcx
  0000000140AF76F6: 48 8B C8           mov         rcx,rax
  0000000140AF76F9: 48 C7 C0 FF FF FF  mov         rax,0FFFFFFFFFFFFFFFFh
                    FF
  0000000140AF7700: 48 0F 40 C8        cmovo       rcx,rax
  0000000140AF7704: E8 53 4B 97 00     call        000000014146C25C
  0000000140AF7709: 48 89 84 24 48 04  mov         qword ptr [rsp+0000000000000448h],rax
                    00 00
  0000000140AF7711: 48 8B 84 24 48 04  mov         rax,qword ptr [rsp+0000000000000448h]
                    00 00
  0000000140AF7719: 48 89 84 24 38 04  mov         qword ptr [rsp+0000000000000438h],rax
                    00 00
  0000000140AF7721: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140AF7729: EB 0B              jmp         0000000140AF7736
  0000000140AF772B: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140AF772F: 83 C0 01           add         eax,1
  0000000140AF7732: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140AF7736: 8B 84 24 40 04 00  mov         eax,dword ptr [rsp+0000000000000440h]
                    00
  0000000140AF773D: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  0000000140AF7741: 7D 33              jge         0000000140AF7776
  0000000140AF7743: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  0000000140AF7748: 48 8B 84 24 38 04  mov         rax,qword ptr [rsp+0000000000000438h]
                    00 00
  0000000140AF7750: 4C 8D 04 C8        lea         r8,[rax+rcx*8]
  0000000140AF7754: 48 8D 15 A1 12 CF  lea         rdx,[00000001427E89FCh]
                    01
  0000000140AF775B: 48 8B 8C 24 80 04  mov         rcx,qword ptr [rsp+0000000000000480h]
                    00 00
  0000000140AF7763: E8 60 B3 97 00     call        0000000141472AC8
  0000000140AF7768: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000140AF776B: 75 07              jne         0000000140AF7774
  0000000140AF776D: B8 01 00 00 00     mov         eax,1
  0000000140AF7772: EB 6E              jmp         0000000140AF77E2
  0000000140AF7774: EB B5              jmp         0000000140AF772B
  0000000140AF7776: 4C 63 84 24 40 04  movsxd      r8,dword ptr [rsp+0000000000000440h]
                    00 00
  0000000140AF777E: 49 C1 E0 03        shl         r8,3
  0000000140AF7782: 48 8B 94 24 38 04  mov         rdx,qword ptr [rsp+0000000000000438h]
                    00 00
  0000000140AF778A: 48 8B 8C 24 88 04  mov         rcx,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF7792: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  0000000140AF7796: E8 65 79 97 00     call        000000014146F100
  0000000140AF779B: 4C 8B 9C 24 38 04  mov         r11,qword ptr [rsp+0000000000000438h]
                    00 00
  0000000140AF77A3: 4C 89 9C 24 50 04  mov         qword ptr [rsp+0000000000000450h],r11
                    00 00
  0000000140AF77AB: 48 8B 8C 24 50 04  mov         rcx,qword ptr [rsp+0000000000000450h]
                    00 00
  0000000140AF77B3: E8 E0 60 97 00     call        000000014146D898
  0000000140AF77B8: 48 8B 8C 24 88 04  mov         rcx,qword ptr [rsp+0000000000000488h]
                    00 00
  0000000140AF77C0: E8 CB 44 DA FF     call        000000014089BC90
  0000000140AF77C5: 85 C0              test        eax,eax
  0000000140AF77C7: 7E 17              jle         0000000140AF77E0
  0000000140AF77C9: 4C 8B 84 24 80 04  mov         r8,qword ptr [rsp+0000000000000480h]
                    00 00
  0000000140AF77D1: BA 00 04 00 00     mov         edx,400h
  0000000140AF77D6: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140AF77DB: E8 CC BB 97 00     call        00000001414733AC
  0000000140AF77E0: 33 C0              xor         eax,eax
  0000000140AF77E2: 48 8B 8C 24 60 04  mov         rcx,qword ptr [rsp+0000000000000460h]
                    00 00
  0000000140AF77EA: 48 33 CC           xor         rcx,rsp
  0000000140AF77ED: E8 1E 67 97 00     call        000000014146DF10
  0000000140AF77F2: 48 81 C4 78 04 00  add         rsp,478h
                    00
  0000000140AF77F9: C3                 ret

  Summary

     1E02000 .text

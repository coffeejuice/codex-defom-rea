Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065390320: 40 57              push        rdi
  0000000065390322: 48 83 EC 40        sub         rsp,40h
  0000000065390326: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006539032F: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  0000000065390334: 48 89 6C 24 58     mov         qword ptr [rsp+58h],rbp
  0000000065390339: 48 89 74 24 60     mov         qword ptr [rsp+60h],rsi
  000000006539033E: 49 8B E9           mov         rbp,r9
  0000000065390341: 8B F2              mov         esi,edx
  0000000065390343: 48 8B F9           mov         rdi,rcx
  0000000065390346: E8 45 1D CE FF     call        0000000065072090
  000000006539034B: 48 63 D8           movsxd      rbx,eax
  000000006539034E: 85 C0              test        eax,eax
  0000000065390350: 79 07              jns         0000000065390359
  0000000065390352: 8B C3              mov         eax,ebx
  0000000065390354: E9 AC 02 00 00     jmp         0000000065390605
  0000000065390359: 85 F6              test        esi,esi
  000000006539035B: 75 1D              jne         000000006539037A
  000000006539035D: 83 FB 18           cmp         ebx,18h
  0000000065390360: 7D 10              jge         0000000065390372
  0000000065390362: 4C 8B CD           mov         r9,rbp
  0000000065390365: 44 8B C3           mov         r8d,ebx
  0000000065390368: 33 D2              xor         edx,edx
  000000006539036A: 48 8B CF           mov         rcx,rdi
  000000006539036D: E8 CE FC FF FF     call        0000000065390040
  0000000065390372: 83 EB 18           sub         ebx,18h
  0000000065390375: E9 89 02 00 00     jmp         0000000065390603
  000000006539037A: 83 FE 01           cmp         esi,1
  000000006539037D: 0F 85 49 01 00 00  jne         00000000653904CC
  0000000065390383: 48 8B 75 00        mov         rsi,qword ptr [rbp]
  0000000065390387: 83 FB 0E           cmp         ebx,0Eh
  000000006539038A: 0F 87 34 01 00 00  ja          00000000653904C4
  0000000065390390: 48 8D 0D 69 FC C6  lea         rcx,[0000000065000000h]
                    FF
  0000000065390397: 8B 84 99 1C 06 39  mov         eax,dword ptr [rcx+rbx*4+000000000039061Ch]
                    00
  000000006539039E: 48 03 C1           add         rax,rcx
  00000000653903A1: FF E0              jmp         rax
  00000000653903A3: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  00000000653903A7: 48 83 B8 E8 01 00  cmp         qword ptr [rax+00000000000001E8h],0
                    00 00
  00000000653903AF: 0F 95 C0           setne       al
  00000000653903B2: 88 06              mov         byte ptr [rsi],al
  00000000653903B4: E9 0B 01 00 00     jmp         00000000653904C4
  00000000653903B9: 48 8B CF           mov         rcx,rdi
  00000000653903BC: E8 1F 6F FF FF     call        00000000653872E0
  00000000653903C1: 89 06              mov         dword ptr [rsi],eax
  00000000653903C3: E9 FC 00 00 00     jmp         00000000653904C4
  00000000653903C8: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000653903CD: 48 8B CF           mov         rcx,rdi
  00000000653903D0: E8 EB 9A FF FF     call        0000000065389EC0
  00000000653903D5: 90                 nop
  00000000653903D6: 48 8B D0           mov         rdx,rax
  00000000653903D9: 48 8B CE           mov         rcx,rsi
  00000000653903DC: FF 15 9E 63 30 00  call        qword ptr [0000000065696780h]
  00000000653903E2: 90                 nop
  00000000653903E3: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000000653903E8: FF 15 72 63 30 00  call        qword ptr [0000000065696760h]
  00000000653903EE: E9 D1 00 00 00     jmp         00000000653904C4
  00000000653903F3: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  00000000653903F7: 48 81 C1 48 02 00  add         rcx,248h
                    00
  00000000653903FE: FF 15 3C 84 30 00  call        qword ptr [0000000065698840h]
  0000000065390404: 89 06              mov         dword ptr [rsi],eax
  0000000065390406: E9 B9 00 00 00     jmp         00000000653904C4
  000000006539040B: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006539040F: 8B 88 10 02 00 00  mov         ecx,dword ptr [rax+0000000000000210h]
  0000000065390415: 89 0E              mov         dword ptr [rsi],ecx
  0000000065390417: E9 A8 00 00 00     jmp         00000000653904C4
  000000006539041C: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065390420: 8B 88 14 02 00 00  mov         ecx,dword ptr [rax+0000000000000214h]
  0000000065390426: 89 0E              mov         dword ptr [rsi],ecx
  0000000065390428: E9 97 00 00 00     jmp         00000000653904C4
  000000006539042D: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065390431: 8B 88 F8 01 00 00  mov         ecx,dword ptr [rax+00000000000001F8h]
  0000000065390437: 89 0E              mov         dword ptr [rsi],ecx
  0000000065390439: E9 86 00 00 00     jmp         00000000653904C4
  000000006539043E: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065390442: 8B 88 FC 01 00 00  mov         ecx,dword ptr [rax+00000000000001FCh]
  0000000065390448: 89 0E              mov         dword ptr [rsi],ecx
  000000006539044A: EB 78              jmp         00000000653904C4
  000000006539044C: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065390450: 8B 88 00 02 00 00  mov         ecx,dword ptr [rax+0000000000000200h]
  0000000065390456: 89 0E              mov         dword ptr [rsi],ecx
  0000000065390458: EB 6A              jmp         00000000653904C4
  000000006539045A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006539045F: 48 8B CF           mov         rcx,rdi
  0000000065390462: E8 99 71 FF FF     call        0000000065387600
  0000000065390467: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006539046A: 48 89 0E           mov         qword ptr [rsi],rcx
  000000006539046D: EB 55              jmp         00000000653904C4
  000000006539046F: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065390473: 8B 88 0C 02 00 00  mov         ecx,dword ptr [rax+000000000000020Ch]
  0000000065390479: D1 E9              shr         ecx,1
  000000006539047B: 80 E1 01           and         cl,1
  000000006539047E: 88 0E              mov         byte ptr [rsi],cl
  0000000065390480: EB 42              jmp         00000000653904C4
  0000000065390482: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065390486: 8B 88 58 02 00 00  mov         ecx,dword ptr [rax+0000000000000258h]
  000000006539048C: 89 0E              mov         dword ptr [rsi],ecx
  000000006539048E: EB 34              jmp         00000000653904C4
  0000000065390490: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065390494: 8B 88 0C 02 00 00  mov         ecx,dword ptr [rax+000000000000020Ch]
  000000006539049A: C1 E9 02           shr         ecx,2
  000000006539049D: 80 E1 01           and         cl,1
  00000000653904A0: 88 0E              mov         byte ptr [rsi],cl
  00000000653904A2: EB 20              jmp         00000000653904C4
  00000000653904A4: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  00000000653904A8: 8B 88 0C 02 00 00  mov         ecx,dword ptr [rax+000000000000020Ch]
  00000000653904AE: C1 E9 03           shr         ecx,3
  00000000653904B1: 80 E1 01           and         cl,1
  00000000653904B4: 88 0E              mov         byte ptr [rsi],cl
  00000000653904B6: EB 0C              jmp         00000000653904C4
  00000000653904B8: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  00000000653904BC: 8B 88 18 02 00 00  mov         ecx,dword ptr [rax+0000000000000218h]
  00000000653904C2: 89 0E              mov         dword ptr [rsi],ecx
  00000000653904C4: 83 EB 0F           sub         ebx,0Fh
  00000000653904C7: E9 37 01 00 00     jmp         0000000065390603
  00000000653904CC: 83 FE 02           cmp         esi,2
  00000000653904CF: 0F 85 F4 00 00 00  jne         00000000653905C9
  00000000653904D5: 48 8B 55 00        mov         rdx,qword ptr [rbp]
  00000000653904D9: 83 FB 0E           cmp         ebx,0Eh
  00000000653904DC: 0F 87 E2 00 00 00  ja          00000000653905C4
  00000000653904E2: 48 8D 0D 17 FB C6  lea         rcx,[0000000065000000h]
                    FF
  00000000653904E9: 8B 84 99 58 06 39  mov         eax,dword ptr [rcx+rbx*4+0000000000390658h]
                    00
  00000000653904F0: 48 03 C1           add         rax,rcx
  00000000653904F3: FF E0              jmp         rax
  00000000653904F5: 0F B6 12           movzx       edx,byte ptr [rdx]
  00000000653904F8: 48 8B CF           mov         rcx,rdi
  00000000653904FB: E8 30 C5 FF FF     call        000000006538CA30
  0000000065390500: E9 BF 00 00 00     jmp         00000000653905C4
  0000000065390505: 8B 12              mov         edx,dword ptr [rdx]
  0000000065390507: 48 8B CF           mov         rcx,rdi
  000000006539050A: E8 41 BC FF FF     call        000000006538C150
  000000006539050F: E9 B0 00 00 00     jmp         00000000653905C4
  0000000065390514: 8B 02              mov         eax,dword ptr [rdx]
  0000000065390516: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006539051A: 85 C0              test        eax,eax
  000000006539051C: 79 14              jns         0000000065390532
  000000006539051E: 8B D0              mov         edx,eax
  0000000065390520: 48 8D 0D 19 86 36  lea         rcx,[00000000656F8B40h]
                    00
  0000000065390527: FF 15 6B 64 30 00  call        qword ptr [0000000065696998h]
  000000006539052D: E9 92 00 00 00     jmp         00000000653905C4
  0000000065390532: 89 81 10 02 00 00  mov         dword ptr [rcx+0000000000000210h],eax
  0000000065390538: E9 87 00 00 00     jmp         00000000653905C4
  000000006539053D: 8B 12              mov         edx,dword ptr [rdx]
  000000006539053F: 48 8B CF           mov         rcx,rdi
  0000000065390542: E8 D9 6D FF FF     call        0000000065387320
  0000000065390547: EB 7B              jmp         00000000653905C4
  0000000065390549: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006539054D: 8B 02              mov         eax,dword ptr [rdx]
  000000006539054F: 89 81 F8 01 00 00  mov         dword ptr [rcx+00000000000001F8h],eax
  0000000065390555: EB 6D              jmp         00000000653905C4
  0000000065390557: 8B 12              mov         edx,dword ptr [rdx]
  0000000065390559: 48 8B CF           mov         rcx,rdi
  000000006539055C: E8 FF E1 FF FF     call        000000006538E760
  0000000065390561: EB 61              jmp         00000000653905C4
  0000000065390563: 8B 12              mov         edx,dword ptr [rdx]
  0000000065390565: 48 8B CF           mov         rcx,rdi
  0000000065390568: E8 63 E2 FF FF     call        000000006538E7D0
  000000006539056D: EB 55              jmp         00000000653905C4
  000000006539056F: 48 8B CF           mov         rcx,rdi
  0000000065390572: E8 89 8A FF FF     call        0000000065389000
  0000000065390577: EB 4B              jmp         00000000653905C4
  0000000065390579: 0F B6 12           movzx       edx,byte ptr [rdx]
  000000006539057C: 48 8B CF           mov         rcx,rdi
  000000006539057F: E8 9C 6E FF FF     call        0000000065387420
  0000000065390584: EB 3E              jmp         00000000653905C4
  0000000065390586: 8B 12              mov         edx,dword ptr [rdx]
  0000000065390588: 48 8B CF           mov         rcx,rdi
  000000006539058B: E8 A0 6F FF FF     call        0000000065387530
  0000000065390590: EB 32              jmp         00000000653905C4
  0000000065390592: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  0000000065390596: 0F B6 02           movzx       eax,byte ptr [rdx]
  0000000065390599: C1 E0 02           shl         eax,2
  000000006539059C: 33 81 0C 02 00 00  xor         eax,dword ptr [rcx+000000000000020Ch]
  00000000653905A2: 83 E0 04           and         eax,4
  00000000653905A5: 31 81 0C 02 00 00  xor         dword ptr [rcx+000000000000020Ch],eax
  00000000653905AB: EB 17              jmp         00000000653905C4
  00000000653905AD: 0F B6 12           movzx       edx,byte ptr [rdx]
  00000000653905B0: 48 8B CF           mov         rcx,rdi
  00000000653905B3: E8 68 7A FF FF     call        0000000065388020
  00000000653905B8: EB 0A              jmp         00000000653905C4
  00000000653905BA: 8B 12              mov         edx,dword ptr [rdx]
  00000000653905BC: 48 8B CF           mov         rcx,rdi
  00000000653905BF: E8 BC DA FF FF     call        000000006538E080
  00000000653905C4: 83 EB 0F           sub         ebx,0Fh
  00000000653905C7: EB 3A              jmp         0000000065390603
  00000000653905C9: 83 FE 03           cmp         esi,3
  00000000653905CC: 75 05              jne         00000000653905D3
  00000000653905CE: 83 EB 0F           sub         ebx,0Fh
  00000000653905D1: EB 30              jmp         0000000065390603
  00000000653905D3: 83 FE 04           cmp         esi,4
  00000000653905D6: 75 05              jne         00000000653905DD
  00000000653905D8: 83 EB 0F           sub         ebx,0Fh
  00000000653905DB: EB 26              jmp         0000000065390603
  00000000653905DD: 83 FE 05           cmp         esi,5
  00000000653905E0: 75 05              jne         00000000653905E7
  00000000653905E2: 83 EB 0F           sub         ebx,0Fh
  00000000653905E5: EB 1C              jmp         0000000065390603
  00000000653905E7: 83 FE 06           cmp         esi,6
  00000000653905EA: 75 05              jne         00000000653905F1
  00000000653905EC: 83 EB 0F           sub         ebx,0Fh
  00000000653905EF: EB 12              jmp         0000000065390603
  00000000653905F1: 83 FE 07           cmp         esi,7
  00000000653905F4: 75 05              jne         00000000653905FB
  00000000653905F6: 83 EB 0F           sub         ebx,0Fh
  00000000653905F9: EB 08              jmp         0000000065390603
  00000000653905FB: 83 FE 08           cmp         esi,8
  00000000653905FE: 75 03              jne         0000000065390603
  0000000065390600: 83 EB 0F           sub         ebx,0Fh
  0000000065390603: 8B C3              mov         eax,ebx
  0000000065390605: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  000000006539060A: 48 8B 6C 24 58     mov         rbp,qword ptr [rsp+58h]
  000000006539060F: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  0000000065390614: 48 83 C4 40        add         rsp,40h
  0000000065390618: 5F                 pop         rdi
  0000000065390619: C3                 ret
  000000006539061A: 66 90              nop
  000000006539061C: A3 03 39 00 B9 03  mov         dword ptr [C8003903B9003903h],eax
                    39 00 C8
  0000000065390625: 03 39              add         edi,dword ptr [rcx]
  0000000065390627: 00 F3              add         bl,dh
  0000000065390629: 03 39              add         edi,dword ptr [rcx]
  000000006539062B: 00 0B              add         byte ptr [rbx],cl
  000000006539062D: 04 39              add         al,39h
  000000006539062F: 00 1C 04           add         byte ptr [rsp+rax],bl
  0000000065390632: 39 00              cmp         dword ptr [rax],eax
  0000000065390634: 2D 04 39 00 3E     sub         eax,3E003904h
  0000000065390639: 04 39              add         al,39h
  000000006539063B: 00 4C 04 39        add         byte ptr [rsp+rax+39h],cl
  000000006539063F: 00 5A 04           add         byte ptr [rdx+4],bl
  0000000065390642: 39 00              cmp         dword ptr [rax],eax
  0000000065390644: 6F                 outs        dx,dword ptr [rsi]
  0000000065390645: 04 39              add         al,39h
  0000000065390647: 00 82 04 39 00 90  add         byte ptr [rdx+FFFFFFFF90003904h],al
  000000006539064D: 04 39              add         al,39h
  000000006539064F: 00 A4 04 39 00 B8  add         byte ptr [rsp+rax+0000000004B80039h],ah
                    04
  0000000065390656: 39 00              cmp         dword ptr [rax],eax
  0000000065390658: F5                 cmc
  0000000065390659: 04 39              add         al,39h
  000000006539065B: 00 C4              add         ah,al
  000000006539065D: 05 39 00 C4 05     add         eax,5C40039h
  0000000065390662: 39 00              cmp         dword ptr [rax],eax
  0000000065390664: 05 05 39 00 14     add         eax,14003905h
  0000000065390669: 05 39 00 3D 05     add         eax,53D0039h
  000000006539066E: 39 00              cmp         dword ptr [rax],eax
  0000000065390670: 49 05 39 00 57 05  add         rax,5570039h
  0000000065390676: 39 00              cmp         dword ptr [rax],eax
  0000000065390678: 63 05 39 00 6F 05  movsxd      eax,dword ptr [000000006AA806B7h]
  000000006539067E: 39 00              cmp         dword ptr [rax],eax
  0000000065390680: 79 05              jns         0000000065390687
  0000000065390682: 39 00              cmp         dword ptr [rax],eax
  0000000065390684: 86 05 39 00 92 05  xchg        al,byte ptr [000000006ACB06C3h]
  000000006539068A: 39 00              cmp         dword ptr [rax],eax
  000000006539068C: AD                 lods        dword ptr [rsi]
  000000006539068D: 05 39 00 BA 05     add         eax,5BA0039h
  0000000065390692: 39 00              cmp         dword ptr [rax],eax

  Summary

      695000 .text

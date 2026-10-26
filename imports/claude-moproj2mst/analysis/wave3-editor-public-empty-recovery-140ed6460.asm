Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140ED6460: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140ED6465: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140ED6469: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140ED646E: 56                 push        rsi
  0000000140ED646F: 57                 push        rdi
  0000000140ED6470: 48 81 EC E8 01 00  sub         rsp,1E8h
                    00
  0000000140ED6477: 48 C7 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140ED6483: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED648B: E8 60 A6 25 FF     call        0000000140130AF0
  0000000140ED6490: 48 8B C8           mov         rcx,rax
  0000000140ED6493: E8 68 3F 18 FF     call        000000014005A400
  0000000140ED6498: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140ED649D: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED64A5: E8 46 A6 25 FF     call        0000000140130AF0
  0000000140ED64AA: 48 8B C8           mov         rcx,rax
  0000000140ED64AD: E8 4E 62 2F FF     call        00000001401CC700
  0000000140ED64B2: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140ED64B7: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  0000000140ED64BF: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140ED64C4: E8 67 39 E7 FF     call        0000000140D49E30
  0000000140ED64C9: 90                 nop
  0000000140ED64CA: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140ED64D2: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  0000000140ED64D8: 0F 84 D5 00 00 00  je          0000000140ED65B3
  0000000140ED64DE: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140ED64E3: E8 F8 E5 12 FF     call        0000000140004AE0
  0000000140ED64E8: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140ED64EC: 83 BC 24 08 02 00  cmp         dword ptr [rsp+0000000000000208h],0FFFFFFFFh
                    00 FF
  0000000140ED64F4: 7E 4E              jle         0000000140ED6544
  0000000140ED64F6: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140ED64FA: 39 84 24 08 02 00  cmp         dword ptr [rsp+0000000000000208h],eax
                    00
  0000000140ED6501: 7D 41              jge         0000000140ED6544
  0000000140ED6503: 8B 84 24 08 02 00  mov         eax,dword ptr [rsp+0000000000000208h]
                    00
  0000000140ED650A: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140ED650E: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140ED6512: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140ED6517: E8 E4 0B 19 FF     call        0000000140067100
  0000000140ED651C: 48 8D BC 24 F8 00  lea         rdi,[rsp+00000000000000F8h]
                    00 00
  0000000140ED6524: 48 8B F0           mov         rsi,rax
  0000000140ED6527: B9 20 00 00 00     mov         ecx,20h
  0000000140ED652C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140ED652E: 48 8D 7C 24 38     lea         rdi,[rsp+38h]
  0000000140ED6533: 48 8D B4 24 F8 00  lea         rsi,[rsp+00000000000000F8h]
                    00 00
  0000000140ED653B: B9 20 00 00 00     mov         ecx,20h
  0000000140ED6540: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140ED6542: EB 6D              jmp         0000000140ED65B1
  0000000140ED6544: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140ED6548: 39 84 24 08 02 00  cmp         dword ptr [rsp+0000000000000208h],eax
                    00
  0000000140ED654F: 7C 60              jl          0000000140ED65B1
  0000000140ED6551: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140ED6556: E8 85 E5 12 FF     call        0000000140004AE0
  0000000140ED655B: 8B D0              mov         edx,eax
  0000000140ED655D: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140ED6561: 03 C2              add         eax,edx
  0000000140ED6563: 39 84 24 08 02 00  cmp         dword ptr [rsp+0000000000000208h],eax
                    00
  0000000140ED656A: 7D 45              jge         0000000140ED65B1
  0000000140ED656C: 8B 4C 24 30        mov         ecx,dword ptr [rsp+30h]
  0000000140ED6570: 8B 84 24 08 02 00  mov         eax,dword ptr [rsp+0000000000000208h]
                    00
  0000000140ED6577: 2B C1              sub         eax,ecx
  0000000140ED6579: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140ED657D: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140ED6581: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140ED6586: E8 75 0B 19 FF     call        0000000140067100
  0000000140ED658B: 48 8D BC 24 D8 00  lea         rdi,[rsp+00000000000000D8h]
                    00 00
  0000000140ED6593: 48 8B F0           mov         rsi,rax
  0000000140ED6596: B9 20 00 00 00     mov         ecx,20h
  0000000140ED659B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140ED659D: 48 8D 7C 24 38     lea         rdi,[rsp+38h]
  0000000140ED65A2: 48 8D B4 24 D8 00  lea         rsi,[rsp+00000000000000D8h]
                    00 00
  0000000140ED65AA: B9 20 00 00 00     mov         ecx,20h
  0000000140ED65AF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140ED65B1: EB 50              jmp         0000000140ED6603
  0000000140ED65B3: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140ED65B8: E8 53 0E 43 FF     call        0000000140307410
  0000000140ED65BD: 0F B6 C0           movzx       eax,al
  0000000140ED65C0: 85 C0              test        eax,eax
  0000000140ED65C2: 75 3F              jne         0000000140ED6603
  0000000140ED65C4: 8B 84 24 08 02 00  mov         eax,dword ptr [rsp+0000000000000208h]
                    00
  0000000140ED65CB: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140ED65CF: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  0000000140ED65D3: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  0000000140ED65D8: E8 23 0B 19 FF     call        0000000140067100
  0000000140ED65DD: 48 8D BC 24 B8 00  lea         rdi,[rsp+00000000000000B8h]
                    00 00
  0000000140ED65E5: 48 8B F0           mov         rsi,rax
  0000000140ED65E8: B9 20 00 00 00     mov         ecx,20h
  0000000140ED65ED: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140ED65EF: 48 8D 7C 24 38     lea         rdi,[rsp+38h]
  0000000140ED65F4: 48 8D B4 24 B8 00  lea         rsi,[rsp+00000000000000B8h]
                    00 00
  0000000140ED65FC: B9 20 00 00 00     mov         ecx,20h
  0000000140ED6601: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140ED6603: 83 7C 24 20 FF     cmp         dword ptr [rsp+20h],0FFFFFFFFh
  0000000140ED6608: 0F 8E DD 09 00 00  jle         0000000140ED6FEB
  0000000140ED660E: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140ED6613: 0F 85 BE 06 00 00  jne         0000000140ED6CD7
  0000000140ED6619: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140ED661D: 89 84 24 20 01 00  mov         dword ptr [rsp+0000000000000120h],eax
                    00
  0000000140ED6624: 8B 84 24 20 01 00  mov         eax,dword ptr [rsp+0000000000000120h]
                    00
  0000000140ED662B: 83 E8 02           sub         eax,2
  0000000140ED662E: 89 84 24 20 01 00  mov         dword ptr [rsp+0000000000000120h],eax
                    00
  0000000140ED6635: 83 BC 24 20 01 00  cmp         dword ptr [rsp+0000000000000120h],5
                    00 05
  0000000140ED663D: 0F 87 8F 06 00 00  ja          0000000140ED6CD2
  0000000140ED6643: 48 63 84 24 20 01  movsxd      rax,dword ptr [rsp+0000000000000120h]
                    00 00
  0000000140ED664B: 48 8D 0D AE 99 12  lea         rcx,[0000000140000000h]
                    FF
  0000000140ED6652: 8B 84 81 00 70 ED  mov         eax,dword ptr [rcx+rax*4+0000000000ED7000h]
                    00
  0000000140ED6659: 48 03 C1           add         rax,rcx
  0000000140ED665C: FF E0              jmp         rax
  0000000140ED665E: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6666: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED666D: 48 8B 80 B0 17 00  mov         rax,qword ptr [rax+00000000000017B0h]
                    00
  0000000140ED6674: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED667C: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6683: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED668A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED668D: B2 01              mov         dl,1
  0000000140ED668F: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6692: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED669A: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED66A1: 48 8B 80 C0 17 00  mov         rax,qword ptr [rax+00000000000017C0h]
                    00
  0000000140ED66A8: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED66B0: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED66B7: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED66BE: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED66C1: B2 01              mov         dl,1
  0000000140ED66C3: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED66C6: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED66CE: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED66D5: 33 D2              xor         edx,edx
  0000000140ED66D7: 48 8B 89 B8 17 00  mov         rcx,qword ptr [rcx+00000000000017B8h]
                    00
  0000000140ED66DE: FF 15 E4 4D F3 00  call        qword ptr [0000000141E0B4C8h]
  0000000140ED66E4: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED66EA: 41 B0 67           mov         r8b,67h
  0000000140ED66ED: F2 0F 10 4C 24 40  movsd       xmm1,mmword ptr [rsp+40h]
  0000000140ED66F3: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140ED66F8: FF 15 9A E0 F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED66FE: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  0000000140ED6706: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140ED670E: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  0000000140ED6716: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED671E: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6725: 48 8B 94 24 30 01  mov         rdx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140ED672D: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6734: FF 15 7E FF F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED673A: 90                 nop
  0000000140ED673B: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140ED6740: FF 15 9A F2 F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED6746: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED674C: 41 B0 67           mov         r8b,67h
  0000000140ED674F: F2 0F 10 4C 24 48  movsd       xmm1,mmword ptr [rsp+48h]
  0000000140ED6755: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140ED675A: FF 15 38 E0 F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED6760: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  0000000140ED6768: 48 8B 84 24 38 01  mov         rax,qword ptr [rsp+0000000000000138h]
                    00 00
  0000000140ED6770: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  0000000140ED6778: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6780: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6787: 48 8B 94 24 40 01  mov         rdx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140ED678F: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED6796: FF 15 1C FF F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED679C: 90                 nop
  0000000140ED679D: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140ED67A2: FF 15 38 F2 F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED67A8: E9 25 05 00 00     jmp         0000000140ED6CD2
  0000000140ED67AD: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED67B5: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED67BC: 48 8B 80 B0 17 00  mov         rax,qword ptr [rax+00000000000017B0h]
                    00
  0000000140ED67C3: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED67CB: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED67D2: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED67D9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED67DC: B2 01              mov         dl,1
  0000000140ED67DE: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED67E1: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED67E9: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED67F0: 48 8B 80 C0 17 00  mov         rax,qword ptr [rax+00000000000017C0h]
                    00
  0000000140ED67F7: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED67FF: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6806: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED680D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6810: B2 01              mov         dl,1
  0000000140ED6812: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6815: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED681D: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6824: BA 01 00 00 00     mov         edx,1
  0000000140ED6829: 48 8B 89 B8 17 00  mov         rcx,qword ptr [rcx+00000000000017B8h]
                    00
  0000000140ED6830: FF 15 92 4C F3 00  call        qword ptr [0000000141E0B4C8h]
  0000000140ED6836: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED683C: 41 B0 67           mov         r8b,67h
  0000000140ED683F: F2 0F 10 4C 24 40  movsd       xmm1,mmword ptr [rsp+40h]
  0000000140ED6845: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140ED684A: FF 15 48 DF F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED6850: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  0000000140ED6858: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140ED6860: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  0000000140ED6868: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6870: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6877: 48 8B 94 24 50 01  mov         rdx,qword ptr [rsp+0000000000000150h]
                    00 00
  0000000140ED687F: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6886: FF 15 2C FE F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED688C: 90                 nop
  0000000140ED688D: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140ED6892: FF 15 48 F1 F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED6898: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED689E: 41 B0 67           mov         r8b,67h
  0000000140ED68A1: F2 0F 10 4C 24 48  movsd       xmm1,mmword ptr [rsp+48h]
  0000000140ED68A7: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140ED68AC: FF 15 E6 DE F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED68B2: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  0000000140ED68BA: 48 8B 84 24 58 01  mov         rax,qword ptr [rsp+0000000000000158h]
                    00 00
  0000000140ED68C2: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  0000000140ED68CA: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED68D2: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED68D9: 48 8B 94 24 60 01  mov         rdx,qword ptr [rsp+0000000000000160h]
                    00 00
  0000000140ED68E1: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED68E8: FF 15 CA FD F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED68EE: 90                 nop
  0000000140ED68EF: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140ED68F4: FF 15 E6 F0 F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED68FA: E9 D3 03 00 00     jmp         0000000140ED6CD2
  0000000140ED68FF: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6907: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED690E: 48 8B 80 B0 17 00  mov         rax,qword ptr [rax+00000000000017B0h]
                    00
  0000000140ED6915: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED691D: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6924: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED692B: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED692E: B2 01              mov         dl,1
  0000000140ED6930: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6933: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED693B: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6942: 48 8B 80 C0 17 00  mov         rax,qword ptr [rax+00000000000017C0h]
                    00
  0000000140ED6949: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6951: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6958: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED695F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6962: 33 D2              xor         edx,edx
  0000000140ED6964: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6967: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED696F: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6976: BA 02 00 00 00     mov         edx,2
  0000000140ED697B: 48 8B 89 B8 17 00  mov         rcx,qword ptr [rcx+00000000000017B8h]
                    00
  0000000140ED6982: FF 15 40 4B F3 00  call        qword ptr [0000000141E0B4C8h]
  0000000140ED6988: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED698E: 41 B0 67           mov         r8b,67h
  0000000140ED6991: F2 0F 10 4C 24 40  movsd       xmm1,mmword ptr [rsp+40h]
  0000000140ED6997: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140ED699F: FF 15 F3 DD F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED69A5: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  0000000140ED69AD: 48 8B 84 24 68 01  mov         rax,qword ptr [rsp+0000000000000168h]
                    00 00
  0000000140ED69B5: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  0000000140ED69BD: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED69C5: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED69CC: 48 8B 94 24 70 01  mov         rdx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140ED69D4: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED69DB: FF 15 D7 FC F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED69E1: 90                 nop
  0000000140ED69E2: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140ED69EA: FF 15 F0 EF F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED69F0: E9 DD 02 00 00     jmp         0000000140ED6CD2
  0000000140ED69F5: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED69FD: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6A04: 48 8B 80 B0 17 00  mov         rax,qword ptr [rax+00000000000017B0h]
                    00
  0000000140ED6A0B: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6A13: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6A1A: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6A21: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6A24: B2 01              mov         dl,1
  0000000140ED6A26: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6A29: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6A31: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6A38: 48 8B 80 C0 17 00  mov         rax,qword ptr [rax+00000000000017C0h]
                    00
  0000000140ED6A3F: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6A47: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6A4E: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED6A55: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6A58: 33 D2              xor         edx,edx
  0000000140ED6A5A: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6A5D: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6A65: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6A6C: BA 03 00 00 00     mov         edx,3
  0000000140ED6A71: 48 8B 89 B8 17 00  mov         rcx,qword ptr [rcx+00000000000017B8h]
                    00
  0000000140ED6A78: FF 15 4A 4A F3 00  call        qword ptr [0000000141E0B4C8h]
  0000000140ED6A7E: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED6A84: 41 B0 67           mov         r8b,67h
  0000000140ED6A87: F2 0F 10 4C 24 40  movsd       xmm1,mmword ptr [rsp+40h]
  0000000140ED6A8D: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140ED6A95: FF 15 FD DC F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED6A9B: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  0000000140ED6AA3: 48 8B 84 24 78 01  mov         rax,qword ptr [rsp+0000000000000178h]
                    00 00
  0000000140ED6AAB: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  0000000140ED6AB3: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6ABB: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6AC2: 48 8B 94 24 80 01  mov         rdx,qword ptr [rsp+0000000000000180h]
                    00 00
  0000000140ED6ACA: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6AD1: FF 15 E1 FB F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED6AD7: 90                 nop
  0000000140ED6AD8: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140ED6AE0: FF 15 FA EE F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED6AE6: E9 E7 01 00 00     jmp         0000000140ED6CD2
  0000000140ED6AEB: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6AF3: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6AFA: 48 8B 80 B0 17 00  mov         rax,qword ptr [rax+00000000000017B0h]
                    00
  0000000140ED6B01: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6B09: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6B10: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6B17: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6B1A: B2 01              mov         dl,1
  0000000140ED6B1C: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6B1F: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6B27: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6B2E: 48 8B 80 C0 17 00  mov         rax,qword ptr [rax+00000000000017C0h]
                    00
  0000000140ED6B35: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6B3D: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6B44: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED6B4B: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6B4E: 33 D2              xor         edx,edx
  0000000140ED6B50: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6B53: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6B5B: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6B62: BA 04 00 00 00     mov         edx,4
  0000000140ED6B67: 48 8B 89 B8 17 00  mov         rcx,qword ptr [rcx+00000000000017B8h]
                    00
  0000000140ED6B6E: FF 15 54 49 F3 00  call        qword ptr [0000000141E0B4C8h]
  0000000140ED6B74: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED6B7A: 41 B0 67           mov         r8b,67h
  0000000140ED6B7D: F2 0F 10 4C 24 40  movsd       xmm1,mmword ptr [rsp+40h]
  0000000140ED6B83: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140ED6B8B: FF 15 07 DC F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED6B91: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  0000000140ED6B99: 48 8B 84 24 88 01  mov         rax,qword ptr [rsp+0000000000000188h]
                    00 00
  0000000140ED6BA1: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  0000000140ED6BA9: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6BB1: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6BB8: 48 8B 94 24 90 01  mov         rdx,qword ptr [rsp+0000000000000190h]
                    00 00
  0000000140ED6BC0: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6BC7: FF 15 EB FA F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED6BCD: 90                 nop
  0000000140ED6BCE: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140ED6BD6: FF 15 04 EE F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED6BDC: E9 F1 00 00 00     jmp         0000000140ED6CD2
  0000000140ED6BE1: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6BE9: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6BF0: 48 8B 80 B0 17 00  mov         rax,qword ptr [rax+00000000000017B0h]
                    00
  0000000140ED6BF7: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6BFF: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6C06: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6C0D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6C10: B2 01              mov         dl,1
  0000000140ED6C12: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6C15: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6C1D: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6C24: 48 8B 80 C0 17 00  mov         rax,qword ptr [rax+00000000000017C0h]
                    00
  0000000140ED6C2B: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6C33: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6C3A: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED6C41: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6C44: 33 D2              xor         edx,edx
  0000000140ED6C46: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6C49: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6C51: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6C58: BA 05 00 00 00     mov         edx,5
  0000000140ED6C5D: 48 8B 89 B8 17 00  mov         rcx,qword ptr [rcx+00000000000017B8h]
                    00
  0000000140ED6C64: FF 15 5E 48 F3 00  call        qword ptr [0000000141E0B4C8h]
  0000000140ED6C6A: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED6C70: 41 B0 67           mov         r8b,67h
  0000000140ED6C73: F2 0F 10 4C 24 40  movsd       xmm1,mmword ptr [rsp+40h]
  0000000140ED6C79: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140ED6C81: FF 15 11 DB F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED6C87: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  0000000140ED6C8F: 48 8B 84 24 98 01  mov         rax,qword ptr [rsp+0000000000000198h]
                    00 00
  0000000140ED6C97: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  0000000140ED6C9F: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6CA7: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6CAE: 48 8B 94 24 A0 01  mov         rdx,qword ptr [rsp+00000000000001A0h]
                    00 00
  0000000140ED6CB6: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6CBD: FF 15 F5 F9 F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED6CC3: 90                 nop
  0000000140ED6CC4: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140ED6CCC: FF 15 0E ED F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED6CD2: E9 14 03 00 00     jmp         0000000140ED6FEB
  0000000140ED6CD7: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140ED6CDB: 89 84 24 A8 01 00  mov         dword ptr [rsp+00000000000001A8h],eax
                    00
  0000000140ED6CE2: 83 BC 24 A8 01 00  cmp         dword ptr [rsp+00000000000001A8h],1
                    00 01
  0000000140ED6CEA: 74 21              je          0000000140ED6D0D
  0000000140ED6CEC: 83 BC 24 A8 01 00  cmp         dword ptr [rsp+00000000000001A8h],2
                    00 02
  0000000140ED6CF4: 0F 84 09 01 00 00  je          0000000140ED6E03
  0000000140ED6CFA: 83 BC 24 A8 01 00  cmp         dword ptr [rsp+00000000000001A8h],3
                    00 03
  0000000140ED6D02: 0F 84 F1 01 00 00  je          0000000140ED6EF9
  0000000140ED6D08: E9 DE 02 00 00     jmp         0000000140ED6FEB
  0000000140ED6D0D: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6D15: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6D1C: 48 8B 80 B0 17 00  mov         rax,qword ptr [rax+00000000000017B0h]
                    00
  0000000140ED6D23: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6D2B: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6D32: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6D39: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6D3C: B2 01              mov         dl,1
  0000000140ED6D3E: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6D41: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6D49: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6D50: 48 8B 80 C0 17 00  mov         rax,qword ptr [rax+00000000000017C0h]
                    00
  0000000140ED6D57: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6D5F: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6D66: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED6D6D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6D70: 33 D2              xor         edx,edx
  0000000140ED6D72: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6D75: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6D7D: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6D84: BA 06 00 00 00     mov         edx,6
  0000000140ED6D89: 48 8B 89 B8 17 00  mov         rcx,qword ptr [rcx+00000000000017B8h]
                    00
  0000000140ED6D90: FF 15 32 47 F3 00  call        qword ptr [0000000141E0B4C8h]
  0000000140ED6D96: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED6D9C: 41 B0 67           mov         r8b,67h
  0000000140ED6D9F: F2 0F 10 4C 24 40  movsd       xmm1,mmword ptr [rsp+40h]
  0000000140ED6DA5: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140ED6DAD: FF 15 E5 D9 F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED6DB3: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  0000000140ED6DBB: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140ED6DC3: 48 89 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],rax
                    00 00
  0000000140ED6DCB: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6DD3: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6DDA: 48 8B 94 24 B8 01  mov         rdx,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140ED6DE2: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6DE9: FF 15 C9 F8 F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED6DEF: 90                 nop
  0000000140ED6DF0: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140ED6DF8: FF 15 E2 EB F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED6DFE: E9 E8 01 00 00     jmp         0000000140ED6FEB
  0000000140ED6E03: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6E0B: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6E12: 48 8B 80 B0 17 00  mov         rax,qword ptr [rax+00000000000017B0h]
                    00
  0000000140ED6E19: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6E21: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6E28: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6E2F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6E32: B2 01              mov         dl,1
  0000000140ED6E34: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6E37: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6E3F: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6E46: 48 8B 80 C0 17 00  mov         rax,qword ptr [rax+00000000000017C0h]
                    00
  0000000140ED6E4D: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6E55: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6E5C: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED6E63: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6E66: 33 D2              xor         edx,edx
  0000000140ED6E68: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6E6B: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6E73: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6E7A: BA 07 00 00 00     mov         edx,7
  0000000140ED6E7F: 48 8B 89 B8 17 00  mov         rcx,qword ptr [rcx+00000000000017B8h]
                    00
  0000000140ED6E86: FF 15 3C 46 F3 00  call        qword ptr [0000000141E0B4C8h]
  0000000140ED6E8C: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED6E92: 41 B0 67           mov         r8b,67h
  0000000140ED6E95: F2 0F 10 4C 24 40  movsd       xmm1,mmword ptr [rsp+40h]
  0000000140ED6E9B: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140ED6EA3: FF 15 EF D8 F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED6EA9: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  0000000140ED6EB1: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140ED6EB9: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  0000000140ED6EC1: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6EC9: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6ED0: 48 8B 94 24 C8 01  mov         rdx,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000140ED6ED8: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6EDF: FF 15 D3 F7 F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED6EE5: 90                 nop
  0000000140ED6EE6: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140ED6EEE: FF 15 EC EA F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED6EF4: E9 F2 00 00 00     jmp         0000000140ED6FEB
  0000000140ED6EF9: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6F01: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6F08: 48 8B 80 B0 17 00  mov         rax,qword ptr [rax+00000000000017B0h]
                    00
  0000000140ED6F0F: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6F17: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6F1E: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6F25: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6F28: B2 01              mov         dl,1
  0000000140ED6F2A: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6F2D: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6F35: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140ED6F3C: 48 8B 80 C0 17 00  mov         rax,qword ptr [rax+00000000000017C0h]
                    00
  0000000140ED6F43: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6F4B: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6F52: 48 8B 89 C0 17 00  mov         rcx,qword ptr [rcx+00000000000017C0h]
                    00
  0000000140ED6F59: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140ED6F5C: 33 D2              xor         edx,edx
  0000000140ED6F5E: FF 50 58           call        qword ptr [rax+58h]
  0000000140ED6F61: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6F69: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6F70: BA 08 00 00 00     mov         edx,8
  0000000140ED6F75: 48 8B 89 B8 17 00  mov         rcx,qword ptr [rcx+00000000000017B8h]
                    00
  0000000140ED6F7C: FF 15 46 45 F3 00  call        qword ptr [0000000141E0B4C8h]
  0000000140ED6F82: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ED6F88: 41 B0 67           mov         r8b,67h
  0000000140ED6F8B: F2 0F 10 4C 24 40  movsd       xmm1,mmword ptr [rsp+40h]
  0000000140ED6F91: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140ED6F99: FF 15 F9 D7 F2 00  call        qword ptr [0000000141E04798h]
  0000000140ED6F9F: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
  0000000140ED6FA7: 48 8B 84 24 D0 01  mov         rax,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140ED6FAF: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  0000000140ED6FB7: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ED6FBF: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ED6FC6: 48 8B 94 24 D8 01  mov         rdx,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140ED6FCE: 48 8B 89 B0 17 00  mov         rcx,qword ptr [rcx+00000000000017B0h]
                    00
  0000000140ED6FD5: FF 15 DD F6 F2 00  call        qword ptr [0000000141E066B8h]
  0000000140ED6FDB: 90                 nop
  0000000140ED6FDC: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140ED6FE4: FF 15 F6 E9 F2 00  call        qword ptr [0000000141E059E0h]
  0000000140ED6FEA: 90                 nop
  0000000140ED6FEB: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140ED6FF0: E8 3B 51 DD FF     call        0000000140CAC130
  0000000140ED6FF5: 48 81 C4 E8 01 00  add         rsp,1E8h
                    00
  0000000140ED6FFC: 5F                 pop         rdi
  0000000140ED6FFD: 5E                 pop         rsi
  0000000140ED6FFE: C3                 ret
  0000000140ED6FFF: 90                 nop
  0000000140ED7000: 5E                 pop         rsi
  0000000140ED7001: 66 ED              in          ax,dx
  0000000140ED7003: 00 AD 67 ED 00 FF  add         byte ptr [rbp+FFFFFFFFFF00ED67h],ch
  0000000140ED7009: 68 ED 00 F5 69     push        69F500EDh
  0000000140ED700E: ED                 in          eax,dx
  0000000140ED700F: 00 EB              add         bl,ch
  0000000140ED7011: 6A ED              push        0FFFFFFFFFFFFFFEDh
  0000000140ED7013: 00 E1              add         cl,ah
  0000000140ED7015: 6B ED 00           imul        ebp,ebp,0

  Summary

     1E02000 .text

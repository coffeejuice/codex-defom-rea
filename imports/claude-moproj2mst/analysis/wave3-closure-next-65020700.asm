Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065020700: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065020705: 55                 push        rbp
  0000000065020706: 56                 push        rsi
  0000000065020707: 57                 push        rdi
  0000000065020708: 41 54              push        r12
  000000006502070A: 41 55              push        r13
  000000006502070C: 48 83 EC 40        sub         rsp,40h
  0000000065020710: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065020719: 48 89 9C 24 80 00  mov         qword ptr [rsp+0000000000000080h],rbx
                    00 00
  0000000065020721: 4C 8B E1           mov         r12,rcx
  0000000065020724: 48 8D 05 8D EC 67  lea         rax,[000000006569F3B8h]
                    00
  000000006502072B: 48 89 01           mov         qword ptr [rcx],rax
  000000006502072E: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  0000000065020732: 48 83 3D D6 8F 9A  cmp         qword ptr [00000000659C9710h],0
                    00 00
  000000006502073A: 74 2F              je          000000006502076B
  000000006502073C: BA 28 00 00 00     mov         edx,28h
  0000000065020741: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065020746: FF 15 CC 60 67 00  call        qword ptr [0000000065696818h]
  000000006502074C: 90                 nop
  000000006502074D: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065020752: 48 8B 0D B7 8F 9A  mov         rcx,qword ptr [00000000659C9710h]
                    00
  0000000065020759: FF 15 B1 60 67 00  call        qword ptr [0000000065696810h]
  000000006502075F: 90                 nop
  0000000065020760: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065020765: FF 15 9D 60 67 00  call        qword ptr [0000000065696808h]
  000000006502076B: FF 15 DF 65 67 00  call        qword ptr [0000000065696D50h]
  0000000065020771: 48 8D 8F F8 00 00  lea         rcx,[rdi+00000000000000F8h]
                    00
  0000000065020778: FF 15 CA 65 67 00  call        qword ptr [0000000065696D48h]
  000000006502077E: 48 8D 8F FC 00 00  lea         rcx,[rdi+00000000000000FCh]
                    00
  0000000065020785: FF 15 BD 65 67 00  call        qword ptr [0000000065696D48h]
  000000006502078B: 4C 8B 1D AE 65 67  mov         r11,qword ptr [0000000065696D40h]
                    00
  0000000065020792: 49 8B 0B           mov         rcx,qword ptr [r11]
  0000000065020795: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065020798: FF 90 B8 00 00 00  call        qword ptr [rax+00000000000000B8h]
  000000006502079E: 45 33 ED           xor         r13d,r13d
  00000000650207A1: 48 8B 05 98 65 67  mov         rax,qword ptr [0000000065696D40h]
                    00
  00000000650207A8: 4C 89 28           mov         qword ptr [rax],r13
  00000000650207AB: 48 8B 05 AE 64 67  mov         rax,qword ptr [0000000065696C60h]
                    00
  00000000650207B2: C6 00 01           mov         byte ptr [rax],1
  00000000650207B5: 48 8B 05 AC 64 67  mov         rax,qword ptr [0000000065696C68h]
                    00
  00000000650207BC: 44 88 28           mov         byte ptr [rax],r13b
  00000000650207BF: 48 8B 1D 72 91 9A  mov         rbx,qword ptr [00000000659C9938h]
                    00
  00000000650207C6: 48 85 DB           test        rbx,rbx
  00000000650207C9: 74 29              je          00000000650207F4
  00000000650207CB: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000650207CE: 48 83 C1 10        add         rcx,10h
  00000000650207D2: FF 15 A0 61 67 00  call        qword ptr [0000000065696978h]
  00000000650207D8: 84 C0              test        al,al
  00000000650207DA: 75 10              jne         00000000650207EC
  00000000650207DC: 48 8D 15 CD EA 07  lea         rdx,[000000006509F2B0h]
                    00
  00000000650207E3: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000650207E6: FF 15 C4 63 67 00  call        qword ptr [0000000065696BB0h]
  00000000650207EC: 48 8B CB           mov         rcx,rbx
  00000000650207EF: E8 BA 08 5C 00     call        00000000655E10AE
  00000000650207F4: 4C 89 2D 3D 91 9A  mov         qword ptr [00000000659C9938h],r13
                    00
  00000000650207FB: 48 8B 35 3E 91 9A  mov         rsi,qword ptr [00000000659C9940h]
                    00
  0000000065020802: 48 85 F6           test        rsi,rsi
  0000000065020805: 74 74              je          000000006502087B
  0000000065020807: 4C 89 2D 32 91 9A  mov         qword ptr [00000000659C9940h],r13
                    00
  000000006502080E: 48 8B 0E           mov         rcx,qword ptr [rsi]
  0000000065020811: FF 15 81 63 67 00  call        qword ptr [0000000065696B98h]
  0000000065020817: 48 8B D8           mov         rbx,rax
  000000006502081A: 48 3B 06           cmp         rax,qword ptr [rsi]
  000000006502081D: 74 33              je          0000000065020852
  000000006502081F: 90                 nop
  0000000065020820: 48 8B 6B 10        mov         rbp,qword ptr [rbx+10h]
  0000000065020824: 48 8B CD           mov         rcx,rbp
  0000000065020827: FF 15 3B 60 67 00  call        qword ptr [0000000065696868h]
  000000006502082D: 48 85 C0           test        rax,rax
  0000000065020830: 75 0F              jne         0000000065020841
  0000000065020832: 41 B0 01           mov         r8b,1
  0000000065020835: 41 0F B6 D0        movzx       edx,r8b
  0000000065020839: 48 8B CD           mov         rcx,rbp
  000000006502083C: E8 2F 8D 07 00     call        0000000065099570
  0000000065020841: 48 8B CB           mov         rcx,rbx
  0000000065020844: FF 15 FE 62 67 00  call        qword ptr [0000000065696B48h]
  000000006502084A: 48 8B D8           mov         rbx,rax
  000000006502084D: 48 3B 06           cmp         rax,qword ptr [rsi]
  0000000065020850: 75 CE              jne         0000000065020820
  0000000065020852: 48 8B 0E           mov         rcx,qword ptr [rsi]
  0000000065020855: 48 83 C1 10        add         rcx,10h
  0000000065020859: FF 15 19 61 67 00  call        qword ptr [0000000065696978h]
  000000006502085F: 84 C0              test        al,al
  0000000065020861: 75 10              jne         0000000065020873
  0000000065020863: 48 8D 15 46 EA 07  lea         rdx,[000000006509F2B0h]
                    00
  000000006502086A: 48 8B 0E           mov         rcx,qword ptr [rsi]
  000000006502086D: FF 15 3D 63 67 00  call        qword ptr [0000000065696BB0h]
  0000000065020873: 48 8B CE           mov         rcx,rsi
  0000000065020876: E8 33 08 5C 00     call        00000000655E10AE
  000000006502087B: 48 8B 0D 86 8E 9A  mov         rcx,qword ptr [00000000659C9708h]
                    00
  0000000065020882: 48 85 C9           test        rcx,rcx
  0000000065020885: 74 0B              je          0000000065020892
  0000000065020887: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006502088A: BA 01 00 00 00     mov         edx,1
  000000006502088F: FF 50 18           call        qword ptr [rax+18h]
  0000000065020892: 4C 89 2D 6F 8E 9A  mov         qword ptr [00000000659C9708h],r13
                    00
  0000000065020899: 48 8B 0D 70 8E 9A  mov         rcx,qword ptr [00000000659C9710h]
                    00
  00000000650208A0: 48 85 C9           test        rcx,rcx
  00000000650208A3: 74 0B              je          00000000650208B0
  00000000650208A5: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000650208A8: BA 01 00 00 00     mov         edx,1
  00000000650208AD: FF 50 18           call        qword ptr [rax+18h]
  00000000650208B0: 4C 89 2D 59 8E 9A  mov         qword ptr [00000000659C9710h],r13
                    00
  00000000650208B7: 48 8B 8F 58 01 00  mov         rcx,qword ptr [rdi+0000000000000158h]
                    00
  00000000650208BE: 48 85 C9           test        rcx,rcx
  00000000650208C1: 74 0A              je          00000000650208CD
  00000000650208C3: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000650208C6: BA 01 00 00 00     mov         edx,1
  00000000650208CB: FF 10              call        qword ptr [rax]
  00000000650208CD: 4C 89 AF 58 01 00  mov         qword ptr [rdi+0000000000000158h],r13
                    00
  00000000650208D4: 48 8B 8F 60 01 00  mov         rcx,qword ptr [rdi+0000000000000160h]
                    00
  00000000650208DB: 48 85 C9           test        rcx,rcx
  00000000650208DE: 74 0A              je          00000000650208EA
  00000000650208E0: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000650208E3: BA 01 00 00 00     mov         edx,1
  00000000650208E8: FF 10              call        qword ptr [rax]
  00000000650208EA: 4C 89 AF 60 01 00  mov         qword ptr [rdi+0000000000000160h],r13
                    00
  00000000650208F1: 48 8B 8F 68 01 00  mov         rcx,qword ptr [rdi+0000000000000168h]
                    00
  00000000650208F8: 48 85 C9           test        rcx,rcx
  00000000650208FB: 74 0A              je          0000000065020907
  00000000650208FD: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065020900: BA 01 00 00 00     mov         edx,1
  0000000065020905: FF 10              call        qword ptr [rax]
  0000000065020907: 4C 89 AF 68 01 00  mov         qword ptr [rdi+0000000000000168h],r13
                    00
  000000006502090E: 48 8B 8F 70 01 00  mov         rcx,qword ptr [rdi+0000000000000170h]
                    00
  0000000065020915: 48 85 C9           test        rcx,rcx
  0000000065020918: 74 0A              je          0000000065020924
  000000006502091A: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006502091D: BA 01 00 00 00     mov         edx,1
  0000000065020922: FF 10              call        qword ptr [rax]
  0000000065020924: 4C 89 AF 70 01 00  mov         qword ptr [rdi+0000000000000170h],r13
                    00
  000000006502092B: 48 8B 1D 5E 8D 9A  mov         rbx,qword ptr [00000000659C9690h]
                    00
  0000000065020932: 48 85 DB           test        rbx,rbx
  0000000065020935: 74 10              je          0000000065020947
  0000000065020937: 48 8B CB           mov         rcx,rbx
  000000006502093A: E8 91 98 02 00     call        000000006504A1D0
  000000006502093F: 48 8B CB           mov         rcx,rbx
  0000000065020942: E8 67 07 5C 00     call        00000000655E10AE
  0000000065020947: 4C 89 2D 42 8D 9A  mov         qword ptr [00000000659C9690h],r13
                    00
  000000006502094E: 48 8B 1D 43 8D 9A  mov         rbx,qword ptr [00000000659C9698h]
                    00
  0000000065020955: 48 85 DB           test        rbx,rbx
  0000000065020958: 74 10              je          000000006502096A
  000000006502095A: 48 8B CB           mov         rcx,rbx
  000000006502095D: E8 6E 98 02 00     call        000000006504A1D0
  0000000065020962: 48 8B CB           mov         rcx,rbx
  0000000065020965: E8 44 07 5C 00     call        00000000655E10AE
  000000006502096A: 4C 89 2D 27 8D 9A  mov         qword ptr [00000000659C9698h],r13
                    00
  0000000065020971: 48 8B 1D 28 8D 9A  mov         rbx,qword ptr [00000000659C96A0h]
                    00
  0000000065020978: 48 85 DB           test        rbx,rbx
  000000006502097B: 74 10              je          000000006502098D
  000000006502097D: 48 8B CB           mov         rcx,rbx
  0000000065020980: E8 4B 98 02 00     call        000000006504A1D0
  0000000065020985: 48 8B CB           mov         rcx,rbx
  0000000065020988: E8 21 07 5C 00     call        00000000655E10AE
  000000006502098D: 4C 89 2D 0C 8D 9A  mov         qword ptr [00000000659C96A0h],r13
                    00
  0000000065020994: E8 F7 F3 FF FF     call        000000006501FD90
  0000000065020999: 48 8B C8           mov         rcx,rax
  000000006502099C: E8 8F D1 FF FF     call        000000006501DB30
  00000000650209A1: E8 AA A8 FF FF     call        000000006501B250
  00000000650209A6: 48 8B D0           mov         rdx,rax
  00000000650209A9: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000000650209AE: FF 15 5C 62 67 00  call        qword ptr [0000000065696C10h]
  00000000650209B4: 90                 nop
  00000000650209B5: 48 8B 1D FC 8C 9A  mov         rbx,qword ptr [00000000659C96B8h]
                    00
  00000000650209BC: 48 85 DB           test        rbx,rbx
  00000000650209BF: 74 10              je          00000000650209D1
  00000000650209C1: 48 8B CB           mov         rcx,rbx
  00000000650209C4: E8 57 61 1E 00     call        0000000065206B20
  00000000650209C9: 48 8B CB           mov         rcx,rbx
  00000000650209CC: E8 DD 06 5C 00     call        00000000655E10AE
  00000000650209D1: 4C 89 2D E0 8C 9A  mov         qword ptr [00000000659C96B8h],r13
                    00
  00000000650209D8: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000000650209DD: FF 15 1D 62 67 00  call        qword ptr [0000000065696C00h]
  00000000650209E3: 48 8B 1D D6 8C 9A  mov         rbx,qword ptr [00000000659C96C0h]
                    00
  00000000650209EA: 48 85 DB           test        rbx,rbx
  00000000650209ED: 74 10              je          00000000650209FF
  00000000650209EF: 48 8B CB           mov         rcx,rbx
  00000000650209F2: E8 29 61 1E 00     call        0000000065206B20
  00000000650209F7: 48 8B CB           mov         rcx,rbx
  00000000650209FA: E8 AF 06 5C 00     call        00000000655E10AE
  00000000650209FF: 4C 89 2D BA 8C 9A  mov         qword ptr [00000000659C96C0h],r13
                    00
  0000000065020A06: 48 8B 1D BB 8C 9A  mov         rbx,qword ptr [00000000659C96C8h]
                    00
  0000000065020A0D: 48 85 DB           test        rbx,rbx
  0000000065020A10: 74 10              je          0000000065020A22
  0000000065020A12: 48 8B CB           mov         rcx,rbx
  0000000065020A15: E8 06 61 1E 00     call        0000000065206B20
  0000000065020A1A: 48 8B CB           mov         rcx,rbx
  0000000065020A1D: E8 8C 06 5C 00     call        00000000655E10AE
  0000000065020A22: 4C 89 2D 9F 8C 9A  mov         qword ptr [00000000659C96C8h],r13
                    00
  0000000065020A29: E8 72 F4 FF FF     call        000000006501FEA0
  0000000065020A2E: 48 8B C8           mov         rcx,rax
  0000000065020A31: E8 AA D1 FF FF     call        000000006501DBE0
  0000000065020A36: 48 8B 0D 4B 8C 9A  mov         rcx,qword ptr [00000000659C9688h]
                    00
  0000000065020A3D: 48 85 C9           test        rcx,rcx
  0000000065020A40: 74 0B              je          0000000065020A4D
  0000000065020A42: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065020A45: BA 01 00 00 00     mov         edx,1
  0000000065020A4A: FF 50 18           call        qword ptr [rax+18h]
  0000000065020A4D: 4C 89 2D 34 8C 9A  mov         qword ptr [00000000659C9688h],r13
                    00
  0000000065020A54: 48 8B 1D 75 8C 9A  mov         rbx,qword ptr [00000000659C96D0h]
                    00
  0000000065020A5B: 48 85 DB           test        rbx,rbx
  0000000065020A5E: 74 10              je          0000000065020A70
  0000000065020A60: 48 8B CB           mov         rcx,rbx
  0000000065020A63: E8 C8 09 08 00     call        00000000650A1430
  0000000065020A68: 48 8B CB           mov         rcx,rbx
  0000000065020A6B: E8 3E 06 5C 00     call        00000000655E10AE
  0000000065020A70: 4C 89 2D 59 8C 9A  mov         qword ptr [00000000659C96D0h],r13
                    00
  0000000065020A77: 48 8B 0D 2A 8C 9A  mov         rcx,qword ptr [00000000659C96A8h]
                    00
  0000000065020A7E: 48 85 C9           test        rcx,rcx
  0000000065020A81: 74 0B              je          0000000065020A8E
  0000000065020A83: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065020A86: BA 01 00 00 00     mov         edx,1
  0000000065020A8B: FF 50 18           call        qword ptr [rax+18h]
  0000000065020A8E: 4C 89 2D 13 8C 9A  mov         qword ptr [00000000659C96A8h],r13
                    00
  0000000065020A95: 48 8D 8F E8 00 00  lea         rcx,[rdi+00000000000000E8h]
                    00
  0000000065020A9C: E8 FF C3 FF FF     call        000000006501CEA0
  0000000065020AA1: 80 3D C0 8B 9A 00  cmp         byte ptr [00000000659C9668h],0
                    00
  0000000065020AA8: 74 19              je          0000000065020AC3
  0000000065020AAA: E8 41 FC 00 00     call        00000000650306F0
  0000000065020AAF: 48 85 C0           test        rax,rax
  0000000065020AB2: 74 0F              je          0000000065020AC3
  0000000065020AB4: 4C 8B 00           mov         r8,qword ptr [rax]
  0000000065020AB7: BA 01 00 00 00     mov         edx,1
  0000000065020ABC: 48 8B C8           mov         rcx,rax
  0000000065020ABF: 41 FF 50 18        call        qword ptr [r8+18h]
  0000000065020AC3: 48 8B CF           mov         rcx,rdi
  0000000065020AC6: E8 85 5A 06 00     call        0000000065086550
  0000000065020ACB: 48 8D 8F 78 01 00  lea         rcx,[rdi+0000000000000178h]
                    00
  0000000065020AD2: E8 A9 C4 FF FF     call        000000006501CF80
  0000000065020AD7: 48 8D 8F 80 01 00  lea         rcx,[rdi+0000000000000180h]
                    00
  0000000065020ADE: E8 DD C7 FF FF     call        000000006501D2C0
  0000000065020AE3: E8 C8 95 06 00     call        000000006508A0B0
  0000000065020AE8: 80 3D 0D 8C 9A 00  cmp         byte ptr [00000000659C96FCh],0
                    00
  0000000065020AEF: 74 1A              je          0000000065020B0B
  0000000065020AF1: 44 8B 05 54 8E 9A  mov         r8d,dword ptr [00000000659C994Ch]
                    00
  0000000065020AF8: 8B 15 4A 8E 9A 00  mov         edx,dword ptr [00000000659C9948h]
  0000000065020AFE: 48 8D 0D 83 E8 67  lea         rcx,[000000006569F388h]
                    00
  0000000065020B05: FF 15 2D 62 67 00  call        qword ptr [0000000065696D38h]
  0000000065020B0B: 48 8B 8F C8 00 00  mov         rcx,qword ptr [rdi+00000000000000C8h]
                    00
  0000000065020B12: 48 85 C9           test        rcx,rcx
  0000000065020B15: 74 0B              je          0000000065020B22
  0000000065020B17: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065020B1A: BA 01 00 00 00     mov         edx,1
  0000000065020B1F: FF 50 18           call        qword ptr [rax+18h]
  0000000065020B22: 4C 89 AF C8 00 00  mov         qword ptr [rdi+00000000000000C8h],r13
                    00
  0000000065020B29: C6 05 AA B6 99 00  mov         byte ptr [00000000659BC1DAh],1
                    01
  0000000065020B30: C7 05 A2 B6 99 00  mov         dword ptr [00000000659BC1DCh],3E8h
                    E8 03 00 00
  0000000065020B3A: C7 05 9C B6 99 00  mov         dword ptr [00000000659BC1E0h],190h
                    90 01 00 00
  0000000065020B44: C7 05 96 B6 99 00  mov         dword ptr [00000000659BC1E4h],190h
                    90 01 00 00
  0000000065020B4E: C7 05 94 B6 99 00  mov         dword ptr [00000000659BC1ECh],1F4h
                    F4 01 00 00
  0000000065020B58: C7 05 8E B6 99 00  mov         dword ptr [00000000659BC1F0h],4
                    04 00 00 00
  0000000065020B62: 44 89 2D 4B 8B 9A  mov         dword ptr [00000000659C96B4h],r13d
                    00
  0000000065020B69: 45 33 C0           xor         r8d,r8d
  0000000065020B6C: 33 D2              xor         edx,edx
  0000000065020B6E: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000065020B73: FF 15 A7 5E 67 00  call        qword ptr [0000000065696A20h]
  0000000065020B79: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065020B7C: 48 89 0D CD 8B 9A  mov         qword ptr [00000000659C9750h],rcx
                    00
  0000000065020B83: C6 05 6A B6 99 00  mov         byte ptr [00000000659BC1F4h],1
                    01
  0000000065020B8A: C6 05 21 8B 9A 00  mov         byte ptr [00000000659C96B2h],0
                    00
  0000000065020B91: C6 05 1B 8B 9A 00  mov         byte ptr [00000000659C96B3h],0
                    00
  0000000065020B98: C6 05 59 8B 9A 00  mov         byte ptr [00000000659C96F8h],0
                    00
  0000000065020B9F: C6 05 53 8B 9A 00  mov         byte ptr [00000000659C96F9h],0
                    00
  0000000065020BA6: C6 05 4D 8B 9A 00  mov         byte ptr [00000000659C96FAh],0
                    00
  0000000065020BAD: C6 05 48 8B 9A 00  mov         byte ptr [00000000659C96FCh],0
                    00
  0000000065020BB4: E8 97 F3 59 00     call        00000000655BFF50
  0000000065020BB9: E8 42 38 03 00     call        0000000065054400
  0000000065020BBE: 90                 nop
  0000000065020BBF: 49 8B CC           mov         rcx,r12
  0000000065020BC2: 48 8B 9C 24 80 00  mov         rbx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000065020BCA: 48 83 C4 40        add         rsp,40h
  0000000065020BCE: 41 5D              pop         r13
  0000000065020BD0: 41 5C              pop         r12
  0000000065020BD2: 5F                 pop         rdi
  0000000065020BD3: 5E                 pop         rsi
  0000000065020BD4: 5D                 pop         rbp
  0000000065020BD5: 48 FF 25 54 61 67  jmp         qword ptr [0000000065696D30h]
                    00

  Summary

      695000 .text

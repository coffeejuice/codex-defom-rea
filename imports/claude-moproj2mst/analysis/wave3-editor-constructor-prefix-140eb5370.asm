Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EB5370: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EB5375: 48 83 EC 78        sub         rsp,78h
  0000000140EB5379: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140EB5382: B9 DC 09 00 00     mov         ecx,9DCh
  0000000140EB5387: E8 30 97 5B 00     call        000000014146EABC
  0000000140EB538C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140EB5391: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140EB5397: 74 11              je          0000000140EB53AA
  0000000140EB5399: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140EB539E: E8 4D C1 41 FF     call        00000001402D14F0
  0000000140EB53A3: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140EB53A8: EB 09              jmp         0000000140EB53B3
  0000000140EB53AA: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000140EB53B3: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140EB53B8: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140EB53BD: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140EB53C2: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140EB53C7: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140EB53CC: F3 0F 10 80 30 03  movss       xmm0,dword ptr [rax+0000000000000330h]
                    00 00
  0000000140EB53D4: F3 0F 59 05 3C 67  mulss       xmm0,dword ptr [00000001421EBB18h]
                    33 01
  0000000140EB53DC: F3 0F 2C C0        cvttss2si   eax,xmm0
  0000000140EB53E0: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  0000000140EB53E4: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140EB53E9: F3 0F 10 80 34 03  movss       xmm0,dword ptr [rax+0000000000000334h]
                    00 00
  0000000140EB53F1: F3 0F 59 05 1F 67  mulss       xmm0,dword ptr [00000001421EBB18h]
                    33 01
  0000000140EB53F9: F3 0F 2C C0        cvttss2si   eax,xmm0
  0000000140EB53FD: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140EB5401: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140EB5406: F3 0F 10 80 38 03  movss       xmm0,dword ptr [rax+0000000000000338h]
                    00 00
  0000000140EB540E: F3 0F 59 05 02 67  mulss       xmm0,dword ptr [00000001421EBB18h]
                    33 01
  0000000140EB5416: F3 0F 2C C0        cvttss2si   eax,xmm0
  0000000140EB541A: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140EB541E: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140EB5426: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB542D: 33 D2              xor         edx,edx
  0000000140EB542F: 48 8B 89 00 07 00  mov         rcx,qword ptr [rcx+0000000000000700h]
                    00
  0000000140EB5436: FF 15 FC 50 F5 00  call        qword ptr [0000000141E0A538h]
  0000000140EB543C: 4C 8B 5C 24 28     mov         r11,qword ptr [rsp+28h]
  0000000140EB5441: F3 41 0F 10 83 40  movss       xmm0,dword ptr [r11+0000000000000340h]
                    03 00 00
  0000000140EB544A: F3 0F 59 05 C6 66  mulss       xmm0,dword ptr [00000001421EBB18h]
                    33 01
  0000000140EB5452: F3 0F 2C C0        cvttss2si   eax,xmm0
  0000000140EB5456: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  0000000140EB545A: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140EB545F: F3 0F 10 80 44 03  movss       xmm0,dword ptr [rax+0000000000000344h]
                    00 00
  0000000140EB5467: F3 0F 59 05 A9 66  mulss       xmm0,dword ptr [00000001421EBB18h]
                    33 01
  0000000140EB546F: F3 0F 2C C0        cvttss2si   eax,xmm0
  0000000140EB5473: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140EB5477: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140EB547C: F3 0F 10 80 48 03  movss       xmm0,dword ptr [rax+0000000000000348h]
                    00 00
  0000000140EB5484: F3 0F 59 05 8C 66  mulss       xmm0,dword ptr [00000001421EBB18h]
                    33 01
  0000000140EB548C: F3 0F 2C C0        cvttss2si   eax,xmm0
  0000000140EB5490: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140EB5494: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140EB549C: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB54A3: 33 D2              xor         edx,edx
  0000000140EB54A5: 48 8B 89 98 07 00  mov         rcx,qword ptr [rcx+0000000000000798h]
                    00
  0000000140EB54AC: FF 15 86 50 F5 00  call        qword ptr [0000000141E0A538h]
  0000000140EB54B2: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140EB54B7: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140EB54BC: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140EB54C1: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140EB54C6: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140EB54CC: 74 16              je          0000000140EB54E4
  0000000140EB54CE: BA 01 00 00 00     mov         edx,1
  0000000140EB54D3: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140EB54D8: E8 F3 F1 07 00     call        0000000140F346D0
  0000000140EB54DD: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140EB54E2: EB 09              jmp         0000000140EB54ED
  0000000140EB54E4: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  0000000140EB54ED: 48 83 C4 78        add         rsp,78h
  0000000140EB54F1: C3                 ret

  Summary

     1E02000 .text

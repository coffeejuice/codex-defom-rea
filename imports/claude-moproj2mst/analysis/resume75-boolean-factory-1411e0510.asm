Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411E0510: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001411E0515: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411E051A: 48 81 EC A8 00 00  sub         rsp,0A8h
                    00
  00000001411E0521: 48 C7 44 24 78 FE  mov         qword ptr [rsp+78h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001411E052A: 48 C7 44 24 38 00  mov         qword ptr [rsp+38h],0
                    00 00 00
  00000001411E0533: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  00000001411E0538: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001411E0540: E8 5B E4 B6 FF     call        0000000140D4E9A0
  00000001411E0545: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001411E054D: E8 1E EB 41 FF     call        00000001405FF070
  00000001411E0552: 48 8B C8           mov         rcx,rax
  00000001411E0555: E8 56 ED 40 FF     call        00000001405EF2B0
  00000001411E055A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001411E055F: 48 83 7C 24 28 00  cmp         qword ptr [rsp+28h],0
  00000001411E0565: 75 07              jne         00000001411E056E
  00000001411E0567: 33 C0              xor         eax,eax
  00000001411E0569: E9 C0 01 00 00     jmp         00000001411E072E
  00000001411E056E: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000001411E0573: E8 F8 F1 E7 FE     call        000000014005F770
  00000001411E0578: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411E057D: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001411E0583: 75 07              jne         00000001411E058C
  00000001411E0585: 33 C0              xor         eax,eax
  00000001411E0587: E9 A2 01 00 00     jmp         00000001411E072E
  00000001411E058C: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001411E0594: E8 D7 E4 B6 FF     call        0000000140D4EA70
  00000001411E0599: 45 33 C0           xor         r8d,r8d
  00000001411E059C: 8B D0              mov         edx,eax
  00000001411E059E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001411E05A3: E8 48 AC A9 FF     call        0000000140C7B1F0
  00000001411E05A8: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001411E05B0: E8 0B 1F F6 FE     call        00000001401424C0
  00000001411E05B5: 48 8D 15 2C 21 AF  lea         rdx,[0000000142CD26E8h]
                    01
  00000001411E05BC: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001411E05C1: FF 15 49 3B C2 00  call        qword ptr [0000000141E04110h]
  00000001411E05C7: 48 8B D0           mov         rdx,rax
  00000001411E05CA: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001411E05CF: E8 8C 1E F6 FE     call        0000000140142460
  00000001411E05D4: 0F B6 C0           movzx       eax,al
  00000001411E05D7: 85 C0              test        eax,eax
  00000001411E05D9: 74 55              je          00000001411E0630
  00000001411E05DB: B9 58 00 00 00     mov         ecx,58h
  00000001411E05E0: E8 D7 E4 28 00     call        000000014146EABC
  00000001411E05E5: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001411E05EA: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  00000001411E05F0: 74 19              je          00000001411E060B
  00000001411E05F2: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001411E05F7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001411E05FC: E8 6F 33 00 00     call        00000001411E3970
  00000001411E0601: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001411E0609: EB 0C              jmp         00000001411E0617
  00000001411E060B: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00 00
  00000001411E0617: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001411E061F: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001411E0624: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001411E0629: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001411E062E: EB 79              jmp         00000001411E06A9
  00000001411E0630: 48 8D 15 BD 20 AF  lea         rdx,[0000000142CD26F4h]
                    01
  00000001411E0637: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001411E063C: FF 15 CE 3A C2 00  call        qword ptr [0000000141E04110h]
  00000001411E0642: 48 8B D0           mov         rdx,rax
  00000001411E0645: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001411E064A: E8 11 1E F6 FE     call        0000000140142460
  00000001411E064F: 0F B6 C0           movzx       eax,al
  00000001411E0652: 85 C0              test        eax,eax
  00000001411E0654: 74 53              je          00000001411E06A9
  00000001411E0656: B9 50 00 00 00     mov         ecx,50h
  00000001411E065B: E8 5C E4 28 00     call        000000014146EABC
  00000001411E0660: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001411E0665: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  00000001411E066B: 74 19              je          00000001411E0686
  00000001411E066D: 48 8B 54 24 28     mov         rdx,qword ptr [rsp+28h]
  00000001411E0672: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001411E0677: E8 04 1E 00 00     call        00000001411E2480
  00000001411E067C: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001411E0684: EB 0C              jmp         00000001411E0692
  00000001411E0686: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  00000001411E0692: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001411E069A: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001411E069F: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001411E06A4: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001411E06A9: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001411E06AF: 75 1A              jne         00000001411E06CB
  00000001411E06B1: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001411E06B9: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001411E06C1: E8 EA E9 41 FF     call        00000001405FF0B0
  00000001411E06C6: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001411E06CB: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001411E06D3: E8 08 02 42 FF     call        00000001406008E0
  00000001411E06D8: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001411E06DE: 74 49              je          00000001411E0729
  00000001411E06E0: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  00000001411E06E5: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001411E06ED: E8 DE E2 B6 FF     call        0000000140D4E9D0
  00000001411E06F2: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001411E06FA: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411E0702: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000001411E070A: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001411E0712: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001411E0717: FF 15 C3 B5 C2 00  call        qword ptr [0000000141E0BCE0h]
  00000001411E071D: 90                 nop
  00000001411E071E: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001411E0723: FF 15 B7 52 C2 00  call        qword ptr [0000000141E059E0h]
  00000001411E0729: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001411E072E: 48 81 C4 A8 00 00  add         rsp,0A8h
                    00
  00000001411E0735: C3                 ret

  Summary

     1E02000 .text

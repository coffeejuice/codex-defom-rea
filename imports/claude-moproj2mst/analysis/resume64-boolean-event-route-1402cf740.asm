Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001402CF740: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001402CF745: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001402CF74A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001402CF74F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001402CF754: 56                 push        rsi
  00000001402CF755: 57                 push        rdi
  00000001402CF756: 48 81 EC 98 01 00  sub         rsp,198h
                    00
  00000001402CF75D: 48 C7 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001402CF769: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001402CF771: 0F B6 80 41 01 00  movzx       eax,byte ptr [rax+0000000000000141h]
                    00
  00000001402CF778: 85 C0              test        eax,eax
  00000001402CF77A: 0F 85 43 03 00 00  jne         00000001402CFAC3
  00000001402CF780: 48 8B 94 24 B8 01  mov         rdx,qword ptr [rsp+00000000000001B8h]
                    00 00
  00000001402CF788: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001402CF790: FF 15 EA 4C B3 01  call        qword ptr [0000000141E04480h]
  00000001402CF796: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  00000001402CF79E: 48 8B 84 24 68 01  mov         rax,qword ptr [rsp+0000000000000168h]
                    00 00
  00000001402CF7A6: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  00000001402CF7AE: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001402CF7B6: 48 81 C1 88 00 00  add         rcx,88h
                    00
  00000001402CF7BD: 48 8B 94 24 70 01  mov         rdx,qword ptr [rsp+0000000000000170h]
                    00 00
  00000001402CF7C5: E8 26 0C 00 00     call        00000001402D03F0
  00000001402CF7CA: 48 8B D0           mov         rdx,rax
  00000001402CF7CD: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CF7D2: E8 99 F8 01 00     call        00000001402EF070
  00000001402CF7D7: 90                 nop
  00000001402CF7D8: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001402CF7E0: FF 15 FA 61 B3 01  call        qword ptr [0000000141E059E0h]
  00000001402CF7E6: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CF7EB: E8 F0 52 D3 FF     call        0000000140004AE0
  00000001402CF7F0: 89 44 24 7C        mov         dword ptr [rsp+7Ch],eax
  00000001402CF7F4: C7 44 24 78 00 00  mov         dword ptr [rsp+78h],0
                    00 00
  00000001402CF7FC: EB 0B              jmp         00000001402CF809
  00000001402CF7FE: 8B 44 24 78        mov         eax,dword ptr [rsp+78h]
  00000001402CF802: 83 C0 01           add         eax,1
  00000001402CF805: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  00000001402CF809: 8B 44 24 7C        mov         eax,dword ptr [rsp+7Ch]
  00000001402CF80D: 39 44 24 78        cmp         dword ptr [rsp+78h],eax
  00000001402CF811: 0F 8D A2 02 00 00  jge         00000001402CFAB9
  00000001402CF817: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  00000001402CF81B: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CF820: E8 EB F5 01 00     call        00000001402EEE10
  00000001402CF825: 48 8B 00           mov         rax,qword ptr [rax]
  00000001402CF828: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001402CF830: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001402CF838: E8 B3 B6 94 00     call        0000000140C1AEF0
  00000001402CF83D: 48 85 C0           test        rax,rax
  00000001402CF840: 0F 84 32 02 00 00  je          00000001402CFA78
  00000001402CF846: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001402CF84E: E8 9D B6 94 00     call        0000000140C1AEF0
  00000001402CF853: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  00000001402CF85B: 48 8B 84 24 78 01  mov         rax,qword ptr [rsp+0000000000000178h]
                    00 00
  00000001402CF863: 48 8B 00           mov         rax,qword ptr [rax]
  00000001402CF866: 48 8B 8C 24 78 01  mov         rcx,qword ptr [rsp+0000000000000178h]
                    00 00
  00000001402CF86E: FF 10              call        qword ptr [rax]
  00000001402CF870: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001402CF878: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  00000001402CF880: 48 8B B4 24 10 02  mov         rsi,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001402CF888: B9 10 00 00 00     mov         ecx,10h
  00000001402CF88D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF88F: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  00000001402CF897: 48 8B B4 24 08 02  mov         rsi,qword ptr [rsp+0000000000000208h]
                    00 00
  00000001402CF89F: B9 10 00 00 00     mov         ecx,10h
  00000001402CF8A4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF8A6: 48 8D BC 24 30 01  lea         rdi,[rsp+0000000000000130h]
                    00 00
  00000001402CF8AE: 48 8B B4 24 00 02  mov         rsi,qword ptr [rsp+0000000000000200h]
                    00 00
  00000001402CF8B6: B9 10 00 00 00     mov         ecx,10h
  00000001402CF8BB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF8BD: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  00000001402CF8C5: 48 8B B4 24 F8 01  mov         rsi,qword ptr [rsp+00000000000001F8h]
                    00 00
  00000001402CF8CD: B9 10 00 00 00     mov         ecx,10h
  00000001402CF8D2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF8D4: 48 8D BC 24 10 01  lea         rdi,[rsp+0000000000000110h]
                    00 00
  00000001402CF8DC: 48 8B B4 24 F0 01  mov         rsi,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001402CF8E4: B9 10 00 00 00     mov         ecx,10h
  00000001402CF8E9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF8EB: 48 8D BC 24 00 01  lea         rdi,[rsp+0000000000000100h]
                    00 00
  00000001402CF8F3: 48 8B B4 24 E8 01  mov         rsi,qword ptr [rsp+00000000000001E8h]
                    00 00
  00000001402CF8FB: B9 10 00 00 00     mov         ecx,10h
  00000001402CF900: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF902: 48 8D BC 24 F0 00  lea         rdi,[rsp+00000000000000F0h]
                    00 00
  00000001402CF90A: 48 8B B4 24 E0 01  mov         rsi,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001402CF912: B9 10 00 00 00     mov         ecx,10h
  00000001402CF917: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF919: 48 8D BC 24 E0 00  lea         rdi,[rsp+00000000000000E0h]
                    00 00
  00000001402CF921: 48 8B B4 24 D8 01  mov         rsi,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001402CF929: B9 10 00 00 00     mov         ecx,10h
  00000001402CF92E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF930: 48 8D BC 24 D0 00  lea         rdi,[rsp+00000000000000D0h]
                    00 00
  00000001402CF938: 48 8B B4 24 D0 01  mov         rsi,qword ptr [rsp+00000000000001D0h]
                    00 00
  00000001402CF940: B9 10 00 00 00     mov         ecx,10h
  00000001402CF945: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF947: 48 8D BC 24 C0 00  lea         rdi,[rsp+00000000000000C0h]
                    00 00
  00000001402CF94F: 48 8B B4 24 C8 01  mov         rsi,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000001402CF957: B9 10 00 00 00     mov         ecx,10h
  00000001402CF95C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF95E: 48 8D BC 24 B0 00  lea         rdi,[rsp+00000000000000B0h]
                    00 00
  00000001402CF966: 48 8B B4 24 C0 01  mov         rsi,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001402CF96E: B9 10 00 00 00     mov         ecx,10h
  00000001402CF973: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001402CF975: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001402CF97D: 48 83 C1 08        add         rcx,8
  00000001402CF981: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000001402CF989: FF 15 91 5E B3 01  call        qword ptr [0000000141E05820h]
  00000001402CF98F: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  00000001402CF997: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001402CF99F: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  00000001402CF9A7: 48 8B 8C 24 88 01  mov         rcx,qword ptr [rsp+0000000000000188h]
                    00 00
  00000001402CF9AF: FF 15 73 48 B3 01  call        qword ptr [0000000141E04228h]
  00000001402CF9B5: 48 8B F8           mov         rdi,rax
  00000001402CF9B8: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001402CF9C0: E8 2B B5 94 00     call        0000000140C1AEF0
  00000001402CF9C5: 48 8B C8           mov         rcx,rax
  00000001402CF9C8: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  00000001402CF9D0: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001402CF9D5: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  00000001402CF9DD: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001402CF9E2: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  00000001402CF9EA: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001402CF9EF: 48 8D 84 24 20 01  lea         rax,[rsp+0000000000000120h]
                    00 00
  00000001402CF9F7: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001402CF9FC: 48 8D 84 24 10 01  lea         rax,[rsp+0000000000000110h]
                    00 00
  00000001402CFA04: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001402CFA09: 48 8D 84 24 00 01  lea         rax,[rsp+0000000000000100h]
                    00 00
  00000001402CFA11: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001402CFA16: 48 8D 84 24 F0 00  lea         rax,[rsp+00000000000000F0h]
                    00 00
  00000001402CFA1E: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001402CFA23: 48 8D 84 24 E0 00  lea         rax,[rsp+00000000000000E0h]
                    00 00
  00000001402CFA2B: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001402CFA30: 48 8D 84 24 D0 00  lea         rax,[rsp+00000000000000D0h]
                    00 00
  00000001402CFA38: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001402CFA3D: 48 8D 84 24 C0 00  lea         rax,[rsp+00000000000000C0h]
                    00 00
  00000001402CFA45: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001402CFA4A: 4C 8D 8C 24 B0 00  lea         r9,[rsp+00000000000000B0h]
                    00 00
  00000001402CFA52: 41 B8 01 00 00 00  mov         r8d,1
  00000001402CFA58: 48 8B D7           mov         rdx,rdi
  00000001402CFA5B: FF 15 E7 58 B3 01  call        qword ptr [0000000141E05348h]
  00000001402CFA61: 88 84 24 8C 00 00  mov         byte ptr [rsp+000000000000008Ch],al
                    00
  00000001402CFA68: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001402CFA70: FF 15 1A 48 B3 01  call        qword ptr [0000000141E04290h]
  00000001402CFA76: EB 3C              jmp         00000001402CFAB4
  00000001402CFA78: 8B 54 24 78        mov         edx,dword ptr [rsp+78h]
  00000001402CFA7C: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CFA81: E8 6A 8E 54 00     call        00000001408188F0
  00000001402CFA86: 8B 44 24 7C        mov         eax,dword ptr [rsp+7Ch]
  00000001402CFA8A: 83 E8 01           sub         eax,1
  00000001402CFA8D: 89 44 24 7C        mov         dword ptr [rsp+7Ch],eax
  00000001402CFA91: 8B 44 24 78        mov         eax,dword ptr [rsp+78h]
  00000001402CFA95: 83 E8 01           sub         eax,1
  00000001402CFA98: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  00000001402CFA9C: 83 7C 24 78 FF     cmp         dword ptr [rsp+78h],0FFFFFFFFh
  00000001402CFAA1: 7D 08              jge         00000001402CFAAB
  00000001402CFAA3: C7 44 24 78 FF FF  mov         dword ptr [rsp+78h],0FFFFFFFFh
                    FF FF
  00000001402CFAAB: 83 7C 24 7C 00     cmp         dword ptr [rsp+7Ch],0
  00000001402CFAB0: 7F 02              jg          00000001402CFAB4
  00000001402CFAB2: EB 05              jmp         00000001402CFAB9
  00000001402CFAB4: E9 45 FD FF FF     jmp         00000001402CF7FE
  00000001402CFAB9: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001402CFABE: E8 DD 86 F3 FF     call        00000001402081A0
  00000001402CFAC3: 48 81 C4 98 01 00  add         rsp,198h
                    00
  00000001402CFACA: 5F                 pop         rdi
  00000001402CFACB: 5E                 pop         rsi
  00000001402CFACC: C3                 ret

  Summary

     1E02000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140048280: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140048284: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140048289: 56                 push        rsi
  000000014004828A: 57                 push        rdi
  000000014004828B: 48 81 EC F8 01 00  sub         rsp,1F8h
                    00
  0000000140048292: 48 C7 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014004829E: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001400482A6: 8B 84 24 18 02 00  mov         eax,dword ptr [rsp+0000000000000218h]
                    00
  00000001400482AD: 39 41 2C           cmp         dword ptr [rcx+2Ch],eax
  00000001400482B0: 75 07              jne         00000001400482B9
  00000001400482B2: B0 01              mov         al,1
  00000001400482B4: E9 69 03 00 00     jmp         0000000140048622
  00000001400482B9: 83 BC 24 18 02 00  cmp         dword ptr [rsp+0000000000000218h],0
                    00 00
  00000001400482C1: 0F 8E CF 02 00 00  jle         0000000140048596
  00000001400482C7: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001400482CF: E8 6C FD FF FF     call        0000000140048040
  00000001400482D4: 45 33 C0           xor         r8d,r8d
  00000001400482D7: 33 D2              xor         edx,edx
  00000001400482D9: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000001400482E1: FF 15 81 BE DB 01  call        qword ptr [0000000141E04168h]
  00000001400482E7: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  00000001400482EF: 48 8B F0           mov         rsi,rax
  00000001400482F2: B9 10 00 00 00     mov         ecx,10h
  00000001400482F7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400482F9: 45 33 C0           xor         r8d,r8d
  00000001400482FC: 33 D2              xor         edx,edx
  00000001400482FE: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140048306: FF 15 5C BE DB 01  call        qword ptr [0000000141E04168h]
  000000014004830C: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  0000000140048314: 48 8B F0           mov         rsi,rax
  0000000140048317: B9 10 00 00 00     mov         ecx,10h
  000000014004831C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014004831E: 45 33 C0           xor         r8d,r8d
  0000000140048321: 33 D2              xor         edx,edx
  0000000140048323: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014004832B: FF 15 37 BE DB 01  call        qword ptr [0000000141E04168h]
  0000000140048331: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  0000000140048339: 48 8B F0           mov         rsi,rax
  000000014004833C: B9 10 00 00 00     mov         ecx,10h
  0000000140048341: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140048343: 45 33 C0           xor         r8d,r8d
  0000000140048346: 33 D2              xor         edx,edx
  0000000140048348: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140048350: FF 15 12 BE DB 01  call        qword ptr [0000000141E04168h]
  0000000140048356: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  000000014004835E: 48 8B F0           mov         rsi,rax
  0000000140048361: B9 10 00 00 00     mov         ecx,10h
  0000000140048366: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140048368: 45 33 C0           xor         r8d,r8d
  000000014004836B: 33 D2              xor         edx,edx
  000000014004836D: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140048375: FF 15 ED BD DB 01  call        qword ptr [0000000141E04168h]
  000000014004837B: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140048383: 48 8B F0           mov         rsi,rax
  0000000140048386: B9 10 00 00 00     mov         ecx,10h
  000000014004838B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014004838D: 45 33 C0           xor         r8d,r8d
  0000000140048390: 33 D2              xor         edx,edx
  0000000140048392: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  000000014004839A: FF 15 C8 BD DB 01  call        qword ptr [0000000141E04168h]
  00000001400483A0: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  00000001400483A8: 48 8B F0           mov         rsi,rax
  00000001400483AB: B9 10 00 00 00     mov         ecx,10h
  00000001400483B0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400483B2: 45 33 C0           xor         r8d,r8d
  00000001400483B5: 33 D2              xor         edx,edx
  00000001400483B7: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001400483BF: FF 15 A3 BD DB 01  call        qword ptr [0000000141E04168h]
  00000001400483C5: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  00000001400483CD: 48 8B F0           mov         rsi,rax
  00000001400483D0: B9 10 00 00 00     mov         ecx,10h
  00000001400483D5: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400483D7: 45 33 C0           xor         r8d,r8d
  00000001400483DA: 33 D2              xor         edx,edx
  00000001400483DC: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  00000001400483E4: FF 15 7E BD DB 01  call        qword ptr [0000000141E04168h]
  00000001400483EA: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  00000001400483F2: 48 8B F0           mov         rsi,rax
  00000001400483F5: B9 10 00 00 00     mov         ecx,10h
  00000001400483FA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400483FC: 45 33 C0           xor         r8d,r8d
  00000001400483FF: 33 D2              xor         edx,edx
  0000000140048401: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  0000000140048409: FF 15 59 BD DB 01  call        qword ptr [0000000141E04168h]
  000000014004840F: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  0000000140048417: 48 8B F0           mov         rsi,rax
  000000014004841A: B9 10 00 00 00     mov         ecx,10h
  000000014004841F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140048421: 45 33 C0           xor         r8d,r8d
  0000000140048424: 33 D2              xor         edx,edx
  0000000140048426: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  000000014004842E: FF 15 34 BD DB 01  call        qword ptr [0000000141E04168h]
  0000000140048434: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  000000014004843C: 48 8B F0           mov         rsi,rax
  000000014004843F: B9 10 00 00 00     mov         ecx,10h
  0000000140048444: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140048446: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  000000014004844E: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140048453: 48 8D 84 24 C0 01  lea         rax,[rsp+00000000000001C0h]
                    00 00
  000000014004845B: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140048460: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  0000000140048468: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014004846D: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140048475: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014004847A: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140048482: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140048487: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  000000014004848F: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140048494: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  000000014004849C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400484A1: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  00000001400484A9: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400484AE: 4C 8D 8C 24 50 01  lea         r9,[rsp+0000000000000150h]
                    00 00
  00000001400484B6: 4C 8D 84 24 40 01  lea         r8,[rsp+0000000000000140h]
                    00 00
  00000001400484BE: 48 8D 15 CB 51 0D  lea         rdx,[000000014211D690h]
                    02
  00000001400484C5: 48 8D 0D 34 E2 D2  lea         rcx,[0000000143D76700h]
                    03
  00000001400484CC: E8 EF 6E 28 00     call        00000001402CF3C0
  00000001400484D1: 8B 94 24 18 02 00  mov         edx,dword ptr [rsp+0000000000000218h]
                    00
  00000001400484D8: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001400484E0: E8 0B F6 FF FF     call        0000000140047AF0
  00000001400484E5: 8B 94 24 18 02 00  mov         edx,dword ptr [rsp+0000000000000218h]
                    00
  00000001400484EC: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001400484F4: E8 87 FA FF FF     call        0000000140047F80
  00000001400484F9: 88 44 24 60        mov         byte ptr [rsp+60h],al
  00000001400484FD: 0F B6 44 24 60     movzx       eax,byte ptr [rsp+60h]
  0000000140048502: 85 C0              test        eax,eax
  0000000140048504: 74 5C              je          0000000140048562
  0000000140048506: 48 8B 84 24 10 02  mov         rax,qword ptr [rsp+0000000000000210h]
                    00 00
  000000014004850E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140048511: 4C 8D 84 24 18 02  lea         r8,[rsp+0000000000000218h]
                    00 00
  0000000140048519: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000014004851E: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  0000000140048526: FF 90 E8 01 00 00  call        qword ptr [rax+00000000000001E8h]
  000000014004852C: 90                 nop
  000000014004852D: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140048532: FF 15 50 D4 DB 01  call        qword ptr [0000000141E05988h]
  0000000140048538: 89 84 24 28 01 00  mov         dword ptr [rsp+0000000000000128h],eax
                    00
  000000014004853F: 48 8D 94 24 28 01  lea         rdx,[rsp+0000000000000128h]
                    00 00
  0000000140048547: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  000000014004854F: E8 3C 09 D0 00     call        0000000140D48E90
  0000000140048554: 90                 nop
  0000000140048555: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000014004855A: FF 15 80 B9 DB 01  call        qword ptr [0000000141E03EE0h]
  0000000140048560: EB 32              jmp         0000000140048594
  0000000140048562: BA FF FF FF FF     mov         edx,0FFFFFFFFh
  0000000140048567: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  000000014004856F: E8 7C F5 FF FF     call        0000000140047AF0
  0000000140048574: C7 84 24 2C 01 00  mov         dword ptr [rsp+000000000000012Ch],0FFFFFFFFh
                    00 FF FF FF FF
  000000014004857F: 48 8D 94 24 2C 01  lea         rdx,[rsp+000000000000012Ch]
                    00 00
  0000000140048587: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  000000014004858F: E8 FC 08 D0 00     call        0000000140D48E90
  0000000140048594: EB 5F              jmp         00000001400485F5
  0000000140048596: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  000000014004859E: E8 4D FB FF FF     call        00000001400480F0
  00000001400485A3: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001400485AB: E8 50 F4 FF FF     call        0000000140047A00
  00000001400485B0: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400485B8: 48 83 BC 24 80 00  cmp         qword ptr [rsp+0000000000000080h],0
                    00 00 00
  00000001400485C1: 75 32              jne         00000001400485F5
  00000001400485C3: BA FF FF FF FF     mov         edx,0FFFFFFFFh
  00000001400485C8: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001400485D0: E8 1B F5 FF FF     call        0000000140047AF0
  00000001400485D5: C7 84 24 30 01 00  mov         dword ptr [rsp+0000000000000130h],0FFFFFFFFh
                    00 FF FF FF FF
  00000001400485E0: 48 8D 94 24 30 01  lea         rdx,[rsp+0000000000000130h]
                    00 00
  00000001400485E8: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001400485F0: E8 9B 08 D0 00     call        0000000140D48E90
  00000001400485F5: 48 8B 84 24 10 02  mov         rax,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001400485FD: 83 78 2C 00        cmp         dword ptr [rax+2Ch],0
  0000000140048601: 7E 0D              jle         0000000140048610
  0000000140048603: C7 84 24 E8 01 00  mov         dword ptr [rsp+00000000000001E8h],1
                    00 01 00 00 00
  000000014004860E: EB 0B              jmp         000000014004861B
  0000000140048610: C7 84 24 E8 01 00  mov         dword ptr [rsp+00000000000001E8h],0
                    00 00 00 00 00
  000000014004861B: 8A 84 24 E8 01 00  mov         al,byte ptr [rsp+00000000000001E8h]
                    00
  0000000140048622: 48 81 C4 F8 01 00  add         rsp,1F8h
                    00
  0000000140048629: 5F                 pop         rdi
  000000014004862A: 5E                 pop         rsi
  000000014004862B: C3                 ret

  Summary

     1E02000 .text

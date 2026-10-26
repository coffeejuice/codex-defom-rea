Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067066840: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  0000000067066845: 48 89 6C 24 18     mov         qword ptr [rsp+18h],rbp
  000000006706684A: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  000000006706684F: 57                 push        rdi
  0000000067066850: 48 83 EC 30        sub         rsp,30h
  0000000067066854: 4C 8B 91 A0 00 00  mov         r10,qword ptr [rcx+00000000000000A0h]
                    00
  000000006706685B: 33 ED              xor         ebp,ebp
  000000006706685D: 48 8B D9           mov         rbx,rcx
  0000000067066860: 0F B6 4C 24 68     movzx       ecx,byte ptr [rsp+68h]
  0000000067066865: 66 89 6C 24 40     mov         word ptr [rsp+40h],bp
  000000006706686A: 41 8B F9           mov         edi,r9d
  000000006706686D: 45 8B D8           mov         r11d,r8d
  0000000067066870: 48 8B F2           mov         rsi,rdx
  0000000067066873: 41 38 6A 49        cmp         byte ptr [r10+49h],bpl
  0000000067066877: 74 4B              je          00000000670668C4
  0000000067066879: 84 C9              test        cl,cl
  000000006706687B: 75 47              jne         00000000670668C4
  000000006706687D: 49 8B 42 68        mov         rax,qword ptr [r10+68h]
  0000000067066881: 44 8B C7           mov         r8d,edi
  0000000067066884: 41 8B D3           mov         edx,r11d
  0000000067066887: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006706688A: 4C 8B 48 10        mov         r9,qword ptr [rax+10h]
  000000006706688E: 41 8B 42 4C        mov         eax,dword ptr [r10+4Ch]
  0000000067066892: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000067066896: 89 4C 24 20        mov         dword ptr [rsp+20h],ecx
  000000006706689A: 48 8B CE           mov         rcx,rsi
  000000006706689D: E8 5E 72 00 00     call        000000006706DB00
  00000000670668A2: 48 8B 8B A0 00 00  mov         rcx,qword ptr [rbx+00000000000000A0h]
                    00
  00000000670668A9: 89 43 08           mov         dword ptr [rbx+8],eax
  00000000670668AC: 48 8B 51 68        mov         rdx,qword ptr [rcx+68h]
  00000000670668B0: 83 F8 FF           cmp         eax,0FFFFFFFFh
  00000000670668B3: 8B 4A 08           mov         ecx,dword ptr [rdx+8]
  00000000670668B6: 0F 95 C0           setne       al
  00000000670668B9: 89 8B 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],ecx
  00000000670668BF: E9 92 00 00 00     jmp         0000000067066956
  00000000670668C4: 48 89 13           mov         qword ptr [rbx],rdx
  00000000670668C7: 48 85 D2           test        rdx,rdx
  00000000670668CA: 75 08              jne         00000000670668D4
  00000000670668CC: 48 8D 44 24 40     lea         rax,[rsp+40h]
  00000000670668D1: 48 89 03           mov         qword ptr [rbx],rax
  00000000670668D4: 8B 44 24 70        mov         eax,dword ptr [rsp+70h]
  00000000670668D8: 44 89 4B 08        mov         dword ptr [rbx+8],r9d
  00000000670668DC: 44 89 43 10        mov         dword ptr [rbx+10h],r8d
  00000000670668E0: 89 43 0C           mov         dword ptr [rbx+0Ch],eax
  00000000670668E3: 0F B6 44 24 60     movzx       eax,byte ptr [rsp+60h]
  00000000670668E8: 89 AB 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],ebp
  00000000670668EE: 88 43 14           mov         byte ptr [rbx+14h],al
  00000000670668F1: 89 AB 9C 00 00 00  mov         dword ptr [rbx+000000000000009Ch],ebp
  00000000670668F7: 41 38 6A 4A        cmp         byte ptr [r10+4Ah],bpl
  00000000670668FB: 0F 84 01 01 00 00  je          0000000067066A02
  0000000067066901: 45 85 C9           test        r9d,r9d
  0000000067066904: 0F 88 F8 00 00 00  js          0000000067066A02
  000000006706690A: 45 3B C8           cmp         r9d,r8d
  000000006706690D: 0F 8F EF 00 00 00  jg          0000000067066A02
  0000000067066913: 84 C9              test        cl,cl
  0000000067066915: 74 0A              je          0000000067066921
  0000000067066917: 48 8B CB           mov         rcx,rbx
  000000006706691A: E8 C1 DB FF FF     call        00000000670644E0
  000000006706691F: EB 35              jmp         0000000067066956
  0000000067066921: 45 2B 5A 70        sub         r11d,dword ptr [r10+70h]
  0000000067066925: 45 3B CB           cmp         r9d,r11d
  0000000067066928: 0F 8F D4 00 00 00  jg          0000000067066A02
  000000006706692E: 41 38 6A 48        cmp         byte ptr [r10+48h],bpl
  0000000067066932: 74 0A              je          000000006706693E
  0000000067066934: 48 8B CB           mov         rcx,rbx
  0000000067066937: E8 A4 DB FF FF     call        00000000670644E0
  000000006706693C: EB 18              jmp         0000000067066956
  000000006706693E: 48 8B D3           mov         rdx,rbx
  0000000067066941: 49 8B CA           mov         rcx,r10
  0000000067066944: 41 38 6A 58        cmp         byte ptr [r10+58h],bpl
  0000000067066948: 74 07              je          0000000067066951
  000000006706694A: E8 51 FA FF FF     call        00000000670663A0
  000000006706694F: EB 05              jmp         0000000067066956
  0000000067066951: E8 3A FB FF FF     call        0000000067066490
  0000000067066956: 84 C0              test        al,al
  0000000067066958: 0F 84 A4 00 00 00  je          0000000067066A02
  000000006706695E: 48 8B 93 80 00 00  mov         rdx,qword ptr [rbx+0000000000000080h]
                    00
  0000000067066965: 8B 43 08           mov         eax,dword ptr [rbx+8]
  0000000067066968: 89 02              mov         dword ptr [rdx],eax
  000000006706696A: 8B 83 98 00 00 00  mov         eax,dword ptr [rbx+0000000000000098h]
  0000000067066970: 48 83 C2 08        add         rdx,8
  0000000067066974: 89 42 FC           mov         dword ptr [rdx-4],eax
  0000000067066977: 8B 83 8C 00 00 00  mov         eax,dword ptr [rbx+000000000000008Ch]
  000000006706697D: 83 E8 02           sub         eax,2
  0000000067066980: D1 F8              sar         eax,1
  0000000067066982: 4C 63 D0           movsxd      r10,eax
  0000000067066985: 85 C0              test        eax,eax
  0000000067066987: 0F 8E 8F 00 00 00  jle         0000000067066A1C
  000000006706698D: 41 B9 10 00 00 00  mov         r9d,10h
  0000000067066993: 66 66 66 66 66 0F  nop         word ptr [rax+rax+0000000000000000h]
                    1F 84 00 00 00 00
                    00
  00000000670669A0: 48 8B 83 A0 00 00  mov         rax,qword ptr [rbx+00000000000000A0h]
                    00
  00000000670669A7: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  00000000670669AB: 48 8B 43 68        mov         rax,qword ptr [rbx+68h]
  00000000670669AF: 4D 63 1C 09        movsxd      r11,dword ptr [r9+rcx]
  00000000670669B3: 46 8B 04 98        mov         r8d,dword ptr [rax+r11*4]
  00000000670669B7: 41 81 F8 FF FF FF  cmp         r8d,7FFFFFFFh
                    7F
  00000000670669BE: 74 25              je          00000000670669E5
  00000000670669C0: 48 8B 43 70        mov         rax,qword ptr [rbx+70h]
  00000000670669C4: 42 8B 0C 98        mov         ecx,dword ptr [rax+r11*4]
  00000000670669C8: 41 2B C8           sub         ecx,r8d
  00000000670669CB: 85 C9              test        ecx,ecx
  00000000670669CD: 7E 0C              jle         00000000670669DB
  00000000670669CF: 44 03 43 08        add         r8d,dword ptr [rbx+8]
  00000000670669D3: 44 89 02           mov         dword ptr [rdx],r8d
  00000000670669D6: 89 4A 04           mov         dword ptr [rdx+4],ecx
  00000000670669D9: EB 17              jmp         00000000670669F2
  00000000670669DB: 89 2A              mov         dword ptr [rdx],ebp
  00000000670669DD: 44 8B C5           mov         r8d,ebp
  00000000670669E0: 89 4A 04           mov         dword ptr [rdx+4],ecx
  00000000670669E3: EB 0D              jmp         00000000670669F2
  00000000670669E5: C7 02 FF FF FF FF  mov         dword ptr [rdx],0FFFFFFFFh
  00000000670669EB: C7 42 04 FF FF FF  mov         dword ptr [rdx+4],0FFFFFFFFh
                    FF
  00000000670669F2: 48 83 C2 08        add         rdx,8
  00000000670669F6: 49 83 C1 04        add         r9,4
  00000000670669FA: 49 83 EA 01        sub         r10,1
  00000000670669FE: 75 A0              jne         00000000670669A0
  0000000067066A00: EB 1A              jmp         0000000067066A1C
  0000000067066A02: 4C 63 83 8C 00 00  movsxd      r8,dword ptr [rbx+000000000000008Ch]
                    00
  0000000067066A09: 48 8B 8B 80 00 00  mov         rcx,qword ptr [rbx+0000000000000080h]
                    00
  0000000067066A10: 83 CA FF           or          edx,0FFFFFFFFh
  0000000067066A13: 49 C1 E0 02        shl         r8,2
  0000000067066A17: E8 56 21 11 00     call        0000000067178B72
  0000000067066A1C: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000067066A21: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  0000000067066A26: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  0000000067066A2B: 48 83 C4 30        add         rsp,30h
  0000000067066A2F: 5F                 pop         rdi
  0000000067066A30: C3                 ret

  Summary

      1A7000 .text

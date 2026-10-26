Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtXml4_64.dll

File Type: DLL

  000000006100DF60: 48 8B C4           mov         rax,rsp
  000000006100DF63: 55                 push        rbp
  000000006100DF64: 56                 push        rsi
  000000006100DF65: 57                 push        rdi
  000000006100DF66: 41 54              push        r12
  000000006100DF68: 41 55              push        r13
  000000006100DF6A: 48 81 EC B0 00 00  sub         rsp,0B0h
                    00
  000000006100DF71: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006100DF7A: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006100DF7E: 4D 8B E1           mov         r12,r9
  000000006100DF81: 49 8B E8           mov         rbp,r8
  000000006100DF84: 4C 8B EA           mov         r13,rdx
  000000006100DF87: 48 8B F1           mov         rsi,rcx
  000000006100DF8A: C7 40 08 00 00 00  mov         dword ptr [rax+8],0
                    00
  000000006100DF91: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006100DF94: FF 50 48           call        qword ptr [rax+48h]
  000000006100DF97: B9 04 00 00 00     mov         ecx,4
  000000006100DF9C: E8 99 88 02 00     call        000000006103683A
  000000006100DFA1: 48 8B D8           mov         rbx,rax
  000000006100DFA4: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  000000006100DFAC: 48 85 C0           test        rax,rax
  000000006100DFAF: 74 0D              je          000000006100DFBE
  000000006100DFB1: 33 D2              xor         edx,edx
  000000006100DFB3: 48 8B C8           mov         rcx,rax
  000000006100DFB6: FF 15 C4 58 03 00  call        qword ptr [0000000061043880h]
  000000006100DFBC: EB 02              jmp         000000006100DFC0
  000000006100DFBE: 33 DB              xor         ebx,ebx
  000000006100DFC0: 48 3B 5E 68        cmp         rbx,qword ptr [rsi+68h]
  000000006100DFC4: 74 30              je          000000006100DFF6
  000000006100DFC6: 48 85 DB           test        rbx,rbx
  000000006100DFC9: 74 09              je          000000006100DFD4
  000000006100DFCB: 48 8B CB           mov         rcx,rbx
  000000006100DFCE: FF 15 5C 58 03 00  call        qword ptr [0000000061043830h]
  000000006100DFD4: 48 8B 7E 68        mov         rdi,qword ptr [rsi+68h]
  000000006100DFD8: 48 89 5E 68        mov         qword ptr [rsi+68h],rbx
  000000006100DFDC: 48 85 FF           test        rdi,rdi
  000000006100DFDF: 74 15              je          000000006100DFF6
  000000006100DFE1: 48 8B CF           mov         rcx,rdi
  000000006100DFE4: FF 15 3E 58 03 00  call        qword ptr [0000000061043828h]
  000000006100DFEA: 84 C0              test        al,al
  000000006100DFEC: 75 08              jne         000000006100DFF6
  000000006100DFEE: 48 8B CF           mov         rcx,rdi
  000000006100DFF1: E8 80 87 02 00     call        0000000061036776
  000000006100DFF6: B9 90 00 00 00     mov         ecx,90h
  000000006100DFFB: E8 3A 88 02 00     call        000000006103683A
  000000006100E000: 48 8B F8           mov         rdi,rax
  000000006100E003: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  000000006100E00B: 48 85 C0           test        rax,rax
  000000006100E00E: 74 4B              je          000000006100E05B
  000000006100E010: 4C 8B C6           mov         r8,rsi
  000000006100E013: 48 8B D6           mov         rdx,rsi
  000000006100E016: 48 8B C8           mov         rcx,rax
  000000006100E019: E8 A2 43 FF FF     call        00000000610023C0
  000000006100E01E: 90                 nop
  000000006100E01F: 48 8D 05 B2 80 03  lea         rax,[00000000610460D8h]
                    00
  000000006100E026: 48 89 07           mov         qword ptr [rdi],rax
  000000006100E029: 48 8D 4F 78        lea         rcx,[rdi+78h]
  000000006100E02D: FF 15 9D 58 03 00  call        qword ptr [00000000610438D0h]
  000000006100E033: 90                 nop
  000000006100E034: 48 8D 8F 80 00 00  lea         rcx,[rdi+0000000000000080h]
                    00
  000000006100E03B: FF 15 8F 58 03 00  call        qword ptr [00000000610438D0h]
  000000006100E041: 90                 nop
  000000006100E042: 48 8D 8F 88 00 00  lea         rcx,[rdi+0000000000000088h]
                    00
  000000006100E049: FF 15 81 58 03 00  call        qword ptr [00000000610438D0h]
  000000006100E04F: 90                 nop
  000000006100E050: 48 8B CF           mov         rcx,rdi
  000000006100E053: E8 A8 EC FF FF     call        000000006100CD00
  000000006100E058: 90                 nop
  000000006100E059: EB 02              jmp         000000006100E05D
  000000006100E05B: 33 FF              xor         edi,edi
  000000006100E05D: 48 8B D7           mov         rdx,rdi
  000000006100E060: 48 8D 4E 70        lea         rcx,[rsi+70h]
  000000006100E064: E8 17 5B FF FF     call        0000000061003B80
  000000006100E069: 48 8B 4E 70        mov         rcx,qword ptr [rsi+70h]
  000000006100E06D: 48 83 C1 08        add         rcx,8
  000000006100E071: FF 15 B1 57 03 00  call        qword ptr [0000000061043828h]
  000000006100E077: 48 8D 15 EA 60 03  lea         rdx,[0000000061044168h]
                    00
  000000006100E07E: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006100E083: FF 15 0F 58 03 00  call        qword ptr [0000000061043898h]
  000000006100E089: 48 8B D0           mov         rdx,rax
  000000006100E08C: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006100E091: FF 15 29 57 03 00  call        qword ptr [00000000610437C0h]
  000000006100E097: 90                 nop
  000000006100E098: BB 01 00 00 00     mov         ebx,1
  000000006100E09D: 89 9C 24 E0 00 00  mov         dword ptr [rsp+00000000000000E0h],ebx
                    00
  000000006100E0A4: 48 8B 45 00        mov         rax,qword ptr [rbp]
  000000006100E0A8: 45 33 C0           xor         r8d,r8d
  000000006100E0AB: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000006100E0B0: 48 8B CD           mov         rcx,rbp
  000000006100E0B3: FF 50 08           call        qword ptr [rax+8]
  000000006100E0B6: 84 C0              test        al,al
  000000006100E0B8: 74 4E              je          000000006100E108
  000000006100E0BA: 48 8D 15 77 60 03  lea         rdx,[0000000061044138h]
                    00
  000000006100E0C1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006100E0C6: FF 15 CC 57 03 00  call        qword ptr [0000000061043898h]
  000000006100E0CC: 48 8B D0           mov         rdx,rax
  000000006100E0CF: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000006100E0D7: FF 15 E3 56 03 00  call        qword ptr [00000000610437C0h]
  000000006100E0DD: 90                 nop
  000000006100E0DE: BB 03 00 00 00     mov         ebx,3
  000000006100E0E3: 89 9C 24 E0 00 00  mov         dword ptr [rsp+00000000000000E0h],ebx
                    00
  000000006100E0EA: 48 8B 45 00        mov         rax,qword ptr [rbp]
  000000006100E0EE: 45 33 C0           xor         r8d,r8d
  000000006100E0F1: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  000000006100E0F9: 48 8B CD           mov         rcx,rbp
  000000006100E0FC: FF 50 08           call        qword ptr [rax+8]
  000000006100E0FF: 84 C0              test        al,al
  000000006100E101: 75 05              jne         000000006100E108
  000000006100E103: 40 B7 01           mov         dil,1
  000000006100E106: EB 03              jmp         000000006100E10B
  000000006100E108: 40 32 FF           xor         dil,dil
  000000006100E10B: F6 C3 02           test        bl,2
  000000006100E10E: 74 19              je          000000006100E129
  000000006100E110: 83 E3 FD           and         ebx,0FFFFFFFDh
  000000006100E113: 89 9C 24 E0 00 00  mov         dword ptr [rsp+00000000000000E0h],ebx
                    00
  000000006100E11A: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000006100E122: FF 15 B8 57 03 00  call        qword ptr [00000000610438E0h]
  000000006100E128: 90                 nop
  000000006100E129: F6 C3 01           test        bl,1
  000000006100E12C: 74 0B              je          000000006100E139
  000000006100E12E: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006100E133: FF 15 A7 57 03 00  call        qword ptr [00000000610438E0h]
  000000006100E139: 44 0F B6 C7        movzx       r8d,dil
  000000006100E13D: 48 8B D6           mov         rdx,rsi
  000000006100E140: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006100E145: E8 16 57 FF FF     call        0000000061003860
  000000006100E14A: 90                 nop
  000000006100E14B: 48 8B 45 00        mov         rax,qword ptr [rbp]
  000000006100E14F: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  000000006100E154: 48 8B CD           mov         rcx,rbp
  000000006100E157: FF 50 58           call        qword ptr [rax+58h]
  000000006100E15A: 4C 8B 5D 00        mov         r11,qword ptr [rbp]
  000000006100E15E: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006100E163: 48 8B CD           mov         rcx,rbp
  000000006100E166: 41 FF 53 68        call        qword ptr [r11+68h]
  000000006100E16A: 4C 8B 5D 00        mov         r11,qword ptr [rbp]
  000000006100E16E: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  000000006100E173: 48 8B CD           mov         rcx,rbp
  000000006100E176: 41 FF 53 78        call        qword ptr [r11+78h]
  000000006100E17A: 4C 8B 5D 00        mov         r11,qword ptr [rbp]
  000000006100E17E: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  000000006100E183: 48 8B CD           mov         rcx,rbp
  000000006100E186: 41 FF 93 88 00 00  call        qword ptr [r11+0000000000000088h]
                    00
  000000006100E18D: 4C 8B 5D 00        mov         r11,qword ptr [rbp]
  000000006100E191: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006100E196: 48 8B CD           mov         rcx,rbp
  000000006100E199: 41 FF 53 48        call        qword ptr [r11+48h]
  000000006100E19D: 4C 8B 5D 00        mov         r11,qword ptr [rbp]
  000000006100E1A1: 49 8B D5           mov         rdx,r13
  000000006100E1A4: 48 8B CD           mov         rcx,rbp
  000000006100E1A7: 41 FF 93 98 00 00  call        qword ptr [r11+0000000000000098h]
                    00
  000000006100E1AE: 84 C0              test        al,al
  000000006100E1B0: 75 4D              jne         000000006100E1FF
  000000006100E1B2: 4D 85 E4           test        r12,r12
  000000006100E1B5: 74 0E              je          000000006100E1C5
  000000006100E1B7: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  000000006100E1BC: 49 8B CC           mov         rcx,r12
  000000006100E1BF: FF 15 EB 56 03 00  call        qword ptr [00000000610438B0h]
  000000006100E1C5: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  000000006100E1CD: 48 85 C9           test        rcx,rcx
  000000006100E1D0: 74 09              je          000000006100E1DB
  000000006100E1D2: 8B 84 24 80 00 00  mov         eax,dword ptr [rsp+0000000000000080h]
                    00
  000000006100E1D9: 89 01              mov         dword ptr [rcx],eax
  000000006100E1DB: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  000000006100E1E3: 48 85 C9           test        rcx,rcx
  000000006100E1E6: 74 09              je          000000006100E1F1
  000000006100E1E8: 8B 84 24 84 00 00  mov         eax,dword ptr [rsp+0000000000000084h]
                    00
  000000006100E1EF: 89 01              mov         dword ptr [rcx],eax
  000000006100E1F1: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006100E1F6: E8 25 57 FF FF     call        0000000061003920
  000000006100E1FB: 32 C0              xor         al,al
  000000006100E1FD: EB 0C              jmp         000000006100E20B
  000000006100E1FF: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  000000006100E204: E8 17 57 FF FF     call        0000000061003920
  000000006100E209: B0 01              mov         al,1
  000000006100E20B: 48 8B 9C 24 E8 00  mov         rbx,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000006100E213: 48 81 C4 B0 00 00  add         rsp,0B0h
                    00
  000000006100E21A: 41 5D              pop         r13
  000000006100E21C: 41 5C              pop         r12
  000000006100E21E: 5F                 pop         rdi
  000000006100E21F: 5E                 pop         rsi
  000000006100E220: 5D                 pop         rbp
  000000006100E221: C3                 ret

  Summary

       42000 .text

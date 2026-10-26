Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400B8FB0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400B8FB5: 48 81 EC 78 01 00  sub         rsp,178h
                    00
  00000001400B8FBC: 48 C7 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400B8FC8: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],10000h
                    01 00
  00000001400B8FD0: C7 44 24 3C 00 00  mov         dword ptr [rsp+3Ch],0
                    00 00
  00000001400B8FD8: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  00000001400B8FE1: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001400B8FE9: E8 82 CA FF FF     call        00000001400B5A70
  00000001400B8FEE: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400B8FF3: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001400B8FF9: 0F 84 5A 02 00 00  je          00000001400B9259
  00000001400B8FFF: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  00000001400B9004: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400B9009: E8 C2 BA FC FF     call        0000000140084AD0
  00000001400B900E: 90                 nop
  00000001400B900F: C7 44 24 48 00 00  mov         dword ptr [rsp+48h],0
                    00 00
  00000001400B9017: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400B901C: E8 BF BA F4 FF     call        0000000140004AE0
  00000001400B9021: 89 44 24 4C        mov         dword ptr [rsp+4Ch],eax
  00000001400B9025: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  00000001400B902A: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400B902F: E8 BC 4A FB FF     call        000000014006DAF0
  00000001400B9034: 90                 nop
  00000001400B9035: 48 8D 44 24 60     lea         rax,[rsp+60h]
  00000001400B903A: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400B9042: EB 48              jmp         00000001400B908C
  00000001400B9044: 33 C0              xor         eax,eax
  00000001400B9046: 83 F8 01           cmp         eax,1
  00000001400B9049: 74 0E              je          00000001400B9059
  00000001400B904B: 48 C7 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],0
                    00 00 00 00 00 00
  00000001400B9057: EB 12              jmp         00000001400B906B
  00000001400B9059: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400B905E: E8 ED BB 0C 00     call        0000000140184C50
  00000001400B9063: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  00000001400B906B: 48 8B 94 24 E8 00  mov         rdx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001400B9073: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400B907B: E8 00 33 D4 00     call        0000000140DFC380
  00000001400B9080: 48 8B C8           mov         rcx,rax
  00000001400B9083: 48 83 C1 10        add         rcx,10h
  00000001400B9087: E8 74 07 D3 00     call        0000000140DE9800
  00000001400B908C: 33 C0              xor         eax,eax
  00000001400B908E: 83 F8 01           cmp         eax,1
  00000001400B9091: 74 0E              je          00000001400B90A1
  00000001400B9093: 48 C7 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],0
                    00 00 00 00 00 00
  00000001400B909F: EB 12              jmp         00000001400B90B3
  00000001400B90A1: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400B90A6: E8 A5 BB 0C 00     call        0000000140184C50
  00000001400B90AB: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000001400B90B3: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400B90BB: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400B90C3: E8 B8 32 D4 00     call        0000000140DFC380
  00000001400B90C8: 48 8B C8           mov         rcx,rax
  00000001400B90CB: E8 B0 2F D4 00     call        0000000140DFC080
  00000001400B90D0: 0F B6 C0           movzx       eax,al
  00000001400B90D3: 85 C0              test        eax,eax
  00000001400B90D5: 0F 84 69 01 00 00  je          00000001400B9244
  00000001400B90DB: 33 C0              xor         eax,eax
  00000001400B90DD: 83 F8 01           cmp         eax,1
  00000001400B90E0: 74 0E              je          00000001400B90F0
  00000001400B90E2: 48 C7 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],0
                    00 00 00 00 00 00
  00000001400B90EE: EB 12              jmp         00000001400B9102
  00000001400B90F0: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400B90F5: E8 56 BB 0C 00     call        0000000140184C50
  00000001400B90FA: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  00000001400B9102: 48 8B 94 24 F8 00  mov         rdx,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001400B910A: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400B9112: E8 69 32 D4 00     call        0000000140DFC380
  00000001400B9117: 48 8B C8           mov         rcx,rax
  00000001400B911A: 48 83 C1 10        add         rcx,10h
  00000001400B911E: E8 DD 62 59 00     call        000000014064F400
  00000001400B9123: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400B9126: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400B912B: EB 5F              jmp         00000001400B918C
  00000001400B912D: 33 C0              xor         eax,eax
  00000001400B912F: 83 F8 01           cmp         eax,1
  00000001400B9132: 74 0E              je          00000001400B9142
  00000001400B9134: 48 C7 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],0
                    00 00 00 00 00 00
  00000001400B9140: EB 12              jmp         00000001400B9154
  00000001400B9142: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400B9147: E8 04 BB 0C 00     call        0000000140184C50
  00000001400B914C: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  00000001400B9154: 48 8B 94 24 00 01  mov         rdx,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001400B915C: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400B9164: E8 17 32 D4 00     call        0000000140DFC380
  00000001400B9169: 48 83 C0 08        add         rax,8
  00000001400B916D: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  00000001400B9175: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400B917D: 8B 08              mov         ecx,dword ptr [rax]
  00000001400B917F: 83 E9 01           sub         ecx,1
  00000001400B9182: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400B918A: 89 08              mov         dword ptr [rax],ecx
  00000001400B918C: 33 C0              xor         eax,eax
  00000001400B918E: 83 F8 01           cmp         eax,1
  00000001400B9191: 74 0E              je          00000001400B91A1
  00000001400B9193: 48 C7 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],0
                    00 00 00 00 00 00
  00000001400B919F: EB 12              jmp         00000001400B91B3
  00000001400B91A1: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400B91A6: E8 A5 BA 0C 00     call        0000000140184C50
  00000001400B91AB: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  00000001400B91B3: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001400B91BB: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400B91C3: E8 B8 31 D4 00     call        0000000140DFC380
  00000001400B91C8: 83 78 08 00        cmp         dword ptr [rax+8],0
  00000001400B91CC: 74 71              je          00000001400B923F
  00000001400B91CE: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  00000001400B91D4: 74 59              je          00000001400B922F
  00000001400B91D6: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400B91DB: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400B91DE: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400B91E3: FF 50 08           call        qword ptr [rax+8]
  00000001400B91E6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400B91EB: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001400B91F1: 74 3C              je          00000001400B922F
  00000001400B91F3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400B91F8: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400B91FB: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400B9200: FF 50 60           call        qword ptr [rax+60h]
  00000001400B9203: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  00000001400B920A: 8B 84 24 88 00 00  mov         eax,dword ptr [rsp+0000000000000088h]
                    00
  00000001400B9211: 83 E0 02           and         eax,2
  00000001400B9214: 85 C0              test        eax,eax
  00000001400B9216: 74 17              je          00000001400B922F
  00000001400B9218: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  00000001400B921C: 83 E8 01           sub         eax,1
  00000001400B921F: 39 44 24 48        cmp         dword ptr [rsp+48h],eax
  00000001400B9223: 74 0A              je          00000001400B922F
  00000001400B9225: C7 44 24 3C 01 00  mov         dword ptr [rsp+3Ch],1
                    00 00
  00000001400B922D: EB 10              jmp         00000001400B923F
  00000001400B922F: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  00000001400B9233: 83 C0 01           add         eax,1
  00000001400B9236: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  00000001400B923A: E9 EE FE FF FF     jmp         00000001400B912D
  00000001400B923F: E9 00 FE FF FF     jmp         00000001400B9044
  00000001400B9244: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400B9249: E8 D2 82 B9 00     call        0000000140C51520
  00000001400B924E: 90                 nop
  00000001400B924F: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400B9254: E8 47 EF 14 00     call        00000001402081A0
  00000001400B9259: 83 7C 24 3C 01     cmp         dword ptr [rsp+3Ch],1
  00000001400B925E: 0F 85 76 02 00 00  jne         00000001400B94DA
  00000001400B9264: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400B926C: FF 15 4E B2 D4 01  call        qword ptr [0000000141E044C0h]
  00000001400B9272: 90                 nop
  00000001400B9273: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001400B9279: 0F 84 4D 02 00 00  je          00000001400B94CC
  00000001400B927F: B2 20              mov         dl,20h
  00000001400B9281: 48 8D 8C 24 AA 00  lea         rcx,[rsp+00000000000000AAh]
                    00 00
  00000001400B9289: E8 D2 95 F4 FF     call        0000000140002860
  00000001400B928E: 0F B6 10           movzx       edx,byte ptr [rax]
  00000001400B9291: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  00000001400B9299: FF 15 B9 B0 D4 01  call        qword ptr [0000000141E04358h]
  00000001400B929F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400B92A4: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400B92A7: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000001400B92AF: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400B92B4: FF 50 40           call        qword ptr [rax+40h]
  00000001400B92B7: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  00000001400B92BF: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  00000001400B92C7: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  00000001400B92CF: 48 8D 15 5A 1A 09  lea         rdx,[000000014214AD30h]
                    02
  00000001400B92D6: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400B92DE: FF 15 9C B1 D4 01  call        qword ptr [0000000141E04480h]
  00000001400B92E4: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  00000001400B92EC: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001400B92F4: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  00000001400B92FC: 48 8D 84 24 A8 00  lea         rax,[rsp+00000000000000A8h]
                    00 00
  00000001400B9304: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400B9309: 45 33 C9           xor         r9d,r9d
  00000001400B930C: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  00000001400B9314: 48 8D 94 24 B8 00  lea         rdx,[rsp+00000000000000B8h]
                    00 00
  00000001400B931C: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001400B9324: FF 15 36 B0 D4 01  call        qword ptr [0000000141E04360h]
  00000001400B932A: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  00000001400B9332: 48 8B 84 24 38 01  mov         rax,qword ptr [rsp+0000000000000138h]
                    00 00
  00000001400B933A: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  00000001400B9342: 48 8B 94 24 40 01  mov         rdx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400B934A: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400B9352: FF 15 18 B1 D4 01  call        qword ptr [0000000141E04470h]
  00000001400B9358: 90                 nop
  00000001400B9359: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000001400B9361: FF 15 79 C6 D4 01  call        qword ptr [0000000141E059E0h]
  00000001400B9367: 90                 nop
  00000001400B9368: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000001400B9370: FF 15 6A C6 D4 01  call        qword ptr [0000000141E059E0h]
  00000001400B9376: 90                 nop
  00000001400B9377: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000001400B937F: FF 15 5B C6 D4 01  call        qword ptr [0000000141E059E0h]
  00000001400B9385: 48 8D 15 9C 19 09  lea         rdx,[000000014214AD28h]
                    02
  00000001400B938C: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001400B9394: FF 15 E6 B0 D4 01  call        qword ptr [0000000141E04480h]
  00000001400B939A: 90                 nop
  00000001400B939B: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  00000001400B93A3: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400B93AB: FF 15 6F AE D4 01  call        qword ptr [0000000141E04220h]
  00000001400B93B1: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  00000001400B93B9: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001400B93C1: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  00000001400B93C9: 48 8B 8C 24 50 01  mov         rcx,qword ptr [rsp+0000000000000150h]
                    00 00
  00000001400B93D1: FF 15 51 AE D4 01  call        qword ptr [0000000141E04228h]
  00000001400B93D7: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  00000001400B93DF: 45 33 C9           xor         r9d,r9d
  00000001400B93E2: 4C 8D 05 8F 19 09  lea         r8,[000000014214AD78h]
                    02
  00000001400B93E9: 48 8B D0           mov         rdx,rax
  00000001400B93EC: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001400B93F4: FF 15 36 B3 D4 01  call        qword ptr [0000000141E04730h]
  00000001400B93FA: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001400B9402: FF 15 D8 C5 D4 01  call        qword ptr [0000000141E059E0h]
  00000001400B9408: 90                 nop
  00000001400B9409: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000001400B9411: FF 15 79 AE D4 01  call        qword ptr [0000000141E04290h]
  00000001400B9417: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  00000001400B941F: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001400B9427: FF 15 F3 AD D4 01  call        qword ptr [0000000141E04220h]
  00000001400B942D: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  00000001400B9435: 48 8B 84 24 58 01  mov         rax,qword ptr [rsp+0000000000000158h]
                    00 00
  00000001400B943D: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  00000001400B9445: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001400B944D: FF 15 D5 AD D4 01  call        qword ptr [0000000141E04228h]
  00000001400B9453: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  00000001400B945B: 45 33 C9           xor         r9d,r9d
  00000001400B945E: 4C 8D 05 2B 19 09  lea         r8,[000000014214AD90h]
                    02
  00000001400B9465: 48 8B D0           mov         rdx,rax
  00000001400B9468: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  00000001400B9470: FF 15 BA B2 D4 01  call        qword ptr [0000000141E04730h]
  00000001400B9476: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  00000001400B947E: FF 15 5C C5 D4 01  call        qword ptr [0000000141E059E0h]
  00000001400B9484: 90                 nop
  00000001400B9485: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000001400B948D: FF 15 FD AD D4 01  call        qword ptr [0000000141E04290h]
  00000001400B9493: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],10000h
                    01 00
  00000001400B949B: 41 B9 00 40 00 00  mov         r9d,4000h
  00000001400B94A1: 4C 8D 84 24 90 00  lea         r8,[rsp+0000000000000090h]
                    00 00
  00000001400B94A9: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  00000001400B94B1: 33 C9              xor         ecx,ecx
  00000001400B94B3: FF 15 FF 22 D5 01  call        qword ptr [0000000141E0B7B8h]
  00000001400B94B9: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  00000001400B94BD: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001400B94C5: FF 15 15 C5 D4 01  call        qword ptr [0000000141E059E0h]
  00000001400B94CB: 90                 nop
  00000001400B94CC: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400B94D4: FF 15 06 C5 D4 01  call        qword ptr [0000000141E059E0h]
  00000001400B94DA: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001400B94DE: 48 81 C4 78 01 00  add         rsp,178h
                    00
  00000001400B94E5: C3                 ret

  Summary

     1E02000 .text

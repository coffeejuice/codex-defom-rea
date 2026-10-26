Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B71E0: 40 55              push        rbp
  00000000670B71E2: 56                 push        rsi
  00000000670B71E3: 57                 push        rdi
  00000000670B71E4: 41 54              push        r12
  00000000670B71E6: 41 55              push        r13
  00000000670B71E8: 41 56              push        r14
  00000000670B71EA: 41 57              push        r15
  00000000670B71EC: B8 00 11 00 00     mov         eax,1100h
  00000000670B71F1: E8 7A 23 0C 00     call        0000000067179570
  00000000670B71F6: 48 2B E0           sub         rsp,rax
  00000000670B71F9: 48 C7 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000000670B7205: 48 89 9C 24 50 11  mov         qword ptr [rsp+0000000000001150h],rbx
                    00 00
  00000000670B720D: 48 8B 05 8C C9 23  mov         rax,qword ptr [00000000672F3BA0h]
                    00
  00000000670B7214: 48 33 C4           xor         rax,rsp
  00000000670B7217: 48 89 84 24 F0 10  mov         qword ptr [rsp+00000000000010F0h],rax
                    00 00
  00000000670B721F: 4C 8B F2           mov         r14,rdx
  00000000670B7222: 48 8B F9           mov         rdi,rcx
  00000000670B7225: 4C 8B 61 08        mov         r12,qword ptr [rcx+8]
  00000000670B7229: 49 8B 84 24 E8 00  mov         rax,qword ptr [r12+00000000000000E8h]
                    00 00
  00000000670B7231: 83 78 08 00        cmp         dword ptr [rax+8],0
  00000000670B7235: 75 13              jne         00000000670B724A
  00000000670B7237: 48 8D 0D 5A 35 17  lea         rcx,[000000006722A798h]
                    00
  00000000670B723E: E8 9D 5C F6 FF     call        000000006701CEE0
  00000000670B7243: 32 C0              xor         al,al
  00000000670B7245: E9 FD 05 00 00     jmp         00000000670B7847
  00000000670B724A: B9 30 01 00 00     mov         ecx,130h
  00000000670B724F: E8 32 18 0C 00     call        0000000067178A86
  00000000670B7254: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000000670B725C: 33 ED              xor         ebp,ebp
  00000000670B725E: 48 85 C0           test        rax,rax
  00000000670B7261: 74 0A              je          00000000670B726D
  00000000670B7263: 48 8B C8           mov         rcx,rax
  00000000670B7266: E8 C5 E8 FF FF     call        00000000670B5B30
  00000000670B726B: EB 03              jmp         00000000670B7270
  00000000670B726D: 48 8B C5           mov         rax,rbp
  00000000670B7270: 45 33 C0           xor         r8d,r8d
  00000000670B7273: 48 8B D0           mov         rdx,rax
  00000000670B7276: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000000670B727B: E8 00 2D 00 00     call        00000000670B9F80
  00000000670B7280: 90                 nop
  00000000670B7281: 4C 8D 2D 90 2A 17  lea         r13,[0000000067229D18h]
                    00
  00000000670B7288: 4C 89 6C 24 60     mov         qword ptr [rsp+60h],r13
  00000000670B728D: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000000670B7292: 48 81 C1 E8 00 00  add         rcx,0E8h
                    00
  00000000670B7299: 49 8B D6           mov         rdx,r14
  00000000670B729C: E8 2F 91 FB FF     call        00000000670703D0
  00000000670B72A1: 90                 nop
  00000000670B72A2: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000000670B72A7: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000000670B72AC: FF 90 D8 00 00 00  call        qword ptr [rax+00000000000000D8h]
  00000000670B72B2: C7 44 24 70 00 00  mov         dword ptr [rsp+70h],0FF00000h
                    F0 0F
  00000000670B72BA: 4C 8B 08           mov         r9,qword ptr [rax]
  00000000670B72BD: 4C 8D 44 24 70     lea         r8,[rsp+70h]
  00000000670B72C2: 48 8D 94 24 DC 00  lea         rdx,[rsp+00000000000000DCh]
                    00 00
  00000000670B72CA: 48 8B C8           mov         rcx,rax
  00000000670B72CD: 41 FF 91 90 00 00  call        qword ptr [r9+0000000000000090h]
                    00
  00000000670B72D4: 8B 08              mov         ecx,dword ptr [rax]
  00000000670B72D6: 81 E1 00 00 40 00  and         ecx,400000h
  00000000670B72DC: 0F 95 C3           setne       bl
  00000000670B72DF: 4C 89 6C 24 60     mov         qword ptr [rsp+60h],r13
  00000000670B72E4: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000000670B72E9: E8 42 EC FF FF     call        00000000670B5F30
  00000000670B72EE: 90                 nop
  00000000670B72EF: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000000670B72F4: E8 B7 2C 00 00     call        00000000670B9FB0
  00000000670B72F9: 84 DB              test        bl,bl
  00000000670B72FB: 74 57              je          00000000670B7354
  00000000670B72FD: 45 33 C0           xor         r8d,r8d
  00000000670B7300: 48 8D 15 E9 31 17  lea         rdx,[000000006722A4F0h]
                    00
  00000000670B7307: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  00000000670B730F: E8 4C 4E F6 FF     call        000000006701C160
  00000000670B7314: 90                 nop
  00000000670B7315: 41 C7 84 24 20 01  mov         dword ptr [r12+0000000000000120h],0Eh
                    00 00 0E 00 00 00
  00000000670B7321: 49 8D 8C 24 90 00  lea         rcx,[r12+0000000000000090h]
                    00 00
  00000000670B7329: 48 8B D0           mov         rdx,rax
  00000000670B732C: E8 9F 90 FB FF     call        00000000670703D0
  00000000670B7331: 90                 nop
  00000000670B7332: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000000670B733A: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B733E: 75 0D              jne         00000000670B734D
  00000000670B7340: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000000670B7348: E8 83 8D FB FF     call        00000000670700D0
  00000000670B734D: 32 C0              xor         al,al
  00000000670B734F: E9 F3 04 00 00     jmp         00000000670B7847
  00000000670B7354: 48 8B CF           mov         rcx,rdi
  00000000670B7357: E8 F4 D4 FF FF     call        00000000670B4850
  00000000670B735C: 4C 8B 1F           mov         r11,qword ptr [rdi]
  00000000670B735F: 48 8B CF           mov         rcx,rdi
  00000000670B7362: 41 FF 53 68        call        qword ptr [r11+68h]
  00000000670B7366: 4C 8B 5F 08        mov         r11,qword ptr [rdi+8]
  00000000670B736A: 41 83 BB 20 01 00  cmp         dword ptr [r11+0000000000000120h],0
                    00 00
  00000000670B7372: 0F 85 CD 04 00 00  jne         00000000670B7845
  00000000670B7378: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B737B: 48 8B CF           mov         rcx,rdi
  00000000670B737E: FF 90 D8 00 00 00  call        qword ptr [rax+00000000000000D8h]
  00000000670B7384: 4C 8B 00           mov         r8,qword ptr [rax]
  00000000670B7387: 49 8B D6           mov         rdx,r14
  00000000670B738A: 48 8B C8           mov         rcx,rax
  00000000670B738D: 41 FF 50 48        call        qword ptr [r8+48h]
  00000000670B7391: 84 C0              test        al,al
  00000000670B7393: 74 0F              je          00000000670B73A4
  00000000670B7395: 48 8B CF           mov         rcx,rdi
  00000000670B7398: E8 B3 D4 FF FF     call        00000000670B4850
  00000000670B739D: B0 01              mov         al,1
  00000000670B739F: E9 A3 04 00 00     jmp         00000000670B7847
  00000000670B73A4: 45 32 ED           xor         r13b,r13b
  00000000670B73A7: C7 44 24 58 01 00  mov         dword ptr [rsp+58h],1
                    00 00
  00000000670B73AF: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B73B2: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  00000000670B73B7: 48 8B CF           mov         rcx,rdi
  00000000670B73BA: FF 50 60           call        qword ptr [rax+60h]
  00000000670B73BD: 84 C0              test        al,al
  00000000670B73BF: 0F 85 9E 00 00 00  jne         00000000670B7463
  00000000670B73C5: 41 BF 20 00 00 00  mov         r15d,20h
  00000000670B73CB: 66 44 89 7C 24 36  mov         word ptr [rsp+36h],r15w
  00000000670B73D1: 45 33 C0           xor         r8d,r8d
  00000000670B73D4: 48 8D 15 9D 33 17  lea         rdx,[000000006722A778h]
                    00
  00000000670B73DB: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000000670B73E0: E8 7B 4D F6 FF     call        000000006701C160
  00000000670B73E5: 90                 nop
  00000000670B73E6: 48 8D 4C 24 36     lea         rcx,[rsp+36h]
  00000000670B73EB: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  00000000670B73F0: 45 33 C9           xor         r9d,r9d
  00000000670B73F3: 4D 8D 84 24 E8 00  lea         r8,[r12+00000000000000E8h]
                    00 00
  00000000670B73FB: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  00000000670B7403: 48 8B C8           mov         rcx,rax
  00000000670B7406: E8 C5 A1 FB FF     call        00000000670715D0
  00000000670B740B: 90                 nop
  00000000670B740C: 41 C7 84 24 20 01  mov         dword ptr [r12+0000000000000120h],0Eh
                    00 00 0E 00 00 00
  00000000670B7418: 49 8D 8C 24 90 00  lea         rcx,[r12+0000000000000090h]
                    00 00
  00000000670B7420: 48 8B D0           mov         rdx,rax
  00000000670B7423: E8 A8 8F FB FF     call        00000000670703D0
  00000000670B7428: 90                 nop
  00000000670B7429: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670B7431: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670B7435: 75 0E              jne         00000000670B7445
  00000000670B7437: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670B743F: E8 8C 8C FB FF     call        00000000670700D0
  00000000670B7444: 90                 nop
  00000000670B7445: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000000670B744A: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B744E: 0F 85 F1 03 00 00  jne         00000000670B7845
  00000000670B7454: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000000670B7459: E8 72 8C FB FF     call        00000000670700D0
  00000000670B745E: E9 E2 03 00 00     jmp         00000000670B7845
  00000000670B7463: 83 CA FF           or          edx,0FFFFFFFFh
  00000000670B7466: 48 8D 0D F3 32 17  lea         rcx,[000000006722A760h]
                    00
  00000000670B746D: E8 CE 4A FB FF     call        000000006706BF40
  00000000670B7472: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000000670B7477: 41 BF 20 00 00 00  mov         r15d,20h
  00000000670B747D: 66 44 89 7C 24 34  mov         word ptr [rsp+34h],r15w
  00000000670B7483: 49 8B D6           mov         rdx,r14
  00000000670B7486: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000000670B748E: E8 6D 25 00 00     call        00000000670B9A00
  00000000670B7493: 90                 nop
  00000000670B7494: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  00000000670B749C: 48 8B C8           mov         rcx,rax
  00000000670B749F: E8 CC 05 00 00     call        00000000670B7A70
  00000000670B74A4: 90                 nop
  00000000670B74A5: 48 8D 4C 24 34     lea         rcx,[rsp+34h]
  00000000670B74AA: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  00000000670B74AF: 45 33 C9           xor         r9d,r9d
  00000000670B74B2: 4C 8B C0           mov         r8,rax
  00000000670B74B5: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  00000000670B74BD: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000670B74C2: E8 09 A1 FB FF     call        00000000670715D0
  00000000670B74C7: 90                 nop
  00000000670B74C8: 48 8B D0           mov         rdx,rax
  00000000670B74CB: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670B74D0: E8 5B 06 01 00     call        00000000670C7B30
  00000000670B74D5: 90                 nop
  00000000670B74D6: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  00000000670B74DE: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B74E2: 75 0E              jne         00000000670B74F2
  00000000670B74E4: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  00000000670B74EC: E8 DF 8B FB FF     call        00000000670700D0
  00000000670B74F1: 90                 nop
  00000000670B74F2: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  00000000670B74FA: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B74FE: 75 0E              jne         00000000670B750E
  00000000670B7500: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  00000000670B7508: E8 C3 8B FB FF     call        00000000670700D0
  00000000670B750D: 90                 nop
  00000000670B750E: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000000670B7516: E8 B5 26 00 00     call        00000000670B9BD0
  00000000670B751B: C7 44 24 5C 03 00  mov         dword ptr [rsp+5Ch],3
                    00 00
  00000000670B7523: 48 8D 54 24 5C     lea         rdx,[rsp+5Ch]
  00000000670B7528: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670B752D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670B7532: FF 50 60           call        qword ptr [rax+60h]
  00000000670B7535: 84 C0              test        al,al
  00000000670B7537: 0F 85 0A 01 00 00  jne         00000000670B7647
  00000000670B753D: 66 44 89 7C 24 32  mov         word ptr [rsp+32h],r15w
  00000000670B7543: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  00000000670B754B: E8 80 16 04 00     call        00000000670F8BD0
  00000000670B7550: 90                 nop
  00000000670B7551: 48 8D 4C 24 32     lea         rcx,[rsp+32h]
  00000000670B7556: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  00000000670B755B: 45 33 C9           xor         r9d,r9d
  00000000670B755E: 4C 8B C0           mov         r8,rax
  00000000670B7561: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  00000000670B7569: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000000670B756E: E8 5D A0 FB FF     call        00000000670715D0
  00000000670B7573: 90                 nop
  00000000670B7574: 48 8B D0           mov         rdx,rax
  00000000670B7577: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670B757C: E8 1F 08 01 00     call        00000000670C7DA0
  00000000670B7581: 90                 nop
  00000000670B7582: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000000670B758A: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B758E: 75 0E              jne         00000000670B759E
  00000000670B7590: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000000670B7598: E8 33 8B FB FF     call        00000000670700D0
  00000000670B759D: 90                 nop
  00000000670B759E: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000000670B75A6: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B75AA: 75 0D              jne         00000000670B75B9
  00000000670B75AC: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000000670B75B4: E8 17 8B FB FF     call        00000000670700D0
  00000000670B75B9: C7 44 24 38 03 00  mov         dword ptr [rsp+38h],3
                    00 00
  00000000670B75C1: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000000670B75C6: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670B75CB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670B75D0: FF 50 60           call        qword ptr [rax+60h]
  00000000670B75D3: 84 C0              test        al,al
  00000000670B75D5: 75 70              jne         00000000670B7647
  00000000670B75D7: 41 B5 01           mov         r13b,1
  00000000670B75DA: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670B75DF: E8 4C E9 FF FF     call        00000000670B5F30
  00000000670B75E4: 4C 8B 1F           mov         r11,qword ptr [rdi]
  00000000670B75E7: 48 8B CF           mov         rcx,rdi
  00000000670B75EA: 41 FF 53 68        call        qword ptr [r11+68h]
  00000000670B75EE: 45 33 C0           xor         r8d,r8d
  00000000670B75F1: 48 8D 15 50 31 17  lea         rdx,[000000006722A748h]
                    00
  00000000670B75F8: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000000670B7600: E8 5B 4B F6 FF     call        000000006701C160
  00000000670B7605: 90                 nop
  00000000670B7606: 41 C7 84 24 20 01  mov         dword ptr [r12+0000000000000120h],0Eh
                    00 00 0E 00 00 00
  00000000670B7612: 49 8D 8C 24 90 00  lea         rcx,[r12+0000000000000090h]
                    00 00
  00000000670B761A: 48 8B D0           mov         rdx,rax
  00000000670B761D: E8 AE 8D FB FF     call        00000000670703D0
  00000000670B7622: 90                 nop
  00000000670B7623: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000000670B762B: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B762F: 0F 85 BA 01 00 00  jne         00000000670B77EF
  00000000670B7635: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000000670B763D: E8 8E 8A FB FF     call        00000000670700D0
  00000000670B7642: E9 A8 01 00 00     jmp         00000000670B77EF
  00000000670B7647: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B764A: 48 8B CF           mov         rcx,rdi
  00000000670B764D: FF 90 88 00 00 00  call        qword ptr [rax+0000000000000088h]
  00000000670B7653: 84 C0              test        al,al
  00000000670B7655: 0F 85 B2 00 00 00  jne         00000000670B770D
  00000000670B765B: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  00000000670B7660: 41 B8 00 10 00 00  mov         r8d,1000h
  00000000670B7666: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  00000000670B766E: 48 8B CF           mov         rcx,rdi
  00000000670B7671: E8 FA 30 00 00     call        00000000670BA770
  00000000670B7676: 48 8B F0           mov         rsi,rax
  00000000670B7679: 48 85 C0           test        rax,rax
  00000000670B767C: 0F 8E 8B 00 00 00  jle         00000000670B770D
  00000000670B7682: 48 03 E8           add         rbp,rax
  00000000670B7685: 4C 8B C0           mov         r8,rax
  00000000670B7688: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  00000000670B7690: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670B7695: E8 B6 40 00 00     call        00000000670BB750
  00000000670B769A: 48 3B F0           cmp         rsi,rax
  00000000670B769D: 75 12              jne         00000000670B76B1
  00000000670B769F: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B76A2: 48 8B CF           mov         rcx,rdi
  00000000670B76A5: FF 90 88 00 00 00  call        qword ptr [rax+0000000000000088h]
  00000000670B76AB: 84 C0              test        al,al
  00000000670B76AD: 74 B1              je          00000000670B7660
  00000000670B76AF: EB 5C              jmp         00000000670B770D
  00000000670B76B1: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B76B4: 48 8B CF           mov         rcx,rdi
  00000000670B76B7: FF 50 68           call        qword ptr [rax+68h]
  00000000670B76BA: 45 33 C0           xor         r8d,r8d
  00000000670B76BD: 48 8D 15 6C 30 17  lea         rdx,[000000006722A730h]
                    00
  00000000670B76C4: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000000670B76CC: E8 8F 4A F6 FF     call        000000006701C160
  00000000670B76D1: 90                 nop
  00000000670B76D2: 41 C7 84 24 20 01  mov         dword ptr [r12+0000000000000120h],0Eh
                    00 00 0E 00 00 00
  00000000670B76DE: 49 8D 8C 24 90 00  lea         rcx,[r12+0000000000000090h]
                    00 00
  00000000670B76E6: 48 8B D0           mov         rdx,rax
  00000000670B76E9: E8 E2 8C FB FF     call        00000000670703D0
  00000000670B76EE: 90                 nop
  00000000670B76EF: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000000670B76F7: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B76FB: 75 0D              jne         00000000670B770A
  00000000670B76FD: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000000670B7705: E8 C6 89 FB FF     call        00000000670700D0
  00000000670B770A: 41 B5 01           mov         r13b,1
  00000000670B770D: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B7710: 48 8B CF           mov         rcx,rdi
  00000000670B7713: FF 50 78           call        qword ptr [rax+78h]
  00000000670B7716: 48 3B E8           cmp         rbp,rax
  00000000670B7719: 74 08              je          00000000670B7723
  00000000670B771B: 41 B5 01           mov         r13b,1
  00000000670B771E: E9 CC 00 00 00     jmp         00000000670B77EF
  00000000670B7723: 45 84 ED           test        r13b,r13b
  00000000670B7726: 0F 85 C3 00 00 00  jne         00000000670B77EF
  00000000670B772C: 49 8B D6           mov         rdx,r14
  00000000670B772F: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670B7734: E8 37 EB FF FF     call        00000000670B6270
  00000000670B7739: 84 C0              test        al,al
  00000000670B773B: 0F 85 A1 00 00 00  jne         00000000670B77E2
  00000000670B7741: 41 B5 01           mov         r13b,1
  00000000670B7744: 48 8B 07           mov         rax,qword ptr [rdi]
  00000000670B7747: 48 8B CF           mov         rcx,rdi
  00000000670B774A: FF 50 68           call        qword ptr [rax+68h]
  00000000670B774D: 66 44 89 7C 24 30  mov         word ptr [rsp+30h],r15w
  00000000670B7753: 45 33 C0           xor         r8d,r8d
  00000000670B7756: 48 8D 15 B3 2F 17  lea         rdx,[000000006722A710h]
                    00
  00000000670B775D: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000000670B7765: E8 F6 49 F6 FF     call        000000006701C160
  00000000670B776A: 90                 nop
  00000000670B776B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000000670B7770: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  00000000670B7775: 45 33 C9           xor         r9d,r9d
  00000000670B7778: 4D 8B C6           mov         r8,r14
  00000000670B777B: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  00000000670B7783: 48 8B C8           mov         rcx,rax
  00000000670B7786: E8 45 9E FB FF     call        00000000670715D0
  00000000670B778B: 90                 nop
  00000000670B778C: 41 C7 84 24 20 01  mov         dword ptr [r12+0000000000000120h],0Eh
                    00 00 0E 00 00 00
  00000000670B7798: 49 8D 8C 24 90 00  lea         rcx,[r12+0000000000000090h]
                    00 00
  00000000670B77A0: 48 8B D0           mov         rdx,rax
  00000000670B77A3: E8 28 8C FB FF     call        00000000670703D0
  00000000670B77A8: 90                 nop
  00000000670B77A9: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000000670B77B1: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B77B5: 75 0E              jne         00000000670B77C5
  00000000670B77B7: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000000670B77BF: E8 0C 89 FB FF     call        00000000670700D0
  00000000670B77C4: 90                 nop
  00000000670B77C5: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000000670B77CD: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B77D1: 75 1C              jne         00000000670B77EF
  00000000670B77D3: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000000670B77DB: E8 F0 88 FB FF     call        00000000670700D0
  00000000670B77E0: EB 0D              jmp         00000000670B77EF
  00000000670B77E2: 33 D2              xor         edx,edx
  00000000670B77E4: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670B77E9: E8 12 05 01 00     call        00000000670C7D00
  00000000670B77EE: 90                 nop
  00000000670B77EF: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670B77F4: E8 97 04 01 00     call        00000000670C7C90
  00000000670B77F9: 90                 nop
  00000000670B77FA: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000000670B77FF: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670B7803: 75 0A              jne         00000000670B780F
  00000000670B7805: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000670B780A: E8 C1 88 FB FF     call        00000000670700D0
  00000000670B780F: 45 84 ED           test        r13b,r13b
  00000000670B7812: 75 31              jne         00000000670B7845
  00000000670B7814: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  00000000670B781C: 48 8B CF           mov         rcx,rdi
  00000000670B781F: E8 4C CE FF FF     call        00000000670B4670
  00000000670B7824: 48 8B D0           mov         rdx,rax
  00000000670B7827: 49 8B CE           mov         rcx,r14
  00000000670B782A: E8 91 F3 FF FF     call        00000000670B6BC0
  00000000670B782F: 4C 8B 1F           mov         r11,qword ptr [rdi]
  00000000670B7832: 48 8B CF           mov         rcx,rdi
  00000000670B7835: 41 FF 53 68        call        qword ptr [r11+68h]
  00000000670B7839: 48 8B CF           mov         rcx,rdi
  00000000670B783C: E8 0F D0 FF FF     call        00000000670B4850
  00000000670B7841: B0 01              mov         al,1
  00000000670B7843: EB 02              jmp         00000000670B7847
  00000000670B7845: 32 C0              xor         al,al
  00000000670B7847: 48 8B 8C 24 F0 10  mov         rcx,qword ptr [rsp+00000000000010F0h]
                    00 00
  00000000670B784F: 48 33 CC           xor         rcx,rsp
  00000000670B7852: E8 B9 13 0C 00     call        0000000067178C10
  00000000670B7857: 48 8B 9C 24 50 11  mov         rbx,qword ptr [rsp+0000000000001150h]
                    00 00
  00000000670B785F: 48 81 C4 00 11 00  add         rsp,1100h
                    00
  00000000670B7866: 41 5F              pop         r15
  00000000670B7868: 41 5E              pop         r14
  00000000670B786A: 41 5D              pop         r13
  00000000670B786C: 41 5C              pop         r12
  00000000670B786E: 5F                 pop         rdi
  00000000670B786F: 5E                 pop         rsi
  00000000670B7870: 5D                 pop         rbp
  00000000670B7871: C3                 ret

  Summary

      1A7000 .text

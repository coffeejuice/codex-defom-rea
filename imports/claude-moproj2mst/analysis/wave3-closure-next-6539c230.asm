Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006539C230: 40 53              push        rbx
  000000006539C232: 55                 push        rbp
  000000006539C233: 56                 push        rsi
  000000006539C234: 57                 push        rdi
  000000006539C235: 41 54              push        r12
  000000006539C237: 41 55              push        r13
  000000006539C239: 41 56              push        r14
  000000006539C23B: 48 81 EC A0 00 00  sub         rsp,0A0h
                    00
  000000006539C242: 48 C7 44 24 70 FE  mov         qword ptr [rsp+70h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006539C24B: 4C 8B F2           mov         r14,rdx
  000000006539C24E: 4C 8B E9           mov         r13,rcx
  000000006539C251: 33 DB              xor         ebx,ebx
  000000006539C253: 89 9C 24 E0 00 00  mov         dword ptr [rsp+00000000000000E0h],ebx
                    00
  000000006539C25A: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  000000006539C25E: 48 8B CA           mov         rcx,rdx
  000000006539C261: FF 15 01 A8 2F 00  call        qword ptr [0000000065696A68h]
  000000006539C267: 83 F8 11           cmp         eax,11h
  000000006539C26A: 0F 85 17 03 00 00  jne         000000006539C587
  000000006539C270: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006539C277: 48 89 8C 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rcx
                    00 00
  000000006539C27F: FF 15 3B A6 2F 00  call        qword ptr [00000000656968C0h]
  000000006539C285: 4C 8B 9C 24 F0 00  mov         r11,qword ptr [rsp+00000000000000F0h]
                    00 00
  000000006539C28D: 41 F6 43 10 01     test        byte ptr [r11+10h],1
  000000006539C292: 75 12              jne         000000006539C2A6
  000000006539C294: 41 8B 53 04        mov         edx,dword ptr [r11+4]
  000000006539C298: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000006539C2A0: E8 7B 7C E0 FF     call        00000000651A3F20
  000000006539C2A5: 90                 nop
  000000006539C2A6: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000006539C2AE: FF 15 1C A6 2F 00  call        qword ptr [00000000656968D0h]
  000000006539C2B4: 84 C0              test        al,al
  000000006539C2B6: 74 05              je          000000006539C2BD
  000000006539C2B8: 48 8B EB           mov         rbp,rbx
  000000006539C2BB: EB 23              jmp         000000006539C2E0
  000000006539C2BD: 33 D2              xor         edx,edx
  000000006539C2BF: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000006539C2C7: FF 15 73 A6 2F 00  call        qword ptr [0000000065696940h]
  000000006539C2CD: 48 8B 10           mov         rdx,qword ptr [rax]
  000000006539C2D0: 48 8D 0D B9 CA 36  lea         rcx,[0000000065708D90h]
                    00
  000000006539C2D7: FF 15 2B A7 2F 00  call        qword ptr [0000000065696A08h]
  000000006539C2DD: 48 8B E8           mov         rbp,rax
  000000006539C2E0: 45 32 E4           xor         r12b,r12b
  000000006539C2E3: 48 8B F3           mov         rsi,rbx
  000000006539C2E6: 49 8B FD           mov         rdi,r13
  000000006539C2E9: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006539C2F0: 48 8B 57 20        mov         rdx,qword ptr [rdi+20h]
  000000006539C2F4: 0F B6 52 0C        movzx       edx,byte ptr [rdx+0Ch]
  000000006539C2F8: F6 C2 01           test        dl,1
  000000006539C2FB: 75 29              jne         000000006539C326
  000000006539C2FD: 48 8B CF           mov         rcx,rdi
  000000006539C300: FF 15 62 A5 2F 00  call        qword ptr [0000000065696868h]
  000000006539C306: 48 8B F8           mov         rdi,rax
  000000006539C309: 48 8B D0           mov         rdx,rax
  000000006539C30C: 48 8D 0D 95 EB 37  lea         rcx,[000000006571AEA8h]
                    00
  000000006539C313: FF 15 EF A6 2F 00  call        qword ptr [0000000065696A08h]
  000000006539C319: 48 8B F0           mov         rsi,rax
  000000006539C31C: 48 85 C0           test        rax,rax
  000000006539C31F: 75 05              jne         000000006539C326
  000000006539C321: 48 85 FF           test        rdi,rdi
  000000006539C324: 75 CA              jne         000000006539C2F0
  000000006539C326: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006539C32B: FF 15 37 A4 2F 00  call        qword ptr [0000000065696768h]
  000000006539C331: 90                 nop
  000000006539C332: 49 8B CD           mov         rcx,r13
  000000006539C335: 48 85 F6           test        rsi,rsi
  000000006539C338: 48 0F 45 CE        cmovne      rcx,rsi
  000000006539C33C: 4C 8B C0           mov         r8,rax
  000000006539C33F: 48 8D 94 24 F8 00  lea         rdx,[rsp+00000000000000F8h]
                    00 00
  000000006539C347: E8 C4 F9 FF FF     call        000000006539BD10
  000000006539C34C: 90                 nop
  000000006539C34D: 48 8B D0           mov         rdx,rax
  000000006539C350: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006539C355: E8 66 1E 1B 00     call        000000006554E1C0
  000000006539C35A: 90                 nop
  000000006539C35B: 48 8B 8C 24 F8 00  mov         rcx,qword ptr [rsp+00000000000000F8h]
                    00 00
  000000006539C363: FF 15 0F A6 2F 00  call        qword ptr [0000000065696978h]
  000000006539C369: 84 C0              test        al,al
  000000006539C36B: 75 0F              jne         000000006539C37C
  000000006539C36D: 48 8B 8C 24 F8 00  mov         rcx,qword ptr [rsp+00000000000000F8h]
                    00 00
  000000006539C375: FF 15 E5 A5 2F 00  call        qword ptr [0000000065696960h]
  000000006539C37B: 90                 nop
  000000006539C37C: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006539C381: FF 15 D9 A3 2F 00  call        qword ptr [0000000065696760h]
  000000006539C387: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  000000006539C390: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  000000006539C394: FF 44 24 28        inc         dword ptr [rsp+28h]
  000000006539C398: 85 C0              test        eax,eax
  000000006539C39A: 75 11              jne         000000006539C3AD
  000000006539C39C: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000006539C3A1: 48 39 44 24 30     cmp         qword ptr [rsp+30h],rax
  000000006539C3A6: 74 05              je          000000006539C3AD
  000000006539C3A8: 40 B7 01           mov         dil,1
  000000006539C3AB: EB 03              jmp         000000006539C3B0
  000000006539C3AD: 40 32 FF           xor         dil,dil
  000000006539C3B0: F6 C3 08           test        bl,8
  000000006539C3B3: 74 24              je          000000006539C3D9
  000000006539C3B5: 83 E3 F7           and         ebx,0FFFFFFF7h
  000000006539C3B8: 89 9C 24 E0 00 00  mov         dword ptr [rsp+00000000000000E0h],ebx
                    00
  000000006539C3BF: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006539C3C4: FF 15 AE A5 2F 00  call        qword ptr [0000000065696978h]
  000000006539C3CA: 84 C0              test        al,al
  000000006539C3CC: 75 0B              jne         000000006539C3D9
  000000006539C3CE: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006539C3D3: FF 15 87 A5 2F 00  call        qword ptr [0000000065696960h]
  000000006539C3D9: F6 C3 04           test        bl,4
  000000006539C3DC: 74 11              je          000000006539C3EF
  000000006539C3DE: 83 E3 FB           and         ebx,0FFFFFFFBh
  000000006539C3E1: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000006539C3E9: FF 15 71 A3 2F 00  call        qword ptr [0000000065696760h]
  000000006539C3EF: 40 84 FF           test        dil,dil
  000000006539C3F2: 0F 84 3E 01 00 00  je          000000006539C536
  000000006539C3F8: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000006539C3FD: 48 8B 30           mov         rsi,qword ptr [rax]
  000000006539C400: F6 C3 20           test        bl,20h
  000000006539C403: 74 24              je          000000006539C429
  000000006539C405: 83 E3 DF           and         ebx,0FFFFFFDFh
  000000006539C408: 89 9C 24 E0 00 00  mov         dword ptr [rsp+00000000000000E0h],ebx
                    00
  000000006539C40F: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000006539C414: FF 15 5E A5 2F 00  call        qword ptr [0000000065696978h]
  000000006539C41A: 84 C0              test        al,al
  000000006539C41C: 75 0B              jne         000000006539C429
  000000006539C41E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000006539C423: FF 15 37 A5 2F 00  call        qword ptr [0000000065696960h]
  000000006539C429: F6 C3 10           test        bl,10h
  000000006539C42C: 74 12              je          000000006539C440
  000000006539C42E: 83 E3 EF           and         ebx,0FFFFFFEFh
  000000006539C431: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000006539C439: FF 15 21 A3 2F 00  call        qword ptr [0000000065696760h]
  000000006539C43F: 90                 nop
  000000006539C440: 8B 7C 24 28        mov         edi,dword ptr [rsp+28h]
  000000006539C444: 0F BA E3 09        bt          ebx,9
  000000006539C448: 73 25              jae         000000006539C46F
  000000006539C44A: 0F BA F3 09        btr         ebx,9
  000000006539C44E: 89 9C 24 E0 00 00  mov         dword ptr [rsp+00000000000000E0h],ebx
                    00
  000000006539C455: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006539C45A: FF 15 18 A5 2F 00  call        qword ptr [0000000065696978h]
  000000006539C460: 84 C0              test        al,al
  000000006539C462: 75 0B              jne         000000006539C46F
  000000006539C464: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000006539C469: FF 15 F1 A4 2F 00  call        qword ptr [0000000065696960h]
  000000006539C46F: 0F BA E3 08        bt          ebx,8
  000000006539C473: 73 12              jae         000000006539C487
  000000006539C475: 0F BA F3 08        btr         ebx,8
  000000006539C479: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000006539C481: FF 15 D9 A2 2F 00  call        qword ptr [0000000065696760h]
  000000006539C487: 85 FF              test        edi,edi
  000000006539C489: 74 5D              je          000000006539C4E8
  000000006539C48B: 48 8B CE           mov         rcx,rsi
  000000006539C48E: E8 2D AF 05 00     call        00000000653F73C0
  000000006539C493: 84 C0              test        al,al
  000000006539C495: 74 05              je          000000006539C49C
  000000006539C497: 48 3B F5           cmp         rsi,rbp
  000000006539C49A: 75 49              jne         000000006539C4E5
  000000006539C49C: FF 4C 24 28        dec         dword ptr [rsp+28h]
  000000006539C4A0: 84 DB              test        bl,bl
  000000006539C4A2: 79 25              jns         000000006539C4C9
  000000006539C4A4: 0F BA F3 07        btr         ebx,7
  000000006539C4A8: 89 9C 24 E0 00 00  mov         dword ptr [rsp+00000000000000E0h],ebx
                    00
  000000006539C4AF: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006539C4B4: FF 15 BE A4 2F 00  call        qword ptr [0000000065696978h]
  000000006539C4BA: 84 C0              test        al,al
  000000006539C4BC: 75 0B              jne         000000006539C4C9
  000000006539C4BE: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006539C4C3: FF 15 97 A4 2F 00  call        qword ptr [0000000065696960h]
  000000006539C4C9: F6 C3 40           test        bl,40h
  000000006539C4CC: 0F 84 6E FF FF FF  je          000000006539C440
  000000006539C4D2: 83 E3 BF           and         ebx,0FFFFFFBFh
  000000006539C4D5: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000006539C4DA: FF 15 80 A2 2F 00  call        qword ptr [0000000065696760h]
  000000006539C4E0: E9 5B FF FF FF     jmp         000000006539C440
  000000006539C4E5: 41 B4 01           mov         r12b,1
  000000006539C4E8: 48 83 44 24 30 08  add         qword ptr [rsp+30h],8
  000000006539C4EE: F6 C3 02           test        bl,2
  000000006539C4F1: 74 24              je          000000006539C517
  000000006539C4F3: 83 E3 FD           and         ebx,0FFFFFFFDh
  000000006539C4F6: 89 9C 24 E0 00 00  mov         dword ptr [rsp+00000000000000E0h],ebx
                    00
  000000006539C4FD: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006539C502: FF 15 70 A4 2F 00  call        qword ptr [0000000065696978h]
  000000006539C508: 84 C0              test        al,al
  000000006539C50A: 75 0B              jne         000000006539C517
  000000006539C50C: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  000000006539C511: FF 15 49 A4 2F 00  call        qword ptr [0000000065696960h]
  000000006539C517: F6 C3 01           test        bl,1
  000000006539C51A: 0F 84 70 FE FF FF  je          000000006539C390
  000000006539C520: 83 E3 FE           and         ebx,0FFFFFFFEh
  000000006539C523: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000006539C52B: FF 15 2F A2 2F 00  call        qword ptr [0000000065696760h]
  000000006539C531: E9 5A FE FF FF     jmp         000000006539C390
  000000006539C536: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006539C53B: FF 15 37 A4 2F 00  call        qword ptr [0000000065696978h]
  000000006539C541: 84 C0              test        al,al
  000000006539C543: 75 0B              jne         000000006539C550
  000000006539C545: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  000000006539C54A: FF 15 10 A4 2F 00  call        qword ptr [0000000065696960h]
  000000006539C550: 45 84 E4           test        r12b,r12b
  000000006539C553: 75 10              jne         000000006539C565
  000000006539C555: 48 85 ED           test        rbp,rbp
  000000006539C558: 74 0B              je          000000006539C565
  000000006539C55A: B2 01              mov         dl,1
  000000006539C55C: 48 8B CD           mov         rcx,rbp
  000000006539C55F: E8 2C AF 05 00     call        00000000653F7490
  000000006539C564: 90                 nop
  000000006539C565: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  000000006539C56D: FF 15 05 A4 2F 00  call        qword ptr [0000000065696978h]
  000000006539C573: 84 C0              test        al,al
  000000006539C575: 75 26              jne         000000006539C59D
  000000006539C577: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  000000006539C57F: FF 15 DB A3 2F 00  call        qword ptr [0000000065696960h]
  000000006539C585: EB 16              jmp         000000006539C59D
  000000006539C587: 49 8B CE           mov         rcx,r14
  000000006539C58A: FF 15 D8 A4 2F 00  call        qword ptr [0000000065696A68h]
  000000006539C590: 83 F8 59           cmp         eax,59h
  000000006539C593: 75 08              jne         000000006539C59D
  000000006539C595: 48 8B CF           mov         rcx,rdi
  000000006539C598: E8 F3 F7 FF FF     call        000000006539BD90
  000000006539C59D: 49 8B D6           mov         rdx,r14
  000000006539C5A0: 49 8B CD           mov         rcx,r13
  000000006539C5A3: 48 81 C4 A0 00 00  add         rsp,0A0h
                    00
  000000006539C5AA: 41 5E              pop         r14
  000000006539C5AC: 41 5D              pop         r13
  000000006539C5AE: 41 5C              pop         r12
  000000006539C5B0: 5F                 pop         rdi
  000000006539C5B1: 5E                 pop         rsi
  000000006539C5B2: 5D                 pop         rbp
  000000006539C5B3: 5B                 pop         rbx
  000000006539C5B4: E9 97 03 CD FF     jmp         000000006506C950

  Summary

      695000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065459420: 40 55              push        rbp
  0000000065459422: 56                 push        rsi
  0000000065459423: 57                 push        rdi
  0000000065459424: 48 83 EC 40        sub         rsp,40h
  0000000065459428: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065459431: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  0000000065459436: 48 8B D9           mov         rbx,rcx
  0000000065459439: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  000000006545943D: 48 8D BE 38 02 00  lea         rdi,[rsi+0000000000000238h]
                    00
  0000000065459444: 48 83 3F 00        cmp         qword ptr [rdi],0
  0000000065459448: 74 15              je          000000006545945F
  000000006545944A: 48 8D 0D 17 1D 2C  lea         rcx,[000000006571B168h]
                    00
  0000000065459451: FF 15 41 D5 23 00  call        qword ptr [0000000065696998h]
  0000000065459457: 83 C8 FF           or          eax,0FFFFFFFFh
  000000006545945A: E9 2C 01 00 00     jmp         000000006545958B
  000000006545945F: BA 37 00 00 00     mov         edx,37h
  0000000065459464: E8 A7 4B C0 FF     call        000000006505E010
  0000000065459469: 0F B6 E8           movzx       ebp,al
  000000006545946C: 45 33 C0           xor         r8d,r8d
  000000006545946F: 41 8D 50 37        lea         edx,[r8+37h]
  0000000065459473: 48 8B CB           mov         rcx,rbx
  0000000065459476: E8 65 FB C0 FF     call        0000000065068FE0
  000000006545947B: 48 8B CE           mov         rcx,rsi
  000000006545947E: E8 9D FC FF FF     call        0000000065459120
  0000000065459483: BA 46 00 00 00     mov         edx,46h
  0000000065459488: 48 8B CB           mov         rcx,rbx
  000000006545948B: E8 80 4B C0 FF     call        000000006505E010
  0000000065459490: 0F B6 F0           movzx       esi,al
  0000000065459493: 41 B0 01           mov         r8b,1
  0000000065459496: BA 46 00 00 00     mov         edx,46h
  000000006545949B: 48 8B CB           mov         rcx,rbx
  000000006545949E: E8 3D FB C0 FF     call        0000000065068FE0
  00000000654594A3: 4C 8B 5B 08        mov         r11,qword ptr [rbx+8]
  00000000654594A7: 41 C7 83 2C 02 00  mov         dword ptr [r11+000000000000022Ch],0
                    00 00 00 00 00
  00000000654594B2: 4C 8B 03           mov         r8,qword ptr [rbx]
  00000000654594B5: B2 01              mov         dl,1
  00000000654594B7: 48 8B CB           mov         rcx,rbx
  00000000654594BA: 41 FF 50 58        call        qword ptr [r8+58h]
  00000000654594BE: 33 D2              xor         edx,edx
  00000000654594C0: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000654594C5: FF 15 95 F6 23 00  call        qword ptr [0000000065698B60h]
  00000000654594CB: 90                 nop
  00000000654594CC: 48 8D 44 24 28     lea         rax,[rsp+28h]
  00000000654594D1: 48 39 07           cmp         qword ptr [rdi],rax
  00000000654594D4: 74 0E              je          00000000654594E4
  00000000654594D6: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000654594DB: 48 8B CF           mov         rcx,rdi
  00000000654594DE: FF 15 04 D4 23 00  call        qword ptr [00000000656968E8h]
  00000000654594E4: 48 89 5C 24 68     mov         qword ptr [rsp+68h],rbx
  00000000654594E9: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000654594EE: FF 15 E4 D3 23 00  call        qword ptr [00000000656968D8h]
  00000000654594F4: 90                 nop
  00000000654594F5: C7 44 24 60 40 00  mov         dword ptr [rsp+60h],40h
                    00 00
  00000000654594FD: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000065459502: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065459507: FF 15 4B F6 23 00  call        qword ptr [0000000065698B58h]
  000000006545950D: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000065459513: 75 1B              jne         0000000065459530
  0000000065459515: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000006545951A: FF 15 C0 D3 23 00  call        qword ptr [00000000656968E0h]
  0000000065459520: 90                 nop
  0000000065459521: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065459526: FF 15 24 F6 23 00  call        qword ptr [0000000065698B50h]
  000000006545952C: 33 C0              xor         eax,eax
  000000006545952E: EB 5B              jmp         000000006545958B
  0000000065459530: 48 83 3F 00        cmp         qword ptr [rdi],0
  0000000065459534: 74 0B              je          0000000065459541
  0000000065459536: 33 D2              xor         edx,edx
  0000000065459538: 48 8B CF           mov         rcx,rdi
  000000006545953B: FF 15 A7 D3 23 00  call        qword ptr [00000000656968E8h]
  0000000065459541: 44 0F B6 C6        movzx       r8d,sil
  0000000065459545: BA 46 00 00 00     mov         edx,46h
  000000006545954A: 48 8B CB           mov         rcx,rbx
  000000006545954D: E8 8E FA C0 FF     call        0000000065068FE0
  0000000065459552: 4C 8B 5B 08        mov         r11,qword ptr [rbx+8]
  0000000065459556: 41 8B BB 2C 02 00  mov         edi,dword ptr [r11+000000000000022Ch]
                    00
  000000006545955D: 40 84 ED           test        bpl,bpl
  0000000065459560: 74 10              je          0000000065459572
  0000000065459562: 4C 8B 03           mov         r8,qword ptr [rbx]
  0000000065459565: BA 01 00 00 00     mov         edx,1
  000000006545956A: 48 8B CB           mov         rcx,rbx
  000000006545956D: 41 FF 50 18        call        qword ptr [r8+18h]
  0000000065459571: 90                 nop
  0000000065459572: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000065459577: FF 15 63 D3 23 00  call        qword ptr [00000000656968E0h]
  000000006545957D: 90                 nop
  000000006545957E: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065459583: FF 15 C7 F5 23 00  call        qword ptr [0000000065698B50h]
  0000000065459589: 8B C7              mov         eax,edi
  000000006545958B: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  0000000065459590: 48 83 C4 40        add         rsp,40h
  0000000065459594: 5F                 pop         rdi
  0000000065459595: 5E                 pop         rsi
  0000000065459596: 5D                 pop         rbp
  0000000065459597: C3                 ret

  Summary

      695000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006506A510: 40 55              push        rbp
  000000006506A512: 56                 push        rsi
  000000006506A513: 57                 push        rdi
  000000006506A514: 48 83 EC 40        sub         rsp,40h
  000000006506A518: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006506A521: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  000000006506A526: 8B EA              mov         ebp,edx
  000000006506A528: 48 8B F1           mov         rsi,rcx
  000000006506A52B: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006506A52F: 48 89 4C 24 60     mov         qword ptr [rsp+60h],rcx
  000000006506A534: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006506A538: 44 0F B6 88 CB 01  movzx       r9d,byte ptr [rax+00000000000001CBh]
                    00 00
  000000006506A540: 41 80 E1 01        and         r9b,1
  000000006506A544: 33 FF              xor         edi,edi
  000000006506A546: 44 8B C7           mov         r8d,edi
  000000006506A549: 83 FA 01           cmp         edx,1
  000000006506A54C: 41 0F 94 C0        sete        r8b
  000000006506A550: 41 0F B6 C1        movzx       eax,r9b
  000000006506A554: 44 3B C0           cmp         r8d,eax
  000000006506A557: 0F 84 B3 00 00 00  je          000000006506A610
  000000006506A55D: 83 FA 01           cmp         edx,1
  000000006506A560: 41 0F 94 C0        sete        r8b
  000000006506A564: 8D 57 38           lea         edx,[rdi+38h]
  000000006506A567: E8 74 EA FF FF     call        0000000065068FE0
  000000006506A56C: 48 83 C6 18        add         rsi,18h
  000000006506A570: 48 8B CE           mov         rcx,rsi
  000000006506A573: FF 15 57 C3 62 00  call        qword ptr [00000000656968D0h]
  000000006506A579: 84 C0              test        al,al
  000000006506A57B: 75 66              jne         000000006506A5E3
  000000006506A57D: 48 8B CE           mov         rcx,rsi
  000000006506A580: FF 15 32 C3 62 00  call        qword ptr [00000000656968B8h]
  000000006506A586: 85 C0              test        eax,eax
  000000006506A588: 7E 59              jle         000000006506A5E3
  000000006506A58A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006506A590: 8B D7              mov         edx,edi
  000000006506A592: 48 8B CE           mov         rcx,rsi
  000000006506A595: FF 15 A5 C3 62 00  call        qword ptr [0000000065696940h]
  000000006506A59B: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006506A59E: 48 85 DB           test        rbx,rbx
  000000006506A5A1: 74 31              je          000000006506A5D4
  000000006506A5A3: 48 8B CB           mov         rcx,rbx
  000000006506A5A6: FF 15 AC C2 62 00  call        qword ptr [0000000065696858h]
  000000006506A5AC: 84 C0              test        al,al
  000000006506A5AE: 74 24              je          000000006506A5D4
  000000006506A5B0: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  000000006506A5B4: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506A5B8: A8 01              test        al,1
  000000006506A5BA: 75 18              jne         000000006506A5D4
  000000006506A5BC: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006506A5C0: 8B 81 C8 01 00 00  mov         eax,dword ptr [rcx+00000000000001C8h]
  000000006506A5C6: C1 E8 19           shr         eax,19h
  000000006506A5C9: 24 01              and         al,1
  000000006506A5CB: 75 07              jne         000000006506A5D4
  000000006506A5CD: 8B D5              mov         edx,ebp
  000000006506A5CF: E8 3C FF FF FF     call        000000006506A510
  000000006506A5D4: FF C7              inc         edi
  000000006506A5D6: 48 8B CE           mov         rcx,rsi
  000000006506A5D9: FF 15 D9 C2 62 00  call        qword ptr [00000000656968B8h]
  000000006506A5DF: 3B F8              cmp         edi,eax
  000000006506A5E1: 7C AD              jl          000000006506A590
  000000006506A5E3: BA 5A 00 00 00     mov         edx,5Ah
  000000006506A5E8: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006506A5ED: FF 15 25 C2 62 00  call        qword ptr [0000000065696818h]
  000000006506A5F3: 90                 nop
  000000006506A5F4: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006506A5F9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006506A5FE: FF 15 0C C2 62 00  call        qword ptr [0000000065696810h]
  000000006506A604: 90                 nop
  000000006506A605: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006506A60A: FF 15 F8 C1 62 00  call        qword ptr [0000000065696808h]
  000000006506A610: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  000000006506A615: 48 83 C4 40        add         rsp,40h
  000000006506A619: 5F                 pop         rdi
  000000006506A61A: 5E                 pop         rsi
  000000006506A61B: 5D                 pop         rbp
  000000006506A61C: C3                 ret

  Summary

      695000 .text

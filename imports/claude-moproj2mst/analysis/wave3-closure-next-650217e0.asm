Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650217E0: 40 55              push        rbp
  00000000650217E2: 56                 push        rsi
  00000000650217E3: 57                 push        rdi
  00000000650217E4: 41 54              push        r12
  00000000650217E6: 41 55              push        r13
  00000000650217E8: 48 83 EC 40        sub         rsp,40h
  00000000650217EC: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650217F5: 48 89 5C 24 70     mov         qword ptr [rsp+70h],rbx
  00000000650217FA: 48 8B F2           mov         rsi,rdx
  00000000650217FD: 4C 8B E1           mov         r12,rcx
  0000000065021800: 40 32 ED           xor         bpl,bpl
  0000000065021803: E8 98 E6 FF FF     call        000000006501FEA0
  0000000065021808: 48 8B D8           mov         rbx,rax
  000000006502180B: 45 33 ED           xor         r13d,r13d
  000000006502180E: 48 85 F6           test        rsi,rsi
  0000000065021811: 75 7D              jne         0000000065021890
  0000000065021813: E8 38 9A FF FF     call        000000006501B250
  0000000065021818: 48 8B D0           mov         rdx,rax
  000000006502181B: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000065021820: FF 15 EA 53 67 00  call        qword ptr [0000000065696C10h]
  0000000065021826: 90                 nop
  0000000065021827: 48 8B 0D 8A 7E 9A  mov         rcx,qword ptr [00000000659C96B8h]
                    00
  000000006502182E: 48 85 C9           test        rcx,rcx
  0000000065021831: 75 2E              jne         0000000065021861
  0000000065021833: 8D 4E 10           lea         ecx,[rsi+10h]
  0000000065021836: E8 37 F9 5B 00     call        00000000655E1172
  000000006502183B: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000065021843: 48 85 C0           test        rax,rax
  0000000065021846: 74 0D              je          0000000065021855
  0000000065021848: 49 8B D4           mov         rdx,r12
  000000006502184B: 48 8B C8           mov         rcx,rax
  000000006502184E: E8 6D 78 21 00     call        00000000652390C0
  0000000065021853: EB 03              jmp         0000000065021858
  0000000065021855: 49 8B C5           mov         rax,r13
  0000000065021858: 48 89 05 59 7E 9A  mov         qword ptr [00000000659C96B8h],rax
                    00
  000000006502185F: EB 08              jmp         0000000065021869
  0000000065021861: 49 8B D4           mov         rdx,r12
  0000000065021864: E8 F7 17 1D 00     call        00000000651F3060
  0000000065021869: 48 85 DB           test        rbx,rbx
  000000006502186C: 74 15              je          0000000065021883
  000000006502186E: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065021871: 83 78 14 00        cmp         dword ptr [rax+14h],0
  0000000065021875: 74 0C              je          0000000065021883
  0000000065021877: 40 B5 01           mov         bpl,1
  000000006502187A: 48 8B CB           mov         rcx,rbx
  000000006502187D: E8 5E C3 FF FF     call        000000006501DBE0
  0000000065021882: 90                 nop
  0000000065021883: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000065021888: FF 15 72 53 67 00  call        qword ptr [0000000065696C00h]
  000000006502188E: EB 38              jmp         00000000650218C8
  0000000065021890: 48 85 C0           test        rax,rax
  0000000065021893: 74 33              je          00000000650218C8
  0000000065021895: 48 8B D6           mov         rdx,rsi
  0000000065021898: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000006502189D: FF 15 85 54 67 00  call        qword ptr [0000000065696D28h]
  00000000650218A3: 90                 nop
  00000000650218A4: 4D 8B CC           mov         r9,r12
  00000000650218A7: 4C 8D 44 24 78     lea         r8,[rsp+78h]
  00000000650218AC: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  00000000650218B4: 48 8B CB           mov         rcx,rbx
  00000000650218B7: E8 D4 C3 FF FF     call        000000006501DC90
  00000000650218BC: 90                 nop
  00000000650218BD: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000000650218C2: FF 15 88 51 67 00  call        qword ptr [0000000065696A50h]
  00000000650218C8: 48 8B 05 99 53 67  mov         rax,qword ptr [0000000065696C68h]
                    00
  00000000650218CF: 80 38 00           cmp         byte ptr [rax],0
  00000000650218D2: 0F 84 1E 01 00 00  je          00000000650219F6
  00000000650218D8: 48 8B 05 81 53 67  mov         rax,qword ptr [0000000065696C60h]
                    00
  00000000650218DF: 80 38 00           cmp         byte ptr [rax],0
  00000000650218E2: 0F 85 0E 01 00 00  jne         00000000650219F6
  00000000650218E8: BA 24 00 00 00     mov         edx,24h
  00000000650218ED: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650218F2: FF 15 20 4F 67 00  call        qword ptr [0000000065696818h]
  00000000650218F8: 90                 nop
  00000000650218F9: FF 15 21 4F 67 00  call        qword ptr [0000000065696820h]
  00000000650218FF: 48 8B C8           mov         rcx,rax
  0000000065021902: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065021907: FF 15 03 4F 67 00  call        qword ptr [0000000065696810h]
  000000006502190D: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000065021912: E8 F9 BF FF FF     call        000000006501D910
  0000000065021917: 90                 nop
  0000000065021918: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000006502191D: FF 15 25 50 67 00  call        qword ptr [0000000065696948h]
  0000000065021923: 48 8B D8           mov         rbx,rax
  0000000065021926: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000006502192B: FF 15 1F 50 67 00  call        qword ptr [0000000065696950h]
  0000000065021931: 48 3B D8           cmp         rbx,rax
  0000000065021934: 74 4B              je          0000000065021981
  0000000065021936: 48 8B 3B           mov         rdi,qword ptr [rbx]
  0000000065021939: 40 84 ED           test        bpl,bpl
  000000006502193C: 75 21              jne         000000006502195F
  000000006502193E: 48 85 F6           test        rsi,rsi
  0000000065021941: 75 0C              jne         000000006502194F
  0000000065021943: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065021947: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006502194B: A8 01              test        al,1
  000000006502194D: 75 10              jne         000000006502195F
  000000006502194F: 48 8B D6           mov         rdx,rsi
  0000000065021952: 48 8B CF           mov         rcx,rdi
  0000000065021955: FF 15 05 54 67 00  call        qword ptr [0000000065696D60h]
  000000006502195B: 84 C0              test        al,al
  000000006502195D: 74 0E              je          000000006502196D
  000000006502195F: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065021964: 48 8B CF           mov         rcx,rdi
  0000000065021967: FF 15 A3 4E 67 00  call        qword ptr [0000000065696810h]
  000000006502196D: 48 83 C3 08        add         rbx,8
  0000000065021971: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000065021976: FF 15 D4 4F 67 00  call        qword ptr [0000000065696950h]
  000000006502197C: 48 3B D8           cmp         rbx,rax
  000000006502197F: 75 B5              jne         0000000065021936
  0000000065021981: FF 15 99 4E 67 00  call        qword ptr [0000000065696820h]
  0000000065021987: 48 8B 78 08        mov         rdi,qword ptr [rax+8]
  000000006502198B: 48 81 C7 F0 00 00  add         rdi,0F0h
                    00
  0000000065021992: 48 8B CF           mov         rcx,rdi
  0000000065021995: FF 15 AD 4F 67 00  call        qword ptr [0000000065696948h]
  000000006502199B: 48 8B D8           mov         rbx,rax
  000000006502199E: 48 8B CF           mov         rcx,rdi
  00000000650219A1: FF 15 A9 4F 67 00  call        qword ptr [0000000065696950h]
  00000000650219A7: 48 3B D8           cmp         rbx,rax
  00000000650219AA: 74 24              je          00000000650219D0
  00000000650219AC: 0F 1F 40 00        nop         dword ptr [rax]
  00000000650219B0: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000650219B5: 48 8B 0B           mov         rcx,qword ptr [rbx]
  00000000650219B8: FF 15 52 4E 67 00  call        qword ptr [0000000065696810h]
  00000000650219BE: 48 83 C3 08        add         rbx,8
  00000000650219C2: 48 8B CF           mov         rcx,rdi
  00000000650219C5: FF 15 85 4F 67 00  call        qword ptr [0000000065696950h]
  00000000650219CB: 48 3B D8           cmp         rbx,rax
  00000000650219CE: 75 E0              jne         00000000650219B0
  00000000650219D0: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000000650219D5: FF 15 9D 4F 67 00  call        qword ptr [0000000065696978h]
  00000000650219DB: 84 C0              test        al,al
  00000000650219DD: 75 0C              jne         00000000650219EB
  00000000650219DF: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000000650219E4: FF 15 76 4F 67 00  call        qword ptr [0000000065696960h]
  00000000650219EA: 90                 nop
  00000000650219EB: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650219F0: FF 15 12 4E 67 00  call        qword ptr [0000000065696808h]
  00000000650219F6: 48 85 F6           test        rsi,rsi
  00000000650219F9: 75 57              jne         0000000065021A52
  00000000650219FB: 48 8B 15 BE 7C 9A  mov         rdx,qword ptr [00000000659C96C0h]
                    00
  0000000065021A02: 48 85 D2           test        rdx,rdx
  0000000065021A05: 74 0C              je          0000000065021A13
  0000000065021A07: 49 8B CC           mov         rcx,r12
  0000000065021A0A: E8 41 3E 1E 00     call        0000000065205850
  0000000065021A0F: 84 C0              test        al,al
  0000000065021A11: 75 3F              jne         0000000065021A52
  0000000065021A13: 48 8B 0D AE 7C 9A  mov         rcx,qword ptr [00000000659C96C8h]
                    00
  0000000065021A1A: 48 85 C9           test        rcx,rcx
  0000000065021A1D: 75 2B              jne         0000000065021A4A
  0000000065021A1F: B9 10 00 00 00     mov         ecx,10h
  0000000065021A24: E8 49 F7 5B 00     call        00000000655E1172
  0000000065021A29: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000065021A2E: 48 85 C0           test        rax,rax
  0000000065021A31: 74 0E              je          0000000065021A41
  0000000065021A33: 49 8B D4           mov         rdx,r12
  0000000065021A36: 48 8B C8           mov         rcx,rax
  0000000065021A39: E8 82 76 21 00     call        00000000652390C0
  0000000065021A3E: 4C 8B E8           mov         r13,rax
  0000000065021A41: 4C 89 2D 80 7C 9A  mov         qword ptr [00000000659C96C8h],r13
                    00
  0000000065021A48: EB 08              jmp         0000000065021A52
  0000000065021A4A: 49 8B D4           mov         rdx,r12
  0000000065021A4D: E8 0E 16 1D 00     call        00000000651F3060
  0000000065021A52: 48 8B 5C 24 70     mov         rbx,qword ptr [rsp+70h]
  0000000065021A57: 48 83 C4 40        add         rsp,40h
  0000000065021A5B: 41 5D              pop         r13
  0000000065021A5D: 41 5C              pop         r12
  0000000065021A5F: 5F                 pop         rdi
  0000000065021A60: 5E                 pop         rsi
  0000000065021A61: 5D                 pop         rbp
  0000000065021A62: C3                 ret

  Summary

      695000 .text

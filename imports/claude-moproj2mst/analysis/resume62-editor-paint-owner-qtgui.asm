Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065146E30: 48 83 EC 38        sub         rsp,38h
  0000000065146E34: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065146E3D: 48 8B 05 14 4A 88  mov         rax,qword ptr [00000000659CB858h]
                    00
  0000000065146E44: 48 85 C0           test        rax,rax
  0000000065146E47: 0F 85 8F 00 00 00  jne         0000000065146EDC
  0000000065146E4D: 38 05 0D 4A 88 00  cmp         byte ptr [00000000659CB860h],al
  0000000065146E53: 75 7E              jne         0000000065146ED3
  0000000065146E55: 8D 48 04           lea         ecx,[rax+4]
  0000000065146E58: E8 15 A3 49 00     call        00000000655E1172
  0000000065146E5D: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000065146E62: 48 85 C0           test        rax,rax
  0000000065146E65: 74 10              je          0000000065146E77
  0000000065146E67: 33 D2              xor         edx,edx
  0000000065146E69: 48 8B C8           mov         rcx,rax
  0000000065146E6C: FF 15 06 02 55 00  call        qword ptr [0000000065697078h]
  0000000065146E72: 48 8B C8           mov         rcx,rax
  0000000065146E75: EB 02              jmp         0000000065146E79
  0000000065146E77: 33 C9              xor         ecx,ecx
  0000000065146E79: 33 C0              xor         eax,eax
  0000000065146E7B: F0 48 0F B1 0D D4  lock cmpxchg qword ptr [00000000659CB858h],rcx
                    49 88 00
  0000000065146E84: 74 0E              je          0000000065146E94
  0000000065146E86: E8 23 A2 49 00     call        00000000655E10AE
  0000000065146E8B: 48 8B 05 C6 49 88  mov         rax,qword ptr [00000000659CB858h]
                    00
  0000000065146E92: EB 4F              jmp         0000000065146EE3
  0000000065146E94: 8B 05 B6 49 88 00  mov         eax,dword ptr [00000000659CB850h]
  0000000065146E9A: A8 01              test        al,1
  0000000065146E9C: 75 2C              jne         0000000065146ECA
  0000000065146E9E: 83 C8 01           or          eax,1
  0000000065146EA1: 89 05 A9 49 88 00  mov         dword ptr [00000000659CB850h],eax
  0000000065146EA7: 48 8D 05 AA 49 88  lea         rax,[00000000659CB858h]
                    00
  0000000065146EAE: 48 89 05 93 49 88  mov         qword ptr [00000000659CB848h],rax
                    00
  0000000065146EB5: 48 8D 0D 44 E1 54  lea         rcx,[0000000065695000h]
                    00
  0000000065146EBC: E8 6B A1 49 00     call        00000000655E102C
  0000000065146EC1: 48 8B 05 90 49 88  mov         rax,qword ptr [00000000659CB858h]
                    00
  0000000065146EC8: EB 19              jmp         0000000065146EE3
  0000000065146ECA: 48 8B 05 87 49 88  mov         rax,qword ptr [00000000659CB858h]
                    00
  0000000065146ED1: EB 10              jmp         0000000065146EE3
  0000000065146ED3: 48 8B 05 7E 49 88  mov         rax,qword ptr [00000000659CB858h]
                    00
  0000000065146EDA: EB 07              jmp         0000000065146EE3
  0000000065146EDC: 48 8B 05 75 49 88  mov         rax,qword ptr [00000000659CB858h]
                    00
  0000000065146EE3: 48 83 C4 38        add         rsp,38h
  0000000065146EE7: C3                 ret

  Summary

      695000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065148840: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065148845: 53                 push        rbx
  0000000065148846: 56                 push        rsi
  0000000065148847: 57                 push        rdi
  0000000065148848: 48 83 EC 30        sub         rsp,30h
  000000006514884C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065148855: 48 8B F1           mov         rsi,rcx
  0000000065148858: E8 D3 E5 FF FF     call        0000000065146E30
  000000006514885D: 48 85 C0           test        rax,rax
  0000000065148860: 0F 84 20 01 00 00  je          0000000065148986
  0000000065148866: E8 C5 E5 FF FF     call        0000000065146E30
  000000006514886B: 48 8B C8           mov         rcx,rax
  000000006514886E: 33 D2              xor         edx,edx
  0000000065148870: FF 15 DA E2 54 00  call        qword ptr [0000000065696B50h]
  0000000065148876: 84 C0              test        al,al
  0000000065148878: 0F 85 08 01 00 00  jne         0000000065148986
  000000006514887E: 33 DB              xor         ebx,ebx
  0000000065148880: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  0000000065148885: E8 C6 E4 FF FF     call        0000000065146D50
  000000006514888A: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000006514888F: EB 0C              jmp         000000006514889D
  0000000065148891: 33 DB              xor         ebx,ebx
  0000000065148893: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  0000000065148898: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  000000006514889D: 48 8B D0           mov         rdx,rax
  00000000651488A0: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000000651488A5: FF 15 65 E3 54 00  call        qword ptr [0000000065696C10h]
  00000000651488AB: 90                 nop
  00000000651488AC: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000651488B1: E8 CA FB FF FF     call        0000000065148480
  00000000651488B6: 48 8B F8           mov         rdi,rax
  00000000651488B9: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000000651488BE: EB 0C              jmp         00000000651488CC
  00000000651488C0: 33 DB              xor         ebx,ebx
  00000000651488C2: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  00000000651488C7: 48 8B 7C 24 58     mov         rdi,qword ptr [rsp+58h]
  00000000651488CC: 48 85 FF           test        rdi,rdi
  00000000651488CF: 0F 84 A6 00 00 00  je          000000006514897B
  00000000651488D5: 48 8B CF           mov         rcx,rdi
  00000000651488D8: FF 15 DA DF 54 00  call        qword ptr [00000000656968B8h]
  00000000651488DE: 85 C0              test        eax,eax
  00000000651488E0: 0F 8E 95 00 00 00  jle         000000006514897B
  00000000651488E6: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  00000000651488F0: 8B D3              mov         edx,ebx
  00000000651488F2: 48 8B CF           mov         rcx,rdi
  00000000651488F5: FF 15 45 E0 54 00  call        qword ptr [0000000065696940h]
  00000000651488FB: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000651488FE: 48 39 31           cmp         qword ptr [rcx],rsi
  0000000065148901: 74 18              je          000000006514891B
  0000000065148903: 8B D3              mov         edx,ebx
  0000000065148905: 48 8B CF           mov         rcx,rdi
  0000000065148908: FF 15 32 E0 54 00  call        qword ptr [0000000065696940h]
  000000006514890E: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065148911: 48 39 71 08        cmp         qword ptr [rcx+8],rsi
  0000000065148915: 74 04              je          000000006514891B
  0000000065148917: FF C3              inc         ebx
  0000000065148919: EB 4F              jmp         000000006514896A
  000000006514891B: 85 DB              test        ebx,ebx
  000000006514891D: 78 4B              js          000000006514896A
  000000006514891F: 48 8B CF           mov         rcx,rdi
  0000000065148922: FF 15 90 DF 54 00  call        qword ptr [00000000656968B8h]
  0000000065148928: 3B D8              cmp         ebx,eax
  000000006514892A: 7D 3E              jge         000000006514896A
  000000006514892C: BA 01 00 00 00     mov         edx,1
  0000000065148931: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000065148934: FF 15 36 E1 54 00  call        qword ptr [0000000065696A70h]
  000000006514893A: 84 C0              test        al,al
  000000006514893C: 74 0E              je          000000006514894C
  000000006514893E: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000065148941: 8B 50 04           mov         edx,dword ptr [rax+4]
  0000000065148944: 48 8B CF           mov         rcx,rdi
  0000000065148947: E8 C4 AA FF FF     call        0000000065143410
  000000006514894C: 8B D3              mov         edx,ebx
  000000006514894E: 48 8B CF           mov         rcx,rdi
  0000000065148951: FF 15 E9 DF 54 00  call        qword ptr [0000000065696940h]
  0000000065148957: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006514895A: E8 4F 87 49 00     call        00000000655E10AE
  000000006514895F: 8B D3              mov         edx,ebx
  0000000065148961: 48 8B CF           mov         rcx,rdi
  0000000065148964: FF 15 16 E1 54 00  call        qword ptr [0000000065696A80h]
  000000006514896A: 48 8B CF           mov         rcx,rdi
  000000006514896D: FF 15 45 DF 54 00  call        qword ptr [00000000656968B8h]
  0000000065148973: 3B D8              cmp         ebx,eax
  0000000065148975: 0F 8C 75 FF FF FF  jl          00000000651488F0
  000000006514897B: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000065148980: FF 15 7A E2 54 00  call        qword ptr [0000000065696C00h]
  0000000065148986: 48 83 C4 30        add         rsp,30h
  000000006514898A: 5F                 pop         rdi
  000000006514898B: 5E                 pop         rsi
  000000006514898C: 5B                 pop         rbx
  000000006514898D: C3                 ret

  Summary

      695000 .text

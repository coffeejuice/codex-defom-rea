
Dump of file evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006552C340: 40 56              push        rsi
  000000006552C342: 57                 push        rdi
  000000006552C343: 41 54              push        r12
  000000006552C345: 48 83 EC 40        sub         rsp,40h
  000000006552C349: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006552C352: 48 89 5C 24 68     mov         qword ptr [rsp+68h],rbx
  000000006552C357: 48 89 6C 24 70     mov         qword ptr [rsp+70h],rbp
  000000006552C35C: 4C 8B E2           mov         r12,rdx
  000000006552C35F: 48 8B D9           mov         rbx,rcx
  000000006552C362: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006552C366: 83 78 08 00        cmp         dword ptr [rax+8],0
  000000006552C36A: 75 33              jne         000000006552C39F
  000000006552C36C: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006552C370: 48 85 C9           test        rcx,rcx
  000000006552C373: 74 21              je          000000006552C396
  000000006552C375: 4C 8B 43 30        mov         r8,qword ptr [rbx+30h]
  000000006552C379: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006552C37E: E8 FD DA FF FF     call        0000000065529E80
  000000006552C383: 90                 nop
  000000006552C384: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006552C388: 48 8B D0           mov         rdx,rax
  000000006552C38B: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006552C38F: FF 15 DB D1 16 00  call        qword ptr [0000000065699570h]
  000000006552C395: 90                 nop
  000000006552C396: 4C 89 63 08        mov         qword ptr [rbx+8],r12
  000000006552C39A: E9 23 02 00 00     jmp         000000006552C5C2
  000000006552C39F: 48 8B 0D 9A A7 16  mov         rcx,qword ptr [0000000065696B40h]
                    00
  000000006552C3A6: 48 89 4C 24 60     mov         qword ptr [rsp+60h],rcx
  000000006552C3AB: FF 15 0F A5 16 00  call        qword ptr [00000000656968C0h]
  000000006552C3B1: 90                 nop
  000000006552C3B2: BA 01 00 00 00     mov         edx,1
  000000006552C3B7: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006552C3BC: FF 15 AE A6 16 00  call        qword ptr [0000000065696A70h]
  000000006552C3C2: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006552C3C7: 84 C0              test        al,al
  000000006552C3C9: 75 17              jne         000000006552C3E2
  000000006552C3CB: 48 63 51 08        movsxd      rdx,dword ptr [rcx+8]
  000000006552C3CF: 8D 42 01           lea         eax,[rdx+1]
  000000006552C3D2: 3B 41 04           cmp         eax,dword ptr [rcx+4]
  000000006552C3D5: 7F 0B              jg          000000006552C3E2
  000000006552C3D7: 48 8B 43 30        mov         rax,qword ptr [rbx+30h]
  000000006552C3DB: 48 89 44 D1 10     mov         qword ptr [rcx+rdx*8+10h],rax
  000000006552C3E0: EB 3D              jmp         000000006552C41F
  000000006552C3E2: 48 8B 5B 30        mov         rbx,qword ptr [rbx+30h]
  000000006552C3E6: 8B 51 08           mov         edx,dword ptr [rcx+8]
  000000006552C3E9: FF C2              inc         edx
  000000006552C3EB: 45 33 C9           xor         r9d,r9d
  000000006552C3EE: 41 8D 49 18        lea         ecx,[r9+18h]
  000000006552C3F2: 45 8D 41 08        lea         r8d,[r9+8]
  000000006552C3F6: FF 15 EC AC 16 00  call        qword ptr [00000000656970E8h]
  000000006552C3FC: 44 8B C0           mov         r8d,eax
  000000006552C3FF: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000006552C404: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006552C407: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006552C40C: E8 AF 38 FE FF     call        000000006550FCC0
  000000006552C411: 4C 8B 5C 24 60     mov         r11,qword ptr [rsp+60h]
  000000006552C416: 49 63 43 08        movsxd      rax,dword ptr [r11+8]
  000000006552C41A: 49 89 5C C3 10     mov         qword ptr [r11+rax*8+10h],rbx
  000000006552C41F: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000006552C424: FF 40 08           inc         dword ptr [rax+8]
  000000006552C427: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006552C42C: 83 79 08 00        cmp         dword ptr [rcx+8],0
  000000006552C430: 0F 84 72 01 00 00  je          000000006552C5A8
  000000006552C436: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  000000006552C440: BA 01 00 00 00     mov         edx,1
  000000006552C445: FF 15 25 A6 16 00  call        qword ptr [0000000065696A70h]
  000000006552C44B: 84 C0              test        al,al
  000000006552C44D: 74 16              je          000000006552C465
  000000006552C44F: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000006552C454: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006552C458: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006552C45B: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006552C460: E8 5B 38 FE FF     call        000000006550FCC0
  000000006552C465: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000006552C46A: 48 63 50 08        movsxd      rdx,dword ptr [rax+8]
  000000006552C46E: 48 8B 7C D0 08     mov         rdi,qword ptr [rax+rdx*8+8]
  000000006552C473: 8D 5A FF           lea         ebx,[rdx-1]
  000000006552C476: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006552C47A: 41 3B D8           cmp         ebx,r8d
  000000006552C47D: 7F 14              jg          000000006552C493
  000000006552C47F: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006552C483: 75 24              jne         000000006552C4A9
  000000006552C485: 3B 58 08           cmp         ebx,dword ptr [rax+8]
  000000006552C488: 7D 1F              jge         000000006552C4A9
  000000006552C48A: 41 8B C0           mov         eax,r8d
  000000006552C48D: D1 F8              sar         eax,1
  000000006552C48F: 3B D8              cmp         ebx,eax
  000000006552C491: 7D 16              jge         000000006552C4A9
  000000006552C493: 45 33 C9           xor         r9d,r9d
  000000006552C496: 45 8D 41 08        lea         r8d,[r9+8]
  000000006552C49A: 8B D3              mov         edx,ebx
  000000006552C49C: 41 8D 49 18        lea         ecx,[r9+18h]
  000000006552C4A0: FF 15 42 AC 16 00  call        qword ptr [00000000656970E8h]
  000000006552C4A6: 44 8B C0           mov         r8d,eax
  000000006552C4A9: 8B D3              mov         edx,ebx
  000000006552C4AB: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006552C4B0: E8 0B 38 FE FF     call        000000006550FCC0
  000000006552C4B5: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006552C4B9: 48 83 79 08 00     cmp         qword ptr [rcx+8],0
  000000006552C4BE: 74 28              je          000000006552C4E8
  000000006552C4C0: 4C 8B C7           mov         r8,rdi
  000000006552C4C3: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006552C4C8: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006552C4CC: E8 AF D9 FF FF     call        0000000065529E80
  000000006552C4D1: 90                 nop
  000000006552C4D2: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006552C4D6: 48 8B 51 08        mov         rdx,qword ptr [rcx+8]
  000000006552C4DA: 48 8B 4A 08        mov         rcx,qword ptr [rdx+8]
  000000006552C4DE: 48 8B D0           mov         rdx,rax
  000000006552C4E1: FF 15 89 D0 16 00  call        qword ptr [0000000065699570h]
  000000006552C4E7: 90                 nop
  000000006552C4E8: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006552C4EC: 4C 89 60 08        mov         qword ptr [rax+8],r12
  000000006552C4F0: 48 8B 6F 08        mov         rbp,qword ptr [rdi+8]
  000000006552C4F4: 33 F6              xor         esi,esi
  000000006552C4F6: 48 8B 45 20        mov         rax,qword ptr [rbp+20h]
  000000006552C4FA: 39 70 08           cmp         dword ptr [rax+8],esi
  000000006552C4FD: 0F 8E 96 00 00 00  jle         000000006552C599
  000000006552C503: BF 10 00 00 00     mov         edi,10h
  000000006552C508: 0F 1F 84 00 00 00  nop         dword ptr [rax+rax+0000000000000000h]
                    00 00
  000000006552C510: 48 8B 1C 07        mov         rbx,qword ptr [rdi+rax]
  000000006552C514: 48 85 DB           test        rbx,rbx
  000000006552C517: 74 6D              je          000000006552C586
  000000006552C519: BA 01 00 00 00     mov         edx,1
  000000006552C51E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006552C523: FF 15 47 A5 16 00  call        qword ptr [0000000065696A70h]
  000000006552C529: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006552C52E: 84 C0              test        al,al
  000000006552C530: 75 13              jne         000000006552C545
  000000006552C532: 48 63 51 08        movsxd      rdx,dword ptr [rcx+8]
  000000006552C536: 8D 42 01           lea         eax,[rdx+1]
  000000006552C539: 3B 41 04           cmp         eax,dword ptr [rcx+4]
  000000006552C53C: 7F 07              jg          000000006552C545
  000000006552C53E: 48 89 5C D1 10     mov         qword ptr [rcx+rdx*8+10h],rbx
  000000006552C543: EB 39              jmp         000000006552C57E
  000000006552C545: 8B 51 08           mov         edx,dword ptr [rcx+8]
  000000006552C548: FF C2              inc         edx
  000000006552C54A: 45 33 C9           xor         r9d,r9d
  000000006552C54D: 41 8D 49 18        lea         ecx,[r9+18h]
  000000006552C551: 45 8D 41 08        lea         r8d,[r9+8]
  000000006552C555: FF 15 8D AB 16 00  call        qword ptr [00000000656970E8h]
  000000006552C55B: 44 8B C0           mov         r8d,eax
  000000006552C55E: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000006552C563: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006552C566: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  000000006552C56B: E8 50 37 FE FF     call        000000006550FCC0
  000000006552C570: 4C 8B 5C 24 60     mov         r11,qword ptr [rsp+60h]
  000000006552C575: 49 63 43 08        movsxd      rax,dword ptr [r11+8]
  000000006552C579: 49 89 5C C3 10     mov         qword ptr [r11+rax*8+10h],rbx
  000000006552C57E: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  000000006552C583: FF 40 08           inc         dword ptr [rax+8]
  000000006552C586: FF C6              inc         esi
  000000006552C588: 48 83 C7 08        add         rdi,8
  000000006552C58C: 48 8B 45 20        mov         rax,qword ptr [rbp+20h]
  000000006552C590: 3B 70 08           cmp         esi,dword ptr [rax+8]
  000000006552C593: 0F 8C 77 FF FF FF  jl          000000006552C510
  000000006552C599: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006552C59E: 83 79 08 00        cmp         dword ptr [rcx+8],0
  000000006552C5A2: 0F 85 98 FE FF FF  jne         000000006552C440
  000000006552C5A8: FF 15 CA A3 16 00  call        qword ptr [0000000065696978h]
  000000006552C5AE: 84 C0              test        al,al
  000000006552C5B0: 75 10              jne         000000006552C5C2
  000000006552C5B2: BA 08 00 00 00     mov         edx,8
  000000006552C5B7: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000006552C5BC: FF 15 36 A3 16 00  call        qword ptr [00000000656968F8h]
  000000006552C5C2: 48 8B 5C 24 68     mov         rbx,qword ptr [rsp+68h]
  000000006552C5C7: 48 8B 6C 24 70     mov         rbp,qword ptr [rsp+70h]
  000000006552C5CC: 48 83 C4 40        add         rsp,40h
  000000006552C5D0: 41 5C              pop         r12
  000000006552C5D2: 5F                 pop         rdi
  000000006552C5D3: 5E                 pop         rsi
  000000006552C5D4: C3                 ret

  Summary

      695000 .text

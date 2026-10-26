; DUMPBIN decoded from 0x67063b30 through 0x670640b2; selected intervals [(1728461616, 1728461845), (1728462544, 1728462624)]
  0000000067063B30: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000067063B35: 48 89 6C 24 10     mov         qword ptr [rsp+10h],rbp
  0000000067063B3A: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  0000000067063B3F: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  0000000067063B44: 41 54              push        r12
  0000000067063B46: 48 83 EC 20        sub         rsp,20h
  0000000067063B4A: 8B 81 8C 00 00 00  mov         eax,dword ptr [rcx+000000000000008Ch]
  0000000067063B50: 48 8B B1 98 00 00  mov         rsi,qword ptr [rcx+0000000000000098h]
                    00
  0000000067063B57: 8B B9 94 00 00 00  mov         edi,dword ptr [rcx+0000000000000094h]
  0000000067063B5D: FF C8              dec         eax
  0000000067063B5F: 33 ED              xor         ebp,ebp
  0000000067063B61: 48 8B D9           mov         rbx,rcx
  0000000067063B64: 89 81 88 00 00 00  mov         dword ptr [rcx+0000000000000088h],eax
  0000000067063B6A: 89 2E              mov         dword ptr [rsi],ebp
  0000000067063B6C: 48 8B 46 08        mov         rax,qword ptr [rsi+8]
  0000000067063B70: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  0000000067063B74: 45 85 C0           test        r8d,r8d
  0000000067063B77: 78 14              js          0000000067063B8D
  0000000067063B79: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  0000000067063B7D: 75 22              jne         0000000067063BA1
  0000000067063B7F: 39 68 08           cmp         dword ptr [rax+8],ebp
  0000000067063B82: 7E 1D              jle         0000000067063BA1
  0000000067063B84: 41 F7 C0 FE FF FF  test        r8d,0FFFFFFFEh
                    FF
  0000000067063B8B: 7E 14              jle         0000000067063BA1
  0000000067063B8D: 45 33 C9           xor         r9d,r9d
  0000000067063B90: 33 D2              xor         edx,edx
  0000000067063B92: 8D 4A 14           lea         ecx,[rdx+14h]
  0000000067063B95: 45 8D 41 04        lea         r8d,[r9+4]
  0000000067063B99: E8 02 8A 01 00     call        000000006707C5A0
  0000000067063B9E: 44 8B C0           mov         r8d,eax
  0000000067063BA1: 48 8D 4E 08        lea         rcx,[rsi+8]
  0000000067063BA5: 33 D2              xor         edx,edx
  0000000067063BA7: E8 F4 46 0A 00     call        00000000671082A0
  0000000067063BAC: 40 88 6E 10        mov         byte ptr [rsi+10h],bpl
  0000000067063BB0: 48 63 83 8C 00 00  movsxd      rax,dword ptr [rbx+000000000000008Ch]
                    00
  0000000067063BB7: 89 AB A0 00 00 00  mov         dword ptr [rbx+00000000000000A0h],ebp
  0000000067063BBD: 89 AB A4 00 00 00  mov         dword ptr [rbx+00000000000000A4h],ebp
  0000000067063BC3: 3B 83 90 00 00 00  cmp         eax,dword ptr [rbx+0000000000000090h]
  0000000067063BC9: 75 05              jne         0000000067063BD0
  0000000067063BCB: 83 CA FF           or          edx,0FFFFFFFFh
  0000000067063BCE: EB 16              jmp         0000000067063BE6
  0000000067063BD0: 48 8B 8B 80 00 00  mov         rcx,qword ptr [rbx+0000000000000080h]
                    00
  0000000067063BD7: 48 8B D0           mov         rdx,rax
  0000000067063BDA: FF C0              inc         eax
  0000000067063BDC: 89 83 8C 00 00 00  mov         dword ptr [rbx+000000000000008Ch],eax
  0000000067063BE2: 0F B7 14 51        movzx       edx,word ptr [rcx+rdx*2]
  0000000067063BE6: 8D 47 01           lea         eax,[rdi+1]
  0000000067063BE9: 89 93 94 00 00 00  mov         dword ptr [rbx+0000000000000094h],edx
  0000000067063BEF: 83 F8 7E           cmp         eax,7Eh
  0000000067063BF2: 0F 87 D8 03 00 00  ja          0000000067063FD0
  0000000067063BF8: 4C 8D 05 01 C4 F9  lea         r8,[0000000067000000h]
                    FF
  0000000067063BFF: 48 98              cdqe
  0000000067063C01: 41 0F B6 84 00 34  movzx       eax,byte ptr [r8+rax+0000000000064034h]
                    40 06 00
  0000000067063C0A: 41 8B 8C 80 F4 3F  mov         ecx,dword ptr [r8+rax*4+0000000000063FF4h]
                    06 00
  0000000067063C12: 49 03 C8           add         rcx,r8
  0000000067063C15: FF E1              jmp         rcx
  0000000067063ED5: E8 46 62 FF FF     call        000000006705A120
  0000000067063EDA: B8 07 00 00 00     mov         eax,7
  0000000067063EDF: E9 F2 00 00 00     jmp         0000000067063FD6
  0000000067063EE4: E8 87 53 FF FF     call        0000000067059270
  0000000067063EE9: 89 83 94 00 00 00  mov         dword ptr [rbx+0000000000000094h],eax
  0000000067063EEF: B8 07 00 00 00     mov         eax,7
  0000000067063EF4: E9 DD 00 00 00     jmp         0000000067063FD6
  0000000067063EF9: 48 8B CB           mov         rcx,rbx
  0000000067063EFC: E8 CF EA FF FF     call        00000000670629D0
  0000000067063F01: E9 D0 00 00 00     jmp         0000000067063FD6
  0000000067063F06: 48 8D 15 EB F9 18  lea         rdx,[00000000671F38F8h]
                    00
  0000000067063F0D: 48 8B CB           mov         rcx,rbx
  0000000067063F10: E8 0B 62 FF FF     call        000000006705A120
  0000000067063F15: B8 5D 00 01 00     mov         eax,1005Dh
  0000000067063F1A: E9 B7 00 00 00     jmp         0000000067063FD6
  0000000067063F1F: B8 08 00 00 00     mov         eax,8

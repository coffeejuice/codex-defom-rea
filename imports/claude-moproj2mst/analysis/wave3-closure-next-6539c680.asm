Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006539C680: 40 53              push        rbx
  000000006539C682: 55                 push        rbp
  000000006539C683: 56                 push        rsi
  000000006539C684: 57                 push        rdi
  000000006539C685: 41 54              push        r12
  000000006539C687: 41 55              push        r13
  000000006539C689: 41 56              push        r14
  000000006539C68B: 41 57              push        r15
  000000006539C68D: 48 83 EC 48        sub         rsp,48h
  000000006539C691: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006539C69A: 48 8B F9           mov         rdi,rcx
  000000006539C69D: 48 8B 69 08        mov         rbp,qword ptr [rcx+8]
  000000006539C6A1: 48 89 6C 24 28     mov         qword ptr [rsp+28h],rbp
  000000006539C6A6: 48 8B 89 38 02 00  mov         rcx,qword ptr [rcx+0000000000000238h]
                    00
  000000006539C6AD: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006539C6B0: FF 50 78           call        qword ptr [rax+78h]
  000000006539C6B3: 8B F0              mov         esi,eax
  000000006539C6B5: 83 EE 01           sub         esi,1
  000000006539C6B8: 78 46              js          000000006539C700
  000000006539C6BA: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006539C6C0: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539C6C7: 4C 8B 01           mov         r8,qword ptr [rcx]
  000000006539C6CA: 8B D6              mov         edx,esi
  000000006539C6CC: 41 FF 50 68        call        qword ptr [r8+68h]
  000000006539C6D0: 48 8B D8           mov         rbx,rax
  000000006539C6D3: 48 8B 10           mov         rdx,qword ptr [rax]
  000000006539C6D6: 48 8B C8           mov         rcx,rax
  000000006539C6D9: FF 52 60           call        qword ptr [rdx+60h]
  000000006539C6DC: 48 85 C0           test        rax,rax
  000000006539C6DF: 74 0C              je          000000006539C6ED
  000000006539C6E1: 4C 8B 00           mov         r8,qword ptr [rax]
  000000006539C6E4: 33 D2              xor         edx,edx
  000000006539C6E6: 48 8B C8           mov         rcx,rax
  000000006539C6E9: 41 FF 50 58        call        qword ptr [r8+58h]
  000000006539C6ED: 4C 8B 03           mov         r8,qword ptr [rbx]
  000000006539C6F0: BA 01 00 00 00     mov         edx,1
  000000006539C6F5: 48 8B CB           mov         rcx,rbx
  000000006539C6F8: 41 FF 10           call        qword ptr [r8]
  000000006539C6FB: 83 EE 01           sub         esi,1
  000000006539C6FE: 79 C0              jns         000000006539C6C0
  000000006539C700: 8B B7 34 02 00 00  mov         esi,dword ptr [rdi+0000000000000234h]
  000000006539C706: 83 FE 01           cmp         esi,1
  000000006539C709: 75 38              jne         000000006539C743
  000000006539C70B: 48 8D 1D F6 08 36  lea         rbx,[00000000656FD008h]
                    00
  000000006539C712: 4C 8D 25 03 09 36  lea         r12,[00000000656FD01Ch]
                    00
  000000006539C719: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006539C720: 48 63 03           movsxd      rax,dword ptr [rbx]
  000000006539C723: 48 8D 8C C7 E0 01  lea         rcx,[rdi+rax*8+00000000000001E0h]
                    00 00
  000000006539C72B: FF 15 9F A1 2F 00  call        qword ptr [00000000656968D0h]
  000000006539C731: 84 C0              test        al,al
  000000006539C733: 74 0E              je          000000006539C743
  000000006539C735: 48 83 C3 04        add         rbx,4
  000000006539C739: 49 3B DC           cmp         rbx,r12
  000000006539C73C: 7C E2              jl          000000006539C720
  000000006539C73E: BE 04 00 00 00     mov         esi,4
  000000006539C743: 48 63 F6           movsxd      rsi,esi
  000000006539C746: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  000000006539C74B: 45 33 ED           xor         r13d,r13d
  000000006539C74E: 4D 8B FD           mov         r15,r13
  000000006539C751: 83 BF 30 02 00 00  cmp         dword ptr [rdi+0000000000000230h],2
                    02
  000000006539C758: 41 0F 94 C7        sete        r15b
  000000006539C75C: 4A 8D 04 BE        lea         rax,[rsi+r15*4]
  000000006539C760: 4C 03 F8           add         r15,rax
  000000006539C763: 4D 6B FF 38        imul        r15,r15,38h
  000000006539C767: 48 8D 05 12 FE 35  lea         rax,[00000000656FC580h]
                    00
  000000006539C76E: 4C 03 F8           add         r15,rax
  000000006539C771: 44 38 AF 41 02 00  cmp         byte ptr [rdi+0000000000000241h],r13b
                    00
  000000006539C778: 74 0E              je          000000006539C788
  000000006539C77A: 33 D2              xor         edx,edx
  000000006539C77C: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539C783: E8 68 EE C8 FF     call        000000006502B5F0
  000000006539C788: 48 8B 8F E0 01 00  mov         rcx,qword ptr [rdi+00000000000001E0h]
                    00
  000000006539C78F: 48 89 8C 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rcx
                    00 00
  000000006539C797: FF 15 23 A1 2F 00  call        qword ptr [00000000656968C0h]
  000000006539C79D: 4C 8B 9C 24 A8 00  mov         r11,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006539C7A5: 41 F6 43 10 01     test        byte ptr [r11+10h],1
  000000006539C7AA: 75 12              jne         000000006539C7BE
  000000006539C7AC: 41 8B 53 04        mov         edx,dword ptr [r11+4]
  000000006539C7B0: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006539C7B8: E8 63 77 E0 FF     call        00000000651A3F20
  000000006539C7BD: 90                 nop
  000000006539C7BE: 41 8B 07           mov         eax,dword ptr [r15]
  000000006539C7C1: 3D 00 00 00 40     cmp         eax,40000000h
  000000006539C7C6: 0F 84 04 03 00 00  je          000000006539CAD0
  000000006539C7CC: 83 CB FF           or          ebx,0FFFFFFFFh
  000000006539C7CF: 90                 nop
  000000006539C7D0: 8B C8              mov         ecx,eax
  000000006539C7D2: 0F BA F1 1F        btr         ecx,1Fh
  000000006539C7D6: A9 00 00 00 80     test        eax,80000000h
  000000006539C7DB: 41 0F 95 C6        setne       r14b
  000000006539C7DF: 83 F9 08           cmp         ecx,8
  000000006539C7E2: 0F 8F 47 01 00 00  jg          000000006539C92F
  000000006539C7E8: 83 F9 03           cmp         ecx,3
  000000006539C7EB: 0F 8D 22 01 00 00  jge         000000006539C913
  000000006539C7F1: 8B D1              mov         edx,ecx
  000000006539C7F3: 85 C9              test        ecx,ecx
  000000006539C7F5: 0F 84 8F 00 00 00  je          000000006539C88A
  000000006539C7FB: 83 EA 01           sub         edx,1
  000000006539C7FE: 0F 84 0F 01 00 00  je          000000006539C913
  000000006539C804: 83 FA 01           cmp         edx,1
  000000006539C807: 0F 85 A7 02 00 00  jne         000000006539CAB4
  000000006539C80D: 48 83 FE 01        cmp         rsi,1
  000000006539C811: 75 31              jne         000000006539C844
  000000006539C813: 48 8D 8F F0 01 00  lea         rcx,[rdi+00000000000001F0h]
                    00
  000000006539C81A: FF 15 B0 A0 2F 00  call        qword ptr [00000000656968D0h]
  000000006539C820: 84 C0              test        al,al
  000000006539C822: 75 20              jne         000000006539C844
  000000006539C824: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539C82B: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006539C82E: FF 50 78           call        qword ptr [rax+78h]
  000000006539C831: 3B C6              cmp         eax,esi
  000000006539C833: 7E 0F              jle         000000006539C844
  000000006539C835: 8D 56 1B           lea         edx,[rsi+1Bh]
  000000006539C838: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539C83F: E8 9C ED C8 FF     call        000000006502B5E0
  000000006539C844: 45 0F B6 C6        movzx       r8d,r14b
  000000006539C848: 48 8D 97 F0 01 00  lea         rdx,[rdi+00000000000001F0h]
                    00
  000000006539C84F: 48 8B CF           mov         rcx,rdi
  000000006539C852: E8 19 E9 FF FF     call        000000006539B170
  000000006539C857: 48 83 FE 01        cmp         rsi,1
  000000006539C85B: 0F 85 53 02 00 00  jne         000000006539CAB4
  000000006539C861: 48 8D 8F F0 01 00  lea         rcx,[rdi+00000000000001F0h]
                    00
  000000006539C868: FF 15 62 A0 2F 00  call        qword ptr [00000000656968D0h]
  000000006539C86E: 84 C0              test        al,al
  000000006539C870: 0F 85 3E 02 00 00  jne         000000006539CAB4
  000000006539C876: 8D 56 1B           lea         edx,[rsi+1Bh]
  000000006539C879: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539C880: E8 5B ED C8 FF     call        000000006502B5E0
  000000006539C885: E9 2A 02 00 00     jmp         000000006539CAB4
  000000006539C88A: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006539C892: FF 15 38 A0 2F 00  call        qword ptr [00000000656968D0h]
  000000006539C898: 84 C0              test        al,al
  000000006539C89A: 0F 85 14 02 00 00  jne         000000006539CAB4
  000000006539C8A0: BA 01 00 00 00     mov         edx,1
  000000006539C8A5: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006539C8AD: FF 15 BD A1 2F 00  call        qword ptr [0000000065696A70h]
  000000006539C8B3: 84 C0              test        al,al
  000000006539C8B5: 74 18              je          000000006539C8CF
  000000006539C8B7: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006539C8BF: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006539C8C2: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006539C8CA: E8 51 76 E0 FF     call        00000000651A3F20
  000000006539C8CF: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006539C8D7: FF 15 6B A0 2F 00  call        qword ptr [0000000065696948h]
  000000006539C8DD: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006539C8E0: 44 89 AC 24 90 00  mov         dword ptr [rsp+0000000000000090h],r13d
                    00 00
  000000006539C8E8: 4C 8D 8C 24 90 00  lea         r9,[rsp+0000000000000090h]
                    00 00
  000000006539C8F0: 45 33 C0           xor         r8d,r8d
  000000006539C8F3: 48 8B D3           mov         rdx,rbx
  000000006539C8F6: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539C8FD: E8 8E ED C8 FF     call        000000006502B690
  000000006539C902: 4C 8B 1B           mov         r11,qword ptr [rbx]
  000000006539C905: B2 01              mov         dl,1
  000000006539C907: 48 8B CB           mov         rcx,rbx
  000000006539C90A: 41 FF 53 58        call        qword ptr [r11+58h]
  000000006539C90E: E9 A1 01 00 00     jmp         000000006539CAB4
  000000006539C913: 48 63 C1           movsxd      rax,ecx
  000000006539C916: 48 8D 94 C7 E0 01  lea         rdx,[rdi+rax*8+00000000000001E0h]
                    00 00
  000000006539C91E: 45 0F B6 C6        movzx       r8d,r14b
  000000006539C922: 48 8B CF           mov         rcx,rdi
  000000006539C925: E8 46 E8 FF FF     call        000000006539B170
  000000006539C92A: E9 85 01 00 00     jmp         000000006539CAB4
  000000006539C92F: 81 F9 00 00 00 10  cmp         ecx,10000000h
  000000006539C935: 74 2C              je          000000006539C963
  000000006539C937: 81 F9 00 00 00 20  cmp         ecx,20000000h
  000000006539C93D: 0F 85 71 01 00 00  jne         000000006539CAB4
  000000006539C943: 80 BF 41 02 00 00  cmp         byte ptr [rdi+0000000000000241h],0
                    00
  000000006539C94A: 0F 85 64 01 00 00  jne         000000006539CAB4
  000000006539C950: 33 D2              xor         edx,edx
  000000006539C952: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539C959: E8 92 EC C8 FF     call        000000006502B5F0
  000000006539C95E: E9 51 01 00 00     jmp         000000006539CAB4
  000000006539C963: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000006539C96B: FF 15 47 9F 2F 00  call        qword ptr [00000000656968B8h]
  000000006539C971: 83 F8 02           cmp         eax,2
  000000006539C974: 0F 8C 3A 01 00 00  jl          000000006539CAB4
  000000006539C97A: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006539C982: 48 89 8C 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rcx
                    00 00
  000000006539C98A: FF 15 30 9F 2F 00  call        qword ptr [00000000656968C0h]
  000000006539C990: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006539C998: F6 41 10 01        test        byte ptr [rcx+10h],1
  000000006539C99C: 75 18              jne         000000006539C9B6
  000000006539C99E: 8B 51 04           mov         edx,dword ptr [rcx+4]
  000000006539C9A1: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006539C9A9: E8 72 75 E0 FF     call        00000000651A3F20
  000000006539C9AE: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006539C9B6: BA 01 00 00 00     mov         edx,1
  000000006539C9BB: FF 15 AF A0 2F 00  call        qword ptr [0000000065696A70h]
  000000006539C9C1: 84 C0              test        al,al
  000000006539C9C3: 74 18              je          000000006539C9DD
  000000006539C9C5: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006539C9CD: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006539C9D0: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006539C9D8: E8 43 75 E0 FF     call        00000000651A3F20
  000000006539C9DD: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006539C9E5: FF 15 5D 9F 2F 00  call        qword ptr [0000000065696948h]
  000000006539C9EB: 48 8B D0           mov         rdx,rax
  000000006539C9EE: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006539C9F6: FF 15 1C A0 2F 00  call        qword ptr [0000000065696A18h]
  000000006539C9FC: 45 84 F6           test        r14b,r14b
  000000006539C9FF: 74 15              je          000000006539CA16
  000000006539CA01: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006539CA09: FF 15 A9 9E 2F 00  call        qword ptr [00000000656968B8h]
  000000006539CA0F: 8D 70 FF           lea         esi,[rax-1]
  000000006539CA12: 8B EB              mov         ebp,ebx
  000000006539CA14: EB 13              jmp         000000006539CA29
  000000006539CA16: 41 8B F5           mov         esi,r13d
  000000006539CA19: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006539CA21: FF 15 91 9E 2F 00  call        qword ptr [00000000656968B8h]
  000000006539CA27: 8B E8              mov         ebp,eax
  000000006539CA29: 41 BC 01 00 00 00  mov         r12d,1
  000000006539CA2F: 45 84 F6           test        r14b,r14b
  000000006539CA32: 44 0F 45 E3        cmovne      r12d,ebx
  000000006539CA36: 3B F5              cmp         esi,ebp
  000000006539CA38: 74 4E              je          000000006539CA88
  000000006539CA3A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006539CA40: 8B D6              mov         edx,esi
  000000006539CA42: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000006539CA4A: FF 15 F0 9E 2F 00  call        qword ptr [0000000065696940h]
  000000006539CA50: 48 8B 18           mov         rbx,qword ptr [rax]
  000000006539CA53: 44 89 AC 24 98 00  mov         dword ptr [rsp+0000000000000098h],r13d
                    00 00
  000000006539CA5B: 4C 8D 8C 24 98 00  lea         r9,[rsp+0000000000000098h]
                    00 00
  000000006539CA63: 45 33 C0           xor         r8d,r8d
  000000006539CA66: 48 8B D3           mov         rdx,rbx
  000000006539CA69: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539CA70: E8 1B EC C8 FF     call        000000006502B690
  000000006539CA75: 4C 8B 1B           mov         r11,qword ptr [rbx]
  000000006539CA78: B2 01              mov         dl,1
  000000006539CA7A: 48 8B CB           mov         rcx,rbx
  000000006539CA7D: 41 FF 53 58        call        qword ptr [r11+58h]
  000000006539CA81: 41 03 F4           add         esi,r12d
  000000006539CA84: 3B F5              cmp         esi,ebp
  000000006539CA86: 75 B8              jne         000000006539CA40
  000000006539CA88: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006539CA90: FF 15 E2 9E 2F 00  call        qword ptr [0000000065696978h]
  000000006539CA96: 84 C0              test        al,al
  000000006539CA98: 75 15              jne         000000006539CAAF
  000000006539CA9A: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000006539CAA2: FF 15 B8 9E 2F 00  call        qword ptr [0000000065696960h]
  000000006539CAA8: 48 8B 74 24 20     mov         rsi,qword ptr [rsp+20h]
  000000006539CAAD: EB 05              jmp         000000006539CAB4
  000000006539CAAF: 48 8B 74 24 20     mov         rsi,qword ptr [rsp+20h]
  000000006539CAB4: 49 83 C7 04        add         r15,4
  000000006539CAB8: 41 8B 07           mov         eax,dword ptr [r15]
  000000006539CABB: 3D 00 00 00 40     cmp         eax,40000000h
  000000006539CAC0: BB FF FF FF FF     mov         ebx,0FFFFFFFFh
  000000006539CAC5: 0F 85 05 FD FF FF  jne         000000006539C7D0
  000000006539CACB: 48 8B 6C 24 28     mov         rbp,qword ptr [rsp+28h]
  000000006539CAD0: 33 D2              xor         edx,edx
  000000006539CAD2: 48 8B CD           mov         rcx,rbp
  000000006539CAD5: E8 86 7B CC FF     call        0000000065064660
  000000006539CADA: 41 8B F5           mov         esi,r13d
  000000006539CADD: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539CAE4: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006539CAE7: FF 50 78           call        qword ptr [rax+78h]
  000000006539CAEA: 85 C0              test        eax,eax
  000000006539CAEC: 7E 55              jle         000000006539CB43
  000000006539CAEE: 66 90              nop
  000000006539CAF0: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539CAF7: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006539CAFA: 8B D6              mov         edx,esi
  000000006539CAFC: FF 50 60           call        qword ptr [rax+60h]
  000000006539CAFF: 48 8B 10           mov         rdx,qword ptr [rax]
  000000006539CB02: 48 8B C8           mov         rcx,rax
  000000006539CB05: FF 52 60           call        qword ptr [rdx+60h]
  000000006539CB08: 48 8B D8           mov         rbx,rax
  000000006539CB0B: 48 85 C0           test        rax,rax
  000000006539CB0E: 74 20              je          000000006539CB30
  000000006539CB10: 4D 85 ED           test        r13,r13
  000000006539CB13: 74 0D              je          000000006539CB22
  000000006539CB15: 48 8B D0           mov         rdx,rax
  000000006539CB18: 49 8B CD           mov         rcx,r13
  000000006539CB1B: E8 30 81 CC FF     call        0000000065064C50
  000000006539CB20: EB 0B              jmp         000000006539CB2D
  000000006539CB22: 48 8B D0           mov         rdx,rax
  000000006539CB25: 48 8B CD           mov         rcx,rbp
  000000006539CB28: E8 33 7B CC FF     call        0000000065064660
  000000006539CB2D: 4C 8B EB           mov         r13,rbx
  000000006539CB30: FF C6              inc         esi
  000000006539CB32: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539CB39: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006539CB3C: FF 50 78           call        qword ptr [rax+78h]
  000000006539CB3F: 3B F0              cmp         esi,eax
  000000006539CB41: 7C AD              jl          000000006539CAF0
  000000006539CB43: 80 BF 41 02 00 00  cmp         byte ptr [rdi+0000000000000241h],0
                    00
  000000006539CB4A: 74 0F              je          000000006539CB5B
  000000006539CB4C: 33 D2              xor         edx,edx
  000000006539CB4E: 48 8B 8F 38 02 00  mov         rcx,qword ptr [rdi+0000000000000238h]
                    00
  000000006539CB55: E8 96 EA C8 FF     call        000000006502B5F0
  000000006539CB5A: 90                 nop
  000000006539CB5B: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006539CB63: FF 15 0F 9E 2F 00  call        qword ptr [0000000065696978h]
  000000006539CB69: 84 C0              test        al,al
  000000006539CB6B: 75 0E              jne         000000006539CB7B
  000000006539CB6D: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000006539CB75: FF 15 E5 9D 2F 00  call        qword ptr [0000000065696960h]
  000000006539CB7B: 48 83 C4 48        add         rsp,48h
  000000006539CB7F: 41 5F              pop         r15
  000000006539CB81: 41 5E              pop         r14
  000000006539CB83: 41 5D              pop         r13
  000000006539CB85: 41 5C              pop         r12
  000000006539CB87: 5F                 pop         rdi
  000000006539CB88: 5E                 pop         rsi
  000000006539CB89: 5D                 pop         rbp
  000000006539CB8A: 5B                 pop         rbx
  000000006539CB8B: C3                 ret

  Summary

      695000 .text

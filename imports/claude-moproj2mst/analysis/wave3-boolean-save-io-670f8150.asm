Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670F8150: 48 8B C4           mov         rax,rsp
  00000000670F8153: 56                 push        rsi
  00000000670F8154: 57                 push        rdi
  00000000670F8155: 41 54              push        r12
  00000000670F8157: 48 83 EC 60        sub         rsp,60h
  00000000670F815B: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670F8164: 48 89 58 18        mov         qword ptr [rax+18h],rbx
  00000000670F8168: 48 89 68 20        mov         qword ptr [rax+20h],rbp
  00000000670F816C: 4C 8B E2           mov         r12,rdx
  00000000670F816F: 48 8B F1           mov         rsi,rcx
  00000000670F8172: 48 8B 69 18        mov         rbp,qword ptr [rcx+18h]
  00000000670F8176: 8B 02              mov         eax,dword ptr [rdx]
  00000000670F8178: 44 8B C0           mov         r8d,eax
  00000000670F817B: 41 83 E0 01        and         r8d,1
  00000000670F817F: BF 00 00 00 00     mov         edi,0
  00000000670F8184: B9 00 00 00 80     mov         ecx,80000000h
  00000000670F8189: 0F 45 F9           cmovne      edi,ecx
  00000000670F818C: 83 E0 02           and         eax,2
  00000000670F818F: 74 04              je          00000000670F8195
  00000000670F8191: 0F BA EF 1E        bts         edi,1Eh
  00000000670F8195: C7 44 24 48 18 00  mov         dword ptr [rsp+48h],18h
                    00 00
  00000000670F819D: 48 C7 44 24 50 00  mov         qword ptr [rsp+50h],0
                    00 00 00
  00000000670F81A6: C7 44 24 58 00 00  mov         dword ptr [rsp+58h],0
                    00 00
  00000000670F81AE: 33 DB              xor         ebx,ebx
  00000000670F81B0: 85 C0              test        eax,eax
  00000000670F81B2: 0F 95 C3           setne       bl
  00000000670F81B5: 83 C3 03           add         ebx,3
  00000000670F81B8: 48 8D 4E 20        lea         rcx,[rsi+20h]
  00000000670F81BC: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  00000000670F81C4: E8 57 79 FF FF     call        00000000670EFB20
  00000000670F81C9: 90                 nop
  00000000670F81CA: 48 8B C8           mov         rcx,rax
  00000000670F81CD: E8 7E 88 F7 FF     call        0000000067070A50
  00000000670F81D2: 48 8B C8           mov         rcx,rax
  00000000670F81D5: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  00000000670F81DE: C7 44 24 28 80 00  mov         dword ptr [rsp+28h],80h
                    00 00
  00000000670F81E6: 89 5C 24 20        mov         dword ptr [rsp+20h],ebx
  00000000670F81EA: 4C 8D 4C 24 48     lea         r9,[rsp+48h]
  00000000670F81EF: 41 B8 03 00 00 00  mov         r8d,3
  00000000670F81F5: 8B D7              mov         edx,edi
  00000000670F81F7: FF 15 3B FF 0A 00  call        qword ptr [00000000671A8138h]
  00000000670F81FD: 48 89 46 78        mov         qword ptr [rsi+78h],rax
  00000000670F8201: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000000670F8209: 83 CB FF           or          ebx,0FFFFFFFFh
  00000000670F820C: 8B C3              mov         eax,ebx
  00000000670F820E: F0 0F C1 01        lock xadd   dword ptr [rcx],eax
  00000000670F8212: 03 C3              add         eax,ebx
  00000000670F8214: 75 0D              jne         00000000670F8223
  00000000670F8216: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000000670F821E: E8 AD 7E F7 FF     call        00000000670700D0
  00000000670F8223: 48 83 7E 78 FF     cmp         qword ptr [rsi+78h],0FFFFFFFFFFFFFFFFh
  00000000670F8228: 75 40              jne         00000000670F826A
  00000000670F822A: 8B D3              mov         edx,ebx
  00000000670F822C: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000000670F8234: E8 D7 44 F2 FF     call        000000006701C710
  00000000670F8239: 90                 nop
  00000000670F823A: 4C 8B C0           mov         r8,rax
  00000000670F823D: BA 05 00 00 00     mov         edx,5
  00000000670F8242: 48 8B CD           mov         rcx,rbp
  00000000670F8245: E8 F6 F7 FA FF     call        00000000670A7A40
  00000000670F824A: 90                 nop
  00000000670F824B: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670F8253: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F8257: 75 0D              jne         00000000670F8266
  00000000670F8259: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670F8261: E8 6A 7E F7 FF     call        00000000670700D0
  00000000670F8266: 32 C0              xor         al,al
  00000000670F8268: EB 31              jmp         00000000670F829B
  00000000670F826A: 41 F6 04 24 08     test        byte ptr [r12],8
  00000000670F826F: 74 0C              je          00000000670F827D
  00000000670F8271: 48 8B 45 00        mov         rax,qword ptr [rbp]
  00000000670F8275: 33 D2              xor         edx,edx
  00000000670F8277: 48 8B CD           mov         rcx,rbp
  00000000670F827A: FF 50 70           call        qword ptr [rax+70h]
  00000000670F827D: 41 F6 04 24 04     test        byte ptr [r12],4
  00000000670F8282: 74 15              je          00000000670F8299
  00000000670F8284: 41 B9 02 00 00 00  mov         r9d,2
  00000000670F828A: 45 33 C0           xor         r8d,r8d
  00000000670F828D: 33 D2              xor         edx,edx
  00000000670F828F: 48 8B 4E 78        mov         rcx,qword ptr [rsi+78h]
  00000000670F8293: FF 15 CF FE 0A 00  call        qword ptr [00000000671A8168h]
  00000000670F8299: B0 01              mov         al,1
  00000000670F829B: 4C 8D 5C 24 60     lea         r11,[rsp+60h]
  00000000670F82A0: 49 8B 5B 30        mov         rbx,qword ptr [r11+30h]
  00000000670F82A4: 49 8B 6B 38        mov         rbp,qword ptr [r11+38h]
  00000000670F82A8: 49 8B E3           mov         rsp,r11
  00000000670F82AB: 41 5C              pop         r12
  00000000670F82AD: 5F                 pop         rdi
  00000000670F82AE: 5E                 pop         rsi
  00000000670F82AF: C3                 ret

  Summary

      1A7000 .text

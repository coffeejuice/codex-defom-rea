Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000651EEF50: 40 55              push        rbp
  00000000651EEF52: 56                 push        rsi
  00000000651EEF53: 57                 push        rdi
  00000000651EEF54: 48 83 EC 60        sub         rsp,60h
  00000000651EEF58: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000651EEF61: 48 89 9C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rbx
                    00 00
  00000000651EEF69: 48 8B EA           mov         rbp,rdx
  00000000651EEF6C: 48 8B F9           mov         rdi,rcx
  00000000651EEF6F: 48 8B 71 08        mov         rsi,qword ptr [rcx+8]
  00000000651EEF73: 48 8B CE           mov         rcx,rsi
  00000000651EEF76: E8 E5 DB E6 FF     call        000000006505CB60
  00000000651EEF7B: 4C 8B 40 08        mov         r8,qword ptr [rax+8]
  00000000651EEF7F: 49 8B 98 88 00 00  mov         rbx,qword ptr [r8+0000000000000088h]
                    00
  00000000651EEF86: 48 85 DB           test        rbx,rbx
  00000000651EEF89: 74 06              je          00000000651EEF91
  00000000651EEF8B: 48 8B 5B 08        mov         rbx,qword ptr [rbx+8]
  00000000651EEF8F: EB 02              jmp         00000000651EEF93
  00000000651EEF91: 33 DB              xor         ebx,ebx
  00000000651EEF93: FF 15 5F 80 4A 00  call        qword ptr [0000000065696FF8h]
  00000000651EEF99: 84 C0              test        al,al
  00000000651EEF9B: 0F 85 43 01 00 00  jne         00000000651EF0E4
  00000000651EEFA1: 48 85 DB           test        rbx,rbx
  00000000651EEFA4: 0F 84 3A 01 00 00  je          00000000651EF0E4
  00000000651EEFAA: 0F BA 63 7C 0A     bt          dword ptr [rbx+7Ch],0Ah
  00000000651EEFAF: 0F 82 2F 01 00 00  jb          00000000651EF0E4
  00000000651EEFB5: 48 83 7B 10 00     cmp         qword ptr [rbx+10h],0
  00000000651EEFBA: 0F 84 24 01 00 00  je          00000000651EF0E4
  00000000651EEFC0: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  00000000651EEFC4: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000651EEFC7: 8B C1              mov         eax,ecx
  00000000651EEFC9: C1 E8 0F           shr         eax,0Fh
  00000000651EEFCC: 24 01              and         al,1
  00000000651EEFCE: 0F 84 10 01 00 00  je          00000000651EF0E4
  00000000651EEFD4: C1 E9 0A           shr         ecx,0Ah
  00000000651EEFD7: 80 E1 01           and         cl,1
  00000000651EEFDA: 0F 85 04 01 00 00  jne         00000000651EF0E4
  00000000651EEFE0: 48 8B CD           mov         rcx,rbp
  00000000651EEFE3: FF 15 5F 83 4A 00  call        qword ptr [0000000065697348h]
  00000000651EEFE9: 84 C0              test        al,al
  00000000651EEFEB: 0F 85 F3 00 00 00  jne         00000000651EF0E4
  00000000651EEFF1: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000651EEFF6: 8B 45 00           mov         eax,dword ptr [rbp]
  00000000651EEFF9: 89 01              mov         dword ptr [rcx],eax
  00000000651EEFFB: 8B 45 04           mov         eax,dword ptr [rbp+4]
  00000000651EEFFE: 89 41 04           mov         dword ptr [rcx+4],eax
  00000000651EF001: 8B 45 08           mov         eax,dword ptr [rbp+8]
  00000000651EF004: 89 41 08           mov         dword ptr [rcx+8],eax
  00000000651EF007: 8B 45 0C           mov         eax,dword ptr [rbp+0Ch]
  00000000651EF00A: 89 41 0C           mov         dword ptr [rcx+0Ch],eax
  00000000651EF00D: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000000651EF012: 48 8B CF           mov         rcx,rdi
  00000000651EF015: E8 06 40 E7 FF     call        0000000065063020
  00000000651EF01A: 48 8B D0           mov         rdx,rax
  00000000651EF01D: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000651EF022: FF 15 90 83 4A 00  call        qword ptr [00000000656973B8h]
  00000000651EF028: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000651EF02D: FF 15 15 83 4A 00  call        qword ptr [0000000065697348h]
  00000000651EF033: 84 C0              test        al,al
  00000000651EF035: 0F 85 A9 00 00 00  jne         00000000651EF0E4
  00000000651EF03B: 48 83 BF D8 00 00  cmp         qword ptr [rdi+00000000000000D8h],0
                    00 00
  00000000651EF043: 0F 85 82 00 00 00  jne         00000000651EF0CB
  00000000651EF049: 48 8B 97 88 00 00  mov         rdx,qword ptr [rdi+0000000000000088h]
                    00
  00000000651EF050: 48 85 D2           test        rdx,rdx
  00000000651EF053: 74 76              je          00000000651EF0CB
  00000000651EF055: F6 42 5C 80        test        byte ptr [rdx+5Ch],80h
  00000000651EF059: 74 70              je          00000000651EF0CB
  00000000651EF05B: 48 83 C2 30        add         rdx,30h
  00000000651EF05F: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000000651EF067: E8 24 26 F8 FF     call        0000000065171690
  00000000651EF06C: 90                 nop
  00000000651EF06D: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000000651EF072: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000000651EF07A: E8 61 00 FA FF     call        000000006518F0E0
  00000000651EF07F: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000000651EF087: E8 94 90 F9 FF     call        0000000065188120
  00000000651EF08C: 84 C0              test        al,al
  00000000651EF08E: 74 0F              je          00000000651EF09F
  00000000651EF090: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000000651EF098: E8 83 A3 F9 FF     call        0000000065189420
  00000000651EF09D: EB 45              jmp         00000000651EF0E4
  00000000651EF09F: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  00000000651EF0A4: 45 33 C9           xor         r9d,r9d
  00000000651EF0A7: 4C 8B C6           mov         r8,rsi
  00000000651EF0AA: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  00000000651EF0B2: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  00000000651EF0B6: E8 35 F4 FF FF     call        00000000651EE4F0
  00000000651EF0BB: 90                 nop
  00000000651EF0BC: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000000651EF0C4: E8 57 A3 F9 FF     call        0000000065189420
  00000000651EF0C9: EB 19              jmp         00000000651EF0E4
  00000000651EF0CB: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  00000000651EF0D0: 45 33 C9           xor         r9d,r9d
  00000000651EF0D3: 4C 8B C6           mov         r8,rsi
  00000000651EF0D6: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000000651EF0DB: 48 8B 4B 10        mov         rcx,qword ptr [rbx+10h]
  00000000651EF0DF: E8 2C F8 FF FF     call        00000000651EE910
  00000000651EF0E4: 48 8B 9C 24 88 00  mov         rbx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000651EF0EC: 48 83 C4 60        add         rsp,60h
  00000000651EF0F0: 5F                 pop         rdi
  00000000651EF0F1: 5E                 pop         rsi
  00000000651EF0F2: 5D                 pop         rbp
  00000000651EF0F3: C3                 ret

  Summary

      695000 .text

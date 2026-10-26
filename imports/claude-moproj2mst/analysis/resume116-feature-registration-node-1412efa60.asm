Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001412EFA60: 4C 8B DC           mov         r11,rsp
  00000001412EFA63: 57                 push        rdi
  00000001412EFA64: 48 81 EC A0 00 00  sub         rsp,0A0h
                    00
  00000001412EFA6B: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001412EFA74: 49 89 5B 08        mov         qword ptr [r11+8],rbx
  00000001412EFA78: 49 89 6B 10        mov         qword ptr [r11+10h],rbp
  00000001412EFA7C: 49 89 73 18        mov         qword ptr [r11+18h],rsi
  00000001412EFA80: 49 8B D9           mov         rbx,r9
  00000001412EFA83: 41 0F B6 F8        movzx       edi,r8b
  00000001412EFA87: 48 8B EA           mov         rbp,rdx
  00000001412EFA8A: 48 8B F1           mov         rsi,rcx
  00000001412EFA8D: 48 B8 FE FF FF FF  mov         rax,0FFFFFFFFFFFFFFEh
                    FF FF FF 0F
  00000001412EFA97: 48 39 41 38        cmp         qword ptr [rcx+38h],rax
  00000001412EFA9B: 0F 82 8E 00 00 00  jb          00000001412EFB2F
  00000001412EFAA1: 49 C7 43 B0 0F 00  mov         qword ptr [r11-50h],0Fh
                    00 00
  00000001412EFAA9: 33 DB              xor         ebx,ebx
  00000001412EFAAB: 49 89 5B A8        mov         qword ptr [r11-58h],rbx
  00000001412EFAAF: 88 5C 24 40        mov         byte ptr [rsp+40h],bl
  00000001412EFAB3: 44 8D 43 13        lea         r8d,[rbx+13h]
  00000001412EFAB7: 48 8D 15 1A 1D 4F  lea         rdx,[00000001427E17D8h]
                    01
  00000001412EFABE: 49 8D 4B 90        lea         rcx,[r11-70h]
  00000001412EFAC2: E8 C9 0B E5 FE     call        0000000140140690
  00000001412EFAC7: 90                 nop
  00000001412EFAC8: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001412EFACD: E8 D6 0B 18 00     call        00000001414706A8
  00000001412EFAD2: 90                 nop
  00000001412EFAD3: 48 8D 05 06 C3 4D  lea         rax,[00000001427CBDE0h]
                    01
  00000001412EFADA: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001412EFADF: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0Fh
                    00 00 0F 00 00 00
  00000001412EFAEB: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  00000001412EFAF3: 88 9C 24 80 00 00  mov         byte ptr [rsp+0000000000000080h],bl
                    00
  00000001412EFAFA: 49 83 C9 FF        or          r9,0FFFFFFFFFFFFFFFFh
  00000001412EFAFE: 45 33 C0           xor         r8d,r8d
  00000001412EFB01: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001412EFB06: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001412EFB0B: E8 E0 EE DB FE     call        00000001400AE9F0
  00000001412EFB10: 90                 nop
  00000001412EFB11: 48 8D 05 80 C2 4D  lea         rax,[00000001427CBD98h]
                    01
  00000001412EFB18: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001412EFB1D: 48 8D 15 94 BB 21  lea         rdx,[000000014350B6B8h]
                    02
  00000001412EFB24: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001412EFB29: E8 2A F0 17 00     call        000000014146EB58
  00000001412EFB2E: 90                 nop
  00000001412EFB2F: 48 8B 51 30        mov         rdx,qword ptr [rcx+30h]
  00000001412EFB33: C6 44 24 28 00     mov         byte ptr [rsp+28h],0
  00000001412EFB38: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001412EFB40: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001412EFB45: 4C 8B CA           mov         r9,rdx
  00000001412EFB48: 4C 8B C3           mov         r8,rbx
  00000001412EFB4B: E8 A0 DE FF FF     call        00000001412ED9F0
  00000001412EFB50: 4C 8B D8           mov         r11,rax
  00000001412EFB53: 48 FF 46 38        inc         qword ptr [rsi+38h]
  00000001412EFB57: 48 8B 4E 30        mov         rcx,qword ptr [rsi+30h]
  00000001412EFB5B: 48 3B D9           cmp         rbx,rcx
  00000001412EFB5E: 75 15              jne         00000001412EFB75
  00000001412EFB60: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001412EFB64: 48 8B 4E 30        mov         rcx,qword ptr [rsi+30h]
  00000001412EFB68: 48 89 01           mov         qword ptr [rcx],rax
  00000001412EFB6B: 48 8B 4E 30        mov         rcx,qword ptr [rsi+30h]
  00000001412EFB6F: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001412EFB73: EB 28              jmp         00000001412EFB9D
  00000001412EFB75: 40 84 FF           test        dil,dil
  00000001412EFB78: 74 11              je          00000001412EFB8B
  00000001412EFB7A: 48 89 03           mov         qword ptr [rbx],rax
  00000001412EFB7D: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412EFB81: 48 3B 18           cmp         rbx,qword ptr [rax]
  00000001412EFB84: 75 17              jne         00000001412EFB9D
  00000001412EFB86: 4C 89 18           mov         qword ptr [rax],r11
  00000001412EFB89: EB 12              jmp         00000001412EFB9D
  00000001412EFB8B: 48 89 43 10        mov         qword ptr [rbx+10h],rax
  00000001412EFB8F: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412EFB93: 48 3B 58 10        cmp         rbx,qword ptr [rax+10h]
  00000001412EFB97: 75 04              jne         00000001412EFB9D
  00000001412EFB99: 4C 89 58 10        mov         qword ptr [rax+10h],r11
  00000001412EFB9D: 49 8B D3           mov         rdx,r11
  00000001412EFBA0: 4D 8D 43 08        lea         r8,[r11+8]
  00000001412EFBA4: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFBA7: 80 78 28 00        cmp         byte ptr [rax+28h],0
  00000001412EFBAB: 0F 85 F7 01 00 00  jne         00000001412EFDA8
  00000001412EFBB1: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    66 0F 1F 84 00 00
                    00 00 00
  00000001412EFBC0: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFBC3: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412EFBC7: 48 3B 01           cmp         rax,qword ptr [rcx]
  00000001412EFBCA: 0F 85 E7 00 00 00  jne         00000001412EFCB7
  00000001412EFBD0: 4C 8B 49 10        mov         r9,qword ptr [rcx+10h]
  00000001412EFBD4: 41 80 79 28 00     cmp         byte ptr [r9+28h],0
  00000001412EFBD9: 75 20              jne         00000001412EFBFB
  00000001412EFBDB: C6 40 28 01        mov         byte ptr [rax+28h],1
  00000001412EFBDF: 41 C6 41 28 01     mov         byte ptr [r9+28h],1
  00000001412EFBE4: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFBE7: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412EFBEB: C6 41 28 00        mov         byte ptr [rcx+28h],0
  00000001412EFBEF: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFBF2: 48 8B 50 08        mov         rdx,qword ptr [rax+8]
  00000001412EFBF6: E9 9C 01 00 00     jmp         00000001412EFD97
  00000001412EFBFB: 48 3B 50 10        cmp         rdx,qword ptr [rax+10h]
  00000001412EFBFF: 75 4D              jne         00000001412EFC4E
  00000001412EFC01: 48 8B D0           mov         rdx,rax
  00000001412EFC04: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000001412EFC08: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412EFC0B: 48 89 42 10        mov         qword ptr [rdx+10h],rax
  00000001412EFC0F: 48 8B 01           mov         rax,qword ptr [rcx]
  00000001412EFC12: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412EFC16: 75 04              jne         00000001412EFC1C
  00000001412EFC18: 48 89 50 08        mov         qword ptr [rax+8],rdx
  00000001412EFC1C: 4C 8D 42 08        lea         r8,[rdx+8]
  00000001412EFC20: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFC23: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001412EFC27: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412EFC2B: 48 3B 50 08        cmp         rdx,qword ptr [rax+8]
  00000001412EFC2F: 75 06              jne         00000001412EFC37
  00000001412EFC31: 48 89 48 08        mov         qword ptr [rax+8],rcx
  00000001412EFC35: EB 11              jmp         00000001412EFC48
  00000001412EFC37: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFC3A: 48 3B 10           cmp         rdx,qword ptr [rax]
  00000001412EFC3D: 75 05              jne         00000001412EFC44
  00000001412EFC3F: 48 89 08           mov         qword ptr [rax],rcx
  00000001412EFC42: EB 04              jmp         00000001412EFC48
  00000001412EFC44: 48 89 48 10        mov         qword ptr [rax+10h],rcx
  00000001412EFC48: 48 89 11           mov         qword ptr [rcx],rdx
  00000001412EFC4B: 49 89 08           mov         qword ptr [r8],rcx
  00000001412EFC4E: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFC51: C6 40 28 01        mov         byte ptr [rax+28h],1
  00000001412EFC55: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFC58: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412EFC5C: C6 41 28 00        mov         byte ptr [rcx+28h],0
  00000001412EFC60: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFC63: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412EFC67: 4C 8B 01           mov         r8,qword ptr [rcx]
  00000001412EFC6A: 49 8B 40 10        mov         rax,qword ptr [r8+10h]
  00000001412EFC6E: 48 89 01           mov         qword ptr [rcx],rax
  00000001412EFC71: 49 8B 40 10        mov         rax,qword ptr [r8+10h]
  00000001412EFC75: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412EFC79: 75 04              jne         00000001412EFC7F
  00000001412EFC7B: 48 89 48 08        mov         qword ptr [rax+8],rcx
  00000001412EFC7F: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000001412EFC83: 49 89 40 08        mov         qword ptr [r8+8],rax
  00000001412EFC87: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412EFC8B: 48 3B 48 08        cmp         rcx,qword ptr [rax+8]
  00000001412EFC8F: 75 06              jne         00000001412EFC97
  00000001412EFC91: 4C 89 40 08        mov         qword ptr [rax+8],r8
  00000001412EFC95: EB 13              jmp         00000001412EFCAA
  00000001412EFC97: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000001412EFC9B: 48 3B 48 10        cmp         rcx,qword ptr [rax+10h]
  00000001412EFC9F: 75 06              jne         00000001412EFCA7
  00000001412EFCA1: 4C 89 40 10        mov         qword ptr [rax+10h],r8
  00000001412EFCA5: EB 03              jmp         00000001412EFCAA
  00000001412EFCA7: 4C 89 00           mov         qword ptr [rax],r8
  00000001412EFCAA: 49 89 48 10        mov         qword ptr [r8+10h],rcx
  00000001412EFCAE: 4C 89 41 08        mov         qword ptr [rcx+8],r8
  00000001412EFCB2: E9 E0 00 00 00     jmp         00000001412EFD97
  00000001412EFCB7: 4C 8B 09           mov         r9,qword ptr [rcx]
  00000001412EFCBA: 41 80 79 28 00     cmp         byte ptr [r9+28h],0
  00000001412EFCBF: 75 20              jne         00000001412EFCE1
  00000001412EFCC1: C6 40 28 01        mov         byte ptr [rax+28h],1
  00000001412EFCC5: 41 C6 41 28 01     mov         byte ptr [r9+28h],1
  00000001412EFCCA: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFCCD: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412EFCD1: C6 41 28 00        mov         byte ptr [rcx+28h],0
  00000001412EFCD5: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFCD8: 48 8B 50 08        mov         rdx,qword ptr [rax+8]
  00000001412EFCDC: E9 B6 00 00 00     jmp         00000001412EFD97
  00000001412EFCE1: 48 3B 10           cmp         rdx,qword ptr [rax]
  00000001412EFCE4: 75 4F              jne         00000001412EFD35
  00000001412EFCE6: 48 8B D0           mov         rdx,rax
  00000001412EFCE9: 48 8B 08           mov         rcx,qword ptr [rax]
  00000001412EFCEC: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000001412EFCF0: 48 89 02           mov         qword ptr [rdx],rax
  00000001412EFCF3: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  00000001412EFCF7: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412EFCFB: 75 04              jne         00000001412EFD01
  00000001412EFCFD: 48 89 50 08        mov         qword ptr [rax+8],rdx
  00000001412EFD01: 4C 8D 42 08        lea         r8,[rdx+8]
  00000001412EFD05: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFD08: 48 89 41 08        mov         qword ptr [rcx+8],rax
  00000001412EFD0C: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412EFD10: 48 3B 50 08        cmp         rdx,qword ptr [rax+8]
  00000001412EFD14: 75 06              jne         00000001412EFD1C
  00000001412EFD16: 48 89 48 08        mov         qword ptr [rax+8],rcx
  00000001412EFD1A: EB 12              jmp         00000001412EFD2E
  00000001412EFD1C: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFD1F: 48 3B 50 10        cmp         rdx,qword ptr [rax+10h]
  00000001412EFD23: 75 06              jne         00000001412EFD2B
  00000001412EFD25: 48 89 48 10        mov         qword ptr [rax+10h],rcx
  00000001412EFD29: EB 03              jmp         00000001412EFD2E
  00000001412EFD2B: 48 89 08           mov         qword ptr [rax],rcx
  00000001412EFD2E: 48 89 51 10        mov         qword ptr [rcx+10h],rdx
  00000001412EFD32: 49 89 08           mov         qword ptr [r8],rcx
  00000001412EFD35: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFD38: C6 40 28 01        mov         byte ptr [rax+28h],1
  00000001412EFD3C: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFD3F: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412EFD43: C6 41 28 00        mov         byte ptr [rcx+28h],0
  00000001412EFD47: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFD4A: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412EFD4E: 4C 8B 41 10        mov         r8,qword ptr [rcx+10h]
  00000001412EFD52: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFD55: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001412EFD59: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFD5C: 80 78 29 00        cmp         byte ptr [rax+29h],0
  00000001412EFD60: 75 04              jne         00000001412EFD66
  00000001412EFD62: 48 89 48 08        mov         qword ptr [rax+8],rcx
  00000001412EFD66: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000001412EFD6A: 49 89 40 08        mov         qword ptr [r8+8],rax
  00000001412EFD6E: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412EFD72: 48 3B 48 08        cmp         rcx,qword ptr [rax+8]
  00000001412EFD76: 75 06              jne         00000001412EFD7E
  00000001412EFD78: 4C 89 40 08        mov         qword ptr [rax+8],r8
  00000001412EFD7C: EB 12              jmp         00000001412EFD90
  00000001412EFD7E: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  00000001412EFD82: 48 3B 08           cmp         rcx,qword ptr [rax]
  00000001412EFD85: 75 05              jne         00000001412EFD8C
  00000001412EFD87: 4C 89 00           mov         qword ptr [rax],r8
  00000001412EFD8A: EB 04              jmp         00000001412EFD90
  00000001412EFD8C: 4C 89 40 10        mov         qword ptr [rax+10h],r8
  00000001412EFD90: 49 89 08           mov         qword ptr [r8],rcx
  00000001412EFD93: 4C 89 41 08        mov         qword ptr [rcx+8],r8
  00000001412EFD97: 4C 8D 42 08        lea         r8,[rdx+8]
  00000001412EFD9B: 49 8B 00           mov         rax,qword ptr [r8]
  00000001412EFD9E: 80 78 28 00        cmp         byte ptr [rax+28h],0
  00000001412EFDA2: 0F 84 18 FE FF FF  je          00000001412EFBC0
  00000001412EFDA8: 48 8B 46 30        mov         rax,qword ptr [rsi+30h]
  00000001412EFDAC: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  00000001412EFDB0: C6 41 28 01        mov         byte ptr [rcx+28h],1
  00000001412EFDB4: 4C 89 5D 08        mov         qword ptr [rbp+8],r11
  00000001412EFDB8: 48 8B 06           mov         rax,qword ptr [rsi]
  00000001412EFDBB: 48 89 45 00        mov         qword ptr [rbp],rax
  00000001412EFDBF: 48 8B C5           mov         rax,rbp
  00000001412EFDC2: 4C 8D 9C 24 A0 00  lea         r11,[rsp+00000000000000A0h]
                    00 00
  00000001412EFDCA: 49 8B 5B 10        mov         rbx,qword ptr [r11+10h]
  00000001412EFDCE: 49 8B 6B 18        mov         rbp,qword ptr [r11+18h]
  00000001412EFDD2: 49 8B 73 20        mov         rsi,qword ptr [r11+20h]
  00000001412EFDD6: 49 8B E3           mov         rsp,r11
  00000001412EFDD9: 5F                 pop         rdi
  00000001412EFDDA: C3                 ret

  Summary

     1E02000 .text

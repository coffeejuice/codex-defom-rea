Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711AE90: 40 55              push        rbp
  000000006711AE92: 56                 push        rsi
  000000006711AE93: 57                 push        rdi
  000000006711AE94: 41 54              push        r12
  000000006711AE96: 41 55              push        r13
  000000006711AE98: 48 83 EC 40        sub         rsp,40h
  000000006711AE9C: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711AEA5: 48 89 5C 24 78     mov         qword ptr [rsp+78h],rbx
  000000006711AEAA: 45 8B E8           mov         r13d,r8d
  000000006711AEAD: 48 8B F2           mov         rsi,rdx
  000000006711AEB0: 4C 8B E1           mov         r12,rcx
  000000006711AEB3: 48 85 C9           test        rcx,rcx
  000000006711AEB6: 75 27              jne         000000006711AEDF
  000000006711AEB8: 48 8D 0D B9 F0 11  lea         rcx,[0000000067239F78h]
                    00
  000000006711AEBF: E8 1C 20 F0 FF     call        000000006701CEE0
  000000006711AEC4: 48 85 F6           test        rsi,rsi
  000000006711AEC7: 0F 84 5B 01 00 00  je          000000006711B028
  000000006711AECD: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006711AED0: 41 8D 54 24 01     lea         edx,[r12+1]
  000000006711AED5: 48 8B CE           mov         rcx,rsi
  000000006711AED8: FF 10              call        qword ptr [rax]
  000000006711AEDA: E9 49 01 00 00     jmp         000000006711B028
  000000006711AEDF: 48 8B 69 08        mov         rbp,qword ptr [rcx+8]
  000000006711AEE3: 48 8B 7D 40        mov         rdi,qword ptr [rbp+40h]
  000000006711AEE7: 48 85 FF           test        rdi,rdi
  000000006711AEEA: 75 19              jne         000000006711AF05
  000000006711AEEC: 48 85 D2           test        rdx,rdx
  000000006711AEEF: 0F 84 33 01 00 00  je          000000006711B028
  000000006711AEF5: 48 8B 02           mov         rax,qword ptr [rdx]
  000000006711AEF8: 8D 57 01           lea         edx,[rdi+1]
  000000006711AEFB: 48 8B CE           mov         rcx,rsi
  000000006711AEFE: FF 10              call        qword ptr [rax]
  000000006711AF00: E9 23 01 00 00     jmp         000000006711B028
  000000006711AF05: 48 8D 5F 48        lea         rbx,[rdi+48h]
  000000006711AF09: 48 8B CB           mov         rcx,rbx
  000000006711AF0C: E8 9F AE F0 FF     call        0000000067025DB0
  000000006711AF11: 4C 8B 5D 40        mov         r11,qword ptr [rbp+40h]
  000000006711AF15: 49 3B FB           cmp         rdi,r11
  000000006711AF18: 74 49              je          000000006711AF63
  000000006711AF1A: 66 0F 1F 44 00 00  nop         word ptr [rax+rax]
  000000006711AF20: 48 8B CB           mov         rcx,rbx
  000000006711AF23: E8 08 AC F0 FF     call        0000000067025B30
  000000006711AF28: 48 8B 7D 40        mov         rdi,qword ptr [rbp+40h]
  000000006711AF2C: 48 85 FF           test        rdi,rdi
  000000006711AF2F: 74 17              je          000000006711AF48
  000000006711AF31: 48 8D 5F 48        lea         rbx,[rdi+48h]
  000000006711AF35: 48 8B CB           mov         rcx,rbx
  000000006711AF38: E8 73 AE F0 FF     call        0000000067025DB0
  000000006711AF3D: 4C 8B 5D 40        mov         r11,qword ptr [rbp+40h]
  000000006711AF41: 49 3B FB           cmp         rdi,r11
  000000006711AF44: 75 DA              jne         000000006711AF20
  000000006711AF46: EB 1B              jmp         000000006711AF63
  000000006711AF48: 48 85 F6           test        rsi,rsi
  000000006711AF4B: 0F 84 D7 00 00 00  je          000000006711B028
  000000006711AF51: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006711AF54: BA 01 00 00 00     mov         edx,1
  000000006711AF59: 48 8B CE           mov         rcx,rsi
  000000006711AF5C: FF 10              call        qword ptr [rax]
  000000006711AF5E: E9 C5 00 00 00     jmp         000000006711B028
  000000006711AF63: 48 8D 5F 48        lea         rbx,[rdi+48h]
  000000006711AF67: 48 89 9C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rbx
                    00 00
  000000006711AF6F: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  000000006711AF74: 83 78 24 00        cmp         dword ptr [rax+24h],0
  000000006711AF78: 74 33              je          000000006711AFAD
  000000006711AF7A: 48 8B 0D 9F 92 1D  mov         rcx,qword ptr [00000000672F4220h]
                    00
  000000006711AF81: 48 85 C9           test        rcx,rcx
  000000006711AF84: 74 27              je          000000006711AFAD
  000000006711AF86: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006711AF89: 4C 8D 4F 30        lea         r9,[rdi+30h]
  000000006711AF8D: 4D 8B C4           mov         r8,r12
  000000006711AF90: 48 8B D6           mov         rdx,rsi
  000000006711AF93: FF 50 68           call        qword ptr [rax+68h]
  000000006711AF96: 84 C0              test        al,al
  000000006711AF98: 74 13              je          000000006711AFAD
  000000006711AF9A: 48 85 DB           test        rbx,rbx
  000000006711AF9D: 0F 84 85 00 00 00  je          000000006711B028
  000000006711AFA3: 48 8B CB           mov         rcx,rbx
  000000006711AFA6: E8 85 AB F0 FF     call        0000000067025B30
  000000006711AFAB: EB 7B              jmp         000000006711B028
  000000006711AFAD: 66 83 7E 10 34     cmp         word ptr [rsi+10h],34h
  000000006711AFB2: 75 12              jne         000000006711AFC6
  000000006711AFB4: E8 E7 FC F0 FF     call        000000006702ACA0
  000000006711AFB9: 48 3B F8           cmp         rdi,rax
  000000006711AFBC: 75 08              jne         000000006711AFC6
  000000006711AFBE: 48 63 47 1C        movsxd      rax,dword ptr [rdi+1Ch]
  000000006711AFC2: 48 89 46 08        mov         qword ptr [rsi+8],rax
  000000006711AFC6: 48 89 74 24 70     mov         qword ptr [rsp+70h],rsi
  000000006711AFCB: 4C 89 64 24 28     mov         qword ptr [rsp+28h],r12
  000000006711AFD0: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  000000006711AFD5: 44 89 6C 24 38     mov         dword ptr [rsp+38h],r13d
  000000006711AFDA: 48 8D 4F 30        lea         rcx,[rdi+30h]
  000000006711AFDE: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006711AFE3: E8 E8 FC FF FF     call        000000006711ACD0
  000000006711AFE8: 33 ED              xor         ebp,ebp
  000000006711AFEA: 48 89 6C 24 70     mov         qword ptr [rsp+70h],rbp
  000000006711AFEF: 66 83 4E 12 01     or          word ptr [rsi+12h],1
  000000006711AFF4: 49 8B 44 24 08     mov         rax,qword ptr [r12+8]
  000000006711AFF9: FF 40 24           inc         dword ptr [rax+24h]
  000000006711AFFC: 40 88 6F 50        mov         byte ptr [rdi+50h],bpl
  000000006711B000: 48 85 DB           test        rbx,rbx
  000000006711B003: 74 08              je          000000006711B00D
  000000006711B005: 48 8B CB           mov         rcx,rbx
  000000006711B008: E8 23 AB F0 FF     call        0000000067025B30
  000000006711B00D: 48 89 AC 24 88 00  mov         qword ptr [rsp+0000000000000088h],rbp
                    00 00
  000000006711B015: 48 8B 4F 20        mov         rcx,qword ptr [rdi+20h]
  000000006711B019: 48 85 C9           test        rcx,rcx
  000000006711B01C: 74 0A              je          000000006711B028
  000000006711B01E: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006711B021: FF 90 98 00 00 00  call        qword ptr [rax+0000000000000098h]
  000000006711B027: 90                 nop
  000000006711B028: 48 8B 5C 24 78     mov         rbx,qword ptr [rsp+78h]
  000000006711B02D: 48 83 C4 40        add         rsp,40h
  000000006711B031: 41 5D              pop         r13
  000000006711B033: 41 5C              pop         r12
  000000006711B035: 5F                 pop         rdi
  000000006711B036: 5E                 pop         rsi
  000000006711B037: 5D                 pop         rbp
  000000006711B038: C3                 ret

  Summary

      1A7000 .text

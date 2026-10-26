Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006714BC20: 40 57              push        rdi
  000000006714BC22: 48 83 EC 30        sub         rsp,30h
  000000006714BC26: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006714BC2F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006714BC34: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  000000006714BC39: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  000000006714BC3E: 48 8B F9           mov         rdi,rcx
  000000006714BC41: E8 FA FE FF FF     call        000000006714BB40
  000000006714BC46: 48 8B D8           mov         rbx,rax
  000000006714BC49: 33 F6              xor         esi,esi
  000000006714BC4B: 8D 6E 01           lea         ebp,[rsi+1]
  000000006714BC4E: 48 85 C0           test        rax,rax
  000000006714BC51: 74 32              je          000000006714BC85
  000000006714BC53: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006714BC56: 40 84 69 04        test        byte ptr [rcx+4],bpl
  000000006714BC5A: 74 0A              je          000000006714BC66
  000000006714BC5C: 48 8B C8           mov         rcx,rax
  000000006714BC5F: E8 4C A1 ED FF     call        0000000067025DB0
  000000006714BC64: EB 15              jmp         000000006714BC7B
  000000006714BC66: 33 C0              xor         eax,eax
  000000006714BC68: F0 0F B1 29        lock cmpxchg dword ptr [rcx],ebp
  000000006714BC6C: 0F 94 C1           sete        cl
  000000006714BC6F: 84 C9              test        cl,cl
  000000006714BC71: 75 08              jne         000000006714BC7B
  000000006714BC73: 48 8B CB           mov         rcx,rbx
  000000006714BC76: E8 35 9F ED FF     call        0000000067025BB0
  000000006714BC7B: 48 0B DD           or          rbx,rbp
  000000006714BC7E: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006714BC83: EB 08              jmp         000000006714BC8D
  000000006714BC85: 48 8B DE           mov         rbx,rsi
  000000006714BC88: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006714BC8D: 48 89 3D A4 87 1A  mov         qword ptr [00000000672F4438h],rdi
                    00
  000000006714BC94: 48 85 FF           test        rdi,rdi
  000000006714BC97: 75 13              jne         000000006714BCAC
  000000006714BC99: 48 8D 0D 98 D8 0D  lea         rcx,[0000000067229538h]
                    00
  000000006714BCA0: E8 0B 63 F7 FF     call        00000000670C1FB0
  000000006714BCA5: 48 89 05 8C 87 1A  mov         qword ptr [00000000672F4438h],rax
                    00
  000000006714BCAC: F6 C3 01           test        bl,1
  000000006714BCAF: 74 30              je          000000006714BCE1
  000000006714BCB1: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006714BCB5: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  000000006714BCBA: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006714BCBE: 48 8B 0B           mov         rcx,qword ptr [rbx]
  000000006714BCC1: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006714BCC5: 74 0A              je          000000006714BCD1
  000000006714BCC7: 48 8B CB           mov         rcx,rbx
  000000006714BCCA: E8 61 9E ED FF     call        0000000067025B30
  000000006714BCCF: EB 10              jmp         000000006714BCE1
  000000006714BCD1: 8B C5              mov         eax,ebp
  000000006714BCD3: F0 0F B1 31        lock cmpxchg dword ptr [rcx],esi
  000000006714BCD7: 74 08              je          000000006714BCE1
  000000006714BCD9: 48 8B CB           mov         rcx,rbx
  000000006714BCDC: E8 8F 9E ED FF     call        0000000067025B70
  000000006714BCE1: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006714BCE6: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  000000006714BCEB: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  000000006714BCF0: 48 83 C4 30        add         rsp,30h
  000000006714BCF4: 5F                 pop         rdi
  000000006714BCF5: C3                 ret

  Summary

      1A7000 .text

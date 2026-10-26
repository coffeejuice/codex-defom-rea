Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670701A0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000000670701A4: 55                 push        rbp
  00000000670701A5: 56                 push        rsi
  00000000670701A6: 57                 push        rdi
  00000000670701A7: 41 54              push        r12
  00000000670701A9: 41 55              push        r13
  00000000670701AB: 48 83 EC 30        sub         rsp,30h
  00000000670701AF: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670701B8: 48 89 5C 24 78     mov         qword ptr [rsp+78h],rbx
  00000000670701BD: 48 8B F9           mov         rdi,rcx
  00000000670701C0: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670701C3: 8B 10              mov         edx,dword ptr [rax]
  00000000670701C5: 83 FA 01           cmp         edx,1
  00000000670701C8: 0F 85 EE 00 00 00  jne         00000000670702BC
  00000000670701CE: 48 8B 09           mov         rcx,qword ptr [rcx]
  00000000670701D1: 48 8D 41 1A        lea         rax,[rcx+1Ah]
  00000000670701D5: 48 39 41 10        cmp         qword ptr [rcx+10h],rax
  00000000670701D9: 0F 85 DD 00 00 00  jne         00000000670702BC
  00000000670701DF: F6 41 18 08        test        byte ptr [rcx+18h],8
  00000000670701E3: 0F 84 9E 00 00 00  je          0000000067070287
  00000000670701E9: E8 42 FA FF FF     call        000000006706FC30
  00000000670701EE: 48 8B D8           mov         rbx,rax
  00000000670701F1: 33 ED              xor         ebp,ebp
  00000000670701F3: 44 8D 65 01        lea         r12d,[rbp+1]
  00000000670701F7: 48 85 C0           test        rax,rax
  00000000670701FA: 74 33              je          000000006707022F
  00000000670701FC: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000670701FF: 44 84 61 04        test        byte ptr [rcx+4],r12b
  0000000067070203: 74 0A              je          000000006707020F
  0000000067070205: 48 8B C8           mov         rcx,rax
  0000000067070208: E8 A3 5B FB FF     call        0000000067025DB0
  000000006707020D: EB 16              jmp         0000000067070225
  000000006707020F: 33 C0              xor         eax,eax
  0000000067070211: F0 44 0F B1 21     lock cmpxchg dword ptr [rcx],r12d
  0000000067070216: 0F 94 C1           sete        cl
  0000000067070219: 84 C9              test        cl,cl
  000000006707021B: 75 08              jne         0000000067070225
  000000006707021D: 48 8B CB           mov         rcx,rbx
  0000000067070220: E8 8B 59 FB FF     call        0000000067025BB0
  0000000067070225: 49 0B DC           or          rbx,r12
  0000000067070228: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  000000006707022D: EB 08              jmp         0000000067070237
  000000006707022F: 48 8B DD           mov         rbx,rbp
  0000000067070232: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  0000000067070237: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006707023A: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000006707023F: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000067070244: 48 8B 0D CD 3B 28  mov         rcx,qword ptr [00000000672F3E18h]
                    00
  000000006707024B: E8 10 FC FF FF     call        000000006706FE60
  0000000067070250: 90                 nop
  0000000067070251: F6 C3 01           test        bl,1
  0000000067070254: 74 31              je          0000000067070287
  0000000067070256: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  000000006707025A: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  000000006707025F: 48 83 E3 FE        and         rbx,0FFFFFFFFFFFFFFFEh
  0000000067070263: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067070266: F6 41 04 01        test        byte ptr [rcx+4],1
  000000006707026A: 74 0A              je          0000000067070276
  000000006707026C: 48 8B CB           mov         rcx,rbx
  000000006707026F: E8 BC 58 FB FF     call        0000000067025B30
  0000000067070274: EB 11              jmp         0000000067070287
  0000000067070276: 41 8B C4           mov         eax,r12d
  0000000067070279: F0 0F B1 29        lock cmpxchg dword ptr [rcx],ebp
  000000006707027D: 74 08              je          0000000067070287
  000000006707027F: 48 8B CB           mov         rcx,rbx
  0000000067070282: E8 E9 58 FB FF     call        0000000067025B70
  0000000067070287: 48 63 74 24 68     movsxd      rsi,dword ptr [rsp+68h]
  000000006707028C: 48 8D 54 36 20     lea         rdx,[rsi+rsi+20h]
  0000000067070291: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000067070294: E8 27 01 FB FF     call        00000000670203C0
  0000000067070299: 48 8B D8           mov         rbx,rax
  000000006707029C: 48 85 C0           test        rax,rax
  000000006707029F: 75 05              jne         00000000670702A6
  00000000670702A1: E8 0A BD FA FF     call        000000006701BFB0
  00000000670702A6: 48 89 1F           mov         qword ptr [rdi],rbx
  00000000670702A9: 89 73 04           mov         dword ptr [rbx+4],esi
  00000000670702AC: 48 8B 0F           mov         rcx,qword ptr [rdi]
  00000000670702AF: 48 8D 41 1A        lea         rax,[rcx+1Ah]
  00000000670702B3: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000000670702B7: E9 E5 00 00 00     jmp         00000000670703A1
  00000000670702BC: 4C 63 6C 24 68     movsxd      r13,dword ptr [rsp+68h]
  00000000670702C1: 4B 8D 4C 2D 20     lea         rcx,[r13+r13+20h]
  00000000670702C6: E8 D5 00 FB FF     call        00000000670203A0
  00000000670702CB: 48 8B F0           mov         rsi,rax
  00000000670702CE: 48 85 C0           test        rax,rax
  00000000670702D1: 75 05              jne         00000000670702D8
  00000000670702D3: E8 D8 BC FA FF     call        000000006701BFB0
  00000000670702D8: 48 8B 17           mov         rdx,qword ptr [rdi]
  00000000670702DB: 48 83 C2 08        add         rdx,8
  00000000670702DF: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000000670702E4: 44 3B 2A           cmp         r13d,dword ptr [rdx]
  00000000670702E7: 48 0F 4D CA        cmovge      rcx,rdx
  00000000670702EB: 48 63 11           movsxd      rdx,dword ptr [rcx]
  00000000670702EE: 89 56 08           mov         dword ptr [rsi+8],edx
  00000000670702F1: 48 8D 5E 1A        lea         rbx,[rsi+1Ah]
  00000000670702F5: 4C 8B C2           mov         r8,rdx
  00000000670702F8: 4D 03 C0           add         r8,r8
  00000000670702FB: 48 8B 17           mov         rdx,qword ptr [rdi]
  00000000670702FE: 48 8B 52 10        mov         rdx,qword ptr [rdx+10h]
  0000000067070302: 48 8B CB           mov         rcx,rbx
  0000000067070305: E8 94 87 10 00     call        0000000067178A9E
  000000006707030A: 4C 63 5E 08        movsxd      r11,dword ptr [rsi+8]
  000000006707030E: 33 ED              xor         ebp,ebp
  0000000067070310: 66 42 89 6C 5E 1A  mov         word ptr [rsi+r11*2+1Ah],bp
  0000000067070316: B8 F7 FF 00 00     mov         eax,0FFF7h
  000000006707031B: 66 21 46 18        and         word ptr [rsi+18h],ax
  000000006707031F: 41 BC 01 00 00 00  mov         r12d,1
  0000000067070325: 44 89 26           mov         dword ptr [rsi],r12d
  0000000067070328: 44 89 6E 04        mov         dword ptr [rsi+4],r13d
  000000006707032C: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006707032F: 0F B7 48 18        movzx       ecx,word ptr [rax+18h]
  0000000067070333: 66 33 4E 18        xor         cx,word ptr [rsi+18h]
  0000000067070337: 66 41 23 CC        and         cx,r12w
  000000006707033B: 66 31 4E 18        xor         word ptr [rsi+18h],cx
  000000006707033F: 0F B7 4E 18        movzx       ecx,word ptr [rsi+18h]
  0000000067070343: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067070346: 0F B7 50 18        movzx       edx,word ptr [rax+18h]
  000000006707034A: 66 33 D1           xor         dx,cx
  000000006707034D: 66 83 E2 02        and         dx,2
  0000000067070351: 66 33 D1           xor         dx,cx
  0000000067070354: 66 89 56 18        mov         word ptr [rsi+18h],dx
  0000000067070358: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006707035B: 44 0F B7 40 18     movzx       r8d,word ptr [rax+18h]
  0000000067070360: 66 44 33 C2        xor         r8w,dx
  0000000067070364: 66 41 83 E0 04     and         r8w,4
  0000000067070369: 66 44 33 C2        xor         r8w,dx
  000000006707036D: 66 44 89 46 18     mov         word ptr [rsi+18h],r8w
  0000000067070372: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000067070375: 0F B7 48 18        movzx       ecx,word ptr [rax+18h]
  0000000067070379: 66 41 33 C8        xor         cx,r8w
  000000006707037D: 66 83 E1 10        and         cx,10h
  0000000067070381: 66 41 33 C8        xor         cx,r8w
  0000000067070385: 66 89 4E 18        mov         word ptr [rsi+18h],cx
  0000000067070389: 48 89 5E 10        mov         qword ptr [rsi+10h],rbx
  000000006707038D: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000067070390: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  0000000067070394: 75 08              jne         000000006707039E
  0000000067070396: 48 8B 0F           mov         rcx,qword ptr [rdi]
  0000000067070399: E8 32 FD FF FF     call        00000000670700D0
  000000006707039E: 48 89 37           mov         qword ptr [rdi],rsi
  00000000670703A1: 48 8B 5C 24 78     mov         rbx,qword ptr [rsp+78h]
  00000000670703A6: 48 83 C4 30        add         rsp,30h
  00000000670703AA: 41 5D              pop         r13
  00000000670703AC: 41 5C              pop         r12
  00000000670703AE: 5F                 pop         rdi
  00000000670703AF: 5E                 pop         rsi
  00000000670703B0: 5D                 pop         rbp
  00000000670703B1: C3                 ret

  Summary

      1A7000 .text

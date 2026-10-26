Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141482374: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000141482379: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  000000014148237E: 57                 push        rdi
  000000014148237F: 48 83 EC 30        sub         rsp,30h
  0000000141482383: 33 DB              xor         ebx,ebx
  0000000141482385: 8D 4B 01           lea         ecx,[rbx+1]
  0000000141482388: E8 4F D8 FF FF     call        000000014147FBDC
  000000014148238D: 90                 nop
  000000014148238E: 33 FF              xor         edi,edi
  0000000141482390: 89 7C 24 20        mov         dword ptr [rsp+20h],edi
  0000000141482394: 3B 3D A6 2F BD 03  cmp         edi,dword ptr [0000000145055340h]
  000000014148239A: 0F 8D FA 00 00 00  jge         000000014148249A
  00000001414823A0: 48 63 F7           movsxd      rsi,edi
  00000001414823A3: 48 8B 05 86 1F BD  mov         rax,qword ptr [0000000145054330h]
                    03
  00000001414823AA: 48 83 3C F0 00     cmp         qword ptr [rax+rsi*8],0
  00000001414823AF: 74 6B              je          000000014148241C
  00000001414823B1: 48 8B 04 F0        mov         rax,qword ptr [rax+rsi*8]
  00000001414823B5: F6 40 18 83        test        byte ptr [rax+18h],83h
  00000001414823B9: 75 56              jne         0000000141482411
  00000001414823BB: 0F BA 60 18 0F     bt          dword ptr [rax+18h],0Fh
  00000001414823C0: 72 4F              jb          0000000141482411
  00000001414823C2: 8D 47 FD           lea         eax,[rdi-3]
  00000001414823C5: 83 F8 10           cmp         eax,10h
  00000001414823C8: 77 10              ja          00000001414823DA
  00000001414823CA: 8D 4F 10           lea         ecx,[rdi+10h]
  00000001414823CD: E8 22 D7 FF FF     call        000000014147FAF4
  00000001414823D2: 85 C0              test        eax,eax
  00000001414823D4: 0F 84 C0 00 00 00  je          000000014148249A
  00000001414823DA: 48 8B 15 4F 1F BD  mov         rdx,qword ptr [0000000145054330h]
                    03
  00000001414823E1: 48 8B 14 F2        mov         rdx,qword ptr [rdx+rsi*8]
  00000001414823E5: 8B CF              mov         ecx,edi
  00000001414823E7: E8 0C B9 FE FF     call        000000014146DCF8
  00000001414823EC: 4C 8B 1D 3D 1F BD  mov         r11,qword ptr [0000000145054330h]
                    03
  00000001414823F3: 49 8B 04 F3        mov         rax,qword ptr [r11+rsi*8]
  00000001414823F7: F6 40 18 83        test        byte ptr [rax+18h],83h
  00000001414823FB: 74 0C              je          0000000141482409
  00000001414823FD: 48 8B D0           mov         rdx,rax
  0000000141482400: 8B CF              mov         ecx,edi
  0000000141482402: E8 79 B9 FE FF     call        000000014146DD80
  0000000141482407: EB 08              jmp         0000000141482411
  0000000141482409: 48 8B D8           mov         rbx,rax
  000000014148240C: E9 89 00 00 00     jmp         000000014148249A
  0000000141482411: FF C7              inc         edi
  0000000141482413: 89 7C 24 20        mov         dword ptr [rsp+20h],edi
  0000000141482417: E9 78 FF FF FF     jmp         0000000141482394
  000000014148241C: 48 63 FF           movsxd      rdi,edi
  000000014148241F: B9 58 00 00 00     mov         ecx,58h
  0000000141482424: E8 AB BF FF FF     call        000000014147E3D4
  0000000141482429: 48 8B 0D 00 1F BD  mov         rcx,qword ptr [0000000145054330h]
                    03
  0000000141482430: 48 89 04 F9        mov         qword ptr [rcx+rdi*8],rax
  0000000141482434: 48 8B 0D F5 1E BD  mov         rcx,qword ptr [0000000145054330h]
                    03
  000000014148243B: 48 83 3C F9 00     cmp         qword ptr [rcx+rdi*8],0
  0000000141482440: 74 58              je          000000014148249A
  0000000141482442: 48 8B 0C F9        mov         rcx,qword ptr [rcx+rdi*8]
  0000000141482446: 48 83 C1 30        add         rcx,30h
  000000014148244A: BA A0 0F 00 00     mov         edx,0FA0h
  000000014148244F: E8 5C F8 FF FF     call        0000000141481CB0
  0000000141482454: 85 C0              test        eax,eax
  0000000141482456: 75 1E              jne         0000000141482476
  0000000141482458: 48 8B 0D D1 1E BD  mov         rcx,qword ptr [0000000145054330h]
                    03
  000000014148245F: 48 8B 0C F9        mov         rcx,qword ptr [rcx+rdi*8]
  0000000141482463: E8 14 F7 FE FF     call        0000000141471B7C
  0000000141482468: 4C 8B 1D C1 1E BD  mov         r11,qword ptr [0000000145054330h]
                    03
  000000014148246F: 49 83 24 FB 00     and         qword ptr [r11+rdi*8],0
  0000000141482474: EB 24              jmp         000000014148249A
  0000000141482476: 48 8B 05 B3 1E BD  mov         rax,qword ptr [0000000145054330h]
                    03
  000000014148247D: 48 8B 0C F8        mov         rcx,qword ptr [rax+rdi*8]
  0000000141482481: 48 83 C1 30        add         rcx,30h
  0000000141482485: FF 15 CD 0C 98 00  call        qword ptr [0000000141E03158h]
  000000014148248B: 4C 8B 1D 9E 1E BD  mov         r11,qword ptr [0000000145054330h]
                    03
  0000000141482492: 49 8B 1C FB        mov         rbx,qword ptr [r11+rdi*8]
  0000000141482496: 83 63 18 00        and         dword ptr [rbx+18h],0
  000000014148249A: 48 85 DB           test        rbx,rbx
  000000014148249D: 74 1D              je          00000001414824BC
  000000014148249F: 81 63 18 00 80 00  and         dword ptr [rbx+18h],8000h
                    00
  00000001414824A6: 83 63 08 00        and         dword ptr [rbx+8],0
  00000001414824AA: 48 83 63 10 00     and         qword ptr [rbx+10h],0
  00000001414824AF: 48 83 23 00        and         qword ptr [rbx],0
  00000001414824B3: 48 83 63 28 00     and         qword ptr [rbx+28h],0
  00000001414824B8: 83 4B 1C FF        or          dword ptr [rbx+1Ch],0FFFFFFFFh
  00000001414824BC: B9 01 00 00 00     mov         ecx,1
  00000001414824C1: E8 16 D6 FF FF     call        000000014147FADC
  00000001414824C6: 48 8B C3           mov         rax,rbx
  00000001414824C9: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000001414824CE: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  00000001414824D3: 48 83 C4 30        add         rsp,30h
  00000001414824D7: 5F                 pop         rdi
  00000001414824D8: C3                 ret

  Summary

     1E02000 .text

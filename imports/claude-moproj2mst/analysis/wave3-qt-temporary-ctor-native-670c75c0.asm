Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C75C0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670C75C5: 57                 push        rdi
  00000000670C75C6: 48 83 EC 40        sub         rsp,40h
  00000000670C75CA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670C75D3: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000670C75D8: 48 89 6C 24 60     mov         qword ptr [rsp+60h],rbp
  00000000670C75DD: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  00000000670C75E2: 41 0F B6 D8        movzx       ebx,r8b
  00000000670C75E6: 48 8B EA           mov         rbp,rdx
  00000000670C75E9: 48 8B F1           mov         rsi,rcx
  00000000670C75EC: E8 EF 36 02 00     call        00000000670EACE0
  00000000670C75F1: 90                 nop
  00000000670C75F2: 48 8D 05 7F 53 16  lea         rax,[000000006722C978h]
                    00
  00000000670C75F9: 48 89 06           mov         qword ptr [rsi],rax
  00000000670C75FC: 88 5E 10           mov         byte ptr [rsi+10h],bl
  00000000670C75FF: 48 8B 5E 08        mov         rbx,qword ptr [rsi+8]
  00000000670C7603: 48 8B D5           mov         rdx,rbp
  00000000670C7606: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670C760B: E8 00 77 02 00     call        00000000670EED10
  00000000670C7610: 48 8B F8           mov         rdi,rax
  00000000670C7613: 48 8B D0           mov         rdx,rax
  00000000670C7616: 48 8D 4B 20        lea         rcx,[rbx+20h]
  00000000670C761A: E8 B1 8D FA FF     call        00000000670703D0
  00000000670C761F: 48 8D 57 08        lea         rdx,[rdi+8]
  00000000670C7623: 48 8D 4B 28        lea         rcx,[rbx+28h]
  00000000670C7627: E8 A4 8D FA FF     call        00000000670703D0
  00000000670C762C: 44 0F B7 5F 10     movzx       r11d,word ptr [rdi+10h]
  00000000670C7631: 66 44 89 5B 30     mov         word ptr [rbx+30h],r11w
  00000000670C7636: 0F B7 4F 12        movzx       ecx,word ptr [rdi+12h]
  00000000670C763A: 66 89 4B 32        mov         word ptr [rbx+32h],cx
  00000000670C763E: 0F B7 47 14        movzx       eax,word ptr [rdi+14h]
  00000000670C7642: 66 89 43 34        mov         word ptr [rbx+34h],ax
  00000000670C7646: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670C764B: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670C764F: 75 0B              jne         00000000670C765C
  00000000670C7651: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670C7656: E8 75 8A FA FF     call        00000000670700D0
  00000000670C765B: 90                 nop
  00000000670C765C: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670C7661: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670C7665: 75 0A              jne         00000000670C7671
  00000000670C7667: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670C766C: E8 5F 8A FA FF     call        00000000670700D0
  00000000670C7671: 80 7E 10 00        cmp         byte ptr [rsi+10h],0
  00000000670C7675: 75 11              jne         00000000670C7688
  00000000670C7677: 48 8B D5           mov         rdx,rbp
  00000000670C767A: 48 8B CE           mov         rcx,rsi
  00000000670C767D: E8 3E 2E 02 00     call        00000000670EA4C0
  00000000670C7682: 90                 nop
  00000000670C7683: 48 8B C6           mov         rax,rsi
  00000000670C7686: EB 03              jmp         00000000670C768B
  00000000670C7688: 48 8B C6           mov         rax,rsi
  00000000670C768B: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670C7690: 48 8B 6C 24 60     mov         rbp,qword ptr [rsp+60h]
  00000000670C7695: 48 8B 74 24 68     mov         rsi,qword ptr [rsp+68h]
  00000000670C769A: 48 83 C4 40        add         rsp,40h
  00000000670C769E: 5F                 pop         rdi
  00000000670C769F: C3                 ret

  Summary

      1A7000 .text

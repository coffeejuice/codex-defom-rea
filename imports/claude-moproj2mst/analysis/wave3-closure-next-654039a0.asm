Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000654039A0: 40 55              push        rbp
  00000000654039A2: 56                 push        rsi
  00000000654039A3: 57                 push        rdi
  00000000654039A4: 41 54              push        r12
  00000000654039A6: 41 55              push        r13
  00000000654039A8: 41 56              push        r14
  00000000654039AA: 41 57              push        r15
  00000000654039AC: 48 83 EC 40        sub         rsp,40h
  00000000654039B0: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000654039B9: 48 89 9C 24 90 00  mov         qword ptr [rsp+0000000000000090h],rbx
                    00 00
  00000000654039C1: 4D 8B E0           mov         r12,r8
  00000000654039C4: 8B EA              mov         ebp,edx
  00000000654039C6: 4C 8B F1           mov         r14,rcx
  00000000654039C9: 4C 8B 79 08        mov         r15,qword ptr [rcx+8]
  00000000654039CD: 45 33 ED           xor         r13d,r13d
  00000000654039D0: 48 81 C1 10 02 00  add         rcx,210h
                    00
  00000000654039D7: FF 15 DB 2E 29 00  call        qword ptr [00000000656968B8h]
  00000000654039DD: 8B F8              mov         edi,eax
  00000000654039DF: 33 DB              xor         ebx,ebx
  00000000654039E1: 49 8D 8E 10 02 00  lea         rcx,[r14+0000000000000210h]
                    00
  00000000654039E8: FF 15 CA 2E 29 00  call        qword ptr [00000000656968B8h]
  00000000654039EE: 85 C0              test        eax,eax
  00000000654039F0: 7E 40              jle         0000000065403A32
  00000000654039F2: 66 66 66 66 66 66  nop         word ptr [rax+rax+0000000000000000h]
                    0F 1F 84 00 00 00
                    00 00
  0000000065403A00: 8B D3              mov         edx,ebx
  0000000065403A02: 49 8D 8E 10 02 00  lea         rcx,[r14+0000000000000210h]
                    00
  0000000065403A09: FF 15 31 2F 29 00  call        qword ptr [0000000065696940h]
  0000000065403A0F: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065403A12: 4C 39 61 18        cmp         qword ptr [rcx+18h],r12
  0000000065403A16: 74 15              je          0000000065403A2D
  0000000065403A18: FF C3              inc         ebx
  0000000065403A1A: 49 8D 8E 10 02 00  lea         rcx,[r14+0000000000000210h]
                    00
  0000000065403A21: FF 15 91 2E 29 00  call        qword ptr [00000000656968B8h]
  0000000065403A27: 3B D8              cmp         ebx,eax
  0000000065403A29: 7C D5              jl          0000000065403A00
  0000000065403A2B: EB 05              jmp         0000000065403A32
  0000000065403A2D: 4C 8B E9           mov         r13,rcx
  0000000065403A30: FF CF              dec         edi
  0000000065403A32: 85 ED              test        ebp,ebp
  0000000065403A34: 78 04              js          0000000065403A3A
  0000000065403A36: 3B EF              cmp         ebp,edi
  0000000065403A38: 7E 02              jle         0000000065403A3C
  0000000065403A3A: 8B EF              mov         ebp,edi
  0000000065403A3C: 4D 85 ED           test        r13,r13
  0000000065403A3F: 74 46              je          0000000065403A87
  0000000065403A41: BA 01 00 00 00     mov         edx,1
  0000000065403A46: 49 8B 8E 10 02 00  mov         rcx,qword ptr [r14+0000000000000210h]
                    00
  0000000065403A4D: FF 15 1D 30 29 00  call        qword ptr [0000000065696A70h]
  0000000065403A53: 84 C0              test        al,al
  0000000065403A55: 74 16              je          0000000065403A6D
  0000000065403A57: 49 8B 96 10 02 00  mov         rdx,qword ptr [r14+0000000000000210h]
                    00
  0000000065403A5E: 8B 52 04           mov         edx,dword ptr [rdx+4]
  0000000065403A61: 49 8D 8E 10 02 00  lea         rcx,[r14+0000000000000210h]
                    00
  0000000065403A68: E8 B3 04 DA FF     call        00000000651A3F20
  0000000065403A6D: 44 8B C5           mov         r8d,ebp
  0000000065403A70: 8B D3              mov         edx,ebx
  0000000065403A72: 49 8D 8E 10 02 00  lea         rcx,[r14+0000000000000210h]
                    00
  0000000065403A79: FF 15 A1 35 29 00  call        qword ptr [0000000065697020h]
  0000000065403A7F: 49 8B C5           mov         rax,r13
  0000000065403A82: E9 DC 01 00 00     jmp         0000000065403C63
  0000000065403A87: B9 28 00 00 00     mov         ecx,28h
  0000000065403A8C: E8 E1 D6 1D 00     call        00000000655E1172
  0000000065403A91: 48 8B F8           mov         rdi,rax
  0000000065403A94: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000065403A9C: 48 85 C0           test        rax,rax
  0000000065403A9F: 74 2A              je          0000000065403ACB
  0000000065403AA1: 48 8B C8           mov         rcx,rax
  0000000065403AA4: FF 15 8E 2D 29 00  call        qword ptr [0000000065696838h]
  0000000065403AAA: C7 47 10 FF FF FF  mov         dword ptr [rdi+10h],0FFFFFFFFh
                    FF
  0000000065403AB1: 83 67 14 FC        and         dword ptr [rdi+14h],0FFFFFFFCh
  0000000065403AB5: 83 4F 14 04        or          dword ptr [rdi+14h],4
  0000000065403AB9: 48 C7 47 18 00 00  mov         qword ptr [rdi+18h],0
                    00 00
  0000000065403AC1: 48 C7 47 20 00 00  mov         qword ptr [rdi+20h],0
                    00 00
  0000000065403AC9: EB 02              jmp         0000000065403ACD
  0000000065403ACB: 33 FF              xor         edi,edi
  0000000065403ACD: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  0000000065403AD2: 48 8D 15 FF 7C 30  lea         rdx,[000000006570B7D8h]
                    00
  0000000065403AD9: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000065403ADE: FF 15 FC 2E 29 00  call        qword ptr [00000000656969E0h]
  0000000065403AE4: 48 8B D0           mov         rdx,rax
  0000000065403AE7: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000065403AEF: FF 15 13 31 29 00  call        qword ptr [0000000065696C08h]
  0000000065403AF5: 90                 nop
  0000000065403AF6: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000065403AFE: 49 8B CC           mov         rcx,r12
  0000000065403B01: FF 15 49 33 29 00  call        qword ptr [0000000065696E50h]
  0000000065403B07: 90                 nop
  0000000065403B08: 48 8B D0           mov         rdx,rax
  0000000065403B0B: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000065403B13: FF 15 0F 35 29 00  call        qword ptr [0000000065697028h]
  0000000065403B19: 90                 nop
  0000000065403B1A: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000065403B22: FF 15 38 2C 29 00  call        qword ptr [0000000065696760h]
  0000000065403B28: 4D 8B 1F           mov         r11,qword ptr [r15]
  0000000065403B2B: 49 8B CF           mov         rcx,r15
  0000000065403B2E: 41 FF 93 90 01 00  call        qword ptr [r11+0000000000000190h]
                    00
  0000000065403B35: 48 8B D8           mov         rbx,rax
  0000000065403B38: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  0000000065403B40: 48 8B C8           mov         rcx,rax
  0000000065403B43: FF 15 EF 2E 29 00  call        qword ptr [0000000065696A38h]
  0000000065403B49: 48 89 5F 20        mov         qword ptr [rdi+20h],rbx
  0000000065403B4D: 4C 89 67 18        mov         qword ptr [rdi+18h],r12
  0000000065403B51: 49 8B CC           mov         rcx,r12
  0000000065403B54: E8 97 4D C6 FF     call        00000000650688F0
  0000000065403B59: 4C 8D 44 24 20     lea         r8,[rsp+20h]
  0000000065403B5E: 8B D5              mov         edx,ebp
  0000000065403B60: 49 8D 8E 10 02 00  lea         rcx,[r14+0000000000000210h]
                    00
  0000000065403B67: E8 A4 D0 C4 FF     call        0000000065050C10
  0000000065403B6C: 48 85 DB           test        rbx,rbx
  0000000065403B6F: 74 1A              je          0000000065403B8B
  0000000065403B71: 49 8B 47 20        mov         rax,qword ptr [r15+20h]
  0000000065403B75: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065403B78: C1 E9 0F           shr         ecx,0Fh
  0000000065403B7B: 80 E1 01           and         cl,1
  0000000065403B7E: 74 0B              je          0000000065403B8B
  0000000065403B80: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065403B83: B2 01              mov         dl,1
  0000000065403B85: 48 8B CB           mov         rcx,rbx
  0000000065403B88: FF 50 58           call        qword ptr [rax+58h]
  0000000065403B8B: 41 80 BE 1F 02 00  cmp         byte ptr [r14+000000000000021Fh],0
                    00 00
  0000000065403B93: 0F 84 B9 00 00 00  je          0000000065403C52
  0000000065403B99: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000065403BA1: 48 8B 4F 18        mov         rcx,qword ptr [rdi+18h]
  0000000065403BA5: E8 C6 A3 C5 FF     call        000000006505DF70
  0000000065403BAA: 44 8B 9C 24 88 00  mov         r11d,dword ptr [rsp+0000000000000088h]
                    00 00
  0000000065403BB2: 41 8B C3           mov         eax,r11d
  0000000065403BB5: C1 E8 10           shr         eax,10h
  0000000065403BB8: 0F B6 C8           movzx       ecx,al
  0000000065403BBB: 89 8C 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],ecx
                    00
  0000000065403BC2: 41 C1 EB 18        shr         r11d,18h
  0000000065403BC6: 44 89 9C 24 80 00  mov         dword ptr [rsp+0000000000000080h],r11d
                    00 00
  0000000065403BCE: 48 8D 84 24 88 00  lea         rax,[rsp+0000000000000088h]
                    00 00
  0000000065403BD6: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000065403BDE: 44 3B D9           cmp         r11d,ecx
  0000000065403BE1: 48 0F 4D C2        cmovge      rax,rdx
  0000000065403BE5: 8B 08              mov         ecx,dword ptr [rax]
  0000000065403BE7: 81 F9 F3 00 00 00  cmp         ecx,0F3h
  0000000065403BED: 75 2D              jne         0000000065403C1C
  0000000065403BEF: 48 8B 5F 18        mov         rbx,qword ptr [rdi+18h]
  0000000065403BF3: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000065403BFB: 48 8B CB           mov         rcx,rbx
  0000000065403BFE: E8 6D A3 C5 FF     call        000000006505DF70
  0000000065403C03: 0F B7 94 24 88 00  movzx       edx,word ptr [rsp+0000000000000088h]
                    00 00
  0000000065403C0B: 89 94 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],edx
                    00
  0000000065403C12: 48 8B CB           mov         rcx,rbx
  0000000065403C15: E8 06 9F C6 FF     call        000000006506DB20
  0000000065403C1A: EB 36              jmp         0000000065403C52
  0000000065403C1C: 85 C9              test        ecx,ecx
  0000000065403C1E: 75 32              jne         0000000065403C52
  0000000065403C20: 48 8B 5F 18        mov         rbx,qword ptr [rdi+18h]
  0000000065403C24: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000065403C2C: 48 8B CB           mov         rcx,rbx
  0000000065403C2F: E8 3C A3 C5 FF     call        000000006505DF70
  0000000065403C34: 0F B7 94 24 88 00  movzx       edx,word ptr [rsp+0000000000000088h]
                    00 00
  0000000065403C3C: 81 CA 00 00 01 01  or          edx,1010000h
  0000000065403C42: 89 94 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],edx
                    00
  0000000065403C49: 48 8B CB           mov         rcx,rbx
  0000000065403C4C: E8 CF 9E C6 FF     call        000000006506DB20
  0000000065403C51: 90                 nop
  0000000065403C52: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000065403C5A: FF 15 00 2B 29 00  call        qword ptr [0000000065696760h]
  0000000065403C60: 48 8B C7           mov         rax,rdi
  0000000065403C63: 48 8B 9C 24 90 00  mov         rbx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000065403C6B: 48 83 C4 40        add         rsp,40h
  0000000065403C6F: 41 5F              pop         r15
  0000000065403C71: 41 5E              pop         r14
  0000000065403C73: 41 5D              pop         r13
  0000000065403C75: 41 5C              pop         r12
  0000000065403C77: 5F                 pop         rdi
  0000000065403C78: 5E                 pop         rsi
  0000000065403C79: 5D                 pop         rbp
  0000000065403C7A: C3                 ret

  Summary

      695000 .text

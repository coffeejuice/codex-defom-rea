Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C30E0: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  00000000670C30E5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000000670C30EA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000000670C30EF: 55                 push        rbp
  00000000670C30F0: 56                 push        rsi
  00000000670C30F1: 57                 push        rdi
  00000000670C30F2: 41 54              push        r12
  00000000670C30F4: 41 55              push        r13
  00000000670C30F6: 41 56              push        r14
  00000000670C30F8: 41 57              push        r15
  00000000670C30FA: 48 83 EC 30        sub         rsp,30h
  00000000670C30FE: 45 33 D2           xor         r10d,r10d
  00000000670C3101: 45 8B F1           mov         r14d,r9d
  00000000670C3104: 45 32 C9           xor         r9b,r9b
  00000000670C3107: 41 8B FA           mov         edi,r10d
  00000000670C310A: 40 32 F6           xor         sil,sil
  00000000670C310D: 4C 8B E9           mov         r13,rcx
  00000000670C3110: 41 8B EA           mov         ebp,r10d
  00000000670C3113: 44 88 4C 24 70     mov         byte ptr [rsp+70h],r9b
  00000000670C3118: 48 39 39           cmp         qword ptr [rcx],rdi
  00000000670C311B: 74 09              je          00000000670C3126
  00000000670C311D: 44 8B A1 A0 00 00  mov         r12d,dword ptr [rcx+00000000000000A0h]
                    00
  00000000670C3124: EB 04              jmp         00000000670C312A
  00000000670C3126: 44 8B 61 30        mov         r12d,dword ptr [rcx+30h]
  00000000670C312A: 44 8B 84 24 90 00  mov         r8d,dword ptr [rsp+0000000000000090h]
                    00 00
  00000000670C3132: 41 0F B7 DA        movzx       ebx,r10w
  00000000670C3136: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  00000000670C313B: 0F 1F 44 00 00     nop         dword ptr [rax+rax]
  00000000670C3140: 49 83 7D 00 00     cmp         qword ptr [r13],0
  00000000670C3145: 74 10              je          00000000670C3157
  00000000670C3147: 49 8B 85 98 00 00  mov         rax,qword ptr [r13+0000000000000098h]
                    00
  00000000670C314E: 48 8B 50 10        mov         rdx,qword ptr [rax+10h]
  00000000670C3152: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000670C3155: EB 0E              jmp         00000000670C3165
  00000000670C3157: 49 8B 45 28        mov         rax,qword ptr [r13+28h]
  00000000670C315B: 48 8B 08           mov         rcx,qword ptr [rax]
  00000000670C315E: 48 8B 51 10        mov         rdx,qword ptr [rcx+10h]
  00000000670C3162: 8B 49 08           mov         ecx,dword ptr [rcx+8]
  00000000670C3165: 49 63 C4           movsxd      rax,r12d
  00000000670C3168: 89 4C 24 28        mov         dword ptr [rsp+28h],ecx
  00000000670C316C: 4C 8D 3C 42        lea         r15,[rdx+rax*2]
  00000000670C3170: 44 3B E1           cmp         r12d,ecx
  00000000670C3173: 0F 8D 9F 00 00 00  jge         00000000670C3218
  00000000670C3179: 45 85 F6           test        r14d,r14d
  00000000670C317C: 74 09              je          00000000670C3187
  00000000670C317E: 41 3B EE           cmp         ebp,r14d
  00000000670C3181: 0F 8D 91 00 00 00  jge         00000000670C3218
  00000000670C3187: 41 0F B7 07        movzx       eax,word ptr [r15]
  00000000670C318B: 49 83 C7 02        add         r15,2
  00000000670C318F: FF C5              inc         ebp
  00000000670C3191: 41 8B D0           mov         edx,r8d
  00000000670C3194: 66 89 44 24 24     mov         word ptr [rsp+24h],ax
  00000000670C3199: 45 85 C0           test        r8d,r8d
  00000000670C319C: 74 44              je          00000000670C31E2
  00000000670C319E: 83 EA 01           sub         edx,1
  00000000670C31A1: 74 24              je          00000000670C31C7
  00000000670C31A3: 83 FA 01           cmp         edx,1
  00000000670C31A6: 75 5B              jne         00000000670C3203
  00000000670C31A8: 66 83 F8 0A        cmp         ax,0Ah
  00000000670C31AC: 75 14              jne         00000000670C31C2
  00000000670C31AE: 66 83 FB 0D        cmp         bx,0Dh
  00000000670C31B2: 41 8B FA           mov         edi,r10d
  00000000670C31B5: 0F B6 F2           movzx       esi,dl
  00000000670C31B8: 40 0F 94 C7        sete        dil
  00000000670C31BC: 88 54 24 70        mov         byte ptr [rsp+70h],dl
  00000000670C31C0: FF C7              inc         edi
  00000000670C31C2: 66 8B D8           mov         bx,ax
  00000000670C31C5: EB 3C              jmp         00000000670C3203
  00000000670C31C7: 48 8D 4C 24 24     lea         rcx,[rsp+24h]
  00000000670C31CC: E8 AF 6A FA FF     call        0000000067069C80
  00000000670C31D1: 44 8B 84 24 90 00  mov         r8d,dword ptr [rsp+0000000000000090h]
                    00 00
  00000000670C31D9: 45 33 D2           xor         r10d,r10d
  00000000670C31DC: 84 C0              test        al,al
  00000000670C31DE: 75 23              jne         00000000670C3203
  00000000670C31E0: EB 19              jmp         00000000670C31FB
  00000000670C31E2: 48 8D 4C 24 24     lea         rcx,[rsp+24h]
  00000000670C31E7: E8 94 6A FA FF     call        0000000067069C80
  00000000670C31EC: 44 8B 84 24 90 00  mov         r8d,dword ptr [rsp+0000000000000090h]
                    00 00
  00000000670C31F4: 45 33 D2           xor         r10d,r10d
  00000000670C31F7: 84 C0              test        al,al
  00000000670C31F9: 74 08              je          00000000670C3203
  00000000670C31FB: BF 01 00 00 00     mov         edi,1
  00000000670C3200: 40 8A F7           mov         sil,dil
  00000000670C3203: 41 FF C4           inc         r12d
  00000000670C3206: 40 84 F6           test        sil,sil
  00000000670C3209: 0F 85 99 00 00 00  jne         00000000670C32A8
  00000000670C320F: 8B 4C 24 28        mov         ecx,dword ptr [rsp+28h]
  00000000670C3213: E9 58 FF FF FF     jmp         00000000670C3170
  00000000670C3218: 45 85 F6           test        r14d,r14d
  00000000670C321B: 74 05              je          00000000670C3222
  00000000670C321D: 41 3B EE           cmp         ebp,r14d
  00000000670C3220: 7D 33              jge         00000000670C3255
  00000000670C3222: 49 83 7D 00 00     cmp         qword ptr [r13],0
  00000000670C3227: 74 2C              je          00000000670C3255
  00000000670C3229: 48 83 CA FF        or          rdx,0FFFFFFFFFFFFFFFFh
  00000000670C322D: 49 8B CD           mov         rcx,r13
  00000000670C3230: E8 0B FA FF FF     call        00000000670C2C40
  00000000670C3235: 44 8B 84 24 90 00  mov         r8d,dword ptr [rsp+0000000000000090h]
                    00 00
  00000000670C323D: 41 BA 00 00 00 00  mov         r10d,0
  00000000670C3243: 44 0F B6 F8        movzx       r15d,al
  00000000670C3247: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000000670C324B: 84 C0              test        al,al
  00000000670C324D: 0F 85 ED FE FF FF  jne         00000000670C3140
  00000000670C3253: EB 05              jmp         00000000670C325A
  00000000670C3255: 44 8A 7C 24 20     mov         r15b,byte ptr [rsp+20h]
  00000000670C325A: 45 85 F6           test        r14d,r14d
  00000000670C325D: 74 05              je          00000000670C3264
  00000000670C325F: 41 3B EE           cmp         ebp,r14d
  00000000670C3262: 7D 44              jge         00000000670C32A8
  00000000670C3264: 85 ED              test        ebp,ebp
  00000000670C3266: 0F 84 F2 00 00 00  je          00000000670C335E
  00000000670C326C: 49 8B 55 28        mov         rdx,qword ptr [r13+28h]
  00000000670C3270: 48 85 D2           test        rdx,rdx
  00000000670C3273: 74 14              je          00000000670C3289
  00000000670C3275: 41 8B 45 30        mov         eax,dword ptr [r13+30h]
  00000000670C3279: 8D 4C 05 00        lea         ecx,[rbp+rax]
  00000000670C327D: 48 8B 02           mov         rax,qword ptr [rdx]
  00000000670C3280: 3B 48 08           cmp         ecx,dword ptr [rax+8]
  00000000670C3283: 0F 8C D5 00 00 00  jl          00000000670C335E
  00000000670C3289: 49 8B 4D 00        mov         rcx,qword ptr [r13]
  00000000670C328D: 48 85 C9           test        rcx,rcx
  00000000670C3290: 74 16              je          00000000670C32A8
  00000000670C3292: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670C3295: FF 90 88 00 00 00  call        qword ptr [rax+0000000000000088h]
  00000000670C329B: 84 C0              test        al,al
  00000000670C329D: 75 09              jne         00000000670C32A8
  00000000670C329F: 45 84 FF           test        r15b,r15b
  00000000670C32A2: 0F 85 B6 00 00 00  jne         00000000670C335E
  00000000670C32A8: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],2
                    00 02
  00000000670C32B0: 75 45              jne         00000000670C32F7
  00000000670C32B2: 85 ED              test        ebp,ebp
  00000000670C32B4: 7E 41              jle         00000000670C32F7
  00000000670C32B6: 40 84 F6           test        sil,sil
  00000000670C32B9: 75 3C              jne         00000000670C32F7
  00000000670C32BB: 49 8B 55 28        mov         rdx,qword ptr [r13+28h]
  00000000670C32BF: 48 85 D2           test        rdx,rdx
  00000000670C32C2: 74 10              je          00000000670C32D4
  00000000670C32C4: 41 8B 45 30        mov         eax,dword ptr [r13+30h]
  00000000670C32C8: 8D 4C 05 00        lea         ecx,[rbp+rax]
  00000000670C32CC: 48 8B 02           mov         rax,qword ptr [rdx]
  00000000670C32CF: 3B 48 08           cmp         ecx,dword ptr [rax+8]
  00000000670C32D2: 74 16              je          00000000670C32EA
  00000000670C32D4: 49 8B 4D 00        mov         rcx,qword ptr [r13]
  00000000670C32D8: 48 85 C9           test        rcx,rcx
  00000000670C32DB: 74 1A              je          00000000670C32F7
  00000000670C32DD: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670C32E0: FF 90 88 00 00 00  call        qword ptr [rax+0000000000000088h]
  00000000670C32E6: 84 C0              test        al,al
  00000000670C32E8: 74 0D              je          00000000670C32F7
  00000000670C32EA: 66 83 FB 0D        cmp         bx,0Dh
  00000000670C32EE: 75 07              jne         00000000670C32F7
  00000000670C32F0: 41 B1 01           mov         r9b,1
  00000000670C32F3: FF C7              inc         edi
  00000000670C32F5: EB 06              jmp         00000000670C32FD
  00000000670C32F7: 44 0F B6 4C 24 70  movzx       r9d,byte ptr [rsp+70h]
  00000000670C32FD: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000000670C3305: 48 85 C9           test        rcx,rcx
  00000000670C3308: 74 06              je          00000000670C3310
  00000000670C330A: 8B C5              mov         eax,ebp
  00000000670C330C: 2B C7              sub         eax,edi
  00000000670C330E: 89 01              mov         dword ptr [rcx],eax
  00000000670C3310: 4C 8B 44 24 78     mov         r8,qword ptr [rsp+78h]
  00000000670C3315: 4D 85 C0           test        r8,r8
  00000000670C3318: 74 2B              je          00000000670C3345
  00000000670C331A: 49 8B 45 28        mov         rax,qword ptr [r13+28h]
  00000000670C331E: 48 85 C0           test        rax,rax
  00000000670C3321: 74 09              je          00000000670C332C
  00000000670C3323: 49 63 55 30        movsxd      rdx,dword ptr [r13+30h]
  00000000670C3327: 48 8B 00           mov         rax,qword ptr [rax]
  00000000670C332A: EB 0E              jmp         00000000670C333A
  00000000670C332C: 49 63 95 A0 00 00  movsxd      rdx,dword ptr [r13+00000000000000A0h]
                    00
  00000000670C3333: 49 8B 85 98 00 00  mov         rax,qword ptr [r13+0000000000000098h]
                    00
  00000000670C333A: 48 8B 48 10        mov         rcx,qword ptr [rax+10h]
  00000000670C333E: 48 8D 04 51        lea         rax,[rcx+rdx*2]
  00000000670C3342: 49 89 00           mov         qword ptr [r8],rax
  00000000670C3345: 41 89 AD 8C 00 00  mov         dword ptr [r13+000000000000008Ch],ebp
                    00
  00000000670C334C: 45 84 C9           test        r9b,r9b
  00000000670C334F: 75 09              jne         00000000670C335A
  00000000670C3351: 2B EF              sub         ebp,edi
  00000000670C3353: 41 89 AD 8C 00 00  mov         dword ptr [r13+000000000000008Ch],ebp
                    00
  00000000670C335A: B0 01              mov         al,1
  00000000670C335C: EB 02              jmp         00000000670C3360
  00000000670C335E: 32 C0              xor         al,al
  00000000670C3360: 48 8B 9C 24 88 00  mov         rbx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000000670C3368: 48 83 C4 30        add         rsp,30h
  00000000670C336C: 41 5F              pop         r15
  00000000670C336E: 41 5E              pop         r14
  00000000670C3370: 41 5D              pop         r13
  00000000670C3372: 41 5C              pop         r12
  00000000670C3374: 5F                 pop         rdi
  00000000670C3375: 5E                 pop         rsi
  00000000670C3376: 5D                 pop         rbp
  00000000670C3377: C3                 ret
  00000000670C3378: CC                 int         3
  00000000670C3379: CC                 int         3
  00000000670C337A: CC                 int         3
  00000000670C337B: CC                 int         3
  00000000670C337C: CC                 int         3
  00000000670C337D: CC                 int         3
  00000000670C337E: CC                 int         3
  00000000670C337F: CC                 int         3
  00000000670C3380: 40

  Summary

      1A7000 .text

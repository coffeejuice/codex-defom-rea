Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065300790: 48 8B C4           mov         rax,rsp
  0000000065300793: 57                 push        rdi
  0000000065300794: 41 54              push        r12
  0000000065300796: 41 55              push        r13
  0000000065300798: 41 56              push        r14
  000000006530079A: 41 57              push        r15
  000000006530079C: 48 81 EC 50 01 00  sub         rsp,150h
                    00
  00000000653007A3: 48 C7 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000000653007AF: 48 89 58 08        mov         qword ptr [rax+8],rbx
  00000000653007B3: 48 89 68 10        mov         qword ptr [rax+10h],rbp
  00000000653007B7: 48 89 70 18        mov         qword ptr [rax+18h],rsi
  00000000653007BB: 4D 8B E1           mov         r12,r9
  00000000653007BE: 49 63 D8           movsxd      rbx,r8d
  00000000653007C1: 4C 8B EA           mov         r13,rdx
  00000000653007C4: 48 8B F9           mov         rdi,rcx
  00000000653007C7: 33 C9              xor         ecx,ecx
  00000000653007C9: E8 A2 AF 00 00     call        000000006530B770
  00000000653007CE: 84 C0              test        al,al
  00000000653007D0: 0F 84 DD 06 00 00  je          0000000065300EB3
  00000000653007D6: 48 8B 05 DB 6A 39  mov         rax,qword ptr [00000000656972B8h]
                    00
  00000000653007DD: 8B 08              mov         ecx,dword ptr [rax]
  00000000653007DF: 81 F9 80 00 00 00  cmp         ecx,80h
  00000000653007E5: 0F 8C C8 06 00 00  jl          0000000065300EB3
  00000000653007EB: F6 C1 F0           test        cl,0F0h
  00000000653007EE: 0F 84 BF 06 00 00  je          0000000065300EB3
  00000000653007F4: 48 8B B4 24 A0 01  mov         rsi,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000000653007FC: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  0000000065300801: 4D 8B CC           mov         r9,r12
  0000000065300804: 44 8B C3           mov         r8d,ebx
  0000000065300807: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006530080C: 48 8B CF           mov         rcx,rdi
  000000006530080F: E8 CC E5 00 00     call        000000006530EDE0
  0000000065300814: 83 FB 3A           cmp         ebx,3Ah
  0000000065300817: 0F 87 71 06 00 00  ja          0000000065300E8E
  000000006530081D: 48 8D 15 DC F7 CF  lea         rdx,[0000000065000000h]
                    FF
  0000000065300824: 0F B6 84 1A 14 0F  movzx       eax,byte ptr [rdx+rbx+0000000000300F14h]
                    30 00
  000000006530082C: 8B 8C 82 F8 0E 30  mov         ecx,dword ptr [rdx+rax*4+0000000000300EF8h]
                    00
  0000000065300833: 48 03 CA           add         rcx,rdx
  0000000065300836: FF E1              jmp         rcx
  0000000065300838: 4D 85 E4           test        r12,r12
  000000006530083B: 0F 84 4D 06 00 00  je          0000000065300E8E
  0000000065300841: 41 83 3C 24 01     cmp         dword ptr [r12],1
  0000000065300846: 0F 8C 42 06 00 00  jl          0000000065300E8E
  000000006530084C: 41 83 7C 24 04 02  cmp         dword ptr [r12+4],2
  0000000065300852: 0F 85 36 06 00 00  jne         0000000065300E8E
  0000000065300858: 33 ED              xor         ebp,ebp
  000000006530085A: 48 85 F6           test        rsi,rsi
  000000006530085D: 74 0A              je          0000000065300869
  000000006530085F: 48 8B CE           mov         rcx,rsi
  0000000065300862: E8 29 C2 00 00     call        000000006530CA90
  0000000065300867: EB 03              jmp         000000006530086C
  0000000065300869: 48 8B C5           mov         rax,rbp
  000000006530086C: 48 8D 15 D5 B8 3E  lea         rdx,[00000000656EC148h]
                    00
  0000000065300873: 48 8B C8           mov         rcx,rax
  0000000065300876: FF 15 A4 C0 6C 00  call        qword ptr [00000000659CC920h]
  000000006530087C: 4C 8B F8           mov         r15,rax
  000000006530087F: 48 85 C0           test        rax,rax
  0000000065300882: 0F 84 06 06 00 00  je          0000000065300E8E
  0000000065300888: 41 8B 4C 24 08     mov         ecx,dword ptr [r12+8]
  000000006530088D: 41 BE 05 00 00 00  mov         r14d,5
  0000000065300893: F6 C1 01           test        cl,1
  0000000065300896: 75 06              jne         000000006530089E
  0000000065300898: 41 8D 5E FF        lea         ebx,[r14-1]
  000000006530089C: EB 28              jmp         00000000653008C6
  000000006530089E: F6 C1 04           test        cl,4
  00000000653008A1: 74 07              je          00000000653008AA
  00000000653008A3: BB 03 00 00 00     mov         ebx,3
  00000000653008A8: EB 1C              jmp         00000000653008C6
  00000000653008AA: 0F BA E1 0D        bt          ecx,0Dh
  00000000653008AE: 73 07              jae         00000000653008B7
  00000000653008B0: BB 02 00 00 00     mov         ebx,2
  00000000653008B5: EB 0F              jmp         00000000653008C6
  00000000653008B7: 41 F6 44 24 38 04  test        byte ptr [r12+38h],4
  00000000653008BD: BB 01 00 00 00     mov         ebx,1
  00000000653008C2: 41 0F 45 DE        cmovne      ebx,r14d
  00000000653008C6: 48 8B CF           mov         rcx,rdi
  00000000653008C9: E8 D2 76 FA FF     call        00000000652A7FA0
  00000000653008CE: 4C 8B 10           mov         r10,qword ptr [rax]
  00000000653008D1: 4C 8B CE           mov         r9,rsi
  00000000653008D4: 4D 8B C4           mov         r8,r12
  00000000653008D7: 41 8B D6           mov         edx,r14d
  00000000653008DA: 48 8B C8           mov         rcx,rax
  00000000653008DD: 41 FF 92 D8 00 00  call        qword ptr [r10+00000000000000D8h]
                    00
  00000000653008E4: 8B C8              mov         ecx,eax
  00000000653008E6: F7 D9              neg         ecx
  00000000653008E8: 89 4C 24 28        mov         dword ptr [rsp+28h],ecx
  00000000653008EC: 89 4C 24 20        mov         dword ptr [rsp+20h],ecx
  00000000653008F0: 44 8B C8           mov         r9d,eax
  00000000653008F3: 44 8B C0           mov         r8d,eax
  00000000653008F6: 48 8D 94 24 30 01  lea         rdx,[rsp+0000000000000130h]
                    00 00
  00000000653008FE: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  0000000065300903: FF 15 27 60 39 00  call        qword ptr [0000000065696930h]
  0000000065300909: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006530090E: 8B 08              mov         ecx,dword ptr [rax]
  0000000065300910: 89 0A              mov         dword ptr [rdx],ecx
  0000000065300912: 8B 48 04           mov         ecx,dword ptr [rax+4]
  0000000065300915: 89 4A 04           mov         dword ptr [rdx+4],ecx
  0000000065300918: 8B 48 08           mov         ecx,dword ptr [rax+8]
  000000006530091B: 89 4A 08           mov         dword ptr [rdx+8],ecx
  000000006530091E: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  0000000065300921: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  0000000065300924: 48 8D 44 24 40     lea         rax,[rsp+40h]
  0000000065300929: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006530092E: 48 89 6C 24 28     mov         qword ptr [rsp+28h],rbp
  0000000065300933: C7 44 24 20 12 0E  mov         dword ptr [rsp+20h],0E12h
                    00 00
  000000006530093B: 44 8B CB           mov         r9d,ebx
  000000006530093E: 33 D2              xor         edx,edx
  0000000065300940: 44 8D 42 01        lea         r8d,[rdx+1]
  0000000065300944: 49 8B CF           mov         rcx,r15
  0000000065300947: FF 15 33 C0 6C 00  call        qword ptr [00000000659CC980h]
  000000006530094D: 85 C0              test        eax,eax
  000000006530094F: 0F 85 39 05 00 00  jne         0000000065300E8E
  0000000065300955: 8B 44 24 4C        mov         eax,dword ptr [rsp+4Ch]
  0000000065300959: F7 D8              neg         eax
  000000006530095B: 44 8B 4C 24 44     mov         r9d,dword ptr [rsp+44h]
  0000000065300960: 41 F7 D9           neg         r9d
  0000000065300963: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000065300967: 44 8B 44 24 48     mov         r8d,dword ptr [rsp+48h]
  000000006530096C: 8B 54 24 40        mov         edx,dword ptr [rsp+40h]
  0000000065300970: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065300975: FF 15 8D 67 39 00  call        qword ptr [0000000065697108h]
  000000006530097B: 4C 8D 4C 24 50     lea         r9,[rsp+50h]
  0000000065300980: 4D 8D 44 24 10     lea         r8,[r12+10h]
  0000000065300985: 41 8B 54 24 0C     mov         edx,dword ptr [r12+0Ch]
  000000006530098A: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000065300992: E8 A9 66 FA FF     call        00000000652A7040
  0000000065300997: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006530099C: 8B 08              mov         ecx,dword ptr [rax]
  000000006530099E: 89 0A              mov         dword ptr [rdx],ecx
  00000000653009A0: 8B 48 04           mov         ecx,dword ptr [rax+4]
  00000000653009A3: 89 4A 04           mov         dword ptr [rdx+4],ecx
  00000000653009A6: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000653009A9: 89 4A 08           mov         dword ptr [rdx+8],ecx
  00000000653009AC: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  00000000653009AF: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  00000000653009B2: E9 D7 04 00 00     jmp         0000000065300E8E
  00000000653009B7: 0F 28 44 24 50     movaps      xmm0,xmmword ptr [rsp+50h]
  00000000653009BC: 66 0F 7F 44 24 70  movdqa      xmmword ptr [rsp+70h],xmm0
  00000000653009C2: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  00000000653009C7: FF 15 5B 5D 39 00  call        qword ptr [0000000065696728h]
  00000000653009CD: 89 44 24 64        mov         dword ptr [rsp+64h],eax
  00000000653009D1: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  00000000653009D6: FF 15 84 5E 39 00  call        qword ptr [0000000065696860h]
  00000000653009DC: 44 8B F8           mov         r15d,eax
  00000000653009DF: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  00000000653009E4: FF 15 16 5F 39 00  call        qword ptr [0000000065696900h]
  00000000653009EA: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000000653009EE: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  00000000653009F3: FF 15 0F 5F 39 00  call        qword ptr [0000000065696908h]
  00000000653009F9: 44 8B F0           mov         r14d,eax
  00000000653009FC: 48 8B CF           mov         rcx,rdi
  00000000653009FF: E8 9C 75 FA FF     call        00000000652A7FA0
  0000000065300A04: 4C 8B 10           mov         r10,qword ptr [rax]
  0000000065300A07: 4C 8B CE           mov         r9,rsi
  0000000065300A0A: 4D 8B C4           mov         r8,r12
  0000000065300A0D: BA 30 00 00 00     mov         edx,30h
  0000000065300A12: 48 8B C8           mov         rcx,rax
  0000000065300A15: 41 FF 92 D8 00 00  call        qword ptr [r10+00000000000000D8h]
                    00
  0000000065300A1C: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  0000000065300A20: 48 8D 15 19 B7 3E  lea         rdx,[00000000656EC140h]
                    00
  0000000065300A27: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000065300A2F: FF 15 AB 5F 39 00  call        qword ptr [00000000656969E0h]
  0000000065300A35: 48 8B D0           mov         rdx,rax
  0000000065300A38: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000065300A40: FF 15 C2 61 39 00  call        qword ptr [0000000065696C08h]
  0000000065300A46: 90                 nop
  0000000065300A47: 48 89 B4 24 90 00  mov         qword ptr [rsp+0000000000000090h],rsi
                    00 00
  0000000065300A4F: 33 ED              xor         ebp,ebp
  0000000065300A51: 48 89 AC 24 98 00  mov         qword ptr [rsp+0000000000000098h],rbp
                    00 00
  0000000065300A59: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000065300A61: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000065300A69: FF 15 81 5F 39 00  call        qword ptr [00000000656969F0h]
  0000000065300A6F: 48 89 AC 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rbp
                    00 00
  0000000065300A77: BB 04 00 00 00     mov         ebx,4
  0000000065300A7C: 89 9C 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],ebx
                    00
  0000000065300A83: BB 02 00 00 00     mov         ebx,2
  0000000065300A88: 89 9C 24 B4 00 00  mov         dword ptr [rsp+00000000000000B4h],ebx
                    00
  0000000065300A8F: 83 A4 24 B8 00 00  and         dword ptr [rsp+00000000000000B8h],0FFFFFFF0h
                    00 F0
  0000000065300A97: 89 AC 24 BC 00 00  mov         dword ptr [rsp+00000000000000BCh],ebp
                    00
  0000000065300A9E: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000065300AA6: 41 8B 44 24 10     mov         eax,dword ptr [r12+10h]
  0000000065300AAB: 89 01              mov         dword ptr [rcx],eax
  0000000065300AAD: 41 8B 44 24 14     mov         eax,dword ptr [r12+14h]
  0000000065300AB2: 89 41 04           mov         dword ptr [rcx+4],eax
  0000000065300AB5: 41 8B 44 24 18     mov         eax,dword ptr [r12+18h]
  0000000065300ABA: 89 41 08           mov         dword ptr [rcx+8],eax
  0000000065300ABD: 41 8B 44 24 1C     mov         eax,dword ptr [r12+1Ch]
  0000000065300AC2: 89 41 0C           mov         dword ptr [rcx+0Ch],eax
  0000000065300AC5: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000065300ACD: FF 15 8D 5C 39 00  call        qword ptr [0000000065696760h]
  0000000065300AD3: 8D 5D 0D           lea         ebx,[rbp+0Dh]
  0000000065300AD6: 8D 7D 05           lea         edi,[rbp+5]
  0000000065300AD9: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000065300AE1: E8 3A F3 00 00     call        000000006530FE20
  0000000065300AE6: 84 C0              test        al,al
  0000000065300AE8: 74 4B              je          0000000065300B35
  0000000065300AEA: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000065300AF2: E8 79 C0 00 00     call        000000006530CB70
  0000000065300AF7: 48 8B C8           mov         rcx,rax
  0000000065300AFA: 48 8D 44 24 40     lea         rax,[rsp+40h]
  0000000065300AFF: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000065300B04: C7 44 24 28 01 00  mov         dword ptr [rsp+28h],1
                    00 00
  0000000065300B0C: 48 89 6C 24 20     mov         qword ptr [rsp+20h],rbp
  0000000065300B11: 44 8B 8C 24 B4 00  mov         r9d,dword ptr [rsp+00000000000000B4h]
                    00 00
  0000000065300B19: 44 8B 84 24 B0 00  mov         r8d,dword ptr [rsp+00000000000000B0h]
                    00 00
  0000000065300B21: 33 D2              xor         edx,edx
  0000000065300B23: FF 15 67 BE 6C 00  call        qword ptr [00000000659CC990h]
  0000000065300B29: 85 C0              test        eax,eax
  0000000065300B2B: 75 08              jne         0000000065300B35
  0000000065300B2D: 8B 5C 24 40        mov         ebx,dword ptr [rsp+40h]
  0000000065300B31: 8B 7C 24 44        mov         edi,dword ptr [rsp+44h]
  0000000065300B35: 41 8B 44 24 08     mov         eax,dword ptr [r12+8]
  0000000065300B3A: 25 80 00 00 00     and         eax,80h
  0000000065300B3F: 74 30              je          0000000065300B71
  0000000065300B41: 8B C3              mov         eax,ebx
  0000000065300B43: 99                 cdq
  0000000065300B44: 2B C2              sub         eax,edx
  0000000065300B46: D1 F8              sar         eax,1
  0000000065300B48: 8B C8              mov         ecx,eax
  0000000065300B4A: 41 8B C7           mov         eax,r15d
  0000000065300B4D: 99                 cdq
  0000000065300B4E: 2B C2              sub         eax,edx
  0000000065300B50: D1 F8              sar         eax,1
  0000000065300B52: 2B C1              sub         eax,ecx
  0000000065300B54: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  0000000065300B58: 03 D0              add         edx,eax
  0000000065300B5A: 89 7C 24 20        mov         dword ptr [rsp+20h],edi
  0000000065300B5E: 44 8B CB           mov         r9d,ebx
  0000000065300B61: 45 8B C6           mov         r8d,r14d
  0000000065300B64: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000065300B69: FF 15 A9 67 39 00  call        qword ptr [0000000065697318h]
  0000000065300B6F: EB 39              jmp         0000000065300BAA
  0000000065300B71: 41 8B C7           mov         eax,r15d
  0000000065300B74: 99                 cdq
  0000000065300B75: 2B C2              sub         eax,edx
  0000000065300B77: D1 F8              sar         eax,1
  0000000065300B79: 8B 4C 24 68        mov         ecx,dword ptr [rsp+68h]
  0000000065300B7D: 03 C9              add         ecx,ecx
  0000000065300B7F: 44 2B F9           sub         r15d,ecx
  0000000065300B82: 44 2B F1           sub         r14d,ecx
  0000000065300B85: 44 2B F0           sub         r14d,eax
  0000000065300B88: 44 03 74 24 64     add         r14d,dword ptr [rsp+64h]
  0000000065300B8D: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  0000000065300B91: 83 C2 05           add         edx,5
  0000000065300B94: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000065300B98: 45 8D 4F FB        lea         r9d,[r15-5]
  0000000065300B9C: 45 8B C6           mov         r8d,r14d
  0000000065300B9F: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000065300BA4: FF 15 6E 67 39 00  call        qword ptr [0000000065697318h]
  0000000065300BAA: 4C 8D 4C 24 70     lea         r9,[rsp+70h]
  0000000065300BAF: 4D 8D 44 24 10     lea         r8,[r12+10h]
  0000000065300BB4: 41 8B 54 24 0C     mov         edx,dword ptr [r12+0Ch]
  0000000065300BB9: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000065300BC1: E8 7A 64 FA FF     call        00000000652A7040
  0000000065300BC6: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000065300BCB: 8B 08              mov         ecx,dword ptr [rax]
  0000000065300BCD: 89 0A              mov         dword ptr [rdx],ecx
  0000000065300BCF: 8B 48 04           mov         ecx,dword ptr [rax+4]
  0000000065300BD2: 89 4A 04           mov         dword ptr [rdx+4],ecx
  0000000065300BD5: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065300BD8: 89 4A 08           mov         dword ptr [rdx+8],ecx
  0000000065300BDB: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  0000000065300BDE: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  0000000065300BE1: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000065300BE9: FF 15 71 5B 39 00  call        qword ptr [0000000065696760h]
  0000000065300BEF: E9 9A 02 00 00     jmp         0000000065300E8E
  0000000065300BF4: 48 8B CF           mov         rcx,rdi
  0000000065300BF7: E8 A4 73 FA FF     call        00000000652A7FA0
  0000000065300BFC: 4C 8B 10           mov         r10,qword ptr [rax]
  0000000065300BFF: 4C 8B CE           mov         r9,rsi
  0000000065300C02: 4D 8B C4           mov         r8,r12
  0000000065300C05: BA 30 00 00 00     mov         edx,30h
  0000000065300C0A: 48 8B C8           mov         rcx,rax
  0000000065300C0D: 41 FF 92 D8 00 00  call        qword ptr [r10+00000000000000D8h]
                    00
  0000000065300C14: 8B E8              mov         ebp,eax
  0000000065300C16: 48 8D 44 24 40     lea         rax,[rsp+40h]
  0000000065300C1B: 41 8B 4C 24 10     mov         ecx,dword ptr [r12+10h]
  0000000065300C20: 89 08              mov         dword ptr [rax],ecx
  0000000065300C22: 41 8B 4C 24 14     mov         ecx,dword ptr [r12+14h]
  0000000065300C27: 89 48 04           mov         dword ptr [rax+4],ecx
  0000000065300C2A: 41 8B 4C 24 18     mov         ecx,dword ptr [r12+18h]
  0000000065300C2F: 89 48 08           mov         dword ptr [rax+8],ecx
  0000000065300C32: 41 8B 4C 24 1C     mov         ecx,dword ptr [r12+1Ch]
  0000000065300C37: 89 48 0C           mov         dword ptr [rax+0Ch],ecx
  0000000065300C3A: 44 8D 74 2D 00     lea         r14d,[rbp+rbp]
  0000000065300C3F: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  0000000065300C44: FF 15 DE 5A 39 00  call        qword ptr [0000000065696728h]
  0000000065300C4A: 8B F0              mov         esi,eax
  0000000065300C4C: 41 2B F6           sub         esi,r14d
  0000000065300C4F: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  0000000065300C54: FF 15 06 5C 39 00  call        qword ptr [0000000065696860h]
  0000000065300C5A: 8B F8              mov         edi,eax
  0000000065300C5C: 41 2B FE           sub         edi,r14d
  0000000065300C5F: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  0000000065300C64: FF 15 9E 5C 39 00  call        qword ptr [0000000065696908h]
  0000000065300C6A: 8D 5C 05 00        lea         ebx,[rbp+rax]
  0000000065300C6E: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  0000000065300C73: FF 15 87 5C 39 00  call        qword ptr [0000000065696900h]
  0000000065300C79: 8D 54 05 00        lea         edx,[rbp+rax]
  0000000065300C7D: 89 74 24 20        mov         dword ptr [rsp+20h],esi
  0000000065300C81: 44 8B CF           mov         r9d,edi
  0000000065300C84: 44 8B C3           mov         r8d,ebx
  0000000065300C87: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065300C8C: FF 15 86 66 39 00  call        qword ptr [0000000065697318h]
  0000000065300C92: 41 83 3C 24 01     cmp         dword ptr [r12],1
  0000000065300C97: 7C 48              jl          0000000065300CE1
  0000000065300C99: 41 83 7C 24 04 08  cmp         dword ptr [r12+4],8
  0000000065300C9F: 75 40              jne         0000000065300CE1
  0000000065300CA1: 41 83 7C 24 64 00  cmp         dword ptr [r12+64h],0
  0000000065300CA7: 74 38              je          0000000065300CE1
  0000000065300CA9: 41 F6 44 24 08 80  test        byte ptr [r12+8],80h
  0000000065300CAF: 75 30              jne         0000000065300CE1
  0000000065300CB1: 49 8D 4C 24 10     lea         rcx,[r12+10h]
  0000000065300CB6: FF 15 A4 5B 39 00  call        qword ptr [0000000065696860h]
  0000000065300CBC: 99                 cdq
  0000000065300CBD: 2B C2              sub         eax,edx
  0000000065300CBF: D1 F8              sar         eax,1
  0000000065300CC1: 41 F7 DE           neg         r14d
  0000000065300CC4: 44 2B F0           sub         r14d,eax
  0000000065300CC7: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065300CCC: FF 15 56 5A 39 00  call        qword ptr [0000000065696728h]
  0000000065300CD2: 41 8D 14 06        lea         edx,[r14+rax]
  0000000065300CD6: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000065300CDB: FF 15 07 6F 39 00  call        qword ptr [0000000065697BE8h]
  0000000065300CE1: 4C 8D 4C 24 40     lea         r9,[rsp+40h]
  0000000065300CE6: 4D 8D 44 24 10     lea         r8,[r12+10h]
  0000000065300CEB: 41 8B 54 24 0C     mov         edx,dword ptr [r12+0Ch]
  0000000065300CF0: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000065300CF8: E8 43 63 FA FF     call        00000000652A7040
  0000000065300CFD: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000065300D02: 8B 08              mov         ecx,dword ptr [rax]
  0000000065300D04: 89 0A              mov         dword ptr [rdx],ecx
  0000000065300D06: 8B 48 04           mov         ecx,dword ptr [rax+4]
  0000000065300D09: 89 4A 04           mov         dword ptr [rdx+4],ecx
  0000000065300D0C: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065300D0F: 89 4A 08           mov         dword ptr [rdx+8],ecx
  0000000065300D12: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  0000000065300D15: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  0000000065300D18: E9 71 01 00 00     jmp         0000000065300E8E
  0000000065300D1D: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  0000000065300D22: 4D 8B CC           mov         r9,r12
  0000000065300D25: 41 B8 0D 00 00 00  mov         r8d,0Dh
  0000000065300D2B: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  0000000065300D33: 48 8B CF           mov         rcx,rdi
  0000000065300D36: E8 25 C1 FB FF     call        00000000652BCE60
  0000000065300D3B: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000065300D40: 8B 08              mov         ecx,dword ptr [rax]
  0000000065300D42: 89 0A              mov         dword ptr [rdx],ecx
  0000000065300D44: 8B 48 04           mov         ecx,dword ptr [rax+4]
  0000000065300D47: 89 4A 04           mov         dword ptr [rdx+4],ecx
  0000000065300D4A: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065300D4D: 89 4A 08           mov         dword ptr [rdx+8],ecx
  0000000065300D50: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  0000000065300D53: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  0000000065300D56: E9 33 01 00 00     jmp         0000000065300E8E
  0000000065300D5B: 4D 85 E4           test        r12,r12
  0000000065300D5E: 0F 84 2A 01 00 00  je          0000000065300E8E
  0000000065300D64: 41 83 3C 24 04     cmp         dword ptr [r12],4
  0000000065300D69: 0F 8C 1F 01 00 00  jl          0000000065300E8E
  0000000065300D6F: 41 83 7C 24 04 0C  cmp         dword ptr [r12+4],0Ch
  0000000065300D75: 0F 85 13 01 00 00  jne         0000000065300E8E
  0000000065300D7B: 33 ED              xor         ebp,ebp
  0000000065300D7D: 89 6C 24 20        mov         dword ptr [rsp+20h],ebp
  0000000065300D81: 44 8D 4D 02        lea         r9d,[rbp+2]
  0000000065300D85: 45 33 C0           xor         r8d,r8d
  0000000065300D88: 8D 55 FE           lea         edx,[rbp-2]
  0000000065300D8B: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065300D90: FF 15 72 63 39 00  call        qword ptr [0000000065697108h]
  0000000065300D96: E9 F3 00 00 00     jmp         0000000065300E8E
  0000000065300D9B: 4D 85 E4           test        r12,r12
  0000000065300D9E: 0F 84 EA 00 00 00  je          0000000065300E8E
  0000000065300DA4: 41 83 3C 24 04     cmp         dword ptr [r12],4
  0000000065300DA9: 0F 8C DF 00 00 00  jl          0000000065300E8E
  0000000065300DAF: 41 83 7C 24 04 0C  cmp         dword ptr [r12+4],0Ch
  0000000065300DB5: 0F 85 D3 00 00 00  jne         0000000065300E8E
  0000000065300DBB: 48 8B 07           mov         rax,qword ptr [rdi]
  0000000065300DBE: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  0000000065300DC3: 4D 8B CC           mov         r9,r12
  0000000065300DC6: 41 B8 39 00 00 00  mov         r8d,39h
  0000000065300DCC: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000065300DD1: 48 8B CF           mov         rcx,rdi
  0000000065300DD4: FF 90 B8 00 00 00  call        qword ptr [rax+00000000000000B8h]
  0000000065300DDA: 4C 8B 1F           mov         r11,qword ptr [rdi]
  0000000065300DDD: 48 89 74 24 20     mov         qword ptr [rsp+20h],rsi
  0000000065300DE2: 4D 8B CC           mov         r9,r12
  0000000065300DE5: 41 B8 38 00 00 00  mov         r8d,38h
  0000000065300DEB: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000065300DF0: 48 8B CF           mov         rcx,rdi
  0000000065300DF3: 41 FF 93 B8 00 00  call        qword ptr [r11+00000000000000B8h]
                    00
  0000000065300DFA: 49 8D 8C 24 A0 00  lea         rcx,[r12+00000000000000A0h]
                    00 00
  0000000065300E02: E8 D9 FB D9 FF     call        00000000650A09E0
  0000000065300E07: 84 C0              test        al,al
  0000000065300E09: 75 35              jne         0000000065300E40
  0000000065300E0B: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  0000000065300E10: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  0000000065300E18: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000065300E1D: FF 15 ED 79 39 00  call        qword ptr [0000000065698810h]
  0000000065300E23: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000065300E28: 8B 08              mov         ecx,dword ptr [rax]
  0000000065300E2A: 89 0A              mov         dword ptr [rdx],ecx
  0000000065300E2C: 8B 48 04           mov         ecx,dword ptr [rax+4]
  0000000065300E2F: 89 4A 04           mov         dword ptr [rdx+4],ecx
  0000000065300E32: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065300E35: 89 4A 08           mov         dword ptr [rdx+8],ecx
  0000000065300E38: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  0000000065300E3B: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  0000000065300E3E: EB 0B              jmp         0000000065300E4B
  0000000065300E40: 0F 28 44 24 70     movaps      xmm0,xmmword ptr [rsp+70h]
  0000000065300E45: 66 0F 7F 44 24 50  movdqa      xmmword ptr [rsp+50h],xmm0
  0000000065300E4B: 33 ED              xor         ebp,ebp
  0000000065300E4D: 89 6C 24 28        mov         dword ptr [rsp+28h],ebp
  0000000065300E51: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  0000000065300E59: 45 33 C9           xor         r9d,r9d
  0000000065300E5C: 44 8D 45 01        lea         r8d,[rbp+1]
  0000000065300E60: 48 8D 94 24 20 01  lea         rdx,[rsp+0000000000000120h]
                    00 00
  0000000065300E68: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065300E6D: FF 15 BD 5A 39 00  call        qword ptr [0000000065696930h]
  0000000065300E73: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000065300E78: 8B 08              mov         ecx,dword ptr [rax]
  0000000065300E7A: 89 0A              mov         dword ptr [rdx],ecx
  0000000065300E7C: 8B 48 04           mov         ecx,dword ptr [rax+4]
  0000000065300E7F: 89 4A 04           mov         dword ptr [rdx+4],ecx
  0000000065300E82: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065300E85: 89 4A 08           mov         dword ptr [rdx+8],ecx
  0000000065300E88: 8B 40 0C           mov         eax,dword ptr [rax+0Ch]
  0000000065300E8B: 89 42 0C           mov         dword ptr [rdx+0Ch],eax
  0000000065300E8E: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000065300E93: 8B 01              mov         eax,dword ptr [rcx]
  0000000065300E95: 41 89 45 00        mov         dword ptr [r13],eax
  0000000065300E99: 8B 41 04           mov         eax,dword ptr [rcx+4]
  0000000065300E9C: 41 89 45 04        mov         dword ptr [r13+4],eax
  0000000065300EA0: 8B 41 08           mov         eax,dword ptr [rcx+8]
  0000000065300EA3: 41 89 45 08        mov         dword ptr [r13+8],eax
  0000000065300EA7: 8B 41 0C           mov         eax,dword ptr [rcx+0Ch]
  0000000065300EAA: 41 89 45 0C        mov         dword ptr [r13+0Ch],eax
  0000000065300EAE: 49 8B C5           mov         rax,r13
  0000000065300EB1: EB 21              jmp         0000000065300ED4
  0000000065300EB3: 48 8B 84 24 A0 01  mov         rax,qword ptr [rsp+00000000000001A0h]
                    00 00
  0000000065300EBB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000065300EC0: 4D 8B CC           mov         r9,r12
  0000000065300EC3: 44 8B C3           mov         r8d,ebx
  0000000065300EC6: 49 8B D5           mov         rdx,r13
  0000000065300EC9: 48 8B CF           mov         rcx,rdi
  0000000065300ECC: E8 BF 81 05 00     call        0000000065359090
  0000000065300ED1: 49 8B C5           mov         rax,r13
  0000000065300ED4: 4C 8D 9C 24 50 01  lea         r11,[rsp+0000000000000150h]
                    00 00
  0000000065300EDC: 49 8B 5B 30        mov         rbx,qword ptr [r11+30h]
  0000000065300EE0: 49 8B 6B 38        mov         rbp,qword ptr [r11+38h]
  0000000065300EE4: 49 8B 73 40        mov         rsi,qword ptr [r11+40h]
  0000000065300EE8: 49 8B E3           mov         rsp,r11
  0000000065300EEB: 41 5F              pop         r15
  0000000065300EED: 41 5E              pop         r14
  0000000065300EEF: 41 5D              pop         r13
  0000000065300EF1: 41 5C              pop         r12
  0000000065300EF3: 5F                 pop         rdi
  0000000065300EF4: C3                 ret
  0000000065300EF5: 0F 1F 00           nop         dword ptr [rax]
  0000000065300EF8: 38 08              cmp         byte ptr [rax],cl
  0000000065300EFA: 30 00              xor         byte ptr [rax],al
  0000000065300EFC: 1D 0D 30 00 F4     sbb         eax,0F400300Dh
  0000000065300F01: 0B 30              or          esi,dword ptr [rax]
  0000000065300F03: 00 B7 09 30 00 5B  add         byte ptr [rdi+000000005B003009h],dh
  0000000065300F09: 0D 30 00 9B 0D     or          eax,0D9B0030h
  0000000065300F0E: 30 00              xor         byte ptr [rax],al
  0000000065300F10: 8E 0E              mov         cs,word ptr [rsi]
  0000000065300F12: 30 00              xor         byte ptr [rax],al
  0000000065300F14: 00 06              add         byte ptr [rsi],al
  0000000065300F16: 06
  0000000065300F17: 06
  0000000065300F18: 06
  0000000065300F19: 06
  0000000065300F1A: 06
  0000000065300F1B: 06
  0000000065300F1C: 06
  0000000065300F1D: 06
  0000000065300F1E: 06
  0000000065300F1F: 06
  0000000065300F20: 06
  0000000065300F21: 06
  0000000065300F22: 01 06              add         dword ptr [rsi],eax
  0000000065300F24: 06
  0000000065300F25: 06
  0000000065300F26: 06
  0000000065300F27: 06
  0000000065300F28: 06
  0000000065300F29: 06
  0000000065300F2A: 06
  0000000065300F2B: 06
  0000000065300F2C: 06
  0000000065300F2D: 06
  0000000065300F2E: 02 03              add         al,byte ptr [rbx]
  0000000065300F30: 06
  0000000065300F31: 06
  0000000065300F32: 06
  0000000065300F33: 06
  0000000065300F34: 06
  0000000065300F35: 06
  0000000065300F36: 06
  0000000065300F37: 06
  0000000065300F38: 06
  0000000065300F39: 06
  0000000065300F3A: 06
  0000000065300F3B: 06
  0000000065300F3C: 06
  0000000065300F3D: 06
  0000000065300F3E: 06
  0000000065300F3F: 06
  0000000065300F40: 06
  0000000065300F41: 06
  0000000065300F42: 06
  0000000065300F43: 06
  0000000065300F44: 06
  0000000065300F45: 06
  0000000065300F46: 06
  0000000065300F47: 06
  0000000065300F48: 06
  0000000065300F49: 06
  0000000065300F4A: 06
  0000000065300F4B: 06
  0000000065300F4C: 04 06              add         al,6
  0000000065300F4E: 05

  Summary

      695000 .text

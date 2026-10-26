Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400011A0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400011A5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400011AA: 48 81 EC B8 00 00  sub         rsp,0B8h
                    00
  00000001400011B1: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400011BD: 48 8B 05 6C CE B5  mov         rax,qword ptr [0000000143B5E030h]
                    03
  00000001400011C4: 48 33 C4           xor         rax,rsp
  00000001400011C7: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001400011CF: C7 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00
  00000001400011DA: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  00000001400011E2: FF 15 C8 32 E0 01  call        qword ptr [0000000141E044B0h]
  00000001400011E8: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001400011EC: 83 7C 24 20 00     cmp         dword ptr [rsp+20h],0
  00000001400011F1: 75 34              jne         0000000140001227
  00000001400011F3: 48 8D 15 25 3C E1  lea         rdx,[0000000141E14E1Fh]
                    01
  00000001400011FA: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140001202: FF 15 78 32 E0 01  call        qword ptr [0000000141E04480h]
  0000000140001208: 90                 nop
  0000000140001209: 8B 84 24 88 00 00  mov         eax,dword ptr [rsp+0000000000000088h]
                    00
  0000000140001210: 83 C8 01           or          eax,1
  0000000140001213: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  000000014000121A: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140001222: E9 C5 01 00 00     jmp         00000001400013EC
  0000000140001227: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014000122B: 99                 cdq
  000000014000122C: 83 E2 03           and         edx,3
  000000014000122F: 03 C2              add         eax,edx
  0000000140001231: C1 F8 02           sar         eax,2
  0000000140001234: 48 98              cdqe
  0000000140001236: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014000123B: B8 02 00 00 00     mov         eax,2
  0000000140001240: 48 F7 64 24 60     mul         rax,qword ptr [rsp+60h]
  0000000140001245: 48 8B C8           mov         rcx,rax
  0000000140001248: 48 C7 C0 FF FF FF  mov         rax,0FFFFFFFFFFFFFFFFh
                    FF
  000000014000124F: 48 0F 40 C8        cmovo       rcx,rax
  0000000140001253: E8 04 B0 46 01     call        000000014146C25C
  0000000140001258: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000014000125D: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140001263: 74 2A              je          000000014000128F
  0000000140001265: 4C 8B 0D 1C 32 E0  mov         r9,qword ptr [0000000141E04488h]
                    01
  000000014000126C: 44 8B 44 24 60     mov         r8d,dword ptr [rsp+60h]
  0000000140001271: BA 02 00 00 00     mov         edx,2
  0000000140001276: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014000127B: E8 40 0E 00 00     call        00000001400020C0
  0000000140001280: 4C 8B 5C 24 70     mov         r11,qword ptr [rsp+70h]
  0000000140001285: 4C 89 9C 24 98 00  mov         qword ptr [rsp+0000000000000098h],r11
                    00 00
  000000014000128D: EB 0C              jmp         000000014000129B
  000000014000128F: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00 00
  000000014000129B: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001400012A3: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001400012A8: 48 8B 44 24 68     mov         rax,qword ptr [rsp+68h]
  00000001400012AD: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400012B2: C7 44 24 50 00 00  mov         dword ptr [rsp+50h],0
                    00 00
  00000001400012BA: EB 0B              jmp         00000001400012C7
  00000001400012BC: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001400012C0: 83 C0 01           add         eax,1
  00000001400012C3: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  00000001400012C7: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001400012CB: C1 E0 02           shl         eax,2
  00000001400012CE: 3B 44 24 20        cmp         eax,dword ptr [rsp+20h]
  00000001400012D2: 0F 8D 97 00 00 00  jge         000000014000136F
  00000001400012D8: C7 44 24 58 00 00  mov         dword ptr [rsp+58h],0
                    00 00
  00000001400012E0: EB 0B              jmp         00000001400012ED
  00000001400012E2: 8B 44 24 58        mov         eax,dword ptr [rsp+58h]
  00000001400012E6: 83 C0 01           add         eax,1
  00000001400012E9: 89 44 24 58        mov         dword ptr [rsp+58h],eax
  00000001400012ED: 83 7C 24 58 04     cmp         dword ptr [rsp+58h],4
  00000001400012F2: 7D 38              jge         000000014000132C
  00000001400012F4: 8B 4C 24 58        mov         ecx,dword ptr [rsp+58h]
  00000001400012F8: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001400012FC: 44 8D 04 81        lea         r8d,[rcx+rax*4]
  0000000140001300: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140001305: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014000130D: FF 15 7D 31 E0 01  call        qword ptr [0000000141E04490h]
  0000000140001313: 48 8B C8           mov         rcx,rax
  0000000140001316: FF 15 7C 31 E0 01  call        qword ptr [0000000141E04498h]
  000000014000131C: 44 0F B6 D8        movzx       r11d,al
  0000000140001320: 48 63 44 24 58     movsxd      rax,dword ptr [rsp+58h]
  0000000140001325: 44 88 5C 04 30     mov         byte ptr [rsp+rax+30h],r11b
  000000014000132A: EB B6              jmp         00000001400012E2
  000000014000132C: C6 44 24 34 00     mov         byte ptr [rsp+34h],0
  0000000140001331: 4C 8D 44 24 54     lea         r8,[rsp+54h]
  0000000140001336: 48 8D 15 87 3E E1  lea         rdx,[0000000141E151C4h]
                    01
  000000014000133D: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140001342: E8 8D CC 46 01     call        000000014146DFD4
  0000000140001347: 8B 54 24 54        mov         edx,dword ptr [rsp+54h]
  000000014000134B: 48 8D 4C 24 7A     lea         rcx,[rsp+7Ah]
  0000000140001350: FF 15 4A 31 E0 01  call        qword ptr [0000000141E044A0h]
  0000000140001356: 4C 63 5C 24 50     movsxd      r11,dword ptr [rsp+50h]
  000000014000135B: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140001360: 0F B7 44 24 7A     movzx       eax,word ptr [rsp+7Ah]
  0000000140001365: 66 42 89 04 59     mov         word ptr [rcx+r11*2],ax
  000000014000136A: E9 4D FF FF FF     jmp         00000001400012BC
  000000014000136F: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140001374: FF 15 46 31 E0 01  call        qword ptr [0000000141E044C0h]
  000000014000137A: 90                 nop
  000000014000137B: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014000137F: 99                 cdq
  0000000140001380: 83 E2 03           and         edx,3
  0000000140001383: 03 C2              add         eax,edx
  0000000140001385: C1 F8 02           sar         eax,2
  0000000140001388: 44 8B C0           mov         r8d,eax
  000000014000138B: 48 8B 54 24 48     mov         rdx,qword ptr [rsp+48h]
  0000000140001390: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140001395: FF 15 0D 31 E0 01  call        qword ptr [0000000141E044A8h]
  000000014000139B: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001400013A0: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400013A8: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400013B0: E8 E3 C4 46 01     call        000000014146D898
  00000001400013B5: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001400013BA: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000001400013C2: FF 15 10 46 E0 01  call        qword ptr [0000000141E059D8h]
  00000001400013C8: 8B 84 24 88 00 00  mov         eax,dword ptr [rsp+0000000000000088h]
                    00
  00000001400013CF: 83 C8 01           or          eax,1
  00000001400013D2: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  00000001400013D9: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001400013DE: FF 15 FC 45 E0 01  call        qword ptr [0000000141E059E0h]
  00000001400013E4: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000001400013EC: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400013F4: 48 33 CC           xor         rcx,rsp
  00000001400013F7: E8 14 CB 46 01     call        000000014146DF10
  00000001400013FC: 48 81 C4 B8 00 00  add         rsp,0B8h
                    00
  0000000140001403: C3                 ret
  0000000140001404: CC                 int         3

  Summary

     1E02000 .text

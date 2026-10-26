Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140018790: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140018795: 53                 push        rbx
  0000000140018796: 48 81 EC A0 00 00  sub         rsp,0A0h
                    00
  000000014001879D: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400187A9: C7 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00
  00000001400187B4: B9 28 00 00 00     mov         ecx,28h
  00000001400187B9: E8 FE 62 45 01     call        000000014146EABC
  00000001400187BE: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400187C3: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001400187C9: 74 3B              je          0000000140018806
  00000001400187CB: 33 D2              xor         edx,edx
  00000001400187CD: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400187D2: FF 15 78 33 DF 01  call        qword ptr [0000000141E0BB50h]
  00000001400187D8: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  00000001400187DD: 48 8D 05 F4 31 0E  lea         rax,[00000001420FB9D8h]
                    02
  00000001400187E4: 49 89 03           mov         qword ptr [r11],rax
  00000001400187E7: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400187EC: 48 8D 05 A5 31 0E  lea         rax,[00000001420FB998h]
                    02
  00000001400187F3: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  00000001400187F7: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400187FC: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140018804: EB 0C              jmp         0000000140018812
  0000000140018806: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00 00
  0000000140018812: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014001881A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014001881F: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018827: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  000000014001882C: 48 89 81 80 04 00  mov         qword ptr [rcx+0000000000000480h],rax
                    00
  0000000140018833: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014001883B: E8 A0 B0 FF FF     call        00000001400138E0
  0000000140018840: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140018845: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014001884A: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  000000014001884E: 48 63 50 04        movsxd      rdx,dword ptr [rax+4]
  0000000140018852: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140018857: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  000000014001885B: 48 63 48 04        movsxd      rcx,dword ptr [rax+4]
  000000014001885F: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140018864: 48 8D 4C 08 10     lea         rcx,[rax+rcx+10h]
  0000000140018869: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  000000014001886E: 48 8B 44 10 10     mov         rax,qword ptr [rax+rdx+10h]
  0000000140018873: FF 50 20           call        qword ptr [rax+20h]
  0000000140018876: 48 8B D0           mov         rdx,rax
  0000000140018879: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018881: 48 8B 89 80 04 00  mov         rcx,qword ptr [rcx+0000000000000480h]
                    00
  0000000140018888: FF 15 CA 32 DF 01  call        qword ptr [0000000141E0BB58h]
  000000014001888E: 41 B8 2C 01 00 00  mov         r8d,12Ch
  0000000140018894: BA 90 01 00 00     mov         edx,190h
  0000000140018899: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000014001889E: FF 15 04 B8 DE 01  call        qword ptr [0000000141E040A8h]
  00000001400188A4: 48 8B D8           mov         rbx,rax
  00000001400188A7: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400188AF: E8 AC FB FF FF     call        0000000140018460
  00000001400188B4: 48 8B D3           mov         rdx,rbx
  00000001400188B7: 48 8B C8           mov         rcx,rax
  00000001400188BA: FF 15 58 31 DF 01  call        qword ptr [0000000141E0BA18h]
  00000001400188C0: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400188C8: E8 93 FB FF FF     call        0000000140018460
  00000001400188CD: 48 8B D0           mov         rdx,rax
  00000001400188D0: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400188D8: 48 8B 89 80 04 00  mov         rcx,qword ptr [rcx+0000000000000480h]
                    00
  00000001400188DF: FF 15 73 32 DF 01  call        qword ptr [0000000141E0BB58h]
  00000001400188E5: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400188ED: E8 9E A9 FF FF     call        0000000140013290
  00000001400188F2: 48 8B D0           mov         rdx,rax
  00000001400188F5: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400188FD: 48 8B 89 80 04 00  mov         rcx,qword ptr [rcx+0000000000000480h]
                    00
  0000000140018904: FF 15 4E 32 DF 01  call        qword ptr [0000000141E0BB58h]
  000000014001890A: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018912: E8 F9 01 00 00     call        0000000140018B10
  0000000140018917: 48 8B D0           mov         rdx,rax
  000000014001891A: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018922: 48 8B 89 80 04 00  mov         rcx,qword ptr [rcx+0000000000000480h]
                    00
  0000000140018929: FF 15 29 32 DF 01  call        qword ptr [0000000141E0BB58h]
  000000014001892F: B9 60 00 00 00     mov         ecx,60h
  0000000140018934: E8 83 61 45 01     call        000000014146EABC
  0000000140018939: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014001893E: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140018944: 74 6F              je          00000001400189B5
  0000000140018946: 48 8D 44 24 68     lea         rax,[rsp+68h]
  000000014001894B: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140018950: 48 8D 15 61 16 0E  lea         rdx,[00000001420F9FB8h]
                    02
  0000000140018957: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000014001895C: FF 15 1E BB DE 01  call        qword ptr [0000000141E04480h]
  0000000140018962: 90                 nop
  0000000140018963: 8B 84 24 80 00 00  mov         eax,dword ptr [rsp+0000000000000080h]
                    00
  000000014001896A: 83 C8 01           or          eax,1
  000000014001896D: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  0000000140018974: 33 D2              xor         edx,edx
  0000000140018976: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014001897B: E8 F0 E2 52 00     call        0000000140546C70
  0000000140018980: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140018985: 4C 8B 8C 24 B0 00  mov         r9,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014001898D: 4C 8B 84 24 B0 00  mov         r8,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018995: 4D 8B 80 80 04 00  mov         r8,qword ptr [r8+0000000000000480h]
                    00
  000000014001899C: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  00000001400189A1: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400189A6: E8 15 64 CD 00     call        0000000140CEEDC0
  00000001400189AB: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000001400189B3: EB 0C              jmp         00000001400189C1
  00000001400189B5: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00 00
  00000001400189C1: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001400189C9: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400189CE: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000001400189D3: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400189D8: 8B 84 24 80 00 00  mov         eax,dword ptr [rsp+0000000000000080h]
                    00
  00000001400189DF: 83 E0 01           and         eax,1
  00000001400189E2: 85 C0              test        eax,eax
  00000001400189E4: 74 13              je          00000001400189F9
  00000001400189E6: 83 A4 24 80 00 00  and         dword ptr [rsp+0000000000000080h],0FFFFFFFEh
                    00 FE
  00000001400189EE: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001400189F3: FF 15 E7 CF DE 01  call        qword ptr [0000000141E059E0h]
  00000001400189F9: 4C 8B 44 24 30     mov         r8,qword ptr [rsp+30h]
  00000001400189FE: BA 02 00 00 00     mov         edx,2
  0000000140018A03: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018A0B: FF 15 D7 30 DF 01  call        qword ptr [0000000141E0BAE8h]
  0000000140018A11: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018A19: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140018A1C: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018A24: FF 90 A0 02 00 00  call        qword ptr [rax+00000000000002A0h]
  0000000140018A2A: 48 8B D0           mov         rdx,rax
  0000000140018A2D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140018A32: E8 99 69 CD 00     call        0000000140CEF3D0
  0000000140018A37: 4C 8B 9C 24 B0 00  mov         r11,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018A3F: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140018A44: 49 89 83 78 04 00  mov         qword ptr [r11+0000000000000478h],rax
                    00
  0000000140018A4B: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018A53: 48 8B 89 78 04 00  mov         rcx,qword ptr [rcx+0000000000000478h]
                    00
  0000000140018A5A: FF 15 90 30 DF 01  call        qword ptr [0000000141E0BAF0h]
  0000000140018A60: 48 8B D0           mov         rdx,rax
  0000000140018A63: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018A6B: 48 8B 89 E8 00 00  mov         rcx,qword ptr [rcx+00000000000000E8h]
                    00
  0000000140018A72: FF 15 A8 34 DF 01  call        qword ptr [0000000141E0BF20h]
  0000000140018A78: 4C 8B 9C 24 B0 00  mov         r11,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018A80: 49 C7 83 40 04 00  mov         qword ptr [r11+0000000000000440h],0
                    00 00 00 00 00
  0000000140018A8B: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018A93: E8 58 96 01 00     call        00000001400320F0
  0000000140018A98: BA 5E 01 00 00     mov         edx,15Eh
  0000000140018A9D: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018AA5: 48 8B 89 78 04 00  mov         rcx,qword ptr [rcx+0000000000000478h]
                    00
  0000000140018AAC: FF 15 6E 2F DF 01  call        qword ptr [0000000141E0BA20h]
  0000000140018AB2: BA 9A 01 00 00     mov         edx,19Ah
  0000000140018AB7: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140018ABF: 48 8B 89 78 04 00  mov         rcx,qword ptr [rcx+0000000000000478h]
                    00
  0000000140018AC6: FF 15 5C 2F DF 01  call        qword ptr [0000000141E0BA28h]
  0000000140018ACC: 48 81 C4 A0 00 00  add         rsp,0A0h
                    00
  0000000140018AD3: 5B                 pop         rbx
  0000000140018AD4: C3                 ret

  Summary

     1E02000 .text

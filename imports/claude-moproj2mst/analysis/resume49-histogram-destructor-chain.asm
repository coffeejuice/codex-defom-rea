Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C2C600: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C2C605: 56                 push        rsi
  0000000140C2C606: 57                 push        rdi
  0000000140C2C607: 48 81 EC C8 01 00  sub         rsp,1C8h
                    00
  0000000140C2C60E: 48 C7 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140C2C61A: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C2C622: 48 8D 05 9F 19 C1  lea         rax,[000000014283DFC8h]
                    01
  0000000140C2C629: 48 89 01           mov         qword ptr [rcx],rax
  0000000140C2C62C: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C2C634: 48 8D 05 3D 18 C1  lea         rax,[000000014283DE78h]
                    01
  0000000140C2C63B: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140C2C63F: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C2C647: 48 81 C1 28 01 00  add         rcx,128h
                    00
  0000000140C2C64E: E8 9D E8 FE FF     call        0000000140C1AEF0
  0000000140C2C653: 48 85 C0           test        rax,rax
  0000000140C2C656: 74 3A              je          0000000140C2C692
  0000000140C2C658: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C2C660: 48 81 C1 28 01 00  add         rcx,128h
                    00
  0000000140C2C667: E8 84 E8 FE FF     call        0000000140C1AEF0
  0000000140C2C66C: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140C2C671: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C2C676: FF 15 A4 8F 1D 01  call        qword ptr [0000000141E05620h]
  0000000140C2C67C: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C2C684: 48 81 C1 28 01 00  add         rcx,128h
                    00
  0000000140C2C68B: 33 D2              xor         edx,edx
  0000000140C2C68D: E8 8E 02 09 00     call        0000000140CBC920
  0000000140C2C692: 45 33 C0           xor         r8d,r8d
  0000000140C2C695: 33 D2              xor         edx,edx
  0000000140C2C697: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140C2C69C: FF 15 C6 7A 1D 01  call        qword ptr [0000000141E04168h]
  0000000140C2C6A2: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  0000000140C2C6AA: 48 8B F0           mov         rsi,rax
  0000000140C2C6AD: B9 10 00 00 00     mov         ecx,10h
  0000000140C2C6B2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2C6B4: 45 33 C0           xor         r8d,r8d
  0000000140C2C6B7: 33 D2              xor         edx,edx
  0000000140C2C6B9: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140C2C6BE: FF 15 A4 7A 1D 01  call        qword ptr [0000000141E04168h]
  0000000140C2C6C4: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140C2C6CC: 48 8B F0           mov         rsi,rax
  0000000140C2C6CF: B9 10 00 00 00     mov         ecx,10h
  0000000140C2C6D4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2C6D6: 45 33 C0           xor         r8d,r8d
  0000000140C2C6D9: 33 D2              xor         edx,edx
  0000000140C2C6DB: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140C2C6E3: FF 15 7F 7A 1D 01  call        qword ptr [0000000141E04168h]
  0000000140C2C6E9: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140C2C6F1: 48 8B F0           mov         rsi,rax
  0000000140C2C6F4: B9 10 00 00 00     mov         ecx,10h
  0000000140C2C6F9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2C6FB: 45 33 C0           xor         r8d,r8d
  0000000140C2C6FE: 33 D2              xor         edx,edx
  0000000140C2C700: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140C2C708: FF 15 5A 7A 1D 01  call        qword ptr [0000000141E04168h]
  0000000140C2C70E: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140C2C716: 48 8B F0           mov         rsi,rax
  0000000140C2C719: B9 10 00 00 00     mov         ecx,10h
  0000000140C2C71E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2C720: 45 33 C0           xor         r8d,r8d
  0000000140C2C723: 33 D2              xor         edx,edx
  0000000140C2C725: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140C2C72D: FF 15 35 7A 1D 01  call        qword ptr [0000000141E04168h]
  0000000140C2C733: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  0000000140C2C73B: 48 8B F0           mov         rsi,rax
  0000000140C2C73E: B9 10 00 00 00     mov         ecx,10h
  0000000140C2C743: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2C745: 45 33 C0           xor         r8d,r8d
  0000000140C2C748: 33 D2              xor         edx,edx
  0000000140C2C74A: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140C2C752: FF 15 10 7A 1D 01  call        qword ptr [0000000141E04168h]
  0000000140C2C758: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  0000000140C2C760: 48 8B F0           mov         rsi,rax
  0000000140C2C763: B9 10 00 00 00     mov         ecx,10h
  0000000140C2C768: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2C76A: 45 33 C0           xor         r8d,r8d
  0000000140C2C76D: 33 D2              xor         edx,edx
  0000000140C2C76F: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140C2C777: FF 15 EB 79 1D 01  call        qword ptr [0000000141E04168h]
  0000000140C2C77D: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  0000000140C2C785: 48 8B F0           mov         rsi,rax
  0000000140C2C788: B9 10 00 00 00     mov         ecx,10h
  0000000140C2C78D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2C78F: 45 33 C0           xor         r8d,r8d
  0000000140C2C792: 33 D2              xor         edx,edx
  0000000140C2C794: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000140C2C79C: FF 15 C6 79 1D 01  call        qword ptr [0000000141E04168h]
  0000000140C2C7A2: 48 8D BC 24 30 01  lea         rdi,[rsp+0000000000000130h]
                    00 00
  0000000140C2C7AA: 48 8B F0           mov         rsi,rax
  0000000140C2C7AD: B9 10 00 00 00     mov         ecx,10h
  0000000140C2C7B2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2C7B4: 45 33 C0           xor         r8d,r8d
  0000000140C2C7B7: 33 D2              xor         edx,edx
  0000000140C2C7B9: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140C2C7C1: FF 15 A1 79 1D 01  call        qword ptr [0000000141E04168h]
  0000000140C2C7C7: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  0000000140C2C7CF: 48 8B F0           mov         rsi,rax
  0000000140C2C7D2: B9 10 00 00 00     mov         ecx,10h
  0000000140C2C7D7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2C7D9: 45 33 C0           xor         r8d,r8d
  0000000140C2C7DC: 33 D2              xor         edx,edx
  0000000140C2C7DE: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140C2C7E6: FF 15 7C 79 1D 01  call        qword ptr [0000000141E04168h]
  0000000140C2C7EC: 48 8D BC 24 10 01  lea         rdi,[rsp+0000000000000110h]
                    00 00
  0000000140C2C7F4: 48 8B F0           mov         rsi,rax
  0000000140C2C7F7: B9 10 00 00 00     mov         ecx,10h
  0000000140C2C7FC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2C7FE: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140C2C806: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C2C80B: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140C2C813: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140C2C818: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  0000000140C2C820: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140C2C825: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  0000000140C2C82D: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140C2C832: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  0000000140C2C83A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C2C83F: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  0000000140C2C847: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C2C84C: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  0000000140C2C854: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140C2C859: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  0000000140C2C861: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140C2C866: 4C 8D 8C 24 20 01  lea         r9,[rsp+0000000000000120h]
                    00 00
  0000000140C2C86E: 4C 8D 84 24 10 01  lea         r8,[rsp+0000000000000110h]
                    00 00
  0000000140C2C876: 48 8D 15 5B 11 C1  lea         rdx,[000000014283D9D8h]
                    01
  0000000140C2C87D: 48 8D 0D 7C 9E 14  lea         rcx,[0000000143D76700h]
                    03
  0000000140C2C884: E8 37 2B 6A FF     call        00000001402CF3C0
  0000000140C2C889: 90                 nop
  0000000140C2C88A: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C2C892: 48 81 C1 28 01 00  add         rcx,128h
                    00
  0000000140C2C899: E8 62 00 09 00     call        0000000140CBC900
  0000000140C2C89E: 90                 nop
  0000000140C2C89F: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140C2C8A7: E8 94 C6 00 00     call        0000000140C38F40
  0000000140C2C8AC: 48 81 C4 C8 01 00  add         rsp,1C8h
                    00
  0000000140C2C8B3: 5F                 pop         rdi
  0000000140C2C8B4: 5E                 pop         rsi
  0000000140C2C8B5: C3                 ret
  0000000140C2C8B6: CC                 int         3
  0000000140C2C8B7: CC                 int         3
  0000000140C2C8B8: CC                 int         3
  0000000140C2C8B9: CC                 int         3
  0000000140C2C8BA: CC                 int         3
  0000000140C2C8BB: CC                 int         3
  0000000140C2C8BC: CC                 int         3
  0000000140C2C8BD: CC                 int         3
  0000000140C2C8BE: CC                 int         3
  0000000140C2C8BF: CC                 int         3
  0000000140C2C8C0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C2C8C5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C2C8CA: 48 83 EC 58        sub         rsp,58h
  0000000140C2C8CE: 48 C7 44 24 40 FE  mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140C2C8D7: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140C2C8DC: E8 0F E6 FE FF     call        0000000140C1AEF0
  0000000140C2C8E1: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C2C8E9: 4C 8D 0D 08 FB CB  lea         r9,[00000001438EC3F8h]
                    02
  0000000140C2C8F0: 4C 8D 05 51 8E 93  lea         r8,[0000000143565748h]
                    02
  0000000140C2C8F7: 33 D2              xor         edx,edx
  0000000140C2C8F9: 48 8B C8           mov         rcx,rax
  0000000140C2C8FC: E8 FB 2F 84 00     call        000000014146F8FC
  0000000140C2C901: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C2C906: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140C2C90C: 74 4C              je          0000000140C2C95A
  0000000140C2C90E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C2C913: 48 81 C1 28 01 00  add         rcx,128h
                    00
  0000000140C2C91A: E8 D1 E5 FE FF     call        0000000140C1AEF0
  0000000140C2C91F: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140C2C924: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C2C929: 48 81 C1 28 01 00  add         rcx,128h
                    00
  0000000140C2C930: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140C2C935: E8 E6 FF 08 00     call        0000000140CBC920
  0000000140C2C93A: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140C2C940: 74 18              je          0000000140C2C95A
  0000000140C2C942: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140C2C947: 48 39 44 24 38     cmp         qword ptr [rsp+38h],rax
  0000000140C2C94C: 74 0C              je          0000000140C2C95A
  0000000140C2C94E: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140C2C953: FF 15 C7 8C 1D 01  call        qword ptr [0000000141E05620h]
  0000000140C2C959: 90                 nop
  0000000140C2C95A: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140C2C95F: E8 9C FF 08 00     call        0000000140CBC900
  0000000140C2C964: 48 83 C4 58        add         rsp,58h
  0000000140C2C968: C3                 ret
  0000000140C2C969: CC                 int         3
  0000000140C2C96A: CC                 int         3
  0000000140C2C96B: CC                 int         3
  0000000140C2C96C: CC                 int         3
  0000000140C2C96D: CC                 int         3
  0000000140C2C96E: CC                 int         3
  0000000140C2C96F: CC                 int         3
  0000000140C2C970: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C2C975: 48 83 EC 48        sub         rsp,48h
  0000000140C2C979: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C2C97E: 48 83 C1 10        add         rcx,10h
  0000000140C2C982: FF 15 F0 F1 1D 01  call        qword ptr [0000000141E0BB78h]
  0000000140C2C988: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C2C98D: 48 81 C1 28 01 00  add         rcx,128h
                    00
  0000000140C2C994: E8 57 E5 FE FF     call        0000000140C1AEF0
  0000000140C2C999: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C2C9A1: 4C 8D 0D 80 33 CC  lea         r9,[00000001438EFD28h]
                    02
  0000000140C2C9A8: 4C 8D 05 49 FA CB  lea         r8,[00000001438EC3F8h]
                    02
  0000000140C2C9AF: 33 D2              xor         edx,edx
  0000000140C2C9B1: 48 8B C8           mov         rcx,rax
  0000000140C2C9B4: E8 43 2F 84 00     call        000000014146F8FC
  0000000140C2C9B9: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140C2C9BE: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140C2C9C4: 74 0B              je          0000000140C2C9D1
  0000000140C2C9C6: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C2C9CB: FF 15 4F 8C 1D 01  call        qword ptr [0000000141E05620h]
  0000000140C2C9D1: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C2C9D6: FF 15 44 8C 1D 01  call        qword ptr [0000000141E05620h]
  0000000140C2C9DC: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140C2C9E1: E8 1A C8 01 00     call        0000000140C49200
  0000000140C2C9E6: 48 83 C4 48        add         rsp,48h
  0000000140C2C9EA: C3                 ret
  0000000140C2C9EB: CC                 int         3
  0000000140C2C9EC: CC                 int         3
  0000000140C2C9ED: CC                 int         3
  0000000140C2C9EE: CC                 int         3
  0000000140C2C9EF: CC                 int         3
  0000000140C2C9F0: CC                 int         3
  0000000140C2C9F1: CC                 int         3
  0000000140C2C9F2: CC                 int         3
  0000000140C2C9F3: CC                 int         3
  0000000140C2C9F4: CC                 int         3
  0000000140C2C9F5: CC                 int         3
  0000000140C2C9F6: CC                 int         3
  0000000140C2C9F7: CC                 int         3
  0000000140C2C9F8: CC                 int         3
  0000000140C2C9F9: CC                 int         3
  0000000140C2C9FA: CC                 int         3
  0000000140C2C9FB: CC                 int         3
  0000000140C2C9FC: CC                 int         3
  0000000140C2C9FD: CC                 int         3
  0000000140C2C9FE: CC                 int         3
  0000000140C2C9FF: CC                 int         3
  0000000140C2CA00: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C2CA05: 48 83 EC 28        sub         rsp,28h
  0000000140C2CA09: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140C2CA0E: 48 83 C1 10        add         rcx,10h
  0000000140C2CA12: FF 15 C8 AA 1D 01  call        qword ptr [0000000141E074E0h]
  0000000140C2CA18: 48 83 C4 28        add         rsp,28h
  0000000140C2CA1C: C3                 ret
  0000000140C2CA1D: CC                 int         3
  0000000140C2CA1E: CC                 int         3
  0000000140C2CA1F: CC                 int         3
  0000000140C2CA20: CC                 int         3
  0000000140C2CA21: CC                 int         3
  0000000140C2CA22: CC                 int         3
  0000000140C2CA23: CC                 int         3
  0000000140C2CA24: CC                 int         3
  0000000140C2CA25: CC                 int         3
  0000000140C2CA26: CC                 int         3
  0000000140C2CA27: CC                 int         3
  0000000140C2CA28: CC                 int         3
  0000000140C2CA29: CC                 int         3
  0000000140C2CA2A: CC                 int         3
  0000000140C2CA2B: CC                 int         3
  0000000140C2CA2C: CC                 int         3
  0000000140C2CA2D: CC                 int         3
  0000000140C2CA2E: CC                 int         3
  0000000140C2CA2F: CC                 int         3
  0000000140C2CA30: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C2CA35: 56                 push        rsi
  0000000140C2CA36: 57                 push        rdi
  0000000140C2CA37: 48 81 EC 08 01 00  sub         rsp,108h
                    00
  0000000140C2CA3E: C7 44 24 24 00 00  mov         dword ptr [rsp+24h],0
                    00 00
  0000000140C2CA46: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  0000000140C2CA4E: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CA56: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CA59: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CA61: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  0000000140C2CA67: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000140C2CA6B: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CA73: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CA76: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CA7E: FF 90 D0 00 00 00  call        qword ptr [rax+00000000000000D0h]
  0000000140C2CA84: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140C2CA88: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CA90: E8 7B 0F 00 00     call        0000000140C2DA10
  0000000140C2CA95: 0F B6 C0           movzx       eax,al
  0000000140C2CA98: 85 C0              test        eax,eax
  0000000140C2CA9A: 0F 84 BB 00 00 00  je          0000000140C2CB5B
  0000000140C2CAA0: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CAA8: 48 8B 80 F8 00 00  mov         rax,qword ptr [rax+00000000000000F8h]
                    00
  0000000140C2CAAF: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CAB7: 48 8B 89 F8 00 00  mov         rcx,qword ptr [rcx+00000000000000F8h]
                    00
  0000000140C2CABE: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CAC1: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  0000000140C2CAC7: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  0000000140C2CACB: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CAD3: 48 8B 80 F8 00 00  mov         rax,qword ptr [rax+00000000000000F8h]
                    00
  0000000140C2CADA: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CAE2: 48 8B 89 F8 00 00  mov         rcx,qword ptr [rcx+00000000000000F8h]
                    00
  0000000140C2CAE9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CAEC: FF 90 D0 00 00 00  call        qword ptr [rax+00000000000000D0h]
  0000000140C2CAF2: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140C2CAF6: 66 0F 6E 54 24 38  movd        xmm2,dword ptr [rsp+38h]
  0000000140C2CAFC: F3 0F E6 D2        cvtdq2pd    xmm2,xmm2
  0000000140C2CB00: 66 0F 6E 4C 24 24  movd        xmm1,dword ptr [rsp+24h]
  0000000140C2CB06: F3 0F E6 C9        cvtdq2pd    xmm1,xmm1
  0000000140C2CB0A: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140C2CB0F: FF 15 C3 8C 1D 01  call        qword ptr [0000000141E057D8h]
  0000000140C2CB15: 48 8D BC 24 B0 00  lea         rdi,[rsp+00000000000000B0h]
                    00 00
  0000000140C2CB1D: 48 8B F0           mov         rsi,rax
  0000000140C2CB20: B9 10 00 00 00     mov         ecx,10h
  0000000140C2CB25: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2CB27: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CB2F: 48 8B 80 F8 00 00  mov         rax,qword ptr [rax+00000000000000F8h]
                    00
  0000000140C2CB36: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CB3E: 48 8B 89 F8 00 00  mov         rcx,qword ptr [rcx+00000000000000F8h]
                    00
  0000000140C2CB45: 48 83 C1 10        add         rcx,10h
  0000000140C2CB49: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C2CB4D: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  0000000140C2CB55: FF 90 20 01 00 00  call        qword ptr [rax+0000000000000120h]
  0000000140C2CB5B: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CB63: 48 8B 80 E8 00 00  mov         rax,qword ptr [rax+00000000000000E8h]
                    00
  0000000140C2CB6A: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CB72: 48 8B 89 E8 00 00  mov         rcx,qword ptr [rcx+00000000000000E8h]
                    00
  0000000140C2CB79: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CB7C: FF 90 D0 00 00 00  call        qword ptr [rax+00000000000000D0h]
  0000000140C2CB82: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140C2CB86: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140C2CB8A: 83 E8 04           sub         eax,4
  0000000140C2CB8D: 2B 44 24 38        sub         eax,dword ptr [rsp+38h]
  0000000140C2CB91: 83 E8 04           sub         eax,4
  0000000140C2CB94: 2B 44 24 30        sub         eax,dword ptr [rsp+30h]
  0000000140C2CB98: 83 E8 04           sub         eax,4
  0000000140C2CB9B: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  0000000140C2CB9F: 8B 54 24 44        mov         edx,dword ptr [rsp+44h]
  0000000140C2CBA3: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CBAB: 48 8B 89 F0 00 00  mov         rcx,qword ptr [rcx+00000000000000F0h]
                    00
  0000000140C2CBB2: E8 B9 67 FF FF     call        0000000140C23370
  0000000140C2CBB7: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CBBF: 48 8B 80 F0 00 00  mov         rax,qword ptr [rax+00000000000000F0h]
                    00
  0000000140C2CBC6: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CBCE: 48 8B 89 F0 00 00  mov         rcx,qword ptr [rcx+00000000000000F0h]
                    00
  0000000140C2CBD5: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CBD8: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  0000000140C2CBDE: 89 44 24 2C        mov         dword ptr [rsp+2Ch],eax
  0000000140C2CBE2: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140C2CBE6: 83 E8 04           sub         eax,4
  0000000140C2CBE9: 2B 44 24 2C        sub         eax,dword ptr [rsp+2Ch]
  0000000140C2CBED: 83 E8 04           sub         eax,4
  0000000140C2CBF0: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  0000000140C2CBF4: 66 0F 6E 54 24 30  movd        xmm2,dword ptr [rsp+30h]
  0000000140C2CBFA: F3 0F E6 D2        cvtdq2pd    xmm2,xmm2
  0000000140C2CBFE: 66 0F 6E 4C 24 34  movd        xmm1,dword ptr [rsp+34h]
  0000000140C2CC04: F3 0F E6 C9        cvtdq2pd    xmm1,xmm1
  0000000140C2CC08: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140C2CC0D: FF 15 C5 8B 1D 01  call        qword ptr [0000000141E057D8h]
  0000000140C2CC13: 48 8D BC 24 C0 00  lea         rdi,[rsp+00000000000000C0h]
                    00 00
  0000000140C2CC1B: 48 8B F0           mov         rsi,rax
  0000000140C2CC1E: B9 10 00 00 00     mov         ecx,10h
  0000000140C2CC23: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2CC25: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CC2D: 48 8B 80 E8 00 00  mov         rax,qword ptr [rax+00000000000000E8h]
                    00
  0000000140C2CC34: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CC3C: 48 8B 89 E8 00 00  mov         rcx,qword ptr [rcx+00000000000000E8h]
                    00
  0000000140C2CC43: 48 83 C1 10        add         rcx,10h
  0000000140C2CC47: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C2CC4B: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  0000000140C2CC53: FF 90 20 01 00 00  call        qword ptr [rax+0000000000000120h]
  0000000140C2CC59: 8B 54 24 34        mov         edx,dword ptr [rsp+34h]
  0000000140C2CC5D: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CC65: 48 8B 89 E8 00 00  mov         rcx,qword ptr [rcx+00000000000000E8h]
                    00
  0000000140C2CC6C: E8 FF 66 FF FF     call        0000000140C23370
  0000000140C2CC71: 66 0F 6E 54 24 44  movd        xmm2,dword ptr [rsp+44h]
  0000000140C2CC77: F3 0F E6 D2        cvtdq2pd    xmm2,xmm2
  0000000140C2CC7B: 66 0F 6E 4C 24 2C  movd        xmm1,dword ptr [rsp+2Ch]
  0000000140C2CC81: F3 0F E6 C9        cvtdq2pd    xmm1,xmm1
  0000000140C2CC85: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140C2CC8A: FF 15 48 8B 1D 01  call        qword ptr [0000000141E057D8h]
  0000000140C2CC90: 48 8D BC 24 D0 00  lea         rdi,[rsp+00000000000000D0h]
                    00 00
  0000000140C2CC98: 48 8B F0           mov         rsi,rax
  0000000140C2CC9B: B9 10 00 00 00     mov         ecx,10h
  0000000140C2CCA0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2CCA2: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CCAA: 48 8B 80 F0 00 00  mov         rax,qword ptr [rax+00000000000000F0h]
                    00
  0000000140C2CCB1: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CCB9: 48 8B 89 F0 00 00  mov         rcx,qword ptr [rcx+00000000000000F0h]
                    00
  0000000140C2CCC0: 48 83 C1 10        add         rcx,10h
  0000000140C2CCC4: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C2CCC8: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  0000000140C2CCD0: FF 90 20 01 00 00  call        qword ptr [rax+0000000000000120h]
  0000000140C2CCD6: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CCDE: 48 8B 80 00 01 00  mov         rax,qword ptr [rax+0000000000000100h]
                    00
  0000000140C2CCE5: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CCED: 48 8B 89 00 01 00  mov         rcx,qword ptr [rcx+0000000000000100h]
                    00
  0000000140C2CCF4: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CCF7: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  0000000140C2CCFD: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140C2CD01: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CD09: 48 8B 80 00 01 00  mov         rax,qword ptr [rax+0000000000000100h]
                    00
  0000000140C2CD10: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CD18: 48 8B 89 00 01 00  mov         rcx,qword ptr [rcx+0000000000000100h]
                    00
  0000000140C2CD1F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CD22: FF 90 D0 00 00 00  call        qword ptr [rax+00000000000000D0h]
  0000000140C2CD28: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140C2CD2C: 66 0F 6E 54 24 28  movd        xmm2,dword ptr [rsp+28h]
  0000000140C2CD32: F3 0F E6 D2        cvtdq2pd    xmm2,xmm2
  0000000140C2CD36: 66 0F 6E 4C 24 20  movd        xmm1,dword ptr [rsp+20h]
  0000000140C2CD3C: F3 0F E6 C9        cvtdq2pd    xmm1,xmm1
  0000000140C2CD40: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140C2CD45: FF 15 8D 8A 1D 01  call        qword ptr [0000000141E057D8h]
  0000000140C2CD4B: 48 8D BC 24 E0 00  lea         rdi,[rsp+00000000000000E0h]
                    00 00
  0000000140C2CD53: 48 8B F0           mov         rsi,rax
  0000000140C2CD56: B9 10 00 00 00     mov         ecx,10h
  0000000140C2CD5B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2CD5D: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CD65: 48 8B 80 00 01 00  mov         rax,qword ptr [rax+0000000000000100h]
                    00
  0000000140C2CD6C: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CD74: 48 8B 89 00 01 00  mov         rcx,qword ptr [rcx+0000000000000100h]
                    00
  0000000140C2CD7B: 48 83 C1 10        add         rcx,10h
  0000000140C2CD7F: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C2CD83: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  0000000140C2CD8B: FF 90 20 01 00 00  call        qword ptr [rax+0000000000000120h]
  0000000140C2CD91: 48 8D 84 24 88 00  lea         rax,[rsp+0000000000000088h]
                    00 00
  0000000140C2CD99: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140C2CDA1: 45 33 C0           xor         r8d,r8d
  0000000140C2CDA4: BA 01 00 00 00     mov         edx,1
  0000000140C2CDA9: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CDB1: 48 8B 89 F0 00 00  mov         rcx,qword ptr [rcx+00000000000000F0h]
                    00
  0000000140C2CDB8: E8 33 6C FF FF     call        0000000140C239F0
  0000000140C2CDBD: 8B F0              mov         esi,eax
  0000000140C2CDBF: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CDC7: 48 8B 89 F0 00 00  mov         rcx,qword ptr [rcx+00000000000000F0h]
                    00
  0000000140C2CDCE: E8 1D B4 FF FF     call        0000000140C281F0
  0000000140C2CDD3: 0F AF F0           imul        esi,eax
  0000000140C2CDD6: 45 33 C0           xor         r8d,r8d
  0000000140C2CDD9: BA 01 00 00 00     mov         edx,1
  0000000140C2CDDE: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CDE6: 48 8B 89 E8 00 00  mov         rcx,qword ptr [rcx+00000000000000E8h]
                    00
  0000000140C2CDED: E8 CE 6A FF FF     call        0000000140C238C0
  0000000140C2CDF2: 8B F8              mov         edi,eax
  0000000140C2CDF4: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CDFC: 48 8B 89 E8 00 00  mov         rcx,qword ptr [rcx+00000000000000E8h]
                    00
  0000000140C2CE03: E8 E8 B3 FF FF     call        0000000140C281F0
  0000000140C2CE08: 8B D7              mov         edx,edi
  0000000140C2CE0A: 0F AF D0           imul        edx,eax
  0000000140C2CE0D: 44 8B C6           mov         r8d,esi
  0000000140C2CE10: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140C2CE18: FF 15 9A 77 1D 01  call        qword ptr [0000000141E045B8h]
  0000000140C2CE1E: 48 8B D0           mov         rdx,rax
  0000000140C2CE21: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140C2CE29: FF 15 99 88 1D 01  call        qword ptr [0000000141E056C8h]
  0000000140C2CE2F: 48 8D BC 24 F0 00  lea         rdi,[rsp+00000000000000F0h]
                    00 00
  0000000140C2CE37: 48 8B F0           mov         rsi,rax
  0000000140C2CE3A: B9 10 00 00 00     mov         ecx,10h
  0000000140C2CE3F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140C2CE41: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CE49: 48 8B 80 08 01 00  mov         rax,qword ptr [rax+0000000000000108h]
                    00
  0000000140C2CE50: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CE58: 48 8B 89 08 01 00  mov         rcx,qword ptr [rcx+0000000000000108h]
                    00
  0000000140C2CE5F: 48 83 C1 10        add         rcx,10h
  0000000140C2CE63: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  0000000140C2CE67: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  0000000140C2CE6F: FF 90 20 01 00 00  call        qword ptr [rax+0000000000000120h]
  0000000140C2CE75: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CE7D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CE80: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140C2CE88: FF 90 B8 00 00 00  call        qword ptr [rax+00000000000000B8h]
  0000000140C2CE8E: 48 81 C4 08 01 00  add         rsp,108h
                    00
  0000000140C2CE95: 5F                 pop         rdi
  0000000140C2CE96: 5E                 pop         rsi
  0000000140C2CE97: C3                 ret
  0000000140C2CE98: CC                 int         3
  0000000140C2CE99: CC                 int         3
  0000000140C2CE9A: CC                 int         3
  0000000140C2CE9B: CC                 int         3
  0000000140C2CE9C: CC                 int         3
  0000000140C2CE9D: CC                 int         3
  0000000140C2CE9E: CC                 int         3
  0000000140C2CE9F: CC                 int         3
  0000000140C2CEA0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C2CEA5: 53                 push        rbx
  0000000140C2CEA6: 48 81 EC C0 00 00  sub         rsp,0C0h
                    00
  0000000140C2CEAD: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CEB5: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CEB8: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CEC0: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  0000000140C2CEC6: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  0000000140C2CECA: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CED2: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CED5: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CEDD: FF 90 D0 00 00 00  call        qword ptr [rax+00000000000000D0h]
  0000000140C2CEE3: 89 44 24 5C        mov         dword ptr [rsp+5Ch],eax
  0000000140C2CEE7: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CEEF: E8 1C 0B 00 00     call        0000000140C2DA10
  0000000140C2CEF4: 0F B6 C0           movzx       eax,al
  0000000140C2CEF7: 85 C0              test        eax,eax
  0000000140C2CEF9: 0F 84 50 01 00 00  je          0000000140C2D04F
  0000000140C2CEFF: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CF07: 48 8B 89 F8 00 00  mov         rcx,qword ptr [rcx+00000000000000F8h]
                    00
  0000000140C2CF0E: 48 83 C1 10        add         rcx,10h
  0000000140C2CF12: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000140C2CF1A: FF 15 08 A6 1D 01  call        qword ptr [0000000141E07528h]
  0000000140C2CF20: 48 8B C8           mov         rcx,rax
  0000000140C2CF23: FF 15 67 78 1D 01  call        qword ptr [0000000141E04790h]
  0000000140C2CF29: F2 0F 2C C0        cvttsd2si   eax,xmm0
  0000000140C2CF2D: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140C2CF31: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CF39: 48 8B 89 F8 00 00  mov         rcx,qword ptr [rcx+00000000000000F8h]
                    00
  0000000140C2CF40: 48 83 C1 10        add         rcx,10h
  0000000140C2CF44: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  0000000140C2CF4C: FF 15 D6 A5 1D 01  call        qword ptr [0000000141E07528h]
  0000000140C2CF52: 48 8B C8           mov         rcx,rax
  0000000140C2CF55: FF 15 45 78 1D 01  call        qword ptr [0000000141E047A0h]
  0000000140C2CF5B: F2 0F 2C C0        cvttsd2si   eax,xmm0
  0000000140C2CF5F: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140C2CF63: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CF6B: 48 8B 80 F8 00 00  mov         rax,qword ptr [rax+00000000000000F8h]
                    00
  0000000140C2CF72: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CF7A: 48 8B 89 F8 00 00  mov         rcx,qword ptr [rcx+00000000000000F8h]
                    00
  0000000140C2CF81: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CF84: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  0000000140C2CF8A: 8B 5C 24 38        mov         ebx,dword ptr [rsp+38h]
  0000000140C2CF8E: 03 D8              add         ebx,eax
  0000000140C2CF90: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CF98: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CF9B: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CFA3: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  0000000140C2CFA9: 83 E8 04           sub         eax,4
  0000000140C2CFAC: 3B D8              cmp         ebx,eax
  0000000140C2CFAE: 7E 51              jle         0000000140C2D001
  0000000140C2CFB0: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CFB8: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CFBB: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CFC3: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  0000000140C2CFC9: 8B D8              mov         ebx,eax
  0000000140C2CFCB: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CFD3: 48 8B 80 F8 00 00  mov         rax,qword ptr [rax+00000000000000F8h]
                    00
  0000000140C2CFDA: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140C2CFE2: 48 8B 89 F8 00 00  mov         rcx,qword ptr [rcx+00000000000000F8h]
                    00
  0000000140C2CFE9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C2CFEC: FF 90 C8 00 00 00  call        qword ptr [rax+00000000000000C8h]
  0000000140C2CFF2: 44 8B D8           mov         r11d,eax
  0000000140C2CFF5: 8B C3              mov         eax,ebx
  0000000140C2CFF7: 41 2B C3           sub         eax,r11d
  0000000140C2CFFA: 83 E8 04           sub         eax,4
  0000000140C2CFFD: 89 44 24 38        mov         dword ptr [rsp+38h],eax

  Summary

     1E02000 .text

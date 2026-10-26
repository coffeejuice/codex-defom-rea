Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file analysis\resume48-distortion-vt198-rea-session\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140ECDEE0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140ECDEE5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140ECDEEA: 56                 push        rsi
  0000000140ECDEEB: 57                 push        rdi
  0000000140ECDEEC: 48 81 EC E8 01 00  sub         rsp,1E8h
                    00
  0000000140ECDEF3: 48 C7 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140ECDEFF: 48 83 BC 24 08 02  cmp         qword ptr [rsp+0000000000000208h],0
                    00 00 00
  0000000140ECDF08: 75 05              jne         0000000140ECDF0F
  0000000140ECDF0A: E9 9A 06 00 00     jmp         0000000140ECE5A9
  0000000140ECDF0F: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140ECDF14: FF 15 A6 65 F3 00  call        qword ptr [0000000141E044C0h]
  0000000140ECDF1A: 90                 nop
  0000000140ECDF1B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140ECDF20: E8 2B C5 20 00     call        00000001410DA450
  0000000140ECDF25: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECDF2D: 83 B8 18 01 00 00  cmp         dword ptr [rax+0000000000000118h],0FFFFFFFFh
                    FF
  0000000140ECDF34: 0F 84 1F 03 00 00  je          0000000140ECE259
  0000000140ECDF3A: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECDF42: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECDF49: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECDF51: 8B 90 18 01 00 00  mov         edx,dword ptr [rax+0000000000000118h]
  0000000140ECDF57: 48 8B 89 10 16 00  mov         rcx,qword ptr [rcx+0000000000001610h]
                    00
  0000000140ECDF5E: FF 15 6C 85 F3 00  call        qword ptr [0000000141E064D0h]
  0000000140ECDF64: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140ECDF69: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140ECDF6F: 0F 84 E4 02 00 00  je          0000000140ECE259
  0000000140ECDF75: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECDF7D: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECDF84: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140ECDF89: 48 8B 89 50 16 00  mov         rcx,qword ptr [rcx+0000000000001650h]
                    00
  0000000140ECDF90: FF 15 12 D5 F3 00  call        qword ptr [0000000141E0B4A8h]
  0000000140ECDF96: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  0000000140ECDF9E: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140ECDFA6: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  0000000140ECDFAE: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140ECDFB6: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECDFBE: E8 BD B9 FF FF     call        0000000140EC9980
  0000000140ECDFC3: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140ECDFC7: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140ECDFCC: FF 15 0E 7A F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECDFD2: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECDFDA: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECDFE1: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140ECDFE6: 48 8B 89 38 16 00  mov         rcx,qword ptr [rcx+0000000000001638h]
                    00
  0000000140ECDFED: FF 15 BD 86 F3 00  call        qword ptr [0000000141E066B0h]
  0000000140ECDFF3: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  0000000140ECDFFB: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140ECE003: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  0000000140ECE00B: 33 D2              xor         edx,edx
  0000000140ECE00D: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140ECE015: FF 15 F5 77 F3 00  call        qword ptr [0000000141E05810h]
  0000000140ECE01B: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  0000000140ECE021: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140ECE026: FF 15 B4 79 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE02C: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE034: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE03B: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  0000000140ECE043: 48 8B 89 A0 16 00  mov         rcx,qword ptr [rcx+00000000000016A0h]
                    00
  0000000140ECE04A: FF 15 60 86 F3 00  call        qword ptr [0000000141E066B0h]
  0000000140ECE050: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  0000000140ECE058: 48 8B 84 24 38 01  mov         rax,qword ptr [rsp+0000000000000138h]
                    00 00
  0000000140ECE060: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  0000000140ECE068: 33 D2              xor         edx,edx
  0000000140ECE06A: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  0000000140ECE072: FF 15 98 77 F3 00  call        qword ptr [0000000141E05810h]
  0000000140ECE078: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  0000000140ECE07E: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140ECE086: FF 15 54 79 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE08C: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE094: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE09B: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000140ECE0A3: 48 8B 89 88 16 00  mov         rcx,qword ptr [rcx+0000000000001688h]
                    00
  0000000140ECE0AA: FF 15 F8 D3 F3 00  call        qword ptr [0000000141E0B4A8h]
  0000000140ECE0B0: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  0000000140ECE0B8: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140ECE0C0: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  0000000140ECE0C8: 48 8B 94 24 50 01  mov         rdx,qword ptr [rsp+0000000000000150h]
                    00 00
  0000000140ECE0D0: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE0D8: E8 A3 B8 FF FF     call        0000000140EC9980
  0000000140ECE0DD: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140ECE0E1: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140ECE0E9: FF 15 F1 78 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE0EF: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE0F7: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE0FE: 48 8D 94 24 90 00  lea         rdx,[rsp+0000000000000090h]
                    00 00
  0000000140ECE106: 48 8B 89 48 16 00  mov         rcx,qword ptr [rcx+0000000000001648h]
                    00
  0000000140ECE10D: FF 15 9D 85 F3 00  call        qword ptr [0000000141E066B0h]
  0000000140ECE113: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  0000000140ECE11B: 48 8B 84 24 58 01  mov         rax,qword ptr [rsp+0000000000000158h]
                    00 00
  0000000140ECE123: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  0000000140ECE12B: 33 D2              xor         edx,edx
  0000000140ECE12D: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  0000000140ECE135: FF 15 D5 76 F3 00  call        qword ptr [0000000141E05810h]
  0000000140ECE13B: F2 0F 11 44 24 40  movsd       mmword ptr [rsp+40h],xmm0
  0000000140ECE141: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140ECE149: FF 15 91 78 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE14F: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE157: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE15E: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  0000000140ECE166: 48 8B 89 90 16 00  mov         rcx,qword ptr [rcx+0000000000001690h]
                    00
  0000000140ECE16D: FF 15 3D 85 F3 00  call        qword ptr [0000000141E066B0h]
  0000000140ECE173: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  0000000140ECE17B: 48 8B 84 24 68 01  mov         rax,qword ptr [rsp+0000000000000168h]
                    00 00
  0000000140ECE183: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  0000000140ECE18B: 33 D2              xor         edx,edx
  0000000140ECE18D: 48 8B 8C 24 70 01  mov         rcx,qword ptr [rsp+0000000000000170h]
                    00 00
  0000000140ECE195: FF 15 75 76 F3 00  call        qword ptr [0000000141E05810h]
  0000000140ECE19B: F2 0F 11 44 24 48  movsd       mmword ptr [rsp+48h],xmm0
  0000000140ECE1A1: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140ECE1A9: FF 15 31 78 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE1AF: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE1B7: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE1BE: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  0000000140ECE1C6: 48 8B 89 80 16 00  mov         rcx,qword ptr [rcx+0000000000001680h]
                    00
  0000000140ECE1CD: FF 15 DD 84 F3 00  call        qword ptr [0000000141E066B0h]
  0000000140ECE1D3: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  0000000140ECE1DB: 48 8B 84 24 78 01  mov         rax,qword ptr [rsp+0000000000000178h]
                    00 00
  0000000140ECE1E3: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  0000000140ECE1EB: 33 D2              xor         edx,edx
  0000000140ECE1ED: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  0000000140ECE1F5: FF 15 15 76 F3 00  call        qword ptr [0000000141E05810h]
  0000000140ECE1FB: F2 0F 11 44 24 50  movsd       mmword ptr [rsp+50h],xmm0
  0000000140ECE201: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140ECE209: FF 15 D1 77 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE20F: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE217: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE21E: 48 8B 89 28 16 00  mov         rcx,qword ptr [rcx+0000000000001628h]
                    00
  0000000140ECE225: FF 15 3D 7B F3 00  call        qword ptr [0000000141E05D68h]
  0000000140ECE22B: 0F B6 C0           movzx       eax,al
  0000000140ECE22E: 85 C0              test        eax,eax
  0000000140ECE230: 74 0A              je          0000000140ECE23C
  0000000140ECE232: C7 44 24 58 01 00  mov         dword ptr [rsp+58h],1
                    00 00
  0000000140ECE23A: EB 08              jmp         0000000140ECE244
  0000000140ECE23C: C7 44 24 58 00 00  mov         dword ptr [rsp+58h],0
                    00 00
  0000000140ECE244: 48 8B 7C 24 60     mov         rdi,qword ptr [rsp+60h]
  0000000140ECE249: 48 8D 74 24 20     lea         rsi,[rsp+20h]
  0000000140ECE24E: 48 83 C7 30        add         rdi,30h
  0000000140ECE252: B9 40 00 00 00     mov         ecx,40h
  0000000140ECE257: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140ECE259: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE261: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE268: 48 8B 94 24 08 02  mov         rdx,qword ptr [rsp+0000000000000208h]
                    00 00
  0000000140ECE270: 48 8B 89 10 16 00  mov         rcx,qword ptr [rcx+0000000000001610h]
                    00
  0000000140ECE277: FF 15 FB 9D F3 00  call        qword ptr [0000000141E08078h]
  0000000140ECE27D: 8B D0              mov         edx,eax
  0000000140ECE27F: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE287: 39 90 18 01 00 00  cmp         dword ptr [rax+0000000000000118h],edx
  0000000140ECE28D: 75 10              jne         0000000140ECE29F
  0000000140ECE28F: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140ECE294: FF 15 46 77 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE29A: E9 0A 03 00 00     jmp         0000000140ECE5A9
  0000000140ECE29F: 48 8B 84 24 08 02  mov         rax,qword ptr [rsp+0000000000000208h]
                    00 00
  0000000140ECE2A7: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140ECE2AC: 48 8D BC 24 D0 00  lea         rdi,[rsp+00000000000000D0h]
                    00 00
  0000000140ECE2B4: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  0000000140ECE2B9: 48 83 C6 30        add         rsi,30h
  0000000140ECE2BD: B9 40 00 00 00     mov         ecx,40h
  0000000140ECE2C2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140ECE2C4: 48 8D 7C 24 20     lea         rdi,[rsp+20h]
  0000000140ECE2C9: 48 8D B4 24 D0 00  lea         rsi,[rsp+00000000000000D0h]
                    00 00
  0000000140ECE2D1: B9 40 00 00 00     mov         ecx,40h
  0000000140ECE2D6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140ECE2D8: 48 8B 94 24 00 02  mov         rdx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE2E0: 48 8B 92 40 01 00  mov         rdx,qword ptr [rdx+0000000000000140h]
                    00
  0000000140ECE2E7: 44 8B 44 24 20     mov         r8d,dword ptr [rsp+20h]
  0000000140ECE2EC: 48 8B 92 50 16 00  mov         rdx,qword ptr [rdx+0000000000001650h]
                    00
  0000000140ECE2F3: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE2FB: E8 A0 AA FF FF     call        0000000140EC8DA0
  0000000140ECE300: 48 8B 94 24 00 02  mov         rdx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE308: 48 8B 92 40 01 00  mov         rdx,qword ptr [rdx+0000000000000140h]
                    00
  0000000140ECE30F: 44 8B 44 24 38     mov         r8d,dword ptr [rsp+38h]
  0000000140ECE314: 48 8B 92 88 16 00  mov         rdx,qword ptr [rdx+0000000000001688h]
                    00
  0000000140ECE31B: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE323: E8 78 AA FF FF     call        0000000140EC8DA0
  0000000140ECE328: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ECE32E: 41 B0 67           mov         r8b,67h
  0000000140ECE331: F2 0F 10 4C 24 28  movsd       xmm1,mmword ptr [rsp+28h]
  0000000140ECE337: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140ECE33F: FF 15 53 64 F3 00  call        qword ptr [0000000141E04798h]
  0000000140ECE345: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  0000000140ECE34D: 48 8B 84 24 88 01  mov         rax,qword ptr [rsp+0000000000000188h]
                    00 00
  0000000140ECE355: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  0000000140ECE35D: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE365: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE36C: 48 8B 94 24 90 01  mov         rdx,qword ptr [rsp+0000000000000190h]
                    00 00
  0000000140ECE374: 48 8B 89 38 16 00  mov         rcx,qword ptr [rcx+0000000000001638h]
                    00
  0000000140ECE37B: FF 15 37 83 F3 00  call        qword ptr [0000000141E066B8h]
  0000000140ECE381: 90                 nop
  0000000140ECE382: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140ECE38A: FF 15 50 76 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE390: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ECE396: 41 B0 67           mov         r8b,67h
  0000000140ECE399: F2 0F 10 4C 24 30  movsd       xmm1,mmword ptr [rsp+30h]
  0000000140ECE39F: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140ECE3A7: FF 15 EB 63 F3 00  call        qword ptr [0000000141E04798h]
  0000000140ECE3AD: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  0000000140ECE3B5: 48 8B 84 24 98 01  mov         rax,qword ptr [rsp+0000000000000198h]
                    00 00
  0000000140ECE3BD: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  0000000140ECE3C5: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE3CD: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE3D4: 48 8B 94 24 A0 01  mov         rdx,qword ptr [rsp+00000000000001A0h]
                    00 00
  0000000140ECE3DC: 48 8B 89 A0 16 00  mov         rcx,qword ptr [rcx+00000000000016A0h]
                    00
  0000000140ECE3E3: FF 15 CF 82 F3 00  call        qword ptr [0000000141E066B8h]
  0000000140ECE3E9: 90                 nop
  0000000140ECE3EA: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140ECE3F2: FF 15 E8 75 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE3F8: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ECE3FE: 41 B0 67           mov         r8b,67h
  0000000140ECE401: F2 0F 10 4C 24 40  movsd       xmm1,mmword ptr [rsp+40h]
  0000000140ECE407: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140ECE40F: FF 15 83 63 F3 00  call        qword ptr [0000000141E04798h]
  0000000140ECE415: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  0000000140ECE41D: 48 8B 84 24 A8 01  mov         rax,qword ptr [rsp+00000000000001A8h]
                    00 00
  0000000140ECE425: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  0000000140ECE42D: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE435: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE43C: 48 8B 94 24 B0 01  mov         rdx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140ECE444: 48 8B 89 48 16 00  mov         rcx,qword ptr [rcx+0000000000001648h]
                    00
  0000000140ECE44B: FF 15 67 82 F3 00  call        qword ptr [0000000141E066B8h]
  0000000140ECE451: 90                 nop
  0000000140ECE452: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140ECE45A: FF 15 80 75 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE460: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ECE466: 41 B0 67           mov         r8b,67h
  0000000140ECE469: F2 0F 10 4C 24 48  movsd       xmm1,mmword ptr [rsp+48h]
  0000000140ECE46F: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140ECE477: FF 15 1B 63 F3 00  call        qword ptr [0000000141E04798h]
  0000000140ECE47D: 48 89 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],rax
                    00 00
  0000000140ECE485: 48 8B 84 24 B8 01  mov         rax,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140ECE48D: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  0000000140ECE495: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE49D: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE4A4: 48 8B 94 24 C0 01  mov         rdx,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140ECE4AC: 48 8B 89 90 16 00  mov         rcx,qword ptr [rcx+0000000000001690h]
                    00
  0000000140ECE4B3: FF 15 FF 81 F3 00  call        qword ptr [0000000141E066B8h]
  0000000140ECE4B9: 90                 nop
  0000000140ECE4BA: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140ECE4C2: FF 15 18 75 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE4C8: 41 B9 06 00 00 00  mov         r9d,6
  0000000140ECE4CE: 41 B0 67           mov         r8b,67h
  0000000140ECE4D1: F2 0F 10 4C 24 50  movsd       xmm1,mmword ptr [rsp+50h]
  0000000140ECE4D7: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140ECE4DF: FF 15 B3 62 F3 00  call        qword ptr [0000000141E04798h]
  0000000140ECE4E5: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  0000000140ECE4ED: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000140ECE4F5: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
  0000000140ECE4FD: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE505: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE50C: 48 8B 94 24 D0 01  mov         rdx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140ECE514: 48 8B 89 80 16 00  mov         rcx,qword ptr [rcx+0000000000001680h]
                    00
  0000000140ECE51B: FF 15 97 81 F3 00  call        qword ptr [0000000141E066B8h]
  0000000140ECE521: 90                 nop
  0000000140ECE522: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140ECE52A: FF 15 B0 74 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE530: 83 7C 24 58 01     cmp         dword ptr [rsp+58h],1
  0000000140ECE535: 75 0A              jne         0000000140ECE541
  0000000140ECE537: C6 84 24 D8 01 00  mov         byte ptr [rsp+00000000000001D8h],1
                    00 01
  0000000140ECE53F: EB 08              jmp         0000000140ECE549
  0000000140ECE541: C6 84 24 D8 01 00  mov         byte ptr [rsp+00000000000001D8h],0
                    00 00
  0000000140ECE549: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE551: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE558: 0F B6 94 24 D8 01  movzx       edx,byte ptr [rsp+00000000000001D8h]
                    00 00
  0000000140ECE560: 48 8B 89 28 16 00  mov         rcx,qword ptr [rcx+0000000000001628h]
                    00
  0000000140ECE567: FF 15 2B 8D F3 00  call        qword ptr [0000000141E07298h]
  0000000140ECE56D: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE575: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140ECE57C: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140ECE581: 48 8B 89 10 16 00  mov         rcx,qword ptr [rcx+0000000000001610h]
                    00
  0000000140ECE588: FF 15 EA 9A F3 00  call        qword ptr [0000000141E08078h]
  0000000140ECE58E: 8B D0              mov         edx,eax
  0000000140ECE590: 48 8B 84 24 00 02  mov         rax,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140ECE598: 89 90 18 01 00 00  mov         dword ptr [rax+0000000000000118h],edx
  0000000140ECE59E: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140ECE5A3: FF 15 37 74 F3 00  call        qword ptr [0000000141E059E0h]
  0000000140ECE5A9: 48 81 C4 E8 01 00  add         rsp,1E8h
                    00
  0000000140ECE5B0: 5F                 pop         rdi
  0000000140ECE5B1: 5E                 pop         rsi
  0000000140ECE5B2: C3                 ret
  0000000140ECE5B3: CC                 int         3
  0000000140ECE5B4: CC                 int         3

  Summary

     1E02000 .text

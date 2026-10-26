Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400FAF60: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400FAF65: 56                 push        rsi
  00000001400FAF66: 57                 push        rdi
  00000001400FAF67: 48 81 EC 68 02 00  sub         rsp,268h
                    00
  00000001400FAF6E: 48 8B 8C 24 80 02  mov         rcx,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400FAF76: FF 15 F4 91 D0 01  call        qword ptr [0000000141E04170h]
  00000001400FAF7C: 48 8B C8           mov         rcx,rax
  00000001400FAF7F: E8 9C 5B 00 00     call        0000000140100B20
  00000001400FAF84: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001400FAF89: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  00000001400FAF8F: 0F 84 8B 04 00 00  je          00000001400FB420
  00000001400FAF95: 48 8B BC 24 80 02  mov         rdi,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400FAF9D: 48 8B 7F 50        mov         rdi,qword ptr [rdi+50h]
  00000001400FAFA1: 48 8B 8C 24 80 02  mov         rcx,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400FAFA9: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  00000001400FAFAD: 48 8B 3F           mov         rdi,qword ptr [rdi]
  00000001400FAFB0: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  00000001400FAFB5: 8B 50 30           mov         edx,dword ptr [rax+30h]
  00000001400FAFB8: FF 97 A0 01 00 00  call        qword ptr [rdi+00000000000001A0h]
  00000001400FAFBE: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001400FAFC3: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  00000001400FAFC9: 0F 84 51 04 00 00  je          00000001400FB420
  00000001400FAFCF: 33 D2              xor         edx,edx
  00000001400FAFD1: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400FAFD6: E8 E5 CB 71 00     call        0000000140817BC0
  00000001400FAFDB: 89 44 24 74        mov         dword ptr [rsp+74h],eax
  00000001400FAFDF: 33 D2              xor         edx,edx
  00000001400FAFE1: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400FAFE6: E8 75 CB 71 00     call        0000000140817B60
  00000001400FAFEB: 89 44 24 70        mov         dword ptr [rsp+70h],eax
  00000001400FAFEF: 8B 54 24 70        mov         edx,dword ptr [rsp+70h]
  00000001400FAFF3: 48 8B 8C 24 80 02  mov         rcx,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400FAFFB: E8 30 12 00 00     call        00000001400FC230
  00000001400FB000: 48 8B 8C 24 80 02  mov         rcx,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400FB008: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  00000001400FB00C: E8 4F 4C 00 00     call        00000001400FFC60
  00000001400FB011: 48 8B C8           mov         rcx,rax
  00000001400FB014: E8 27 AC FB FF     call        00000001400B5C40
  00000001400FB019: 48 8B C8           mov         rcx,rax
  00000001400FB01C: E8 8F A8 FA FF     call        00000001400A58B0
  00000001400FB021: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  00000001400FB026: 48 8B C8           mov         rcx,rax
  00000001400FB029: E8 92 CD F4 FF     call        0000000140047DC0
  00000001400FB02E: 48 8B C8           mov         rcx,rax
  00000001400FB031: E8 3A 47 F6 FF     call        000000014005F770
  00000001400FB036: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001400FB03B: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001400FB040: E8 5B 10 B8 00     call        0000000140C7C0A0
  00000001400FB045: 0F B6 C0           movzx       eax,al
  00000001400FB048: 85 C0              test        eax,eax
  00000001400FB04A: 74 1C              je          00000001400FB068
  00000001400FB04C: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  00000001400FB051: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  00000001400FB056: 48 8B 8C 24 80 02  mov         rcx,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400FB05E: E8 6D 16 00 00     call        00000001400FC6D0
  00000001400FB063: E9 B8 03 00 00     jmp         00000001400FB420
  00000001400FB068: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001400FB06D: E8 5E FC B7 00     call        0000000140C7ACD0
  00000001400FB072: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400FB07A: 48 83 BC 24 80 00  cmp         qword ptr [rsp+0000000000000080h],0
                    00 00 00
  00000001400FB083: 0F 84 97 03 00 00  je          00000001400FB420
  00000001400FB089: 48 8B 8C 24 80 02  mov         rcx,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400FB091: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  00000001400FB095: E8 C6 4B 00 00     call        00000001400FFC60
  00000001400FB09A: 48 8B C8           mov         rcx,rax
  00000001400FB09D: E8 9E AB FB FF     call        00000001400B5C40
  00000001400FB0A2: 48 8B C8           mov         rcx,rax
  00000001400FB0A5: E8 06 A8 FA FF     call        00000001400A58B0
  00000001400FB0AA: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  00000001400FB0AF: 48 8B C8           mov         rcx,rax
  00000001400FB0B2: E8 09 CD F4 FF     call        0000000140047DC0
  00000001400FB0B7: 8B 54 24 74        mov         edx,dword ptr [rsp+74h]
  00000001400FB0BB: 48 8B C8           mov         rcx,rax
  00000001400FB0BE: E8 BD 4A F6 FF     call        000000014005FB80
  00000001400FB0C3: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  00000001400FB0CA: 48 8B 8C 24 80 02  mov         rcx,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400FB0D2: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  00000001400FB0D6: E8 85 4B 00 00     call        00000001400FFC60
  00000001400FB0DB: 48 8B C8           mov         rcx,rax
  00000001400FB0DE: E8 5D AB FB FF     call        00000001400B5C40
  00000001400FB0E3: 48 8B C8           mov         rcx,rax
  00000001400FB0E6: E8 C5 A7 FA FF     call        00000001400A58B0
  00000001400FB0EB: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  00000001400FB0F0: 48 8B C8           mov         rcx,rax
  00000001400FB0F3: E8 C8 CC F4 FF     call        0000000140047DC0
  00000001400FB0F8: 8B 94 24 88 00 00  mov         edx,dword ptr [rsp+0000000000000088h]
                    00
  00000001400FB0FF: 48 8B C8           mov         rcx,rax
  00000001400FB102: E8 09 4B F6 FF     call        000000014005FC10
  00000001400FB107: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  00000001400FB10E: 48 8D 84 24 98 00  lea         rax,[rsp+0000000000000098h]
                    00 00
  00000001400FB116: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001400FB11E: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400FB126: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001400FB12B: E8 90 96 20 00     call        00000001403047C0
  00000001400FB130: 48 89 84 24 50 02  mov         qword ptr [rsp+0000000000000250h],rax
                    00 00
  00000001400FB138: 4C 8B 8C 24 50 02  mov         r9,qword ptr [rsp+0000000000000250h]
                    00 00
  00000001400FB140: 44 8B 84 24 88 00  mov         r8d,dword ptr [rsp+0000000000000088h]
                    00 00
  00000001400FB148: 8B 54 24 70        mov         edx,dword ptr [rsp+70h]
  00000001400FB14C: 48 8B 8C 24 80 02  mov         rcx,qword ptr [rsp+0000000000000280h]
                    00 00
  00000001400FB154: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  00000001400FB158: E8 C3 A7 FD FF     call        00000001400D5920
  00000001400FB15D: 8B 94 24 88 00 00  mov         edx,dword ptr [rsp+0000000000000088h]
                    00
  00000001400FB164: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400FB16C: E8 FF 50 73 00     call        0000000140830270
  00000001400FB171: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001400FB179: C6 84 24 38 01 00  mov         byte ptr [rsp+0000000000000138h],0
                    00 00
  00000001400FB181: 4C 8D 84 24 38 01  lea         r8,[rsp+0000000000000138h]
                    00 00
  00000001400FB189: 48 8D 15 D0 05 09  lea         rdx,[000000014218B760h]
                    02
  00000001400FB190: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  00000001400FB198: E8 E3 9E F2 FF     call        0000000140025080
  00000001400FB19D: 48 8D BC 24 40 02  lea         rdi,[rsp+0000000000000240h]
                    00 00
  00000001400FB1A5: 48 8B F0           mov         rsi,rax
  00000001400FB1A8: B9 10 00 00 00     mov         ecx,10h
  00000001400FB1AD: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB1AF: 48 8D BC 24 18 01  lea         rdi,[rsp+0000000000000118h]
                    00 00
  00000001400FB1B7: 48 8D B4 24 40 02  lea         rsi,[rsp+0000000000000240h]
                    00 00
  00000001400FB1BF: B9 10 00 00 00     mov         ecx,10h
  00000001400FB1C4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB1C6: 4C 8D 44 24 78     lea         r8,[rsp+78h]
  00000001400FB1CB: 48 8D 15 96 05 09  lea         rdx,[000000014218B768h]
                    02
  00000001400FB1D2: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001400FB1DA: E8 A1 9E F2 FF     call        0000000140025080
  00000001400FB1DF: 48 8D BC 24 30 02  lea         rdi,[rsp+0000000000000230h]
                    00 00
  00000001400FB1E7: 48 8B F0           mov         rsi,rax
  00000001400FB1EA: B9 10 00 00 00     mov         ecx,10h
  00000001400FB1EF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB1F1: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  00000001400FB1F9: 48 8D B4 24 30 02  lea         rsi,[rsp+0000000000000230h]
                    00 00
  00000001400FB201: B9 10 00 00 00     mov         ecx,10h
  00000001400FB206: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB208: 4C 8D 84 24 90 00  lea         r8,[rsp+0000000000000090h]
                    00 00
  00000001400FB210: 48 8D 15 61 05 09  lea         rdx,[000000014218B778h]
                    02
  00000001400FB217: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  00000001400FB21F: E8 5C 9E F2 FF     call        0000000140025080
  00000001400FB224: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  00000001400FB22C: 48 8B F0           mov         rsi,rax
  00000001400FB22F: B9 10 00 00 00     mov         ecx,10h
  00000001400FB234: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB236: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  00000001400FB23E: 48 8D B4 24 20 02  lea         rsi,[rsp+0000000000000220h]
                    00 00
  00000001400FB246: B9 10 00 00 00     mov         ecx,10h
  00000001400FB24B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB24D: 45 33 C0           xor         r8d,r8d
  00000001400FB250: 33 D2              xor         edx,edx
  00000001400FB252: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  00000001400FB25A: FF 15 08 8F D0 01  call        qword ptr [0000000141E04168h]
  00000001400FB260: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  00000001400FB268: 48 8B F0           mov         rsi,rax
  00000001400FB26B: B9 10 00 00 00     mov         ecx,10h
  00000001400FB270: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB272: 45 33 C0           xor         r8d,r8d
  00000001400FB275: 33 D2              xor         edx,edx
  00000001400FB277: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000001400FB27F: FF 15 E3 8E D0 01  call        qword ptr [0000000141E04168h]
  00000001400FB285: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  00000001400FB28D: 48 8B F0           mov         rsi,rax
  00000001400FB290: B9 10 00 00 00     mov         ecx,10h
  00000001400FB295: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB297: 45 33 C0           xor         r8d,r8d
  00000001400FB29A: 33 D2              xor         edx,edx
  00000001400FB29C: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001400FB2A4: FF 15 BE 8E D0 01  call        qword ptr [0000000141E04168h]
  00000001400FB2AA: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  00000001400FB2B2: 48 8B F0           mov         rsi,rax
  00000001400FB2B5: B9 10 00 00 00     mov         ecx,10h
  00000001400FB2BA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB2BC: 45 33 C0           xor         r8d,r8d
  00000001400FB2BF: 33 D2              xor         edx,edx
  00000001400FB2C1: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  00000001400FB2C9: FF 15 99 8E D0 01  call        qword ptr [0000000141E04168h]
  00000001400FB2CF: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  00000001400FB2D7: 48 8B F0           mov         rsi,rax
  00000001400FB2DA: B9 10 00 00 00     mov         ecx,10h
  00000001400FB2DF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB2E1: 45 33 C0           xor         r8d,r8d
  00000001400FB2E4: 33 D2              xor         edx,edx
  00000001400FB2E6: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001400FB2EE: FF 15 74 8E D0 01  call        qword ptr [0000000141E04168h]
  00000001400FB2F4: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  00000001400FB2FC: 48 8B F0           mov         rsi,rax
  00000001400FB2FF: B9 10 00 00 00     mov         ecx,10h
  00000001400FB304: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB306: 45 33 C0           xor         r8d,r8d
  00000001400FB309: 33 D2              xor         edx,edx
  00000001400FB30B: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  00000001400FB313: FF 15 4F 8E D0 01  call        qword ptr [0000000141E04168h]
  00000001400FB319: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  00000001400FB321: 48 8B F0           mov         rsi,rax
  00000001400FB324: B9 10 00 00 00     mov         ecx,10h
  00000001400FB329: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB32B: 45 33 C0           xor         r8d,r8d
  00000001400FB32E: 33 D2              xor         edx,edx
  00000001400FB330: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  00000001400FB338: FF 15 2A 8E D0 01  call        qword ptr [0000000141E04168h]
  00000001400FB33E: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  00000001400FB346: 48 8B F0           mov         rsi,rax
  00000001400FB349: B9 10 00 00 00     mov         ecx,10h
  00000001400FB34E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB350: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  00000001400FB358: 48 8D B4 24 18 01  lea         rsi,[rsp+0000000000000118h]
                    00 00
  00000001400FB360: B9 10 00 00 00     mov         ecx,10h
  00000001400FB365: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB367: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  00000001400FB36F: 48 8D B4 24 40 01  lea         rsi,[rsp+0000000000000140h]
                    00 00
  00000001400FB377: B9 10 00 00 00     mov         ecx,10h
  00000001400FB37C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB37E: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  00000001400FB386: 48 8D B4 24 60 01  lea         rsi,[rsp+0000000000000160h]
                    00 00
  00000001400FB38E: B9 10 00 00 00     mov         ecx,10h
  00000001400FB393: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400FB395: 48 8D 84 24 10 02  lea         rax,[rsp+0000000000000210h]
                    00 00
  00000001400FB39D: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400FB3A2: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  00000001400FB3AA: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400FB3AF: 48 8D 84 24 F0 01  lea         rax,[rsp+00000000000001F0h]
                    00 00
  00000001400FB3B7: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400FB3BC: 48 8D 84 24 E0 01  lea         rax,[rsp+00000000000001E0h]
                    00 00
  00000001400FB3C4: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400FB3C9: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  00000001400FB3D1: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400FB3D6: 48 8D 84 24 C0 01  lea         rax,[rsp+00000000000001C0h]
                    00 00
  00000001400FB3DE: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400FB3E3: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  00000001400FB3EB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400FB3F0: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  00000001400FB3F8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400FB3FD: 4C 8D 8C 24 90 01  lea         r9,[rsp+0000000000000190h]
                    00 00
  00000001400FB405: 4C 8D 84 24 80 01  lea         r8,[rsp+0000000000000180h]
                    00 00
  00000001400FB40D: 48 8D 15 74 03 09  lea         rdx,[000000014218B788h]
                    02
  00000001400FB414: 48 8D 0D E5 B2 C7  lea         rcx,[0000000143D76700h]
                    03
  00000001400FB41B: E8 A0 3F 1D 00     call        00000001402CF3C0
  00000001400FB420: 48 81 C4 68 02 00  add         rsp,268h
                    00
  00000001400FB427: 5F                 pop         rdi
  00000001400FB428: 5E                 pop         rsi
  00000001400FB429: C3                 ret

  Summary

     1E02000 .text

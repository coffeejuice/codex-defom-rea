Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CC54A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CC54A5: 56                 push        rsi
  0000000140CC54A6: 57                 push        rdi
  0000000140CC54A7: 48 81 EC 18 02 00  sub         rsp,218h
                    00
  0000000140CC54AE: 48 C7 84 24 F8 01  mov         qword ptr [rsp+00000000000001F8h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140CC54BA: C7 84 24 F0 01 00  mov         dword ptr [rsp+00000000000001F0h],0
                    00 00 00 00 00
  0000000140CC54C5: 48 8B 84 24 30 02  mov         rax,qword ptr [rsp+0000000000000230h]
                    00 00
  0000000140CC54CD: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CC54D0: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  0000000140CC54D8: FF 90 B8 07 00 00  call        qword ptr [rax+00000000000007B8h]
  0000000140CC54DE: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140CC54E3: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000140CC54EC: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  0000000140CC54F2: 75 05              jne         0000000140CC54F9
  0000000140CC54F4: E9 EF 03 00 00     jmp         0000000140CC58E8
  0000000140CC54F9: 48 8D 15 30 C9 C4  lea         rdx,[0000000142911E30h]
                    01
  0000000140CC5500: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140CC5508: FF 15 02 EC 13 01  call        qword ptr [0000000141E04110h]
  0000000140CC550E: 48 8B D0           mov         rdx,rax
  0000000140CC5511: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140CC5519: FF 15 F9 EB 13 01  call        qword ptr [0000000141E04118h]
  0000000140CC551F: 90                 nop
  0000000140CC5520: 4C 8D 84 24 88 00  lea         r8,[rsp+0000000000000088h]
                    00 00
  0000000140CC5528: BA 01 00 00 00     mov         edx,1
  0000000140CC552D: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140CC5532: E8 99 F7 06 00     call        0000000140D34CD0
  0000000140CC5537: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140CC553C: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140CC5544: FF 15 96 04 14 01  call        qword ptr [0000000141E059E0h]
  0000000140CC554A: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140CC5550: 75 05              jne         0000000140CC5557
  0000000140CC5552: E9 91 03 00 00     jmp         0000000140CC58E8
  0000000140CC5557: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CC555C: FF 15 F6 62 14 01  call        qword ptr [0000000141E0B858h]
  0000000140CC5562: 0F B6 C0           movzx       eax,al
  0000000140CC5565: 85 C0              test        eax,eax
  0000000140CC5567: 75 05              jne         0000000140CC556E
  0000000140CC5569: E9 7A 03 00 00     jmp         0000000140CC58E8
  0000000140CC556E: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140CC5573: E8 C8 03 00 00     call        0000000140CC5940
  0000000140CC5578: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140CC557D: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140CC5583: 0F 84 5F 03 00 00  je          0000000140CC58E8
  0000000140CC5589: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140CC558E: FF 15 04 21 14 01  call        qword ptr [0000000141E07698h]
  0000000140CC5594: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140CC559C: 48 83 BC 24 80 00  cmp         qword ptr [rsp+0000000000000080h],0
                    00 00 00
  0000000140CC55A5: 0F 84 3D 03 00 00  je          0000000140CC58E8
  0000000140CC55AB: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CC55B3: FF 15 CF 2D 14 01  call        qword ptr [0000000141E08388h]
  0000000140CC55B9: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CC55C1: 4C 8D 0D 00 63 C9  lea         r9,[000000014395B8C8h]
                    02
  0000000140CC55C8: 4C 8D 05 C9 F7 8F  lea         r8,[00000001435C4D98h]
                    02
  0000000140CC55CF: 33 D2              xor         edx,edx
  0000000140CC55D1: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CC55D9: E8 1E A3 7A 00     call        000000014146F8FC
  0000000140CC55DE: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140CC55E3: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  0000000140CC55E9: 0F 84 F9 02 00 00  je          0000000140CC58E8
  0000000140CC55EF: 48 8D 15 4A C8 C4  lea         rdx,[0000000142911E40h]
                    01
  0000000140CC55F6: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140CC55FE: FF 15 7C EE 13 01  call        qword ptr [0000000141E04480h]
  0000000140CC5604: 90                 nop
  0000000140CC5605: 8B 84 24 F0 01 00  mov         eax,dword ptr [rsp+00000000000001F0h]
                    00
  0000000140CC560C: 83 C8 01           or          eax,1
  0000000140CC560F: 89 84 24 F0 01 00  mov         dword ptr [rsp+00000000000001F0h],eax
                    00
  0000000140CC5616: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  0000000140CC561E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140CC5623: E8 68 FE 00 00     call        0000000140CD5490
  0000000140CC5628: 48 85 C0           test        rax,rax
  0000000140CC562B: 75 4B              jne         0000000140CC5678
  0000000140CC562D: 48 8D 15 24 C8 C4  lea         rdx,[0000000142911E58h]
                    01
  0000000140CC5634: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140CC563C: FF 15 3E EE 13 01  call        qword ptr [0000000141E04480h]
  0000000140CC5642: 90                 nop
  0000000140CC5643: 8B 84 24 F0 01 00  mov         eax,dword ptr [rsp+00000000000001F0h]
                    00
  0000000140CC564A: 83 C8 02           or          eax,2
  0000000140CC564D: 89 84 24 F0 01 00  mov         dword ptr [rsp+00000000000001F0h],eax
                    00
  0000000140CC5654: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  0000000140CC565C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140CC5661: E8 2A FE 00 00     call        0000000140CD5490
  0000000140CC5666: 48 85 C0           test        rax,rax
  0000000140CC5669: 75 0D              jne         0000000140CC5678
  0000000140CC566B: C7 84 24 00 02 00  mov         dword ptr [rsp+0000000000000200h],0
                    00 00 00 00 00
  0000000140CC5676: EB 0B              jmp         0000000140CC5683
  0000000140CC5678: C7 84 24 00 02 00  mov         dword ptr [rsp+0000000000000200h],1
                    00 01 00 00 00
  0000000140CC5683: 0F B6 84 24 00 02  movzx       eax,byte ptr [rsp+0000000000000200h]
                    00 00
  0000000140CC568B: 88 84 24 98 00 00  mov         byte ptr [rsp+0000000000000098h],al
                    00
  0000000140CC5692: 8B 84 24 F0 01 00  mov         eax,dword ptr [rsp+00000000000001F0h]
                    00
  0000000140CC5699: 83 E0 02           and         eax,2
  0000000140CC569C: 85 C0              test        eax,eax
  0000000140CC569E: 74 17              je          0000000140CC56B7
  0000000140CC56A0: 83 A4 24 F0 01 00  and         dword ptr [rsp+00000000000001F0h],0FFFFFFFDh
                    00 FD
  0000000140CC56A8: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140CC56B0: FF 15 2A 03 14 01  call        qword ptr [0000000141E059E0h]
  0000000140CC56B6: 90                 nop
  0000000140CC56B7: 8B 84 24 F0 01 00  mov         eax,dword ptr [rsp+00000000000001F0h]
                    00
  0000000140CC56BE: 83 E0 01           and         eax,1
  0000000140CC56C1: 85 C0              test        eax,eax
  0000000140CC56C3: 74 16              je          0000000140CC56DB
  0000000140CC56C5: 83 A4 24 F0 01 00  and         dword ptr [rsp+00000000000001F0h],0FFFFFFFEh
                    00 FE
  0000000140CC56CD: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140CC56D5: FF 15 05 03 14 01  call        qword ptr [0000000141E059E0h]
  0000000140CC56DB: 0F B6 84 24 98 00  movzx       eax,byte ptr [rsp+0000000000000098h]
                    00 00
  0000000140CC56E3: 85 C0              test        eax,eax
  0000000140CC56E5: 0F 84 FD 01 00 00  je          0000000140CC58E8
  0000000140CC56EB: 45 33 C0           xor         r8d,r8d
  0000000140CC56EE: 33 D2              xor         edx,edx
  0000000140CC56F0: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140CC56F8: FF 15 6A EA 13 01  call        qword ptr [0000000141E04168h]
  0000000140CC56FE: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  0000000140CC5706: 48 8B F0           mov         rsi,rax
  0000000140CC5709: B9 10 00 00 00     mov         ecx,10h
  0000000140CC570E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CC5710: 45 33 C0           xor         r8d,r8d
  0000000140CC5713: 33 D2              xor         edx,edx
  0000000140CC5715: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140CC571D: FF 15 45 EA 13 01  call        qword ptr [0000000141E04168h]
  0000000140CC5723: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  0000000140CC572B: 48 8B F0           mov         rsi,rax
  0000000140CC572E: B9 10 00 00 00     mov         ecx,10h
  0000000140CC5733: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CC5735: 45 33 C0           xor         r8d,r8d
  0000000140CC5738: 33 D2              xor         edx,edx
  0000000140CC573A: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140CC5742: FF 15 20 EA 13 01  call        qword ptr [0000000141E04168h]
  0000000140CC5748: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  0000000140CC5750: 48 8B F0           mov         rsi,rax
  0000000140CC5753: B9 10 00 00 00     mov         ecx,10h
  0000000140CC5758: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CC575A: 45 33 C0           xor         r8d,r8d
  0000000140CC575D: 33 D2              xor         edx,edx
  0000000140CC575F: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140CC5767: FF 15 FB E9 13 01  call        qword ptr [0000000141E04168h]
  0000000140CC576D: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  0000000140CC5775: 48 8B F0           mov         rsi,rax
  0000000140CC5778: B9 10 00 00 00     mov         ecx,10h
  0000000140CC577D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CC577F: 45 33 C0           xor         r8d,r8d
  0000000140CC5782: 33 D2              xor         edx,edx
  0000000140CC5784: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000140CC578C: FF 15 D6 E9 13 01  call        qword ptr [0000000141E04168h]
  0000000140CC5792: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  0000000140CC579A: 48 8B F0           mov         rsi,rax
  0000000140CC579D: B9 10 00 00 00     mov         ecx,10h
  0000000140CC57A2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CC57A4: 45 33 C0           xor         r8d,r8d
  0000000140CC57A7: 33 D2              xor         edx,edx
  0000000140CC57A9: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140CC57B1: FF 15 B1 E9 13 01  call        qword ptr [0000000141E04168h]
  0000000140CC57B7: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140CC57BF: 48 8B F0           mov         rsi,rax
  0000000140CC57C2: B9 10 00 00 00     mov         ecx,10h
  0000000140CC57C7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CC57C9: 45 33 C0           xor         r8d,r8d
  0000000140CC57CC: 33 D2              xor         edx,edx
  0000000140CC57CE: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000140CC57D6: FF 15 8C E9 13 01  call        qword ptr [0000000141E04168h]
  0000000140CC57DC: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140CC57E4: 48 8B F0           mov         rsi,rax
  0000000140CC57E7: B9 10 00 00 00     mov         ecx,10h
  0000000140CC57EC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CC57EE: 45 33 C0           xor         r8d,r8d
  0000000140CC57F1: 33 D2              xor         edx,edx
  0000000140CC57F3: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  0000000140CC57FB: FF 15 67 E9 13 01  call        qword ptr [0000000141E04168h]
  0000000140CC5801: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140CC5809: 48 8B F0           mov         rsi,rax
  0000000140CC580C: B9 10 00 00 00     mov         ecx,10h
  0000000140CC5811: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CC5813: 45 33 C0           xor         r8d,r8d
  0000000140CC5816: 33 D2              xor         edx,edx
  0000000140CC5818: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  0000000140CC5820: FF 15 42 E9 13 01  call        qword ptr [0000000141E04168h]
  0000000140CC5826: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  0000000140CC582E: 48 8B F0           mov         rsi,rax
  0000000140CC5831: B9 10 00 00 00     mov         ecx,10h
  0000000140CC5836: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CC5838: 45 33 C0           xor         r8d,r8d
  0000000140CC583B: 33 D2              xor         edx,edx
  0000000140CC583D: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000140CC5845: FF 15 1D E9 13 01  call        qword ptr [0000000141E04168h]
  0000000140CC584B: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  0000000140CC5853: 48 8B F0           mov         rsi,rax
  0000000140CC5856: B9 10 00 00 00     mov         ecx,10h
  0000000140CC585B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140CC585D: 48 8D 84 24 E0 01  lea         rax,[rsp+00000000000001E0h]
                    00 00
  0000000140CC5865: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140CC586A: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  0000000140CC5872: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140CC5877: 48 8D 84 24 C0 01  lea         rax,[rsp+00000000000001C0h]
                    00 00
  0000000140CC587F: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140CC5884: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  0000000140CC588C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CC5891: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140CC5899: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140CC589E: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140CC58A6: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140CC58AB: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  0000000140CC58B3: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140CC58B8: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  0000000140CC58C0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140CC58C5: 4C 8D 8C 24 60 01  lea         r9,[rsp+0000000000000160h]
                    00 00
  0000000140CC58CD: 4C 8D 84 24 50 01  lea         r8,[rsp+0000000000000150h]
                    00 00
  0000000140CC58D5: 48 8D 15 8C C5 C4  lea         rdx,[0000000142911E68h]
                    01
  0000000140CC58DC: 48 8D 0D 1D 0E 0B  lea         rcx,[0000000143D76700h]
                    03
  0000000140CC58E3: E8 D8 9A 60 FF     call        00000001402CF3C0
  0000000140CC58E8: 48 81 C4 18 02 00  add         rsp,218h
                    00
  0000000140CC58EF: 5F                 pop         rdi
  0000000140CC58F0: 5E                 pop         rsi
  0000000140CC58F1: C3                 ret

  Summary

     1E02000 .text

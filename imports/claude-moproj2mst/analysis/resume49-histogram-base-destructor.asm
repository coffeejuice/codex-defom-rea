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
  0000000140C2C73E: B9
  0000000140C2C73F: 10 00              adc         byte ptr [rax],al

  Summary

     1E02000 .text

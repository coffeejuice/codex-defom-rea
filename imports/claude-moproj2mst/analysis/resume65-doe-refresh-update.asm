Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400F47E0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400F47E5: 48 81 EC 38 01 00  sub         rsp,138h
                    00
  00000001400F47EC: 48 C7 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400F47F8: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4800: C6 80 98 00 00 00  mov         byte ptr [rax+0000000000000098h],1
                    01
  00000001400F4807: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F480F: 83 B8 A0 00 00 00  cmp         dword ptr [rax+00000000000000A0h],0FFFFFFFFh
                    FF
  00000001400F4816: 7E 1A              jle         00000001400F4832
  00000001400F4818: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4820: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400F4824: B2 01              mov         dl,1
  00000001400F4826: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001400F482A: FF 15 10 72 D1 01  call        qword ptr [0000000141E0BA40h]
  00000001400F4830: EB 18              jmp         00000001400F484A
  00000001400F4832: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F483A: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400F483E: 33 D2              xor         edx,edx
  00000001400F4840: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001400F4844: FF 15 F6 71 D1 01  call        qword ptr [0000000141E0BA40h]
  00000001400F484A: 48 8D 0D 67 1E C8  lea         rcx,[0000000143D766B8h]
                    03
  00000001400F4851: E8 3A 38 1D 00     call        00000001402C8090
  00000001400F4856: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400F485B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400F4863: 4C 8D 0D EE 49 47  lea         r9,[0000000143569258h]
                    03
  00000001400F486A: 4C 8D 05 0F 4A 47  lea         r8,[0000000143569280h]
                    03
  00000001400F4871: 33 D2              xor         edx,edx
  00000001400F4873: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001400F4878: E8 7F B0 37 01     call        000000014146F8FC
  00000001400F487D: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400F4882: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  00000001400F4888: 0F 84 F9 00 00 00  je          00000001400F4987
  00000001400F488E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400F4893: E8 78 8C F2 FF     call        000000014001D510
  00000001400F4898: 4C 8B D8           mov         r11,rax
  00000001400F489B: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F48A3: 4C 89 58 40        mov         qword ptr [rax+40h],r11
  00000001400F48A7: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F48AF: 48 83 78 40 00     cmp         qword ptr [rax+40h],0
  00000001400F48B4: 74 62              je          00000001400F4918
  00000001400F48B6: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F48BE: 48 8B 49 40        mov         rcx,qword ptr [rcx+40h]
  00000001400F48C2: E8 B9 D8 FA FF     call        00000001400A2180
  00000001400F48C7: 44 8B D8           mov         r11d,eax
  00000001400F48CA: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F48D2: 44 89 58 58        mov         dword ptr [rax+58h],r11d
  00000001400F48D6: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F48DE: 48 8B 49 40        mov         rcx,qword ptr [rcx+40h]
  00000001400F48E2: E8 A9 2A FB FF     call        00000001400A7390
  00000001400F48E7: 4C 8B D8           mov         r11,rax
  00000001400F48EA: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F48F2: 4C 89 58 48        mov         qword ptr [rax+48h],r11
  00000001400F48F6: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F48FE: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  00000001400F4902: E8 89 11 FC FF     call        00000001400B5A90
  00000001400F4907: 4C 8B D8           mov         r11,rax
  00000001400F490A: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4912: 4C 89 58 50        mov         qword ptr [rax+50h],r11
  00000001400F4916: EB 2F              jmp         00000001400F4947
  00000001400F4918: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4920: C7 40 58 02 00 00  mov         dword ptr [rax+58h],2
                    00
  00000001400F4927: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F492F: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  00000001400F4937: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F493F: 48 C7 40 50 00 00  mov         qword ptr [rax+50h],0
                    00 00
  00000001400F4947: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001400F494C: E8 7F 8E F2 FF     call        000000014001D7D0
  00000001400F4951: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400F4956: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  00000001400F495C: 74 27              je          00000001400F4985
  00000001400F495E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400F4963: E8 D8 30 F5 FF     call        0000000140047A40
  00000001400F4968: 89 44 24 58        mov         dword ptr [rsp+58h],eax
  00000001400F496C: 83 7C 24 58 FF     cmp         dword ptr [rsp+58h],0FFFFFFFFh
  00000001400F4971: 7E 12              jle         00000001400F4985
  00000001400F4973: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F497B: 8B 44 24 58        mov         eax,dword ptr [rsp+58h]
  00000001400F497F: 89 81 9C 00 00 00  mov         dword ptr [rcx+000000000000009Ch],eax
  00000001400F4985: EB 20              jmp         00000001400F49A7
  00000001400F4987: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F498F: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  00000001400F4997: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F499F: 48 C7 40 50 00 00  mov         qword ptr [rax+50h],0
                    00 00
  00000001400F49A7: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F49AF: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400F49B3: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  00000001400F49BA: FF 15 80 19 D1 01  call        qword ptr [0000000141E06340h]
  00000001400F49C0: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F49C8: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400F49CC: 33 D2              xor         edx,edx
  00000001400F49CE: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  00000001400F49D5: FF 15 6D 19 D1 01  call        qword ptr [0000000141E06348h]
  00000001400F49DB: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F49E3: 48 83 78 50 00     cmp         qword ptr [rax+50h],0
  00000001400F49E8: 75 05              jne         00000001400F49EF
  00000001400F49EA: E9 0F 04 00 00     jmp         00000001400F4DFE
  00000001400F49EF: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F49F7: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400F49FB: 33 D2              xor         edx,edx
  00000001400F49FD: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  00000001400F4A04: FF 15 46 19 D1 01  call        qword ptr [0000000141E06350h]
  00000001400F4A0A: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4A12: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400F4A16: BA 08 00 00 00     mov         edx,8
  00000001400F4A1B: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  00000001400F4A22: FF 15 30 19 D1 01  call        qword ptr [0000000141E06358h]
  00000001400F4A28: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001400F4A2D: E8 AE 09 F7 FF     call        00000001400653E0
  00000001400F4A32: 90                 nop
  00000001400F4A33: 45 33 C0           xor         r8d,r8d
  00000001400F4A36: 48 8D 15 5B 69 09  lea         rdx,[000000014218B398h]
                    02
  00000001400F4A3D: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001400F4A45: E8 C6 B1 00 00     call        00000001400FFC10
  00000001400F4A4A: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001400F4A52: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001400F4A5A: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001400F4A62: 45 33 C0           xor         r8d,r8d
  00000001400F4A65: 48 8D 15 24 69 09  lea         rdx,[000000014218B390h]
                    02
  00000001400F4A6C: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400F4A74: E8 97 B1 00 00     call        00000001400FFC10
  00000001400F4A79: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000001400F4A81: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001400F4A89: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  00000001400F4A91: 45 33 C0           xor         r8d,r8d
  00000001400F4A94: 48 8D 15 ED 68 09  lea         rdx,[000000014218B388h]
                    02
  00000001400F4A9B: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000001400F4AA3: E8 68 B1 00 00     call        00000001400FFC10
  00000001400F4AA8: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  00000001400F4AB0: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  00000001400F4AB8: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  00000001400F4AC0: 45 33 C0           xor         r8d,r8d
  00000001400F4AC3: 48 8D 15 B6 68 09  lea         rdx,[000000014218B380h]
                    02
  00000001400F4ACA: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000001400F4AD2: E8 39 B1 00 00     call        00000001400FFC10
  00000001400F4AD7: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  00000001400F4ADF: 48 8B 84 24 D8 00  mov         rax,qword ptr [rsp+00000000000000D8h]
                    00 00
  00000001400F4AE7: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  00000001400F4AEF: 45 33 C0           xor         r8d,r8d
  00000001400F4AF2: 48 8D 15 7F 68 09  lea         rdx,[000000014218B378h]
                    02
  00000001400F4AF9: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001400F4AFE: E8 0D B1 00 00     call        00000001400FFC10
  00000001400F4B03: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  00000001400F4B0B: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001400F4B13: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000001400F4B1B: 45 33 C0           xor         r8d,r8d
  00000001400F4B1E: 48 8D 15 43 68 09  lea         rdx,[000000014218B368h]
                    02
  00000001400F4B25: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001400F4B2A: E8 E1 B0 00 00     call        00000001400FFC10
  00000001400F4B2F: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  00000001400F4B37: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001400F4B3F: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  00000001400F4B47: 45 33 C0           xor         r8d,r8d
  00000001400F4B4A: 48 8D 15 07 68 09  lea         rdx,[000000014218B358h]
                    02
  00000001400F4B51: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001400F4B56: E8 B5 B0 00 00     call        00000001400FFC10
  00000001400F4B5B: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  00000001400F4B63: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  00000001400F4B6B: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  00000001400F4B73: 45 33 C0           xor         r8d,r8d
  00000001400F4B76: 48 8D 15 CB 67 09  lea         rdx,[000000014218B348h]
                    02
  00000001400F4B7D: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400F4B82: E8 89 B0 00 00     call        00000001400FFC10
  00000001400F4B87: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  00000001400F4B8F: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  00000001400F4B97: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  00000001400F4B9F: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  00000001400F4BA7: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001400F4BAC: E8 0F 58 BD 00     call        0000000140CCA3C0
  00000001400F4BB1: 48 8B 94 24 10 01  mov         rdx,qword ptr [rsp+0000000000000110h]
                    00 00
  00000001400F4BB9: 48 8B C8           mov         rcx,rax
  00000001400F4BBC: E8 FF 57 BD 00     call        0000000140CCA3C0
  00000001400F4BC1: 48 8B 94 24 00 01  mov         rdx,qword ptr [rsp+0000000000000100h]
                    00 00
  00000001400F4BC9: 48 8B C8           mov         rcx,rax
  00000001400F4BCC: E8 EF 57 BD 00     call        0000000140CCA3C0
  00000001400F4BD1: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001400F4BD9: 48 8B C8           mov         rcx,rax
  00000001400F4BDC: E8 DF 57 BD 00     call        0000000140CCA3C0
  00000001400F4BE1: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400F4BE9: 48 8B C8           mov         rcx,rax
  00000001400F4BEC: E8 CF 57 BD 00     call        0000000140CCA3C0
  00000001400F4BF1: 48 8B 94 24 D0 00  mov         rdx,qword ptr [rsp+00000000000000D0h]
                    00 00
  00000001400F4BF9: 48 8B C8           mov         rcx,rax
  00000001400F4BFC: E8 BF 57 BD 00     call        0000000140CCA3C0
  00000001400F4C01: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000001400F4C09: 48 8B C8           mov         rcx,rax
  00000001400F4C0C: E8 AF 57 BD 00     call        0000000140CCA3C0
  00000001400F4C11: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400F4C19: 48 8B C8           mov         rcx,rax
  00000001400F4C1C: E8 9F 57 BD 00     call        0000000140CCA3C0
  00000001400F4C21: 90                 nop
  00000001400F4C22: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400F4C27: FF 15 B3 0D D1 01  call        qword ptr [0000000141E059E0h]
  00000001400F4C2D: 90                 nop
  00000001400F4C2E: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001400F4C33: FF 15 A7 0D D1 01  call        qword ptr [0000000141E059E0h]
  00000001400F4C39: 90                 nop
  00000001400F4C3A: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  00000001400F4C3F: FF 15 9B 0D D1 01  call        qword ptr [0000000141E059E0h]
  00000001400F4C45: 90                 nop
  00000001400F4C46: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001400F4C4B: FF 15 8F 0D D1 01  call        qword ptr [0000000141E059E0h]
  00000001400F4C51: 90                 nop
  00000001400F4C52: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  00000001400F4C5A: FF 15 80 0D D1 01  call        qword ptr [0000000141E059E0h]
  00000001400F4C60: 90                 nop
  00000001400F4C61: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  00000001400F4C69: FF 15 71 0D D1 01  call        qword ptr [0000000141E059E0h]
  00000001400F4C6F: 90                 nop
  00000001400F4C70: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  00000001400F4C78: FF 15 62 0D D1 01  call        qword ptr [0000000141E059E0h]
  00000001400F4C7E: 90                 nop
  00000001400F4C7F: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001400F4C87: FF 15 53 0D D1 01  call        qword ptr [0000000141E059E0h]
  00000001400F4C8D: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4C95: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400F4C99: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  00000001400F4C9E: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  00000001400F4CA5: FF 15 B5 16 D1 01  call        qword ptr [0000000141E06360h]
  00000001400F4CAB: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4CB3: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400F4CB7: 48 8B 89 90 00 00  mov         rcx,qword ptr [rcx+0000000000000090h]
                    00
  00000001400F4CBE: FF 15 A4 16 D1 01  call        qword ptr [0000000141E06368h]
  00000001400F4CC4: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400F4CC9: BA 14 00 00 00     mov         edx,14h
  00000001400F4CCE: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400F4CD3: FF 15 97 16 D1 01  call        qword ptr [0000000141E06370h]
  00000001400F4CD9: 41 B8 64 00 00 00  mov         r8d,64h
  00000001400F4CDF: 33 D2              xor         edx,edx
  00000001400F4CE1: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400F4CE6: FF 15 6C 21 D1 01  call        qword ptr [0000000141E06E58h]
  00000001400F4CEC: 41 B8 88 00 00 00  mov         r8d,88h
  00000001400F4CF2: BA 01 00 00 00     mov         edx,1
  00000001400F4CF7: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400F4CFC: FF 15 56 21 D1 01  call        qword ptr [0000000141E06E58h]
  00000001400F4D02: 41 B8 3C 00 00 00  mov         r8d,3Ch
  00000001400F4D08: BA 02 00 00 00     mov         edx,2
  00000001400F4D0D: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400F4D12: FF 15 40 21 D1 01  call        qword ptr [0000000141E06E58h]
  00000001400F4D18: 41 B8 A8 00 00 00  mov         r8d,0A8h
  00000001400F4D1E: BA 06 00 00 00     mov         edx,6
  00000001400F4D23: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400F4D28: FF 15 2A 21 D1 01  call        qword ptr [0000000141E06E58h]
  00000001400F4D2E: 41 B8 64 00 00 00  mov         r8d,64h
  00000001400F4D34: BA 07 00 00 00     mov         edx,7
  00000001400F4D39: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400F4D3E: FF 15 14 21 D1 01  call        qword ptr [0000000141E06E58h]
  00000001400F4D44: 41 B8 54 00 00 00  mov         r8d,54h
  00000001400F4D4A: BA 05 00 00 00     mov         edx,5
  00000001400F4D4F: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400F4D54: FF 15 FE 20 D1 01  call        qword ptr [0000000141E06E58h]
  00000001400F4D5A: 41 B8 54 00 00 00  mov         r8d,54h
  00000001400F4D60: BA 03 00 00 00     mov         edx,3
  00000001400F4D65: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400F4D6A: FF 15 E8 20 D1 01  call        qword ptr [0000000141E06E58h]
  00000001400F4D70: 41 B8 54 00 00 00  mov         r8d,54h
  00000001400F4D76: BA 04 00 00 00     mov         edx,4
  00000001400F4D7B: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400F4D80: FF 15 D2 20 D1 01  call        qword ptr [0000000141E06E58h]
  00000001400F4D86: B2 01              mov         dl,1
  00000001400F4D88: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001400F4D8D: FF 15 E5 15 D1 01  call        qword ptr [0000000141E06378h]
  00000001400F4D93: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4D9B: 83 78 5C 00        cmp         dword ptr [rax+5Ch],0
  00000001400F4D9F: 75 37              jne         00000001400F4DD8
  00000001400F4DA1: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4DA9: 48 8B 40 38        mov         rax,qword ptr [rax+38h]
  00000001400F4DAD: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001400F4DB1: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4DB9: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  00000001400F4DBD: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001400F4DC1: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400F4DC4: 33 D2              xor         edx,edx
  00000001400F4DC6: FF 50 58           call        qword ptr [rax+58h]
  00000001400F4DC9: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4DD1: E8 7A F4 FF FF     call        00000001400F4250
  00000001400F4DD6: EB 0D              jmp         00000001400F4DE5
  00000001400F4DD8: 48 8B 8C 24 40 01  mov         rcx,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4DE0: E8 CB E3 FF FF     call        00000001400F31B0
  00000001400F4DE5: 48 8B 84 24 40 01  mov         rax,qword ptr [rsp+0000000000000140h]
                    00 00
  00000001400F4DED: C6 80 98 00 00 00  mov         byte ptr [rax+0000000000000098h],0
                    00
  00000001400F4DF4: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001400F4DF9: E8 D2 FA F0 FF     call        00000001400048D0
  00000001400F4DFE: 48 81 C4 38 01 00  add         rsp,138h
                    00
  00000001400F4E05: C3                 ret

  Summary

     1E02000 .text

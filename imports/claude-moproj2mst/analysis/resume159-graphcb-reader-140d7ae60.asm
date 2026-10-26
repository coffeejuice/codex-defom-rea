Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D7AE60: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140D7AE65: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140D7AE6A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D7AE6F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D7AE74: 48 81 EC F8 00 00  sub         rsp,0F8h
                    00
  0000000140D7AE7B: 48 C7 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D7AE87: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140D7AE8F: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D7AE97: E8 94 6C FC FF     call        0000000140D41B30
  0000000140D7AE9C: 83 F8 01           cmp         eax,1
  0000000140D7AE9F: 0F 85 62 02 00 00  jne         0000000140D7B107
  0000000140D7AEA5: 48 8B 94 24 08 01  mov         rdx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140D7AEAD: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D7AEB5: FF 15 0D AD 08 01  call        qword ptr [0000000141E05BC8h]
  0000000140D7AEBB: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D7AEC0: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140D7AEC6: 0F 84 39 02 00 00  je          0000000140D7B105
  0000000140D7AECC: C7 44 24 48 00 00  mov         dword ptr [rsp+48h],0
                    00 00
  0000000140D7AED4: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D7AED9: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D7AEDC: 41 B8 22 00 00 00  mov         r8d,22h
  0000000140D7AEE2: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  0000000140D7AEE7: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140D7AEEC: FF 50 08           call        qword ptr [rax+8]
  0000000140D7AEEF: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140D7AEF7: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140D7AEFF: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140D7AF07: 33 D2              xor         edx,edx
  0000000140D7AF09: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D7AF11: FF 15 41 97 08 01  call        qword ptr [0000000141E04658h]
  0000000140D7AF17: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140D7AF1B: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140D7AF20: FF 15 92 AA 08 01  call        qword ptr [0000000141E059B8h]
  0000000140D7AF26: 48 83 BC 24 18 01  cmp         qword ptr [rsp+0000000000000118h],0
                    00 00 00
  0000000140D7AF2F: 74 5C              je          0000000140D7AF8D
  0000000140D7AF31: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D7AF36: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D7AF39: 41 B8 23 00 00 00  mov         r8d,23h
  0000000140D7AF3F: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140D7AF44: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140D7AF49: FF 50 08           call        qword ptr [rax+8]
  0000000140D7AF4C: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140D7AF54: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D7AF5C: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140D7AF64: 33 D2              xor         edx,edx
  0000000140D7AF66: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140D7AF6E: FF 15 E4 96 08 01  call        qword ptr [0000000141E04658h]
  0000000140D7AF74: 44 8B D8           mov         r11d,eax
  0000000140D7AF77: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140D7AF7F: 44 89 18           mov         dword ptr [rax],r11d
  0000000140D7AF82: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140D7AF87: FF 15 2B AA 08 01  call        qword ptr [0000000141E059B8h]
  0000000140D7AF8D: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D7AF92: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D7AF95: 41 B8 21 00 00 00  mov         r8d,21h
  0000000140D7AF9B: 48 8D 94 24 88 00  lea         rdx,[rsp+0000000000000088h]
                    00 00
  0000000140D7AFA3: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140D7AFA8: FF 50 08           call        qword ptr [rax+8]
  0000000140D7AFAB: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000140D7AFB3: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140D7AFBB: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000140D7AFC3: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140D7AFC8: 48 8B 8C 24 D8 00  mov         rcx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140D7AFD0: FF 15 8A A9 08 01  call        qword ptr [0000000141E05960h]
  0000000140D7AFD6: 90                 nop
  0000000140D7AFD7: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  0000000140D7AFDF: FF 15 D3 A9 08 01  call        qword ptr [0000000141E059B8h]
  0000000140D7AFE5: 48 8B 05 A4 91 08  mov         rax,qword ptr [0000000141E04190h]
                    01
  0000000140D7AFEC: 0F B6 10           movzx       edx,byte ptr [rax]
  0000000140D7AFEF: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140D7AFF4: E8 57 6B 2C FF     call        0000000140041B50
  0000000140D7AFF9: 0F B6 C0           movzx       eax,al
  0000000140D7AFFC: 85 C0              test        eax,eax
  0000000140D7AFFE: 74 3C              je          0000000140D7B03C
  0000000140D7B000: 48 83 BC 24 10 01  cmp         qword ptr [rsp+0000000000000110h],0
                    00 00 00
  0000000140D7B009: 74 17              je          0000000140D7B022
  0000000140D7B00B: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B013: 48 8B 84 24 00 01  mov         rax,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D7B01B: 48 8B 40 50        mov         rax,qword ptr [rax+50h]
  0000000140D7B01F: 48 89 01           mov         qword ptr [rcx],rax
  0000000140D7B022: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D7B02A: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D7B02E: E8 ED 11 F0 FF     call        0000000140C7C220
  0000000140D7B033: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  0000000140D7B037: E9 A5 00 00 00     jmp         0000000140D7B0E1
  0000000140D7B03C: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D7B044: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D7B048: E8 B3 FD EF FF     call        0000000140C7AE00
  0000000140D7B04D: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140D7B052: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  0000000140D7B058: 0F 84 83 00 00 00  je          0000000140D7B0E1
  0000000140D7B05E: 48 83 BC 24 10 01  cmp         qword ptr [rsp+0000000000000110h],0
                    00 00 00
  0000000140D7B067: 74 78              je          0000000140D7B0E1
  0000000140D7B069: 48 83 BC 24 10 01  cmp         qword ptr [rsp+0000000000000110h],0
                    00 00 00
  0000000140D7B072: 74 6D              je          0000000140D7B0E1
  0000000140D7B074: 48 8D 84 24 98 00  lea         rax,[rsp+0000000000000098h]
                    00 00
  0000000140D7B07C: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140D7B084: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140D7B089: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140D7B091: FF 15 41 A9 08 01  call        qword ptr [0000000141E059D8h]
  0000000140D7B097: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140D7B09F: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140D7B0A7: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D7B0AC: E8 8F EF F7 FF     call        0000000140CFA040
  0000000140D7B0B1: 4C 8B D8           mov         r11,rax
  0000000140D7B0B4: 48 8B 84 24 10 01  mov         rax,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B0BC: 4C 89 18           mov         qword ptr [rax],r11
  0000000140D7B0BF: 48 8B 84 24 10 01  mov         rax,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B0C7: 48 83 38 00        cmp         qword ptr [rax],0
  0000000140D7B0CB: 74 14              je          0000000140D7B0E1
  0000000140D7B0CD: 48 8B 8C 24 10 01  mov         rcx,qword ptr [rsp+0000000000000110h]
                    00 00
  0000000140D7B0D5: 48 8B 09           mov         rcx,qword ptr [rcx]
  0000000140D7B0D8: E8 43 11 F0 FF     call        0000000140C7C220
  0000000140D7B0DD: 89 44 24 48        mov         dword ptr [rsp+48h],eax
  0000000140D7B0E1: 83 7C 24 30 00     cmp         dword ptr [rsp+30h],0
  0000000140D7B0E6: 7E 0A              jle         0000000140D7B0F2
  0000000140D7B0E8: 8B 44 24 48        mov         eax,dword ptr [rsp+48h]
  0000000140D7B0EC: 39 44 24 30        cmp         dword ptr [rsp+30h],eax
  0000000140D7B0F0: 7E 08              jle         0000000140D7B0FA
  0000000140D7B0F2: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140D7B0FA: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140D7B0FF: FF 15 DB A8 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7B105: EB 75              jmp         0000000140D7B17C
  0000000140D7B107: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D7B10F: E8 1C 6A FC FF     call        0000000140D41B30
  0000000140D7B114: 83 F8 02           cmp         eax,2
  0000000140D7B117: 75 63              jne         0000000140D7B17C
  0000000140D7B119: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140D7B121: FF 15 61 A8 08 01  call        qword ptr [0000000141E05988h]
  0000000140D7B127: 45 33 C0           xor         r8d,r8d
  0000000140D7B12A: 8B D0              mov         edx,eax
  0000000140D7B12C: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140D7B134: FF 15 1E 05 09 01  call        qword ptr [0000000141E0B658h]
  0000000140D7B13A: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140D7B13F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140D7B147: 4C 8D 0D 0A C1 BF  lea         r9,[0000000143977258h]
                    02
  0000000140D7B14E: 4C 8D 05 7B 3A 7F  lea         r8,[000000014356EBD0h]
                    02
  0000000140D7B155: 33 D2              xor         edx,edx
  0000000140D7B157: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D7B15C: E8 9B 47 6F 00     call        000000014146F8FC
  0000000140D7B161: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140D7B166: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  0000000140D7B16C: 74 0E              je          0000000140D7B17C
  0000000140D7B16E: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D7B173: E8 F8 38 FD FF     call        0000000140D4EA70
  0000000140D7B178: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140D7B17C: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140D7B180: 48 81 C4 F8 00 00  add         rsp,0F8h
                    00
  0000000140D7B187: C3                 ret

  Summary

     1E02000 .text

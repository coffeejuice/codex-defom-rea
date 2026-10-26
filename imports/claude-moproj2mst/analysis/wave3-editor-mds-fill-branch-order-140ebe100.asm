Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EBE100: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000140EBE104: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EBE109: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  0000000140EBE110: 48 C7 44 24 78 FE  mov         qword ptr [rsp+78h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140EBE119: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140EBE122: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140EBE12B: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EBE130: FF 15 8A 63 F4 00  call        qword ptr [0000000141E044C0h]
  0000000140EBE136: 90                 nop
  0000000140EBE137: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  0000000140EBE13C: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000140EBE141: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE149: E8 C2 80 FF FF     call        0000000140EB6210
  0000000140EBE14E: 83 BC 24 A8 00 00  cmp         dword ptr [rsp+00000000000000A8h],0
                    00 00
  0000000140EBE156: 0F 85 1D 03 00 00  jne         0000000140EBE479
  0000000140EBE15C: 83 7C 24 38 00     cmp         dword ptr [rsp+38h],0
  0000000140EBE161: 0F 8E 12 03 00 00  jle         0000000140EBE479
  0000000140EBE167: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE16F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140EBE172: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE17A: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140EBE180: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140EBE184: 48 8B C8           mov         rcx,rax
  0000000140EBE187: E8 F4 CC DB FF     call        0000000140C7AE80
  0000000140EBE18C: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140EBE191: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140EBE197: 75 1D              jne         0000000140EBE1B6
  0000000140EBE199: 48 8D 0D 20 67 C5  lea         rcx,[0000000142B148C0h]
                    01
  0000000140EBE1A0: E8 EB 45 5B 00     call        0000000141472790
  0000000140EBE1A5: 90                 nop
  0000000140EBE1A6: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EBE1AB: FF 15 2F 78 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EBE1B1: E9 3A 06 00 00     jmp         0000000140EBE7F0
  0000000140EBE1B6: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140EBE1BC: 74 10              je          0000000140EBE1CE
  0000000140EBE1BE: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140EBE1C3: 48 05 A0 1E 00 00  add         rax,1EA0h
  0000000140EBE1C9: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140EBE1CE: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140EBE1D4: 0F 84 8A 01 00 00  je          0000000140EBE364
  0000000140EBE1DA: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE1DF: 8B 40 28           mov         eax,dword ptr [rax+28h]
  0000000140EBE1E2: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  0000000140EBE1E9: 83 BC 24 80 00 00  cmp         dword ptr [rsp+0000000000000080h],0
                    00 00
  0000000140EBE1F1: 74 16              je          0000000140EBE209
  0000000140EBE1F3: 83 BC 24 80 00 00  cmp         dword ptr [rsp+0000000000000080h],1
                    00 01
  0000000140EBE1FB: 74 2C              je          0000000140EBE229
  0000000140EBE1FD: 83 BC 24 80 00 00  cmp         dword ptr [rsp+0000000000000080h],2
                    00 02
  0000000140EBE205: 74 02              je          0000000140EBE209
  0000000140EBE207: EB 40              jmp         0000000140EBE249
  0000000140EBE209: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE211: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE218: B2 01              mov         dl,1
  0000000140EBE21A: 48 8B 89 A0 06 00  mov         rcx,qword ptr [rcx+00000000000006A0h]
                    00
  0000000140EBE221: FF 15 71 90 F4 00  call        qword ptr [0000000141E07298h]
  0000000140EBE227: EB 2C              jmp         0000000140EBE255
  0000000140EBE229: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE231: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE238: B2 01              mov         dl,1
  0000000140EBE23A: 48 8B 89 B0 06 00  mov         rcx,qword ptr [rcx+00000000000006B0h]
                    00
  0000000140EBE241: FF 15 51 90 F4 00  call        qword ptr [0000000141E07298h]
  0000000140EBE247: EB 0C              jmp         0000000140EBE255
  0000000140EBE249: 48 8D 0D C0 66 C5  lea         rcx,[0000000142B14910h]
                    01
  0000000140EBE250: E8 3B 45 5B 00     call        0000000141472790
  0000000140EBE255: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE25A: 83 78 28 02        cmp         dword ptr [rax+28h],2
  0000000140EBE25E: 75 24              jne         0000000140EBE284
  0000000140EBE260: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE268: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE26F: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE274: 8B 50 48           mov         edx,dword ptr [rax+48h]
  0000000140EBE277: 48 8B 89 A8 06 00  mov         rcx,qword ptr [rcx+00000000000006A8h]
                    00
  0000000140EBE27E: FF 15 B4 84 F4 00  call        qword ptr [0000000141E06738h]
  0000000140EBE284: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE289: F2 0F 10 50 30     movsd       xmm2,mmword ptr [rax+30h]
  0000000140EBE28E: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EBE293: 48 8D 15 BE 66 C5  lea         rdx,[0000000142B14958h]
                    01
  0000000140EBE29A: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EBE29F: FF 15 73 5C F4 00  call        qword ptr [0000000141E03F18h]
  0000000140EBE2A5: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE2AD: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE2B4: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140EBE2B9: 48 8B 89 D0 06 00  mov         rcx,qword ptr [rcx+00000000000006D0h]
                    00
  0000000140EBE2C0: FF 15 F2 83 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE2C6: 4C 8B 5C 24 20     mov         r11,qword ptr [rsp+20h]
  0000000140EBE2CB: F2 41 0F 10 53 38  movsd       xmm2,mmword ptr [r11+38h]
  0000000140EBE2D1: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EBE2D6: 48 8D 15 83 66 C5  lea         rdx,[0000000142B14960h]
                    01
  0000000140EBE2DD: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EBE2E2: FF 15 30 5C F4 00  call        qword ptr [0000000141E03F18h]
  0000000140EBE2E8: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE2F0: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE2F7: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140EBE2FC: 48 8B 89 E0 06 00  mov         rcx,qword ptr [rcx+00000000000006E0h]
                    00
  0000000140EBE303: FF 15 AF 83 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE309: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE311: E8 CA 2B 27 FF     call        0000000140130EE0
  0000000140EBE316: 0F B6 C0           movzx       eax,al
  0000000140EBE319: 85 C0              test        eax,eax
  0000000140EBE31B: 74 42              je          0000000140EBE35F
  0000000140EBE31D: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE322: F2 0F 10 50 40     movsd       xmm2,mmword ptr [rax+40h]
  0000000140EBE327: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EBE32C: 48 8D 15 35 66 C5  lea         rdx,[0000000142B14968h]
                    01
  0000000140EBE333: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EBE338: FF 15 DA 5B F4 00  call        qword ptr [0000000141E03F18h]
  0000000140EBE33E: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE346: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE34D: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140EBE352: 48 8B 89 F0 06 00  mov         rcx,qword ptr [rcx+00000000000006F0h]
                    00
  0000000140EBE359: FF 15 59 83 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE35F: E9 10 01 00 00     jmp         0000000140EBE474
  0000000140EBE364: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE36C: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE373: B2 01              mov         dl,1
  0000000140EBE375: 48 8B 89 A0 06 00  mov         rcx,qword ptr [rcx+00000000000006A0h]
                    00
  0000000140EBE37C: FF 15 16 8F F4 00  call        qword ptr [0000000141E07298h]
  0000000140EBE382: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE38A: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE391: 33 D2              xor         edx,edx
  0000000140EBE393: 48 8B 89 A8 06 00  mov         rcx,qword ptr [rcx+00000000000006A8h]
                    00
  0000000140EBE39A: FF 15 98 83 F4 00  call        qword ptr [0000000141E06738h]
  0000000140EBE3A0: 48 8D 15 AD 54 C5  lea         rdx,[0000000142B13854h]
                    01
  0000000140EBE3A7: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140EBE3AC: FF 15 CE 60 F4 00  call        qword ptr [0000000141E04480h]
  0000000140EBE3B2: 90                 nop
  0000000140EBE3B3: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE3BB: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE3C2: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000140EBE3C7: 48 8B 89 D0 06 00  mov         rcx,qword ptr [rcx+00000000000006D0h]
                    00
  0000000140EBE3CE: FF 15 E4 82 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE3D4: 90                 nop
  0000000140EBE3D5: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140EBE3DA: FF 15 00 76 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EBE3E0: 48 8D 15 7D 54 C5  lea         rdx,[0000000142B13864h]
                    01
  0000000140EBE3E7: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140EBE3EC: FF 15 8E 60 F4 00  call        qword ptr [0000000141E04480h]
  0000000140EBE3F2: 90                 nop
  0000000140EBE3F3: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE3FB: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE402: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000140EBE407: 48 8B 89 E0 06 00  mov         rcx,qword ptr [rcx+00000000000006E0h]
                    00
  0000000140EBE40E: FF 15 A4 82 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE414: 90                 nop
  0000000140EBE415: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140EBE41A: FF 15 C0 75 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EBE420: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE428: E8 B3 2A 27 FF     call        0000000140130EE0
  0000000140EBE42D: 0F B6 C0           movzx       eax,al
  0000000140EBE430: 85 C0              test        eax,eax
  0000000140EBE432: 74 40              je          0000000140EBE474
  0000000140EBE434: 48 8D 15 89 54 C5  lea         rdx,[0000000142B138C4h]
                    01
  0000000140EBE43B: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140EBE440: FF 15 3A 60 F4 00  call        qword ptr [0000000141E04480h]
  0000000140EBE446: 90                 nop
  0000000140EBE447: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE44F: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE456: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  0000000140EBE45B: 48 8B 89 F0 06 00  mov         rcx,qword ptr [rcx+00000000000006F0h]
                    00
  0000000140EBE462: FF 15 50 82 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE468: 90                 nop
  0000000140EBE469: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  0000000140EBE46E: FF 15 6C 75 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EBE474: E9 6C 03 00 00     jmp         0000000140EBE7E5
  0000000140EBE479: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  0000000140EBE47E: 0F 8E 61 03 00 00  jle         0000000140EBE7E5
  0000000140EBE484: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE48C: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140EBE48F: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE497: FF 90 98 01 00 00  call        qword ptr [rax+0000000000000198h]
  0000000140EBE49D: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  0000000140EBE4A1: 48 8B C8           mov         rcx,rax
  0000000140EBE4A4: E8 D7 C9 DB FF     call        0000000140C7AE80
  0000000140EBE4A9: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140EBE4AE: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140EBE4B4: 75 1D              jne         0000000140EBE4D3
  0000000140EBE4B6: 48 8D 0D B3 64 C5  lea         rcx,[0000000142B14970h]
                    01
  0000000140EBE4BD: E8 CE 42 5B 00     call        0000000141472790
  0000000140EBE4C2: 90                 nop
  0000000140EBE4C3: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EBE4C8: FF 15 12 75 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EBE4CE: E9 1D 03 00 00     jmp         0000000140EBE7F0
  0000000140EBE4D3: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140EBE4D9: 74 10              je          0000000140EBE4EB
  0000000140EBE4DB: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000140EBE4E0: 48 05 A0 1E 00 00  add         rax,1EA0h
  0000000140EBE4E6: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140EBE4EB: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140EBE4F1: 0F 84 8A 01 00 00  je          0000000140EBE681
  0000000140EBE4F7: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE4FC: 8B 40 28           mov         eax,dword ptr [rax+28h]
  0000000140EBE4FF: 89 84 24 84 00 00  mov         dword ptr [rsp+0000000000000084h],eax
                    00
  0000000140EBE506: 83 BC 24 84 00 00  cmp         dword ptr [rsp+0000000000000084h],0
                    00 00
  0000000140EBE50E: 74 16              je          0000000140EBE526
  0000000140EBE510: 83 BC 24 84 00 00  cmp         dword ptr [rsp+0000000000000084h],1
                    00 01
  0000000140EBE518: 74 2C              je          0000000140EBE546
  0000000140EBE51A: 83 BC 24 84 00 00  cmp         dword ptr [rsp+0000000000000084h],2
                    00 02
  0000000140EBE522: 74 02              je          0000000140EBE526
  0000000140EBE524: EB 40              jmp         0000000140EBE566
  0000000140EBE526: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE52E: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE535: B2 01              mov         dl,1
  0000000140EBE537: 48 8B 89 38 07 00  mov         rcx,qword ptr [rcx+0000000000000738h]
                    00
  0000000140EBE53E: FF 15 54 8D F4 00  call        qword ptr [0000000141E07298h]
  0000000140EBE544: EB 2C              jmp         0000000140EBE572
  0000000140EBE546: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE54E: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE555: B2 01              mov         dl,1
  0000000140EBE557: 48 8B 89 48 07 00  mov         rcx,qword ptr [rcx+0000000000000748h]
                    00
  0000000140EBE55E: FF 15 34 8D F4 00  call        qword ptr [0000000141E07298h]
  0000000140EBE564: EB 0C              jmp         0000000140EBE572
  0000000140EBE566: 48 8D 0D 53 64 C5  lea         rcx,[0000000142B149C0h]
                    01
  0000000140EBE56D: E8 1E 42 5B 00     call        0000000141472790
  0000000140EBE572: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE577: 83 78 28 02        cmp         dword ptr [rax+28h],2
  0000000140EBE57B: 75 24              jne         0000000140EBE5A1
  0000000140EBE57D: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE585: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE58C: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE591: 8B 50 48           mov         edx,dword ptr [rax+48h]
  0000000140EBE594: 48 8B 89 40 07 00  mov         rcx,qword ptr [rcx+0000000000000740h]
                    00
  0000000140EBE59B: FF 15 97 81 F4 00  call        qword ptr [0000000141E06738h]
  0000000140EBE5A1: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE5A6: F2 0F 10 50 30     movsd       xmm2,mmword ptr [rax+30h]
  0000000140EBE5AB: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EBE5B0: 48 8D 15 51 64 C5  lea         rdx,[0000000142B14A08h]
                    01
  0000000140EBE5B7: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EBE5BC: FF 15 56 59 F4 00  call        qword ptr [0000000141E03F18h]
  0000000140EBE5C2: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE5CA: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE5D1: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140EBE5D6: 48 8B 89 68 07 00  mov         rcx,qword ptr [rcx+0000000000000768h]
                    00
  0000000140EBE5DD: FF 15 D5 80 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE5E3: 4C 8B 5C 24 20     mov         r11,qword ptr [rsp+20h]
  0000000140EBE5E8: F2 41 0F 10 53 38  movsd       xmm2,mmword ptr [r11+38h]
  0000000140EBE5EE: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EBE5F3: 48 8D 15 16 64 C5  lea         rdx,[0000000142B14A10h]
                    01
  0000000140EBE5FA: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EBE5FF: FF 15 13 59 F4 00  call        qword ptr [0000000141E03F18h]
  0000000140EBE605: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE60D: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE614: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140EBE619: 48 8B 89 78 07 00  mov         rcx,qword ptr [rcx+0000000000000778h]
                    00
  0000000140EBE620: FF 15 92 80 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE626: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE62E: E8 AD 28 27 FF     call        0000000140130EE0
  0000000140EBE633: 0F B6 C0           movzx       eax,al
  0000000140EBE636: 85 C0              test        eax,eax
  0000000140EBE638: 74 42              je          0000000140EBE67C
  0000000140EBE63A: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE63F: F2 0F 10 50 40     movsd       xmm2,mmword ptr [rax+40h]
  0000000140EBE644: 66 49 0F 7E D0     movq        r8,xmm2
  0000000140EBE649: 48 8D 15 C8 63 C5  lea         rdx,[0000000142B14A18h]
                    01
  0000000140EBE650: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EBE655: FF 15 BD 58 F4 00  call        qword ptr [0000000141E03F18h]
  0000000140EBE65B: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE663: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE66A: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000140EBE66F: 48 8B 89 88 07 00  mov         rcx,qword ptr [rcx+0000000000000788h]
                    00
  0000000140EBE676: FF 15 3C 80 F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE67C: E9 64 01 00 00     jmp         0000000140EBE7E5
  0000000140EBE681: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140EBE686: 8B 40 28           mov         eax,dword ptr [rax+28h]
  0000000140EBE689: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  0000000140EBE690: 83 BC 24 88 00 00  cmp         dword ptr [rsp+0000000000000088h],0
                    00 00
  0000000140EBE698: 74 0C              je          0000000140EBE6A6
  0000000140EBE69A: 83 BC 24 88 00 00  cmp         dword ptr [rsp+0000000000000088h],1
                    00 01
  0000000140EBE6A2: 74 22              je          0000000140EBE6C6
  0000000140EBE6A4: EB 40              jmp         0000000140EBE6E6
  0000000140EBE6A6: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE6AE: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE6B5: B2 01              mov         dl,1
  0000000140EBE6B7: 48 8B 89 38 07 00  mov         rcx,qword ptr [rcx+0000000000000738h]
                    00
  0000000140EBE6BE: FF 15 D4 8B F4 00  call        qword ptr [0000000141E07298h]
  0000000140EBE6C4: EB 2C              jmp         0000000140EBE6F2
  0000000140EBE6C6: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE6CE: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE6D5: B2 01              mov         dl,1
  0000000140EBE6D7: 48 8B 89 48 07 00  mov         rcx,qword ptr [rcx+0000000000000748h]
                    00
  0000000140EBE6DE: FF 15 B4 8B F4 00  call        qword ptr [0000000141E07298h]
  0000000140EBE6E4: EB 0C              jmp         0000000140EBE6F2
  0000000140EBE6E6: 48 8D 0D 33 63 C5  lea         rcx,[0000000142B14A20h]
                    01
  0000000140EBE6ED: E8 9E 40 5B 00     call        0000000141472790
  0000000140EBE6F2: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE6FA: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE701: 33 D2              xor         edx,edx
  0000000140EBE703: 48 8B 89 40 07 00  mov         rcx,qword ptr [rcx+0000000000000740h]
                    00
  0000000140EBE70A: FF 15 28 80 F4 00  call        qword ptr [0000000141E06738h]
  0000000140EBE710: 48 8D 15 3D 56 C5  lea         rdx,[0000000142B13D54h]
                    01
  0000000140EBE717: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140EBE71C: FF 15 5E 5D F4 00  call        qword ptr [0000000141E04480h]
  0000000140EBE722: 90                 nop
  0000000140EBE723: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE72B: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE732: 48 8D 54 24 60     lea         rdx,[rsp+60h]
  0000000140EBE737: 48 8B 89 68 07 00  mov         rcx,qword ptr [rcx+0000000000000768h]
                    00
  0000000140EBE73E: FF 15 74 7F F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE744: 90                 nop
  0000000140EBE745: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140EBE74A: FF 15 90 72 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EBE750: 48 8D 15 11 63 C5  lea         rdx,[0000000142B14A68h]
                    01
  0000000140EBE757: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140EBE75C: FF 15 1E 5D F4 00  call        qword ptr [0000000141E04480h]
  0000000140EBE762: 90                 nop
  0000000140EBE763: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE76B: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE772: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  0000000140EBE777: 48 8B 89 78 07 00  mov         rcx,qword ptr [rcx+0000000000000778h]
                    00
  0000000140EBE77E: FF 15 34 7F F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE784: 90                 nop
  0000000140EBE785: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140EBE78A: FF 15 50 72 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EBE790: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE798: E8 43 27 27 FF     call        0000000140130EE0
  0000000140EBE79D: 0F B6 C0           movzx       eax,al
  0000000140EBE7A0: 85 C0              test        eax,eax
  0000000140EBE7A2: 74 41              je          0000000140EBE7E5
  0000000140EBE7A4: 48 8D 15 C1 62 C5  lea         rdx,[0000000142B14A6Ch]
                    01
  0000000140EBE7AB: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140EBE7B0: FF 15 CA 5C F4 00  call        qword ptr [0000000141E04480h]
  0000000140EBE7B6: 90                 nop
  0000000140EBE7B7: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140EBE7BF: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EBE7C6: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140EBE7CB: 48 8B 89 88 07 00  mov         rcx,qword ptr [rcx+0000000000000788h]
                    00
  0000000140EBE7D2: FF 15 E0 7E F4 00  call        qword ptr [0000000141E066B8h]
  0000000140EBE7D8: 90                 nop
  0000000140EBE7D9: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140EBE7DE: FF 15 FC 71 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EBE7E4: 90                 nop
  0000000140EBE7E5: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140EBE7EA: FF 15 F0 71 F4 00  call        qword ptr [0000000141E059E0h]
  0000000140EBE7F0: 48 81 C4 98 00 00  add         rsp,98h
                    00
  0000000140EBE7F7: C3                 ret

  Summary

     1E02000 .text

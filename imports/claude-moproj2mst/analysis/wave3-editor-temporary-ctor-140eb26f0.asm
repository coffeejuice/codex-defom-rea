Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EB26F0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140EB26F5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140EB26FA: 57                 push        rdi
  0000000140EB26FB: 48 81 EC 10 01 00  sub         rsp,110h
                    00
  0000000140EB2702: 48 C7 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140EB270E: 45 33 C0           xor         r8d,r8d
  0000000140EB2711: 48 8B 94 24 28 01  mov         rdx,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140EB2719: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2721: E8 1A E0 27 FF     call        0000000140130740
  0000000140EB2726: 90                 nop
  0000000140EB2727: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB272F: 48 8D 05 82 3E C6  lea         rax,[0000000142B165B8h]
                    01
  0000000140EB2736: 48 89 01           mov         qword ptr [rcx],rax
  0000000140EB2739: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2741: 48 8D 05 38 3E C6  lea         rax,[0000000142B16580h]
                    01
  0000000140EB2748: 48 89 41 10        mov         qword ptr [rcx+10h],rax
  0000000140EB274C: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2754: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140EB275B: FF 15 0F 63 F5 00  call        qword ptr [0000000141E08A70h]
  0000000140EB2761: 90                 nop
  0000000140EB2762: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB276A: 48 81 C1 68 01 00  add         rcx,168h
                    00
  0000000140EB2771: E8 6A D2 2D FF     call        000000014018F9E0
  0000000140EB2776: 90                 nop
  0000000140EB2777: 48 C7 44 24 40 08  mov         qword ptr [rsp+40h],1808h
                    18 00 00
  0000000140EB2780: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140EB2785: E8 32 C3 5B 00     call        000000014146EABC
  0000000140EB278A: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140EB278F: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140EB2795: 74 1D              je          0000000140EB27B4
  0000000140EB2797: 48 8B 7C 24 48     mov         rdi,qword ptr [rsp+48h]
  0000000140EB279C: 33 C0              xor         eax,eax
  0000000140EB279E: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140EB27A3: F3 AA              rep stos    byte ptr [rdi]
  0000000140EB27A5: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140EB27AA: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140EB27B2: EB 0C              jmp         0000000140EB27C0
  0000000140EB27B4: 48 C7 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],0
                    00 00 00 00 00 00
  0000000140EB27C0: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB27C8: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140EB27D0: 48 89 81 40 01 00  mov         qword ptr [rcx+0000000000000140h],rax
                    00
  0000000140EB27D7: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB27DF: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB27E7: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB27EE: E8 3D 70 02 00     call        0000000140ED9830
  0000000140EB27F3: 48 83 BC 24 28 01  cmp         qword ptr [rsp+0000000000000128h],0
                    00 00 00
  0000000140EB27FC: 74 0D              je          0000000140EB280B
  0000000140EB27FE: 48 8B 8C 24 28 01  mov         rcx,qword ptr [rsp+0000000000000128h]
                    00 00
  0000000140EB2806: E8 35 1D 1A FF     call        0000000140054540
  0000000140EB280B: B9 28 00 00 00     mov         ecx,28h
  0000000140EB2810: E8 A7 C2 5B 00     call        000000014146EABC
  0000000140EB2815: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140EB281A: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  0000000140EB2820: 74 34              je          0000000140EB2856
  0000000140EB2822: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB282A: E8 21 53 4C FF     call        0000000140377B50
  0000000140EB282F: 48 8B C8           mov         rcx,rax
  0000000140EB2832: E8 D9 E6 DD FF     call        0000000140C90F10
  0000000140EB2837: 4C 8B C0           mov         r8,rax
  0000000140EB283A: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2842: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140EB2847: E8 84 19 08 00     call        0000000140F341D0
  0000000140EB284C: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  0000000140EB2854: EB 0C              jmp         0000000140EB2862
  0000000140EB2856: 48 C7 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],0
                    00 00 00 00 00 00
  0000000140EB2862: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140EB286A: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140EB286F: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2877: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140EB287C: 48 89 81 50 01 00  mov         qword ptr [rcx+0000000000000150h],rax
                    00
  0000000140EB2883: 33 D2              xor         edx,edx
  0000000140EB2885: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB288D: E8 6E CA 01 00     call        0000000140ECF300
  0000000140EB2892: 33 D2              xor         edx,edx
  0000000140EB2894: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB289C: E8 7F CA 01 00     call        0000000140ECF320
  0000000140EB28A1: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB28A9: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB28B0: 48 8B 89 F0 16 00  mov         rcx,qword ptr [rcx+00000000000016F0h]
                    00
  0000000140EB28B7: FF 15 F3 92 F5 00  call        qword ptr [0000000141E0BBB0h]
  0000000140EB28BD: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB28C5: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB28CC: 33 D2              xor         edx,edx
  0000000140EB28CE: 48 8B 89 00 17 00  mov         rcx,qword ptr [rcx+0000000000001700h]
                    00
  0000000140EB28D5: FF 15 8D 92 F5 00  call        qword ptr [0000000141E0BB68h]
  0000000140EB28DB: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB28E3: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB28EA: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB28F2: 4C 8D 0D 5F 10 C6  lea         r9,[0000000142B13958h]
                    01
  0000000140EB28F9: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2901: 48 8D 15 80 10 C6  lea         rdx,[0000000142B13988h]
                    01
  0000000140EB2908: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140EB290C: FF 15 BE 18 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2912: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB291A: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2921: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2929: 4C 8D 0D 98 10 C6  lea         r9,[0000000142B139C8h]
                    01
  0000000140EB2930: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2938: 48 8D 15 99 10 C6  lea         rdx,[0000000142B139D8h]
                    01
  0000000140EB293F: 48 8B 49 58        mov         rcx,qword ptr [rcx+58h]
  0000000140EB2943: FF 15 87 18 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2949: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2951: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2958: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2960: 4C 8D 0D 81 10 C6  lea         r9,[0000000142B139E8h]
                    01
  0000000140EB2967: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB296F: 48 8D 15 82 10 C6  lea         rdx,[0000000142B139F8h]
                    01
  0000000140EB2976: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140EB297A: FF 15 50 18 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2980: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2988: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB298F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2997: 4C 8D 0D 6A 10 C6  lea         r9,[0000000142B13A08h]
                    01
  0000000140EB299E: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB29A6: 48 8D 15 7B 10 C6  lea         rdx,[0000000142B13A28h]
                    01
  0000000140EB29AD: 48 8B 89 F8 0C 00  mov         rcx,qword ptr [rcx+0000000000000CF8h]
                    00
  0000000140EB29B4: FF 15 16 18 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB29BA: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB29C2: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB29C9: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB29D1: 4C 8D 0D 60 10 C6  lea         r9,[0000000142B13A38h]
                    01
  0000000140EB29D8: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB29E0: 48 8D 15 71 10 C6  lea         rdx,[0000000142B13A58h]
                    01
  0000000140EB29E7: 48 8B 89 08 0D 00  mov         rcx,qword ptr [rcx+0000000000000D08h]
                    00
  0000000140EB29EE: FF 15 DC 17 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB29F4: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB29FC: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2A03: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2A0B: 4C 8D 0D 56 10 C6  lea         r9,[0000000142B13A68h]
                    01
  0000000140EB2A12: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2A1A: 48 8D 15 67 10 C6  lea         rdx,[0000000142B13A88h]
                    01
  0000000140EB2A21: 48 8B 89 58 0D 00  mov         rcx,qword ptr [rcx+0000000000000D58h]
                    00
  0000000140EB2A28: FF 15 A2 17 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2A2E: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2A36: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2A3D: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2A45: 4C 8D 0D 4C 10 C6  lea         r9,[0000000142B13A98h]
                    01
  0000000140EB2A4C: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2A54: 48 8D 15 5D 10 C6  lea         rdx,[0000000142B13AB8h]
                    01
  0000000140EB2A5B: 48 8B 89 70 0D 00  mov         rcx,qword ptr [rcx+0000000000000D70h]
                    00
  0000000140EB2A62: FF 15 68 17 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2A68: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2A70: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2A77: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2A7F: 4C 8D 0D 42 10 C6  lea         r9,[0000000142B13AC8h]
                    01
  0000000140EB2A86: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2A8E: 48 8D 15 53 10 C6  lea         rdx,[0000000142B13AE8h]
                    01
  0000000140EB2A95: 48 8B 89 08 0E 00  mov         rcx,qword ptr [rcx+0000000000000E08h]
                    00
  0000000140EB2A9C: FF 15 2E 17 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2AA2: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2AAA: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2AB1: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2AB9: 4C 8D 0D 38 10 C6  lea         r9,[0000000142B13AF8h]
                    01
  0000000140EB2AC0: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2AC8: 48 8D 15 49 10 C6  lea         rdx,[0000000142B13B18h]
                    01
  0000000140EB2ACF: 48 8B 89 28 0E 00  mov         rcx,qword ptr [rcx+0000000000000E28h]
                    00
  0000000140EB2AD6: FF 15 F4 16 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2ADC: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2AE4: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2AEB: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2AF3: 4C 8D 0D 2E 10 C6  lea         r9,[0000000142B13B28h]
                    01
  0000000140EB2AFA: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2B02: 48 8D 15 3F 10 C6  lea         rdx,[0000000142B13B48h]
                    01
  0000000140EB2B09: 48 8B 89 70 0E 00  mov         rcx,qword ptr [rcx+0000000000000E70h]
                    00
  0000000140EB2B10: FF 15 BA 16 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2B16: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2B1E: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2B25: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2B2D: 4C 8D 0D 24 10 C6  lea         r9,[0000000142B13B58h]
                    01
  0000000140EB2B34: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2B3C: 48 8D 15 35 10 C6  lea         rdx,[0000000142B13B78h]
                    01
  0000000140EB2B43: 48 8B 89 80 0E 00  mov         rcx,qword ptr [rcx+0000000000000E80h]
                    00
  0000000140EB2B4A: FF 15 80 16 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2B50: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2B58: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2B5F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2B67: 4C 8D 0D 1A 10 C6  lea         r9,[0000000142B13B88h]
                    01
  0000000140EB2B6E: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2B76: 48 8D 15 2B 10 C6  lea         rdx,[0000000142B13BA8h]
                    01
  0000000140EB2B7D: 48 8B 89 78 0E 00  mov         rcx,qword ptr [rcx+0000000000000E78h]
                    00
  0000000140EB2B84: FF 15 46 16 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2B8A: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2B92: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2B99: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2BA1: 4C 8D 0D 10 10 C6  lea         r9,[0000000142B13BB8h]
                    01
  0000000140EB2BA8: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2BB0: 48 8D 15 21 10 C6  lea         rdx,[0000000142B13BD8h]
                    01
  0000000140EB2BB7: 48 8B 89 88 0E 00  mov         rcx,qword ptr [rcx+0000000000000E88h]
                    00
  0000000140EB2BBE: FF 15 0C 16 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2BC4: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2BCC: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2BD3: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2BDB: 4C 8D 0D 06 10 C6  lea         r9,[0000000142B13BE8h]
                    01
  0000000140EB2BE2: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2BEA: 48 8D 15 0F 10 C6  lea         rdx,[0000000142B13C00h]
                    01
  0000000140EB2BF1: 48 8B 89 F0 02 00  mov         rcx,qword ptr [rcx+00000000000002F0h]
                    00
  0000000140EB2BF8: FF 15 D2 15 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2BFE: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2C06: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2C0D: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2C15: 4C 8D 0D F4 0F C6  lea         r9,[0000000142B13C10h]
                    01
  0000000140EB2C1C: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2C24: 48 8D 15 FD 0F C6  lea         rdx,[0000000142B13C28h]
                    01
  0000000140EB2C2B: 48 8B 89 F8 02 00  mov         rcx,qword ptr [rcx+00000000000002F8h]
                    00
  0000000140EB2C32: FF 15 98 15 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2C38: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2C40: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2C47: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2C4F: 4C 8D 0D E2 0F C6  lea         r9,[0000000142B13C38h]
                    01
  0000000140EB2C56: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2C5E: 48 8D 15 EB 0F C6  lea         rdx,[0000000142B13C50h]
                    01
  0000000140EB2C65: 48 8B 89 00 03 00  mov         rcx,qword ptr [rcx+0000000000000300h]
                    00
  0000000140EB2C6C: FF 15 5E 15 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2C72: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2C7A: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2C81: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2C89: 4C 8D 0D D0 0F C6  lea         r9,[0000000142B13C60h]
                    01
  0000000140EB2C90: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2C98: 48 8D 15 F1 0F C6  lea         rdx,[0000000142B13C90h]
                    01
  0000000140EB2C9F: 48 8B 89 88 09 00  mov         rcx,qword ptr [rcx+0000000000000988h]
                    00
  0000000140EB2CA6: FF 15 24 15 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2CAC: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2CB4: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2CBB: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2CC3: 4C 8D 0D D6 0F C6  lea         r9,[0000000142B13CA0h]
                    01
  0000000140EB2CCA: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2CD2: 48 8D 15 F7 0F C6  lea         rdx,[0000000142B13CD0h]
                    01
  0000000140EB2CD9: 48 8B 89 78 09 00  mov         rcx,qword ptr [rcx+0000000000000978h]
                    00
  0000000140EB2CE0: FF 15 EA 14 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2CE6: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2CEE: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2CF5: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2CFD: 4C 8D 0D DC 0F C6  lea         r9,[0000000142B13CE0h]
                    01
  0000000140EB2D04: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2D0C: 48 8D 15 E5 0F C6  lea         rdx,[0000000142B13CF8h]
                    01
  0000000140EB2D13: 48 8B 89 E8 07 00  mov         rcx,qword ptr [rcx+00000000000007E8h]
                    00
  0000000140EB2D1A: FF 15 B0 14 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2D20: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2D28: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2D2F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2D37: 4C 8D 0D CA 0F C6  lea         r9,[0000000142B13D08h]
                    01
  0000000140EB2D3E: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2D46: 48 8D 15 D3 0F C6  lea         rdx,[0000000142B13D20h]
                    01
  0000000140EB2D4D: 48 8B 89 08 08 00  mov         rcx,qword ptr [rcx+0000000000000808h]
                    00
  0000000140EB2D54: FF 15 76 14 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2D5A: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2D62: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2D69: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2D71: 4C 8D 0D B8 0F C6  lea         r9,[0000000142B13D30h]
                    01
  0000000140EB2D78: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2D80: 48 8D 15 D1 0F C6  lea         rdx,[0000000142B13D58h]
                    01
  0000000140EB2D87: 48 8B 89 38 01 00  mov         rcx,qword ptr [rcx+0000000000000138h]
                    00
  0000000140EB2D8E: FF 15 3C 14 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2D94: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2D9C: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2DA3: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2DAB: 4C 8D 0D C6 0F C6  lea         r9,[0000000142B13D78h]
                    01
  0000000140EB2DB2: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2DBA: 48 8D 15 DF 0F C6  lea         rdx,[0000000142B13DA0h]
                    01
  0000000140EB2DC1: 48 8B 89 28 01 00  mov         rcx,qword ptr [rcx+0000000000000128h]
                    00
  0000000140EB2DC8: FF 15 02 14 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2DCE: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2DD6: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2DDD: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2DE5: 4C 8D 0D D4 0F C6  lea         r9,[0000000142B13DC0h]
                    01
  0000000140EB2DEC: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2DF4: 48 8D 15 F5 0F C6  lea         rdx,[0000000142B13DF0h]
                    01
  0000000140EB2DFB: 48 8B 89 10 16 00  mov         rcx,qword ptr [rcx+0000000000001610h]
                    00
  0000000140EB2E02: FF 15 C8 13 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2E08: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2E10: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2E17: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2E1F: 4C 8D 0D EA 0F C6  lea         r9,[0000000142B13E10h]
                    01
  0000000140EB2E26: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2E2E: 48 8D 15 03 10 C6  lea         rdx,[0000000142B13E38h]
                    01
  0000000140EB2E35: 48 8B 89 38 08 00  mov         rcx,qword ptr [rcx+0000000000000838h]
                    00
  0000000140EB2E3C: FF 15 8E 13 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2E42: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2E4A: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2E51: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2E59: 4C 8D 0D E8 0F C6  lea         r9,[0000000142B13E48h]
                    01
  0000000140EB2E60: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2E68: 48 8D 15 01 10 C6  lea         rdx,[0000000142B13E70h]
                    01
  0000000140EB2E6F: 48 8B 89 08 07 00  mov         rcx,qword ptr [rcx+0000000000000708h]
                    00
  0000000140EB2E76: FF 15 54 13 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2E7C: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2E84: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2E8B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2E93: 4C 8D 0D E6 0F C6  lea         r9,[0000000142B13E80h]
                    01
  0000000140EB2E9A: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2EA2: 48 8D 15 FF 0F C6  lea         rdx,[0000000142B13EA8h]
                    01
  0000000140EB2EA9: 48 8B 89 A0 07 00  mov         rcx,qword ptr [rcx+00000000000007A0h]
                    00
  0000000140EB2EB0: FF 15 1A 13 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2EB6: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2EBE: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2EC5: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2ECD: 4C 8D 0D E4 0F C6  lea         r9,[0000000142B13EB8h]
                    01
  0000000140EB2ED4: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2EDC: 48 8D 15 F5 0F C6  lea         rdx,[0000000142B13ED8h]
                    01
  0000000140EB2EE3: 48 8B 89 10 07 00  mov         rcx,qword ptr [rcx+0000000000000710h]
                    00
  0000000140EB2EEA: FF 15 E0 12 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2EF0: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2EF8: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2EFF: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2F07: 4C 8D 0D DA 0F C6  lea         r9,[0000000142B13EE8h]
                    01
  0000000140EB2F0E: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2F16: 48 8D 15 EB 0F C6  lea         rdx,[0000000142B13F08h]
                    01
  0000000140EB2F1D: 48 8B 89 A8 07 00  mov         rcx,qword ptr [rcx+00000000000007A8h]
                    00
  0000000140EB2F24: FF 15 A6 12 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2F2A: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2F32: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2F39: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2F41: 4C 8D 0D D0 0F C6  lea         r9,[0000000142B13F18h]
                    01
  0000000140EB2F48: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2F50: 48 8D 15 E9 0F C6  lea         rdx,[0000000142B13F40h]
                    01
  0000000140EB2F57: 48 8B 89 C0 09 00  mov         rcx,qword ptr [rcx+00000000000009C0h]
                    00
  0000000140EB2F5E: FF 15 6C 12 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2F64: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2F6C: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2F73: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2F7B: 4C 8D 0D CE 0F C6  lea         r9,[0000000142B13F50h]
                    01
  0000000140EB2F82: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2F8A: 48 8D 15 D7 0F C6  lea         rdx,[0000000142B13F68h]
                    01
  0000000140EB2F91: 48 8B 89 D0 06 00  mov         rcx,qword ptr [rcx+00000000000006D0h]
                    00
  0000000140EB2F98: FF 15 32 12 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2F9E: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2FA6: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2FAD: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2FB5: 4C 8D 0D C4 0F C6  lea         r9,[0000000142B13F80h]
                    01
  0000000140EB2FBC: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2FC4: 48 8D 15 CD 0F C6  lea         rdx,[0000000142B13F98h]
                    01
  0000000140EB2FCB: 48 8B 89 E0 06 00  mov         rcx,qword ptr [rcx+00000000000006E0h]
                    00
  0000000140EB2FD2: FF 15 F8 11 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB2FD8: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2FE0: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB2FE7: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB2FEF: 4C 8D 0D BA 0F C6  lea         r9,[0000000142B13FB0h]
                    01
  0000000140EB2FF6: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB2FFE: 48 8D 15 C3 0F C6  lea         rdx,[0000000142B13FC8h]
                    01
  0000000140EB3005: 48 8B 89 F0 06 00  mov         rcx,qword ptr [rcx+00000000000006F0h]
                    00
  0000000140EB300C: FF 15 BE 11 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB3012: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB301A: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB3021: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB3029: 4C 8D 0D B0 0F C6  lea         r9,[0000000142B13FE0h]
                    01
  0000000140EB3030: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3038: 48 8D 15 B9 0F C6  lea         rdx,[0000000142B13FF8h]
                    01
  0000000140EB303F: 48 8B 89 68 07 00  mov         rcx,qword ptr [rcx+0000000000000768h]
                    00
  0000000140EB3046: FF 15 84 11 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB304C: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3054: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB305B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB3063: 4C 8D 0D A6 0F C6  lea         r9,[0000000142B14010h]
                    01
  0000000140EB306A: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3072: 48 8D 15 AF 0F C6  lea         rdx,[0000000142B14028h]
                    01
  0000000140EB3079: 48 8B 89 78 07 00  mov         rcx,qword ptr [rcx+0000000000000778h]
                    00
  0000000140EB3080: FF 15 4A 11 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB3086: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB308E: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB3095: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB309D: 4C 8D 0D 9C 0F C6  lea         r9,[0000000142B14040h]
                    01
  0000000140EB30A4: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB30AC: 48 8D 15 A5 0F C6  lea         rdx,[0000000142B14058h]
                    01
  0000000140EB30B3: 48 8B 89 88 07 00  mov         rcx,qword ptr [rcx+0000000000000788h]
                    00
  0000000140EB30BA: FF 15 10 11 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB30C0: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB30C8: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB30CF: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB30D7: 4C 8D 0D 92 0F C6  lea         r9,[0000000142B14070h]
                    01
  0000000140EB30DE: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB30E6: 48 8D 15 A3 0F C6  lea         rdx,[0000000142B14090h]
                    01
  0000000140EB30ED: 48 8B 89 A8 06 00  mov         rcx,qword ptr [rcx+00000000000006A8h]
                    00
  0000000140EB30F4: FF 15 D6 10 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB30FA: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3102: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB3109: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB3111: 4C 8D 0D 90 0F C6  lea         r9,[0000000142B140A8h]
                    01
  0000000140EB3118: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3120: 48 8D 15 A1 0F C6  lea         rdx,[0000000142B140C8h]
                    01
  0000000140EB3127: 48 8B 89 40 07 00  mov         rcx,qword ptr [rcx+0000000000000740h]
                    00
  0000000140EB312E: FF 15 9C 10 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB3134: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB313C: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB3143: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB314B: 4C 8D 0D 8E 0F C6  lea         r9,[0000000142B140E0h]
                    01
  0000000140EB3152: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB315A: 48 8D 15 9F 0F C6  lea         rdx,[0000000142B14100h]
                    01
  0000000140EB3161: 48 8B 89 C0 07 00  mov         rcx,qword ptr [rcx+00000000000007C0h]
                    00
  0000000140EB3168: FF 15 62 10 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB316E: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3176: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB317D: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB3185: 4C 8D 0D 84 0F C6  lea         r9,[0000000142B14110h]
                    01
  0000000140EB318C: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3194: 48 8D 15 9D 0F C6  lea         rdx,[0000000142B14138h]
                    01
  0000000140EB319B: 48 8B 89 20 05 00  mov         rcx,qword ptr [rcx+0000000000000520h]
                    00
  0000000140EB31A2: FF 15 28 10 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB31A8: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB31B0: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB31B7: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB31BF: 4C 8D 0D 8A 0F C6  lea         r9,[0000000142B14150h]
                    01
  0000000140EB31C6: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB31CE: 48 8D 15 93 0F C6  lea         rdx,[0000000142B14168h]
                    01
  0000000140EB31D5: 48 8B 89 E0 0C 00  mov         rcx,qword ptr [rcx+0000000000000CE0h]
                    00
  0000000140EB31DC: FF 15 EE 0F F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB31E2: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB31EA: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB31F1: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB31F9: 4C 8D 0D 78 0F C6  lea         r9,[0000000142B14178h]
                    01
  0000000140EB3200: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3208: 48 8D 15 81 0F C6  lea         rdx,[0000000142B14190h]
                    01
  0000000140EB320F: 48 8B 89 40 02 00  mov         rcx,qword ptr [rcx+0000000000000240h]
                    00
  0000000140EB3216: FF 15 B4 0F F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB321C: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3224: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB322B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB3233: 4C 8D 0D 66 0F C6  lea         r9,[0000000142B141A0h]
                    01
  0000000140EB323A: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3242: 48 8D 15 6F 0F C6  lea         rdx,[0000000142B141B8h]
                    01
  0000000140EB3249: 48 8B 89 E8 0D 00  mov         rcx,qword ptr [rcx+0000000000000DE8h]
                    00
  0000000140EB3250: FF 15 7A 0F F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB3256: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB325E: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB3265: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB326D: 4C 8D 0D 54 0F C6  lea         r9,[0000000142B141C8h]
                    01
  0000000140EB3274: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB327C: 48 8D 15 5D 0F C6  lea         rdx,[0000000142B141E0h]
                    01
  0000000140EB3283: 48 8B 89 80 0D 00  mov         rcx,qword ptr [rcx+0000000000000D80h]
                    00
  0000000140EB328A: FF 15 40 0F F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB3290: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3298: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB329F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB32A7: 4C 8D 0D 42 0F C6  lea         r9,[0000000142B141F0h]
                    01
  0000000140EB32AE: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB32B6: 48 8D 15 4B 0F C6  lea         rdx,[0000000142B14208h]
                    01
  0000000140EB32BD: 48 8B 89 98 0E 00  mov         rcx,qword ptr [rcx+0000000000000E98h]
                    00
  0000000140EB32C4: FF 15 06 0F F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB32CA: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB32D2: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB32D9: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB32E1: 4C 8D 0D 30 0F C6  lea         r9,[0000000142B14218h]
                    01
  0000000140EB32E8: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB32F0: 48 8D 15 39 0F C6  lea         rdx,[0000000142B14230h]
                    01
  0000000140EB32F7: 48 8B 89 10 02 00  mov         rcx,qword ptr [rcx+0000000000000210h]
                    00
  0000000140EB32FE: FF 15 CC 0E F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB3304: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB330C: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB3313: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB331B: 4C 8D 0D 1E 0F C6  lea         r9,[0000000142B14240h]
                    01
  0000000140EB3322: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB332A: 48 8D 15 2F 0F C6  lea         rdx,[0000000142B14260h]
                    01
  0000000140EB3331: 48 8B 89 50 02 00  mov         rcx,qword ptr [rcx+0000000000000250h]
                    00
  0000000140EB3338: FF 15 92 0E F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB333E: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3346: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB334D: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB3355: 4C 8D 0D 14 0F C6  lea         r9,[0000000142B14270h]
                    01
  0000000140EB335C: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3364: 48 8D 15 25 0F C6  lea         rdx,[0000000142B14290h]
                    01
  0000000140EB336B: 48 8B 89 F8 01 00  mov         rcx,qword ptr [rcx+00000000000001F8h]
                    00
  0000000140EB3372: FF 15 58 0E F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB3378: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3380: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB3387: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB338F: 4C 8D 0D 0A 0F C6  lea         r9,[0000000142B142A0h]
                    01
  0000000140EB3396: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB339E: 48 8D 15 23 0F C6  lea         rdx,[0000000142B142C8h]
                    01
  0000000140EB33A5: 48 8B 89 B8 09 00  mov         rcx,qword ptr [rcx+00000000000009B8h]
                    00
  0000000140EB33AC: FF 15 1E 0E F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB33B2: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB33BA: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB33C1: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB33C9: 4C 8D 0D 08 0F C6  lea         r9,[0000000142B142D8h]
                    01
  0000000140EB33D0: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB33D8: 48 8D 15 21 0F C6  lea         rdx,[0000000142B14300h]
                    01
  0000000140EB33DF: 48 8B 89 A0 11 00  mov         rcx,qword ptr [rcx+00000000000011A0h]
                    00
  0000000140EB33E6: FF 15 E4 0D F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB33EC: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB33F4: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB33FB: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB3403: 4C 8D 0D 06 0F C6  lea         r9,[0000000142B14310h]
                    01
  0000000140EB340A: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3412: 48 8D 15 1F 0F C6  lea         rdx,[0000000142B14338h]
                    01
  0000000140EB3419: 48 8B 89 80 11 00  mov         rcx,qword ptr [rcx+0000000000001180h]
                    00
  0000000140EB3420: FF 15 AA 0D F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB3426: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB342E: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB3435: 33 D2              xor         edx,edx
  0000000140EB3437: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140EB343B: FF 15 07 4C F5 00  call        qword ptr [0000000141E08048h]
  0000000140EB3441: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140EB3449: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3451: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB3458: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3460: 48 8B 40 78        mov         rax,qword ptr [rax+78h]
  0000000140EB3464: 48 89 41 50        mov         qword ptr [rcx+50h],rax
  0000000140EB3468: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3470: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB3477: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB347F: 48 8B 40 70        mov         rax,qword ptr [rax+70h]
  0000000140EB3483: 48 89 41 58        mov         qword ptr [rcx+58h],rax
  0000000140EB3487: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB348F: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB3496: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB349E: 48 8B 80 90 00 00  mov         rax,qword ptr [rax+0000000000000090h]
                    00
  0000000140EB34A5: 48 89 41 60        mov         qword ptr [rcx+60h],rax
  0000000140EB34A9: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB34B1: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB34B8: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB34C0: 48 8B 80 C8 00 00  mov         rax,qword ptr [rax+00000000000000C8h]
                    00
  0000000140EB34C7: 48 89 41 68        mov         qword ptr [rcx+68h],rax
  0000000140EB34CB: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB34D3: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB34DA: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB34E2: 48 8B 80 A0 00 00  mov         rax,qword ptr [rax+00000000000000A0h]
                    00
  0000000140EB34E9: 48 89 41 70        mov         qword ptr [rcx+70h],rax
  0000000140EB34ED: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB34F5: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB34FC: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3504: 48 8B 80 A8 00 00  mov         rax,qword ptr [rax+00000000000000A8h]
                    00
  0000000140EB350B: 48 89 41 78        mov         qword ptr [rcx+78h],rax
  0000000140EB350F: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140EB3517: EB 0B              jmp         0000000140EB3524
  0000000140EB3519: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140EB351D: 83 C0 01           add         eax,1
  0000000140EB3520: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140EB3524: 83 7C 24 30 06     cmp         dword ptr [rsp+30h],6
  0000000140EB3529: 7D 60              jge         0000000140EB358B
  0000000140EB352B: B9 78 00 00 00     mov         ecx,78h
  0000000140EB3530: E8 87 B5 5B 00     call        000000014146EABC
  0000000140EB3535: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140EB353A: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140EB3540: 74 14              je          0000000140EB3556
  0000000140EB3542: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140EB3547: E8 74 58 9E FF     call        0000000140898DC0
  0000000140EB354C: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000140EB3554: EB 0C              jmp         0000000140EB3562
  0000000140EB3556: 48 C7 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],0
                    00 00 00 00 00 00
  0000000140EB3562: 48 8B 84 24 D8 00  mov         rax,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140EB356A: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140EB356F: 48 63 54 24 30     movsxd      rdx,dword ptr [rsp+30h]
  0000000140EB3574: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB357C: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140EB3581: 48 89 84 D1 C8 00  mov         qword ptr [rcx+rdx*8+00000000000000C8h],rax
                    00 00
  0000000140EB3589: EB 8E              jmp         0000000140EB3519
  0000000140EB358B: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3593: 48 C7 80 F8 00 00  mov         qword ptr [rax+00000000000000F8h],0
                    00 00 00 00 00
  0000000140EB359E: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB35A6: C7 80 78 01 00 00  mov         dword ptr [rax+0000000000000178h],0
                    00 00 00 00
  0000000140EB35B0: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB35B8: C7 80 00 01 00 00  mov         dword ptr [rax+0000000000000100h],0FFFFFFFFh
                    FF FF FF FF
  0000000140EB35C2: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB35CA: C7 80 1C 01 00 00  mov         dword ptr [rax+000000000000011Ch],0FFFFFFFFh
                    FF FF FF FF
  0000000140EB35D4: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB35DC: E8 6F 49 00 00     call        0000000140EB7F50
  0000000140EB35E1: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB35E9: 48 8B 89 40 01 00  mov         rcx,qword ptr [rcx+0000000000000140h]
                    00
  0000000140EB35F0: BA 08 00 00 00     mov         edx,8
  0000000140EB35F5: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  0000000140EB35F9: FF 15 D1 2E F5 00  call        qword ptr [0000000141E064D0h]
  0000000140EB35FF: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140EB3604: 48 8B C8           mov         rcx,rax
  0000000140EB3607: FF 15 03 6F F5 00  call        qword ptr [0000000141E0A510h]
  0000000140EB360D: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140EB3615: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140EB361D: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000140EB3625: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  0000000140EB362D: 45 33 C9           xor         r9d,r9d
  0000000140EB3630: 45 33 C0           xor         r8d,r8d
  0000000140EB3633: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000140EB3638: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140EB3640: FF 15 C2 6E F5 00  call        qword ptr [0000000141E0A508h]
  0000000140EB3646: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140EB364E: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140EB3656: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  0000000140EB365E: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3666: 48 81 C1 20 01 00  add         rcx,120h
                    00
  0000000140EB366D: 48 8B 94 24 F8 00  mov         rdx,qword ptr [rsp+00000000000000F8h]
                    00 00
  0000000140EB3675: FF 15 E5 53 F5 00  call        qword ptr [0000000141E08A60h]
  0000000140EB367B: 90                 nop
  0000000140EB367C: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140EB3681: FF 15 51 89 F5 00  call        qword ptr [0000000141E0BFD8h]
  0000000140EB3687: 90                 nop
  0000000140EB3688: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140EB368D: FF 15 0D 80 F5 00  call        qword ptr [0000000141E0B6A0h]
  0000000140EB3693: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB369B: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB36A2: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB36AA: 48 8B 80 68 12 00  mov         rax,qword ptr [rax+0000000000001268h]
                    00
  0000000140EB36B1: 48 89 81 80 00 00  mov         qword ptr [rcx+0000000000000080h],rax
                    00
  0000000140EB36B8: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB36C0: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB36C7: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB36CF: 48 8B 80 70 12 00  mov         rax,qword ptr [rax+0000000000001270h]
                    00
  0000000140EB36D6: 48 89 81 88 00 00  mov         qword ptr [rcx+0000000000000088h],rax
                    00
  0000000140EB36DD: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB36E5: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB36EC: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB36F4: 48 8B 80 78 12 00  mov         rax,qword ptr [rax+0000000000001278h]
                    00
  0000000140EB36FB: 48 89 81 90 00 00  mov         qword ptr [rcx+0000000000000090h],rax
                    00
  0000000140EB3702: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB370A: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB3711: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3719: 48 8B 80 80 12 00  mov         rax,qword ptr [rax+0000000000001280h]
                    00
  0000000140EB3720: 48 89 81 98 00 00  mov         qword ptr [rcx+0000000000000098h],rax
                    00
  0000000140EB3727: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB372F: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB3736: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB373E: 48 8B 80 98 12 00  mov         rax,qword ptr [rax+0000000000001298h]
                    00
  0000000140EB3745: 48 89 81 A0 00 00  mov         qword ptr [rcx+00000000000000A0h],rax
                    00
  0000000140EB374C: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3754: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB375B: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3763: 48 8B 80 88 12 00  mov         rax,qword ptr [rax+0000000000001288h]
                    00
  0000000140EB376A: 48 89 81 A8 00 00  mov         qword ptr [rcx+00000000000000A8h],rax
                    00
  0000000140EB3771: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3779: 48 8B 80 40 01 00  mov         rax,qword ptr [rax+0000000000000140h]
                    00
  0000000140EB3780: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3788: 48 8B 80 90 12 00  mov         rax,qword ptr [rax+0000000000001290h]
                    00
  0000000140EB378F: 48 89 81 B0 00 00  mov         qword ptr [rcx+00000000000000B0h],rax
                    00
  0000000140EB3796: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140EB379E: EB 0B              jmp         0000000140EB37AB
  0000000140EB37A0: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140EB37A4: 83 C0 01           add         eax,1
  0000000140EB37A7: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140EB37AB: 83 7C 24 30 06     cmp         dword ptr [rsp+30h],6
  0000000140EB37B0: 7D 72              jge         0000000140EB3824
  0000000140EB37B2: B9 78 00 00 00     mov         ecx,78h
  0000000140EB37B7: E8 00 B3 5B 00     call        000000014146EABC
  0000000140EB37BC: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140EB37C4: 48 83 BC 24 98 00  cmp         qword ptr [rsp+0000000000000098h],0
                    00 00 00
  0000000140EB37CD: 74 17              je          0000000140EB37E6
  0000000140EB37CF: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140EB37D7: E8 E4 55 9E FF     call        0000000140898DC0
  0000000140EB37DC: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  0000000140EB37E4: EB 0C              jmp         0000000140EB37F2
  0000000140EB37E6: 48 C7 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],0
                    00 00 00 00 00 00
  0000000140EB37F2: 48 8B 84 24 00 01  mov         rax,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140EB37FA: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140EB3802: 48 63 54 24 30     movsxd      rdx,dword ptr [rsp+30h]
  0000000140EB3807: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB380F: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140EB3817: 48 89 84 D1 C8 00  mov         qword ptr [rcx+rdx*8+00000000000000C8h],rax
                    00 00
  0000000140EB381F: E9 7C FF FF FF     jmp         0000000140EB37A0
  0000000140EB3824: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB382C: 48 C7 80 C0 00 00  mov         qword ptr [rax+00000000000000C0h],0
                    00 00 00 00 00
  0000000140EB3837: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  0000000140EB383F: C7 44 24 34 00 00  mov         dword ptr [rsp+34h],0
                    00 00
  0000000140EB3847: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB384F: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140EB3853: 89 81 B8 00 00 00  mov         dword ptr [rcx+00000000000000B8h],eax
  0000000140EB3859: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3861: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  0000000140EB3865: 89 81 BC 00 00 00  mov         dword ptr [rcx+00000000000000BCh],eax
  0000000140EB386B: 44 8B 44 24 34     mov         r8d,dword ptr [rsp+34h]
  0000000140EB3870: 8B 54 24 38        mov         edx,dword ptr [rsp+38h]
  0000000140EB3874: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB387C: E8 AF 1D 00 00     call        0000000140EB5630
  0000000140EB3881: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3889: E8 02 58 00 00     call        0000000140EB9090
  0000000140EB388E: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3896: C6 82 05 01 00 00  mov         byte ptr [rdx+0000000000000105h],0
                    00
  0000000140EB389D: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB38A5: E8 C6 1A 00 00     call        0000000140EB5370
  0000000140EB38AA: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB38B2: FF 15 98 80 F5 00  call        qword ptr [0000000141E0B950h]
  0000000140EB38B8: 48 85 C0           test        rax,rax
  0000000140EB38BB: 74 45              je          0000000140EB3902
  0000000140EB38BD: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB38C5: FF 15 85 80 F5 00  call        qword ptr [0000000141E0B950h]
  0000000140EB38CB: 48 8B C8           mov         rcx,rax
  0000000140EB38CE: FF 15 3C 57 F5 00  call        qword ptr [0000000141E09010h]
  0000000140EB38D4: FF 15 76 0B F5 00  call        qword ptr [0000000141E04450h]
  0000000140EB38DA: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  0000000140EB38E2: 48 89 94 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rdx
                    00 00
  0000000140EB38EA: 33 D2              xor         edx,edx
  0000000140EB38EC: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140EB38F4: E8 67 11 15 FF     call        0000000140004A60
  0000000140EB38F9: 48 8B C8           mov         rcx,rax
  0000000140EB38FC: FF 15 46 0B F5 00  call        qword ptr [0000000141E04448h]
  0000000140EB3902: B9 20 00 00 00     mov         ecx,20h
  0000000140EB3907: E8 B0 B1 5B 00     call        000000014146EABC
  0000000140EB390C: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140EB3914: 48 83 BC 24 B8 00  cmp         qword ptr [rsp+00000000000000B8h],0
                    00 00 00
  0000000140EB391D: 74 34              je          0000000140EB3953
  0000000140EB391F: 33 D2              xor         edx,edx
  0000000140EB3921: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140EB3929: FF 15 D1 07 F5 00  call        qword ptr [0000000141E04100h]
  0000000140EB392F: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140EB3937: 48 8D 05 3A 82 24  lea         rax,[00000001420FBB78h]
                    01
  0000000140EB393E: 48 89 02           mov         qword ptr [rdx],rax
  0000000140EB3941: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140EB3949: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  0000000140EB3951: EB 0C              jmp         0000000140EB395F
  0000000140EB3953: 48 C7 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],0
                    00 00 00 00 00 00
  0000000140EB395F: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140EB3967: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140EB396F: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3977: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140EB397F: 48 89 81 70 01 00  mov         qword ptr [rcx+0000000000000170h],rax
                    00
  0000000140EB3986: B2 01              mov         dl,1
  0000000140EB3988: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3990: 48 8B 89 70 01 00  mov         rcx,qword ptr [rcx+0000000000000170h]
                    00
  0000000140EB3997: FF 15 DB 11 F5 00  call        qword ptr [0000000141E04B78h]
  0000000140EB399D: 33 D2              xor         edx,edx
  0000000140EB399F: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB39A7: 48 8B 89 70 01 00  mov         rcx,qword ptr [rcx+0000000000000170h]
                    00
  0000000140EB39AE: FF 15 A4 13 F5 00  call        qword ptr [0000000141E04D58h]
  0000000140EB39B4: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140EB39BC: 4C 8D 0D 85 09 C6  lea         r9,[0000000142B14348h]
                    01
  0000000140EB39C3: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB39CB: 48 8D 15 8E 09 C6  lea         rdx,[0000000142B14360h]
                    01
  0000000140EB39D2: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB39DA: 48 8B 89 70 01 00  mov         rcx,qword ptr [rcx+0000000000000170h]
                    00
  0000000140EB39E1: FF 15 E9 07 F5 00  call        qword ptr [0000000141E041D0h]
  0000000140EB39E7: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB39EF: E8 FC 16 00 00     call        0000000140EB50F0
  0000000140EB39F4: 48 8B 94 24 20 01  mov         rdx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB39FC: 48 C7 82 48 01 00  mov         qword ptr [rdx+0000000000000148h],0
                    00 00 00 00 00
  0000000140EB3A07: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3A0F: E8 FC B9 01 00     call        0000000140ECF410
  0000000140EB3A14: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140EB3A19: 4C 8D 0D 50 09 C6  lea         r9,[0000000142B14370h]
                    01
  0000000140EB3A20: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3A28: 48 8D 15 61 09 C6  lea         rdx,[0000000142B14390h]
                    01
  0000000140EB3A2F: 48 8D 0D CA 2C EC  lea         rcx,[0000000143D76700h]
                    02
  0000000140EB3A36: E8 A5 C1 41 FF     call        00000001402CFBE0
  0000000140EB3A3B: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3A43: E8 C8 D5 01 00     call        0000000140ED1010
  0000000140EB3A48: 90                 nop
  0000000140EB3A49: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140EB3A51: 48 81 C4 10 01 00  add         rsp,110h
                    00
  0000000140EB3A58: 5F                 pop         rdi
  0000000140EB3A59: C3                 ret

  Summary

     1E02000 .text

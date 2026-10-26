Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014003C730: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014003C735: 53                 push        rbx
  000000014003C736: 48 81 EC E0 01 00  sub         rsp,1E0h
                    00
  000000014003C73D: 48 C7 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000014003C749: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014003C751: FF 15 19 7A DC 01  call        qword ptr [0000000141E04170h]
  000000014003C757: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014003C75C: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014003C764: 4C 8D 0D DD CB 52  lea         r9,[0000000143569348h]
                    03
  000000014003C76B: 4C 8D 05 D6 8F 52  lea         r8,[0000000143565748h]
                    03
  000000014003C772: 33 D2              xor         edx,edx
  000000014003C774: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014003C779: E8 7E 31 43 01     call        000000014146F8FC
  000000014003C77E: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014003C783: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  000000014003C789: 75 05              jne         000000014003C790
  000000014003C78B: E9 13 05 00 00     jmp         000000014003CCA3
  000000014003C790: C6 44 24 50 01     mov         byte ptr [rsp+50h],1
  000000014003C795: 0F B6 54 24 50     movzx       edx,byte ptr [rsp+50h]
  000000014003C79A: 48 8D 0D 17 9F D3  lea         rcx,[0000000143D766B8h]
                    03
  000000014003C7A1: E8 7A E1 28 00     call        00000001402CA920
  000000014003C7A6: 0F B6 C0           movzx       eax,al
  000000014003C7A9: 85 C0              test        eax,eax
  000000014003C7AB: 74 05              je          000000014003C7B2
  000000014003C7AD: E9 F1 04 00 00     jmp         000000014003CCA3
  000000014003C7B2: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014003C7BA: E8 E1 D5 FE FF     call        0000000140029DA0
  000000014003C7BF: 0F B6 C0           movzx       eax,al
  000000014003C7C2: 85 C0              test        eax,eax
  000000014003C7C4: 75 05              jne         000000014003C7CB
  000000014003C7C6: E9 D8 04 00 00     jmp         000000014003CCA3
  000000014003C7CB: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000014003C7D0: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014003C7D5: FF 15 FD EF DC 01  call        qword ptr [0000000141E0B7D8h]
  000000014003C7DB: 90                 nop
  000000014003C7DC: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014003C7E1: FF 15 71 7A DC 01  call        qword ptr [0000000141E04258h]
  000000014003C7E7: 0F B6 C0           movzx       eax,al
  000000014003C7EA: 85 C0              test        eax,eax
  000000014003C7EC: 0F 85 2A 04 00 00  jne         000000014003CC1C
  000000014003C7F2: 48 8D 44 24 58     lea         rax,[rsp+58h]
  000000014003C7F7: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014003C7FC: 41 B8 00 00 01 00  mov         r8d,10000h
  000000014003C802: BA 00 40 00 00     mov         edx,4000h
  000000014003C807: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014003C80C: E8 6F 4E FF FF     call        0000000140031680
  000000014003C811: 48 8B D8           mov         rbx,rax
  000000014003C814: 45 33 C0           xor         r8d,r8d
  000000014003C817: 48 8D 15 82 91 0C  lea         rdx,[00000001421059A0h]
                    02
  000000014003C81E: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000014003C823: E8 58 85 FE FF     call        0000000140024D80
  000000014003C828: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  000000014003C830: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000014003C838: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  000000014003C840: 48 8D 15 DD 52 0C  lea         rdx,[0000000142101B24h]
                    02
  000000014003C847: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000014003C84C: FF 15 2E 7C DC 01  call        qword ptr [0000000141E04480h]
  000000014003C852: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  000000014003C85A: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  000000014003C862: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  000000014003C86A: 48 8D 15 E3 52 0C  lea         rdx,[0000000142101B54h]
                    02
  000000014003C871: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000014003C876: FF 15 04 7C DC 01  call        qword ptr [0000000141E04480h]
  000000014003C87C: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  000000014003C884: 48 8B 84 24 08 01  mov         rax,qword ptr [rsp+0000000000000108h]
                    00 00
  000000014003C88C: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  000000014003C894: 45 33 C0           xor         r8d,r8d
  000000014003C897: 48 8D 15 3A 91 0C  lea         rdx,[00000001421059D8h]
                    02
  000000014003C89E: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014003C8A6: E8 D5 84 FE FF     call        0000000140024D80
  000000014003C8AB: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  000000014003C8B3: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  000000014003C8BB: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  000000014003C8C3: 48 8D 15 EA 53 0C  lea         rdx,[0000000142101CB4h]
                    02
  000000014003C8CA: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000014003C8D2: FF 15 A8 7B DC 01  call        qword ptr [0000000141E04480h]
  000000014003C8D8: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  000000014003C8E0: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  000000014003C8E8: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  000000014003C8F0: 48 8D 15 D5 55 0C  lea         rdx,[0000000142101ECCh]
                    02
  000000014003C8F7: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000014003C8FF: FF 15 7B 7B DC 01  call        qword ptr [0000000141E04480h]
  000000014003C905: 48 89 84 24 38 01  mov         qword ptr [rsp+0000000000000138h],rax
                    00 00
  000000014003C90D: 48 8B 84 24 38 01  mov         rax,qword ptr [rsp+0000000000000138h]
                    00 00
  000000014003C915: 48 89 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],rax
                    00 00
  000000014003C91D: 45 33 C0           xor         r8d,r8d
  000000014003C920: 48 8D 15 D9 90 0C  lea         rdx,[0000000142105A00h]
                    02
  000000014003C927: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014003C92F: E8 4C 84 FE FF     call        0000000140024D80
  000000014003C934: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  000000014003C93C: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  000000014003C944: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  000000014003C94C: 4C 8B 84 24 40 01  mov         r8,qword ptr [rsp+0000000000000140h]
                    00 00
  000000014003C954: 48 8B 94 24 50 01  mov         rdx,qword ptr [rsp+0000000000000150h]
                    00 00
  000000014003C95C: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000014003C964: E8 87 7F FC FF     call        00000001400048F0
  000000014003C969: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  000000014003C971: 48 8B 84 24 58 01  mov         rax,qword ptr [rsp+0000000000000158h]
                    00 00
  000000014003C979: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  000000014003C981: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  000000014003C986: 48 8B 94 24 60 01  mov         rdx,qword ptr [rsp+0000000000000160h]
                    00 00
  000000014003C98E: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014003C996: E8 55 7F FC FF     call        00000001400048F0
  000000014003C99B: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  000000014003C9A3: 48 8B 84 24 68 01  mov         rax,qword ptr [rsp+0000000000000168h]
                    00 00
  000000014003C9AB: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  000000014003C9B3: 4C 8B 84 24 30 01  mov         r8,qword ptr [rsp+0000000000000130h]
                    00 00
  000000014003C9BB: 48 8B 94 24 70 01  mov         rdx,qword ptr [rsp+0000000000000170h]
                    00 00
  000000014003C9C3: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000014003C9CB: E8 20 7F FC FF     call        00000001400048F0
  000000014003C9D0: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  000000014003C9D8: 48 8B 84 24 78 01  mov         rax,qword ptr [rsp+0000000000000178h]
                    00 00
  000000014003C9E0: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  000000014003C9E8: 4C 8B 84 24 20 01  mov         r8,qword ptr [rsp+0000000000000120h]
                    00 00
  000000014003C9F0: 48 8B 94 24 80 01  mov         rdx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000014003C9F8: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014003CA00: E8 EB 7E FC FF     call        00000001400048F0
  000000014003CA05: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  000000014003CA0D: 48 8B 84 24 88 01  mov         rax,qword ptr [rsp+0000000000000188h]
                    00 00
  000000014003CA15: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  000000014003CA1D: 4C 8B 84 24 10 01  mov         r8,qword ptr [rsp+0000000000000110h]
                    00 00
  000000014003CA25: 48 8B 94 24 90 01  mov         rdx,qword ptr [rsp+0000000000000190h]
                    00 00
  000000014003CA2D: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000014003CA35: E8 B6 7E FC FF     call        00000001400048F0
  000000014003CA3A: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  000000014003CA42: 48 8B 84 24 98 01  mov         rax,qword ptr [rsp+0000000000000198h]
                    00 00
  000000014003CA4A: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  000000014003CA52: 4C 8B 84 24 00 01  mov         r8,qword ptr [rsp+0000000000000100h]
                    00 00
  000000014003CA5A: 48 8B 94 24 A0 01  mov         rdx,qword ptr [rsp+00000000000001A0h]
                    00 00
  000000014003CA62: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000014003CA6A: E8 81 7E FC FF     call        00000001400048F0
  000000014003CA6F: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  000000014003CA77: 48 8B 84 24 A8 01  mov         rax,qword ptr [rsp+00000000000001A8h]
                    00 00
  000000014003CA7F: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  000000014003CA87: 4C 8B 84 24 F0 00  mov         r8,qword ptr [rsp+00000000000000F0h]
                    00 00
  000000014003CA8F: 48 8B 94 24 B0 01  mov         rdx,qword ptr [rsp+00000000000001B0h]
                    00 00
  000000014003CA97: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  000000014003CA9F: E8 4C 7E FC FF     call        00000001400048F0
  000000014003CAA4: 48 89 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],rax
                    00 00
  000000014003CAAC: 48 8B 84 24 B8 01  mov         rax,qword ptr [rsp+00000000000001B8h]
                    00 00
  000000014003CAB4: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  000000014003CABC: 45 33 C0           xor         r8d,r8d
  000000014003CABF: 48 8D 15 5A 8F 0C  lea         rdx,[0000000142105A20h]
                    02
  000000014003CAC6: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  000000014003CACE: E8 AD 82 FE FF     call        0000000140024D80
  000000014003CAD3: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  000000014003CADB: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  000000014003CAE3: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
  000000014003CAEB: C7 44 24 20 00 40  mov         dword ptr [rsp+20h],4000h
                    00 00
  000000014003CAF3: 4C 8B CB           mov         r9,rbx
  000000014003CAF6: 4C 8B 84 24 C0 01  mov         r8,qword ptr [rsp+00000000000001C0h]
                    00 00
  000000014003CAFE: 48 8B 94 24 D0 01  mov         rdx,qword ptr [rsp+00000000000001D0h]
                    00 00
  000000014003CB06: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014003CB0E: FF 15 A4 EE DC 01  call        qword ptr [0000000141E0B9B8h]
  000000014003CB14: 89 44 24 54        mov         dword ptr [rsp+54h],eax
  000000014003CB18: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  000000014003CB20: FF 15 BA 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CB26: 90                 nop
  000000014003CB27: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  000000014003CB2F: FF 15 AB 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CB35: 90                 nop
  000000014003CB36: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000014003CB3E: FF 15 9C 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CB44: 90                 nop
  000000014003CB45: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000014003CB4D: FF 15 8D 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CB53: 90                 nop
  000000014003CB54: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000014003CB5C: FF 15 7E 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CB62: 90                 nop
  000000014003CB63: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000014003CB6B: FF 15 6F 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CB71: 90                 nop
  000000014003CB72: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  000000014003CB7A: FF 15 60 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CB80: 90                 nop
  000000014003CB81: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  000000014003CB89: FF 15 51 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CB8F: 90                 nop
  000000014003CB90: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  000000014003CB98: FF 15 42 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CB9E: 90                 nop
  000000014003CB9F: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000014003CBA7: FF 15 33 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CBAD: 90                 nop
  000000014003CBAE: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000014003CBB6: FF 15 24 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CBBC: 90                 nop
  000000014003CBBD: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  000000014003CBC5: FF 15 15 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CBCB: 90                 nop
  000000014003CBCC: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  000000014003CBD1: FF 15 09 8E DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CBD7: 90                 nop
  000000014003CBD8: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000014003CBDD: FF 15 FD 8D DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CBE3: 90                 nop
  000000014003CBE4: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000014003CBE9: FF 15 F1 8D DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CBEF: 81 7C 24 54 00 40  cmp         dword ptr [rsp+54h],4000h
                    00 00
  000000014003CBF7: 75 13              jne         000000014003CC0C
  000000014003CBF9: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000014003CBFE: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014003CC06: E8 35 10 FE FF     call        000000014001DC40
  000000014003CC0B: 90                 nop
  000000014003CC0C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014003CC11: FF 15 C9 8D DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CC17: E9 87 00 00 00     jmp         000000014003CCA3
  000000014003CC1C: B2 01              mov         dl,1
  000000014003CC1E: 48 8D 0D 93 9A D3  lea         rcx,[0000000143D766B8h]
                    03
  000000014003CC25: E8 C6 DD 28 00     call        00000001402CA9F0
  000000014003CC2A: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000014003CC2F: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014003CC37: E8 F4 FD FD FF     call        000000014001CA30
  000000014003CC3C: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014003CC41: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  000000014003CC47: 75 1C              jne         000000014003CC65
  000000014003CC49: 33 D2              xor         edx,edx
  000000014003CC4B: 48 8D 0D 66 9A D3  lea         rcx,[0000000143D766B8h]
                    03
  000000014003CC52: E8 99 DD 28 00     call        00000001402CA9F0
  000000014003CC57: 90                 nop
  000000014003CC58: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014003CC5D: FF 15 7D 8D DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CC63: EB 3E              jmp         000000014003CCA3
  000000014003CC65: 48 8B 54 24 40     mov         rdx,qword ptr [rsp+40h]
  000000014003CC6A: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014003CC72: E8 69 D4 FE FF     call        000000014002A0E0
  000000014003CC77: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000014003CC7C: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014003CC84: E8 77 0F FE FF     call        000000014001DC00
  000000014003CC89: 33 D2              xor         edx,edx
  000000014003CC8B: 48 8D 0D 26 9A D3  lea         rcx,[0000000143D766B8h]
                    03
  000000014003CC92: E8 59 DD 28 00     call        00000001402CA9F0
  000000014003CC97: 90                 nop
  000000014003CC98: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000014003CC9D: FF 15 3D 8D DC 01  call        qword ptr [0000000141E059E0h]
  000000014003CCA3: 48 81 C4 E0 01 00  add         rsp,1E0h
                    00
  000000014003CCAA: 5B                 pop         rbx
  000000014003CCAB: C3                 ret

  Summary

     1E02000 .text

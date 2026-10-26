Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411B73C0: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001411B73C5: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  00000001411B73C9: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411B73CE: 53                 push        rbx
  00000001411B73CF: 57                 push        rdi
  00000001411B73D0: 48 81 EC 48 03 00  sub         rsp,348h
                    00
  00000001411B73D7: 48 C7 84 24 C0 02  mov         qword ptr [rsp+00000000000002C0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001411B73E3: 48 8B 05 46 6C 9A  mov         rax,qword ptr [0000000143B5E030h]
                    02
  00000001411B73EA: 48 33 C4           xor         rax,rsp
  00000001411B73ED: 48 89 84 24 38 03  mov         qword ptr [rsp+0000000000000338h],rax
                    00 00
  00000001411B73F5: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B73FD: E8 5E C3 FF FF     call        00000001411B3760
  00000001411B7402: 48 89 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],rax
                    00 00
  00000001411B740A: 48 C7 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],0
                    00 00 00 00 00 00
  00000001411B7416: 48 83 BC 24 30 01  cmp         qword ptr [rsp+0000000000000130h],0
                    00 00 00
  00000001411B741F: 74 15              je          00000001411B7436
  00000001411B7421: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B7429: E8 A2 38 AC FF     call        0000000140C7ACD0
  00000001411B742E: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  00000001411B7436: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B743E: E8 7D C3 FF FF     call        00000001411B37C0
  00000001411B7443: 88 84 24 40 01 00  mov         byte ptr [rsp+0000000000000140h],al
                    00
  00000001411B744A: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B7452: E8 E9 30 00 00     call        00000001411BA540
  00000001411B7457: C7 84 24 38 01 00  mov         dword ptr [rsp+0000000000000138h],0FFFFFFFFh
                    00 FF FF FF FF
  00000001411B7462: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001411B746A: FF 15 50 D0 C4 00  call        qword ptr [0000000141E044C0h]
  00000001411B7470: 90                 nop
  00000001411B7471: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411B7475: F2 0F 11 84 24 88  movsd       mmword ptr [rsp+0000000000000188h],xmm0
                    01 00 00
  00000001411B747E: 48 83 BC 24 30 01  cmp         qword ptr [rsp+0000000000000130h],0
                    00 00 00
  00000001411B7487: 74 20              je          00000001411B74A9
  00000001411B7489: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B7491: E8 3A 3E AC FF     call        0000000140C7B2D0
  00000001411B7496: F2 0F 10 80 40 06  movsd       xmm0,mmword ptr [rax+0000000000000640h]
                    00 00
  00000001411B749E: F2 0F 11 84 24 C8  movsd       mmword ptr [rsp+00000000000002C8h],xmm0
                    02 00 00
  00000001411B74A7: EB 0D              jmp         00000001411B74B6
  00000001411B74A9: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411B74AD: F2 0F 11 84 24 C8  movsd       mmword ptr [rsp+00000000000002C8h],xmm0
                    02 00 00
  00000001411B74B6: F2 0F 10 84 24 C8  movsd       xmm0,mmword ptr [rsp+00000000000002C8h]
                    02 00 00
  00000001411B74BF: F2 0F 11 84 24 78  movsd       mmword ptr [rsp+0000000000000178h],xmm0
                    01 00 00
  00000001411B74C8: 48 83 BC 24 30 01  cmp         qword ptr [rsp+0000000000000130h],0
                    00 00 00
  00000001411B74D1: 74 20              je          00000001411B74F3
  00000001411B74D3: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B74DB: E8 F0 3D AC FF     call        0000000140C7B2D0
  00000001411B74E0: F2 0F 10 80 48 06  movsd       xmm0,mmword ptr [rax+0000000000000648h]
                    00 00
  00000001411B74E8: F2 0F 11 84 24 D0  movsd       mmword ptr [rsp+00000000000002D0h],xmm0
                    02 00 00
  00000001411B74F1: EB 0D              jmp         00000001411B7500
  00000001411B74F3: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411B74F7: F2 0F 11 84 24 D0  movsd       mmword ptr [rsp+00000000000002D0h],xmm0
                    02 00 00
  00000001411B7500: F2 0F 10 84 24 D0  movsd       xmm0,mmword ptr [rsp+00000000000002D0h]
                    02 00 00
  00000001411B7509: F2 0F 11 84 24 68  movsd       mmword ptr [rsp+0000000000000168h],xmm0
                    01 00 00
  00000001411B7512: C7 84 24 70 01 00  mov         dword ptr [rsp+0000000000000170h],0FFFFFFFFh
                    00 FF FF FF FF
  00000001411B751D: C7 84 24 3C 01 00  mov         dword ptr [rsp+000000000000013Ch],0
                    00 00 00 00 00
  00000001411B7528: EB 11              jmp         00000001411B753B
  00000001411B752A: 8B 84 24 3C 01 00  mov         eax,dword ptr [rsp+000000000000013Ch]
                    00
  00000001411B7531: 83 C0 01           add         eax,1
  00000001411B7534: 89 84 24 3C 01 00  mov         dword ptr [rsp+000000000000013Ch],eax
                    00
  00000001411B753B: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B7543: E8 28 95 FF FF     call        00000001411B0A70
  00000001411B7548: 39 84 24 3C 01 00  cmp         dword ptr [rsp+000000000000013Ch],eax
                    00
  00000001411B754F: 0F 8D E1 1A 00 00  jge         00000001411B9036
  00000001411B7555: 0F B6 84 24 68 03  movzx       eax,byte ptr [rsp+0000000000000368h]
                    00 00
  00000001411B755D: 85 C0              test        eax,eax
  00000001411B755F: 74 57              je          00000001411B75B8
  00000001411B7561: 8B 94 24 70 03 00  mov         edx,dword ptr [rsp+0000000000000370h]
                    00
  00000001411B7568: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B7570: E8 7B 95 FF FF     call        00000001411B0AF0
  00000001411B7575: 0F B6 C0           movzx       eax,al
  00000001411B7578: 85 C0              test        eax,eax
  00000001411B757A: 74 10              je          00000001411B758C
  00000001411B757C: 8B 84 24 70 03 00  mov         eax,dword ptr [rsp+0000000000000370h]
                    00
  00000001411B7583: 89 84 24 70 01 00  mov         dword ptr [rsp+0000000000000170h],eax
                    00
  00000001411B758A: EB 17              jmp         00000001411B75A3
  00000001411B758C: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B7594: E8 A7 D6 35 FF     call        0000000140514C40
  00000001411B7599: 83 E8 01           sub         eax,1
  00000001411B759C: 89 84 24 70 01 00  mov         dword ptr [rsp+0000000000000170h],eax
                    00
  00000001411B75A3: 8B 84 24 3C 01 00  mov         eax,dword ptr [rsp+000000000000013Ch]
                    00
  00000001411B75AA: 39 84 24 70 01 00  cmp         dword ptr [rsp+0000000000000170h],eax
                    00
  00000001411B75B1: 74 05              je          00000001411B75B8
  00000001411B75B3: E9 72 FF FF FF     jmp         00000001411B752A
  00000001411B75B8: 8B 94 24 3C 01 00  mov         edx,dword ptr [rsp+000000000000013Ch]
                    00
  00000001411B75BF: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B75C7: E8 C4 94 FF FF     call        00000001411B0A90
  00000001411B75CC: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  00000001411B75D4: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B75DC: E8 AF 50 B0 FF     call        0000000140CBC690
  00000001411B75E1: 83 E8 01           sub         eax,1
  00000001411B75E4: 89 84 24 D0 01 00  mov         dword ptr [rsp+00000000000001D0h],eax
                    00
  00000001411B75EB: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B75F3: E8 D8 BA 24 00     call        00000001414030D0
  00000001411B75F8: F2 0F 11 84 24 98  movsd       mmword ptr [rsp+0000000000000198h],xmm0
                    01 00 00
  00000001411B7601: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7609: E8 12 63 A9 FF     call        0000000140C4D920
  00000001411B760E: F2 0F 11 84 24 B0  movsd       mmword ptr [rsp+00000000000001B0h],xmm0
                    01 00 00
  00000001411B7617: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B761F: E8 CC B1 00 00     call        00000001411C27F0
  00000001411B7624: F2 0F 11 84 24 C8  movsd       mmword ptr [rsp+00000000000001C8h],xmm0
                    01 00 00
  00000001411B762D: F2 0F 10 84 24 88  movsd       xmm0,mmword ptr [rsp+0000000000000188h]
                    01 00 00
  00000001411B7636: F2 0F 58 84 24 98  addsd       xmm0,mmword ptr [rsp+0000000000000198h]
                    01 00 00
  00000001411B763F: F2 0F 11 84 24 88  movsd       mmword ptr [rsp+0000000000000188h],xmm0
                    01 00 00
  00000001411B7648: F2 0F 10 84 24 78  movsd       xmm0,mmword ptr [rsp+0000000000000178h]
                    01 00 00
  00000001411B7651: F2 0F 58 84 24 98  addsd       xmm0,mmword ptr [rsp+0000000000000198h]
                    01 00 00
  00000001411B765A: F2 0F 11 84 24 78  movsd       mmword ptr [rsp+0000000000000178h],xmm0
                    01 00 00
  00000001411B7663: 8B 94 24 D0 01 00  mov         edx,dword ptr [rsp+00000000000001D0h]
                    00
  00000001411B766A: 83 C2 01           add         edx,1
  00000001411B766D: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B7675: E8 A6 B2 00 00     call        00000001411C2920
  00000001411B767A: 0F B6 C0           movzx       eax,al
  00000001411B767D: 85 C0              test        eax,eax
  00000001411B767F: 75 1A              jne         00000001411B769B
  00000001411B7681: 33 D2              xor         edx,edx
  00000001411B7683: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B768B: E8 10 A4 1B 00     call        0000000141371AA0
  00000001411B7690: C7 84 24 D0 01 00  mov         dword ptr [rsp+00000000000001D0h],0FFFFFFFFh
                    00 FF FF FF FF
  00000001411B769B: 8B 94 24 D0 01 00  mov         edx,dword ptr [rsp+00000000000001D0h]
                    00
  00000001411B76A2: 83 C2 01           add         edx,1
  00000001411B76A5: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B76AD: E8 AE B1 00 00     call        00000001411C2860
  00000001411B76B2: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  00000001411B76BA: 48 83 BC 24 A8 01  cmp         qword ptr [rsp+00000000000001A8h],0
                    00 00 00
  00000001411B76C3: 75 05              jne         00000001411B76CA
  00000001411B76C5: E9 60 FE FF FF     jmp         00000001411B752A
  00000001411B76CA: 4C 8D 84 24 3C 01  lea         r8,[rsp+000000000000013Ch]
                    00 00
  00000001411B76D2: 48 8D 94 24 88 02  lea         rdx,[rsp+0000000000000288h]
                    00 00
  00000001411B76DA: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B76E2: E8 F9 C1 FF FF     call        00000001411B38E0
  00000001411B76E7: 48 89 84 24 D8 02  mov         qword ptr [rsp+00000000000002D8h],rax
                    00 00
  00000001411B76EF: 48 8B 84 24 D8 02  mov         rax,qword ptr [rsp+00000000000002D8h]
                    00 00
  00000001411B76F7: 48 89 84 24 E0 02  mov         qword ptr [rsp+00000000000002E0h],rax
                    00 00
  00000001411B76FF: 48 8B 94 24 E0 02  mov         rdx,qword ptr [rsp+00000000000002E0h]
                    00 00
  00000001411B7707: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001411B770F: FF 15 5B CD C4 00  call        qword ptr [0000000141E04470h]
  00000001411B7715: 90                 nop
  00000001411B7716: 48 8D 8C 24 88 02  lea         rcx,[rsp+0000000000000288h]
                    00 00
  00000001411B771E: FF 15 BC E2 C4 00  call        qword ptr [0000000141E059E0h]
  00000001411B7724: 48 8D 94 24 90 02  lea         rdx,[rsp+0000000000000290h]
                    00 00
  00000001411B772C: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001411B7734: FF 15 16 C8 C4 00  call        qword ptr [0000000141E03F50h]
  00000001411B773A: 48 89 84 24 E8 02  mov         qword ptr [rsp+00000000000002E8h],rax
                    00 00
  00000001411B7742: 48 8B 84 24 E8 02  mov         rax,qword ptr [rsp+00000000000002E8h]
                    00 00
  00000001411B774A: 48 89 84 24 F0 02  mov         qword ptr [rsp+00000000000002F0h],rax
                    00 00
  00000001411B7752: 48 8B 8C 24 F0 02  mov         rcx,qword ptr [rsp+00000000000002F0h]
                    00 00
  00000001411B775A: FF 15 C8 CA C4 00  call        qword ptr [0000000141E04228h]
  00000001411B7760: 48 8D 15 55 AB AE  lea         rdx,[0000000142CA22BCh]
                    01
  00000001411B7767: 48 8B C8           mov         rcx,rax
  00000001411B776A: E8 69 85 2B 00     call        000000014146FCD8
  00000001411B776F: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  00000001411B7777: 48 8D 8C 24 90 02  lea         rcx,[rsp+0000000000000290h]
                    00 00
  00000001411B777F: FF 15 0B CB C4 00  call        qword ptr [0000000141E04290h]
  00000001411B7785: 48 83 BC 24 60 01  cmp         qword ptr [rsp+0000000000000160h],0
                    00 00 00
  00000001411B778E: 75 05              jne         00000001411B7795
  00000001411B7790: E9 95 FD FF FF     jmp         00000001411B752A
  00000001411B7795: 48 8D 94 24 98 02  lea         rdx,[rsp+0000000000000298h]
                    00 00
  00000001411B779D: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B77A5: E8 16 D0 14 FF     call        00000001403047C0
  00000001411B77AA: 48 89 84 24 F8 02  mov         qword ptr [rsp+00000000000002F8h],rax
                    00 00
  00000001411B77B2: 48 8B 84 24 F8 02  mov         rax,qword ptr [rsp+00000000000002F8h]
                    00 00
  00000001411B77BA: 48 89 84 24 00 03  mov         qword ptr [rsp+0000000000000300h],rax
                    00 00
  00000001411B77C2: 48 8D 94 24 A0 02  lea         rdx,[rsp+00000000000002A0h]
                    00 00
  00000001411B77CA: 48 8B 8C 24 00 03  mov         rcx,qword ptr [rsp+0000000000000300h]
                    00 00
  00000001411B77D2: FF 15 78 C7 C4 00  call        qword ptr [0000000141E03F50h]
  00000001411B77D8: 48 89 84 24 08 03  mov         qword ptr [rsp+0000000000000308h],rax
                    00 00
  00000001411B77E0: 48 8B 84 24 08 03  mov         rax,qword ptr [rsp+0000000000000308h]
                    00 00
  00000001411B77E8: 48 89 84 24 10 03  mov         qword ptr [rsp+0000000000000310h],rax
                    00 00
  00000001411B77F0: 48 8B 8C 24 10 03  mov         rcx,qword ptr [rsp+0000000000000310h]
                    00 00
  00000001411B77F8: FF 15 2A CA C4 00  call        qword ptr [0000000141E04228h]
  00000001411B77FE: 44 8B 84 24 3C 01  mov         r8d,dword ptr [rsp+000000000000013Ch]
                    00 00
  00000001411B7806: 41 83 C0 01        add         r8d,1
  00000001411B780A: 4C 8B C8           mov         r9,rax
  00000001411B780D: 48 8D 15 E4 DE AE  lea         rdx,[0000000142CA56F8h]
                    01
  00000001411B7814: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001411B7819: E8 1A 66 2B 00     call        000000014146DE38
  00000001411B781E: 90                 nop
  00000001411B781F: 48 8D 8C 24 A0 02  lea         rcx,[rsp+00000000000002A0h]
                    00 00
  00000001411B7827: FF 15 63 CA C4 00  call        qword ptr [0000000141E04290h]
  00000001411B782D: 90                 nop
  00000001411B782E: 48 8D 8C 24 98 02  lea         rcx,[rsp+0000000000000298h]
                    00 00
  00000001411B7836: FF 15 A4 E1 C4 00  call        qword ptr [0000000141E059E0h]
  00000001411B783C: B9 19 00 00 00     mov         ecx,19h
  00000001411B7841: E8 EA A6 76 FF     call        0000000140921F30
  00000001411B7846: 4C 8D 4C 24 60     lea         r9,[rsp+60h]
  00000001411B784B: 4C 8B C0           mov         r8,rax
  00000001411B784E: 48 8D 15 B3 DE AE  lea         rdx,[0000000142CA5708h]
                    01
  00000001411B7855: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B785D: E8 AA 61 2B 00     call        000000014146DA0C
  00000001411B7862: 0F B6 84 24 40 01  movzx       eax,byte ptr [rsp+0000000000000140h]
                    00 00
  00000001411B786A: 85 C0              test        eax,eax
  00000001411B786C: 0F 84 D5 00 00 00  je          00000001411B7947
  00000001411B7872: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B787A: E8 A1 AF 00 00     call        00000001411C2820
  00000001411B787F: 0F B6 C0           movzx       eax,al
  00000001411B7882: 85 C0              test        eax,eax
  00000001411B7884: 74 7B              je          00000001411B7901
  00000001411B7886: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B788E: E8 7D 25 8E FF     call        0000000140A99E10
  00000001411B7893: 48 8B C8           mov         rcx,rax
  00000001411B7896: E8 65 3F 6E FF     call        000000014089B800
  00000001411B789B: 0F B6 C0           movzx       eax,al
  00000001411B789E: 85 C0              test        eax,eax
  00000001411B78A0: 74 1C              je          00000001411B78BE
  00000001411B78A2: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B78AA: E8 11 AC 00 00     call        00000001411C24C0
  00000001411B78AF: 48 8B C8           mov         rcx,rax
  00000001411B78B2: E8 49 3F 6E FF     call        000000014089B800
  00000001411B78B7: 0F B6 C0           movzx       eax,al
  00000001411B78BA: 85 C0              test        eax,eax
  00000001411B78BC: 75 43              jne         00000001411B7901
  00000001411B78BE: B9 2A 00 00 00     mov         ecx,2Ah
  00000001411B78C3: E8 68 A6 76 FF     call        0000000140921F30
  00000001411B78C8: F2 0F 10 9C 24 98  movsd       xmm3,mmword ptr [rsp+0000000000000198h]
                    01 00 00
  00000001411B78D1: 66 49 0F 7E D9     movq        r9,xmm3
  00000001411B78D6: 4C 8B C0           mov         r8,rax
  00000001411B78D9: 48 8D 15 38 DE AE  lea         rdx,[0000000142CA5718h]
                    01
  00000001411B78E0: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B78E8: E8 1F 61 2B 00     call        000000014146DA0C
  00000001411B78ED: F2 0F 10 AC 24 78  movsd       xmm5,mmword ptr [rsp+0000000000000178h]
                    01 00 00
  00000001411B78F6: F2 0F 11 AC 24 88  movsd       mmword ptr [rsp+0000000000000188h],xmm5
                    01 00 00
  00000001411B78FF: EB 41              jmp         00000001411B7942
  00000001411B7901: B9 2A 00 00 00     mov         ecx,2Ah
  00000001411B7906: E8 25 A6 76 FF     call        0000000140921F30
  00000001411B790B: F2 0F 10 9C 24 88  movsd       xmm3,mmword ptr [rsp+0000000000000188h]
                    01 00 00
  00000001411B7914: 66 49 0F 7E D9     movq        r9,xmm3
  00000001411B7919: 4C 8B C0           mov         r8,rax
  00000001411B791C: 48 8D 15 05 DE AE  lea         rdx,[0000000142CA5728h]
                    01
  00000001411B7923: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B792B: E8 DC 60 2B 00     call        000000014146DA0C
  00000001411B7930: F2 0F 10 AC 24 88  movsd       xmm5,mmword ptr [rsp+0000000000000188h]
                    01 00 00
  00000001411B7939: F2 0F 11 AC 24 78  movsd       mmword ptr [rsp+0000000000000178h],xmm5
                    01 00 00
  00000001411B7942: E9 D0 00 00 00     jmp         00000001411B7A17
  00000001411B7947: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B794F: E8 CC AE 00 00     call        00000001411C2820
  00000001411B7954: 0F B6 C0           movzx       eax,al
  00000001411B7957: 85 C0              test        eax,eax
  00000001411B7959: 74 7B              je          00000001411B79D6
  00000001411B795B: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7963: E8 A8 24 8E FF     call        0000000140A99E10
  00000001411B7968: 48 8B C8           mov         rcx,rax
  00000001411B796B: E8 90 3E 6E FF     call        000000014089B800
  00000001411B7970: 0F B6 C0           movzx       eax,al
  00000001411B7973: 85 C0              test        eax,eax
  00000001411B7975: 74 1C              je          00000001411B7993
  00000001411B7977: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B797F: E8 3C AB 00 00     call        00000001411C24C0
  00000001411B7984: 48 8B C8           mov         rcx,rax
  00000001411B7987: E8 74 3E 6E FF     call        000000014089B800
  00000001411B798C: 0F B6 C0           movzx       eax,al
  00000001411B798F: 85 C0              test        eax,eax
  00000001411B7991: 75 43              jne         00000001411B79D6
  00000001411B7993: B9 08 00 00 00     mov         ecx,8
  00000001411B7998: E8 93 A5 76 FF     call        0000000140921F30
  00000001411B799D: F2 0F 10 9C 24 98  movsd       xmm3,mmword ptr [rsp+0000000000000198h]
                    01 00 00
  00000001411B79A6: 66 49 0F 7E D9     movq        r9,xmm3
  00000001411B79AB: 4C 8B C0           mov         r8,rax
  00000001411B79AE: 48 8D 15 83 DD AE  lea         rdx,[0000000142CA5738h]
                    01
  00000001411B79B5: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B79BD: E8 4A 60 2B 00     call        000000014146DA0C
  00000001411B79C2: F2 0F 10 AC 24 78  movsd       xmm5,mmword ptr [rsp+0000000000000178h]
                    01 00 00
  00000001411B79CB: F2 0F 11 AC 24 88  movsd       mmword ptr [rsp+0000000000000188h],xmm5
                    01 00 00
  00000001411B79D4: EB 41              jmp         00000001411B7A17
  00000001411B79D6: B9 08 00 00 00     mov         ecx,8
  00000001411B79DB: E8 50 A5 76 FF     call        0000000140921F30
  00000001411B79E0: F2 0F 10 9C 24 98  movsd       xmm3,mmword ptr [rsp+0000000000000198h]
                    01 00 00
  00000001411B79E9: 66 49 0F 7E D9     movq        r9,xmm3
  00000001411B79EE: 4C 8B C0           mov         r8,rax
  00000001411B79F1: 48 8D 15 50 DD AE  lea         rdx,[0000000142CA5748h]
                    01
  00000001411B79F8: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7A00: E8 07 60 2B 00     call        000000014146DA0C
  00000001411B7A05: F2 0F 10 AC 24 88  movsd       xmm5,mmword ptr [rsp+0000000000000188h]
                    01 00 00
  00000001411B7A0E: F2 0F 11 AC 24 78  movsd       mmword ptr [rsp+0000000000000178h],xmm5
                    01 00 00
  00000001411B7A17: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B7A1F: E8 AC 38 AC FF     call        0000000140C7B2D0
  00000001411B7A24: 48 8B D8           mov         rbx,rax
  00000001411B7A27: B9 32 00 00 00     mov         ecx,32h
  00000001411B7A2C: E8 FF A4 76 FF     call        0000000140921F30
  00000001411B7A31: F2 0F 10 9B D0 01  movsd       xmm3,mmword ptr [rbx+00000000000001D0h]
                    00 00
  00000001411B7A39: 66 49 0F 7E D9     movq        r9,xmm3
  00000001411B7A3E: 4C 8B C0           mov         r8,rax
  00000001411B7A41: 48 8D 15 10 DD AE  lea         rdx,[0000000142CA5758h]
                    01
  00000001411B7A48: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7A50: E8 B7 5F 2B 00     call        000000014146DA0C
  00000001411B7A55: 48 8D 94 24 A8 02  lea         rdx,[rsp+00000000000002A8h]
                    00 00
  00000001411B7A5D: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B7A65: E8 56 CD 14 FF     call        00000001403047C0
  00000001411B7A6A: 48 89 84 24 18 03  mov         qword ptr [rsp+0000000000000318h],rax
                    00 00
  00000001411B7A72: 48 8B 84 24 18 03  mov         rax,qword ptr [rsp+0000000000000318h]
                    00 00
  00000001411B7A7A: 48 89 84 24 20 03  mov         qword ptr [rsp+0000000000000320h],rax
                    00 00
  00000001411B7A82: 48 8D 94 24 B0 02  lea         rdx,[rsp+00000000000002B0h]
                    00 00
  00000001411B7A8A: 48 8B 8C 24 20 03  mov         rcx,qword ptr [rsp+0000000000000320h]
                    00 00
  00000001411B7A92: FF 15 B8 C4 C4 00  call        qword ptr [0000000141E03F50h]
  00000001411B7A98: 48 89 84 24 28 03  mov         qword ptr [rsp+0000000000000328h],rax
                    00 00
  00000001411B7AA0: 48 8B 84 24 28 03  mov         rax,qword ptr [rsp+0000000000000328h]
                    00 00
  00000001411B7AA8: 48 89 84 24 30 03  mov         qword ptr [rsp+0000000000000330h],rax
                    00 00
  00000001411B7AB0: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001411B7AB8: FF 15 6A C7 C4 00  call        qword ptr [0000000141E04228h]
  00000001411B7ABE: 4C 8B C0           mov         r8,rax
  00000001411B7AC1: 48 8D 15 A0 DC AE  lea         rdx,[0000000142CA5768h]
                    01
  00000001411B7AC8: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7AD0: E8 37 5F 2B 00     call        000000014146DA0C
  00000001411B7AD5: 90                 nop
  00000001411B7AD6: 48 8D 8C 24 B0 02  lea         rcx,[rsp+00000000000002B0h]
                    00 00
  00000001411B7ADE: FF 15 AC C7 C4 00  call        qword ptr [0000000141E04290h]
  00000001411B7AE4: 90                 nop
  00000001411B7AE5: 48 8D 8C 24 A8 02  lea         rcx,[rsp+00000000000002A8h]
                    00 00
  00000001411B7AED: FF 15 ED DE C4 00  call        qword ptr [0000000141E059E0h]
  00000001411B7AF3: 0F B6 84 24 40 01  movzx       eax,byte ptr [rsp+0000000000000140h]
                    00 00
  00000001411B7AFB: 85 C0              test        eax,eax
  00000001411B7AFD: 0F 84 CC 00 00 00  je          00000001411B7BCF
  00000001411B7B03: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7B0B: E8 00 23 8E FF     call        0000000140A99E10
  00000001411B7B10: 48 8B C8           mov         rcx,rax
  00000001411B7B13: E8 E8 3C 6E FF     call        000000014089B800
  00000001411B7B18: 0F B6 C0           movzx       eax,al
  00000001411B7B1B: 85 C0              test        eax,eax
  00000001411B7B1D: 74 1C              je          00000001411B7B3B
  00000001411B7B1F: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7B27: E8 94 A9 00 00     call        00000001411C24C0
  00000001411B7B2C: 48 8B C8           mov         rcx,rax
  00000001411B7B2F: E8 CC 3C 6E FF     call        000000014089B800
  00000001411B7B34: 0F B6 C0           movzx       eax,al
  00000001411B7B37: 85 C0              test        eax,eax
  00000001411B7B39: 75 4E              jne         00000001411B7B89
  00000001411B7B3B: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7B43: E8 D8 AC 00 00     call        00000001411C2820
  00000001411B7B48: 0F B6 D8           movzx       ebx,al
  00000001411B7B4B: B9 1F 00 00 00     mov         ecx,1Fh
  00000001411B7B50: E8 DB A3 76 FF     call        0000000140921F30
  00000001411B7B55: 89 5C 24 28        mov         dword ptr [rsp+28h],ebx
  00000001411B7B59: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411B7B5D: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411B7B63: F2 0F 10 1D 45 2F  movsd       xmm3,mmword ptr [00000001427AAAB0h]
                    5F 01
  00000001411B7B6B: 66 49 0F 7E D9     movq        r9,xmm3
  00000001411B7B70: 4C 8B C0           mov         r8,rax
  00000001411B7B73: 48 8D 15 FE DB AE  lea         rdx,[0000000142CA5778h]
                    01
  00000001411B7B7A: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7B82: E8 85 5E 2B 00     call        000000014146DA0C
  00000001411B7B87: EB 41              jmp         00000001411B7BCA
  00000001411B7B89: B9 1F 00 00 00     mov         ecx,1Fh
  00000001411B7B8E: E8 9D A3 76 FF     call        0000000140921F30
  00000001411B7B93: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000001411B7B9C: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411B7BA0: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411B7BA6: F2 0F 10 1D 02 2F  movsd       xmm3,mmword ptr [00000001427AAAB0h]
                    5F 01
  00000001411B7BAE: 66 49 0F 7E D9     movq        r9,xmm3
  00000001411B7BB3: 4C 8B C0           mov         r8,rax
  00000001411B7BB6: 48 8D 15 DB DB AE  lea         rdx,[0000000142CA5798h]
                    01
  00000001411B7BBD: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7BC5: E8 42 5E 2B 00     call        000000014146DA0C
  00000001411B7BCA: E9 93 00 00 00     jmp         00000001411B7C62
  00000001411B7BCF: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7BD7: E8 34 22 8E FF     call        0000000140A99E10
  00000001411B7BDC: 48 8B C8           mov         rcx,rax
  00000001411B7BDF: E8 1C 3C 6E FF     call        000000014089B800
  00000001411B7BE4: 0F B6 C0           movzx       eax,al
  00000001411B7BE7: 85 C0              test        eax,eax
  00000001411B7BE9: 74 1C              je          00000001411B7C07
  00000001411B7BEB: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7BF3: E8 C8 A8 00 00     call        00000001411C24C0
  00000001411B7BF8: 48 8B C8           mov         rcx,rax
  00000001411B7BFB: E8 00 3C 6E FF     call        000000014089B800
  00000001411B7C00: 0F B6 C0           movzx       eax,al
  00000001411B7C03: 85 C0              test        eax,eax
  00000001411B7C05: 75 5B              jne         00000001411B7C62
  00000001411B7C07: B2 01              mov         dl,1
  00000001411B7C09: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7C11: E8 DA AF 00 00     call        00000001411C2BF0
  00000001411B7C16: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7C1E: E8 FD AB 00 00     call        00000001411C2820
  00000001411B7C23: 0F B6 D8           movzx       ebx,al
  00000001411B7C26: B9 1F 00 00 00     mov         ecx,1Fh
  00000001411B7C2B: E8 00 A3 76 FF     call        0000000140921F30
  00000001411B7C30: 89 5C 24 28        mov         dword ptr [rsp+28h],ebx
  00000001411B7C34: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411B7C38: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411B7C3E: F2 0F 10 1D 6A 2E  movsd       xmm3,mmword ptr [00000001427AAAB0h]
                    5F 01
  00000001411B7C46: 66 49 0F 7E D9     movq        r9,xmm3
  00000001411B7C4B: 4C 8B C0           mov         r8,rax
  00000001411B7C4E: 48 8D 15 63 DB AE  lea         rdx,[0000000142CA57B8h]
                    01
  00000001411B7C55: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7C5D: E8 AA 5D 2B 00     call        000000014146DA0C
  00000001411B7C62: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B7C6A: E8 91 5F 5A FF     call        000000014075DC00
  00000001411B7C6F: 0F B6 C0           movzx       eax,al
  00000001411B7C72: 85 C0              test        eax,eax
  00000001411B7C74: 74 40              je          00000001411B7CB6
  00000001411B7C76: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B7C7E: E8 4D 36 AC FF     call        0000000140C7B2D0
  00000001411B7C83: 48 8B D8           mov         rbx,rax
  00000001411B7C86: B9 4D 00 00 00     mov         ecx,4Dh
  00000001411B7C8B: E8 A0 A2 76 FF     call        0000000140921F30
  00000001411B7C90: F2 0F 10 9B B8 04  movsd       xmm3,mmword ptr [rbx+00000000000004B8h]
                    00 00
  00000001411B7C98: 66 49 0F 7E D9     movq        r9,xmm3
  00000001411B7C9D: 4C 8B C0           mov         r8,rax
  00000001411B7CA0: 48 8D 15 31 DB AE  lea         rdx,[0000000142CA57D8h]
                    01
  00000001411B7CA7: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7CAF: E8 58 5D 2B 00     call        000000014146DA0C
  00000001411B7CB4: EB 2A              jmp         00000001411B7CE0
  00000001411B7CB6: B9 4D 00 00 00     mov         ecx,4Dh
  00000001411B7CBB: E8 70 A2 76 FF     call        0000000140921F30
  00000001411B7CC0: 66 0F 57 DB        xorpd       xmm3,xmm3
  00000001411B7CC4: 66 49 0F 7E D9     movq        r9,xmm3
  00000001411B7CC9: 4C 8B C0           mov         r8,rax
  00000001411B7CCC: 48 8D 15 15 DB AE  lea         rdx,[0000000142CA57E8h]
                    01
  00000001411B7CD3: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7CDB: E8 2C 5D 2B 00     call        000000014146DA0C
  00000001411B7CE0: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7CE8: E8 23 21 8E FF     call        0000000140A99E10
  00000001411B7CED: 48 8B C8           mov         rcx,rax
  00000001411B7CF0: E8 0B 3B 6E FF     call        000000014089B800
  00000001411B7CF5: 0F B6 C0           movzx       eax,al
  00000001411B7CF8: 85 C0              test        eax,eax
  00000001411B7CFA: 74 35              je          00000001411B7D31
  00000001411B7CFC: B9 4B 00 00 00     mov         ecx,4Bh
  00000001411B7D01: E8 2A A2 76 FF     call        0000000140921F30
  00000001411B7D06: F2 0F 10 84 24 B0  movsd       xmm0,mmword ptr [rsp+00000000000001B0h]
                    01 00 00
  00000001411B7D0F: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411B7D15: 45 33 C9           xor         r9d,r9d
  00000001411B7D18: 4C 8B C0           mov         r8,rax
  00000001411B7D1B: 48 8D 15 D6 DA AE  lea         rdx,[0000000142CA57F8h]
                    01
  00000001411B7D22: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7D2A: E8 DD 5C 2B 00     call        000000014146DA0C
  00000001411B7D2F: EB 5E              jmp         00000001411B7D8F
  00000001411B7D31: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7D39: E8 D2 20 8E FF     call        0000000140A99E10
  00000001411B7D3E: 48 8B D8           mov         rbx,rax
  00000001411B7D41: B9 4B 00 00 00     mov         ecx,4Bh
  00000001411B7D46: E8 E5 A1 76 FF     call        0000000140921F30
  00000001411B7D4B: 4C 8B C0           mov         r8,rax
  00000001411B7D4E: 8B 43 1C           mov         eax,dword ptr [rbx+1Ch]
  00000001411B7D51: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001411B7D55: 41 B9 01 00 00 00  mov         r9d,1
  00000001411B7D5B: 48 8D 15 AE DA AE  lea         rdx,[0000000142CA5810h]
                    01
  00000001411B7D62: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7D6A: E8 9D 5C 2B 00     call        000000014146DA0C
  00000001411B7D6F: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7D77: E8 94 20 8E FF     call        0000000140A99E10
  00000001411B7D7C: 45 33 C0           xor         r8d,r8d
  00000001411B7D7F: 48 8B D0           mov         rdx,rax
  00000001411B7D82: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7D8A: E8 91 B5 76 FF     call        0000000140923320
  00000001411B7D8F: 33 D2              xor         edx,edx
  00000001411B7D91: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B7D99: E8 92 A9 00 00     call        00000001411C2730
  00000001411B7D9E: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  00000001411B7DA6: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001411B7DAE: 83 78 04 00        cmp         dword ptr [rax+4],0
  00000001411B7DB2: 75 39              jne         00000001411B7DED
  00000001411B7DB4: B9 4E 00 00 00     mov         ecx,4Eh
  00000001411B7DB9: E8 72 A1 76 FF     call        0000000140921F30
  00000001411B7DBE: 4C 8B C0           mov         r8,rax
  00000001411B7DC1: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001411B7DC9: F2 0F 10 40 08     movsd       xmm0,mmword ptr [rax+8]
  00000001411B7DCE: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411B7DD4: 45 33 C9           xor         r9d,r9d
  00000001411B7DD7: 48 8D 15 42 DA AE  lea         rdx,[0000000142CA5820h]
                    01
  00000001411B7DDE: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7DE6: E8 21 5C 2B 00     call        000000014146DA0C
  00000001411B7DEB: EB 60              jmp         00000001411B7E4D
  00000001411B7DED: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001411B7DF5: 83 78 04 01        cmp         dword ptr [rax+4],1
  00000001411B7DF9: 75 46              jne         00000001411B7E41
  00000001411B7DFB: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001411B7E03: 83 78 10 03        cmp         dword ptr [rax+10h],3
  00000001411B7E07: 75 38              jne         00000001411B7E41
  00000001411B7E09: B9 4E 00 00 00     mov         ecx,4Eh
  00000001411B7E0E: E8 1D A1 76 FF     call        0000000140921F30
  00000001411B7E13: 4C 8B C0           mov         r8,rax
  00000001411B7E16: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001411B7E1E: 8B 40 1C           mov         eax,dword ptr [rax+1Ch]
  00000001411B7E21: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001411B7E25: 41 B9 01 00 00 00  mov         r9d,1
  00000001411B7E2B: 48 8D 15 06 DA AE  lea         rdx,[0000000142CA5838h]
                    01
  00000001411B7E32: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7E3A: E8 CD 5B 2B 00     call        000000014146DA0C
  00000001411B7E3F: EB 0C              jmp         00000001411B7E4D
  00000001411B7E41: 48 8D 0D 00 DA AE  lea         rcx,[0000000142CA5848h]
                    01
  00000001411B7E48: E8 43 A9 2B 00     call        0000000141472790
  00000001411B7E4D: 45 33 C0           xor         r8d,r8d
  00000001411B7E50: 48 8B 94 24 C0 01  mov         rdx,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001411B7E58: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7E60: E8 BB B4 76 FF     call        0000000140923320
  00000001411B7E65: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B7E6D: E8 5E 34 AC FF     call        0000000140C7B2D0
  00000001411B7E72: 48 8B C8           mov         rcx,rax
  00000001411B7E75: E8 46 7F 72 FF     call        00000001408DFDC0
  00000001411B7E7A: 85 C0              test        eax,eax
  00000001411B7E7C: 0F 84 FE 01 00 00  je          00000001411B8080
  00000001411B7E82: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7E8A: E8 31 A6 00 00     call        00000001411C24C0
  00000001411B7E8F: 48 8B C8           mov         rcx,rax
  00000001411B7E92: E8 69 39 6E FF     call        000000014089B800
  00000001411B7E97: 0F B6 C0           movzx       eax,al
  00000001411B7E9A: 85 C0              test        eax,eax
  00000001411B7E9C: 74 35              je          00000001411B7ED3
  00000001411B7E9E: B9 4F 00 00 00     mov         ecx,4Fh
  00000001411B7EA3: E8 88 A0 76 FF     call        0000000140921F30
  00000001411B7EA8: F2 0F 10 84 24 C8  movsd       xmm0,mmword ptr [rsp+00000000000001C8h]
                    01 00 00
  00000001411B7EB1: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411B7EB7: 45 33 C9           xor         r9d,r9d
  00000001411B7EBA: 4C 8B C0           mov         r8,rax
  00000001411B7EBD: 48 8D 15 9C D9 AE  lea         rdx,[0000000142CA5860h]
                    01
  00000001411B7EC4: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7ECC: E8 3B 5B 2B 00     call        000000014146DA0C
  00000001411B7ED1: EB 5E              jmp         00000001411B7F31
  00000001411B7ED3: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7EDB: E8 E0 A5 00 00     call        00000001411C24C0
  00000001411B7EE0: 48 8B D8           mov         rbx,rax
  00000001411B7EE3: B9 4F 00 00 00     mov         ecx,4Fh
  00000001411B7EE8: E8 43 A0 76 FF     call        0000000140921F30
  00000001411B7EED: 4C 8B C0           mov         r8,rax
  00000001411B7EF0: 8B 43 1C           mov         eax,dword ptr [rbx+1Ch]
  00000001411B7EF3: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001411B7EF7: 41 B9 01 00 00 00  mov         r9d,1
  00000001411B7EFD: 48 8D 15 74 D9 AE  lea         rdx,[0000000142CA5878h]
                    01
  00000001411B7F04: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7F0C: E8 FB 5A 2B 00     call        000000014146DA0C
  00000001411B7F11: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B7F19: E8 A2 A5 00 00     call        00000001411C24C0
  00000001411B7F1E: 45 33 C0           xor         r8d,r8d
  00000001411B7F21: 48 8B D0           mov         rdx,rax
  00000001411B7F24: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7F2C: E8 EF B3 76 FF     call        0000000140923320
  00000001411B7F31: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B7F39: E8 F2 87 10 00     call        00000001412C0730
  00000001411B7F3E: 83 78 04 00        cmp         dword ptr [rax+4],0
  00000001411B7F42: 75 44              jne         00000001411B7F88
  00000001411B7F44: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B7F4C: E8 DF 87 10 00     call        00000001412C0730
  00000001411B7F51: 48 8B D8           mov         rbx,rax
  00000001411B7F54: B9 50 00 00 00     mov         ecx,50h
  00000001411B7F59: E8 D2 9F 76 FF     call        0000000140921F30
  00000001411B7F5E: F2 0F 10 43 08     movsd       xmm0,mmword ptr [rbx+8]
  00000001411B7F63: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411B7F69: 45 33 C9           xor         r9d,r9d
  00000001411B7F6C: 4C 8B C0           mov         r8,rax
  00000001411B7F6F: 48 8D 15 12 D9 AE  lea         rdx,[0000000142CA5888h]
                    01
  00000001411B7F76: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7F7E: E8 89 5A 2B 00     call        000000014146DA0C
  00000001411B7F83: E9 D8 00 00 00     jmp         00000001411B8060
  00000001411B7F88: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B7F90: E8 9B 87 10 00     call        00000001412C0730
  00000001411B7F95: 83 78 04 01        cmp         dword ptr [rax+4],1
  00000001411B7F99: 75 53              jne         00000001411B7FEE
  00000001411B7F9B: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B7FA3: E8 88 87 10 00     call        00000001412C0730
  00000001411B7FA8: 83 78 10 03        cmp         dword ptr [rax+10h],3
  00000001411B7FAC: 75 40              jne         00000001411B7FEE
  00000001411B7FAE: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B7FB6: E8 75 87 10 00     call        00000001412C0730
  00000001411B7FBB: 48 8B D8           mov         rbx,rax
  00000001411B7FBE: B9 50 00 00 00     mov         ecx,50h
  00000001411B7FC3: E8 68 9F 76 FF     call        0000000140921F30
  00000001411B7FC8: 4C 8B C0           mov         r8,rax
  00000001411B7FCB: 8B 43 1C           mov         eax,dword ptr [rbx+1Ch]
  00000001411B7FCE: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001411B7FD2: 41 B9 01 00 00 00  mov         r9d,1
  00000001411B7FD8: 48 8D 15 C1 D8 AE  lea         rdx,[0000000142CA58A0h]
                    01
  00000001411B7FDF: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B7FE7: E8 20 5A 2B 00     call        000000014146DA0C
  00000001411B7FEC: EB 72              jmp         00000001411B8060
  00000001411B7FEE: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B7FF6: E8 35 87 10 00     call        00000001412C0730
  00000001411B7FFB: 83 78 04 01        cmp         dword ptr [rax+4],1
  00000001411B7FFF: 75 53              jne         00000001411B8054
  00000001411B8001: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B8009: E8 22 87 10 00     call        00000001412C0730
  00000001411B800E: 83 78 10 0C        cmp         dword ptr [rax+10h],0Ch
  00000001411B8012: 75 40              jne         00000001411B8054
  00000001411B8014: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B801C: E8 0F 87 10 00     call        00000001412C0730
  00000001411B8021: 48 8B D8           mov         rbx,rax
  00000001411B8024: B9 50 00 00 00     mov         ecx,50h
  00000001411B8029: E8 02 9F 76 FF     call        0000000140921F30
  00000001411B802E: 4C 8B C0           mov         r8,rax
  00000001411B8031: 8B 43 1C           mov         eax,dword ptr [rbx+1Ch]
  00000001411B8034: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001411B8038: 41 B9 02 00 00 00  mov         r9d,2
  00000001411B803E: 48 8D 15 6B D8 AE  lea         rdx,[0000000142CA58B0h]
                    01
  00000001411B8045: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B804D: E8 BA 59 2B 00     call        000000014146DA0C
  00000001411B8052: EB 0C              jmp         00000001411B8060
  00000001411B8054: 48 8D 0D 65 D8 AE  lea         rcx,[0000000142CA58C0h]
                    01
  00000001411B805B: E8 30 A7 2B 00     call        0000000141472790
  00000001411B8060: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B8068: E8 C3 86 10 00     call        00000001412C0730
  00000001411B806D: 45 33 C0           xor         r8d,r8d
  00000001411B8070: 48 8B D0           mov         rdx,rax
  00000001411B8073: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B807B: E8 A0 B2 76 FF     call        0000000140923320
  00000001411B8080: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B8088: E8 43 32 AC FF     call        0000000140C7B2D0
  00000001411B808D: 48 8B C8           mov         rcx,rax
  00000001411B8090: E8 0B 7D 72 FF     call        00000001408DFDA0
  00000001411B8095: 85 C0              test        eax,eax
  00000001411B8097: 0F 84 73 02 00 00  je          00000001411B8310
  00000001411B809D: C7 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],0
                    00 00 00 00 00
  00000001411B80A8: EB 11              jmp         00000001411B80BB
  00000001411B80AA: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B80B1: 83 C0 01           add         eax,1
  00000001411B80B4: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B80BB: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B80C3: E8 E8 34 29 FF     call        000000014044B5B0
  00000001411B80C8: 48 8B C8           mov         rcx,rax
  00000001411B80CB: E8 A0 AF 61 FF     call        00000001407D3070
  00000001411B80D0: 39 84 24 58 01 00  cmp         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B80D7: 0F 8D 8F 00 00 00  jge         00000001411B816C
  00000001411B80DD: 8B 9C 24 58 01 00  mov         ebx,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B80E4: 83 C3 01           add         ebx,1
  00000001411B80E7: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B80EF: E8 BC 34 29 FF     call        000000014044B5B0
  00000001411B80F4: 8B D3              mov         edx,ebx
  00000001411B80F6: 48 8B C8           mov         rcx,rax
  00000001411B80F9: E8 A2 49 82 FF     call        00000001409DCAA0
  00000001411B80FE: 48 89 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],rax
                    00 00
  00000001411B8106: 48 83 BC 24 E0 01  cmp         qword ptr [rsp+00000000000001E0h],0
                    00 00 00
  00000001411B810F: 75 02              jne         00000001411B8113
  00000001411B8111: EB 97              jmp         00000001411B80AA
  00000001411B8113: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001411B811B: 83 78 08 FF        cmp         dword ptr [rax+8],0FFFFFFFFh
  00000001411B811F: 74 02              je          00000001411B8123
  00000001411B8121: EB 87              jmp         00000001411B80AA
  00000001411B8123: B9 86 00 00 00     mov         ecx,86h
  00000001411B8128: E8 03 9E 76 FF     call        0000000140921F30
  00000001411B812D: 4C 8B C0           mov         r8,rax
  00000001411B8130: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000001411B8139: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001411B8141: 8B 40 04           mov         eax,dword ptr [rax+4]
  00000001411B8144: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001411B8148: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  00000001411B8150: 44 8B 08           mov         r9d,dword ptr [rax]
  00000001411B8153: 48 8D 15 8E D7 AE  lea         rdx,[0000000142CA58E8h]
                    01
  00000001411B815A: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B8162: E8 A5 58 2B 00     call        000000014146DA0C
  00000001411B8167: E9 3E FF FF FF     jmp         00000001411B80AA
  00000001411B816C: 83 BC 24 3C 01 00  cmp         dword ptr [rsp+000000000000013Ch],0
                    00 00
  00000001411B8174: 0F 8E 96 01 00 00  jle         00000001411B8310
  00000001411B817A: 8B 94 24 3C 01 00  mov         edx,dword ptr [rsp+000000000000013Ch]
                    00
  00000001411B8181: 83 EA 01           sub         edx,1
  00000001411B8184: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B818C: E8 FF 88 FF FF     call        00000001411B0A90
  00000001411B8191: 48 89 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],rax
                    00 00
  00000001411B8199: C7 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],0
                    00 00 00 00 00
  00000001411B81A4: EB 11              jmp         00000001411B81B7
  00000001411B81A6: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B81AD: 83 C0 01           add         eax,1
  00000001411B81B0: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B81B7: 48 8B 8C 24 E8 01  mov         rcx,qword ptr [rsp+00000000000001E8h]
                    00 00
  00000001411B81BF: E8 EC 33 29 FF     call        000000014044B5B0
  00000001411B81C4: 48 8B C8           mov         rcx,rax
  00000001411B81C7: E8 A4 AE 61 FF     call        00000001407D3070
  00000001411B81CC: 39 84 24 58 01 00  cmp         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B81D3: 0F 8D 37 01 00 00  jge         00000001411B8310
  00000001411B81D9: 8B 9C 24 58 01 00  mov         ebx,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B81E0: 83 C3 01           add         ebx,1
  00000001411B81E3: 48 8B 8C 24 E8 01  mov         rcx,qword ptr [rsp+00000000000001E8h]
                    00 00
  00000001411B81EB: E8 C0 33 29 FF     call        000000014044B5B0
  00000001411B81F0: 8B D3              mov         edx,ebx
  00000001411B81F2: 48 8B C8           mov         rcx,rax
  00000001411B81F5: E8 A6 48 82 FF     call        00000001409DCAA0
  00000001411B81FA: 48 89 84 24 F0 01  mov         qword ptr [rsp+00000000000001F0h],rax
                    00 00
  00000001411B8202: 48 83 BC 24 F0 01  cmp         qword ptr [rsp+00000000000001F0h],0
                    00 00 00
  00000001411B820B: 75 02              jne         00000001411B820F
  00000001411B820D: EB 97              jmp         00000001411B81A6
  00000001411B820F: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001411B8217: 83 78 08 FF        cmp         dword ptr [rax+8],0FFFFFFFFh
  00000001411B821B: 74 02              je          00000001411B821F
  00000001411B821D: EB 87              jmp         00000001411B81A6
  00000001411B821F: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B8227: E8 84 33 29 FF     call        000000014044B5B0
  00000001411B822C: 48 8B C8           mov         rcx,rax
  00000001411B822F: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001411B8237: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  00000001411B823B: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001411B8243: 8B 10              mov         edx,dword ptr [rax]
  00000001411B8245: E8 B6 48 82 FF     call        00000001409DCB00
  00000001411B824A: 48 89 84 24 F8 01  mov         qword ptr [rsp+00000000000001F8h],rax
                    00 00
  00000001411B8252: 48 83 BC 24 F8 01  cmp         qword ptr [rsp+00000000000001F8h],0
                    00 00 00
  00000001411B825B: 74 13              je          00000001411B8270
  00000001411B825D: 48 8B 84 24 F8 01  mov         rax,qword ptr [rsp+00000000000001F8h]
                    00 00
  00000001411B8265: 83 78 08 FF        cmp         dword ptr [rax+8],0FFFFFFFFh
  00000001411B8269: 75 05              jne         00000001411B8270
  00000001411B826B: E9 36 FF FF FF     jmp         00000001411B81A6
  00000001411B8270: 48 83 BC 24 48 01  cmp         qword ptr [rsp+0000000000000148h],0
                    00 00 00
  00000001411B8279: 0F 84 8C 00 00 00  je          00000001411B830B
  00000001411B827F: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411B8287: 48 81 C1 58 13 00  add         rcx,1358h
                    00
  00000001411B828E: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001411B8296: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  00000001411B829A: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001411B82A2: 8B 10              mov         edx,dword ptr [rax]
  00000001411B82A4: E8 57 48 82 FF     call        00000001409DCB00
  00000001411B82A9: 48 89 84 24 00 02  mov         qword ptr [rsp+0000000000000200h],rax
                    00 00
  00000001411B82B1: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411B82B9: 48 81 C1 58 13 00  add         rcx,1358h
                    00
  00000001411B82C0: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001411B82C8: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  00000001411B82CC: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001411B82D4: 8B 10              mov         edx,dword ptr [rax]
  00000001411B82D6: E8 85 48 82 FF     call        00000001409DCB60
  00000001411B82DB: 89 84 24 08 02 00  mov         dword ptr [rsp+0000000000000208h],eax
                    00
  00000001411B82E2: 44 8B 8C 24 08 02  mov         r9d,dword ptr [rsp+0000000000000208h]
                    00 00
  00000001411B82EA: 41 83 C1 01        add         r9d,1
  00000001411B82EE: 4C 8B 84 24 00 02  mov         r8,qword ptr [rsp+0000000000000200h]
                    00 00
  00000001411B82F6: 48 8B 94 24 60 01  mov         rdx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B82FE: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411B8306: E8 B5 A0 99 FF     call        0000000140B523C0
  00000001411B830B: E9 96 FE FF FF     jmp         00000001411B81A6
  00000001411B8310: C7 84 24 90 01 00  mov         dword ptr [rsp+0000000000000190h],1
                    00 01 00 00 00
  00000001411B831B: 48 C7 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],0
                    00 00 00 00 00 00
  00000001411B8327: 48 83 BC 24 48 01  cmp         qword ptr [rsp+0000000000000148h],0
                    00 00 00
  00000001411B8330: 0F 84 EE 0C 00 00  je          00000001411B9024
  00000001411B8336: 48 8B 8C 24 48 01  mov         rcx,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411B833E: E8 3D 7E 67 FF     call        0000000140830180
  00000001411B8343: 89 84 24 90 01 00  mov         dword ptr [rsp+0000000000000190h],eax
                    00
  00000001411B834A: C7 84 24 B8 01 00  mov         dword ptr [rsp+00000000000001B8h],0
                    00 00 00 00 00
  00000001411B8355: EB 11              jmp         00000001411B8368
  00000001411B8357: 8B 84 24 B8 01 00  mov         eax,dword ptr [rsp+00000000000001B8h]
                    00
  00000001411B835E: 83 C0 01           add         eax,1
  00000001411B8361: 89 84 24 B8 01 00  mov         dword ptr [rsp+00000000000001B8h],eax
                    00
  00000001411B8368: 8B 84 24 90 01 00  mov         eax,dword ptr [rsp+0000000000000190h]
                    00
  00000001411B836F: 39 84 24 B8 01 00  cmp         dword ptr [rsp+00000000000001B8h],eax
                    00
  00000001411B8376: 0F 8D 29 0C 00 00  jge         00000001411B8FA5
  00000001411B837C: 8B 94 24 B8 01 00  mov         edx,dword ptr [rsp+00000000000001B8h]
                    00
  00000001411B8383: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B838B: E8 40 2B AC FF     call        0000000140C7AED0
  00000001411B8390: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  00000001411B8398: 48 83 BC 24 D8 01  cmp         qword ptr [rsp+00000000000001D8h],0
                    00 00 00
  00000001411B83A1: 75 02              jne         00000001411B83A5
  00000001411B83A3: EB B2              jmp         00000001411B8357
  00000001411B83A5: C7 84 24 80 01 00  mov         dword ptr [rsp+0000000000000180h],1
                    00 01 00 00 00
  00000001411B83B0: EB 11              jmp         00000001411B83C3
  00000001411B83B2: 8B 84 24 80 01 00  mov         eax,dword ptr [rsp+0000000000000180h]
                    00
  00000001411B83B9: 83 C0 01           add         eax,1
  00000001411B83BC: 89 84 24 80 01 00  mov         dword ptr [rsp+0000000000000180h],eax
                    00
  00000001411B83C3: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B83CB: E8 40 A3 00 00     call        00000001411C2710
  00000001411B83D0: 39 84 24 80 01 00  cmp         dword ptr [rsp+0000000000000180h],eax
                    00
  00000001411B83D7: 0F 8D F7 01 00 00  jge         00000001411B85D4
  00000001411B83DD: 8B 94 24 80 01 00  mov         edx,dword ptr [rsp+0000000000000180h]
                    00
  00000001411B83E4: 48 8B 8C 24 A8 01  mov         rcx,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001411B83EC: E8 3F A3 00 00     call        00000001411C2730
  00000001411B83F1: 48 89 84 24 48 02  mov         qword ptr [rsp+0000000000000248h],rax
                    00 00
  00000001411B83F9: C7 84 24 40 02 00  mov         dword ptr [rsp+0000000000000240h],0
                    00 00 00 00 00
  00000001411B8404: C7 84 24 50 02 00  mov         dword ptr [rsp+0000000000000250h],0
                    00 00 00 00 00
  00000001411B840F: 48 8B 84 24 48 02  mov         rax,qword ptr [rsp+0000000000000248h]
                    00 00
  00000001411B8417: 83 78 04 00        cmp         dword ptr [rax+4],0
  00000001411B841B: 75 0D              jne         00000001411B842A
  00000001411B841D: C7 84 24 40 02 00  mov         dword ptr [rsp+0000000000000240h],0
                    00 00 00 00 00
  00000001411B8428: EB 50              jmp         00000001411B847A
  00000001411B842A: 48 8B 84 24 48 02  mov         rax,qword ptr [rsp+0000000000000248h]
                    00 00
  00000001411B8432: 83 78 04 01        cmp         dword ptr [rax+4],1
  00000001411B8436: 75 1B              jne         00000001411B8453
  00000001411B8438: 48 8B 84 24 48 02  mov         rax,qword ptr [rsp+0000000000000248h]
                    00 00
  00000001411B8440: 83 78 10 01        cmp         dword ptr [rax+10h],1
  00000001411B8444: 75 0D              jne         00000001411B8453
  00000001411B8446: C7 84 24 40 02 00  mov         dword ptr [rsp+0000000000000240h],1
                    00 01 00 00 00
  00000001411B8451: EB 27              jmp         00000001411B847A
  00000001411B8453: 48 8B 84 24 48 02  mov         rax,qword ptr [rsp+0000000000000248h]
                    00 00
  00000001411B845B: 83 78 04 01        cmp         dword ptr [rax+4],1
  00000001411B845F: 75 19              jne         00000001411B847A
  00000001411B8461: 48 8B 84 24 48 02  mov         rax,qword ptr [rsp+0000000000000248h]
                    00 00
  00000001411B8469: 83 78 10 03        cmp         dword ptr [rax+10h],3
  00000001411B846D: 75 0B              jne         00000001411B847A
  00000001411B846F: C7 84 24 40 02 00  mov         dword ptr [rsp+0000000000000240h],2
                    00 02 00 00 00
  00000001411B847A: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B8482: E8 89 19 8E FF     call        0000000140A99E10
  00000001411B8487: 48 8B C8           mov         rcx,rax
  00000001411B848A: E8 71 33 6E FF     call        000000014089B800
  00000001411B848F: 0F B6 C0           movzx       eax,al
  00000001411B8492: 85 C0              test        eax,eax
  00000001411B8494: 74 0D              je          00000001411B84A3
  00000001411B8496: C7 84 24 50 02 00  mov         dword ptr [rsp+0000000000000250h],0
                    00 00 00 00 00
  00000001411B84A1: EB 0B              jmp         00000001411B84AE
  00000001411B84A3: C7 84 24 50 02 00  mov         dword ptr [rsp+0000000000000250h],1
                    00 01 00 00 00
  00000001411B84AE: B9 E6 00 00 00     mov         ecx,0E6h
  00000001411B84B3: E8 78 9A 76 FF     call        0000000140921F30
  00000001411B84B8: 4C 8B C0           mov         r8,rax
  00000001411B84BB: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  00000001411B84C4: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  00000001411B84CD: 8B 84 24 40 02 00  mov         eax,dword ptr [rsp+0000000000000240h]
                    00
  00000001411B84D4: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  00000001411B84D8: 8B 84 24 50 02 00  mov         eax,dword ptr [rsp+0000000000000250h]
                    00
  00000001411B84DF: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  00000001411B84E3: 48 C7 44 24 28 00  mov         qword ptr [rsp+28h],0
                    00 00 00
  00000001411B84EC: 8B 84 24 80 01 00  mov         eax,dword ptr [rsp+0000000000000180h]
                    00
  00000001411B84F3: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001411B84F7: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B84FF: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  00000001411B8503: 48 8D 15 F6 D3 AE  lea         rdx,[0000000142CA5900h]
                    01
  00000001411B850A: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B8512: E8 F5 54 2B 00     call        000000014146DA0C
  00000001411B8517: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B851F: E8 EC 18 8E FF     call        0000000140A99E10
  00000001411B8524: 48 8B C8           mov         rcx,rax
  00000001411B8527: E8 D4 32 6E FF     call        000000014089B800
  00000001411B852C: 0F B6 C0           movzx       eax,al
  00000001411B852F: 85 C0              test        eax,eax
  00000001411B8531: 74 24              je          00000001411B8557
  00000001411B8533: F2 0F 10 94 24 B0  movsd       xmm2,mmword ptr [rsp+00000000000001B0h]
                    01 00 00
  00000001411B853C: 66 49 0F 7E D0     movq        r8,xmm2
  00000001411B8541: 48 8D 15 E0 D3 AE  lea         rdx,[0000000142CA5928h]
                    01
  00000001411B8548: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B8550: E8 B7 54 2B 00     call        000000014146DA0C
  00000001411B8555: EB 23              jmp         00000001411B857A
  00000001411B8557: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B855F: E8 AC 18 8E FF     call        0000000140A99E10
  00000001411B8564: 41 B8 01 00 00 00  mov         r8d,1
  00000001411B856A: 48 8B D0           mov         rdx,rax
  00000001411B856D: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B8575: E8 A6 AD 76 FF     call        0000000140923320
  00000001411B857A: 41 B8 01 00 00 00  mov         r8d,1
  00000001411B8580: 48 8B 94 24 48 02  mov         rdx,qword ptr [rsp+0000000000000248h]
                    00 00
  00000001411B8588: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B8590: E8 8B AD 76 FF     call        0000000140923320
  00000001411B8595: 66 0F 57 D2        xorpd       xmm2,xmm2
  00000001411B8599: 66 49 0F 7E D0     movq        r8,xmm2
  00000001411B859E: 48 8D 15 93 D3 AE  lea         rdx,[0000000142CA5938h]
                    01
  00000001411B85A5: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B85AD: E8 5A 54 2B 00     call        000000014146DA0C
  00000001411B85B2: 66 0F 57 D2        xorpd       xmm2,xmm2
  00000001411B85B6: 66 49 0F 7E D0     movq        r8,xmm2
  00000001411B85BB: 48 8D 15 86 D3 AE  lea         rdx,[0000000142CA5948h]
                    01
  00000001411B85C2: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B85CA: E8 3D 54 2B 00     call        000000014146DA0C
  00000001411B85CF: E9 DE FD FF FF     jmp         00000001411B83B2
  00000001411B85D4: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B85DC: E8 AF 40 B0 FF     call        0000000140CBC690
  00000001411B85E1: 44 8B C0           mov         r8d,eax
  00000001411B85E4: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B85EC: 8B 50 08           mov         edx,dword ptr [rax+8]
  00000001411B85EF: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001411B85F7: E8 34 40 00 00     call        00000001411BC630
  00000001411B85FC: 48 89 84 24 10 02  mov         qword ptr [rsp+0000000000000210h],rax
                    00 00
  00000001411B8604: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B860C: E8 6F 0D 68 FF     call        0000000140839380
  00000001411B8611: 89 84 24 28 02 00  mov         dword ptr [rsp+0000000000000228h],eax
                    00
  00000001411B8618: 48 C7 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],0
                    00 00 00 00 00 00
  00000001411B8624: 8B 8C 24 28 02 00  mov         ecx,dword ptr [rsp+0000000000000228h]
                    00
  00000001411B862B: E8 F0 F9 80 FF     call        00000001409C8020
  00000001411B8630: 48 89 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],rax
                    00 00
  00000001411B8638: 8B 8C 24 28 02 00  mov         ecx,dword ptr [rsp+0000000000000228h]
                    00
  00000001411B863F: E8 DC F9 80 FF     call        00000001409C8020
  00000001411B8644: 48 89 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],rax
                    00 00
  00000001411B864C: 8B 8C 24 28 02 00  mov         ecx,dword ptr [rsp+0000000000000228h]
                    00
  00000001411B8653: E8 C8 F9 80 FF     call        00000001409C8020
  00000001411B8658: 48 89 84 24 18 02  mov         qword ptr [rsp+0000000000000218h],rax
                    00 00
  00000001411B8660: 8B 8C 24 28 02 00  mov         ecx,dword ptr [rsp+0000000000000228h]
                    00
  00000001411B8667: E8 B4 F9 80 FF     call        00000001409C8020
  00000001411B866C: 48 89 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],rax
                    00 00
  00000001411B8674: C7 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],0
                    00 00 00 00 00
  00000001411B867F: EB 11              jmp         00000001411B8692
  00000001411B8681: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B8688: 83 C0 01           add         eax,1
  00000001411B868B: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B8692: 8B 84 24 28 02 00  mov         eax,dword ptr [rsp+0000000000000228h]
                    00
  00000001411B8699: 39 84 24 58 01 00  cmp         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B86A0: 7D 5E              jge         00000001411B8700
  00000001411B86A2: 48 63 8C 24 58 01  movsxd      rcx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B86AA: 48 8B 84 24 20 02  mov         rax,qword ptr [rsp+0000000000000220h]
                    00 00
  00000001411B86B2: C7 04 88 02 00 00  mov         dword ptr [rax+rcx*4],2
                    00
  00000001411B86B9: 48 63 8C 24 58 01  movsxd      rcx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B86C1: 48 8B 84 24 30 02  mov         rax,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001411B86C9: C7 04 88 00 00 00  mov         dword ptr [rax+rcx*4],0
                    00
  00000001411B86D0: 48 63 8C 24 58 01  movsxd      rcx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B86D8: 48 8B 84 24 18 02  mov         rax,qword ptr [rsp+0000000000000218h]
                    00 00
  00000001411B86E0: C7 04 88 02 00 00  mov         dword ptr [rax+rcx*4],2
                    00
  00000001411B86E7: 48 63 8C 24 58 01  movsxd      rcx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B86EF: 48 8B 84 24 38 02  mov         rax,qword ptr [rsp+0000000000000238h]
                    00 00
  00000001411B86F7: C7 04 88 00 00 00  mov         dword ptr [rax+rcx*4],0
                    00
  00000001411B86FE: EB 81              jmp         00000001411B8681
  00000001411B8700: C7 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],0
                    00 00 00 00 00
  00000001411B870B: EB 11              jmp         00000001411B871E
  00000001411B870D: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B8714: 83 C0 01           add         eax,1
  00000001411B8717: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B871E: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8726: E8 55 0C 68 FF     call        0000000140839380
  00000001411B872B: 39 84 24 58 01 00  cmp         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B8732: 0F 8D 8F 04 00 00  jge         00000001411B8BC7
  00000001411B8738: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B8740: E8 9B 39 AC FF     call        0000000140C7C0E0
  00000001411B8745: 0F B6 C0           movzx       eax,al
  00000001411B8748: 85 C0              test        eax,eax
  00000001411B874A: 0F 84 36 02 00 00  je          00000001411B8986
  00000001411B8750: EB BB              jmp         00000001411B870D
  00000001411B8752: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411B875A: 8B 80 40 16 00 00  mov         eax,dword ptr [rax+0000000000001640h]
  00000001411B8760: 89 84 24 5C 02 00  mov         dword ptr [rsp+000000000000025Ch],eax
                    00
  00000001411B8767: C7 84 24 58 02 00  mov         dword ptr [rsp+0000000000000258h],0
                    00 00 00 00 00
  00000001411B8772: C7 84 24 54 02 00  mov         dword ptr [rsp+0000000000000254h],0
                    00 00 00 00 00
  00000001411B877D: 4C 8D 8C 24 54 02  lea         r9,[rsp+0000000000000254h]
                    00 00
  00000001411B8785: 4C 8D 84 24 58 02  lea         r8,[rsp+0000000000000258h]
                    00 00
  00000001411B878D: 8B 94 24 58 01 00  mov         edx,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B8794: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B879C: E8 DF C4 6B FF     call        0000000140874C80
  00000001411B87A1: 83 BC 24 58 02 00  cmp         dword ptr [rsp+0000000000000258h],0
                    00 00
  00000001411B87A9: 7E 0A              jle         00000001411B87B5
  00000001411B87AB: 83 BC 24 54 02 00  cmp         dword ptr [rsp+0000000000000254h],0
                    00 00
  00000001411B87B3: 7F 05              jg          00000001411B87BA
  00000001411B87B5: E9 53 FF FF FF     jmp         00000001411B870D
  00000001411B87BA: 8B 84 24 58 02 00  mov         eax,dword ptr [rsp+0000000000000258h]
                    00
  00000001411B87C1: 83 E8 01           sub         eax,1
  00000001411B87C4: 0F AF 84 24 5C 02  imul        eax,dword ptr [rsp+000000000000025Ch]
                    00 00
  00000001411B87CC: 48 63 C8           movsxd      rcx,eax
  00000001411B87CF: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B87D7: 48 8B 80 90 09 00  mov         rax,qword ptr [rax+0000000000000990h]
                    00
  00000001411B87DE: 83 3C 88 01        cmp         dword ptr [rax+rcx*4],1
  00000001411B87E2: 0F 85 B5 00 00 00  jne         00000001411B889D
  00000001411B87E8: 8B 84 24 54 02 00  mov         eax,dword ptr [rsp+0000000000000254h]
                    00
  00000001411B87EF: 83 E8 01           sub         eax,1
  00000001411B87F2: 0F AF 84 24 5C 02  imul        eax,dword ptr [rsp+000000000000025Ch]
                    00 00
  00000001411B87FA: 48 63 C8           movsxd      rcx,eax
  00000001411B87FD: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8805: 48 8B 80 90 09 00  mov         rax,qword ptr [rax+0000000000000990h]
                    00
  00000001411B880C: 83 3C 88 01        cmp         dword ptr [rax+rcx*4],1
  00000001411B8810: 0F 85 87 00 00 00  jne         00000001411B889D
  00000001411B8816: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B881E: 48 81 C1 A0 08 00  add         rcx,8A0h
                    00
  00000001411B8825: E8 C6 C2 69 FF     call        0000000140854AF0
  00000001411B882A: 48 8B F8           mov         rdi,rax
  00000001411B882D: 8B 84 24 58 02 00  mov         eax,dword ptr [rsp+0000000000000258h]
                    00
  00000001411B8834: 8D 44 00 FE        lea         eax,[rax+rax-2]
  00000001411B8838: 48 63 D8           movsxd      rbx,eax
  00000001411B883B: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8843: 48 81 C1 A0 08 00  add         rcx,8A0h
                    00
  00000001411B884A: E8 A1 C2 69 FF     call        0000000140854AF0
  00000001411B884F: 4C 8B D8           mov         r11,rax
  00000001411B8852: 8B 84 24 54 02 00  mov         eax,dword ptr [rsp+0000000000000254h]
                    00
  00000001411B8859: 8D 44 00 FE        lea         eax,[rax+rax-2]
  00000001411B885D: 48 98              cdqe
  00000001411B885F: F2 0F 10 04 DF     movsd       xmm0,mmword ptr [rdi+rbx*8]
  00000001411B8864: F2 41 0F 5C 04 C3  subsd       xmm0,mmword ptr [r11+rax*8]
  00000001411B886A: E8 09 82 2B 00     call        0000000141470A78
  00000001411B886F: 66 0F 28 E8        movapd      xmm5,xmm0
  00000001411B8873: F2 0F 10 05 A5 EB  movsd       xmm0,mmword ptr [0000000142157420h]
                    F9 00
  00000001411B887B: 66 0F 2F C5        comisd      xmm0,xmm5
  00000001411B887F: 76 17              jbe         00000001411B8898
  00000001411B8881: 48 63 8C 24 58 01  movsxd      rcx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B8889: 48 8B 84 24 38 02  mov         rax,qword ptr [rsp+0000000000000238h]
                    00 00
  00000001411B8891: C7 04 88 01 00 00  mov         dword ptr [rax+rcx*4],1
                    00
  00000001411B8898: E9 E4 00 00 00     jmp         00000001411B8981
  00000001411B889D: 8B 84 24 58 02 00  mov         eax,dword ptr [rsp+0000000000000258h]
                    00
  00000001411B88A4: 83 E8 01           sub         eax,1
  00000001411B88A7: 0F AF 84 24 5C 02  imul        eax,dword ptr [rsp+000000000000025Ch]
                    00 00
  00000001411B88AF: 83 C0 01           add         eax,1
  00000001411B88B2: 48 63 C8           movsxd      rcx,eax
  00000001411B88B5: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B88BD: 48 8B 80 90 09 00  mov         rax,qword ptr [rax+0000000000000990h]
                    00
  00000001411B88C4: 83 3C 88 01        cmp         dword ptr [rax+rcx*4],1
  00000001411B88C8: 0F 85 B3 00 00 00  jne         00000001411B8981
  00000001411B88CE: 8B 84 24 54 02 00  mov         eax,dword ptr [rsp+0000000000000254h]
                    00
  00000001411B88D5: 83 E8 01           sub         eax,1
  00000001411B88D8: 0F AF 84 24 5C 02  imul        eax,dword ptr [rsp+000000000000025Ch]
                    00 00
  00000001411B88E0: 83 C0 01           add         eax,1
  00000001411B88E3: 48 63 C8           movsxd      rcx,eax
  00000001411B88E6: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B88EE: 48 8B 80 90 09 00  mov         rax,qword ptr [rax+0000000000000990h]
                    00
  00000001411B88F5: 83 3C 88 01        cmp         dword ptr [rax+rcx*4],1
  00000001411B88F9: 0F 85 82 00 00 00  jne         00000001411B8981
  00000001411B88FF: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8907: 48 81 C1 A0 08 00  add         rcx,8A0h
                    00
  00000001411B890E: E8 DD C1 69 FF     call        0000000140854AF0
  00000001411B8913: 48 8B F8           mov         rdi,rax
  00000001411B8916: 8B 84 24 58 02 00  mov         eax,dword ptr [rsp+0000000000000258h]
                    00
  00000001411B891D: 8D 44 00 FF        lea         eax,[rax+rax-1]
  00000001411B8921: 48 63 D8           movsxd      rbx,eax
  00000001411B8924: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B892C: 48 81 C1 A0 08 00  add         rcx,8A0h
                    00
  00000001411B8933: E8 B8 C1 69 FF     call        0000000140854AF0
  00000001411B8938: 4C 8B D8           mov         r11,rax
  00000001411B893B: 8B 84 24 54 02 00  mov         eax,dword ptr [rsp+0000000000000254h]
                    00
  00000001411B8942: 8D 44 00 FF        lea         eax,[rax+rax-1]
  00000001411B8946: 48 98              cdqe
  00000001411B8948: F2 0F 10 04 DF     movsd       xmm0,mmword ptr [rdi+rbx*8]
  00000001411B894D: F2 41 0F 5C 04 C3  subsd       xmm0,mmword ptr [r11+rax*8]
  00000001411B8953: E8 20 81 2B 00     call        0000000141470A78
  00000001411B8958: 66 0F 28 E8        movapd      xmm5,xmm0
  00000001411B895C: F2 0F 10 05 BC EA  movsd       xmm0,mmword ptr [0000000142157420h]
                    F9 00
  00000001411B8964: 66 0F 2F C5        comisd      xmm0,xmm5
  00000001411B8968: 76 17              jbe         00000001411B8981
  00000001411B896A: 48 63 8C 24 58 01  movsxd      rcx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B8972: 48 8B 84 24 38 02  mov         rax,qword ptr [rsp+0000000000000238h]
                    00 00
  00000001411B897A: C7 04 88 02 00 00  mov         dword ptr [rax+rcx*4],2
                    00
  00000001411B8981: E9 3C 02 00 00     jmp         00000001411B8BC2
  00000001411B8986: 48 8D 8C 24 6C 02  lea         rcx,[rsp+000000000000026Ch]
                    00 00
  00000001411B898E: 48 8D 84 24 68 02  lea         rax,[rsp+0000000000000268h]
                    00 00
  00000001411B8996: 4C 8D 8C 24 64 02  lea         r9,[rsp+0000000000000264h]
                    00 00
  00000001411B899E: 48 89 4C 24 28     mov         qword ptr [rsp+28h],rcx
  00000001411B89A3: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411B89A8: 4C 8D 84 24 60 02  lea         r8,[rsp+0000000000000260h]
                    00 00
  00000001411B89B0: 8B 94 24 58 01 00  mov         edx,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B89B7: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B89BF: E8 AC C6 6B FF     call        0000000140875070
  00000001411B89C4: C7 84 24 74 02 00  mov         dword ptr [rsp+0000000000000274h],0
                    00 00 00 00 00
  00000001411B89CF: EB 11              jmp         00000001411B89E2
  00000001411B89D1: 8B 84 24 74 02 00  mov         eax,dword ptr [rsp+0000000000000274h]
                    00
  00000001411B89D8: 83 C0 01           add         eax,1
  00000001411B89DB: 89 84 24 74 02 00  mov         dword ptr [rsp+0000000000000274h],eax
                    00
  00000001411B89E2: 83 BC 24 74 02 00  cmp         dword ptr [rsp+0000000000000274h],3
                    00 03
  00000001411B89EA: 0F 8D D2 01 00 00  jge         00000001411B8BC2
  00000001411B89F0: 8B 84 24 60 02 00  mov         eax,dword ptr [rsp+0000000000000260h]
                    00
  00000001411B89F7: 83 E8 01           sub         eax,1
  00000001411B89FA: 6B C0 03           imul        eax,eax,3
  00000001411B89FD: 03 84 24 74 02 00  add         eax,dword ptr [rsp+0000000000000274h]
                    00
  00000001411B8A04: 48 63 C8           movsxd      rcx,eax
  00000001411B8A07: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8A0F: 48 8B 80 90 09 00  mov         rax,qword ptr [rax+0000000000000990h]
                    00
  00000001411B8A16: 83 3C 88 06        cmp         dword ptr [rax+rcx*4],6
  00000001411B8A1A: 0F 85 9D 01 00 00  jne         00000001411B8BBD
  00000001411B8A20: 8B 84 24 60 02 00  mov         eax,dword ptr [rsp+0000000000000260h]
                    00
  00000001411B8A27: 83 E8 01           sub         eax,1
  00000001411B8A2A: 6B C0 03           imul        eax,eax,3
  00000001411B8A2D: 03 84 24 74 02 00  add         eax,dword ptr [rsp+0000000000000274h]
                    00
  00000001411B8A34: 48 63 C8           movsxd      rcx,eax
  00000001411B8A37: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8A3F: 48 8B 80 98 09 00  mov         rax,qword ptr [rax+0000000000000998h]
                    00
  00000001411B8A46: 8B 04 88           mov         eax,dword ptr [rax+rcx*4]
  00000001411B8A49: 89 84 24 78 02 00  mov         dword ptr [rsp+0000000000000278h],eax
                    00
  00000001411B8A50: C7 84 24 7C 02 00  mov         dword ptr [rsp+000000000000027Ch],1
                    00 01 00 00 00
  00000001411B8A5B: C7 84 24 70 02 00  mov         dword ptr [rsp+0000000000000270h],0
                    00 00 00 00 00
  00000001411B8A66: EB 11              jmp         00000001411B8A79
  00000001411B8A68: 8B 84 24 70 02 00  mov         eax,dword ptr [rsp+0000000000000270h]
                    00
  00000001411B8A6F: 83 C0 01           add         eax,1
  00000001411B8A72: 89 84 24 70 02 00  mov         dword ptr [rsp+0000000000000270h],eax
                    00
  00000001411B8A79: 83 BC 24 70 02 00  cmp         dword ptr [rsp+0000000000000270h],3
                    00 03
  00000001411B8A81: 7D 76              jge         00000001411B8AF9
  00000001411B8A83: 8B 84 24 64 02 00  mov         eax,dword ptr [rsp+0000000000000264h]
                    00
  00000001411B8A8A: 83 E8 01           sub         eax,1
  00000001411B8A8D: 6B C0 03           imul        eax,eax,3
  00000001411B8A90: 03 84 24 70 02 00  add         eax,dword ptr [rsp+0000000000000270h]
                    00
  00000001411B8A97: 48 63 C8           movsxd      rcx,eax
  00000001411B8A9A: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8AA2: 48 8B 80 90 09 00  mov         rax,qword ptr [rax+0000000000000990h]
                    00
  00000001411B8AA9: 83 3C 88 06        cmp         dword ptr [rax+rcx*4],6
  00000001411B8AAD: 75 45              jne         00000001411B8AF4
  00000001411B8AAF: 8B 84 24 64 02 00  mov         eax,dword ptr [rsp+0000000000000264h]
                    00
  00000001411B8AB6: 83 E8 01           sub         eax,1
  00000001411B8AB9: 6B C0 03           imul        eax,eax,3
  00000001411B8ABC: 03 84 24 70 02 00  add         eax,dword ptr [rsp+0000000000000270h]
                    00
  00000001411B8AC3: 48 63 D0           movsxd      rdx,eax
  00000001411B8AC6: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8ACE: 48 8B 89 98 09 00  mov         rcx,qword ptr [rcx+0000000000000998h]
                    00
  00000001411B8AD5: 8B 84 24 78 02 00  mov         eax,dword ptr [rsp+0000000000000278h]
                    00
  00000001411B8ADC: 39 04 91           cmp         dword ptr [rcx+rdx*4],eax
  00000001411B8ADF: 75 13              jne         00000001411B8AF4
  00000001411B8AE1: 8B 84 24 7C 02 00  mov         eax,dword ptr [rsp+000000000000027Ch]
                    00
  00000001411B8AE8: 83 C0 01           add         eax,1
  00000001411B8AEB: 89 84 24 7C 02 00  mov         dword ptr [rsp+000000000000027Ch],eax
                    00
  00000001411B8AF2: EB 05              jmp         00000001411B8AF9
  00000001411B8AF4: E9 6F FF FF FF     jmp         00000001411B8A68
  00000001411B8AF9: C7 84 24 70 02 00  mov         dword ptr [rsp+0000000000000270h],0
                    00 00 00 00 00
  00000001411B8B04: EB 11              jmp         00000001411B8B17
  00000001411B8B06: 8B 84 24 70 02 00  mov         eax,dword ptr [rsp+0000000000000270h]
                    00
  00000001411B8B0D: 83 C0 01           add         eax,1
  00000001411B8B10: 89 84 24 70 02 00  mov         dword ptr [rsp+0000000000000270h],eax
                    00
  00000001411B8B17: 83 BC 24 70 02 00  cmp         dword ptr [rsp+0000000000000270h],3
                    00 03
  00000001411B8B1F: 7D 76              jge         00000001411B8B97
  00000001411B8B21: 8B 84 24 68 02 00  mov         eax,dword ptr [rsp+0000000000000268h]
                    00
  00000001411B8B28: 83 E8 01           sub         eax,1
  00000001411B8B2B: 6B C0 03           imul        eax,eax,3
  00000001411B8B2E: 03 84 24 70 02 00  add         eax,dword ptr [rsp+0000000000000270h]
                    00
  00000001411B8B35: 48 63 C8           movsxd      rcx,eax
  00000001411B8B38: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8B40: 48 8B 80 90 09 00  mov         rax,qword ptr [rax+0000000000000990h]
                    00
  00000001411B8B47: 83 3C 88 06        cmp         dword ptr [rax+rcx*4],6
  00000001411B8B4B: 75 45              jne         00000001411B8B92
  00000001411B8B4D: 8B 84 24 68 02 00  mov         eax,dword ptr [rsp+0000000000000268h]
                    00
  00000001411B8B54: 83 E8 01           sub         eax,1
  00000001411B8B57: 6B C0 03           imul        eax,eax,3
  00000001411B8B5A: 03 84 24 70 02 00  add         eax,dword ptr [rsp+0000000000000270h]
                    00
  00000001411B8B61: 48 63 D0           movsxd      rdx,eax
  00000001411B8B64: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8B6C: 48 8B 89 98 09 00  mov         rcx,qword ptr [rcx+0000000000000998h]
                    00
  00000001411B8B73: 8B 84 24 78 02 00  mov         eax,dword ptr [rsp+0000000000000278h]
                    00
  00000001411B8B7A: 39 04 91           cmp         dword ptr [rcx+rdx*4],eax
  00000001411B8B7D: 75 13              jne         00000001411B8B92
  00000001411B8B7F: 8B 84 24 7C 02 00  mov         eax,dword ptr [rsp+000000000000027Ch]
                    00
  00000001411B8B86: 83 C0 01           add         eax,1
  00000001411B8B89: 89 84 24 7C 02 00  mov         dword ptr [rsp+000000000000027Ch],eax
                    00
  00000001411B8B90: EB 05              jmp         00000001411B8B97
  00000001411B8B92: E9 6F FF FF FF     jmp         00000001411B8B06
  00000001411B8B97: 83 BC 24 7C 02 00  cmp         dword ptr [rsp+000000000000027Ch],3
                    00 03
  00000001411B8B9F: 7C 1C              jl          00000001411B8BBD
  00000001411B8BA1: 48 63 94 24 58 01  movsxd      rdx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B8BA9: 48 8B 8C 24 38 02  mov         rcx,qword ptr [rsp+0000000000000238h]
                    00 00
  00000001411B8BB1: 8B 84 24 78 02 00  mov         eax,dword ptr [rsp+0000000000000278h]
                    00
  00000001411B8BB8: 89 04 91           mov         dword ptr [rcx+rdx*4],eax
  00000001411B8BBB: EB 05              jmp         00000001411B8BC2
  00000001411B8BBD: E9 0F FE FF FF     jmp         00000001411B89D1
  00000001411B8BC2: E9 46 FB FF FF     jmp         00000001411B870D
  00000001411B8BC7: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B8BCF: E8 0C 35 AC FF     call        0000000140C7C0E0
  00000001411B8BD4: 0F B6 C0           movzx       eax,al
  00000001411B8BD7: 85 C0              test        eax,eax
  00000001411B8BD9: 74 15              je          00000001411B8BF0
  00000001411B8BDB: 48 8B 8C 24 D8 01  mov         rcx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8BE3: E8 78 19 DD FF     call        0000000140F8A560
  00000001411B8BE8: 48 89 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],rax
                    00 00
  00000001411B8BF0: 48 83 BC 24 38 02  cmp         qword ptr [rsp+0000000000000238h],0
                    00 00 00
  00000001411B8BF9: 0F 84 B9 01 00 00  je          00000001411B8DB8
  00000001411B8BFF: 48 83 BC 24 10 02  cmp         qword ptr [rsp+0000000000000210h],0
                    00 00 00
  00000001411B8C08: 0F 84 1F 01 00 00  je          00000001411B8D2D
  00000001411B8C0E: C7 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],0
                    00 00 00 00 00
  00000001411B8C19: EB 11              jmp         00000001411B8C2C
  00000001411B8C1B: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B8C22: 83 C0 01           add         eax,1
  00000001411B8C25: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B8C2C: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001411B8C34: E8 A7 98 00 00     call        00000001411C24E0
  00000001411B8C39: 39 84 24 58 01 00  cmp         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B8C40: 0F 8D E7 00 00 00  jge         00000001411B8D2D
  00000001411B8C46: 8B 94 24 58 01 00  mov         edx,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B8C4D: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001411B8C55: E8 06 99 00 00     call        00000001411C2560
  00000001411B8C5A: 89 84 24 84 02 00  mov         dword ptr [rsp+0000000000000284h],eax
                    00
  00000001411B8C61: 8B 94 24 58 01 00  mov         edx,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B8C68: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  00000001411B8C70: E8 8B 98 00 00     call        00000001411C2500
  00000001411B8C75: 89 84 24 80 02 00  mov         dword ptr [rsp+0000000000000280h],eax
                    00
  00000001411B8C7C: 83 BC 24 84 02 00  cmp         dword ptr [rsp+0000000000000284h],0
                    00 00
  00000001411B8C84: 0F 8E 9E 00 00 00  jle         00000001411B8D28
  00000001411B8C8A: 83 BC 24 80 02 00  cmp         dword ptr [rsp+0000000000000280h],0
                    00 00
  00000001411B8C92: 0F 8E 90 00 00 00  jle         00000001411B8D28
  00000001411B8C98: 8B 84 24 80 02 00  mov         eax,dword ptr [rsp+0000000000000280h]
                    00
  00000001411B8C9F: 83 E8 01           sub         eax,1
  00000001411B8CA2: 48 63 C8           movsxd      rcx,eax
  00000001411B8CA5: 48 8B 84 24 38 02  mov         rax,qword ptr [rsp+0000000000000238h]
                    00 00
  00000001411B8CAD: 83 3C 88 00        cmp         dword ptr [rax+rcx*4],0
  00000001411B8CB1: 74 3A              je          00000001411B8CED
  00000001411B8CB3: 8B 84 24 80 02 00  mov         eax,dword ptr [rsp+0000000000000280h]
                    00
  00000001411B8CBA: 83 E8 01           sub         eax,1
  00000001411B8CBD: 48 63 C8           movsxd      rcx,eax
  00000001411B8CC0: 48 8B 84 24 20 02  mov         rax,qword ptr [rsp+0000000000000220h]
                    00 00
  00000001411B8CC8: C7 04 88 00 00 00  mov         dword ptr [rax+rcx*4],0
                    00
  00000001411B8CCF: 8B 84 24 80 02 00  mov         eax,dword ptr [rsp+0000000000000280h]
                    00
  00000001411B8CD6: 83 E8 01           sub         eax,1
  00000001411B8CD9: 48 63 C8           movsxd      rcx,eax
  00000001411B8CDC: 48 8B 84 24 30 02  mov         rax,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001411B8CE4: C7 04 88 00 00 00  mov         dword ptr [rax+rcx*4],0
                    00
  00000001411B8CEB: EB 3B              jmp         00000001411B8D28
  00000001411B8CED: 8B 84 24 80 02 00  mov         eax,dword ptr [rsp+0000000000000280h]
                    00
  00000001411B8CF4: 83 E8 01           sub         eax,1
  00000001411B8CF7: 48 63 C8           movsxd      rcx,eax
  00000001411B8CFA: 48 8B 84 24 20 02  mov         rax,qword ptr [rsp+0000000000000220h]
                    00 00
  00000001411B8D02: C7 04 88 04 00 00  mov         dword ptr [rax+rcx*4],4
                    00
  00000001411B8D09: 8B 84 24 80 02 00  mov         eax,dword ptr [rsp+0000000000000280h]
                    00
  00000001411B8D10: 83 E8 01           sub         eax,1
  00000001411B8D13: 48 63 D0           movsxd      rdx,eax
  00000001411B8D16: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001411B8D1E: 8B 84 24 84 02 00  mov         eax,dword ptr [rsp+0000000000000284h]
                    00
  00000001411B8D25: 89 04 91           mov         dword ptr [rcx+rdx*4],eax
  00000001411B8D28: E9 EE FE FF FF     jmp         00000001411B8C1B
  00000001411B8D2D: C7 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],0
                    00 00 00 00 00
  00000001411B8D38: EB 11              jmp         00000001411B8D4B
  00000001411B8D3A: 8B 84 24 58 01 00  mov         eax,dword ptr [rsp+0000000000000158h]
                    00
  00000001411B8D41: 83 C0 01           add         eax,1
  00000001411B8D44: 89 84 24 58 01 00  mov         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B8D4B: 8B 84 24 28 02 00  mov         eax,dword ptr [rsp+0000000000000228h]
                    00
  00000001411B8D52: 39 84 24 58 01 00  cmp         dword ptr [rsp+0000000000000158h],eax
                    00
  00000001411B8D59: 7D 5D              jge         00000001411B8DB8
  00000001411B8D5B: 48 63 8C 24 58 01  movsxd      rcx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B8D63: 48 8B 84 24 38 02  mov         rax,qword ptr [rsp+0000000000000238h]
                    00 00
  00000001411B8D6B: 83 3C 88 00        cmp         dword ptr [rax+rcx*4],0
  00000001411B8D6F: 74 45              je          00000001411B8DB6
  00000001411B8D71: 48 63 8C 24 58 01  movsxd      rcx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B8D79: 48 8B 84 24 20 02  mov         rax,qword ptr [rsp+0000000000000220h]
                    00 00
  00000001411B8D81: C7 04 88 00 00 00  mov         dword ptr [rax+rcx*4],0
                    00
  00000001411B8D88: 48 63 8C 24 58 01  movsxd      rcx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B8D90: 48 8B 84 24 30 02  mov         rax,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001411B8D98: C7 04 88 00 00 00  mov         dword ptr [rax+rcx*4],0
                    00
  00000001411B8D9F: 48 63 8C 24 58 01  movsxd      rcx,dword ptr [rsp+0000000000000158h]
                    00 00
  00000001411B8DA7: 48 8B 84 24 18 02  mov         rax,qword ptr [rsp+0000000000000218h]
                    00 00
  00000001411B8DAF: C7 04 88 00 00 00  mov         dword ptr [rax+rcx*4],0
                    00
  00000001411B8DB6: EB 82              jmp         00000001411B8D3A
  00000001411B8DB8: B9 F2 00 00 00     mov         ecx,0F2h
  00000001411B8DBD: E8 6E 91 76 FF     call        0000000140921F30
  00000001411B8DC2: 48 8B D0           mov         rdx,rax
  00000001411B8DC5: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000001411B8DCD: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411B8DD5: 8B 80 08 17 00 00  mov         eax,dword ptr [rax+0000000000001708h]
  00000001411B8DDB: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001411B8DDF: 48 8B 84 24 20 02  mov         rax,qword ptr [rsp+0000000000000220h]
                    00 00
  00000001411B8DE7: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411B8DEC: 4C 8B 8C 24 D8 01  mov         r9,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8DF4: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8DFC: 44 8B 40 08        mov         r8d,dword ptr [rax+8]
  00000001411B8E00: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B8E08: E8 E3 D9 76 FF     call        00000001409267F0
  00000001411B8E0D: B9 F5 00 00 00     mov         ecx,0F5h
  00000001411B8E12: E8 19 91 76 FF     call        0000000140921F30
  00000001411B8E17: 48 8B D0           mov         rdx,rax
  00000001411B8E1A: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000001411B8E22: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411B8E2A: 8B 80 0C 17 00 00  mov         eax,dword ptr [rax+000000000000170Ch]
  00000001411B8E30: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001411B8E34: 48 8B 84 24 30 02  mov         rax,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001411B8E3C: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411B8E41: 4C 8B 8C 24 D8 01  mov         r9,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8E49: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8E51: 44 8B 40 08        mov         r8d,dword ptr [rax+8]
  00000001411B8E55: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B8E5D: E8 8E D9 76 FF     call        00000001409267F0
  00000001411B8E62: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B8E6A: E8 61 24 AC FF     call        0000000140C7B2D0
  00000001411B8E6F: 48 8B C8           mov         rcx,rax
  00000001411B8E72: E8 49 6F 72 FF     call        00000001408DFDC0
  00000001411B8E77: 85 C0              test        eax,eax
  00000001411B8E79: 74 55              je          00000001411B8ED0
  00000001411B8E7B: B9 F3 00 00 00     mov         ecx,0F3h
  00000001411B8E80: E8 AB 90 76 FF     call        0000000140921F30
  00000001411B8E85: 48 8B D0           mov         rdx,rax
  00000001411B8E88: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  00000001411B8E90: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411B8E98: 8B 80 14 17 00 00  mov         eax,dword ptr [rax+0000000000001714h]
  00000001411B8E9E: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001411B8EA2: 48 8B 84 24 18 02  mov         rax,qword ptr [rsp+0000000000000218h]
                    00 00
  00000001411B8EAA: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001411B8EAF: 4C 8B 8C 24 D8 01  mov         r9,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8EB7: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B8EBF: 44 8B 40 08        mov         r8d,dword ptr [rax+8]
  00000001411B8EC3: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B8ECB: E8 20 D9 76 FF     call        00000001409267F0
  00000001411B8ED0: 48 83 BC 24 20 02  cmp         qword ptr [rsp+0000000000000220h],0
                    00 00 00
  00000001411B8ED9: 74 24              je          00000001411B8EFF
  00000001411B8EDB: 48 83 BC 24 20 02  cmp         qword ptr [rsp+0000000000000220h],0
                    00 00 00
  00000001411B8EE4: 74 0D              je          00000001411B8EF3
  00000001411B8EE6: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  00000001411B8EEE: E8 89 8C 2B 00     call        0000000141471B7C
  00000001411B8EF3: 48 C7 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],0
                    00 00 00 00 00 00
  00000001411B8EFF: 48 83 BC 24 30 02  cmp         qword ptr [rsp+0000000000000230h],0
                    00 00 00
  00000001411B8F08: 74 24              je          00000001411B8F2E
  00000001411B8F0A: 48 83 BC 24 30 02  cmp         qword ptr [rsp+0000000000000230h],0
                    00 00 00
  00000001411B8F13: 74 0D              je          00000001411B8F22
  00000001411B8F15: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001411B8F1D: E8 5A 8C 2B 00     call        0000000141471B7C
  00000001411B8F22: 48 C7 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],0
                    00 00 00 00 00 00
  00000001411B8F2E: 48 83 BC 24 18 02  cmp         qword ptr [rsp+0000000000000218h],0
                    00 00 00
  00000001411B8F37: 74 24              je          00000001411B8F5D
  00000001411B8F39: 48 83 BC 24 18 02  cmp         qword ptr [rsp+0000000000000218h],0
                    00 00 00
  00000001411B8F42: 74 0D              je          00000001411B8F51
  00000001411B8F44: 48 8B 8C 24 18 02  mov         rcx,qword ptr [rsp+0000000000000218h]
                    00 00
  00000001411B8F4C: E8 2B 8C 2B 00     call        0000000141471B7C
  00000001411B8F51: 48 C7 84 24 18 02  mov         qword ptr [rsp+0000000000000218h],0
                    00 00 00 00 00 00
  00000001411B8F5D: 48 83 BC 24 38 02  cmp         qword ptr [rsp+0000000000000238h],0
                    00 00 00
  00000001411B8F66: 74 38              je          00000001411B8FA0
  00000001411B8F68: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B8F70: E8 2B 31 AC FF     call        0000000140C7C0A0
  00000001411B8F75: 0F B6 C0           movzx       eax,al
  00000001411B8F78: 85 C0              test        eax,eax
  00000001411B8F7A: 74 24              je          00000001411B8FA0
  00000001411B8F7C: 48 83 BC 24 38 02  cmp         qword ptr [rsp+0000000000000238h],0
                    00 00 00
  00000001411B8F85: 74 0D              je          00000001411B8F94
  00000001411B8F87: 48 8B 8C 24 38 02  mov         rcx,qword ptr [rsp+0000000000000238h]
                    00 00
  00000001411B8F8F: E8 E8 8B 2B 00     call        0000000141471B7C
  00000001411B8F94: 48 C7 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],0
                    00 00 00 00 00 00
  00000001411B8FA0: E9 B2 F3 FF FF     jmp         00000001411B8357
  00000001411B8FA5: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  00000001411B8FAD: E8 1E 23 AC FF     call        0000000140C7B2D0
  00000001411B8FB2: 48 8B C8           mov         rcx,rax
  00000001411B8FB5: E8 E6 6D 72 FF     call        00000001408DFDA0
  00000001411B8FBA: 85 C0              test        eax,eax
  00000001411B8FBC: 74 66              je          00000001411B9024
  00000001411B8FBE: 48 8B 8C 24 A0 01  mov         rcx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001411B8FC6: E8 35 98 00 00     call        00000001411C2800
  00000001411B8FCB: 0F B6 C0           movzx       eax,al
  00000001411B8FCE: 85 C0              test        eax,eax
  00000001411B8FD0: 74 52              je          00000001411B9024
  00000001411B8FD2: B9 CF 00 00 00     mov         ecx,0CFh
  00000001411B8FD7: E8 54 8F 76 FF     call        0000000140921F30
  00000001411B8FDC: 4C 8B C0           mov         r8,rax
  00000001411B8FDF: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411B8FE3: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  00000001411B8FE9: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001411B8FF1: 8B 80 C8 16 00 00  mov         eax,dword ptr [rax+00000000000016C8h]
  00000001411B8FF7: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001411B8FFB: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  00000001411B9004: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001411B900C: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  00000001411B9010: 48 8D 15 41 C9 AE  lea         rdx,[0000000142CA5958h]
                    01
  00000001411B9017: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B901F: E8 E8 49 2B 00     call        000000014146DA0C
  00000001411B9024: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001411B902C: E8 F7 6A 2B 00     call        000000014146FB28
  00000001411B9031: E9 F4 E4 FF FF     jmp         00000001411B752A
  00000001411B9036: C6 84 24 B8 02 00  mov         byte ptr [rsp+00000000000002B8h],1
                    00 01
  00000001411B903E: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001411B9046: FF 15 94 C9 C4 00  call        qword ptr [0000000141E059E0h]
  00000001411B904C: 8A 84 24 B8 02 00  mov         al,byte ptr [rsp+00000000000002B8h]
                    00
  00000001411B9053: 48 8B 8C 24 38 03  mov         rcx,qword ptr [rsp+0000000000000338h]
                    00 00
  00000001411B905B: 48 33 CC           xor         rcx,rsp
  00000001411B905E: E8 AD 4E 2B 00     call        000000014146DF10
  00000001411B9063: 48 81 C4 48 03 00  add         rsp,348h
                    00
  00000001411B906A: 5F                 pop         rdi
  00000001411B906B: 5B                 pop         rbx
  00000001411B906C: C3                 ret

  Summary

     1E02000 .text

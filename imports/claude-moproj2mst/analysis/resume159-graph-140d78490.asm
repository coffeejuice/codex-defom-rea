Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D78490: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D78495: 48 81 EC E8 03 00  sub         rsp,3E8h
                    00
  0000000140D7849C: 48 C7 84 24 A0 02  mov         qword ptr [rsp+00000000000002A0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D784A8: C7 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],0
                    00 00 00 00 00
  0000000140D784B3: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  0000000140D784BC: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00 00
  0000000140D784C8: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  0000000140D784D1: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D784D9: FF 15 21 31 09 01  call        qword ptr [0000000141E0B600h]
  0000000140D784DF: C6 44 24 61 00     mov         byte ptr [rsp+61h],0
  0000000140D784E4: C6 44 24 48 00     mov         byte ptr [rsp+48h],0
  0000000140D784E9: 48 8B 84 24 F0 03  mov         rax,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D784F1: 48 83 78 50 00     cmp         qword ptr [rax+50h],0
  0000000140D784F6: 74 53              je          0000000140D7854B
  0000000140D784F8: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D78500: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D78504: E8 97 3B F0 FF     call        0000000140C7C0A0
  0000000140D78509: 88 44 24 61        mov         byte ptr [rsp+61h],al
  0000000140D7850D: 0F B6 44 24 61     movzx       eax,byte ptr [rsp+61h]
  0000000140D78512: 85 C0              test        eax,eax
  0000000140D78514: 75 35              jne         0000000140D7854B
  0000000140D78516: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D7851E: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D78522: E8 D9 28 F0 FF     call        0000000140C7AE00
  0000000140D78527: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140D7852F: 48 83 BC 24 90 00  cmp         qword ptr [rsp+0000000000000090h],0
                    00 00 00
  0000000140D78538: 74 11              je          0000000140D7854B
  0000000140D7853A: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140D78542: E8 D9 43 F9 FF     call        0000000140D0C920
  0000000140D78547: 88 44 24 48        mov         byte ptr [rsp+48h],al
  0000000140D7854B: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D78550: E8 8B CE 2E FF     call        00000001400653E0
  0000000140D78555: 90                 nop
  0000000140D78556: 48 C7 44 24 58 00  mov         qword ptr [rsp+58h],0
                    00 00 00
  0000000140D7855F: 48 8D 15 BE 39 CC  lea         rdx,[0000000142A3BF24h]
                    01
  0000000140D78566: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140D7856E: FF 15 0C BF 08 01  call        qword ptr [0000000141E04480h]
  0000000140D78574: 90                 nop
  0000000140D78575: 48 8D 15 F0 38 CC  lea         rdx,[0000000142A3BE6Ch]
                    01
  0000000140D7857C: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140D78584: FF 15 F6 BE 08 01  call        qword ptr [0000000141E04480h]
  0000000140D7858A: 90                 nop
  0000000140D7858B: 48 8D 15 CA 38 CC  lea         rdx,[0000000142A3BE5Ch]
                    01
  0000000140D78592: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140D7859A: FF 15 E0 BE 08 01  call        qword ptr [0000000141E04480h]
  0000000140D785A0: 90                 nop
  0000000140D785A1: 48 8D 15 44 37 CC  lea         rdx,[0000000142A3BCECh]
                    01
  0000000140D785A8: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140D785B0: FF 15 CA BE 08 01  call        qword ptr [0000000141E04480h]
  0000000140D785B6: 90                 nop
  0000000140D785B7: 48 8D 15 86 4D CC  lea         rdx,[0000000142A3D344h]
                    01
  0000000140D785BE: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140D785C6: FF 15 B4 BE 08 01  call        qword ptr [0000000141E04480h]
  0000000140D785CC: 90                 nop
  0000000140D785CD: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  0000000140D785D5: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D785DA: E8 E1 1D F5 FF     call        0000000140CCA3C0
  0000000140D785DF: 48 8D 94 24 B8 00  lea         rdx,[rsp+00000000000000B8h]
                    00 00
  0000000140D785E7: 48 8B C8           mov         rcx,rax
  0000000140D785EA: E8 D1 1D F5 FF     call        0000000140CCA3C0
  0000000140D785EF: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  0000000140D785F7: 48 8B C8           mov         rcx,rax
  0000000140D785FA: E8 C1 1D F5 FF     call        0000000140CCA3C0
  0000000140D785FF: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  0000000140D78607: 48 8B C8           mov         rcx,rax
  0000000140D7860A: E8 B1 1D F5 FF     call        0000000140CCA3C0
  0000000140D7860F: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  0000000140D78617: 48 8B C8           mov         rcx,rax
  0000000140D7861A: E8 A1 1D F5 FF     call        0000000140CCA3C0
  0000000140D7861F: 90                 nop
  0000000140D78620: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140D78628: FF 15 B2 D3 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7862E: 90                 nop
  0000000140D7862F: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  0000000140D78637: FF 15 A3 D3 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7863D: 90                 nop
  0000000140D7863E: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140D78646: FF 15 94 D3 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7864C: 90                 nop
  0000000140D7864D: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  0000000140D78655: FF 15 85 D3 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7865B: 90                 nop
  0000000140D7865C: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140D78664: FF 15 76 D3 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7866A: 0F B6 44 24 61     movzx       eax,byte ptr [rsp+61h]
  0000000140D7866F: 85 C0              test        eax,eax
  0000000140D78671: 75 09              jne         0000000140D7867C
  0000000140D78673: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000140D78678: 85 C0              test        eax,eax
  0000000140D7867A: 74 37              je          0000000140D786B3
  0000000140D7867C: 48 8D 15 D1 38 CC  lea         rdx,[0000000142A3BF54h]
                    01
  0000000140D78683: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000140D7868B: FF 15 EF BD 08 01  call        qword ptr [0000000141E04480h]
  0000000140D78691: 90                 nop
  0000000140D78692: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  0000000140D7869A: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D7869F: E8 1C 1D F5 FF     call        0000000140CCA3C0
  0000000140D786A4: 90                 nop
  0000000140D786A5: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000140D786AD: FF 15 2D D3 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D786B3: 0F B6 44 24 61     movzx       eax,byte ptr [rsp+61h]
  0000000140D786B8: 85 C0              test        eax,eax
  0000000140D786BA: 74 37              je          0000000140D786F3
  0000000140D786BC: 48 8D 15 C1 38 CC  lea         rdx,[0000000142A3BF84h]
                    01
  0000000140D786C3: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140D786CB: FF 15 AF BD 08 01  call        qword ptr [0000000141E04480h]
  0000000140D786D1: 90                 nop
  0000000140D786D2: 48 8D 94 24 E0 00  lea         rdx,[rsp+00000000000000E0h]
                    00 00
  0000000140D786DA: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D786DF: E8 DC 1C F5 FF     call        0000000140CCA3C0
  0000000140D786E4: 90                 nop
  0000000140D786E5: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140D786ED: FF 15 ED D2 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D786F3: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000140D786F8: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D78700: FF 15 82 D3 08 01  call        qword ptr [0000000141E05A88h]
  0000000140D78706: B9 10 00 00 00     mov         ecx,10h
  0000000140D7870B: E8 AC 63 6F 00     call        000000014146EABC
  0000000140D78710: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140D78718: 48 83 BC 24 F0 00  cmp         qword ptr [rsp+00000000000000F0h],0
                    00 00 00
  0000000140D78721: 0F 84 CF 00 00 00  je          0000000140D787F6
  0000000140D78727: 48 8D 15 81 34 CC  lea         rdx,[0000000142A3BBAFh]
                    01
  0000000140D7872E: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140D78736: FF 15 44 BD 08 01  call        qword ptr [0000000141E04480h]
  0000000140D7873C: 90                 nop
  0000000140D7873D: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78744: 83 C8 01           or          eax,1
  0000000140D78747: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D7874E: 48 8D 15 93 6A CC  lea         rdx,[0000000142A3F1E8h]
                    01
  0000000140D78755: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  0000000140D7875D: FF 15 1D BD 08 01  call        qword ptr [0000000141E04480h]
  0000000140D78763: 90                 nop
  0000000140D78764: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D7876B: 83 C8 02           or          eax,2
  0000000140D7876E: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78775: 48 8D 94 24 08 01  lea         rdx,[rsp+0000000000000108h]
                    00 00
  0000000140D7877D: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140D78785: FF 15 05 2E 09 01  call        qword ptr [0000000141E0B590h]
  0000000140D7878B: 48 89 84 24 A8 02  mov         qword ptr [rsp+00000000000002A8h],rax
                    00 00
  0000000140D78793: 48 8B 84 24 A8 02  mov         rax,qword ptr [rsp+00000000000002A8h]
                    00 00
  0000000140D7879B: 48 89 84 24 B0 02  mov         qword ptr [rsp+00000000000002B0h],rax
                    00 00
  0000000140D787A3: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D787AA: 83 C8 04           or          eax,4
  0000000140D787AD: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D787B4: 4C 8D 84 24 F8 00  lea         r8,[rsp+00000000000000F8h]
                    00 00
  0000000140D787BC: 48 8B 94 24 B0 02  mov         rdx,qword ptr [rsp+00000000000002B0h]
                    00 00
  0000000140D787C4: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D787CC: FF 15 AE D2 08 01  call        qword ptr [0000000141E05A80h]
  0000000140D787D2: 4C 8B 9C 24 F0 00  mov         r11,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D787DA: 48 8D 05 37 63 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D787E1: 49 89 03           mov         qword ptr [r11],rax
  0000000140D787E4: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D787EC: 48 89 84 24 B8 02  mov         qword ptr [rsp+00000000000002B8h],rax
                    00 00
  0000000140D787F4: EB 0C              jmp         0000000140D78802
  0000000140D787F6: 48 C7 84 24 B8 02  mov         qword ptr [rsp+00000000000002B8h],0
                    00 00 00 00 00 00
  0000000140D78802: 48 8B 84 24 B8 02  mov         rax,qword ptr [rsp+00000000000002B8h]
                    00 00
  0000000140D7880A: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000140D78812: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140D7881A: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140D78822: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78829: 83 E0 04           and         eax,4
  0000000140D7882C: 85 C0              test        eax,eax
  0000000140D7882E: 74 17              je          0000000140D78847
  0000000140D78830: 83 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFFFBh
                    00 FB
  0000000140D78838: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140D78840: FF 15 5A 2E 09 01  call        qword ptr [0000000141E0B6A0h]
  0000000140D78846: 90                 nop
  0000000140D78847: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D7884E: 83 E0 02           and         eax,2
  0000000140D78851: 85 C0              test        eax,eax
  0000000140D78853: 74 17              je          0000000140D7886C
  0000000140D78855: 83 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFFFDh
                    00 FD
  0000000140D7885D: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  0000000140D78865: FF 15 75 D1 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D7886B: 90                 nop
  0000000140D7886C: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78873: 83 E0 01           and         eax,1
  0000000140D78876: 85 C0              test        eax,eax
  0000000140D78878: 74 16              je          0000000140D78890
  0000000140D7887A: 83 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFFFEh
                    00 FE
  0000000140D78882: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140D7888A: FF 15 50 D1 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D78890: 45 33 C0           xor         r8d,r8d
  0000000140D78893: 48 8D 15 86 69 CC  lea         rdx,[0000000142A3F220h]
                    01
  0000000140D7889A: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000140D788A2: E8 19 3D 00 00     call        0000000140D7C5C0
  0000000140D788A7: 48 89 84 24 C0 02  mov         qword ptr [rsp+00000000000002C0h],rax
                    00 00
  0000000140D788AF: 48 8B 84 24 C0 02  mov         rax,qword ptr [rsp+00000000000002C0h]
                    00 00
  0000000140D788B7: 48 89 84 24 C8 02  mov         qword ptr [rsp+00000000000002C8h],rax
                    00 00
  0000000140D788BF: 48 8B 94 24 C8 02  mov         rdx,qword ptr [rsp+00000000000002C8h]
                    00 00
  0000000140D788C7: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D788CF: FF 15 0B 15 09 01  call        qword ptr [0000000141E09DE0h]
  0000000140D788D5: 90                 nop
  0000000140D788D6: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000140D788DE: FF 15 FC D0 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D788E4: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D788EC: BA 01 00 00 00     mov         edx,1
  0000000140D788F1: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D788F9: FF 15 39 F8 08 01  call        qword ptr [0000000141E08138h]
  0000000140D788FF: B9 10 00 00 00     mov         ecx,10h
  0000000140D78904: E8 B3 61 6F 00     call        000000014146EABC
  0000000140D78909: 48 89 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],rax
                    00 00
  0000000140D78911: 48 83 BC 24 20 01  cmp         qword ptr [rsp+0000000000000120h],0
                    00 00 00
  0000000140D7891A: 0F 84 CF 00 00 00  je          0000000140D789EF
  0000000140D78920: 48 8D 15 8F 32 CC  lea         rdx,[0000000142A3BBB6h]
                    01
  0000000140D78927: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  0000000140D7892F: FF 15 4B BB 08 01  call        qword ptr [0000000141E04480h]
  0000000140D78935: 90                 nop
  0000000140D78936: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D7893D: 83 C8 08           or          eax,8
  0000000140D78940: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78947: 48 8D 15 EA 68 CC  lea         rdx,[0000000142A3F238h]
                    01
  0000000140D7894E: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  0000000140D78956: FF 15 24 BB 08 01  call        qword ptr [0000000141E04480h]
  0000000140D7895C: 90                 nop
  0000000140D7895D: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78964: 83 C8 10           or          eax,10h
  0000000140D78967: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D7896E: 48 8D 94 24 38 01  lea         rdx,[rsp+0000000000000138h]
                    00 00
  0000000140D78976: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  0000000140D7897E: FF 15 0C 2C 09 01  call        qword ptr [0000000141E0B590h]
  0000000140D78984: 48 89 84 24 D0 02  mov         qword ptr [rsp+00000000000002D0h],rax
                    00 00
  0000000140D7898C: 48 8B 84 24 D0 02  mov         rax,qword ptr [rsp+00000000000002D0h]
                    00 00
  0000000140D78994: 48 89 84 24 D8 02  mov         qword ptr [rsp+00000000000002D8h],rax
                    00 00
  0000000140D7899C: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D789A3: 83 C8 20           or          eax,20h
  0000000140D789A6: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D789AD: 4C 8D 84 24 28 01  lea         r8,[rsp+0000000000000128h]
                    00 00
  0000000140D789B5: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D789BD: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140D789C5: FF 15 B5 D0 08 01  call        qword ptr [0000000141E05A80h]
  0000000140D789CB: 4C 8B 9C 24 20 01  mov         r11,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140D789D3: 48 8D 05 3E 61 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D789DA: 49 89 03           mov         qword ptr [r11],rax
  0000000140D789DD: 48 8B 84 24 20 01  mov         rax,qword ptr [rsp+0000000000000120h]
                    00 00
  0000000140D789E5: 48 89 84 24 E0 02  mov         qword ptr [rsp+00000000000002E0h],rax
                    00 00
  0000000140D789ED: EB 0C              jmp         0000000140D789FB
  0000000140D789EF: 48 C7 84 24 E0 02  mov         qword ptr [rsp+00000000000002E0h],0
                    00 00 00 00 00 00
  0000000140D789FB: 48 8B 84 24 E0 02  mov         rax,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140D78A03: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  0000000140D78A0B: 48 8B 84 24 18 01  mov         rax,qword ptr [rsp+0000000000000118h]
                    00 00
  0000000140D78A13: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140D78A1B: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78A22: 83 E0 20           and         eax,20h
  0000000140D78A25: 85 C0              test        eax,eax
  0000000140D78A27: 74 17              je          0000000140D78A40
  0000000140D78A29: 83 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFFDFh
                    00 DF
  0000000140D78A31: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  0000000140D78A39: FF 15 61 2C 09 01  call        qword ptr [0000000141E0B6A0h]
  0000000140D78A3F: 90                 nop
  0000000140D78A40: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78A47: 83 E0 10           and         eax,10h
  0000000140D78A4A: 85 C0              test        eax,eax
  0000000140D78A4C: 74 17              je          0000000140D78A65
  0000000140D78A4E: 83 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFFEFh
                    00 EF
  0000000140D78A56: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  0000000140D78A5E: FF 15 7C CF 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D78A64: 90                 nop
  0000000140D78A65: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78A6C: 83 E0 08           and         eax,8
  0000000140D78A6F: 85 C0              test        eax,eax
  0000000140D78A71: 74 16              je          0000000140D78A89
  0000000140D78A73: 83 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFFF7h
                    00 F7
  0000000140D78A7B: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  0000000140D78A83: FF 15 57 CF 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D78A89: 45 33 C0           xor         r8d,r8d
  0000000140D78A8C: 48 8D 15 D5 67 CC  lea         rdx,[0000000142A3F268h]
                    01
  0000000140D78A93: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000140D78A9B: E8 20 3B 00 00     call        0000000140D7C5C0
  0000000140D78AA0: 48 89 84 24 E8 02  mov         qword ptr [rsp+00000000000002E8h],rax
                    00 00
  0000000140D78AA8: 48 8B 84 24 E8 02  mov         rax,qword ptr [rsp+00000000000002E8h]
                    00 00
  0000000140D78AB0: 48 89 84 24 F0 02  mov         qword ptr [rsp+00000000000002F0h],rax
                    00 00
  0000000140D78AB8: 48 8B 94 24 F0 02  mov         rdx,qword ptr [rsp+00000000000002F0h]
                    00 00
  0000000140D78AC0: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D78AC8: FF 15 12 13 09 01  call        qword ptr [0000000141E09DE0h]
  0000000140D78ACE: 90                 nop
  0000000140D78ACF: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000140D78AD7: FF 15 03 CF 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D78ADD: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D78AE5: BA 02 00 00 00     mov         edx,2
  0000000140D78AEA: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D78AF2: FF 15 40 F6 08 01  call        qword ptr [0000000141E08138h]
  0000000140D78AF8: B9 10 00 00 00     mov         ecx,10h
  0000000140D78AFD: E8 BA 5F 6F 00     call        000000014146EABC
  0000000140D78B02: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  0000000140D78B0A: 48 83 BC 24 50 01  cmp         qword ptr [rsp+0000000000000150h],0
                    00 00 00
  0000000140D78B13: 0F 84 D3 00 00 00  je          0000000140D78BEC
  0000000140D78B19: 48 8D 15 97 30 CC  lea         rdx,[0000000142A3BBB7h]
                    01
  0000000140D78B20: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  0000000140D78B28: FF 15 52 B9 08 01  call        qword ptr [0000000141E04480h]
  0000000140D78B2E: 90                 nop
  0000000140D78B2F: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78B36: 83 C8 40           or          eax,40h
  0000000140D78B39: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78B40: 48 8D 15 29 67 CC  lea         rdx,[0000000142A3F270h]
                    01
  0000000140D78B47: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  0000000140D78B4F: FF 15 2B B9 08 01  call        qword ptr [0000000141E04480h]
  0000000140D78B55: 90                 nop
  0000000140D78B56: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78B5D: 0D 80 00 00 00     or          eax,80h
  0000000140D78B62: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78B69: 48 8D 94 24 68 01  lea         rdx,[rsp+0000000000000168h]
                    00 00
  0000000140D78B71: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  0000000140D78B79: FF 15 11 2A 09 01  call        qword ptr [0000000141E0B590h]
  0000000140D78B7F: 48 89 84 24 F8 02  mov         qword ptr [rsp+00000000000002F8h],rax
                    00 00
  0000000140D78B87: 48 8B 84 24 F8 02  mov         rax,qword ptr [rsp+00000000000002F8h]
                    00 00
  0000000140D78B8F: 48 89 84 24 00 03  mov         qword ptr [rsp+0000000000000300h],rax
                    00 00
  0000000140D78B97: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78B9E: 0D 00 01 00 00     or          eax,100h
  0000000140D78BA3: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78BAA: 4C 8D 84 24 58 01  lea         r8,[rsp+0000000000000158h]
                    00 00
  0000000140D78BB2: 48 8B 94 24 00 03  mov         rdx,qword ptr [rsp+0000000000000300h]
                    00 00
  0000000140D78BBA: 48 8B 8C 24 50 01  mov         rcx,qword ptr [rsp+0000000000000150h]
                    00 00
  0000000140D78BC2: FF 15 B8 CE 08 01  call        qword ptr [0000000141E05A80h]
  0000000140D78BC8: 4C 8B 9C 24 50 01  mov         r11,qword ptr [rsp+0000000000000150h]
                    00 00
  0000000140D78BD0: 48 8D 05 41 5F 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D78BD7: 49 89 03           mov         qword ptr [r11],rax
  0000000140D78BDA: 48 8B 84 24 50 01  mov         rax,qword ptr [rsp+0000000000000150h]
                    00 00
  0000000140D78BE2: 48 89 84 24 08 03  mov         qword ptr [rsp+0000000000000308h],rax
                    00 00
  0000000140D78BEA: EB 0C              jmp         0000000140D78BF8
  0000000140D78BEC: 48 C7 84 24 08 03  mov         qword ptr [rsp+0000000000000308h],0
                    00 00 00 00 00 00
  0000000140D78BF8: 48 8B 84 24 08 03  mov         rax,qword ptr [rsp+0000000000000308h]
                    00 00
  0000000140D78C00: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  0000000140D78C08: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  0000000140D78C10: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140D78C18: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78C1F: 25 00 01 00 00     and         eax,100h
  0000000140D78C24: 85 C0              test        eax,eax
  0000000140D78C26: 74 1A              je          0000000140D78C42
  0000000140D78C28: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFEFFh
                    00 FF FE FF FF
  0000000140D78C33: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  0000000140D78C3B: FF 15 5F 2A 09 01  call        qword ptr [0000000141E0B6A0h]
  0000000140D78C41: 90                 nop
  0000000140D78C42: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78C49: 25 80 00 00 00     and         eax,80h
  0000000140D78C4E: 85 C0              test        eax,eax
  0000000140D78C50: 74 1A              je          0000000140D78C6C
  0000000140D78C52: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFF7Fh
                    00 7F FF FF FF
  0000000140D78C5D: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  0000000140D78C65: FF 15 75 CD 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D78C6B: 90                 nop
  0000000140D78C6C: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78C73: 83 E0 40           and         eax,40h
  0000000140D78C76: 85 C0              test        eax,eax
  0000000140D78C78: 74 16              je          0000000140D78C90
  0000000140D78C7A: 83 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFFBFh
                    00 BF
  0000000140D78C82: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  0000000140D78C8A: FF 15 50 CD 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D78C90: 45 33 C0           xor         r8d,r8d
  0000000140D78C93: 48 8D 15 7A 4E CC  lea         rdx,[0000000142A3DB14h]
                    01
  0000000140D78C9A: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  0000000140D78CA2: E8 19 39 00 00     call        0000000140D7C5C0
  0000000140D78CA7: 48 89 84 24 10 03  mov         qword ptr [rsp+0000000000000310h],rax
                    00 00
  0000000140D78CAF: 48 8B 84 24 10 03  mov         rax,qword ptr [rsp+0000000000000310h]
                    00 00
  0000000140D78CB7: 48 89 84 24 18 03  mov         qword ptr [rsp+0000000000000318h],rax
                    00 00
  0000000140D78CBF: 48 8B 94 24 18 03  mov         rdx,qword ptr [rsp+0000000000000318h]
                    00 00
  0000000140D78CC7: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D78CCF: FF 15 0B 11 09 01  call        qword ptr [0000000141E09DE0h]
  0000000140D78CD5: 90                 nop
  0000000140D78CD6: 48 8D 8C 24 70 01  lea         rcx,[rsp+0000000000000170h]
                    00 00
  0000000140D78CDE: FF 15 FC CC 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D78CE4: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D78CEC: BA 03 00 00 00     mov         edx,3
  0000000140D78CF1: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D78CF9: FF 15 39 F4 08 01  call        qword ptr [0000000141E08138h]
  0000000140D78CFF: B9 10 00 00 00     mov         ecx,10h
  0000000140D78D04: E8 B3 5D 6F 00     call        000000014146EABC
  0000000140D78D09: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  0000000140D78D11: 48 83 BC 24 80 01  cmp         qword ptr [rsp+0000000000000180h],0
                    00 00 00
  0000000140D78D1A: 0F 84 D5 00 00 00  je          0000000140D78DF5
  0000000140D78D20: 48 8D 15 A0 2E CC  lea         rdx,[0000000142A3BBC7h]
                    01
  0000000140D78D27: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140D78D2F: FF 15 4B B7 08 01  call        qword ptr [0000000141E04480h]
  0000000140D78D35: 90                 nop
  0000000140D78D36: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78D3D: 0D 00 02 00 00     or          eax,200h
  0000000140D78D42: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78D49: 48 8D 15 48 65 CC  lea         rdx,[0000000142A3F298h]
                    01
  0000000140D78D50: 48 8D 8C 24 98 01  lea         rcx,[rsp+0000000000000198h]
                    00 00
  0000000140D78D58: FF 15 22 B7 08 01  call        qword ptr [0000000141E04480h]
  0000000140D78D5E: 90                 nop
  0000000140D78D5F: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78D66: 0D 00 04 00 00     or          eax,400h
  0000000140D78D6B: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78D72: 48 8D 94 24 98 01  lea         rdx,[rsp+0000000000000198h]
                    00 00
  0000000140D78D7A: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  0000000140D78D82: FF 15 08 28 09 01  call        qword ptr [0000000141E0B590h]
  0000000140D78D88: 48 89 84 24 20 03  mov         qword ptr [rsp+0000000000000320h],rax
                    00 00
  0000000140D78D90: 48 8B 84 24 20 03  mov         rax,qword ptr [rsp+0000000000000320h]
                    00 00
  0000000140D78D98: 48 89 84 24 28 03  mov         qword ptr [rsp+0000000000000328h],rax
                    00 00
  0000000140D78DA0: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78DA7: 0D 00 08 00 00     or          eax,800h
  0000000140D78DAC: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78DB3: 4C 8D 84 24 88 01  lea         r8,[rsp+0000000000000188h]
                    00 00
  0000000140D78DBB: 48 8B 94 24 28 03  mov         rdx,qword ptr [rsp+0000000000000328h]
                    00 00
  0000000140D78DC3: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  0000000140D78DCB: FF 15 AF CC 08 01  call        qword ptr [0000000141E05A80h]
  0000000140D78DD1: 4C 8B 9C 24 80 01  mov         r11,qword ptr [rsp+0000000000000180h]
                    00 00
  0000000140D78DD9: 48 8D 05 38 5D 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D78DE0: 49 89 03           mov         qword ptr [r11],rax
  0000000140D78DE3: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  0000000140D78DEB: 48 89 84 24 30 03  mov         qword ptr [rsp+0000000000000330h],rax
                    00 00
  0000000140D78DF3: EB 0C              jmp         0000000140D78E01
  0000000140D78DF5: 48 C7 84 24 30 03  mov         qword ptr [rsp+0000000000000330h],0
                    00 00 00 00 00 00
  0000000140D78E01: 48 8B 84 24 30 03  mov         rax,qword ptr [rsp+0000000000000330h]
                    00 00
  0000000140D78E09: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  0000000140D78E11: 48 8B 84 24 78 01  mov         rax,qword ptr [rsp+0000000000000178h]
                    00 00
  0000000140D78E19: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140D78E21: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78E28: 25 00 08 00 00     and         eax,800h
  0000000140D78E2D: 85 C0              test        eax,eax
  0000000140D78E2F: 74 1A              je          0000000140D78E4B
  0000000140D78E31: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFF7FFh
                    00 FF F7 FF FF
  0000000140D78E3C: 48 8D 8C 24 90 01  lea         rcx,[rsp+0000000000000190h]
                    00 00
  0000000140D78E44: FF 15 56 28 09 01  call        qword ptr [0000000141E0B6A0h]
  0000000140D78E4A: 90                 nop
  0000000140D78E4B: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78E52: 25 00 04 00 00     and         eax,400h
  0000000140D78E57: 85 C0              test        eax,eax
  0000000140D78E59: 74 1A              je          0000000140D78E75
  0000000140D78E5B: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFBFFh
                    00 FF FB FF FF
  0000000140D78E66: 48 8D 8C 24 98 01  lea         rcx,[rsp+0000000000000198h]
                    00 00
  0000000140D78E6E: FF 15 6C CB 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D78E74: 90                 nop
  0000000140D78E75: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78E7C: 25 00 02 00 00     and         eax,200h
  0000000140D78E81: 85 C0              test        eax,eax
  0000000140D78E83: 74 19              je          0000000140D78E9E
  0000000140D78E85: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFFDFFh
                    00 FF FD FF FF
  0000000140D78E90: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140D78E98: FF 15 42 CB 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D78E9E: 45 33 C0           xor         r8d,r8d
  0000000140D78EA1: 48 8D 15 18 64 CC  lea         rdx,[0000000142A3F2C0h]
                    01
  0000000140D78EA8: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  0000000140D78EB0: E8 0B 37 00 00     call        0000000140D7C5C0
  0000000140D78EB5: 48 89 84 24 38 03  mov         qword ptr [rsp+0000000000000338h],rax
                    00 00
  0000000140D78EBD: 48 8B 84 24 38 03  mov         rax,qword ptr [rsp+0000000000000338h]
                    00 00
  0000000140D78EC5: 48 89 84 24 40 03  mov         qword ptr [rsp+0000000000000340h],rax
                    00 00
  0000000140D78ECD: 48 8B 94 24 40 03  mov         rdx,qword ptr [rsp+0000000000000340h]
                    00 00
  0000000140D78ED5: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D78EDD: FF 15 FD 0E 09 01  call        qword ptr [0000000141E09DE0h]
  0000000140D78EE3: 90                 nop
  0000000140D78EE4: 48 8D 8C 24 A0 01  lea         rcx,[rsp+00000000000001A0h]
                    00 00
  0000000140D78EEC: FF 15 EE CA 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D78EF2: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D78EFA: BA 04 00 00 00     mov         edx,4
  0000000140D78EFF: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D78F07: FF 15 2B F2 08 01  call        qword ptr [0000000141E08138h]
  0000000140D78F0D: 0F B6 44 24 61     movzx       eax,byte ptr [rsp+61h]
  0000000140D78F12: 85 C0              test        eax,eax
  0000000140D78F14: 75 0D              jne         0000000140D78F23
  0000000140D78F16: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  0000000140D78F1B: 85 C0              test        eax,eax
  0000000140D78F1D: 0F 84 0E 02 00 00  je          0000000140D79131
  0000000140D78F23: B9 10 00 00 00     mov         ecx,10h
  0000000140D78F28: E8 8F 5B 6F 00     call        000000014146EABC
  0000000140D78F2D: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  0000000140D78F35: 48 83 BC 24 B0 01  cmp         qword ptr [rsp+00000000000001B0h],0
                    00 00 00
  0000000140D78F3E: 0F 84 D5 00 00 00  je          0000000140D79019
  0000000140D78F44: 48 8D 15 94 2C CC  lea         rdx,[0000000142A3BBDFh]
                    01
  0000000140D78F4B: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  0000000140D78F53: FF 15 27 B5 08 01  call        qword ptr [0000000141E04480h]
  0000000140D78F59: 90                 nop
  0000000140D78F5A: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78F61: 0D 00 10 00 00     or          eax,1000h
  0000000140D78F66: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78F6D: 48 8D 15 5C 63 CC  lea         rdx,[0000000142A3F2D0h]
                    01
  0000000140D78F74: 48 8D 8C 24 C8 01  lea         rcx,[rsp+00000000000001C8h]
                    00 00
  0000000140D78F7C: FF 15 FE B4 08 01  call        qword ptr [0000000141E04480h]
  0000000140D78F82: 90                 nop
  0000000140D78F83: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78F8A: 0D 00 20 00 00     or          eax,2000h
  0000000140D78F8F: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78F96: 48 8D 94 24 C8 01  lea         rdx,[rsp+00000000000001C8h]
                    00 00
  0000000140D78F9E: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  0000000140D78FA6: FF 15 E4 25 09 01  call        qword ptr [0000000141E0B590h]
  0000000140D78FAC: 48 89 84 24 48 03  mov         qword ptr [rsp+0000000000000348h],rax
                    00 00
  0000000140D78FB4: 48 8B 84 24 48 03  mov         rax,qword ptr [rsp+0000000000000348h]
                    00 00
  0000000140D78FBC: 48 89 84 24 50 03  mov         qword ptr [rsp+0000000000000350h],rax
                    00 00
  0000000140D78FC4: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D78FCB: 0D 00 40 00 00     or          eax,4000h
  0000000140D78FD0: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D78FD7: 4C 8D 84 24 B8 01  lea         r8,[rsp+00000000000001B8h]
                    00 00
  0000000140D78FDF: 48 8B 94 24 50 03  mov         rdx,qword ptr [rsp+0000000000000350h]
                    00 00
  0000000140D78FE7: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D78FEF: FF 15 8B CA 08 01  call        qword ptr [0000000141E05A80h]
  0000000140D78FF5: 4C 8B 9C 24 B0 01  mov         r11,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D78FFD: 48 8D 05 14 5B 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D79004: 49 89 03           mov         qword ptr [r11],rax
  0000000140D79007: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D7900F: 48 89 84 24 58 03  mov         qword ptr [rsp+0000000000000358h],rax
                    00 00
  0000000140D79017: EB 0C              jmp         0000000140D79025
  0000000140D79019: 48 C7 84 24 58 03  mov         qword ptr [rsp+0000000000000358h],0
                    00 00 00 00 00 00
  0000000140D79025: 48 8B 84 24 58 03  mov         rax,qword ptr [rsp+0000000000000358h]
                    00 00
  0000000140D7902D: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  0000000140D79035: 48 8B 84 24 A8 01  mov         rax,qword ptr [rsp+00000000000001A8h]
                    00 00
  0000000140D7903D: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140D79045: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D7904C: 25 00 40 00 00     and         eax,4000h
  0000000140D79051: 85 C0              test        eax,eax
  0000000140D79053: 74 1A              je          0000000140D7906F
  0000000140D79055: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFBFFFh
                    00 FF BF FF FF
  0000000140D79060: 48 8D 8C 24 C0 01  lea         rcx,[rsp+00000000000001C0h]
                    00 00
  0000000140D79068: FF 15 32 26 09 01  call        qword ptr [0000000141E0B6A0h]
  0000000140D7906E: 90                 nop
  0000000140D7906F: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D79076: 25 00 20 00 00     and         eax,2000h
  0000000140D7907B: 85 C0              test        eax,eax
  0000000140D7907D: 74 1A              je          0000000140D79099
  0000000140D7907F: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFDFFFh
                    00 FF DF FF FF
  0000000140D7908A: 48 8D 8C 24 C8 01  lea         rcx,[rsp+00000000000001C8h]
                    00 00
  0000000140D79092: FF 15 48 C9 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D79098: 90                 nop
  0000000140D79099: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D790A0: 25 00 10 00 00     and         eax,1000h
  0000000140D790A5: 85 C0              test        eax,eax
  0000000140D790A7: 74 19              je          0000000140D790C2
  0000000140D790A9: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFFEFFFh
                    00 FF EF FF FF
  0000000140D790B4: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  0000000140D790BC: FF 15 1E C9 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D790C2: 45 33 C0           xor         r8d,r8d
  0000000140D790C5: 48 8D 15 34 62 CC  lea         rdx,[0000000142A3F300h]
                    01
  0000000140D790CC: 48 8D 8C 24 D0 01  lea         rcx,[rsp+00000000000001D0h]
                    00 00
  0000000140D790D4: E8 E7 34 00 00     call        0000000140D7C5C0
  0000000140D790D9: 48 89 84 24 60 03  mov         qword ptr [rsp+0000000000000360h],rax
                    00 00
  0000000140D790E1: 48 8B 84 24 60 03  mov         rax,qword ptr [rsp+0000000000000360h]
                    00 00
  0000000140D790E9: 48 89 84 24 68 03  mov         qword ptr [rsp+0000000000000368h],rax
                    00 00
  0000000140D790F1: 48 8B 94 24 68 03  mov         rdx,qword ptr [rsp+0000000000000368h]
                    00 00
  0000000140D790F9: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D79101: FF 15 D9 0C 09 01  call        qword ptr [0000000141E09DE0h]
  0000000140D79107: 90                 nop
  0000000140D79108: 48 8D 8C 24 D0 01  lea         rcx,[rsp+00000000000001D0h]
                    00 00
  0000000140D79110: FF 15 CA C8 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D79116: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D7911E: BA 05 00 00 00     mov         edx,5
  0000000140D79123: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D7912B: FF 15 07 F0 08 01  call        qword ptr [0000000141E08138h]
  0000000140D79131: 0F B6 44 24 61     movzx       eax,byte ptr [rsp+61h]
  0000000140D79136: 85 C0              test        eax,eax
  0000000140D79138: 0F 84 0E 02 00 00  je          0000000140D7934C
  0000000140D7913E: B9 10 00 00 00     mov         ecx,10h
  0000000140D79143: E8 74 59 6F 00     call        000000014146EABC
  0000000140D79148: 48 89 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],rax
                    00 00
  0000000140D79150: 48 83 BC 24 E0 01  cmp         qword ptr [rsp+00000000000001E0h],0
                    00 00 00
  0000000140D79159: 0F 84 D5 00 00 00  je          0000000140D79234
  0000000140D7915F: 48 8D 15 87 2A CC  lea         rdx,[0000000142A3BBEDh]
                    01
  0000000140D79166: 48 8D 8C 24 E8 01  lea         rcx,[rsp+00000000000001E8h]
                    00 00
  0000000140D7916E: FF 15 0C B3 08 01  call        qword ptr [0000000141E04480h]
  0000000140D79174: 90                 nop
  0000000140D79175: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D7917C: 0D 00 80 00 00     or          eax,8000h
  0000000140D79181: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D79188: 48 8D 15 81 61 CC  lea         rdx,[0000000142A3F310h]
                    01
  0000000140D7918F: 48 8D 8C 24 F8 01  lea         rcx,[rsp+00000000000001F8h]
                    00 00
  0000000140D79197: FF 15 E3 B2 08 01  call        qword ptr [0000000141E04480h]
  0000000140D7919D: 90                 nop
  0000000140D7919E: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D791A5: 0D 00 00 01 00     or          eax,10000h
  0000000140D791AA: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D791B1: 48 8D 94 24 F8 01  lea         rdx,[rsp+00000000000001F8h]
                    00 00
  0000000140D791B9: 48 8D 8C 24 F0 01  lea         rcx,[rsp+00000000000001F0h]
                    00 00
  0000000140D791C1: FF 15 C9 23 09 01  call        qword ptr [0000000141E0B590h]
  0000000140D791C7: 48 89 84 24 70 03  mov         qword ptr [rsp+0000000000000370h],rax
                    00 00
  0000000140D791CF: 48 8B 84 24 70 03  mov         rax,qword ptr [rsp+0000000000000370h]
                    00 00
  0000000140D791D7: 48 89 84 24 78 03  mov         qword ptr [rsp+0000000000000378h],rax
                    00 00
  0000000140D791DF: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D791E6: 0D 00 00 02 00     or          eax,20000h
  0000000140D791EB: 89 84 24 98 02 00  mov         dword ptr [rsp+0000000000000298h],eax
                    00
  0000000140D791F2: 4C 8D 84 24 E8 01  lea         r8,[rsp+00000000000001E8h]
                    00 00
  0000000140D791FA: 48 8B 94 24 78 03  mov         rdx,qword ptr [rsp+0000000000000378h]
                    00 00
  0000000140D79202: 48 8B 8C 24 E0 01  mov         rcx,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140D7920A: FF 15 70 C8 08 01  call        qword ptr [0000000141E05A80h]
  0000000140D79210: 4C 8B 9C 24 E0 01  mov         r11,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140D79218: 48 8D 05 F9 58 39  lea         rax,[000000014210EB18h]
                    01
  0000000140D7921F: 49 89 03           mov         qword ptr [r11],rax
  0000000140D79222: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140D7922A: 48 89 84 24 80 03  mov         qword ptr [rsp+0000000000000380h],rax
                    00 00
  0000000140D79232: EB 0C              jmp         0000000140D79240
  0000000140D79234: 48 C7 84 24 80 03  mov         qword ptr [rsp+0000000000000380h],0
                    00 00 00 00 00 00
  0000000140D79240: 48 8B 84 24 80 03  mov         rax,qword ptr [rsp+0000000000000380h]
                    00 00
  0000000140D79248: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  0000000140D79250: 48 8B 84 24 D8 01  mov         rax,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140D79258: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140D79260: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D79267: 25 00 00 02 00     and         eax,20000h
  0000000140D7926C: 85 C0              test        eax,eax
  0000000140D7926E: 74 1A              je          0000000140D7928A
  0000000140D79270: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFDFFFFh
                    00 FF FF FD FF
  0000000140D7927B: 48 8D 8C 24 F0 01  lea         rcx,[rsp+00000000000001F0h]
                    00 00
  0000000140D79283: FF 15 17 24 09 01  call        qword ptr [0000000141E0B6A0h]
  0000000140D79289: 90                 nop
  0000000140D7928A: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D79291: 25 00 00 01 00     and         eax,10000h
  0000000140D79296: 85 C0              test        eax,eax
  0000000140D79298: 74 1A              je          0000000140D792B4
  0000000140D7929A: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFEFFFFh
                    00 FF FF FE FF
  0000000140D792A5: 48 8D 8C 24 F8 01  lea         rcx,[rsp+00000000000001F8h]
                    00 00
  0000000140D792AD: FF 15 2D C7 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D792B3: 90                 nop
  0000000140D792B4: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  0000000140D792BB: 25 00 80 00 00     and         eax,8000h
  0000000140D792C0: 85 C0              test        eax,eax
  0000000140D792C2: 74 19              je          0000000140D792DD
  0000000140D792C4: 81 A4 24 98 02 00  and         dword ptr [rsp+0000000000000298h],0FFFF7FFFh
                    00 FF 7F FF FF
  0000000140D792CF: 48 8D 8C 24 E8 01  lea         rcx,[rsp+00000000000001E8h]
                    00 00
  0000000140D792D7: FF 15 03 C7 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D792DD: 45 33 C0           xor         r8d,r8d
  0000000140D792E0: 48 8D 15 55 60 CC  lea         rdx,[0000000142A3F33Ch]
                    01
  0000000140D792E7: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  0000000140D792EF: E8 CC 32 00 00     call        0000000140D7C5C0
  0000000140D792F4: 48 89 84 24 88 03  mov         qword ptr [rsp+0000000000000388h],rax
                    00 00
  0000000140D792FC: 48 8B 84 24 88 03  mov         rax,qword ptr [rsp+0000000000000388h]
                    00 00
  0000000140D79304: 48 89 84 24 90 03  mov         qword ptr [rsp+0000000000000390h],rax
                    00 00
  0000000140D7930C: 48 8B 94 24 90 03  mov         rdx,qword ptr [rsp+0000000000000390h]
                    00 00
  0000000140D79314: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D7931C: FF 15 BE 0A 09 01  call        qword ptr [0000000141E09DE0h]
  0000000140D79322: 90                 nop
  0000000140D79323: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  0000000140D7932B: FF 15 AF C6 08 01  call        qword ptr [0000000141E059E0h]
  0000000140D79331: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140D79339: BA 06 00 00 00     mov         edx,6
  0000000140D7933E: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D79346: FF 15 EC ED 08 01  call        qword ptr [0000000141E08138h]
  0000000140D7934C: BA 01 00 00 00     mov         edx,1
  0000000140D79351: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D79359: E8 B2 87 FC FF     call        0000000140D41B10
  0000000140D7935E: 48 8B 84 24 F0 03  mov         rax,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D79366: 48 83 78 50 00     cmp         qword ptr [rax+50h],0
  0000000140D7936B: 75 0F              jne         0000000140D7937C
  0000000140D7936D: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D79372: E8 59 B5 28 FF     call        00000001400048D0
  0000000140D79377: E9 C6 03 00 00     jmp         0000000140D79742
  0000000140D7937C: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D79384: FF 15 B6 22 09 01  call        qword ptr [0000000141E0B640h]
  0000000140D7938A: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140D7938F: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  0000000140D79398: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140D7939D: 48 8B 94 24 F0 03  mov         rdx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D793A5: 48 8B 52 50        mov         rdx,qword ptr [rdx+50h]
  0000000140D793A9: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D793B1: E8 BA 68 FC FF     call        0000000140D3FC70
  0000000140D793B6: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140D793BB: 4C 8B 44 24 78     mov         r8,qword ptr [rsp+78h]
  0000000140D793C0: 48 8B 94 24 F0 03  mov         rdx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D793C8: 48 8B 52 50        mov         rdx,qword ptr [rdx+50h]
  0000000140D793CC: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D793D4: E8 77 03 00 00     call        0000000140D79750
  0000000140D793D9: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D793E1: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D793E5: E8 16 1A F0 FF     call        0000000140C7AE00
  0000000140D793EA: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140D793EF: 48 83 7C 24 58 00  cmp         qword ptr [rsp+58h],0
  0000000140D793F5: 75 0F              jne         0000000140D79406
  0000000140D793F7: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D793FC: E8 CF B4 28 FF     call        00000001400048D0
  0000000140D79401: E9 3C 03 00 00     jmp         0000000140D79742
  0000000140D79406: 48 8D 84 24 08 02  lea         rax,[rsp+0000000000000208h]
                    00 00
  0000000140D7940E: 48 89 84 24 10 02  mov         qword ptr [rsp+0000000000000210h],rax
                    00 00
  0000000140D79416: 48 8D 15 27 5F CC  lea         rdx,[0000000142A3F344h]
                    01
  0000000140D7941D: 48 8B 8C 24 10 02  mov         rcx,qword ptr [rsp+0000000000000210h]
                    00 00
  0000000140D79425: FF 15 55 B0 08 01  call        qword ptr [0000000141E04480h]
  0000000140D7942B: 48 89 84 24 98 03  mov         qword ptr [rsp+0000000000000398h],rax
                    00 00
  0000000140D79433: 48 8B 94 24 98 03  mov         rdx,qword ptr [rsp+0000000000000398h]
                    00 00
  0000000140D7943B: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D79440: E8 7B 17 F8 FF     call        0000000140CFABC0
  0000000140D79445: 88 84 24 8C 00 00  mov         byte ptr [rsp+000000000000008Ch],al
                    00
  0000000140D7944C: 0F B6 84 24 8C 00  movzx       eax,byte ptr [rsp+000000000000008Ch]
                    00 00
  0000000140D79454: 85 C0              test        eax,eax
  0000000140D79456: 0F 84 80 00 00 00  je          0000000140D794DC
  0000000140D7945C: 48 8D 84 24 18 02  lea         rax,[rsp+0000000000000218h]
                    00 00
  0000000140D79464: 48 89 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],rax
                    00 00
  0000000140D7946C: 48 8D 15 D9 5E CC  lea         rdx,[0000000142A3F34Ch]
                    01
  0000000140D79473: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140D7947B: FF 15 FF AF 08 01  call        qword ptr [0000000141E04480h]
  0000000140D79481: 48 89 84 24 A0 03  mov         qword ptr [rsp+00000000000003A0h],rax
                    00 00
  0000000140D79489: 48 8B 94 24 A0 03  mov         rdx,qword ptr [rsp+00000000000003A0h]
                    00 00
  0000000140D79491: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D79496: E8 A5 0B F8 FF     call        0000000140CFA040
  0000000140D7949B: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140D794A3: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140D794A8: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140D794B0: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D794B8: E8 B3 67 FC FF     call        0000000140D3FC70
  0000000140D794BD: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140D794C2: 4C 8B 44 24 78     mov         r8,qword ptr [rsp+78h]
  0000000140D794C7: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140D794CF: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D794D7: E8 74 02 00 00     call        0000000140D79750
  0000000140D794DC: 48 8D 84 24 28 02  lea         rax,[rsp+0000000000000228h]
                    00 00
  0000000140D794E4: 48 89 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],rax
                    00 00
  0000000140D794EC: 48 8D 15 65 5E CC  lea         rdx,[0000000142A3F358h]
                    01
  0000000140D794F3: 48 8B 8C 24 30 02  mov         rcx,qword ptr [rsp+0000000000000230h]
                    00 00
  0000000140D794FB: FF 15 7F AF 08 01  call        qword ptr [0000000141E04480h]
  0000000140D79501: 48 89 84 24 A8 03  mov         qword ptr [rsp+00000000000003A8h],rax
                    00 00
  0000000140D79509: 48 8B 94 24 A8 03  mov         rdx,qword ptr [rsp+00000000000003A8h]
                    00 00
  0000000140D79511: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D79516: E8 A5 16 F8 FF     call        0000000140CFABC0
  0000000140D7951B: 88 44 24 60        mov         byte ptr [rsp+60h],al
  0000000140D7951F: 0F B6 44 24 60     movzx       eax,byte ptr [rsp+60h]
  0000000140D79524: 85 C0              test        eax,eax
  0000000140D79526: 0F 84 80 00 00 00  je          0000000140D795AC
  0000000140D7952C: 48 8D 84 24 38 02  lea         rax,[rsp+0000000000000238h]
                    00 00
  0000000140D79534: 48 89 84 24 40 02  mov         qword ptr [rsp+0000000000000240h],rax
                    00 00
  0000000140D7953C: 48 8D 15 1D 5E CC  lea         rdx,[0000000142A3F360h]
                    01
  0000000140D79543: 48 8B 8C 24 40 02  mov         rcx,qword ptr [rsp+0000000000000240h]
                    00 00
  0000000140D7954B: FF 15 2F AF 08 01  call        qword ptr [0000000141E04480h]
  0000000140D79551: 48 89 84 24 B0 03  mov         qword ptr [rsp+00000000000003B0h],rax
                    00 00
  0000000140D79559: 48 8B 94 24 B0 03  mov         rdx,qword ptr [rsp+00000000000003B0h]
                    00 00
  0000000140D79561: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140D79566: E8 D5 0A F8 FF     call        0000000140CFA040
  0000000140D7956B: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140D79573: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140D79578: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140D79580: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D79588: E8 E3 66 FC FF     call        0000000140D3FC70
  0000000140D7958D: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140D79592: 4C 8B 44 24 78     mov         r8,qword ptr [rsp+78h]
  0000000140D79597: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140D7959F: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D795A7: E8 A4 01 00 00     call        0000000140D79750
  0000000140D795AC: B2 01              mov         dl,1
  0000000140D795AE: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D795B6: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D795BA: E8 E1 1B F0 FF     call        0000000140C7B1A0
  0000000140D795BF: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  0000000140D795C6: 44 8B 84 24 88 00  mov         r8d,dword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D795CE: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140D795D3: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D795DB: E8 B0 1B 00 00     call        0000000140D7B190
  0000000140D795E0: 90                 nop
  0000000140D795E1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140D795E6: FF 15 6C C3 08 01  call        qword ptr [0000000141E05958h]
  0000000140D795EC: 0F B6 C0           movzx       eax,al
  0000000140D795EF: 85 C0              test        eax,eax
  0000000140D795F1: 74 45              je          0000000140D79638
  0000000140D795F3: 48 8D 84 24 48 02  lea         rax,[rsp+0000000000000248h]
                    00 00
  0000000140D795FB: 48 89 84 24 60 02  mov         qword ptr [rsp+0000000000000260h],rax
                    00 00
  0000000140D79603: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140D79608: 48 8B 8C 24 60 02  mov         rcx,qword ptr [rsp+0000000000000260h]
                    00 00
  0000000140D79610: FF 15 3A C3 08 01  call        qword ptr [0000000141E05950h]
  0000000140D79616: 48 89 84 24 B8 03  mov         qword ptr [rsp+00000000000003B8h],rax
                    00 00
  0000000140D7961E: 48 8B 94 24 B8 03  mov         rdx,qword ptr [rsp+00000000000003B8h]
                    00 00
  0000000140D79626: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D7962E: E8 7D 16 00 00     call        0000000140D7ACB0
  0000000140D79633: E9 F4 00 00 00     jmp         0000000140D7972C
  0000000140D79638: C7 84 24 A8 00 00  mov         dword ptr [rsp+00000000000000A8h],1
                    00 01 00 00 00
  0000000140D79643: 48 8D 94 24 A8 00  lea         rdx,[rsp+00000000000000A8h]
                    00 00
  0000000140D7964B: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D79653: E8 88 15 00 00     call        0000000140D7ABE0
  0000000140D79658: 48 8D 8C 24 68 02  lea         rcx,[rsp+0000000000000268h]
                    00 00
  0000000140D79660: FF 15 6A C2 08 01  call        qword ptr [0000000141E058D0h]
  0000000140D79666: 48 89 84 24 C0 03  mov         qword ptr [rsp+00000000000003C0h],rax
                    00 00
  0000000140D7966E: 48 8B 84 24 C0 03  mov         rax,qword ptr [rsp+00000000000003C0h]
                    00 00
  0000000140D79676: 48 89 84 24 C8 03  mov         qword ptr [rsp+00000000000003C8h],rax
                    00 00
  0000000140D7967E: 48 8B 84 24 C8 03  mov         rax,qword ptr [rsp+00000000000003C8h]
                    00 00
  0000000140D79686: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D7968B: 41 B9 01 00 00 00  mov         r9d,1
  0000000140D79691: 44 8B 84 24 A8 00  mov         r8d,dword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140D79699: 48 8D 94 24 80 02  lea         rdx,[rsp+0000000000000280h]
                    00 00
  0000000140D796A1: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D796A9: FF 15 C1 20 09 01  call        qword ptr [0000000141E0B770h]
  0000000140D796AF: 48 89 84 24 D0 03  mov         qword ptr [rsp+00000000000003D0h],rax
                    00 00
  0000000140D796B7: 48 8B 84 24 D0 03  mov         rax,qword ptr [rsp+00000000000003D0h]
                    00 00
  0000000140D796BF: 48 89 84 24 D8 03  mov         qword ptr [rsp+00000000000003D8h],rax
                    00 00
  0000000140D796C7: 45 33 C9           xor         r9d,r9d
  0000000140D796CA: 45 33 C0           xor         r8d,r8d
  0000000140D796CD: 48 8B 94 24 D8 03  mov         rdx,qword ptr [rsp+00000000000003D8h]
                    00 00
  0000000140D796D5: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D796DD: E8 7E 17 00 00     call        0000000140D7AE60
  0000000140D796E2: 89 84 24 AC 00 00  mov         dword ptr [rsp+00000000000000ACh],eax
                    00
  0000000140D796E9: 48 8D 8C 24 80 02  lea         rcx,[rsp+0000000000000280h]
                    00 00
  0000000140D796F1: FF 15 E9 A7 08 01  call        qword ptr [0000000141E03EE0h]
  0000000140D796F7: 90                 nop
  0000000140D796F8: 48 8D 8C 24 68 02  lea         rcx,[rsp+0000000000000268h]
                    00 00
  0000000140D79700: FF 15 DA A7 08 01  call        qword ptr [0000000141E03EE0h]
  0000000140D79706: 83 BC 24 AC 00 00  cmp         dword ptr [rsp+00000000000000ACh],0
                    00 00
  0000000140D7970E: 7E 1C              jle         0000000140D7972C
  0000000140D79710: 45 33 C0           xor         r8d,r8d
  0000000140D79713: 8B 94 24 AC 00 00  mov         edx,dword ptr [rsp+00000000000000ACh]
                    00
  0000000140D7971A: 48 8B 8C 24 F0 03  mov         rcx,qword ptr [rsp+00000000000003F0h]
                    00 00
  0000000140D79722: 48 8B 49 50        mov         rcx,qword ptr [rcx+50h]
  0000000140D79726: E8 C5 1A F0 FF     call        0000000140C7B1F0
  0000000140D7972B: 90                 nop
  0000000140D7972C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140D79731: FF 15 A9 A7 08 01  call        qword ptr [0000000141E03EE0h]
  0000000140D79737: 90                 nop
  0000000140D79738: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D7973D: E8 8E B1 28 FF     call        00000001400048D0
  0000000140D79742: 48 81 C4 E8 03 00  add         rsp,3E8h
                    00
  0000000140D79749: C3                 ret

  Summary

     1E02000 .text

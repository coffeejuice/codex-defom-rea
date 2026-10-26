Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D0E540: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000140D0E545: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140D0E54A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D0E54F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D0E554: 48 81 EC C8 02 00  sub         rsp,2C8h
                    00
  0000000140D0E55B: 48 C7 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140D0E567: C7 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],0
                    00 00 00 00 00
  0000000140D0E572: C7 44 24 68 00 00  mov         dword ptr [rsp+68h],0
                    00 00
  0000000140D0E57A: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140D0E57E: F2 0F 11 84 24 80  movsd       mmword ptr [rsp+0000000000000080h],xmm0
                    00 00 00
  0000000140D0E587: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140D0E58B: F2 0F 11 44 24 60  movsd       mmword ptr [rsp+60h],xmm0
  0000000140D0E591: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  0000000140D0E59D: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  0000000140D0E5A6: 48 8B 8C 24 D0 02  mov         rcx,qword ptr [rsp+00000000000002D0h]
                    00 00
  0000000140D0E5AE: E8 9D C6 FE FF     call        0000000140CFAC50
  0000000140D0E5B3: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D0E5B8: 48 8B 8C 24 D0 02  mov         rcx,qword ptr [rsp+00000000000002D0h]
                    00 00
  0000000140D0E5C0: E8 9B B3 FE FF     call        0000000140CF9960
  0000000140D0E5C5: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140D0E5CA: 48 8B 8C 24 D0 02  mov         rcx,qword ptr [rsp+00000000000002D0h]
                    00 00
  0000000140D0E5D2: E8 19 AF FE FF     call        0000000140CF94F0
  0000000140D0E5D7: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D0E5DC: C6 44 24 58 00     mov         byte ptr [rsp+58h],0
  0000000140D0E5E1: B2 01              mov         dl,1
  0000000140D0E5E3: 48 8B 8C 24 D0 02  mov         rcx,qword ptr [rsp+00000000000002D0h]
                    00 00
  0000000140D0E5EB: E8 60 E5 FF FF     call        0000000140D0CB50
  0000000140D0E5F0: 48 85 C0           test        rax,rax
  0000000140D0E5F3: 74 1B              je          0000000140D0E610
  0000000140D0E5F5: B2 01              mov         dl,1
  0000000140D0E5F7: 48 8B 8C 24 D0 02  mov         rcx,qword ptr [rsp+00000000000002D0h]
                    00 00
  0000000140D0E5FF: E8 4C E5 FF FF     call        0000000140D0CB50
  0000000140D0E604: 48 8B C8           mov         rcx,rax
  0000000140D0E607: E8 B4 83 03 00     call        0000000140D469C0
  0000000140D0E60C: 88 44 24 58        mov         byte ptr [rsp+58h],al
  0000000140D0E610: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  0000000140D0E616: 74 3D              je          0000000140D0E655
  0000000140D0E618: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140D0E61D: E8 BE DA F6 FF     call        0000000140C7C0E0
  0000000140D0E622: 0F B6 C0           movzx       eax,al
  0000000140D0E625: 85 C0              test        eax,eax
  0000000140D0E627: 74 2C              je          0000000140D0E655
  0000000140D0E629: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140D0E62F: 74 24              je          0000000140D0E655
  0000000140D0E631: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  0000000140D0E636: E8 65 DA F6 FF     call        0000000140C7C0A0
  0000000140D0E63B: 0F B6 C0           movzx       eax,al
  0000000140D0E63E: 85 C0              test        eax,eax
  0000000140D0E640: 74 13              je          0000000140D0E655
  0000000140D0E642: 0F B6 44 24 58     movzx       eax,byte ptr [rsp+58h]
  0000000140D0E647: 85 C0              test        eax,eax
  0000000140D0E649: 74 0A              je          0000000140D0E655
  0000000140D0E64B: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000140D0E650: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D0E655: C7 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00
  0000000140D0E660: 48 8D 0D 51 80 06  lea         rcx,[0000000143D766B8h]
                    03
  0000000140D0E667: E8 24 9A 5B FF     call        00000001402C8090
  0000000140D0E66C: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D0E671: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140D0E677: 0F 84 98 05 00 00  je          0000000140D0EC15
  0000000140D0E67D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0E682: E8 29 1F F8 FF     call        0000000140C905B0
  0000000140D0E687: 48 8B C8           mov         rcx,rax
  0000000140D0E68A: E8 51 65 F0 FF     call        0000000140C14BE0
  0000000140D0E68F: 89 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],eax
                    00
  0000000140D0E696: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0E69B: E8 10 1F F8 FF     call        0000000140C905B0
  0000000140D0E6A0: 4C 8B 8C 24 E8 02  mov         r9,qword ptr [rsp+00000000000002E8h]
                    00 00
  0000000140D0E6A8: 4C 8B 84 24 E0 02  mov         r8,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140D0E6B0: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D0E6B8: 48 8B C8           mov         rcx,rax
  0000000140D0E6BB: E8 C0 74 00 00     call        0000000140D15B80
  0000000140D0E6C0: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140D0E6C8: 0F 85 D3 04 00 00  jne         0000000140D0EBA1
  0000000140D0E6CE: 48 8D 94 24 F8 00  lea         rdx,[rsp+00000000000000F8h]
                    00 00
  0000000140D0E6D6: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0E6DB: E8 D0 45 F7 FF     call        0000000140C82CB0
  0000000140D0E6E0: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  0000000140D0E6E8: 48 8B 84 24 98 01  mov         rax,qword ptr [rsp+0000000000000198h]
                    00 00
  0000000140D0E6F0: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  0000000140D0E6F8: 48 8B 94 24 A0 01  mov         rdx,qword ptr [rsp+00000000000001A0h]
                    00 00
  0000000140D0E700: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140D0E708: E8 E3 F3 35 FF     call        000000014006DAF0
  0000000140D0E70D: 90                 nop
  0000000140D0E70E: 48 8D 84 24 98 00  lea         rax,[rsp+0000000000000098h]
                    00 00
  0000000140D0E716: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140D0E71E: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  0000000140D0E726: E8 75 9A 4F FF     call        00000001402081A0
  0000000140D0E72B: E9 AA 00 00 00     jmp         0000000140D0E7DA
  0000000140D0E730: 33 C0              xor         eax,eax
  0000000140D0E732: 83 F8 01           cmp         eax,1
  0000000140D0E735: 74 0E              je          0000000140D0E745
  0000000140D0E737: 48 C7 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],0
                    00 00 00 00 00 00
  0000000140D0E743: EB 50              jmp         0000000140D0E795
  0000000140D0E745: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  0000000140D0E74D: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0E752: E8 59 45 F7 FF     call        0000000140C82CB0
  0000000140D0E757: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  0000000140D0E75F: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000140D0E767: 48 89 84 24 B8 01  mov         qword ptr [rsp+00000000000001B8h],rax
                    00 00
  0000000140D0E76F: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0E776: 83 C8 01           or          eax,1
  0000000140D0E779: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140D0E780: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  0000000140D0E788: E8 C3 64 47 FF     call        0000000140184C50
  0000000140D0E78D: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  0000000140D0E795: 48 8B 94 24 A8 01  mov         rdx,qword ptr [rsp+00000000000001A8h]
                    00 00
  0000000140D0E79D: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D0E7A5: E8 D6 DB 0E 00     call        0000000140DFC380
  0000000140D0E7AA: 48 8B C8           mov         rcx,rax
  0000000140D0E7AD: 48 83 C1 10        add         rcx,10h
  0000000140D0E7B1: E8 4A B0 0D 00     call        0000000140DE9800
  0000000140D0E7B6: 90                 nop
  0000000140D0E7B7: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0E7BE: 83 E0 01           and         eax,1
  0000000140D0E7C1: 85 C0              test        eax,eax
  0000000140D0E7C3: 74 15              je          0000000140D0E7DA
  0000000140D0E7C5: 83 A4 24 88 01 00  and         dword ptr [rsp+0000000000000188h],0FFFFFFFEh
                    00 FE
  0000000140D0E7CD: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  0000000140D0E7D5: E8 C6 99 4F FF     call        00000001402081A0
  0000000140D0E7DA: 33 C0              xor         eax,eax
  0000000140D0E7DC: 83 F8 01           cmp         eax,1
  0000000140D0E7DF: 74 0E              je          0000000140D0E7EF
  0000000140D0E7E1: 48 C7 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],0
                    00 00 00 00 00 00
  0000000140D0E7ED: EB 50              jmp         0000000140D0E83F
  0000000140D0E7EF: 48 8D 94 24 10 01  lea         rdx,[rsp+0000000000000110h]
                    00 00
  0000000140D0E7F7: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0E7FC: E8 AF 44 F7 FF     call        0000000140C82CB0
  0000000140D0E801: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  0000000140D0E809: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000140D0E811: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
  0000000140D0E819: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0E820: 83 C8 02           or          eax,2
  0000000140D0E823: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140D0E82A: 48 8B 8C 24 D0 01  mov         rcx,qword ptr [rsp+00000000000001D0h]
                    00 00
  0000000140D0E832: E8 19 64 47 FF     call        0000000140184C50
  0000000140D0E837: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  0000000140D0E83F: 48 8B 94 24 C0 01  mov         rdx,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140D0E847: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D0E84F: E8 2C DB 0E 00     call        0000000140DFC380
  0000000140D0E854: 48 8B C8           mov         rcx,rax
  0000000140D0E857: E8 24 D8 0E 00     call        0000000140DFC080
  0000000140D0E85C: 88 84 24 08 01 00  mov         byte ptr [rsp+0000000000000108h],al
                    00
  0000000140D0E863: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0E86A: 83 E0 02           and         eax,2
  0000000140D0E86D: 85 C0              test        eax,eax
  0000000140D0E86F: 74 15              je          0000000140D0E886
  0000000140D0E871: 83 A4 24 88 01 00  and         dword ptr [rsp+0000000000000188h],0FFFFFFFDh
                    00 FD
  0000000140D0E879: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  0000000140D0E881: E8 1A 99 4F FF     call        00000001402081A0
  0000000140D0E886: 0F B6 84 24 08 01  movzx       eax,byte ptr [rsp+0000000000000108h]
                    00 00
  0000000140D0E88E: 85 C0              test        eax,eax
  0000000140D0E890: 0F 84 C1 02 00 00  je          0000000140D0EB57
  0000000140D0E896: 33 C0              xor         eax,eax
  0000000140D0E898: 83 F8 01           cmp         eax,1
  0000000140D0E89B: 74 0E              je          0000000140D0E8AB
  0000000140D0E89D: 48 C7 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],0
                    00 00 00 00 00 00
  0000000140D0E8A9: EB 50              jmp         0000000140D0E8FB
  0000000140D0E8AB: 48 8D 94 24 18 01  lea         rdx,[rsp+0000000000000118h]
                    00 00
  0000000140D0E8B3: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0E8B8: E8 F3 43 F7 FF     call        0000000140C82CB0
  0000000140D0E8BD: 48 89 84 24 E0 01  mov         qword ptr [rsp+00000000000001E0h],rax
                    00 00
  0000000140D0E8C5: 48 8B 84 24 E0 01  mov         rax,qword ptr [rsp+00000000000001E0h]
                    00 00
  0000000140D0E8CD: 48 89 84 24 E8 01  mov         qword ptr [rsp+00000000000001E8h],rax
                    00 00
  0000000140D0E8D5: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0E8DC: 83 C8 04           or          eax,4
  0000000140D0E8DF: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140D0E8E6: 48 8B 8C 24 E8 01  mov         rcx,qword ptr [rsp+00000000000001E8h]
                    00 00
  0000000140D0E8EE: E8 5D 63 47 FF     call        0000000140184C50
  0000000140D0E8F3: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  0000000140D0E8FB: 48 8B 94 24 D8 01  mov         rdx,qword ptr [rsp+00000000000001D8h]
                    00 00
  0000000140D0E903: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D0E90B: E8 70 DA 0E 00     call        0000000140DFC380
  0000000140D0E910: 48 8B C8           mov         rcx,rax
  0000000140D0E913: 48 83 C1 10        add         rcx,10h
  0000000140D0E917: E8 E4 0A 94 FF     call        000000014064F400
  0000000140D0E91C: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D0E91F: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140D0E927: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0E92E: 83 E0 04           and         eax,4
  0000000140D0E931: 85 C0              test        eax,eax
  0000000140D0E933: 74 15              je          0000000140D0E94A
  0000000140D0E935: 83 A4 24 88 01 00  and         dword ptr [rsp+0000000000000188h],0FFFFFFFBh
                    00 FB
  0000000140D0E93D: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000140D0E945: E8 56 98 4F FF     call        00000001402081A0
  0000000140D0E94A: E9 C0 00 00 00     jmp         0000000140D0EA0F
  0000000140D0E94F: 33 C0              xor         eax,eax
  0000000140D0E951: 83 F8 01           cmp         eax,1
  0000000140D0E954: 74 0E              je          0000000140D0E964
  0000000140D0E956: 48 C7 84 24 F0 01  mov         qword ptr [rsp+00000000000001F0h],0
                    00 00 00 00 00 00
  0000000140D0E962: EB 50              jmp         0000000140D0E9B4
  0000000140D0E964: 48 8D 94 24 20 01  lea         rdx,[rsp+0000000000000120h]
                    00 00
  0000000140D0E96C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0E971: E8 3A 43 F7 FF     call        0000000140C82CB0
  0000000140D0E976: 48 89 84 24 F8 01  mov         qword ptr [rsp+00000000000001F8h],rax
                    00 00
  0000000140D0E97E: 48 8B 84 24 F8 01  mov         rax,qword ptr [rsp+00000000000001F8h]
                    00 00
  0000000140D0E986: 48 89 84 24 00 02  mov         qword ptr [rsp+0000000000000200h],rax
                    00 00
  0000000140D0E98E: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0E995: 83 C8 08           or          eax,8
  0000000140D0E998: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140D0E99F: 48 8B 8C 24 00 02  mov         rcx,qword ptr [rsp+0000000000000200h]
                    00 00
  0000000140D0E9A7: E8 A4 62 47 FF     call        0000000140184C50
  0000000140D0E9AC: 48 89 84 24 F0 01  mov         qword ptr [rsp+00000000000001F0h],rax
                    00 00
  0000000140D0E9B4: 48 8B 94 24 F0 01  mov         rdx,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140D0E9BC: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D0E9C4: E8 B7 D9 0E 00     call        0000000140DFC380
  0000000140D0E9C9: 48 83 C0 08        add         rax,8
  0000000140D0E9CD: 48 89 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],rax
                    00 00
  0000000140D0E9D5: 48 8B 84 24 08 02  mov         rax,qword ptr [rsp+0000000000000208h]
                    00 00
  0000000140D0E9DD: 8B 08              mov         ecx,dword ptr [rax]
  0000000140D0E9DF: 83 E9 01           sub         ecx,1
  0000000140D0E9E2: 48 8B 84 24 08 02  mov         rax,qword ptr [rsp+0000000000000208h]
                    00 00
  0000000140D0E9EA: 89 08              mov         dword ptr [rax],ecx
  0000000140D0E9EC: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0E9F3: 83 E0 08           and         eax,8
  0000000140D0E9F6: 85 C0              test        eax,eax
  0000000140D0E9F8: 74 15              je          0000000140D0EA0F
  0000000140D0E9FA: 83 A4 24 88 01 00  and         dword ptr [rsp+0000000000000188h],0FFFFFFF7h
                    00 F7
  0000000140D0EA02: 48 8D 8C 24 20 01  lea         rcx,[rsp+0000000000000120h]
                    00 00
  0000000140D0EA0A: E8 91 97 4F FF     call        00000001402081A0
  0000000140D0EA0F: 33 C0              xor         eax,eax
  0000000140D0EA11: 83 F8 01           cmp         eax,1
  0000000140D0EA14: 74 0E              je          0000000140D0EA24
  0000000140D0EA16: 48 C7 84 24 10 02  mov         qword ptr [rsp+0000000000000210h],0
                    00 00 00 00 00 00
  0000000140D0EA22: EB 50              jmp         0000000140D0EA74
  0000000140D0EA24: 48 8D 94 24 30 01  lea         rdx,[rsp+0000000000000130h]
                    00 00
  0000000140D0EA2C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0EA31: E8 7A 42 F7 FF     call        0000000140C82CB0
  0000000140D0EA36: 48 89 84 24 18 02  mov         qword ptr [rsp+0000000000000218h],rax
                    00 00
  0000000140D0EA3E: 48 8B 84 24 18 02  mov         rax,qword ptr [rsp+0000000000000218h]
                    00 00
  0000000140D0EA46: 48 89 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],rax
                    00 00
  0000000140D0EA4E: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0EA55: 83 C8 10           or          eax,10h
  0000000140D0EA58: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140D0EA5F: 48 8B 8C 24 20 02  mov         rcx,qword ptr [rsp+0000000000000220h]
                    00 00
  0000000140D0EA67: E8 E4 61 47 FF     call        0000000140184C50
  0000000140D0EA6C: 48 89 84 24 10 02  mov         qword ptr [rsp+0000000000000210h],rax
                    00 00
  0000000140D0EA74: 48 8B 94 24 10 02  mov         rdx,qword ptr [rsp+0000000000000210h]
                    00 00
  0000000140D0EA7C: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140D0EA84: E8 F7 D8 0E 00     call        0000000140DFC380
  0000000140D0EA89: 8B 40 08           mov         eax,dword ptr [rax+8]
  0000000140D0EA8C: 89 84 24 28 01 00  mov         dword ptr [rsp+0000000000000128h],eax
                    00
  0000000140D0EA93: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0EA9A: 83 E0 10           and         eax,10h
  0000000140D0EA9D: 85 C0              test        eax,eax
  0000000140D0EA9F: 74 15              je          0000000140D0EAB6
  0000000140D0EAA1: 83 A4 24 88 01 00  and         dword ptr [rsp+0000000000000188h],0FFFFFFEFh
                    00 EF
  0000000140D0EAA9: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  0000000140D0EAB1: E8 EA 96 4F FF     call        00000001402081A0
  0000000140D0EAB6: 83 BC 24 28 01 00  cmp         dword ptr [rsp+0000000000000128h],0
                    00 00
  0000000140D0EABE: 0F 84 8E 00 00 00  je          0000000140D0EB52
  0000000140D0EAC4: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D0EACC: E8 3F 2F 4E 00     call        00000001411F1A10
  0000000140D0EAD1: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140D0EAD6: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  0000000140D0EADC: 74 6F              je          0000000140D0EB4D
  0000000140D0EADE: C7 84 24 38 01 00  mov         dword ptr [rsp+0000000000000138h],0
                    00 00 00 00 00
  0000000140D0EAE9: 48 8D 94 24 38 01  lea         rdx,[rsp+0000000000000138h]
                    00 00
  0000000140D0EAF1: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D0EAF6: E8 45 FF 4D 00     call        00000001411EEA40
  0000000140D0EAFB: 0F B6 C0           movzx       eax,al
  0000000140D0EAFE: 85 C0              test        eax,eax
  0000000140D0EB00: 74 4B              je          0000000140D0EB4D
  0000000140D0EB02: C7 84 24 3C 01 00  mov         dword ptr [rsp+000000000000013Ch],1
                    00 01 00 00 00
  0000000140D0EB0D: 48 8D 94 24 3C 01  lea         rdx,[rsp+000000000000013Ch]
                    00 00
  0000000140D0EB15: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D0EB1A: E8 21 FF 4D 00     call        00000001411EEA40
  0000000140D0EB1F: 0F B6 C0           movzx       eax,al
  0000000140D0EB22: 85 C0              test        eax,eax
  0000000140D0EB24: 74 27              je          0000000140D0EB4D
  0000000140D0EB26: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D0EB2E: E8 0D DF 4E 00     call        00000001411FCA40
  0000000140D0EB33: 48 8B C8           mov         rcx,rax
  0000000140D0EB36: E8 45 3F 51 00     call        0000000141222A80
  0000000140D0EB3B: 0F B6 C0           movzx       eax,al
  0000000140D0EB3E: 85 C0              test        eax,eax
  0000000140D0EB40: 74 0B              je          0000000140D0EB4D
  0000000140D0EB42: 8B 44 24 68        mov         eax,dword ptr [rsp+68h]
  0000000140D0EB46: 83 C0 01           add         eax,1
  0000000140D0EB49: 89 44 24 68        mov         dword ptr [rsp+68h],eax
  0000000140D0EB4D: E9 FD FD FF FF     jmp         0000000140D0E94F
  0000000140D0EB52: E9 D9 FB FF FF     jmp         0000000140D0E730
  0000000140D0EB57: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140D0EB5F: E8 BC 29 F4 FF     call        0000000140C51520
  0000000140D0EB64: 83 7C 24 68 00     cmp         dword ptr [rsp+68h],0
  0000000140D0EB69: 74 34              je          0000000140D0EB9F
  0000000140D0EB6B: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0EB70: E8 3B 1A F8 FF     call        0000000140C905B0
  0000000140D0EB75: 48 8B C8           mov         rcx,rax
  0000000140D0EB78: 48 8D 44 24 60     lea         rax,[rsp+60h]
  0000000140D0EB7D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D0EB82: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  0000000140D0EB8A: 4C 8B 84 24 E0 02  mov         r8,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140D0EB92: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D0EB9A: E8 21 08 02 00     call        0000000140D2F3C0
  0000000140D0EB9F: EB 74              jmp         0000000140D0EC15
  0000000140D0EBA1: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],1
                    00 01
  0000000140D0EBA9: 75 36              jne         0000000140D0EBE1
  0000000140D0EBAB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0EBB0: E8 FB 19 F8 FF     call        0000000140C905B0
  0000000140D0EBB5: 48 8B C8           mov         rcx,rax
  0000000140D0EBB8: 48 8D 44 24 60     lea         rax,[rsp+60h]
  0000000140D0EBBD: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D0EBC2: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  0000000140D0EBCA: 4C 8B 84 24 E0 02  mov         r8,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140D0EBD2: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D0EBDA: E8 91 E2 01 00     call        0000000140D2CE70
  0000000140D0EBDF: EB 34              jmp         0000000140D0EC15
  0000000140D0EBE1: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0EBE6: E8 C5 19 F8 FF     call        0000000140C905B0
  0000000140D0EBEB: 48 8B C8           mov         rcx,rax
  0000000140D0EBEE: 48 8D 44 24 60     lea         rax,[rsp+60h]
  0000000140D0EBF3: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D0EBF8: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  0000000140D0EC00: 4C 8B 84 24 E0 02  mov         r8,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140D0EC08: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D0EC10: E8 BB 12 02 00     call        0000000140D2FED0
  0000000140D0EC15: 48 8B 8C 24 D0 02  mov         rcx,qword ptr [rsp+00000000000002D0h]
                    00 00
  0000000140D0EC1D: 48 81 C1 D8 01 00  add         rcx,1D8h
                    00
  0000000140D0EC24: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000140D0EC29: E8 02 3E 00 00     call        0000000140D12A30
  0000000140D0EC2E: 90                 nop
  0000000140D0EC2F: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000140D0EC37: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D0EC3C: E8 9F 5E 2F FF     call        0000000140004AE0
  0000000140D0EC41: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  0000000140D0EC45: C7 44 24 40 00 00  mov         dword ptr [rsp+40h],0
                    00 00
  0000000140D0EC4D: EB 0B              jmp         0000000140D0EC5A
  0000000140D0EC4F: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  0000000140D0EC53: 83 C0 01           add         eax,1
  0000000140D0EC56: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  0000000140D0EC5A: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  0000000140D0EC5E: 39 44 24 40        cmp         dword ptr [rsp+40h],eax
  0000000140D0EC62: 0F 8D 4E 06 00 00  jge         0000000140D0F2B6
  0000000140D0EC68: 8B 54 24 40        mov         edx,dword ptr [rsp+40h]
  0000000140D0EC6C: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D0EC71: E8 9A 1F 45 FF     call        0000000140160C10
  0000000140D0EC76: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140D0EC79: E8 82 EF A4 FF     call        000000014075DC00
  0000000140D0EC7E: 0F B6 C0           movzx       eax,al
  0000000140D0EC81: 85 C0              test        eax,eax
  0000000140D0EC83: 0F 84 28 06 00 00  je          0000000140D0F2B1
  0000000140D0EC89: 8B 54 24 40        mov         edx,dword ptr [rsp+40h]
  0000000140D0EC8D: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D0EC92: E8 79 1F 45 FF     call        0000000140160C10
  0000000140D0EC97: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140D0EC9A: E8 61 35 00 00     call        0000000140D12200
  0000000140D0EC9F: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140D0ECA7: 48 83 BC 24 C0 00  cmp         qword ptr [rsp+00000000000000C0h],0
                    00 00 00
  0000000140D0ECB0: 0F 84 FB 05 00 00  je          0000000140D0F2B1
  0000000140D0ECB6: 83 BC 24 90 00 00  cmp         dword ptr [rsp+0000000000000090h],0
                    00 00
  0000000140D0ECBE: 0F 85 76 05 00 00  jne         0000000140D0F23A
  0000000140D0ECC4: 83 7C 24 68 00     cmp         dword ptr [rsp+68h],0
  0000000140D0ECC9: 0F 85 6B 05 00 00  jne         0000000140D0F23A
  0000000140D0ECCF: C7 84 24 C8 00 00  mov         dword ptr [rsp+00000000000000C8h],0
                    00 00 00 00 00
  0000000140D0ECDA: 48 8D 94 24 40 01  lea         rdx,[rsp+0000000000000140h]
                    00 00
  0000000140D0ECE2: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D0ECEA: E8 C1 3F F7 FF     call        0000000140C82CB0
  0000000140D0ECEF: 48 89 84 24 28 02  mov         qword ptr [rsp+0000000000000228h],rax
                    00 00
  0000000140D0ECF7: 48 8B 84 24 28 02  mov         rax,qword ptr [rsp+0000000000000228h]
                    00 00
  0000000140D0ECFF: 48 89 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],rax
                    00 00
  0000000140D0ED07: 48 8B 94 24 30 02  mov         rdx,qword ptr [rsp+0000000000000230h]
                    00 00
  0000000140D0ED0F: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140D0ED17: E8 D4 ED 35 FF     call        000000014006DAF0
  0000000140D0ED1C: 90                 nop
  0000000140D0ED1D: 48 8D 84 24 D0 00  lea         rax,[rsp+00000000000000D0h]
                    00 00
  0000000140D0ED25: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140D0ED2D: 48 8D 8C 24 40 01  lea         rcx,[rsp+0000000000000140h]
                    00 00
  0000000140D0ED35: E8 66 94 4F FF     call        00000001402081A0
  0000000140D0ED3A: E9 AD 00 00 00     jmp         0000000140D0EDEC
  0000000140D0ED3F: 33 C0              xor         eax,eax
  0000000140D0ED41: 83 F8 01           cmp         eax,1
  0000000140D0ED44: 74 0E              je          0000000140D0ED54
  0000000140D0ED46: 48 C7 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],0
                    00 00 00 00 00 00
  0000000140D0ED52: EB 53              jmp         0000000140D0EDA7
  0000000140D0ED54: 48 8D 94 24 48 01  lea         rdx,[rsp+0000000000000148h]
                    00 00
  0000000140D0ED5C: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D0ED64: E8 47 3F F7 FF     call        0000000140C82CB0
  0000000140D0ED69: 48 89 84 24 40 02  mov         qword ptr [rsp+0000000000000240h],rax
                    00 00
  0000000140D0ED71: 48 8B 84 24 40 02  mov         rax,qword ptr [rsp+0000000000000240h]
                    00 00
  0000000140D0ED79: 48 89 84 24 48 02  mov         qword ptr [rsp+0000000000000248h],rax
                    00 00
  0000000140D0ED81: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0ED88: 83 C8 20           or          eax,20h
  0000000140D0ED8B: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140D0ED92: 48 8B 8C 24 48 02  mov         rcx,qword ptr [rsp+0000000000000248h]
                    00 00
  0000000140D0ED9A: E8 B1 5E 47 FF     call        0000000140184C50
  0000000140D0ED9F: 48 89 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],rax
                    00 00
  0000000140D0EDA7: 48 8B 94 24 38 02  mov         rdx,qword ptr [rsp+0000000000000238h]
                    00 00
  0000000140D0EDAF: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D0EDB7: E8 C4 D5 0E 00     call        0000000140DFC380
  0000000140D0EDBC: 48 8B C8           mov         rcx,rax
  0000000140D0EDBF: 48 83 C1 10        add         rcx,10h
  0000000140D0EDC3: E8 38 AA 0D 00     call        0000000140DE9800
  0000000140D0EDC8: 90                 nop
  0000000140D0EDC9: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0EDD0: 83 E0 20           and         eax,20h
  0000000140D0EDD3: 85 C0              test        eax,eax
  0000000140D0EDD5: 74 15              je          0000000140D0EDEC
  0000000140D0EDD7: 83 A4 24 88 01 00  and         dword ptr [rsp+0000000000000188h],0FFFFFFDFh
                    00 DF
  0000000140D0EDDF: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  0000000140D0EDE7: E8 B4 93 4F FF     call        00000001402081A0
  0000000140D0EDEC: 33 C0              xor         eax,eax
  0000000140D0EDEE: 83 F8 01           cmp         eax,1
  0000000140D0EDF1: 74 0E              je          0000000140D0EE01
  0000000140D0EDF3: 48 C7 84 24 50 02  mov         qword ptr [rsp+0000000000000250h],0
                    00 00 00 00 00 00
  0000000140D0EDFF: EB 53              jmp         0000000140D0EE54
  0000000140D0EE01: 48 8D 94 24 58 01  lea         rdx,[rsp+0000000000000158h]
                    00 00
  0000000140D0EE09: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D0EE11: E8 9A 3E F7 FF     call        0000000140C82CB0
  0000000140D0EE16: 48 89 84 24 58 02  mov         qword ptr [rsp+0000000000000258h],rax
                    00 00
  0000000140D0EE1E: 48 8B 84 24 58 02  mov         rax,qword ptr [rsp+0000000000000258h]
                    00 00
  0000000140D0EE26: 48 89 84 24 60 02  mov         qword ptr [rsp+0000000000000260h],rax
                    00 00
  0000000140D0EE2E: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0EE35: 83 C8 40           or          eax,40h
  0000000140D0EE38: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140D0EE3F: 48 8B 8C 24 60 02  mov         rcx,qword ptr [rsp+0000000000000260h]
                    00 00
  0000000140D0EE47: E8 04 5E 47 FF     call        0000000140184C50
  0000000140D0EE4C: 48 89 84 24 50 02  mov         qword ptr [rsp+0000000000000250h],rax
                    00 00
  0000000140D0EE54: 48 8B 94 24 50 02  mov         rdx,qword ptr [rsp+0000000000000250h]
                    00 00
  0000000140D0EE5C: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D0EE64: E8 17 D5 0E 00     call        0000000140DFC380
  0000000140D0EE69: 48 8B C8           mov         rcx,rax
  0000000140D0EE6C: E8 0F D2 0E 00     call        0000000140DFC080
  0000000140D0EE71: 88 84 24 50 01 00  mov         byte ptr [rsp+0000000000000150h],al
                    00
  0000000140D0EE78: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0EE7F: 83 E0 40           and         eax,40h
  0000000140D0EE82: 85 C0              test        eax,eax
  0000000140D0EE84: 74 15              je          0000000140D0EE9B
  0000000140D0EE86: 83 A4 24 88 01 00  and         dword ptr [rsp+0000000000000188h],0FFFFFFBFh
                    00 BF
  0000000140D0EE8E: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  0000000140D0EE96: E8 05 93 4F FF     call        00000001402081A0
  0000000140D0EE9B: 0F B6 84 24 50 01  movzx       eax,byte ptr [rsp+0000000000000150h]
                    00 00
  0000000140D0EEA3: 85 C0              test        eax,eax
  0000000140D0EEA5: 0F 84 E5 02 00 00  je          0000000140D0F190
  0000000140D0EEAB: 33 C0              xor         eax,eax
  0000000140D0EEAD: 83 F8 01           cmp         eax,1
  0000000140D0EEB0: 74 0E              je          0000000140D0EEC0
  0000000140D0EEB2: 48 C7 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],0
                    00 00 00 00 00 00
  0000000140D0EEBE: EB 55              jmp         0000000140D0EF15
  0000000140D0EEC0: 48 8D 94 24 60 01  lea         rdx,[rsp+0000000000000160h]
                    00 00
  0000000140D0EEC8: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D0EED0: E8 DB 3D F7 FF     call        0000000140C82CB0
  0000000140D0EED5: 48 89 84 24 70 02  mov         qword ptr [rsp+0000000000000270h],rax
                    00 00
  0000000140D0EEDD: 48 8B 84 24 70 02  mov         rax,qword ptr [rsp+0000000000000270h]
                    00 00
  0000000140D0EEE5: 48 89 84 24 78 02  mov         qword ptr [rsp+0000000000000278h],rax
                    00 00
  0000000140D0EEED: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0EEF4: 0D 80 00 00 00     or          eax,80h
  0000000140D0EEF9: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140D0EF00: 48 8B 8C 24 78 02  mov         rcx,qword ptr [rsp+0000000000000278h]
                    00 00
  0000000140D0EF08: E8 43 5D 47 FF     call        0000000140184C50
  0000000140D0EF0D: 48 89 84 24 68 02  mov         qword ptr [rsp+0000000000000268h],rax
                    00 00
  0000000140D0EF15: 48 8B 94 24 68 02  mov         rdx,qword ptr [rsp+0000000000000268h]
                    00 00
  0000000140D0EF1D: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D0EF25: E8 56 D4 0E 00     call        0000000140DFC380
  0000000140D0EF2A: 48 8B C8           mov         rcx,rax
  0000000140D0EF2D: 48 83 C1 10        add         rcx,10h
  0000000140D0EF31: E8 CA 04 94 FF     call        000000014064F400
  0000000140D0EF36: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D0EF39: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140D0EF41: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0EF48: 25 80 00 00 00     and         eax,80h
  0000000140D0EF4D: 85 C0              test        eax,eax
  0000000140D0EF4F: 74 18              je          0000000140D0EF69
  0000000140D0EF51: 81 A4 24 88 01 00  and         dword ptr [rsp+0000000000000188h],0FFFFFF7Fh
                    00 7F FF FF FF
  0000000140D0EF5C: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  0000000140D0EF64: E8 37 92 4F FF     call        00000001402081A0
  0000000140D0EF69: E9 CA 00 00 00     jmp         0000000140D0F038
  0000000140D0EF6E: 33 C0              xor         eax,eax
  0000000140D0EF70: 83 F8 01           cmp         eax,1
  0000000140D0EF73: 74 0E              je          0000000140D0EF83
  0000000140D0EF75: 48 C7 84 24 80 02  mov         qword ptr [rsp+0000000000000280h],0
                    00 00 00 00 00 00
  0000000140D0EF81: EB 55              jmp         0000000140D0EFD8
  0000000140D0EF83: 48 8D 94 24 68 01  lea         rdx,[rsp+0000000000000168h]
                    00 00
  0000000140D0EF8B: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D0EF93: E8 18 3D F7 FF     call        0000000140C82CB0
  0000000140D0EF98: 48 89 84 24 88 02  mov         qword ptr [rsp+0000000000000288h],rax
                    00 00
  0000000140D0EFA0: 48 8B 84 24 88 02  mov         rax,qword ptr [rsp+0000000000000288h]
                    00 00
  0000000140D0EFA8: 48 89 84 24 90 02  mov         qword ptr [rsp+0000000000000290h],rax
                    00 00
  0000000140D0EFB0: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0EFB7: 0D 00 01 00 00     or          eax,100h
  0000000140D0EFBC: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140D0EFC3: 48 8B 8C 24 90 02  mov         rcx,qword ptr [rsp+0000000000000290h]
                    00 00
  0000000140D0EFCB: E8 80 5C 47 FF     call        0000000140184C50
  0000000140D0EFD0: 48 89 84 24 80 02  mov         qword ptr [rsp+0000000000000280h],rax
                    00 00
  0000000140D0EFD8: 48 8B 94 24 80 02  mov         rdx,qword ptr [rsp+0000000000000280h]
                    00 00
  0000000140D0EFE0: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D0EFE8: E8 93 D3 0E 00     call        0000000140DFC380
  0000000140D0EFED: 48 83 C0 08        add         rax,8
  0000000140D0EFF1: 48 89 84 24 98 02  mov         qword ptr [rsp+0000000000000298h],rax
                    00 00
  0000000140D0EFF9: 48 8B 84 24 98 02  mov         rax,qword ptr [rsp+0000000000000298h]
                    00 00
  0000000140D0F001: 8B 08              mov         ecx,dword ptr [rax]
  0000000140D0F003: 83 E9 01           sub         ecx,1
  0000000140D0F006: 48 8B 84 24 98 02  mov         rax,qword ptr [rsp+0000000000000298h]
                    00 00
  0000000140D0F00E: 89 08              mov         dword ptr [rax],ecx
  0000000140D0F010: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0F017: 25 00 01 00 00     and         eax,100h
  0000000140D0F01C: 85 C0              test        eax,eax
  0000000140D0F01E: 74 18              je          0000000140D0F038
  0000000140D0F020: 81 A4 24 88 01 00  and         dword ptr [rsp+0000000000000188h],0FFFFFEFFh
                    00 FF FE FF FF
  0000000140D0F02B: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  0000000140D0F033: E8 68 91 4F FF     call        00000001402081A0
  0000000140D0F038: 33 C0              xor         eax,eax
  0000000140D0F03A: 83 F8 01           cmp         eax,1
  0000000140D0F03D: 74 0E              je          0000000140D0F04D
  0000000140D0F03F: 48 C7 84 24 A0 02  mov         qword ptr [rsp+00000000000002A0h],0
                    00 00 00 00 00 00
  0000000140D0F04B: EB 55              jmp         0000000140D0F0A2
  0000000140D0F04D: 48 8D 94 24 78 01  lea         rdx,[rsp+0000000000000178h]
                    00 00
  0000000140D0F055: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D0F05D: E8 4E 3C F7 FF     call        0000000140C82CB0
  0000000140D0F062: 48 89 84 24 A8 02  mov         qword ptr [rsp+00000000000002A8h],rax
                    00 00
  0000000140D0F06A: 48 8B 84 24 A8 02  mov         rax,qword ptr [rsp+00000000000002A8h]
                    00 00
  0000000140D0F072: 48 89 84 24 B0 02  mov         qword ptr [rsp+00000000000002B0h],rax
                    00 00
  0000000140D0F07A: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0F081: 0D 00 02 00 00     or          eax,200h
  0000000140D0F086: 89 84 24 88 01 00  mov         dword ptr [rsp+0000000000000188h],eax
                    00
  0000000140D0F08D: 48 8B 8C 24 B0 02  mov         rcx,qword ptr [rsp+00000000000002B0h]
                    00 00
  0000000140D0F095: E8 B6 5B 47 FF     call        0000000140184C50
  0000000140D0F09A: 48 89 84 24 A0 02  mov         qword ptr [rsp+00000000000002A0h],rax
                    00 00
  0000000140D0F0A2: 48 8B 94 24 A0 02  mov         rdx,qword ptr [rsp+00000000000002A0h]
                    00 00
  0000000140D0F0AA: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140D0F0B2: E8 C9 D2 0E 00     call        0000000140DFC380
  0000000140D0F0B7: 8B 40 08           mov         eax,dword ptr [rax+8]
  0000000140D0F0BA: 89 84 24 70 01 00  mov         dword ptr [rsp+0000000000000170h],eax
                    00
  0000000140D0F0C1: 8B 84 24 88 01 00  mov         eax,dword ptr [rsp+0000000000000188h]
                    00
  0000000140D0F0C8: 25 00 02 00 00     and         eax,200h
  0000000140D0F0CD: 85 C0              test        eax,eax
  0000000140D0F0CF: 74 18              je          0000000140D0F0E9
  0000000140D0F0D1: 81 A4 24 88 01 00  and         dword ptr [rsp+0000000000000188h],0FFFFFDFFh
                    00 FF FD FF FF
  0000000140D0F0DC: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  0000000140D0F0E4: E8 B7 90 4F FF     call        00000001402081A0
  0000000140D0F0E9: 83 BC 24 70 01 00  cmp         dword ptr [rsp+0000000000000170h],0
                    00 00
  0000000140D0F0F1: 0F 84 94 00 00 00  je          0000000140D0F18B
  0000000140D0F0F7: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D0F0FF: E8 0C 29 4E 00     call        00000001411F1A10
  0000000140D0F104: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140D0F109: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  0000000140D0F10F: 74 75              je          0000000140D0F186
  0000000140D0F111: C7 84 24 80 01 00  mov         dword ptr [rsp+0000000000000180h],0
                    00 00 00 00 00
  0000000140D0F11C: 48 8D 94 24 80 01  lea         rdx,[rsp+0000000000000180h]
                    00 00
  0000000140D0F124: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D0F129: E8 12 F9 4D 00     call        00000001411EEA40
  0000000140D0F12E: 0F B6 C0           movzx       eax,al
  0000000140D0F131: 85 C0              test        eax,eax
  0000000140D0F133: 74 51              je          0000000140D0F186
  0000000140D0F135: C7 84 24 84 01 00  mov         dword ptr [rsp+0000000000000184h],1
                    00 01 00 00 00
  0000000140D0F140: 48 8D 94 24 84 01  lea         rdx,[rsp+0000000000000184h]
                    00 00
  0000000140D0F148: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140D0F14D: E8 EE F8 4D 00     call        00000001411EEA40
  0000000140D0F152: 0F B6 C0           movzx       eax,al
  0000000140D0F155: 85 C0              test        eax,eax
  0000000140D0F157: 74 2D              je          0000000140D0F186
  0000000140D0F159: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140D0F161: E8 DA D8 4E 00     call        00000001411FCA40
  0000000140D0F166: 48 8B C8           mov         rcx,rax
  0000000140D0F169: E8 12 39 51 00     call        0000000141222A80
  0000000140D0F16E: 0F B6 C0           movzx       eax,al
  0000000140D0F171: 85 C0              test        eax,eax
  0000000140D0F173: 74 11              je          0000000140D0F186
  0000000140D0F175: 8B 84 24 C8 00 00  mov         eax,dword ptr [rsp+00000000000000C8h]
                    00
  0000000140D0F17C: 83 C0 01           add         eax,1
  0000000140D0F17F: 89 84 24 C8 00 00  mov         dword ptr [rsp+00000000000000C8h],eax
                    00
  0000000140D0F186: E9 E3 FD FF FF     jmp         0000000140D0EF6E
  0000000140D0F18B: E9 AF FB FF FF     jmp         0000000140D0ED3F
  0000000140D0F190: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140D0F198: E8 83 23 F4 FF     call        0000000140C51520
  0000000140D0F19D: 83 BC 24 C8 00 00  cmp         dword ptr [rsp+00000000000000C8h],0
                    00 00
  0000000140D0F1A5: 0F 84 8F 00 00 00  je          0000000140D0F23A
  0000000140D0F1AB: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D0F1B3: E8 F8 13 F8 FF     call        0000000140C905B0
  0000000140D0F1B8: 48 8B C8           mov         rcx,rax
  0000000140D0F1BB: 48 8D 44 24 60     lea         rax,[rsp+60h]
  0000000140D0F1C0: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D0F1C5: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  0000000140D0F1CD: 4C 8B 84 24 E0 02  mov         r8,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140D0F1D5: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D0F1DD: E8 DE 01 02 00     call        0000000140D2F3C0
  0000000140D0F1E2: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0F1E7: E8 C4 13 F8 FF     call        0000000140C905B0
  0000000140D0F1EC: F2 0F 10 44 24 60  movsd       xmm0,mmword ptr [rsp+60h]
  0000000140D0F1F2: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  0000000140D0F1F8: F2 0F 10 9C 24 80  movsd       xmm3,mmword ptr [rsp+0000000000000080h]
                    00 00 00
  0000000140D0F201: 4C 8B 84 24 E0 02  mov         r8,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140D0F209: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D0F211: 48 8B C8           mov         rcx,rax
  0000000140D0F214: E8 77 0E 02 00     call        0000000140D30090
  0000000140D0F219: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140D0F21E: E8 8D 13 F8 FF     call        0000000140C905B0
  0000000140D0F223: F2 0F 10 54 24 60  movsd       xmm2,mmword ptr [rsp+60h]
  0000000140D0F229: F2 0F 10 8C 24 80  movsd       xmm1,mmword ptr [rsp+0000000000000080h]
                    00 00 00
  0000000140D0F232: 48 8B C8           mov         rcx,rax
  0000000140D0F235: E8 06 97 00 00     call        0000000140D18940
  0000000140D0F23A: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D0F242: E8 69 13 F8 FF     call        0000000140C905B0
  0000000140D0F247: F2 0F 10 44 24 60  movsd       xmm0,mmword ptr [rsp+60h]
  0000000140D0F24D: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  0000000140D0F253: F2 0F 10 9C 24 80  movsd       xmm3,mmword ptr [rsp+0000000000000080h]
                    00 00 00
  0000000140D0F25C: 4C 8B 84 24 E0 02  mov         r8,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140D0F264: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D0F26C: 48 8B C8           mov         rcx,rax
  0000000140D0F26F: E8 1C 0E 02 00     call        0000000140D30090
  0000000140D0F274: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140D0F27C: E8 2F 13 F8 FF     call        0000000140C905B0
  0000000140D0F281: C7 44 24 28 02 00  mov         dword ptr [rsp+28h],2
                    00 00
  0000000140D0F289: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  0000000140D0F291: 4C 8B 8C 24 E8 02  mov         r9,qword ptr [rsp+00000000000002E8h]
                    00 00
  0000000140D0F299: 4C 8B 84 24 E0 02  mov         r8,qword ptr [rsp+00000000000002E0h]
                    00 00
  0000000140D0F2A1: 48 8B 94 24 D8 02  mov         rdx,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D0F2A9: 48 8B C8           mov         rcx,rax
  0000000140D0F2AC: E8 3F 6F 00 00     call        0000000140D161F0
  0000000140D0F2B1: E9 99 F9 FF FF     jmp         0000000140D0EC4F
  0000000140D0F2B6: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140D0F2BC: 74 21              je          0000000140D0F2DF
  0000000140D0F2BE: 48 8B 84 24 D8 02  mov         rax,qword ptr [rsp+00000000000002D8h]
                    00 00
  0000000140D0F2C6: 83 38 00           cmp         dword ptr [rax],0
  0000000140D0F2C9: 75 14              jne         0000000140D0F2DF
  0000000140D0F2CB: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000140D0F2D0: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D0F2D3: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140D0F2D8: FF 90 60 03 00 00  call        qword ptr [rax+0000000000000360h]
  0000000140D0F2DE: 90                 nop
  0000000140D0F2DF: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140D0F2E4: E8 B7 8E 4F FF     call        00000001402081A0
  0000000140D0F2E9: 48 81 C4 C8 02 00  add         rsp,2C8h
                    00
  0000000140D0F2F0: C3                 ret

  Summary

     1E02000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140C0C650: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140C0C655: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140C0C65A: 48 81 EC 28 01 00  sub         rsp,128h
                    00
  0000000140C0C661: 48 C7 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140C0C66D: 48 83 BC 24 38 01  cmp         qword ptr [rsp+0000000000000138h],0
                    00 00 00
  0000000140C0C676: 0F 84 F2 04 00 00  je          0000000140C0CB6E
  0000000140C0C67C: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140C0C681: 48 8B 8C 24 38 01  mov         rcx,qword ptr [rsp+0000000000000138h]
                    00 00
  0000000140C0C689: FF 15 19 B6 1F 01  call        qword ptr [0000000141E07CA8h]
  0000000140C0C68F: 90                 nop
  0000000140C0C690: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140C0C695: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140C0C69A: E8 51 14 46 FF     call        000000014006DAF0
  0000000140C0C69F: 90                 nop
  0000000140C0C6A0: 48 8D 44 24 38     lea         rax,[rsp+38h]
  0000000140C0C6A5: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140C0C6AA: EB 45              jmp         0000000140C0C6F1
  0000000140C0C6AC: 33 C0              xor         eax,eax
  0000000140C0C6AE: 83 F8 01           cmp         eax,1
  0000000140C0C6B1: 74 0E              je          0000000140C0C6C1
  0000000140C0C6B3: 48 C7 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],0
                    00 00 00 00 00 00
  0000000140C0C6BF: EB 12              jmp         0000000140C0C6D3
  0000000140C0C6C1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C0C6C6: E8 85 85 57 FF     call        0000000140184C50
  0000000140C0C6CB: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  0000000140C0C6D3: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140C0C6DB: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C0C6E0: E8 9B FC 1E 00     call        0000000140DFC380
  0000000140C0C6E5: 48 8B C8           mov         rcx,rax
  0000000140C0C6E8: 48 83 C1 10        add         rcx,10h
  0000000140C0C6EC: E8 0F D1 1D 00     call        0000000140DE9800
  0000000140C0C6F1: 33 C0              xor         eax,eax
  0000000140C0C6F3: 83 F8 01           cmp         eax,1
  0000000140C0C6F6: 74 0E              je          0000000140C0C706
  0000000140C0C6F8: 48 C7 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],0
                    00 00 00 00 00 00
  0000000140C0C704: EB 12              jmp         0000000140C0C718
  0000000140C0C706: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C0C70B: E8 40 85 57 FF     call        0000000140184C50
  0000000140C0C710: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  0000000140C0C718: 48 8B 94 24 E0 00  mov         rdx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140C0C720: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C0C725: E8 56 FC 1E 00     call        0000000140DFC380
  0000000140C0C72A: 48 8B C8           mov         rcx,rax
  0000000140C0C72D: E8 4E F9 1E 00     call        0000000140DFC080
  0000000140C0C732: 0F B6 C0           movzx       eax,al
  0000000140C0C735: 85 C0              test        eax,eax
  0000000140C0C737: 0F 84 1C 04 00 00  je          0000000140C0CB59
  0000000140C0C73D: 33 C0              xor         eax,eax
  0000000140C0C73F: 83 F8 01           cmp         eax,1
  0000000140C0C742: 74 0E              je          0000000140C0C752
  0000000140C0C744: 48 C7 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],0
                    00 00 00 00 00 00
  0000000140C0C750: EB 12              jmp         0000000140C0C764
  0000000140C0C752: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C0C757: E8 F4 84 57 FF     call        0000000140184C50
  0000000140C0C75C: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  0000000140C0C764: 48 8B 94 24 E8 00  mov         rdx,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140C0C76C: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C0C771: E8 0A FC 1E 00     call        0000000140DFC380
  0000000140C0C776: 48 8B C8           mov         rcx,rax
  0000000140C0C779: 48 83 C1 10        add         rcx,10h
  0000000140C0C77D: E8 7E 2C A4 FF     call        000000014064F400
  0000000140C0C782: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C0C785: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140C0C78A: EB 5C              jmp         0000000140C0C7E8
  0000000140C0C78C: 33 C0              xor         eax,eax
  0000000140C0C78E: 83 F8 01           cmp         eax,1
  0000000140C0C791: 74 0E              je          0000000140C0C7A1
  0000000140C0C793: 48 C7 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],0
                    00 00 00 00 00 00
  0000000140C0C79F: EB 12              jmp         0000000140C0C7B3
  0000000140C0C7A1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C0C7A6: E8 A5 84 57 FF     call        0000000140184C50
  0000000140C0C7AB: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  0000000140C0C7B3: 48 8B 94 24 F0 00  mov         rdx,qword ptr [rsp+00000000000000F0h]
                    00 00
  0000000140C0C7BB: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C0C7C0: E8 BB FB 1E 00     call        0000000140DFC380
  0000000140C0C7C5: 48 83 C0 08        add         rax,8
  0000000140C0C7C9: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  0000000140C0C7D1: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  0000000140C0C7D9: 8B 08              mov         ecx,dword ptr [rax]
  0000000140C0C7DB: 83 E9 01           sub         ecx,1
  0000000140C0C7DE: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  0000000140C0C7E6: 89 08              mov         dword ptr [rax],ecx
  0000000140C0C7E8: 33 C0              xor         eax,eax
  0000000140C0C7EA: 83 F8 01           cmp         eax,1
  0000000140C0C7ED: 74 0E              je          0000000140C0C7FD
  0000000140C0C7EF: 48 C7 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],0
                    00 00 00 00 00 00
  0000000140C0C7FB: EB 12              jmp         0000000140C0C80F
  0000000140C0C7FD: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C0C802: E8 49 84 57 FF     call        0000000140184C50
  0000000140C0C807: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  0000000140C0C80F: 48 8B 94 24 00 01  mov         rdx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140C0C817: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140C0C81C: E8 5F FB 1E 00     call        0000000140DFC380
  0000000140C0C821: 83 78 08 00        cmp         dword ptr [rax+8],0
  0000000140C0C825: 0F 84 29 03 00 00  je          0000000140C0CB54
  0000000140C0C82B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0C833: 4C 8D 0D A6 9A A2  lea         r9,[00000001436362E0h]
                    02
  0000000140C0C83A: 4C 8D 05 AF 49 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0C841: 33 D2              xor         edx,edx
  0000000140C0C843: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C0C848: E8 AF 30 86 00     call        000000014146F8FC
  0000000140C0C84D: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140C0C852: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140C0C858: 74 67              je          0000000140C0C8C1
  0000000140C0C85A: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140C0C85F: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140C0C867: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140C0C86F: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140C0C877: 48 83 BC 24 A0 00  cmp         qword ptr [rsp+00000000000000A0h],0
                    00 00 00
  0000000140C0C880: 74 25              je          0000000140C0C8A7
  0000000140C0C882: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140C0C88A: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C0C88D: BA 01 00 00 00     mov         edx,1
  0000000140C0C892: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140C0C89A: FF 50 18           call        qword ptr [rax+18h]
  0000000140C0C89D: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  0000000140C0C8A5: EB 0C              jmp         0000000140C0C8B3
  0000000140C0C8A7: 48 C7 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],0
                    00 00 00 00 00 00
  0000000140C0C8B3: 48 C7 44 24 70 00  mov         qword ptr [rsp+70h],0
                    00 00 00
  0000000140C0C8BC: E9 CB FE FF FF     jmp         0000000140C0C78C
  0000000140C0C8C1: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0C8C9: 4C 8D 0D 40 A3 AF  lea         r9,[0000000143706C10h]
                    02
  0000000140C0C8D0: 4C 8D 05 19 49 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0C8D7: 33 D2              xor         edx,edx
  0000000140C0C8D9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C0C8DE: E8 19 30 86 00     call        000000014146F8FC
  0000000140C0C8E3: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140C0C8EB: 48 83 BC 24 98 00  cmp         qword ptr [rsp+0000000000000098h],0
                    00 00 00
  0000000140C0C8F4: 74 22              je          0000000140C0C918
  0000000140C0C8F6: 48 8B 8C 24 98 00  mov         rcx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140C0C8FE: E8 5D 16 00 00     call        0000000140C0DF60
  0000000140C0C903: 48 8B D0           mov         rdx,rax
  0000000140C0C906: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140C0C90E: E8 9D 94 00 00     call        0000000140C15DB0
  0000000140C0C913: E9 74 FE FF FF     jmp         0000000140C0C78C
  0000000140C0C918: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0C920: 4C 8D 0D 81 23 CD  lea         r9,[00000001438DECA8h]
                    02
  0000000140C0C927: 4C 8D 05 C2 48 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0C92E: 33 D2              xor         edx,edx
  0000000140C0C930: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C0C935: E8 C2 2F 86 00     call        000000014146F8FC
  0000000140C0C93A: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140C0C93F: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  0000000140C0C945: 74 1F              je          0000000140C0C966
  0000000140C0C947: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140C0C94C: E8 0F 16 00 00     call        0000000140C0DF60
  0000000140C0C951: 48 8B D0           mov         rdx,rax
  0000000140C0C954: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140C0C95C: E8 4F 94 00 00     call        0000000140C15DB0
  0000000140C0C961: E9 26 FE FF FF     jmp         0000000140C0C78C
  0000000140C0C966: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0C96E: 4C 8D 0D 03 23 CD  lea         r9,[00000001438DEC78h]
                    02
  0000000140C0C975: 4C 8D 05 74 48 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0C97C: 33 D2              xor         edx,edx
  0000000140C0C97E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C0C983: E8 74 2F 86 00     call        000000014146F8FC
  0000000140C0C988: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140C0C98D: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140C0C993: 74 1F              je          0000000140C0C9B4
  0000000140C0C995: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140C0C99A: E8 C1 15 00 00     call        0000000140C0DF60
  0000000140C0C99F: 48 8B D0           mov         rdx,rax
  0000000140C0C9A2: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140C0C9AA: E8 01 94 00 00     call        0000000140C15DB0
  0000000140C0C9AF: E9 D8 FD FF FF     jmp         0000000140C0C78C
  0000000140C0C9B4: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0C9BC: 4C 8D 0D 6D 23 CD  lea         r9,[00000001438DED30h]
                    02
  0000000140C0C9C3: 4C 8D 05 26 48 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0C9CA: 33 D2              xor         edx,edx
  0000000140C0C9CC: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C0C9D1: E8 26 2F 86 00     call        000000014146F8FC
  0000000140C0C9D6: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  0000000140C0C9DE: 48 83 BC 24 90 00  cmp         qword ptr [rsp+0000000000000090h],0
                    00 00 00
  0000000140C0C9E7: 74 22              je          0000000140C0CA0B
  0000000140C0C9E9: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140C0C9F1: E8 6A 15 00 00     call        0000000140C0DF60
  0000000140C0C9F6: 48 8B D0           mov         rdx,rax
  0000000140C0C9F9: 48 8B 8C 24 30 01  mov         rcx,qword ptr [rsp+0000000000000130h]
                    00 00
  0000000140C0CA01: E8 AA 93 00 00     call        0000000140C15DB0
  0000000140C0CA06: E9 81 FD FF FF     jmp         0000000140C0C78C
  0000000140C0CA0B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0CA13: 4C 8D 0D E6 22 CD  lea         r9,[00000001438DED00h]
                    02
  0000000140C0CA1A: 4C 8D 05 CF 47 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0CA21: 33 D2              xor         edx,edx
  0000000140C0CA23: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C0CA28: E8 CF 2E 86 00     call        000000014146F8FC
  0000000140C0CA2D: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140C0CA35: 48 83 BC 24 80 00  cmp         qword ptr [rsp+0000000000000080h],0
                    00 00 00
  0000000140C0CA3E: 74 6D              je          0000000140C0CAAD
  0000000140C0CA40: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140C0CA48: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140C0CA50: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140C0CA58: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140C0CA60: 48 83 BC 24 B0 00  cmp         qword ptr [rsp+00000000000000B0h],0
                    00 00 00
  0000000140C0CA69: 74 25              je          0000000140C0CA90
  0000000140C0CA6B: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C0CA73: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C0CA76: BA 01 00 00 00     mov         edx,1
  0000000140C0CA7B: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140C0CA83: FF 50 18           call        qword ptr [rax+18h]
  0000000140C0CA86: 48 89 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],rax
                    00 00
  0000000140C0CA8E: EB 0C              jmp         0000000140C0CA9C
  0000000140C0CA90: 48 C7 84 24 10 01  mov         qword ptr [rsp+0000000000000110h],0
                    00 00 00 00 00 00
  0000000140C0CA9C: 48 C7 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],0
                    00 00 00 00 00 00
  0000000140C0CAA8: E9 DF FC FF FF     jmp         0000000140C0C78C
  0000000140C0CAAD: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140C0CAB5: 4C 8D 0D 1C 22 CD  lea         r9,[00000001438DECD8h]
                    02
  0000000140C0CABC: 4C 8D 05 2D 47 9B  lea         r8,[00000001435C11F0h]
                    02
  0000000140C0CAC3: 33 D2              xor         edx,edx
  0000000140C0CAC5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140C0CACA: E8 2D 2E 86 00     call        000000014146F8FC
  0000000140C0CACF: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140C0CAD7: 48 83 BC 24 88 00  cmp         qword ptr [rsp+0000000000000088h],0
                    00 00 00
  0000000140C0CAE0: 74 6D              je          0000000140C0CB4F
  0000000140C0CAE2: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140C0CAEA: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140C0CAF2: 48 8B 84 24 C8 00  mov         rax,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140C0CAFA: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140C0CB02: 48 83 BC 24 C0 00  cmp         qword ptr [rsp+00000000000000C0h],0
                    00 00 00
  0000000140C0CB0B: 74 25              je          0000000140C0CB32
  0000000140C0CB0D: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140C0CB15: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140C0CB18: BA 01 00 00 00     mov         edx,1
  0000000140C0CB1D: 48 8B 8C 24 C0 00  mov         rcx,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140C0CB25: FF 50 18           call        qword ptr [rax+18h]
  0000000140C0CB28: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  0000000140C0CB30: EB 0C              jmp         0000000140C0CB3E
  0000000140C0CB32: 48 C7 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],0
                    00 00 00 00 00 00
  0000000140C0CB3E: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  0000000140C0CB4A: E9 3D FC FF FF     jmp         0000000140C0C78C
  0000000140C0CB4F: E9 38 FC FF FF     jmp         0000000140C0C78C
  0000000140C0CB54: E9 53 FB FF FF     jmp         0000000140C0C6AC
  0000000140C0CB59: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140C0CB5E: E8 BD 49 04 00     call        0000000140C51520
  0000000140C0CB63: 90                 nop
  0000000140C0CB64: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140C0CB69: E8 32 B6 5F FF     call        00000001402081A0
  0000000140C0CB6E: 48 81 C4 28 01 00  add         rsp,128h
                    00
  0000000140C0CB75: C3                 ret

  Summary

     1E02000 .text

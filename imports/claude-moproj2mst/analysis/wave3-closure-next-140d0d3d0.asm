Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D0D3D0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D0D3D5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D0D3DA: 48 83 EC 58        sub         rsp,58h
  0000000140D0D3DE: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D0D3E7: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D3EC: 48 81 C1 A0 01 00  add         rcx,1A0h
                    00
  0000000140D0D3F3: E8 F8 DA F0 FF     call        0000000140C1AEF0
  0000000140D0D3F8: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D0D3FD: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D402: 48 81 C1 E0 01 00  add         rcx,1E0h
                    00
  0000000140D0D409: E8 E2 DA F0 FF     call        0000000140C1AEF0
  0000000140D0D40E: 48 85 C0           test        rax,rax
  0000000140D0D411: 74 16              je          0000000140D0D429
  0000000140D0D413: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D418: 48 81 C1 E0 01 00  add         rcx,1E0h
                    00
  0000000140D0D41F: E8 CC DA F0 FF     call        0000000140C1AEF0
  0000000140D0D424: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D0D429: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D42E: 48 81 C1 E0 01 00  add         rcx,1E0h
                    00
  0000000140D0D435: 33 D2              xor         edx,edx
  0000000140D0D437: E8 E4 F4 FA FF     call        0000000140CBC920
  0000000140D0D43C: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  0000000140D0D441: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D446: 48 81 C1 D8 01 00  add         rcx,1D8h
                    00
  0000000140D0D44D: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000140D0D452: E8 D9 55 00 00     call        0000000140D12A30
  0000000140D0D457: 90                 nop
  0000000140D0D458: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140D0D45D: E8 7E 76 2F FF     call        0000000140004AE0
  0000000140D0D462: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140D0D466: C7 44 24 24 00 00  mov         dword ptr [rsp+24h],0
                    00 00
  0000000140D0D46E: EB 0B              jmp         0000000140D0D47B
  0000000140D0D470: 8B 44 24 24        mov         eax,dword ptr [rsp+24h]
  0000000140D0D474: 83 C0 01           add         eax,1
  0000000140D0D477: 89 44 24 24        mov         dword ptr [rsp+24h],eax
  0000000140D0D47B: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140D0D47F: 39 44 24 24        cmp         dword ptr [rsp+24h],eax
  0000000140D0D483: 7D 66              jge         0000000140D0D4EB
  0000000140D0D485: 8B 54 24 24        mov         edx,dword ptr [rsp+24h]
  0000000140D0D489: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140D0D48E: E8 7D 37 45 FF     call        0000000140160C10
  0000000140D0D493: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140D0D496: E8 65 4D 00 00     call        0000000140D12200
  0000000140D0D49B: 48 3B 44 24 68     cmp         rax,qword ptr [rsp+68h]
  0000000140D0D4A0: 75 47              jne         0000000140D0D4E9
  0000000140D0D4A2: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140D0D4A7: 8B 54 24 24        mov         edx,dword ptr [rsp+24h]
  0000000140D0D4AB: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140D0D4B0: E8 5B 37 45 FF     call        0000000140160C10
  0000000140D0D4B5: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140D0D4B8: E8 03 4E 00 00     call        0000000140D122C0
  0000000140D0D4BD: 8B 54 24 24        mov         edx,dword ptr [rsp+24h]
  0000000140D0D4C1: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140D0D4C6: E8 45 37 45 FF     call        0000000140160C10
  0000000140D0D4CB: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000140D0D4CE: E8 2D 4D 00 00     call        0000000140D12200
  0000000140D0D4D3: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D4D8: 48 81 C1 E0 01 00  add         rcx,1E0h
                    00
  0000000140D0D4DF: 48 8B D0           mov         rdx,rax
  0000000140D0D4E2: E8 39 F4 FA FF     call        0000000140CBC920
  0000000140D0D4E7: EB 02              jmp         0000000140D0D4EB
  0000000140D0D4E9: EB 85              jmp         0000000140D0D470
  0000000140D0D4EB: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000140D0D4F0: 85 C0              test        eax,eax
  0000000140D0D4F2: 75 49              jne         0000000140D0D53D
  0000000140D0D4F4: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D4F9: 48 81 C1 C8 01 00  add         rcx,1C8h
                    00
  0000000140D0D500: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  0000000140D0D505: E8 56 9D 35 FF     call        0000000140067260
  0000000140D0D50A: 0F B6 C0           movzx       eax,al
  0000000140D0D50D: 85 C0              test        eax,eax
  0000000140D0D50F: 74 2C              je          0000000140D0D53D
  0000000140D0D511: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140D0D516: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D51B: E8 30 CA FE FF     call        0000000140CF9F50
  0000000140D0D520: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140D0D525: 48 81 C2 C8 01 00  add         rdx,1C8h
                    00
  0000000140D0D52C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D531: 48 81 C1 E0 01 00  add         rcx,1E0h
                    00
  0000000140D0D538: E8 C3 E4 F4 FF     call        0000000140C5BA00
  0000000140D0D53D: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000140D0D542: 85 C0              test        eax,eax
  0000000140D0D544: 75 49              jne         0000000140D0D58F
  0000000140D0D546: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D54B: 48 81 C1 A0 01 00  add         rcx,1A0h
                    00
  0000000140D0D552: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  0000000140D0D557: E8 04 9D 35 FF     call        0000000140067260
  0000000140D0D55C: 0F B6 C0           movzx       eax,al
  0000000140D0D55F: 85 C0              test        eax,eax
  0000000140D0D561: 74 2C              je          0000000140D0D58F
  0000000140D0D563: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140D0D568: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D56D: E8 7E C1 FE FF     call        0000000140CF96F0
  0000000140D0D572: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140D0D577: 48 81 C2 A0 01 00  add         rdx,1A0h
                    00
  0000000140D0D57E: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D583: 48 81 C1 E0 01 00  add         rcx,1E0h
                    00
  0000000140D0D58A: E8 71 E4 F4 FF     call        0000000140C5BA00
  0000000140D0D58F: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  0000000140D0D594: 85 C0              test        eax,eax
  0000000140D0D596: 75 49              jne         0000000140D0D5E1
  0000000140D0D598: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D59D: 48 81 C1 B8 01 00  add         rcx,1B8h
                    00
  0000000140D0D5A4: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  0000000140D0D5A9: E8 B2 9C 35 FF     call        0000000140067260
  0000000140D0D5AE: 0F B6 C0           movzx       eax,al
  0000000140D0D5B1: 85 C0              test        eax,eax
  0000000140D0D5B3: 74 2C              je          0000000140D0D5E1
  0000000140D0D5B5: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  0000000140D0D5BA: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D5BF: E8 AC C6 FE FF     call        0000000140CF9C70
  0000000140D0D5C4: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140D0D5C9: 48 81 C2 B8 01 00  add         rdx,1B8h
                    00
  0000000140D0D5D0: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140D0D5D5: 48 81 C1 E0 01 00  add         rcx,1E0h
                    00
  0000000140D0D5DC: E8 1F E4 F4 FF     call        0000000140C5BA00
  0000000140D0D5E1: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  0000000140D0D5E6: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D0D5EB: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140D0D5F0: E8 AB AB 4F FF     call        00000001402081A0
  0000000140D0D5F5: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D0D5FA: 48 83 C4 58        add         rsp,58h
  0000000140D0D5FE: C3                 ret

  Summary

     1E02000 .text

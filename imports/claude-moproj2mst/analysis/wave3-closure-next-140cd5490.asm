Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140CD5490: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140CD5495: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140CD549A: 48 81 EC D8 00 00  sub         rsp,0D8h
                    00
  0000000140CD54A1: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140CD54AD: 48 C7 44 24 40 00  mov         qword ptr [rsp+40h],0
                    00 00 00
  0000000140CD54B6: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140CD54BB: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  0000000140CD54C3: FF 15 DF 27 13 01  call        qword ptr [0000000141E07CA8h]
  0000000140CD54C9: 90                 nop
  0000000140CD54CA: C6 44 24 38 00     mov         byte ptr [rsp+38h],0
  0000000140CD54CF: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  0000000140CD54D4: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140CD54D9: E8 12 86 39 FF     call        000000014006DAF0
  0000000140CD54DE: 90                 nop
  0000000140CD54DF: 48 8D 44 24 48     lea         rax,[rsp+48h]
  0000000140CD54E4: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140CD54E9: EB 45              jmp         0000000140CD5530
  0000000140CD54EB: 33 C0              xor         eax,eax
  0000000140CD54ED: 83 F8 01           cmp         eax,1
  0000000140CD54F0: 74 0E              je          0000000140CD5500
  0000000140CD54F2: 48 C7 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],0
                    00 00 00 00 00 00
  0000000140CD54FE: EB 12              jmp         0000000140CD5512
  0000000140CD5500: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CD5505: E8 46 F7 4A FF     call        0000000140184C50
  0000000140CD550A: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140CD5512: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140CD551A: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140CD551F: E8 5C 6E 12 00     call        0000000140DFC380
  0000000140CD5524: 48 8B C8           mov         rcx,rax
  0000000140CD5527: 48 83 C1 10        add         rcx,10h
  0000000140CD552B: E8 D0 42 11 00     call        0000000140DE9800
  0000000140CD5530: 33 C0              xor         eax,eax
  0000000140CD5532: 83 F8 01           cmp         eax,1
  0000000140CD5535: 74 0E              je          0000000140CD5545
  0000000140CD5537: 48 C7 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],0
                    00 00 00 00 00 00
  0000000140CD5543: EB 12              jmp         0000000140CD5557
  0000000140CD5545: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CD554A: E8 01 F7 4A FF     call        0000000140184C50
  0000000140CD554F: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140CD5557: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140CD555F: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140CD5564: E8 17 6E 12 00     call        0000000140DFC380
  0000000140CD5569: 48 8B C8           mov         rcx,rax
  0000000140CD556C: E8 0F 6B 12 00     call        0000000140DFC080
  0000000140CD5571: 0F B6 C0           movzx       eax,al
  0000000140CD5574: 85 C0              test        eax,eax
  0000000140CD5576: 0F 84 FD 01 00 00  je          0000000140CD5779
  0000000140CD557C: 33 C0              xor         eax,eax
  0000000140CD557E: 83 F8 01           cmp         eax,1
  0000000140CD5581: 74 0E              je          0000000140CD5591
  0000000140CD5583: 48 C7 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],0
                    00 00 00 00 00 00
  0000000140CD558F: EB 12              jmp         0000000140CD55A3
  0000000140CD5591: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CD5596: E8 B5 F6 4A FF     call        0000000140184C50
  0000000140CD559B: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140CD55A3: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140CD55AB: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140CD55B0: E8 CB 6D 12 00     call        0000000140DFC380
  0000000140CD55B5: 48 8B C8           mov         rcx,rax
  0000000140CD55B8: 48 83 C1 10        add         rcx,10h
  0000000140CD55BC: E8 3F 9E 97 FF     call        000000014064F400
  0000000140CD55C1: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140CD55C4: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140CD55C9: EB 5C              jmp         0000000140CD5627
  0000000140CD55CB: 33 C0              xor         eax,eax
  0000000140CD55CD: 83 F8 01           cmp         eax,1
  0000000140CD55D0: 74 0E              je          0000000140CD55E0
  0000000140CD55D2: 48 C7 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],0
                    00 00 00 00 00 00
  0000000140CD55DE: EB 12              jmp         0000000140CD55F2
  0000000140CD55E0: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CD55E5: E8 66 F6 4A FF     call        0000000140184C50
  0000000140CD55EA: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  0000000140CD55F2: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  0000000140CD55FA: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140CD55FF: E8 7C 6D 12 00     call        0000000140DFC380
  0000000140CD5604: 48 83 C0 08        add         rax,8
  0000000140CD5608: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  0000000140CD5610: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140CD5618: 8B 08              mov         ecx,dword ptr [rax]
  0000000140CD561A: 83 E9 01           sub         ecx,1
  0000000140CD561D: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140CD5625: 89 08              mov         dword ptr [rax],ecx
  0000000140CD5627: 33 C0              xor         eax,eax
  0000000140CD5629: 83 F8 01           cmp         eax,1
  0000000140CD562C: 74 0E              je          0000000140CD563C
  0000000140CD562E: 48 C7 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],0
                    00 00 00 00 00 00
  0000000140CD563A: EB 12              jmp         0000000140CD564E
  0000000140CD563C: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CD5641: E8 0A F6 4A FF     call        0000000140184C50
  0000000140CD5646: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  0000000140CD564E: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  0000000140CD5656: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140CD565B: E8 20 6D 12 00     call        0000000140DFC380
  0000000140CD5660: 83 78 08 00        cmp         dword ptr [rax+8],0
  0000000140CD5664: 0F 84 0A 01 00 00  je          0000000140CD5774
  0000000140CD566A: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140CD566F: FF 15 13 3D 13 01  call        qword ptr [0000000141E09388h]
  0000000140CD5675: 48 85 C0           test        rax,rax
  0000000140CD5678: 0F 85 F1 00 00 00  jne         0000000140CD576F
  0000000140CD567E: 48 8D 15 F3 83 D0  lea         rdx,[00000001429DDA78h]
                    01
  0000000140CD5685: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140CD568D: FF 15 D5 ED 12 01  call        qword ptr [0000000141E04468h]
  0000000140CD5693: 0F B6 C0           movzx       eax,al
  0000000140CD5696: 85 C0              test        eax,eax
  0000000140CD5698: 74 5E              je          0000000140CD56F8
  0000000140CD569A: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CD56A2: 4C 8D 0D 67 15 A3  lea         r9,[0000000143706C10h]
                    02
  0000000140CD56A9: 4C 8D 05 40 BB 8E  lea         r8,[00000001435C11F0h]
                    02
  0000000140CD56B0: 33 D2              xor         edx,edx
  0000000140CD56B2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140CD56B7: E8 40 A2 79 00     call        000000014146F8FC
  0000000140CD56BC: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140CD56C1: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  0000000140CD56C7: 74 2F              je          0000000140CD56F8
  0000000140CD56C9: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140CD56CE: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140CD56D6: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140CD56DB: E8 40 BE F7 FF     call        0000000140C51520
  0000000140CD56E0: 90                 nop
  0000000140CD56E1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CD56E6: E8 B5 2A 53 FF     call        00000001402081A0
  0000000140CD56EB: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140CD56F3: E9 A9 00 00 00     jmp         0000000140CD57A1
  0000000140CD56F8: 48 8D 15 91 83 D0  lea         rdx,[00000001429DDA90h]
                    01
  0000000140CD56FF: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  0000000140CD5707: FF 15 5B ED 12 01  call        qword ptr [0000000141E04468h]
  0000000140CD570D: 0F B6 C0           movzx       eax,al
  0000000140CD5710: 85 C0              test        eax,eax
  0000000140CD5712: 74 5B              je          0000000140CD576F
  0000000140CD5714: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140CD571C: 4C 8D 0D BD 0B 96  lea         r9,[00000001436362E0h]
                    02
  0000000140CD5723: 4C 8D 05 C6 BA 8E  lea         r8,[00000001435C11F0h]
                    02
  0000000140CD572A: 33 D2              xor         edx,edx
  0000000140CD572C: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140CD5731: E8 C6 A1 79 00     call        000000014146F8FC
  0000000140CD5736: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140CD573B: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  0000000140CD5741: 74 2C              je          0000000140CD576F
  0000000140CD5743: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140CD5748: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140CD5750: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140CD5755: E8 C6 BD F7 FF     call        0000000140C51520
  0000000140CD575A: 90                 nop
  0000000140CD575B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CD5760: E8 3B 2A 53 FF     call        00000001402081A0
  0000000140CD5765: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140CD576D: EB 32              jmp         0000000140CD57A1
  0000000140CD576F: E9 57 FE FF FF     jmp         0000000140CD55CB
  0000000140CD5774: E9 72 FD FF FF     jmp         0000000140CD54EB
  0000000140CD5779: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140CD577E: E8 9D BD F7 FF     call        0000000140C51520
  0000000140CD5783: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00 00
  0000000140CD578F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140CD5794: E8 07 2A 53 FF     call        00000001402081A0
  0000000140CD5799: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000140CD57A1: 48 81 C4 D8 00 00  add         rsp,0D8h
                    00
  0000000140CD57A8: C3                 ret

  Summary

     1E02000 .text

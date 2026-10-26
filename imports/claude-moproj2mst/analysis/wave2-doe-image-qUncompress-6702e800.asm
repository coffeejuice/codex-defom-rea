Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006702E800: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006702E805: 53                 push        rbx
  000000006702E806: 55                 push        rbp
  000000006702E807: 56                 push        rsi
  000000006702E808: 57                 push        rdi
  000000006702E809: 41 54              push        r12
  000000006702E80B: 48 83 EC 40        sub         rsp,40h
  000000006702E80F: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006702E818: 41 8B E8           mov         ebp,r8d
  000000006702E81B: 48 8B F2           mov         rsi,rdx
  000000006702E81E: 48 8B F9           mov         rdi,rcx
  000000006702E821: 33 DB              xor         ebx,ebx
  000000006702E823: 89 9C 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],ebx
                    00
  000000006702E82A: 48 85 D2           test        rdx,rdx
  000000006702E82D: 75 26              jne         000000006702E855
  000000006702E82F: 48 8D 0D DA 1B 18  lea         rcx,[00000000671B0410h]
                    00
  000000006702E836: E8 A5 E6 FE FF     call        000000006701CEE0
  000000006702E83B: 4C 8D 1D BE 20 2C  lea         r11,[00000000672F0900h]
                    00
  000000006702E842: 4C 89 1F           mov         qword ptr [rdi],r11
  000000006702E845: F0 83 05 B3 20 2C  lock add    dword ptr [00000000672F0900h],1
                    00 01
  000000006702E84D: 48 8B C7           mov         rax,rdi
  000000006702E850: E9 C1 02 00 00     jmp         000000006702EB16
  000000006702E855: 41 83 F8 04        cmp         r8d,4
  000000006702E859: 7F 3F              jg          000000006702E89A
  000000006702E85B: 41 83 F8 04        cmp         r8d,4
  000000006702E85F: 7C 13              jl          000000006702E874
  000000006702E861: 38 1A              cmp         byte ptr [rdx],bl
  000000006702E863: 75 0F              jne         000000006702E874
  000000006702E865: 38 5A 01           cmp         byte ptr [rdx+1],bl
  000000006702E868: 75 0A              jne         000000006702E874
  000000006702E86A: 38 5A 02           cmp         byte ptr [rdx+2],bl
  000000006702E86D: 75 05              jne         000000006702E874
  000000006702E86F: 38 5A 03           cmp         byte ptr [rdx+3],bl
  000000006702E872: 74 0C              je          000000006702E880
  000000006702E874: 48 8D 0D 6D 1B 18  lea         rcx,[00000000671B03E8h]
                    00
  000000006702E87B: E8 60 E6 FE FF     call        000000006701CEE0
  000000006702E880: 4C 8D 1D 79 20 2C  lea         r11,[00000000672F0900h]
                    00
  000000006702E887: 4C 89 1F           mov         qword ptr [rdi],r11
  000000006702E88A: F0 83 05 6E 20 2C  lock add    dword ptr [00000000672F0900h],1
                    00 01
  000000006702E892: 48 8B C7           mov         rax,rdi
  000000006702E895: E9 7C 02 00 00     jmp         000000006702EB16
  000000006702E89A: 0F B6 0A           movzx       ecx,byte ptr [rdx]
  000000006702E89D: C1 E1 08           shl         ecx,8
  000000006702E8A0: 0F B6 42 01        movzx       eax,byte ptr [rdx+1]
  000000006702E8A4: 0B C8              or          ecx,eax
  000000006702E8A6: C1 E1 08           shl         ecx,8
  000000006702E8A9: 0F B6 42 02        movzx       eax,byte ptr [rdx+2]
  000000006702E8AD: 0B C8              or          ecx,eax
  000000006702E8AF: C1 E1 08           shl         ecx,8
  000000006702E8B2: 0F B6 42 03        movzx       eax,byte ptr [rdx+3]
  000000006702E8B6: 0B C8              or          ecx,eax
  000000006702E8B8: 89 4C 24 24        mov         dword ptr [rsp+24h],ecx
  000000006702E8BC: C7 44 24 20 01 00  mov         dword ptr [rsp+20h],1
                    00 00
  000000006702E8C4: 48 8D 44 24 20     lea         rax,[rsp+20h]
  000000006702E8C9: 48 8D 54 24 24     lea         rdx,[rsp+24h]
  000000006702E8CE: 83 F9 01           cmp         ecx,1
  000000006702E8D1: 48 0F 43 C2        cmovae      rax,rdx
  000000006702E8D5: 8B 00              mov         eax,dword ptr [rax]
  000000006702E8D7: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  000000006702E8DB: 48 89 5C 24 28     mov         qword ptr [rsp+28h],rbx
  000000006702E8E0: 44 8B E0           mov         r12d,eax
  000000006702E8E3: 3D E0 FF FF 7F     cmp         eax,7FFFFFE0h
  000000006702E8E8: 0F 83 ED 01 00 00  jae         000000006702EADB
  000000006702E8EE: 66 90              nop
  000000006702E8F0: 8B D0              mov         edx,eax
  000000006702E8F2: 48 83 C2 20        add         rdx,20h
  000000006702E8F6: 48 8B CB           mov         rcx,rbx
  000000006702E8F9: E8 C2 1A FF FF     call        00000000670203C0
  000000006702E8FE: 48 8B D8           mov         rbx,rax
  000000006702E901: 48 85 C0           test        rax,rax
  000000006702E904: 0F 84 94 01 00 00  je          000000006702EA9E
  000000006702E90A: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000006702E90F: 44 8D 4D FC        lea         r9d,[rbp-4]
  000000006702E913: 4C 8D 46 04        lea         r8,[rsi+4]
  000000006702E917: 48 8D 48 18        lea         rcx,[rax+18h]
  000000006702E91B: 48 8D 54 24 78     lea         rdx,[rsp+78h]

  Summary

      1A7000 .text

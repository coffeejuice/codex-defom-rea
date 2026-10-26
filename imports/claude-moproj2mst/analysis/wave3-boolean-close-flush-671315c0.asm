Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000671315C0: 4C 8B DC           mov         r11,rsp
  00000000671315C3: 53                 push        rbx
  00000000671315C4: 55                 push        rbp
  00000000671315C5: 56                 push        rsi
  00000000671315C6: 57                 push        rdi
  00000000671315C7: 41 54              push        r12
  00000000671315C9: 41 55              push        r13
  00000000671315CB: 41 56              push        r14
  00000000671315CD: 41 57              push        r15
  00000000671315CF: 48 81 EC B8 00 00  sub         rsp,0B8h
                    00
  00000000671315D6: 48 C7 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000000671315E2: 49 8B F1           mov         rsi,r9
  00000000671315E5: 4D 8B F0           mov         r14,r8
  00000000671315E8: 4C 8B E2           mov         r12,rdx
  00000000671315EB: 48 8B E9           mov         rbp,rcx
  00000000671315EE: 45 33 ED           xor         r13d,r13d
  00000000671315F1: 45 89 6B 08        mov         dword ptr [r11+8],r13d
  00000000671315F5: 48 89 4C 24 70     mov         qword ptr [rsp+70h],rcx
  00000000671315FA: 49 89 53 80        mov         qword ptr [r11-80h],rdx
  00000000671315FE: 4D 89 43 88        mov         qword ptr [r11-78h],r8
  0000000067131602: 4D 89 4B 90        mov         qword ptr [r11-70h],r9
  0000000067131606: 49 8D 43 28        lea         rax,[r11+28h]
  000000006713160A: 49 89 43 98        mov         qword ptr [r11-68h],rax
  000000006713160E: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000067131613: 33 C9              xor         ecx,ecx
  0000000067131615: E8 66 C6 EE FF     call        000000006701DC80
  000000006713161A: 84 C0              test        al,al
  000000006713161C: 74 07              je          0000000067131625
  000000006713161E: B0 01              mov         al,1
  0000000067131620: E9 6D 06 00 00     jmp         0000000067131C92
  0000000067131625: 8B 84 24 20 01 00  mov         eax,dword ptr [rsp+0000000000000120h]
                    00
  000000006713162C: 83 F8 03           cmp         eax,3
  000000006713162F: 41 0F 44 C5        cmove       eax,r13d
  0000000067131633: 89 84 24 20 01 00  mov         dword ptr [rsp+0000000000000120h],eax
                    00

  Summary

      1A7000 .text

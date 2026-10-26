Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067119B20: 4C 8B DC           mov         r11,rsp
  0000000067119B23: 49 89 4B 08        mov         qword ptr [r11+8],rcx
  0000000067119B27: 55                 push        rbp
  0000000067119B28: 56                 push        rsi
  0000000067119B29: 57                 push        rdi
  0000000067119B2A: 41 54              push        r12
  0000000067119B2C: 41 55              push        r13
  0000000067119B2E: 41 56              push        r14
  0000000067119B30: 41 57              push        r15
  0000000067119B32: 48 83 EC 60        sub         rsp,60h
  0000000067119B36: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000067119B3F: 49 89 5B 10        mov         qword ptr [r11+10h],rbx
  0000000067119B43: 4D 8B F1           mov         r14,r9
  0000000067119B46: 49 8B E8           mov         rbp,r8
  0000000067119B49: 4C 8B FA           mov         r15,rdx
  0000000067119B4C: 48 8B F1           mov         rsi,rcx
  0000000067119B4F: 45 33 E4           xor         r12d,r12d
  0000000067119B52: 45 89 63 18        mov         dword ptr [r11+18h],r12d
  0000000067119B56: 48 8D 05 D3 6F 1D  lea         rax,[00000000672F0B30h]
                    00
  0000000067119B5D: 49 89 43 98        mov         qword ptr [r11-68h],rax
  0000000067119B61: F0 83 05 C7 6F 1D  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  0000000067119B69: 4D 85 C0           test        r8,r8
  0000000067119B6C: 75 2F              jne         0000000067119B9D
  0000000067119B6E: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000067119B73: 48 89 01           mov         qword ptr [rcx],rax
  0000000067119B76: F0 83 00 01        lock add    dword ptr [rax],1
  0000000067119B7A: 41 C7 43 18 01 00  mov         dword ptr [r11+18h],1
                    00 00
  0000000067119B82: 49 8B 43 98        mov         rax,qword ptr [r11-68h]
  0000000067119B86: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067119B8A: 75 09              jne         0000000067119B95
  0000000067119B8C: 49 8B 4B 98        mov         rcx,qword ptr [r11-68h]
  0000000067119B90: E8 3B 65 F5 FF     call        00000000670700D0
  0000000067119B95: 48 8B C6           mov         rax,rsi
  0000000067119B98: E9 E8 01 00 00     jmp         0000000067119D85
  0000000067119B9D: 83 CF FF           or          edi,0FFFFFFFFh
  0000000067119BA0: 44 8B AC 24 C8 00  mov         r13d,dword ptr [rsp+00000000000000C8h]
                    00 00
  0000000067119BA8: 48 8B 05 71 A6 1D  mov         rax,qword ptr [00000000672F4220h]
                    00
  0000000067119BAF: 48 85 C0           test        rax,rax
  0000000067119BB2: 0F 84 AA 00 00 00  je          0000000067119C62
  0000000067119BB8: 48 8B 50 08        mov         rdx,qword ptr [rax+8]
  0000000067119BBC: 48 8B 8A 98 00 00  mov         rcx,qword ptr [rdx+0000000000000098h]
                    00
  0000000067119BC3: 8B 41 08           mov         eax,dword ptr [rcx+8]
  0000000067119BC6: 39 41 0C           cmp         dword ptr [rcx+0Ch],eax
  0000000067119BC9: 0F 84 93 00 00 00  je          0000000067119C62
  0000000067119BCF: 48 63 41 08        movsxd      rax,dword ptr [rcx+8]
  0000000067119BD3: 48 8D 5C C1 18     lea         rbx,[rcx+rax*8+18h]
  0000000067119BD8: 48 63 41 0C        movsxd      rax,dword ptr [rcx+0Ch]
  0000000067119BDC: 48 8D 44 C1 18     lea         rax,[rcx+rax*8+18h]
  0000000067119BE1: 48 3B D8           cmp         rbx,rax
  0000000067119BE4: 74 7C              je          0000000067119C62
  0000000067119BE6: 66 66 0F 1F 84 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00 00
  0000000067119BF0: 44 89 6C 24 28     mov         dword ptr [rsp+28h],r13d
  0000000067119BF5: 4C 89 74 24 20     mov         qword ptr [rsp+20h],r14
  0000000067119BFA: 4C 8B CD           mov         r9,rbp
  0000000067119BFD: 4D 8B C7           mov         r8,r15
  0000000067119C00: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  0000000067119C05: 48 8B 0B           mov         rcx,qword ptr [rbx]
  0000000067119C08: E8 53 CC 01 00     call        0000000067136860
  0000000067119C0D: 90                 nop
  0000000067119C0E: 48 8B D0           mov         rdx,rax
  0000000067119C11: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000067119C16: E8 B5 67 F5 FF     call        00000000670703D0
  0000000067119C1B: 90                 nop
  0000000067119C1C: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  0000000067119C21: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067119C25: 75 0A              jne         0000000067119C31
  0000000067119C27: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000067119C2C: E8 9F 64 F5 FF     call        00000000670700D0
  0000000067119C31: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000067119C36: 83 79 08 00        cmp         dword ptr [rcx+8],0
  0000000067119C3A: 75 2B              jne         0000000067119C67
  0000000067119C3C: 48 83 C3 08        add         rbx,8
  0000000067119C40: 48 8B 05 D9 A5 1D  mov         rax,qword ptr [00000000672F4220h]
                    00
  0000000067119C47: 48 8B 50 08        mov         rdx,qword ptr [rax+8]
  0000000067119C4B: 4C 8B 82 98 00 00  mov         r8,qword ptr [rdx+0000000000000098h]
                    00
  0000000067119C52: 49 63 40 0C        movsxd      rax,dword ptr [r8+0Ch]
  0000000067119C56: 49 8D 44 C0 18     lea         rax,[r8+rax*8+18h]
  0000000067119C5B: 48 3B D8           cmp         rbx,rax
  0000000067119C5E: 75 90              jne         0000000067119BF0
  0000000067119C60: EB 05              jmp         0000000067119C67
  0000000067119C62: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000067119C67: 83 79 08 00        cmp         dword ptr [rcx+8],0
  0000000067119C6B: 0F 85 D7 00 00 00  jne         0000000067119D48
  0000000067119C71: 83 BC 24 C0 00 00  cmp         dword ptr [rsp+00000000000000C0h],1
                    00 01
  0000000067119C79: 75 3D              jne         0000000067119CB8
  0000000067119C7B: 44 8B C7           mov         r8d,edi
  0000000067119C7E: 48 8B D5           mov         rdx,rbp
  0000000067119C81: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000067119C86: E8 C5 A6 F5 FF     call        0000000067074350
  0000000067119C8B: 90                 nop
  0000000067119C8C: 48 8B D0           mov         rdx,rax
  0000000067119C8F: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000067119C94: E8 37 67 F5 FF     call        00000000670703D0
  0000000067119C99: 90                 nop
  0000000067119C9A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000067119C9F: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067119CA3: 0F 85 9F 00 00 00  jne         0000000067119D48
  0000000067119CA9: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000067119CAE: E8 1D 64 F5 FF     call        00000000670700D0
  0000000067119CB3: E9 90 00 00 00     jmp         0000000067119D48
  0000000067119CB8: E8 33 64 03 00     call        00000000671500F0
  0000000067119CBD: 49 8B D4           mov         rdx,r12
  0000000067119CC0: 84 C0              test        al,al
  0000000067119CC2: 48 0F 45 15 76 A7  cmovne      rdx,qword ptr [00000000672F4440h]
                    1D 00
  0000000067119CCA: 48 85 D2           test        rdx,rdx
  0000000067119CCD: 74 45              je          0000000067119D14
  0000000067119CCF: E8 1C 64 03 00     call        00000000671500F0
  0000000067119CD4: 84 C0              test        al,al
  0000000067119CD6: 4C 0F 45 25 62 A7  cmovne      r12,qword ptr [00000000672F4440h]
                    1D 00
  0000000067119CDE: 4C 8B C5           mov         r8,rbp
  0000000067119CE1: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  0000000067119CE6: 49 8B CC           mov         rcx,r12
  0000000067119CE9: E8 12 12 03 00     call        000000006714AF00
  0000000067119CEE: 90                 nop
  0000000067119CEF: 48 8B D0           mov         rdx,rax
  0000000067119CF2: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000067119CF7: E8 D4 66 F5 FF     call        00000000670703D0
  0000000067119CFC: 90                 nop
  0000000067119CFD: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  0000000067119D02: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067119D06: 75 40              jne         0000000067119D48
  0000000067119D08: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000067119D0D: E8 BE 63 F5 FF     call        00000000670700D0
  0000000067119D12: EB 34              jmp         0000000067119D48
  0000000067119D14: 44 8B C7           mov         r8d,edi
  0000000067119D17: 48 8B D5           mov         rdx,rbp
  0000000067119D1A: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000067119D1F: E8 BC A4 F5 FF     call        00000000670741E0
  0000000067119D24: 90                 nop
  0000000067119D25: 48 8B D0           mov         rdx,rax
  0000000067119D28: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000067119D2D: E8 9E 66 F5 FF     call        00000000670703D0
  0000000067119D32: 90                 nop
  0000000067119D33: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000067119D38: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067119D3C: 75 0A              jne         0000000067119D48
  0000000067119D3E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000067119D43: E8 88 63 F5 FF     call        00000000670700D0
  0000000067119D48: 41 8B D5           mov         edx,r13d
  0000000067119D4B: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000067119D50: E8 9B ED FF FF     call        0000000067118AF0
  0000000067119D55: 4C 8B 5C 24 30     mov         r11,qword ptr [rsp+30h]
  0000000067119D5A: 4C 89 1E           mov         qword ptr [rsi],r11
  0000000067119D5D: F0 41 83 03 01     lock add    dword ptr [r11],1
  0000000067119D62: C7 84 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],1
                    00 01 00 00 00
  0000000067119D6D: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  0000000067119D72: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  0000000067119D76: 75 0A              jne         0000000067119D82
  0000000067119D78: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000067119D7D: E8 4E 63 F5 FF     call        00000000670700D0
  0000000067119D82: 48 8B C6           mov         rax,rsi
  0000000067119D85: 48 8B 9C 24 A8 00  mov         rbx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000067119D8D: 48 83 C4 60        add         rsp,60h
  0000000067119D91: 41 5F              pop         r15
  0000000067119D93: 41 5E              pop         r14
  0000000067119D95: 41 5D              pop         r13
  0000000067119D97: 41 5C              pop         r12
  0000000067119D99: 5F                 pop         rdi
  0000000067119D9A: 5E                 pop         rsi
  0000000067119D9B: 5D                 pop         rbp
  0000000067119D9C: C3                 ret
  0000000067119D9D: CC                 int         3

  Summary

      1A7000 .text

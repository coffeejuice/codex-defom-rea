Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065065F1F: 48 89 7C 24 38     mov         qword ptr [rsp+38h],rdi
  0000000065065F24: 48 8B 7B 08        mov         rdi,qword ptr [rbx+8]
  0000000065065F28: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065065F2C: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065065F2F: 8B C1              mov         eax,ecx
  0000000065065F31: C1 E8 04           shr         eax,4
  0000000065065F34: A8 01              test        al,1
  0000000065065F36: 0F 85 E8 00 00 00  jne         0000000065066024
  0000000065065F3C: C1 E9 08           shr         ecx,8
  0000000065065F3F: F6 C1 01           test        cl,1
  0000000065065F42: 0F 85 DC 00 00 00  jne         0000000065066024
  0000000065065F48: 48 8B CF           mov         rcx,rdi
  0000000065065F4B: 48 89 74 24 30     mov         qword ptr [rsp+30h],rsi
  0000000065065F50: E8 BB E4 FF FF     call        0000000065064410
  0000000065065F55: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  0000000065065F59: 48 8B 91 88 00 00  mov         rdx,qword ptr [rcx+0000000000000088h]
                    00
  0000000065065F60: 48 8B F0           mov         rsi,rax
  0000000065065F63: 48 85 D2           test        rdx,rdx
  0000000065065F66: 74 33              je          0000000065065F9B
  0000000065065F68: F6 42 5C 10        test        byte ptr [rdx+5Ch],10h
  0000000065065F6C: 74 2D              je          0000000065065F9B
  0000000065065F6E: 48 8B CF           mov         rcx,rdi
  0000000065065F71: E8 7A 78 FF FF     call        000000006505D7F0
  0000000065065F76: BA 04 00 00 00     mov         edx,4
  0000000065065F7B: 48 8B CE           mov         rcx,rsi
  0000000065065F7E: 44 8B C0           mov         r8d,eax
  0000000065065F81: E8 6A 3C FE FF     call        0000000065049BF0
  0000000065065F86: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065065F89: 83 79 04 00        cmp         dword ptr [rcx+4],0
  0000000065065F8D: 74 0C              je          0000000065065F9B
  0000000065065F8F: 48 8B C8           mov         rcx,rax
  0000000065065F92: E8 F9 60 0B 00     call        000000006511C090
  0000000065065F97: 84 C0              test        al,al
  0000000065065F99: 75 42              jne         0000000065065FDD
  0000000065065F9B: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065065F9F: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  0000000065065FA3: 74 52              je          0000000065065FF7
  0000000065065FA5: 8B 40 08           mov         eax,dword ptr [rax+8]
  0000000065065FA8: C1 E8 09           shr         eax,9
  0000000065065FAB: A8 01              test        al,1
  0000000065065FAD: 75 48              jne         0000000065065FF7
  0000000065065FAF: 48 8B CF           mov         rcx,rdi
  0000000065065FB2: E8 59 E4 FF FF     call        0000000065064410
  0000000065065FB7: BA 04 00 00 00     mov         edx,4
  0000000065065FBC: 44 8D 42 06        lea         r8d,[rdx+6]
  0000000065065FC0: 48 8B C8           mov         rcx,rax
  0000000065065FC3: E8 28 3C FE FF     call        0000000065049BF0
  0000000065065FC8: 48 8B 08           mov         rcx,qword ptr [rax]
  0000000065065FCB: 83 79 04 00        cmp         dword ptr [rcx+4],0
  0000000065065FCF: 74 26              je          0000000065065FF7
  0000000065065FD1: 48 8B C8           mov         rcx,rax
  0000000065065FD4: E8 B7 60 0B 00     call        000000006511C090
  0000000065065FD9: 84 C0              test        al,al
  0000000065065FDB: 74 1A              je          0000000065065FF7
  0000000065065FDD: B2 01              mov         dl,1
  0000000065065FDF: 48 8B CB           mov         rcx,rbx
  0000000065065FE2: E8 69 5F FF FF     call        000000006505BF50
  0000000065065FE7: 48 8B 74 24 30     mov         rsi,qword ptr [rsp+30h]
  0000000065065FEC: 48 8B 7C 24 38     mov         rdi,qword ptr [rsp+38h]
  0000000065065FF1: 48 83 C4 20        add         rsp,20h
  0000000065065FF5: 5B                 pop         rbx
  0000000065065FF6: C3                 ret
  0000000065065FF7: 8B 83 D8 01 00 00  mov         eax,dword ptr [rbx+00000000000001D8h]
  0000000065065FFD: 0F
  0000000065065FFE: BA
  0000000065065FFF: E0

  Summary

      695000 .text

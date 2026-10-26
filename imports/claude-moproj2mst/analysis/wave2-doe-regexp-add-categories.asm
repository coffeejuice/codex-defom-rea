Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006705AF10: 40 53              push        rbx
  000000006705AF12: 48 83 EC 20        sub         rsp,20h
  000000006705AF16: 09 11              or          dword ptr [rcx],edx
  000000006705AF18: 48 8B 41 18        mov         rax,qword ptr [rcx+18h]
  000000006705AF1C: 48 8D 59 18        lea         rbx,[rcx+18h]
  000000006705AF20: 44 8B 40 04        mov         r8d,dword ptr [rax+4]
  000000006705AF24: 41 83 F8 40        cmp         r8d,40h
  000000006705AF28: 7C 19              jl          000000006705AF43
  000000006705AF2A: F6 40 0C 02        test        byte ptr [rax+0Ch],2
  000000006705AF2E: 75 29              jne         000000006705AF59
  000000006705AF30: 83 78 08 40        cmp         dword ptr [rax+8],40h
  000000006705AF34: 7E 23              jle         000000006705AF59
  000000006705AF36: 41 8B C0           mov         eax,r8d
  000000006705AF39: 83 E0 FE           and         eax,0FFFFFFFEh
  000000006705AF3C: 3D 80 00 00 00     cmp         eax,80h
  000000006705AF41: 7E 16              jle         000000006705AF59
  000000006705AF43: 45 33 C9           xor         r9d,r9d
  000000006705AF46: 41 8D 51 40        lea         edx,[r9+40h]
  000000006705AF4A: 45 8D 41 04        lea         r8d,[r9+4]
  000000006705AF4E: 8D 4A D4           lea         ecx,[rdx-2Ch]
  000000006705AF51: E8 4A 16 02 00     call        000000006707C5A0
  000000006705AF56: 44 8B C0           mov         r8d,eax
  000000006705AF59: BA 40 00 00 00     mov         edx,40h
  000000006705AF5E: 48 8B CB           mov         rcx,rbx
  000000006705AF61: E8 3A D3 0A 00     call        00000000671082A0
  000000006705AF66: 4C 8B 1B           mov         r11,qword ptr [rbx]
  000000006705AF69: 49 63 43 08        movsxd      rax,dword ptr [r11+8]
  000000006705AF6D: 85 C0              test        eax,eax
  000000006705AF6F: 74 1B              je          000000006705AF8C
  000000006705AF71: 49 8D 4C 83 10     lea         rcx,[r11+rax*4+10h]
  000000006705AF76: 49 8D 43 10        lea         rax,[r11+10h]
  000000006705AF7A: 48 3B C8           cmp         rcx,rax
  000000006705AF7D: 74 0D              je          000000006705AF8C
  000000006705AF7F: 33 D2              xor         edx,edx
  000000006705AF81: 48 83 E9 04        sub         rcx,4
  000000006705AF85: 89 11              mov         dword ptr [rcx],edx
  000000006705AF87: 48 3B C8           cmp         rcx,rax
  000000006705AF8A: 75 F5              jne         000000006705AF81
  000000006705AF8C: 48 83 C4 20        add         rsp,20h
  000000006705AF90: 5B                 pop         rbx
  000000006705AF91: C3                 ret

  Summary

      1A7000 .text

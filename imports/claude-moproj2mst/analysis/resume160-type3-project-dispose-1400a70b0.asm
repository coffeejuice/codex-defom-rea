Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400A70B0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400A70B4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400A70B9: 48 83 EC 78        sub         rsp,78h
  00000001400A70BD: 48 C7 44 24 60 FE  mov         qword ptr [rsp+60h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400A70C6: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400A70CE: E8 8D 02 00 00     call        00000001400A7360
  00000001400A70D3: 83 F8 01           cmp         eax,1
  00000001400A70D6: 7F 05              jg          00000001400A70DD
  00000001400A70D8: E9 5F 01 00 00     jmp         00000001400A723C
  00000001400A70DD: 83 BC 24 88 00 00  cmp         dword ptr [rsp+0000000000000088h],0FFFFFFFFh
                    00 FF
  00000001400A70E5: 75 12              jne         00000001400A70F9
  00000001400A70E7: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400A70EF: 8B 40 50           mov         eax,dword ptr [rax+50h]
  00000001400A70F2: 89 84 24 88 00 00  mov         dword ptr [rsp+0000000000000088h],eax
                    00
  00000001400A70F9: 83 BC 24 88 00 00  cmp         dword ptr [rsp+0000000000000088h],0
                    00 00
  00000001400A7101: 75 05              jne         00000001400A7108
  00000001400A7103: E9 34 01 00 00     jmp         00000001400A723C
  00000001400A7108: 44 8B 84 24 88 00  mov         r8d,dword ptr [rsp+0000000000000088h]
                    00 00
  00000001400A7110: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001400A7115: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400A711D: E8 BE 07 00 00     call        00000001400A78E0
  00000001400A7122: 90                 nop
  00000001400A7123: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001400A7128: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400A7130: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  00000001400A7134: FF 15 46 55 D6 01  call        qword ptr [0000000141E0C680h]
  00000001400A713A: 90                 nop
  00000001400A713B: 4C 8D 44 24 40     lea         r8,[rsp+40h]
  00000001400A7140: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000001400A7145: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400A714A: FF 15 78 54 D6 01  call        qword ptr [0000000141E0C5C8h]
  00000001400A7150: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001400A7155: FF 15 4D 55 D6 01  call        qword ptr [0000000141E0C6A8h]
  00000001400A715B: C7 44 24 20 FF FF  mov         dword ptr [rsp+20h],0FFFFFFFFh
                    FF FF
  00000001400A7163: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400A716B: E8 F0 01 00 00     call        00000001400A7360
  00000001400A7170: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  00000001400A7174: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  00000001400A7178: 83 E8 01           sub         eax,1
  00000001400A717B: 39 84 24 88 00 00  cmp         dword ptr [rsp+0000000000000088h],eax
                    00
  00000001400A7182: 7C 0D              jl          00000001400A7191
  00000001400A7184: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  00000001400A7188: 83 E8 02           sub         eax,2
  00000001400A718B: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001400A718F: EB 0B              jmp         00000001400A719C
  00000001400A7191: 8B 84 24 88 00 00  mov         eax,dword ptr [rsp+0000000000000088h]
                    00
  00000001400A7198: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001400A719C: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400A71A4: 48 83 C1 58        add         rcx,58h
  00000001400A71A8: 8B 94 24 88 00 00  mov         edx,dword ptr [rsp+0000000000000088h]
                    00
  00000001400A71AF: E8 6C 6E 36 01     call        000000014140E020
  00000001400A71B4: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400A71B9: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001400A71BF: 74 52              je          00000001400A7213
  00000001400A71C1: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001400A71C6: E8 75 4B 01 00     call        00000001400BBD40
  00000001400A71CB: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400A71D3: E8 48 01 00 00     call        00000001400A7320
  00000001400A71D8: 48 8B 44 24 38     mov         rax,qword ptr [rsp+38h]
  00000001400A71DD: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400A71E2: 48 8B 44 24 58     mov         rax,qword ptr [rsp+58h]
  00000001400A71E7: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400A71EC: 48 83 7C 24 50 00  cmp         qword ptr [rsp+50h],0
  00000001400A71F2: 74 16              je          00000001400A720A
  00000001400A71F4: BA 01 00 00 00     mov         edx,1
  00000001400A71F9: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400A71FE: E8 2D 51 00 00     call        00000001400AC330
  00000001400A7203: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001400A7208: EB 09              jmp         00000001400A7213
  00000001400A720A: 48 C7 44 24 68 00  mov         qword ptr [rsp+68h],0
                    00 00 00
  00000001400A7213: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  00000001400A7217: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400A721F: E8 2C 00 00 00     call        00000001400A7250
  00000001400A7224: 90                 nop
  00000001400A7225: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400A722A: FF 15 80 54 D6 01  call        qword ptr [0000000141E0C6B0h]
  00000001400A7230: 90                 nop
  00000001400A7231: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001400A7236: FF 15 74 54 D6 01  call        qword ptr [0000000141E0C6B0h]
  00000001400A723C: 48 83 C4 78        add         rsp,78h
  00000001400A7240: C3                 ret

  Summary

     1E02000 .text

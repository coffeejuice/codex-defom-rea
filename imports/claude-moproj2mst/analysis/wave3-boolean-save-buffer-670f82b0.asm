Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670F82B0: 40 57              push        rdi
  00000000670F82B2: 48 83 EC 30        sub         rsp,30h
  00000000670F82B6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670F82BF: 48 89 5C 24 50     mov         qword ptr [rsp+50h],rbx
  00000000670F82C4: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  00000000670F82C9: 48 8B D9           mov         rbx,rcx
  00000000670F82CC: 48 8B 71 18        mov         rsi,qword ptr [rcx+18h]
  00000000670F82D0: 48 83 79 70 00     cmp         qword ptr [rcx+70h],0
  00000000670F82D5: 0F 85 D9 00 00 00  jne         00000000670F83B4
  00000000670F82DB: 83 B9 98 00 00 00  cmp         dword ptr [rcx+0000000000000098h],0FFFFFFFFh
                    FF
  00000000670F82E2: 0F 85 CC 00 00 00  jne         00000000670F83B4
  00000000670F82E8: 40 B7 01           mov         dil,1
  00000000670F82EB: 8B 89 90 00 00 00  mov         ecx,dword ptr [rcx+0000000000000090h]
  00000000670F82F1: 83 F9 FF           cmp         ecx,0FFFFFFFFh
  00000000670F82F4: 74 66              je          00000000670F835C
  00000000670F82F6: FF 15 54 04 0B 00  call        qword ptr [00000000671A8750h]
  00000000670F82FC: 85 C0              test        eax,eax
  00000000670F82FE: 74 45              je          00000000670F8345
  00000000670F8300: 48 8B 4B 78        mov         rcx,qword ptr [rbx+78h]
  00000000670F8304: FF 15 C6 00 0B 00  call        qword ptr [00000000671A83D0h]
  00000000670F830A: 85 C0              test        eax,eax
  00000000670F830C: 75 37              jne         00000000670F8345
  00000000670F830E: 83 CA FF           or          edx,0FFFFFFFFh
  00000000670F8311: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000000670F8316: E8 F5 43 F2 FF     call        000000006701C710
  00000000670F831B: 90                 nop
  00000000670F831C: 4C 8B C0           mov         r8,rax
  00000000670F831F: BA 08 00 00 00     mov         edx,8
  00000000670F8324: 48 8B CE           mov         rcx,rsi
  00000000670F8327: E8 14 F7 FA FF     call        00000000670A7A40
  00000000670F832C: 90                 nop
  00000000670F832D: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000000670F8332: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F8336: 75 0A              jne         00000000670F8342
  00000000670F8338: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000000670F833D: E8 8E 7D F7 FF     call        00000000670700D0
  00000000670F8342: 40 32 FF           xor         dil,dil
  00000000670F8345: 48 C7 43 78 FF FF  mov         qword ptr [rbx+78h],0FFFFFFFFFFFFFFFFh
                    FF FF
  00000000670F834D: C7 83 90 00 00 00  mov         dword ptr [rbx+0000000000000090h],0FFFFFFFFh
                    FF FF FF FF
  00000000670F8357: 40 8A C7           mov         al,dil
  00000000670F835A: EB 5D              jmp         00000000670F83B9
  00000000670F835C: 48 8B 4B 78        mov         rcx,qword ptr [rbx+78h]
  00000000670F8360: 48 83 F9 FF        cmp         rcx,0FFFFFFFFFFFFFFFFh
  00000000670F8364: 74 0A              je          00000000670F8370
  00000000670F8366: FF 15 64 00 0B 00  call        qword ptr [00000000671A83D0h]
  00000000670F836C: 85 C0              test        eax,eax
  00000000670F836E: 75 37              jne         00000000670F83A7
  00000000670F8370: 83 CA FF           or          edx,0FFFFFFFFh
  00000000670F8373: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000000670F8378: E8 93 43 F2 FF     call        000000006701C710
  00000000670F837D: 90                 nop
  00000000670F837E: 4C 8B C0           mov         r8,rax
  00000000670F8381: BA 08 00 00 00     mov         edx,8
  00000000670F8386: 48 8B CE           mov         rcx,rsi
  00000000670F8389: E8 B2 F6 FA FF     call        00000000670A7A40
  00000000670F838E: 90                 nop
  00000000670F838F: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000000670F8394: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670F8398: 75 0A              jne         00000000670F83A4
  00000000670F839A: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000000670F839F: E8 2C 7D F7 FF     call        00000000670700D0
  00000000670F83A4: 40 32 FF           xor         dil,dil
  00000000670F83A7: 48 C7 43 78 FF FF  mov         qword ptr [rbx+78h],0FFFFFFFFFFFFFFFFh
                    FF FF
  00000000670F83AF: 40 8A C7           mov         al,dil
  00000000670F83B2: EB 05              jmp         00000000670F83B9
  00000000670F83B4: E8 57 1A FF FF     call        00000000670E9E10
  00000000670F83B9: 48 8B 5C 24 50     mov         rbx,qword ptr [rsp+50h]
  00000000670F83BE: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  00000000670F83C3: 48 83 C4 30        add         rsp,30h
  00000000670F83C7: 5F                 pop         rdi
  00000000670F83C8: C3                 ret

  Summary

      1A7000 .text

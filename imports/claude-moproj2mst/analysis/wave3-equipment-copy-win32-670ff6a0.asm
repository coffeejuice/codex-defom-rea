Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670FF6A0: 40 57              push        rdi
  00000000670FF6A2: 48 83 EC 30        sub         rsp,30h
  00000000670FF6A6: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670FF6AF: 48 89 5C 24 40     mov         qword ptr [rsp+40h],rbx
  00000000670FF6B4: 48 89 6C 24 48     mov         qword ptr [rsp+48h],rbp
  00000000670FF6B9: 48 89 74 24 50     mov         qword ptr [rsp+50h],rsi
  00000000670FF6BE: 49 8B E8           mov         rbp,r8
  00000000670FF6C1: 48 8B C2           mov         rax,rdx
  00000000670FF6C4: 48 8B D9           mov         rbx,rcx
  00000000670FF6C7: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670FF6CC: 48 8B C8           mov         rcx,rax
  00000000670FF6CF: E8 4C 04 FF FF     call        00000000670EFB20
  00000000670FF6D4: 48 8B F0           mov         rsi,rax
  00000000670FF6D7: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  00000000670FF6DC: 48 8B CB           mov         rcx,rbx
  00000000670FF6DF: E8 3C 04 FF FF     call        00000000670EFB20
  00000000670FF6E4: 48 8B F8           mov         rdi,rax
  00000000670FF6E7: 48 8B CE           mov         rcx,rsi
  00000000670FF6EA: E8 61 13 F7 FF     call        0000000067070A50
  00000000670FF6EF: 48 8B D8           mov         rbx,rax
  00000000670FF6F2: 48 8B CF           mov         rcx,rdi
  00000000670FF6F5: E8 56 13 F7 FF     call        0000000067070A50
  00000000670FF6FA: 48 8B C8           mov         rcx,rax
  00000000670FF6FD: 41 B8 01 00 00 00  mov         r8d,1
  00000000670FF703: 48 8B D3           mov         rdx,rbx
  00000000670FF706: FF 15 44 8B 0A 00  call        qword ptr [00000000671A8250h]
  00000000670FF70C: 85 C0              test        eax,eax
  00000000670FF70E: 40 0F 95 C7        setne       dil
  00000000670FF712: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000000670FF717: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670FF71B: 75 0B              jne         00000000670FF728
  00000000670FF71D: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000000670FF722: E8 A9 09 F7 FF     call        00000000670700D0
  00000000670FF727: 90                 nop
  00000000670FF728: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000000670FF72D: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670FF731: 75 0A              jne         00000000670FF73D
  00000000670FF733: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000000670FF738: E8 93 09 F7 FF     call        00000000670700D0
  00000000670FF73D: 40 84 FF           test        dil,dil
  00000000670FF740: 75 20              jne         00000000670FF762
  00000000670FF742: FF 15 98 8C 0A 00  call        qword ptr [00000000671A83E0h]
  00000000670FF748: 89 44 24 58        mov         dword ptr [rsp+58h],eax
  00000000670FF74C: C7 44 24 5C 02 00  mov         dword ptr [rsp+5Ch],2
                    00 00
  00000000670FF754: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  00000000670FF759: 48 89 4D 00        mov         qword ptr [rbp],rcx
  00000000670FF75D: 40 8A C7           mov         al,dil
  00000000670FF760: EB 03              jmp         00000000670FF765
  00000000670FF762: 40 8A C7           mov         al,dil
  00000000670FF765: 48 8B 5C 24 40     mov         rbx,qword ptr [rsp+40h]
  00000000670FF76A: 48 8B 6C 24 48     mov         rbp,qword ptr [rsp+48h]
  00000000670FF76F: 48 8B 74 24 50     mov         rsi,qword ptr [rsp+50h]
  00000000670FF774: 48 83 C4 30        add         rsp,30h
  00000000670FF778: 5F                 pop         rdi
  00000000670FF779: C3                 ret

  Summary

      1A7000 .text

Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670AC0B0: 40 57              push        rdi
  00000000670AC0B2: 48 83 EC 40        sub         rsp,40h
  00000000670AC0B6: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670AC0BF: 48 89 5C 24 58     mov         qword ptr [rsp+58h],rbx
  00000000670AC0C4: 48 8B 19           mov         rbx,qword ptr [rcx]
  00000000670AC0C7: 48 8B 02           mov         rax,qword ptr [rdx]
  00000000670AC0CA: 83 78 08 00        cmp         dword ptr [rax+8],0
  00000000670AC0CE: 75 13              jne         00000000670AC0E3
  00000000670AC0D0: 48 8D 0D 91 CF 17  lea         rcx,[0000000067229068h]
                    00
  00000000670AC0D7: E8 04 0E F7 FF     call        000000006701CEE0
  00000000670AC0DC: 32 C0              xor         al,al
  00000000670AC0DE: E9 A3 00 00 00     jmp         00000000670AC186
  00000000670AC0E3: 4C 8B C2           mov         r8,rdx
  00000000670AC0E6: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000670AC0EB: E8 B0 FC FF FF     call        00000000670ABDA0
  00000000670AC0F0: 90                 nop
  00000000670AC0F1: 48 8B 4B 20        mov         rcx,qword ptr [rbx+20h]
  00000000670AC0F5: 48 85 C9           test        rcx,rcx
  00000000670AC0F8: 75 63              jne         00000000670AC15D
  00000000670AC0FA: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000670AC0FF: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000670AC104: E8 07 2C 04 00     call        00000000670EED10
  00000000670AC109: 90                 nop
  00000000670AC10A: B2 01              mov         dl,1
  00000000670AC10C: 48 8B C8           mov         rcx,rax
  00000000670AC10F: E8 9C 2D 05 00     call        00000000670FEEB0
  00000000670AC114: 0F B6 F8           movzx       edi,al
  00000000670AC117: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  00000000670AC11C: F0 83 02 FF        lock add    dword ptr [rdx],0FFFFFFFFh
  00000000670AC120: 75 0B              jne         00000000670AC12D
  00000000670AC122: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000000670AC127: E8 A4 3F FC FF     call        00000000670700D0
  00000000670AC12C: 90                 nop
  00000000670AC12D: 48 8B 44 24 28     mov         rax,qword ptr [rsp+28h]
  00000000670AC132: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670AC136: 75 0B              jne         00000000670AC143
  00000000670AC138: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  00000000670AC13D: E8 8E 3F FC FF     call        00000000670700D0
  00000000670AC142: 90                 nop
  00000000670AC143: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000000670AC148: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  00000000670AC14C: 75 0A              jne         00000000670AC158
  00000000670AC14E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000670AC153: E8 78 3F FC FF     call        00000000670700D0
  00000000670AC158: 40 8A C7           mov         al,dil
  00000000670AC15B: EB 29              jmp         00000000670AC186
  00000000670AC15D: 48 8B 01           mov         rax,qword ptr [rcx]
  00000000670AC160: 41 B0 01           mov         r8b,1
  00000000670AC163: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000000670AC168: FF 50 60           call        qword ptr [rax+60h]
  00000000670AC16B: 0F B6 F8           movzx       edi,al
  00000000670AC16E: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000670AC173: F0 83 01 FF        lock add    dword ptr [rcx],0FFFFFFFFh
  00000000670AC177: 75 0A              jne         00000000670AC183
  00000000670AC179: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000000670AC17E: E8 4D 3F FC FF     call        00000000670700D0
  00000000670AC183: 40 8A C7           mov         al,dil
  00000000670AC186: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  00000000670AC18B: 48 83 C4 40        add         rsp,40h
  00000000670AC18F: 5F                 pop         rdi
  00000000670AC190: C3                 ret

  Summary

      1A7000 .text

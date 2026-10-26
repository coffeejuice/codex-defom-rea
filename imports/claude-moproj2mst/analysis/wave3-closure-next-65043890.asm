Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065043890: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065043895: 53                 push        rbx
  0000000065043896: 48 83 EC 30        sub         rsp,30h
  000000006504389A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650438A3: 48 8B D9           mov         rbx,rcx
  00000000650438A6: BA 04 08 04 00     mov         edx,40804h
  00000000650438AB: FF 15 F7 2E 65 00  call        qword ptr [00000000656967A8h]
  00000000650438B1: 90                 nop
  00000000650438B2: 48 8D 05 37 C7 65  lea         rax,[000000006569FFF0h]
                    00
  00000000650438B9: 48 89 03           mov         qword ptr [rbx],rax
  00000000650438BC: C7 83 88 00 00 00  mov         dword ptr [rbx+0000000000000088h],0FFFFFFFFh
                    FF FF FF FF
  00000000650438C6: C7 83 8C 00 00 00  mov         dword ptr [rbx+000000000000008Ch],0FFFFFFFFh
                    FF FF FF FF
  00000000650438D0: C7 83 90 00 00 00  mov         dword ptr [rbx+0000000000000090h],0FFFFFFFFh
                    FF FF FF FF
  00000000650438DA: C7 83 94 00 00 00  mov         dword ptr [rbx+0000000000000094h],0FFFFFFFFh
                    FF FF FF FF
  00000000650438E4: C7 83 98 00 00 00  mov         dword ptr [rbx+0000000000000098h],0FFFFFFFFh
                    FF FF FF FF
  00000000650438EE: 8B 83 9C 00 00 00  mov         eax,dword ptr [rbx+000000000000009Ch]
  00000000650438F4: 83 E0 F6           and         eax,0FFFFFFF6h
  00000000650438F7: 83 C8 06           or          eax,6
  00000000650438FA: 89 83 9C 00 00 00  mov         dword ptr [rbx+000000000000009Ch],eax
  0000000065043900: C7 83 A0 00 00 00  mov         dword ptr [rbx+00000000000000A0h],0
                    00 00 00 00
  000000006504390A: 48 8D 8B A4 00 00  lea         rcx,[rbx+00000000000000A4h]
                    00
  0000000065043911: FF 15 21 2F 65 00  call        qword ptr [0000000065696838h]
  0000000065043917: 48 C7 83 B8 00 00  mov         qword ptr [rbx+00000000000000B8h],0
                    00 00 00 00 00
  0000000065043922: 48 8B C3           mov         rax,rbx
  0000000065043925: 48 83 C4 30        add         rsp,30h
  0000000065043929: 5B                 pop         rbx
  000000006504392A: C3                 ret

  Summary

      695000 .text

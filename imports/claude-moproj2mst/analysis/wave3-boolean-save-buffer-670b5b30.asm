Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670B5B30: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000670B5B35: 53                 push        rbx
  00000000670B5B36: 48 83 EC 30        sub         rsp,30h
  00000000670B5B3A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000670B5B43: 48 8B D9           mov         rbx,rcx
  00000000670B5B46: E8 75 46 00 00     call        00000000670BA1C0
  00000000670B5B4B: 90                 nop
  00000000670B5B4C: 48 8D 05 9D 46 17  lea         rax,[000000006722A1F0h]
                    00
  00000000670B5B53: 48 89 03           mov         qword ptr [rbx],rax
  00000000670B5B56: 48 8D 05 D3 AF 23  lea         rax,[00000000672F0B30h]
                    00
  00000000670B5B5D: 48 89 83 E8 00 00  mov         qword ptr [rbx+00000000000000E8h],rax
                    00
  00000000670B5B64: F0 83 05 C4 AF 23  lock add    dword ptr [00000000672F0B30h],1
                    00 01
  00000000670B5B6C: 48 C7 83 F0 00 00  mov         qword ptr [rbx+00000000000000F0h],0
                    00 00 00 00 00
  00000000670B5B77: C6 83 F8 00 00 00  mov         byte ptr [rbx+00000000000000F8h],0
                    00
  00000000670B5B7E: 48 8D 8B 00 01 00  lea         rcx,[rbx+0000000000000100h]
                    00
  00000000670B5B85: BA 00 40 00 00     mov         edx,4000h
  00000000670B5B8A: E8 11 FE FF FF     call        00000000670B59A0
  00000000670B5B8F: C7 83 20 01 00 00  mov         dword ptr [rbx+0000000000000120h],0
                    00 00 00 00
  00000000670B5B99: 48 C7 83 28 01 00  mov         qword ptr [rbx+0000000000000128h],0
                    00 00 00 00 00
  00000000670B5BA4: 48 8B C3           mov         rax,rbx
  00000000670B5BA7: 48 83 C4 30        add         rsp,30h
  00000000670B5BAB: 5B                 pop         rbx
  00000000670B5BAC: C3                 ret

  Summary

      1A7000 .text

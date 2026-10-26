Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D31610: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D31615: 48 83 EC 18        sub         rsp,18h
  0000000140D31619: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D3161E: 8B 05 C8 4D 04 03  mov         eax,dword ptr [0000000143D763ECh]
  0000000140D31624: 89 81 28 05 00 00  mov         dword ptr [rcx+0000000000000528h],eax
  0000000140D3162A: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D3162F: 0F B6 05 BA 4D 04  movzx       eax,byte ptr [0000000143D763F0h]
                    03
  0000000140D31636: 88 81 2C 05 00 00  mov         byte ptr [rcx+000000000000052Ch],al
  0000000140D3163C: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140D31641: 0F B6 05 A9 4D 04  movzx       eax,byte ptr [0000000143D763F1h]
                    03
  0000000140D31648: 88 81 2D 05 00 00  mov         byte ptr [rcx+000000000000052Dh],al
  0000000140D3164E: C7 04 24 00 00 00  mov         dword ptr [rsp],0
                    00
  0000000140D31655: EB 09              jmp         0000000140D31660
  0000000140D31657: 8B 04 24           mov         eax,dword ptr [rsp]
  0000000140D3165A: 83 C0 01           add         eax,1
  0000000140D3165D: 89 04 24           mov         dword ptr [rsp],eax
  0000000140D31660: 83 3C 24 19        cmp         dword ptr [rsp],19h
  0000000140D31664: 0F 8D 96 00 00 00  jge         0000000140D31700
  0000000140D3166A: 4C 63 04 24        movsxd      r8,dword ptr [rsp]
  0000000140D3166E: 48 8D 05 0F 4F 04  lea         rax,[0000000143D76584h]
                    03
  0000000140D31675: 48 63 14 24        movsxd      rdx,dword ptr [rsp]
  0000000140D31679: 48 8D 0D E0 26 98  lea         rcx,[00000001446B3D60h]
                    03
  0000000140D31680: 42 8B 04 80        mov         eax,dword ptr [rax+r8*4]
  0000000140D31684: 89 04 91           mov         dword ptr [rcx+rdx*4],eax
  0000000140D31687: 4C 63 04 24        movsxd      r8,dword ptr [rsp]
  0000000140D3168B: 48 8D 05 56 4F 04  lea         rax,[0000000143D765E8h]
                    03
  0000000140D31692: 48 63 14 24        movsxd      rdx,dword ptr [rsp]
  0000000140D31696: 48 8D 0D 33 27 98  lea         rcx,[00000001446B3DD0h]
                    03
  0000000140D3169D: 42 8B 04 80        mov         eax,dword ptr [rax+r8*4]
  0000000140D316A1: 89 04 91           mov         dword ptr [rcx+rdx*4],eax
  0000000140D316A4: C7 44 24 04 00 00  mov         dword ptr [rsp+4],0
                    00 00
  0000000140D316AC: EB 0B              jmp         0000000140D316B9
  0000000140D316AE: 8B 44 24 04        mov         eax,dword ptr [rsp+4]
  0000000140D316B2: 83 C0 01           add         eax,1
  0000000140D316B5: 89 44 24 04        mov         dword ptr [rsp+4],eax
  0000000140D316B9: 83 7C 24 04 04     cmp         dword ptr [rsp+4],4
  0000000140D316BE: 7D 3B              jge         0000000140D316FB
  0000000140D316C0: 48 63 04 24        movsxd      rax,dword ptr [rsp]
  0000000140D316C4: 48 6B C0 10        imul        rax,rax,10h
  0000000140D316C8: 4C 8D 05 25 4D 04  lea         r8,[0000000143D763F4h]
                    03
  0000000140D316CF: 4C 03 C0           add         r8,rax
  0000000140D316D2: 48 63 54 24 04     movsxd      rdx,dword ptr [rsp+4]
  0000000140D316D7: 48 63 04 24        movsxd      rax,dword ptr [rsp]
  0000000140D316DB: 48 6B C0 10        imul        rax,rax,10h
  0000000140D316DF: 48 8D 0D EA 24 98  lea         rcx,[00000001446B3BD0h]
                    03
  0000000140D316E6: 48 03 C8           add         rcx,rax
  0000000140D316E9: 48 63 44 24 04     movsxd      rax,dword ptr [rsp+4]
  0000000140D316EE: F3 41 0F 10 04 90  movss       xmm0,dword ptr [r8+rdx*4]
  0000000140D316F4: F3 0F 11 04 81     movss       dword ptr [rcx+rax*4],xmm0
  0000000140D316F9: EB B3              jmp         0000000140D316AE
  0000000140D316FB: E9 57 FF FF FF     jmp         0000000140D31657
  0000000140D31700: 48 83 C4 18        add         rsp,18h
  0000000140D31704: C3                 ret
  0000000140D31705: CC                 int         3

  Summary

     1E02000 .text

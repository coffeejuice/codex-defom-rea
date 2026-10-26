Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file analysis\resume48-distortion-vt198-rea-session\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400657A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400657A5: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001400657AA: C7 00 01 00 00 00  mov         dword ptr [rax],1
  00000001400657B0: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001400657B5: C7 40 04 01 00 00  mov         dword ptr [rax+4],1
                    00
  00000001400657BC: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001400657C1: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001400657C5: F2 0F 11 40 08     movsd       mmword ptr [rax+8],xmm0
  00000001400657CA: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001400657CF: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001400657D3: F2 0F 11 40 10     movsd       mmword ptr [rax+10h],xmm0
  00000001400657D8: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001400657DD: C6 40 18 00        mov         byte ptr [rax+18h],0
  00000001400657E1: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001400657E6: C6 40 19 01        mov         byte ptr [rax+19h],1
  00000001400657EA: 48 8B 44 24 08     mov         rax,qword ptr [rsp+8]
  00000001400657EF: C3                 ret

  Summary

     1E02000 .text

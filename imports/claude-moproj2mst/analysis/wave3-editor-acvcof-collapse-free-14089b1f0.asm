Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014089B1F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014089B1F5: 48 83 EC 38        sub         rsp,38h
  000000014089B1F9: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  000000014089B1FE: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B203: 83 78 1C 00        cmp         dword ptr [rax+1Ch],0
  000000014089B207: 75 11              jne         000000014089B21A
  000000014089B209: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B20E: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  000000014089B213: 74 05              je          000000014089B21A
  000000014089B215: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  000000014089B21A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B21F: 83 78 20 00        cmp         dword ptr [rax+20h],0
  000000014089B223: 75 11              jne         000000014089B236
  000000014089B225: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B22A: 48 83 78 30 00     cmp         qword ptr [rax+30h],0
  000000014089B22F: 74 05              je          000000014089B236
  000000014089B231: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  000000014089B236: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B23B: 83 78 24 00        cmp         dword ptr [rax+24h],0
  000000014089B23F: 75 11              jne         000000014089B252
  000000014089B241: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B246: 48 83 78 38 00     cmp         qword ptr [rax+38h],0
  000000014089B24B: 74 05              je          000000014089B252
  000000014089B24D: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  000000014089B252: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014089B257: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B25C: 83 78 1C 00        cmp         dword ptr [rax+1Ch],0
  000000014089B260: 7E 30              jle         000000014089B292
  000000014089B262: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014089B267: 85 C0              test        eax,eax
  000000014089B269: 75 27              jne         000000014089B292
  000000014089B26B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B270: 48 83 78 28 00     cmp         qword ptr [rax+28h],0
  000000014089B275: 74 0E              je          000000014089B285
  000000014089B277: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B27C: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014089B280: E8 F7 68 BD 00     call        0000000141471B7C
  000000014089B285: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B28A: 48 C7 40 28 00 00  mov         qword ptr [rax+28h],0
                    00 00
  000000014089B292: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B297: 83 78 20 00        cmp         dword ptr [rax+20h],0
  000000014089B29B: 7E 30              jle         000000014089B2CD
  000000014089B29D: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014089B2A2: 85 C0              test        eax,eax
  000000014089B2A4: 75 27              jne         000000014089B2CD
  000000014089B2A6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B2AB: 48 83 78 30 00     cmp         qword ptr [rax+30h],0
  000000014089B2B0: 74 0E              je          000000014089B2C0
  000000014089B2B2: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B2B7: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  000000014089B2BB: E8 BC 68 BD 00     call        0000000141471B7C
  000000014089B2C0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B2C5: 48 C7 40 30 00 00  mov         qword ptr [rax+30h],0
                    00 00
  000000014089B2CD: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B2D2: 83 78 24 00        cmp         dword ptr [rax+24h],0
  000000014089B2D6: 7E 30              jle         000000014089B308
  000000014089B2D8: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014089B2DD: 85 C0              test        eax,eax
  000000014089B2DF: 75 27              jne         000000014089B308
  000000014089B2E1: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B2E6: 48 83 78 38 00     cmp         qword ptr [rax+38h],0
  000000014089B2EB: 74 0E              je          000000014089B2FB
  000000014089B2ED: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B2F2: 48 8B 49 38        mov         rcx,qword ptr [rcx+38h]
  000000014089B2F6: E8 81 68 BD 00     call        0000000141471B7C
  000000014089B2FB: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B300: 48 C7 40 38 00 00  mov         qword ptr [rax+38h],0
                    00 00
  000000014089B308: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  000000014089B30D: 85 C0              test        eax,eax
  000000014089B30F: 75 27              jne         000000014089B338
  000000014089B311: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B316: 48 83 78 48 00     cmp         qword ptr [rax+48h],0
  000000014089B31B: 74 0E              je          000000014089B32B
  000000014089B31D: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014089B322: 48 8B 49 48        mov         rcx,qword ptr [rcx+48h]
  000000014089B326: E8 51 68 BD 00     call        0000000141471B7C
  000000014089B32B: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B330: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  000000014089B338: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B33D: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  000000014089B345: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B34A: 48 C7 40 38 00 00  mov         qword ptr [rax+38h],0
                    00 00
  000000014089B352: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B357: 48 C7 40 30 00 00  mov         qword ptr [rax+30h],0
                    00 00
  000000014089B35F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  000000014089B364: 48 C7 40 28 00 00  mov         qword ptr [rax+28h],0
                    00 00
  000000014089B36C: 48 83 C4 38        add         rsp,38h
  000000014089B370: C3                 ret

  Summary

     1E02000 .text

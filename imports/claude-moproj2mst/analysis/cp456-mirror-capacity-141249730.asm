Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000141249730: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  0000000141249735: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  000000014124973A: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014124973F: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000141249744: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  000000014124974B: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141249753: 81 B8 68 84 00 00  cmp         dword ptr [rax+0000000000008468h],80h
                    80 00 00 00
  000000014124975D: 75 07              jne         0000000141249766
  000000014124975F: 33 C0              xor         eax,eax
  0000000141249761: E9 0E 03 00 00     jmp         0000000141249A74
  0000000141249766: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014124976E: E8 0D FF FF FF     call        0000000141249680
  0000000141249773: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000141249777: 81 7C 24 3C 80 00  cmp         dword ptr [rsp+3Ch],80h
                    00 00
  000000014124977F: 7C 07              jl          0000000141249788
  0000000141249781: 33 C0              xor         eax,eax
  0000000141249783: E9 EC 02 00 00     jmp         0000000141249A74
  0000000141249788: 48 63 4C 24 3C     movsxd      rcx,dword ptr [rsp+3Ch]
  000000014124978D: 48 69 C9 08 01 00  imul        rcx,rcx,108h
                    00
  0000000141249794: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014124979C: 48 03 C1           add         rax,rcx
  000000014124979F: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001412497A4: 44 0F B6 84 24 A0  movzx       r8d,byte ptr [rsp+00000000000000A0h]
                    00 00 00
  00000001412497AD: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001412497B5: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001412497BA: E8 B1 E2 FF FF     call        0000000141247A70
  00000001412497BF: C6 44 24 68 01     mov         byte ptr [rsp+68h],1
  00000001412497C4: 48 8D 54 24 68     lea         rdx,[rsp+68h]
  00000001412497C9: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001412497CE: E8 FD E6 FF FF     call        0000000141247ED0
  00000001412497D3: 4C 8D 4C 24 20     lea         r9,[rsp+20h]
  00000001412497D8: 4C 8B 84 24 A8 00  mov         r8,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001412497E0: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001412497E8: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001412497F0: E8 4B 06 00 00     call        0000000141249E40
  00000001412497F5: 4C 8D 4C 24 48     lea         r9,[rsp+48h]
  00000001412497FA: 4C 8B 84 24 B0 00  mov         r8,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000141249802: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  000000014124980A: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141249812: E8 29 06 00 00     call        0000000141249E40
  0000000141249817: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  000000014124981F: EB 0B              jmp         000000014124982C
  0000000141249821: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000141249825: 83 C0 01           add         eax,1
  0000000141249828: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  000000014124982C: 83 7C 24 38 03     cmp         dword ptr [rsp+38h],3
  0000000141249831: 0F 8D 13 01 00 00  jge         000000014124994A
  0000000141249837: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  000000014124983C: 48 63 44 24 38     movsxd      rax,dword ptr [rsp+38h]
  0000000141249841: F2 0F 10 44 CC 20  movsd       xmm0,mmword ptr [rsp+rcx*8+20h]
  0000000141249847: 66 0F 2F 44 C4 48  comisd      xmm0,mmword ptr [rsp+rax*8+48h]
  000000014124984D: 76 38              jbe         0000000141249887
  000000014124984F: 48 63 44 24 38     movsxd      rax,dword ptr [rsp+38h]
  0000000141249854: F2 0F 10 44 C4 20  movsd       xmm0,mmword ptr [rsp+rax*8+20h]
  000000014124985A: F2 0F 11 44 24 40  movsd       mmword ptr [rsp+40h],xmm0
  0000000141249860: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  0000000141249865: 48 63 44 24 38     movsxd      rax,dword ptr [rsp+38h]
  000000014124986A: F2 0F 10 44 CC 48  movsd       xmm0,mmword ptr [rsp+rcx*8+48h]
  0000000141249870: F2 0F 11 44 C4 20  movsd       mmword ptr [rsp+rax*8+20h],xmm0
  0000000141249876: 48 63 44 24 38     movsxd      rax,dword ptr [rsp+38h]
  000000014124987B: F2 0F 10 44 24 40  movsd       xmm0,mmword ptr [rsp+40h]
  0000000141249881: F2 0F 11 44 C4 48  movsd       mmword ptr [rsp+rax*8+48h],xmm0
  0000000141249887: 48 63 44 24 38     movsxd      rax,dword ptr [rsp+38h]
  000000014124988C: F2 0F 10 4C C4 20  movsd       xmm1,mmword ptr [rsp+rax*8+20h]
  0000000141249892: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  0000000141249897: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014124989F: F2 0F 10 04 C8     movsd       xmm0,mmword ptr [rax+rcx*8]
  00000001412498A4: 66 0F 2F C1        comisd      xmm0,xmm1
  00000001412498A8: 76 13              jbe         00000001412498BD
  00000001412498AA: 48 63 44 24 38     movsxd      rax,dword ptr [rsp+38h]
  00000001412498AF: F2 0F 10 44 C4 20  movsd       xmm0,mmword ptr [rsp+rax*8+20h]
  00000001412498B5: F2 0F 11 44 24 70  movsd       mmword ptr [rsp+70h],xmm0
  00000001412498BB: EB 18              jmp         00000001412498D5
  00000001412498BD: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  00000001412498C2: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001412498CA: F2 0F 10 04 C8     movsd       xmm0,mmword ptr [rax+rcx*8]
  00000001412498CF: F2 0F 11 44 24 70  movsd       mmword ptr [rsp+70h],xmm0
  00000001412498D5: 48 63 44 24 38     movsxd      rax,dword ptr [rsp+38h]
  00000001412498DA: F2 0F 10 44 24 70  movsd       xmm0,mmword ptr [rsp+70h]
  00000001412498E0: F2 0F 11 44 C4 20  movsd       mmword ptr [rsp+rax*8+20h],xmm0
  00000001412498E6: 48 63 44 24 38     movsxd      rax,dword ptr [rsp+38h]
  00000001412498EB: F2 0F 10 4C C4 48  movsd       xmm1,mmword ptr [rsp+rax*8+48h]
  00000001412498F1: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  00000001412498F6: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001412498FE: F2 0F 10 04 C8     movsd       xmm0,mmword ptr [rax+rcx*8]
  0000000141249903: 66 0F 2F C8        comisd      xmm1,xmm0
  0000000141249907: 76 13              jbe         000000014124991C
  0000000141249909: 48 63 44 24 38     movsxd      rax,dword ptr [rsp+38h]
  000000014124990E: F2 0F 10 44 C4 48  movsd       xmm0,mmword ptr [rsp+rax*8+48h]
  0000000141249914: F2 0F 11 44 24 78  movsd       mmword ptr [rsp+78h],xmm0
  000000014124991A: EB 18              jmp         0000000141249934
  000000014124991C: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  0000000141249921: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000141249929: F2 0F 10 04 C8     movsd       xmm0,mmword ptr [rax+rcx*8]
  000000014124992E: F2 0F 11 44 24 78  movsd       mmword ptr [rsp+78h],xmm0
  0000000141249934: 48 63 44 24 38     movsxd      rax,dword ptr [rsp+38h]
  0000000141249939: F2 0F 10 44 24 78  movsd       xmm0,mmword ptr [rsp+78h]
  000000014124993F: F2 0F 11 44 C4 48  movsd       mmword ptr [rsp+rax*8+48h],xmm0
  0000000141249945: E9 D7 FE FF FF     jmp         0000000141249821
  000000014124994A: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141249952: 83 B8 68 84 00 00  cmp         dword ptr [rax+0000000000008468h],1
                    01
  0000000141249959: 75 65              jne         00000001412499C0
  000000014124995B: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  0000000141249963: EB 0B              jmp         0000000141249970
  0000000141249965: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000141249969: 83 C0 01           add         eax,1
  000000014124996C: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000141249970: 83 7C 24 38 03     cmp         dword ptr [rsp+38h],3
  0000000141249975: 7D 44              jge         00000001412499BB
  0000000141249977: 48 63 54 24 38     movsxd      rdx,dword ptr [rsp+38h]
  000000014124997C: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  0000000141249981: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141249989: F2 0F 10 44 D4 20  movsd       xmm0,mmword ptr [rsp+rdx*8+20h]
  000000014124998F: F2 0F 11 84 C8 38  movsd       mmword ptr [rax+rcx*8+0000000000008438h],xmm0
                    84 00 00
  0000000141249998: 48 63 54 24 38     movsxd      rdx,dword ptr [rsp+38h]
  000000014124999D: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  00000001412499A2: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001412499AA: F2 0F 10 44 D4 48  movsd       xmm0,mmword ptr [rsp+rdx*8+48h]
  00000001412499B0: F2 0F 11 84 C8 50  movsd       mmword ptr [rax+rcx*8+0000000000008450h],xmm0
                    84 00 00
  00000001412499B9: EB AA              jmp         0000000141249965
  00000001412499BB: E9 AD 00 00 00     jmp         0000000141249A6D
  00000001412499C0: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  00000001412499C8: EB 0B              jmp         00000001412499D5
  00000001412499CA: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  00000001412499CE: 83 C0 01           add         eax,1
  00000001412499D1: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  00000001412499D5: 83 7C 24 38 03     cmp         dword ptr [rsp+38h],3
  00000001412499DA: 0F 8D 8D 00 00 00  jge         0000000141249A6D
  00000001412499E0: 48 63 54 24 38     movsxd      rdx,dword ptr [rsp+38h]
  00000001412499E5: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  00000001412499EA: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001412499F2: F2 0F 10 84 D0 38  movsd       xmm0,mmword ptr [rax+rdx*8+0000000000008438h]
                    84 00 00
  00000001412499FB: 66 0F 2F 44 CC 20  comisd      xmm0,mmword ptr [rsp+rcx*8+20h]
  0000000141249A01: 76 21              jbe         0000000141249A24
  0000000141249A03: 48 63 54 24 38     movsxd      rdx,dword ptr [rsp+38h]
  0000000141249A08: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  0000000141249A0D: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141249A15: F2 0F 10 44 D4 20  movsd       xmm0,mmword ptr [rsp+rdx*8+20h]
  0000000141249A1B: F2 0F 11 84 C8 38  movsd       mmword ptr [rax+rcx*8+0000000000008438h],xmm0
                    84 00 00
  0000000141249A24: 48 63 54 24 38     movsxd      rdx,dword ptr [rsp+38h]
  0000000141249A29: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  0000000141249A2E: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141249A36: F2 0F 10 44 CC 48  movsd       xmm0,mmword ptr [rsp+rcx*8+48h]
  0000000141249A3C: 66 0F 2F 84 D0 50  comisd      xmm0,mmword ptr [rax+rdx*8+0000000000008450h]
                    84 00 00
  0000000141249A45: 76 21              jbe         0000000141249A68
  0000000141249A47: 48 63 54 24 38     movsxd      rdx,dword ptr [rsp+38h]
  0000000141249A4C: 48 63 4C 24 38     movsxd      rcx,dword ptr [rsp+38h]
  0000000141249A51: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  0000000141249A59: F2 0F 10 44 D4 48  movsd       xmm0,mmword ptr [rsp+rdx*8+48h]
  0000000141249A5F: F2 0F 11 84 C8 50  movsd       mmword ptr [rax+rcx*8+0000000000008450h],xmm0
                    84 00 00
  0000000141249A68: E9 5D FF FF FF     jmp         00000001412499CA
  0000000141249A6D: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000141249A71: 83 C0 01           add         eax,1
  0000000141249A74: 48 81 C4 88 00 00  add         rsp,88h
                    00
  0000000141249A7B: C3                 ret

  Summary

     1E02000 .text

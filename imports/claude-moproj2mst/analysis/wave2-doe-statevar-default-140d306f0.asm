Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D306F0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D306F5: 48 83 EC 38        sub         rsp,38h
  0000000140D306F9: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140D30701: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D30706: C7 80 80 04 00 00  mov         dword ptr [rax+0000000000000480h],1
                    01 00 00 00
  0000000140D30710: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D30715: C7 80 84 04 00 00  mov         dword ptr [rax+0000000000000484h],14h
                    14 00 00 00
  0000000140D3071F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D30724: C7 80 88 04 00 00  mov         dword ptr [rax+0000000000000488h],0
                    00 00 00 00
  0000000140D3072E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D30733: C6 80 8C 04 00 00  mov         byte ptr [rax+000000000000048Ch],1
                    01
  0000000140D3073A: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D3073F: C6 80 8D 04 00 00  mov         byte ptr [rax+000000000000048Dh],0
                    00
  0000000140D30746: 48 8D 15 FB 19 CD  lea         rdx,[0000000142A02148h]
                    01
  0000000140D3074D: 48 8D 0D 1C 5C 04  lea         rcx,[0000000143D76370h]
                    03
  0000000140D30754: E8 E7 7F AD FF     call        0000000140808740
  0000000140D30759: 44 8B D8           mov         r11d,eax
  0000000140D3075C: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D30761: 44 89 98 80 04 00  mov         dword ptr [rax+0000000000000480h],r11d
                    00
  0000000140D30768: 48 8D 15 01 1A CD  lea         rdx,[0000000142A02170h]
                    01
  0000000140D3076F: 48 8D 0D FA 5B 04  lea         rcx,[0000000143D76370h]
                    03
  0000000140D30776: E8 C5 7F AD FF     call        0000000140808740
  0000000140D3077B: 44 8B D8           mov         r11d,eax
  0000000140D3077E: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D30783: 44 89 98 84 04 00  mov         dword ptr [rax+0000000000000484h],r11d
                    00
  0000000140D3078A: 48 8D 15 07 1A CD  lea         rdx,[0000000142A02198h]
                    01
  0000000140D30791: 48 8D 0D D8 5B 04  lea         rcx,[0000000143D76370h]
                    03
  0000000140D30798: E8 A3 7F AD FF     call        0000000140808740
  0000000140D3079D: 44 8B D8           mov         r11d,eax
  0000000140D307A0: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D307A5: 44 89 98 88 04 00  mov         dword ptr [rax+0000000000000488h],r11d
                    00
  0000000140D307AC: 48 8D 15 0D 1A CD  lea         rdx,[0000000142A021C0h]
                    01
  0000000140D307B3: 48 8D 0D B6 5B 04  lea         rcx,[0000000143D76370h]
                    03
  0000000140D307BA: E8 61 7C AD FF     call        0000000140808420
  0000000140D307BF: 44 0F B6 D8        movzx       r11d,al
  0000000140D307C3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D307C8: 44 88 98 8C 04 00  mov         byte ptr [rax+000000000000048Ch],r11b
                    00
  0000000140D307CF: 48 8D 15 12 1A CD  lea         rdx,[0000000142A021E8h]
                    01
  0000000140D307D6: 48 8D 0D 93 5B 04  lea         rcx,[0000000143D76370h]
                    03
  0000000140D307DD: E8 3E 7C AD FF     call        0000000140808420
  0000000140D307E2: 44 0F B6 D8        movzx       r11d,al
  0000000140D307E6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D307EB: 44 88 98 8D 04 00  mov         byte ptr [rax+000000000000048Dh],r11b
                    00
  0000000140D307F2: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140D307FA: EB 0B              jmp         0000000140D30807
  0000000140D307FC: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140D30800: 83 C0 01           add         eax,1
  0000000140D30803: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140D30807: 83 7C 24 20 04     cmp         dword ptr [rsp+20h],4
  0000000140D3080C: 7D 2B              jge         0000000140D30839
  0000000140D3080E: 4C 8D 05 7B 6D 04  lea         r8,[0000000143D77590h]
                    03
  0000000140D30815: 48 63 54 24 20     movsxd      rdx,dword ptr [rsp+20h]
  0000000140D3081A: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  0000000140D3081F: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D30824: F3 41 0F 10 84 90  movss       xmm0,dword ptr [r8+rdx*4+0000000000000270h]
                    70 02 00 00
  0000000140D3082E: F3 0F 11 84 88 90  movss       dword ptr [rax+rcx*4+0000000000000490h],xmm0
                    04 00 00
  0000000140D30837: EB C3              jmp         0000000140D307FC
  0000000140D30839: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140D3083E: E8 8D 0A 00 00     call        0000000140D312D0
  0000000140D30843: 4C 8B 5C 24 40     mov         r11,qword ptr [rsp+40h]
  0000000140D30848: 41 C6 83 A0 04 00  mov         byte ptr [r11+00000000000004A0h],0
                    00 00
  0000000140D30850: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  0000000140D30858: EB 0B              jmp         0000000140D30865
  0000000140D3085A: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  0000000140D3085E: 83 C0 01           add         eax,1
  0000000140D30861: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  0000000140D30865: 83 7C 24 20 03     cmp         dword ptr [rsp+20h],3
  0000000140D3086A: 7D 19              jge         0000000140D30885
  0000000140D3086C: 48 63 4C 24 20     movsxd      rcx,dword ptr [rsp+20h]
  0000000140D30871: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D30876: 66 0F 57 C0        xorpd       xmm0,xmm0
  0000000140D3087A: F2 0F 11 84 C8 A8  movsd       mmword ptr [rax+rcx*8+00000000000004A8h],xmm0
                    04 00 00
  0000000140D30883: EB D5              jmp         0000000140D3085A
  0000000140D30885: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D3088A: C7 80 C0 04 00 00  mov         dword ptr [rax+00000000000004C0h],0
                    00 00 00 00
  0000000140D30894: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140D30899: C7 80 C4 04 00 00  mov         dword ptr [rax+00000000000004C4h],0
                    00 00 00 00
  0000000140D308A3: 48 83 C4 38        add         rsp,38h
  0000000140D308A7: C3                 ret
  0000000140D308A8: CC                 int         3

  Summary

     1E02000 .text

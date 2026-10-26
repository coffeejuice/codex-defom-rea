Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408959E0: 44 88 4C 24 20     mov         byte ptr [rsp+20h],r9b
  00000001408959E5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001408959EA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001408959EF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408959F4: 53                 push        rbx
  00000001408959F5: 48 83 EC 50        sub         rsp,50h
  00000001408959F9: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  0000000140895A01: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140895A09: 48 C7 44 24 30 00  mov         qword ptr [rsp+30h],0
                    00 00 00
  0000000140895A12: C7 44 24 3C 00 00  mov         dword ptr [rsp+3Ch],0
                    00 00
  0000000140895A1A: 48 83 7C 24 68 00  cmp         qword ptr [rsp+68h],0
  0000000140895A20: 74 10              je          0000000140895A32
  0000000140895A22: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140895A27: E8 54 B1 BD 00     call        0000000141470B80
  0000000140895A2C: 48 83 F8 01        cmp         rax,1
  0000000140895A30: 73 0A              jae         0000000140895A3C
  0000000140895A32: B8 01 00 00 00     mov         eax,1
  0000000140895A37: E9 EC 02 00 00     jmp         0000000140895D28
  0000000140895A3C: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140895A41: 48 81 C1 28 13 00  add         rcx,1328h
                    00
  0000000140895A48: 8B 54 24 70        mov         edx,dword ptr [rsp+70h]
  0000000140895A4C: E8 FF 68 02 00     call        00000001408BC350
  0000000140895A51: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140895A56: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140895A5C: 75 0A              jne         0000000140895A68
  0000000140895A5E: B8 03 00 00 00     mov         eax,3
  0000000140895A63: E9 C0 02 00 00     jmp         0000000140895D28
  0000000140895A68: 48 8D 15 35 6A F1  lea         rdx,[00000001427AC4A4h]
                    01
  0000000140895A6F: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140895A74: E8 5F A2 BD 00     call        000000014146FCD8
  0000000140895A79: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140895A7E: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  0000000140895A84: 75 0A              jne         0000000140895A90
  0000000140895A86: B8 01 00 00 00     mov         eax,1
  0000000140895A8B: E9 98 02 00 00     jmp         0000000140895D28
  0000000140895A90: 48 83 BC 24 80 00  cmp         qword ptr [rsp+0000000000000080h],0
                    00 00 00
  0000000140895A99: 0F 84 8F 00 00 00  je          0000000140895B2E
  0000000140895A9F: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140895AA4: B9 2A 00 00 00     mov         ecx,2Ah
  0000000140895AA9: E8 3E E3 BD 00     call        0000000141473DEC
  0000000140895AAE: C7 44 24 38 00 00  mov         dword ptr [rsp+38h],0
                    00 00
  0000000140895AB6: EB 0B              jmp         0000000140895AC3
  0000000140895AB8: 8B 44 24 38        mov         eax,dword ptr [rsp+38h]
  0000000140895ABC: 83 C0 01           add         eax,1
  0000000140895ABF: 89 44 24 38        mov         dword ptr [rsp+38h],eax
  0000000140895AC3: 8B 5C 24 38        mov         ebx,dword ptr [rsp+38h]
  0000000140895AC7: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140895ACF: E8 AC B0 BD 00     call        0000000141470B80
  0000000140895AD4: 48 3B D8           cmp         rbx,rax
  0000000140895AD7: 73 44              jae         0000000140895B1D
  0000000140895AD9: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  0000000140895ADD: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140895AE5: 0F BE 04 08        movsx       eax,byte ptr [rax+rcx]
  0000000140895AE9: 83 F8 0A           cmp         eax,0Ah
  0000000140895AEC: 75 13              jne         0000000140895B01
  0000000140895AEE: 48 8D 15 CF 6A F1  lea         rdx,[00000001427AC5C4h]
                    01
  0000000140895AF5: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895AFA: E8 0D 7F BD 00     call        000000014146DA0C
  0000000140895AFF: EB 1A              jmp         0000000140895B1B
  0000000140895B01: 8B 4C 24 38        mov         ecx,dword ptr [rsp+38h]
  0000000140895B05: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140895B0D: 0F BE 0C 08        movsx       ecx,byte ptr [rax+rcx]
  0000000140895B11: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140895B16: E8 D1 E2 BD 00     call        0000000141473DEC
  0000000140895B1B: EB 9B              jmp         0000000140895AB8
  0000000140895B1D: 48 8D 15 60 6B F1  lea         rdx,[00000001427AC684h]
                    01
  0000000140895B24: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895B29: E8 DE 7E BD 00     call        000000014146DA0C
  0000000140895B2E: 48 8D 15 AF 6B F1  lea         rdx,[00000001427AC6E4h]
                    01
  0000000140895B35: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895B3A: E8 CD 7E BD 00     call        000000014146DA0C
  0000000140895B3F: 48 8D 15 02 72 F1  lea         rdx,[00000001427ACD48h]
                    01
  0000000140895B46: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895B4B: E8 BC 7E BD 00     call        000000014146DA0C
  0000000140895B50: 48 8D 15 75 6C F1  lea         rdx,[00000001427AC7CCh]
                    01
  0000000140895B57: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895B5C: E8 AB 7E BD 00     call        000000014146DA0C
  0000000140895B61: 0F B6 44 24 78     movzx       eax,byte ptr [rsp+78h]
  0000000140895B66: 85 C0              test        eax,eax
  0000000140895B68: 74 2A              je          0000000140895B94
  0000000140895B6A: B9 1E 00 00 00     mov         ecx,1Eh
  0000000140895B6F: E8 BC C3 08 00     call        0000000140921F30
  0000000140895B74: 4C 8B C0           mov         r8,rax
  0000000140895B77: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140895B7C: 44 8B 88 4C 08 00  mov         r9d,dword ptr [rax+000000000000084Ch]
                    00
  0000000140895B83: 48 8D 15 E6 71 F1  lea         rdx,[00000001427ACD70h]
                    01
  0000000140895B8A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895B8F: E8 78 7E BD 00     call        000000014146DA0C
  0000000140895B94: 4C 8B 8C 24 88 00  mov         r9,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140895B9C: 4C 8B 44 24 20     mov         r8,qword ptr [rsp+20h]
  0000000140895BA1: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140895BA6: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140895BAB: E8 B0 E5 2B 00     call        0000000140B54160
  0000000140895BB0: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000140895BB4: 83 7C 24 3C 00     cmp         dword ptr [rsp+3Ch],0
  0000000140895BB9: 74 13              je          0000000140895BCE
  0000000140895BBB: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895BC0: E8 63 9F BD 00     call        000000014146FB28
  0000000140895BC5: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140895BC9: E9 5A 01 00 00     jmp         0000000140895D28
  0000000140895BCE: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140895BD3: 83 B8 C8 1C 00 00  cmp         dword ptr [rax+0000000000001CC8h],1
                    01
  0000000140895BDA: 0F 85 3C 01 00 00  jne         0000000140895D1C
  0000000140895BE0: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140895BE8: EB 0B              jmp         0000000140895BF5
  0000000140895BEA: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140895BEE: 83 C0 01           add         eax,1
  0000000140895BF1: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140895BF5: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140895BFA: 8B 80 CC 1C 00 00  mov         eax,dword ptr [rax+0000000000001CCCh]
  0000000140895C00: 39 44 24 28        cmp         dword ptr [rsp+28h],eax
  0000000140895C04: 7D 6E              jge         0000000140895C74
  0000000140895C06: 48 63 54 24 28     movsxd      rdx,dword ptr [rsp+28h]
  0000000140895C0B: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140895C10: 48 8B 80 D0 1C 00  mov         rax,qword ptr [rax+0000000000001CD0h]
                    00
  0000000140895C17: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140895C1C: 48 81 C1 28 13 00  add         rcx,1328h
                    00
  0000000140895C23: 8B 14 90           mov         edx,dword ptr [rax+rdx*4]
  0000000140895C26: E8 25 67 02 00     call        00000001408BC350
  0000000140895C2B: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140895C30: 48 83 7C 24 40 00  cmp         qword ptr [rsp+40h],0
  0000000140895C36: 75 02              jne         0000000140895C3A
  0000000140895C38: EB B0              jmp         0000000140895BEA
  0000000140895C3A: 45 33 C9           xor         r9d,r9d
  0000000140895C3D: 4C 8B 44 24 40     mov         r8,qword ptr [rsp+40h]
  0000000140895C42: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140895C47: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140895C4C: E8 0F E5 2B 00     call        0000000140B54160
  0000000140895C51: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000140895C55: 83 7C 24 3C 00     cmp         dword ptr [rsp+3Ch],0
  0000000140895C5A: 74 13              je          0000000140895C6F
  0000000140895C5C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895C61: E8 C2 9E BD 00     call        000000014146FB28
  0000000140895C66: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140895C6A: E9 B9 00 00 00     jmp         0000000140895D28
  0000000140895C6F: E9 76 FF FF FF     jmp         0000000140895BEA
  0000000140895C74: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140895C7C: EB 0B              jmp         0000000140895C89
  0000000140895C7E: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140895C82: 83 C0 01           add         eax,1
  0000000140895C85: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140895C89: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140895C8E: 48 81 C1 58 13 00  add         rcx,1358h
                    00
  0000000140895C95: E8 D6 D3 F3 FF     call        00000001407D3070
  0000000140895C9A: 39 44 24 28        cmp         dword ptr [rsp+28h],eax
  0000000140895C9E: 7D 7C              jge         0000000140895D1C
  0000000140895CA0: 8B 54 24 28        mov         edx,dword ptr [rsp+28h]
  0000000140895CA4: 83 C2 01           add         edx,1
  0000000140895CA7: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140895CAC: 48 81 C1 58 13 00  add         rcx,1358h
                    00
  0000000140895CB3: E8 E8 6D 14 00     call        00000001409DCAA0
  0000000140895CB8: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140895CBD: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140895CC3: 75 02              jne         0000000140895CC7
  0000000140895CC5: EB B7              jmp         0000000140895C7E
  0000000140895CC7: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  0000000140895CCC: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140895CD1: E8 8A 69 14 00     call        00000001409DC660
  0000000140895CD6: 0F B6 C0           movzx       eax,al
  0000000140895CD9: 85 C0              test        eax,eax
  0000000140895CDB: 75 02              jne         0000000140895CDF
  0000000140895CDD: EB 9F              jmp         0000000140895C7E
  0000000140895CDF: 44 8B 4C 24 28     mov         r9d,dword ptr [rsp+28h]
  0000000140895CE4: 41 83 C1 01        add         r9d,1
  0000000140895CE8: 4C 8B 44 24 48     mov         r8,qword ptr [rsp+48h]
  0000000140895CED: 48 8B 54 24 30     mov         rdx,qword ptr [rsp+30h]
  0000000140895CF2: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  0000000140895CF7: E8 C4 C6 2B 00     call        0000000140B523C0
  0000000140895CFC: 89 44 24 3C        mov         dword ptr [rsp+3Ch],eax
  0000000140895D00: 83 7C 24 3C 00     cmp         dword ptr [rsp+3Ch],0
  0000000140895D05: 74 10              je          0000000140895D17
  0000000140895D07: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895D0C: E8 17 9E BD 00     call        000000014146FB28
  0000000140895D11: 8B 44 24 3C        mov         eax,dword ptr [rsp+3Ch]
  0000000140895D15: EB 11              jmp         0000000140895D28
  0000000140895D17: E9 62 FF FF FF     jmp         0000000140895C7E
  0000000140895D1C: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140895D21: E8 02 9E BD 00     call        000000014146FB28
  0000000140895D26: 33 C0              xor         eax,eax
  0000000140895D28: 48 83 C4 50        add         rsp,50h
  0000000140895D2C: 5B                 pop         rbx
  0000000140895D2D: C3                 ret

  Summary

     1E02000 .text

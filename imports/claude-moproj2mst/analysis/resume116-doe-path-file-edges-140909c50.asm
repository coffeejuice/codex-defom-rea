Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140909C50: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140909C55: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140909C5A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140909C5F: 56                 push        rsi
  0000000140909C60: 57                 push        rdi
  0000000140909C61: 48 81 EC E8 00 00  sub         rsp,0E8h
                    00
  0000000140909C68: 48 C7 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140909C74: 0F 29 B4 24 D0 00  movaps      xmmword ptr [rsp+00000000000000D0h],xmm6
                    00 00
  0000000140909C7C: 0F 29 BC 24 C0 00  movaps      xmmword ptr [rsp+00000000000000C0h],xmm7
                    00 00
  0000000140909C84: 44 0F 29 84 24 B0  movaps      xmmword ptr [rsp+00000000000000B0h],xmm8
                    00 00 00
  0000000140909C8D: C6 44 24 38 00     mov         byte ptr [rsp+38h],0
  0000000140909C92: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140909C9A: E8 41 AE 6F FF     call        0000000140004AE0
  0000000140909C9F: 89 44 24 34        mov         dword ptr [rsp+34h],eax
  0000000140909CA3: 48 8B 94 24 00 01  mov         rdx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140909CAB: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140909CB0: FF 15 22 A3 4F 01  call        qword ptr [0000000141E03FD8h]
  0000000140909CB6: 90                 nop
  0000000140909CB7: 48 8D 44 24 70     lea         rax,[rsp+70h]
  0000000140909CBC: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140909CC1: 41 B8 10 00 00 00  mov         r8d,10h
  0000000140909CC7: BA 02 00 00 00     mov         edx,2
  0000000140909CCC: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000140909CD1: E8 AA 79 72 FF     call        0000000140031680
  0000000140909CD6: 48 8B D0           mov         rdx,rax
  0000000140909CD9: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140909CDE: FF 15 FC A2 4F 01  call        qword ptr [0000000141E03FE0h]
  0000000140909CE4: 0F B6 C0           movzx       eax,al
  0000000140909CE7: 85 C0              test        eax,eax
  0000000140909CE9: 0F 84 BB 01 00 00  je          0000000140909EAA
  0000000140909CEF: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140909CF4: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140909CF9: FF 15 D9 A0 4F 01  call        qword ptr [0000000141E03DD8h]
  0000000140909CFF: 90                 nop
  0000000140909D00: 8B 54 24 34        mov         edx,dword ptr [rsp+34h]
  0000000140909D04: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140909D09: FF 15 D9 A0 4F 01  call        qword ptr [0000000141E03DE8h]
  0000000140909D0F: 48 8B 15 82 A6 4F  mov         rdx,qword ptr [0000000141E04398h]
                    01
  0000000140909D16: 48 8B C8           mov         rcx,rax
  0000000140909D19: E8 A2 7E 73 FF     call        0000000140041BC0
  0000000140909D1E: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  0000000140909D26: EB 0B              jmp         0000000140909D33
  0000000140909D28: 8B 44 24 30        mov         eax,dword ptr [rsp+30h]
  0000000140909D2C: 83 C0 01           add         eax,1
  0000000140909D2F: 89 44 24 30        mov         dword ptr [rsp+30h],eax
  0000000140909D33: 8B 44 24 34        mov         eax,dword ptr [rsp+34h]
  0000000140909D37: 39 44 24 30        cmp         dword ptr [rsp+30h],eax
  0000000140909D3B: 0F 8D 23 01 00 00  jge         0000000140909E64
  0000000140909D41: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  0000000140909D45: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140909D4D: E8 2E E5 7C FF     call        00000001400D8280
  0000000140909D52: 48 8D BC 24 98 00  lea         rdi,[rsp+0000000000000098h]
                    00 00
  0000000140909D5A: 48 8B F0           mov         rsi,rax
  0000000140909D5D: B9 10 00 00 00     mov         ecx,10h
  0000000140909D62: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140909D64: 48 8D 7C 24 60     lea         rdi,[rsp+60h]
  0000000140909D69: 48 8D B4 24 98 00  lea         rsi,[rsp+0000000000000098h]
                    00 00
  0000000140909D71: B9 10 00 00 00     mov         ecx,10h
  0000000140909D76: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140909D78: 8B 54 24 30        mov         edx,dword ptr [rsp+30h]
  0000000140909D7C: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140909D84: E8 F7 E4 7C FF     call        00000001400D8280
  0000000140909D89: 48 8D BC 24 88 00  lea         rdi,[rsp+0000000000000088h]
                    00 00
  0000000140909D91: 48 8D 70 10        lea         rsi,[rax+10h]
  0000000140909D95: B9 10 00 00 00     mov         ecx,10h
  0000000140909D9A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140909D9C: 48 8D 7C 24 50     lea         rdi,[rsp+50h]
  0000000140909DA1: 48 8D B4 24 88 00  lea         rsi,[rsp+0000000000000088h]
                    00 00
  0000000140909DA9: B9 10 00 00 00     mov         ecx,10h
  0000000140909DAE: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140909DB0: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140909DB5: FF 15 E5 A9 4F 01  call        qword ptr [0000000141E047A0h]
  0000000140909DBB: 66 44 0F 28 C0     movapd      xmm8,xmm0
  0000000140909DC0: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140909DC5: FF 15 C5 A9 4F 01  call        qword ptr [0000000141E04790h]
  0000000140909DCB: 66 0F 28 F8        movapd      xmm7,xmm0
  0000000140909DCF: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140909DD4: FF 15 C6 A9 4F 01  call        qword ptr [0000000141E047A0h]
  0000000140909DDA: 66 0F 28 F0        movapd      xmm6,xmm0
  0000000140909DDE: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140909DE3: FF 15 A7 A9 4F 01  call        qword ptr [0000000141E04790h]
  0000000140909DE9: 66 0F 28 C8        movapd      xmm1,xmm0
  0000000140909DED: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140909DF2: FF 15 E8 BA 4F 01  call        qword ptr [0000000141E058E0h]
  0000000140909DF8: 48 8D 15 0D 07 EB  lea         rdx,[00000001427BA50Ch]
                    01
  0000000140909DFF: 48 8B C8           mov         rcx,rax
  0000000140909E02: FF 15 D8 9F 4F 01  call        qword ptr [0000000141E03DE0h]
  0000000140909E08: 66 0F 28 CE        movapd      xmm1,xmm6
  0000000140909E0C: 48 8B C8           mov         rcx,rax
  0000000140909E0F: FF 15 CB BA 4F 01  call        qword ptr [0000000141E058E0h]
  0000000140909E15: 48 8D 15 48 09 EB  lea         rdx,[00000001427BA764h]
                    01
  0000000140909E1C: 48 8B C8           mov         rcx,rax
  0000000140909E1F: FF 15 BB 9F 4F 01  call        qword ptr [0000000141E03DE0h]
  0000000140909E25: 66 0F 28 CF        movapd      xmm1,xmm7
  0000000140909E29: 48 8B C8           mov         rcx,rax
  0000000140909E2C: FF 15 AE BA 4F 01  call        qword ptr [0000000141E058E0h]
  0000000140909E32: 48 8D 15 4B 0A EB  lea         rdx,[00000001427BA884h]
                    01
  0000000140909E39: 48 8B C8           mov         rcx,rax
  0000000140909E3C: FF 15 9E 9F 4F 01  call        qword ptr [0000000141E03DE0h]
  0000000140909E42: 66 41 0F 28 C8     movapd      xmm1,xmm8
  0000000140909E47: 48 8B C8           mov         rcx,rax
  0000000140909E4A: FF 15 90 BA 4F 01  call        qword ptr [0000000141E058E0h]
  0000000140909E50: 48 8B 15 41 A5 4F  mov         rdx,qword ptr [0000000141E04398h]
                    01
  0000000140909E57: 48 8B C8           mov         rcx,rax
  0000000140909E5A: E8 61 7D 73 FF     call        0000000140041BC0
  0000000140909E5F: E9 C4 FE FF FF     jmp         0000000140909D28
  0000000140909E64: 83 BC 24 10 01 00  cmp         dword ptr [rsp+0000000000000110h],64h
                    00 64
  0000000140909E6C: 75 21              jne         0000000140909E8F
  0000000140909E6E: 48 8D 15 DB 10 EB  lea         rdx,[00000001427BAF50h]
                    01
  0000000140909E75: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140909E7A: FF 15 60 9F 4F 01  call        qword ptr [0000000141E03DE0h]
  0000000140909E80: 48 8B 15 11 A5 4F  mov         rdx,qword ptr [0000000141E04398h]
                    01
  0000000140909E87: 48 8B C8           mov         rcx,rax
  0000000140909E8A: E8 31 7D 73 FF     call        0000000140041BC0
  0000000140909E8F: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140909E94: FF 15 7E A1 4F 01  call        qword ptr [0000000141E04018h]
  0000000140909E9A: C6 44 24 38 01     mov         byte ptr [rsp+38h],1
  0000000140909E9F: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140909EA4: FF 15 4E 9F 4F 01  call        qword ptr [0000000141E03DF8h]
  0000000140909EAA: 0F B6 44 24 38     movzx       eax,byte ptr [rsp+38h]
  0000000140909EAF: 88 84 24 80 00 00  mov         byte ptr [rsp+0000000000000080h],al
                    00
  0000000140909EB6: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140909EBB: FF 15 5F A1 4F 01  call        qword ptr [0000000141E04020h]
  0000000140909EC1: 90                 nop
  0000000140909EC2: 48 8B 8C 24 00 01  mov         rcx,qword ptr [rsp+0000000000000100h]
                    00 00
  0000000140909ECA: FF 15 10 BB 4F 01  call        qword ptr [0000000141E059E0h]
  0000000140909ED0: 90                 nop
  0000000140909ED1: 48 8B 8C 24 08 01  mov         rcx,qword ptr [rsp+0000000000000108h]
                    00 00
  0000000140909ED9: E8 52 61 CD FF     call        00000001405E0030
  0000000140909EDE: 8A 84 24 80 00 00  mov         al,byte ptr [rsp+0000000000000080h]
                    00
  0000000140909EE5: 0F 28 B4 24 D0 00  movaps      xmm6,xmmword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140909EED: 0F 28 BC 24 C0 00  movaps      xmm7,xmmword ptr [rsp+00000000000000C0h]
                    00 00
  0000000140909EF5: 44 0F 28 84 24 B0  movaps      xmm8,xmmword ptr [rsp+00000000000000B0h]
                    00 00 00
  0000000140909EFE: 48 81 C4 E8 00 00  add         rsp,0E8h
                    00
  0000000140909F05: 5F                 pop         rdi
  0000000140909F06: 5E                 pop         rsi
  0000000140909F07: C3                 ret

  Summary

     1E02000 .text

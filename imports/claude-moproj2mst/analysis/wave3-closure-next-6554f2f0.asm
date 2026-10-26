Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006554F2F0: 48 8B C4           mov         rax,rsp
  000000006554F2F3: 57                 push        rdi
  000000006554F2F4: 48 83 EC 70        sub         rsp,70h
  000000006554F2F8: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006554F301: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  000000006554F305: 48 89 70 20        mov         qword ptr [rax+20h],rsi
  000000006554F309: 0F B6 F2           movzx       esi,dl
  000000006554F30C: 48 8B F9           mov         rdi,rcx
  000000006554F30F: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006554F313: 0F BA A0 38 01 00  bt          dword ptr [rax+0000000000000138h],0Ch
                    00 0C
  000000006554F31B: 73 44              jae         000000006554F361
  000000006554F31D: 48 8B 98 80 00 00  mov         rbx,qword ptr [rax+0000000000000080h]
                    00
  000000006554F324: 48 85 DB           test        rbx,rbx
  000000006554F327: 74 38              je          000000006554F361
  000000006554F329: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  000000006554F330: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006554F333: 48 8B CB           mov         rcx,rbx
  000000006554F336: FF 50 50           call        qword ptr [rax+50h]
  000000006554F339: 83 F8 0A           cmp         eax,0Ah
  000000006554F33C: 74 12              je          000000006554F350
  000000006554F33E: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006554F342: 48 8B 98 80 00 00  mov         rbx,qword ptr [rax+0000000000000080h]
                    00
  000000006554F349: 48 85 DB           test        rbx,rbx
  000000006554F34C: 74 13              je          000000006554F361
  000000006554F34E: EB E0              jmp         000000006554F330
  000000006554F350: 40 0F B6 D6        movzx       edx,sil
  000000006554F354: 48 8B CB           mov         rcx,rbx
  000000006554F357: E8 94 FF FF FF     call        000000006554F2F0
  000000006554F35C: E9 59 01 00 00     jmp         000000006554F4BA
  000000006554F361: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006554F365: F6 80 3C 01 00 00  test        byte ptr [rax+000000000000013Ch],2
                    02
  000000006554F36C: 74 12              je          000000006554F380
  000000006554F36E: F6 80 38 01 00 00  test        byte ptr [rax+0000000000000138h],80h
                    80
  000000006554F375: 74 09              je          000000006554F380
  000000006554F377: F6 80 38 01 00 00  test        byte ptr [rax+0000000000000138h],20h
                    20
  000000006554F37E: 75 03              jne         000000006554F383
  000000006554F380: 40 32 F6           xor         sil,sil
  000000006554F383: 40 0F B6 D6        movzx       edx,sil
  000000006554F387: 8B 80 38 01 00 00  mov         eax,dword ptr [rax+0000000000000138h]
  000000006554F38D: C1 E8 09           shr         eax,9
  000000006554F390: 83 E0 01           and         eax,1
  000000006554F393: 3B C2              cmp         eax,edx
  000000006554F395: 0F 84 1F 01 00 00  je          000000006554F4BA
  000000006554F39B: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006554F3A0: FF 15 4A 8F 14 00  call        qword ptr [00000000656982F0h]
  000000006554F3A6: 90                 nop
  000000006554F3A7: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006554F3AA: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  000000006554F3AF: 41 B8 04 00 00 00  mov         r8d,4
  000000006554F3B5: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006554F3BA: 48 8B CF           mov         rcx,rdi
  000000006554F3BD: FF 90 00 01 00 00  call        qword ptr [rax+0000000000000100h]
  000000006554F3C3: 90                 nop
  000000006554F3C4: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006554F3C9: FF 15 11 73 14 00  call        qword ptr [00000000656966E0h]
  000000006554F3CF: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006554F3D4: FF 15 0E 7F 14 00  call        qword ptr [00000000656972E8h]
  000000006554F3DA: 48 8B 57 08        mov         rdx,qword ptr [rdi+8]
  000000006554F3DE: 0F B6 C8           movzx       ecx,al
  000000006554F3E1: 8B 82 38 01 00 00  mov         eax,dword ptr [rdx+0000000000000138h]
  000000006554F3E7: C1 E8 09           shr         eax,9
  000000006554F3EA: 83 E0 01           and         eax,1
  000000006554F3ED: 3B C1              cmp         eax,ecx
  000000006554F3EF: 75 10              jne         000000006554F401
  000000006554F3F1: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006554F3F6: FF 15 E4 72 14 00  call        qword ptr [00000000656966E0h]
  000000006554F3FC: E9 B9 00 00 00     jmp         000000006554F4BA
  000000006554F401: 81 A2 38 01 00 00  and         dword ptr [rdx+0000000000000138h],0FFFFFDFFh
                    FF FD FF FF
  000000006554F40B: 83 E1 01           and         ecx,1
  000000006554F40E: C1 E1 09           shl         ecx,9
  000000006554F411: 09 8A 38 01 00 00  or          dword ptr [rdx+0000000000000138h],ecx
  000000006554F417: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006554F41C: FF 15 7E 73 14 00  call        qword ptr [00000000656967A0h]
  000000006554F422: 48 8B D0           mov         rdx,rax
  000000006554F425: 48 8B CF           mov         rcx,rdi
  000000006554F428: E8 53 D9 FF FF     call        000000006554CD80
  000000006554F42D: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  000000006554F431: 48 83 78 78 00     cmp         qword ptr [rax+78h],0
  000000006554F436: 74 4F              je          000000006554F487
  000000006554F438: 48 8B 40 78        mov         rax,qword ptr [rax+78h]
  000000006554F43C: 48 8B 58 08        mov         rbx,qword ptr [rax+8]
  000000006554F440: 40 84 F6           test        sil,sil
  000000006554F443: 74 2C              je          000000006554F471
  000000006554F445: 48 89 BC 24 90 00  mov         qword ptr [rsp+0000000000000090h],rdi
                    00 00
  000000006554F44D: 48 8D 8B 08 01 00  lea         rcx,[rbx+0000000000000108h]
                    00
  000000006554F454: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  000000006554F45C: 4C 8D 84 24 90 00  lea         r8,[rsp+0000000000000090h]
                    00 00
  000000006554F464: 48 8D 94 24 80 00  lea         rdx,[rsp+0000000000000080h]
                    00 00
  000000006554F46C: E8 4F 82 B2 FF     call        00000000650776C0
  000000006554F471: 83 BB 00 01 00 00  cmp         dword ptr [rbx+0000000000000100h],0
                    00
  000000006554F478: 75 0D              jne         000000006554F487
  000000006554F47A: 48 8B 4F 08        mov         rcx,qword ptr [rdi+8]
  000000006554F47E: 48 8B 49 78        mov         rcx,qword ptr [rcx+78h]
  000000006554F482: E8 79 48 01 00     call        0000000065563D00
  000000006554F487: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006554F48A: 4C 8D 4C 24 28     lea         r9,[rsp+28h]
  000000006554F48F: 41 B8 0E 00 00 00  mov         r8d,0Eh
  000000006554F495: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  000000006554F49A: 48 8B CF           mov         rcx,rdi
  000000006554F49D: FF 90 00 01 00 00  call        qword ptr [rax+0000000000000100h]
  000000006554F4A3: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  000000006554F4A8: FF 15 32 72 14 00  call        qword ptr [00000000656966E0h]
  000000006554F4AE: 90                 nop
  000000006554F4AF: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006554F4B4: FF 15 26 72 14 00  call        qword ptr [00000000656966E0h]
  000000006554F4BA: 4C 8D 5C 24 70     lea         r11,[rsp+70h]
  000000006554F4BF: 49 8B 5B 18        mov         rbx,qword ptr [r11+18h]
  000000006554F4C3: 49 8B 73 28        mov         rsi,qword ptr [r11+28h]
  000000006554F4C7: 49 8B E3           mov         rsp,r11
  000000006554F4CA: 5F                 pop         rdi
  000000006554F4CB: C3                 ret

  Summary

      695000 .text

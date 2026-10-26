Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140919DE0: 44 88 4C 24 20     mov         byte ptr [rsp+20h],r9b
  0000000140919DE5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  0000000140919DEA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140919DEF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140919DF4: 48 81 EC C8 00 00  sub         rsp,0C8h
                    00
  0000000140919DFB: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  0000000140919E07: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140919E10: 48 8D 44 24 30     lea         rax,[rsp+30h]
  0000000140919E15: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140919E1A: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140919E22: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  0000000140919E27: FF 15 AB BB 4E 01  call        qword ptr [0000000141E059D8h]
  0000000140919E2D: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  0000000140919E35: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919E3D: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140919E40: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  0000000140919E48: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919E50: FF 90 30 01 00 00  call        qword ptr [rax+0000000000000130h]
  0000000140919E56: 0F B6 C0           movzx       eax,al
  0000000140919E59: 85 C0              test        eax,eax
  0000000140919E5B: 0F 85 8A 01 00 00  jne         0000000140919FEB
  0000000140919E61: B9 48 00 00 00     mov         ecx,48h
  0000000140919E66: E8 51 4C B5 00     call        000000014146EABC
  0000000140919E6B: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140919E70: 48 83 7C 24 48 00  cmp         qword ptr [rsp+48h],0
  0000000140919E76: 74 1B              je          0000000140919E93
  0000000140919E78: 8B 94 24 E0 00 00  mov         edx,dword ptr [rsp+00000000000000E0h]
                    00
  0000000140919E7F: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  0000000140919E84: E8 77 CA EF FF     call        0000000140816900
  0000000140919E89: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  0000000140919E91: EB 0C              jmp         0000000140919E9F
  0000000140919E93: 48 C7 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],0
                    00 00 00 00 00 00
  0000000140919E9F: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140919EA7: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140919EAC: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  0000000140919EB1: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140919EB6: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919EBE: 48 83 C1 38        add         rcx,38h
  0000000140919EC2: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  0000000140919EC7: E8 14 39 E1 00     call        000000014172D7E0
  0000000140919ECC: 48 8D 44 24 50     lea         rax,[rsp+50h]
  0000000140919ED1: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140919ED6: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140919EDE: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000140919EE3: FF 15 EF BA 4E 01  call        qword ptr [0000000141E059D8h]
  0000000140919EE9: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  0000000140919EF1: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140919EF9: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140919EFE: E8 FD 1D 4E 00     call        0000000140DFBD00
  0000000140919F03: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919F0B: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140919F0E: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919F16: FF 90 E8 01 00 00  call        qword ptr [rax+00000000000001E8h]
  0000000140919F1C: 89 44 24 2C        mov         dword ptr [rsp+2Ch],eax
  0000000140919F20: 0F B6 84 24 E8 00  movzx       eax,byte ptr [rsp+00000000000000E8h]
                    00 00
  0000000140919F28: 85 C0              test        eax,eax
  0000000140919F2A: 74 0E              je          0000000140919F3A
  0000000140919F2C: 8B 54 24 2C        mov         edx,dword ptr [rsp+2Ch]
  0000000140919F30: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  0000000140919F35: E8 56 56 0E 00     call        00000001409FF590
  0000000140919F3A: 48 8D 44 24 60     lea         rax,[rsp+60h]
  0000000140919F3F: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140919F44: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  0000000140919F4C: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  0000000140919F51: FF 15 81 BA 4E 01  call        qword ptr [0000000141E059D8h]
  0000000140919F57: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  0000000140919F5F: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919F67: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140919F6A: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  0000000140919F72: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919F7A: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  0000000140919F80: 44 8B D8           mov         r11d,eax
  0000000140919F83: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919F8B: 44 89 58 74        mov         dword ptr [rax+74h],r11d
  0000000140919F8F: C7 44 24 28 00 00  mov         dword ptr [rsp+28h],0
                    00 00
  0000000140919F97: EB 0B              jmp         0000000140919FA4
  0000000140919F99: 8B 44 24 28        mov         eax,dword ptr [rsp+28h]
  0000000140919F9D: 83 C0 01           add         eax,1
  0000000140919FA0: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000140919FA4: 83 7C 24 28 03     cmp         dword ptr [rsp+28h],3
  0000000140919FA9: 7D 14              jge         0000000140919FBF
  0000000140919FAB: 48 63 4C 24 28     movsxd      rcx,dword ptr [rsp+28h]
  0000000140919FB0: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919FB8: C6 44 08 70 01     mov         byte ptr [rax+rcx+70h],1
  0000000140919FBD: EB DA              jmp         0000000140919F99
  0000000140919FBF: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919FC7: 83 78 40 02        cmp         dword ptr [rax+40h],2
  0000000140919FCB: 75 1C              jne         0000000140919FE9
  0000000140919FCD: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  0000000140919FD2: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000140919FDA: E8 31 31 00 00     call        000000014091D110
  0000000140919FDF: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140919FE4: E8 17 27 7A FF     call        00000001400BC700
  0000000140919FE9: EB 67              jmp         000000014091A052
  0000000140919FEB: 48 8D 44 24 78     lea         rax,[rsp+78h]
  0000000140919FF0: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140919FF8: 48 8B 94 24 D8 00  mov         rdx,qword ptr [rsp+00000000000000D8h]
                    00 00
  000000014091A000: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A008: FF 15 CA B9 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A00E: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  000000014091A016: 48 8B 84 24 D0 00  mov         rax,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091A01E: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A021: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091A029: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091A031: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  000000014091A037: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091A03F: 48 83 C1 38        add         rcx,38h
  000000014091A043: 8B D0              mov         edx,eax
  000000014091A045: E8 C6 6B 84 FF     call        0000000140160C10
  000000014091A04A: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A04D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014091A052: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014091A057: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  000000014091A05F: 48 8B 8C 24 D8 00  mov         rcx,qword ptr [rsp+00000000000000D8h]
                    00 00
  000000014091A067: FF 15 73 B9 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091A06D: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091A075: 48 81 C4 C8 00 00  add         rsp,0C8h
                    00
  000000014091A07C: C3                 ret

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D25A0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  00000001400D25A5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  00000001400D25AA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400D25AF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D25B4: 56                 push        rsi
  00000001400D25B5: 57                 push        rdi
  00000001400D25B6: 48 81 EC 58 02 00  sub         rsp,258h
                    00
  00000001400D25BD: 48 C7 84 24 20 02  mov         qword ptr [rsp+0000000000000220h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001400D25C9: 48 8D 44 24 68     lea         rax,[rsp+68h]
  00000001400D25CE: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  00000001400D25D3: 48 8B 94 24 78 02  mov         rdx,qword ptr [rsp+0000000000000278h]
                    00 00
  00000001400D25DB: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  00000001400D25E0: FF 15 F2 33 D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D25E6: 48 89 84 24 28 02  mov         qword ptr [rsp+0000000000000228h],rax
                    00 00
  00000001400D25EE: 48 8B 84 24 70 02  mov         rax,qword ptr [rsp+0000000000000270h]
                    00 00
  00000001400D25F6: 48 8B 00           mov         rax,qword ptr [rax]
  00000001400D25F9: 48 8B 94 24 28 02  mov         rdx,qword ptr [rsp+0000000000000228h]
                    00 00
  00000001400D2601: 48 8B 8C 24 70 02  mov         rcx,qword ptr [rsp+0000000000000270h]
                    00 00
  00000001400D2609: FF 90 30 01 00 00  call        qword ptr [rax+0000000000000130h]
  00000001400D260F: 0F B6 C0           movzx       eax,al
  00000001400D2612: 85 C0              test        eax,eax
  00000001400D2614: 0F 84 85 03 00 00  je          00000001400D299F
  00000001400D261A: 48 8D 44 24 78     lea         rax,[rsp+78h]
  00000001400D261F: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400D2627: 48 8D 84 24 88 00  lea         rax,[rsp+0000000000000088h]
                    00 00
  00000001400D262F: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001400D2637: 48 8B 94 24 88 02  mov         rdx,qword ptr [rsp+0000000000000288h]
                    00 00
  00000001400D263F: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400D2647: FF 15 8B 33 D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D264D: 48 89 84 24 30 02  mov         qword ptr [rsp+0000000000000230h],rax
                    00 00
  00000001400D2655: 48 8B 84 24 30 02  mov         rax,qword ptr [rsp+0000000000000230h]
                    00 00
  00000001400D265D: 48 89 84 24 38 02  mov         qword ptr [rsp+0000000000000238h],rax
                    00 00
  00000001400D2665: 48 8B 94 24 78 02  mov         rdx,qword ptr [rsp+0000000000000278h]
                    00 00
  00000001400D266D: 48 8B 8C 24 90 00  mov         rcx,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001400D2675: FF 15 5D 33 D3 01  call        qword ptr [0000000141E059D8h]
  00000001400D267B: 48 89 84 24 40 02  mov         qword ptr [rsp+0000000000000240h],rax
                    00 00
  00000001400D2683: 48 8B 84 24 B0 02  mov         rax,qword ptr [rsp+00000000000002B0h]
                    00 00
  00000001400D268B: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400D2690: 48 8B 84 24 A8 02  mov         rax,qword ptr [rsp+00000000000002A8h]
                    00 00
  00000001400D2698: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400D269D: F2 0F 10 84 24 A0  movsd       xmm0,mmword ptr [rsp+00000000000002A0h]
                    02 00 00
  00000001400D26A6: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  00000001400D26AC: 8B 84 24 98 02 00  mov         eax,dword ptr [rsp+0000000000000298h]
                    00
  00000001400D26B3: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001400D26B7: 8B 84 24 90 02 00  mov         eax,dword ptr [rsp+0000000000000290h]
                    00
  00000001400D26BE: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  00000001400D26C2: 4C 8B 8C 24 38 02  mov         r9,qword ptr [rsp+0000000000000238h]
                    00 00
  00000001400D26CA: 44 8B 84 24 80 02  mov         r8d,dword ptr [rsp+0000000000000280h]
                    00 00
  00000001400D26D2: 48 8B 94 24 40 02  mov         rdx,qword ptr [rsp+0000000000000240h]
                    00 00
  00000001400D26DA: 48 8B 8C 24 70 02  mov         rcx,qword ptr [rsp+0000000000000270h]
                    00 00
  00000001400D26E2: E8 C9 7E 84 00     call        000000014091A5B0
  00000001400D26E7: 48 8B 8C 24 A8 02  mov         rcx,qword ptr [rsp+00000000000002A8h]
                    00 00
  00000001400D26EF: E8 EC 23 F3 FF     call        0000000140004AE0
  00000001400D26F4: 89 44 24 64        mov         dword ptr [rsp+64h],eax
  00000001400D26F8: C7 44 24 60 00 00  mov         dword ptr [rsp+60h],0
                    00 00
  00000001400D2700: EB 0B              jmp         00000001400D270D
  00000001400D2702: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  00000001400D2706: 83 C0 01           add         eax,1
  00000001400D2709: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000001400D270D: 8B 44 24 64        mov         eax,dword ptr [rsp+64h]
  00000001400D2711: 39 44 24 60        cmp         dword ptr [rsp+60h],eax
  00000001400D2715: 0F 8D 84 02 00 00  jge         00000001400D299F
  00000001400D271B: C6 84 24 38 01 00  mov         byte ptr [rsp+0000000000000138h],1
                    00 01
  00000001400D2723: 4C 8D 84 24 38 01  lea         r8,[rsp+0000000000000138h]
                    00 00
  00000001400D272B: 48 8D 15 02 44 09  lea         rdx,[0000000142166B34h]
                    02
  00000001400D2732: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  00000001400D273A: E8 41 29 F5 FF     call        0000000140025080
  00000001400D273F: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  00000001400D2747: 48 8B F0           mov         rsi,rax
  00000001400D274A: B9 10 00 00 00     mov         ecx,10h
  00000001400D274F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D2751: 48 8D BC 24 18 01  lea         rdi,[rsp+0000000000000118h]
                    00 00
  00000001400D2759: 48 8D B4 24 10 02  lea         rsi,[rsp+0000000000000210h]
                    00 00
  00000001400D2761: B9 10 00 00 00     mov         ecx,10h
  00000001400D2766: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D2768: 8B 54 24 60        mov         edx,dword ptr [rsp+60h]
  00000001400D276C: 48 8B 8C 24 A8 02  mov         rcx,qword ptr [rsp+00000000000002A8h]
                    00 00
  00000001400D2774: E8 47 29 F5 FF     call        00000001400250C0
  00000001400D2779: 4C 8B C0           mov         r8,rax
  00000001400D277C: 48 8D 15 91 0D 09  lea         rdx,[0000000142163514h]
                    02
  00000001400D2783: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  00000001400D278B: E8 F0 28 F5 FF     call        0000000140025080
  00000001400D2790: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  00000001400D2798: 48 8B F0           mov         rsi,rax
  00000001400D279B: B9 10 00 00 00     mov         ecx,10h
  00000001400D27A0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D27A2: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  00000001400D27AA: 48 8D B4 24 00 02  lea         rsi,[rsp+0000000000000200h]
                    00 00
  00000001400D27B2: B9 10 00 00 00     mov         ecx,10h
  00000001400D27B7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D27B9: 45 33 C0           xor         r8d,r8d
  00000001400D27BC: 33 D2              xor         edx,edx
  00000001400D27BE: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001400D27C6: FF 15 9C 19 D3 01  call        qword ptr [0000000141E04168h]
  00000001400D27CC: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  00000001400D27D4: 48 8B F0           mov         rsi,rax
  00000001400D27D7: B9 10 00 00 00     mov         ecx,10h
  00000001400D27DC: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D27DE: 45 33 C0           xor         r8d,r8d
  00000001400D27E1: 33 D2              xor         edx,edx
  00000001400D27E3: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  00000001400D27EB: FF 15 77 19 D3 01  call        qword ptr [0000000141E04168h]
  00000001400D27F1: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  00000001400D27F9: 48 8B F0           mov         rsi,rax
  00000001400D27FC: B9 10 00 00 00     mov         ecx,10h
  00000001400D2801: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D2803: 45 33 C0           xor         r8d,r8d
  00000001400D2806: 33 D2              xor         edx,edx
  00000001400D2808: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000001400D2810: FF 15 52 19 D3 01  call        qword ptr [0000000141E04168h]
  00000001400D2816: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  00000001400D281E: 48 8B F0           mov         rsi,rax
  00000001400D2821: B9 10 00 00 00     mov         ecx,10h
  00000001400D2826: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D2828: 45 33 C0           xor         r8d,r8d
  00000001400D282B: 33 D2              xor         edx,edx
  00000001400D282D: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001400D2835: FF 15 2D 19 D3 01  call        qword ptr [0000000141E04168h]
  00000001400D283B: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  00000001400D2843: 48 8B F0           mov         rsi,rax
  00000001400D2846: B9 10 00 00 00     mov         ecx,10h
  00000001400D284B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D284D: 45 33 C0           xor         r8d,r8d
  00000001400D2850: 33 D2              xor         edx,edx
  00000001400D2852: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  00000001400D285A: FF 15 08 19 D3 01  call        qword ptr [0000000141E04168h]
  00000001400D2860: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  00000001400D2868: 48 8B F0           mov         rsi,rax
  00000001400D286B: B9 10 00 00 00     mov         ecx,10h
  00000001400D2870: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D2872: 45 33 C0           xor         r8d,r8d
  00000001400D2875: 33 D2              xor         edx,edx
  00000001400D2877: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  00000001400D287F: FF 15 E3 18 D3 01  call        qword ptr [0000000141E04168h]
  00000001400D2885: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  00000001400D288D: 48 8B F0           mov         rsi,rax
  00000001400D2890: B9 10 00 00 00     mov         ecx,10h
  00000001400D2895: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D2897: 45 33 C0           xor         r8d,r8d
  00000001400D289A: 33 D2              xor         edx,edx
  00000001400D289C: 48 8D 8C 24 F8 00  lea         rcx,[rsp+00000000000000F8h]
                    00 00
  00000001400D28A4: FF 15 BE 18 D3 01  call        qword ptr [0000000141E04168h]
  00000001400D28AA: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  00000001400D28B2: 48 8B F0           mov         rsi,rax
  00000001400D28B5: B9 10 00 00 00     mov         ecx,10h
  00000001400D28BA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D28BC: 45 33 C0           xor         r8d,r8d
  00000001400D28BF: 33 D2              xor         edx,edx
  00000001400D28C1: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  00000001400D28C9: FF 15 99 18 D3 01  call        qword ptr [0000000141E04168h]
  00000001400D28CF: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  00000001400D28D7: 48 8B F0           mov         rsi,rax
  00000001400D28DA: B9 10 00 00 00     mov         ecx,10h
  00000001400D28DF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D28E1: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  00000001400D28E9: 48 8D B4 24 18 01  lea         rsi,[rsp+0000000000000118h]
                    00 00
  00000001400D28F1: B9 10 00 00 00     mov         ecx,10h
  00000001400D28F6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D28F8: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  00000001400D2900: 48 8D B4 24 40 01  lea         rsi,[rsp+0000000000000140h]
                    00 00
  00000001400D2908: B9 10 00 00 00     mov         ecx,10h
  00000001400D290D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001400D290F: 48 8D 84 24 F0 01  lea         rax,[rsp+00000000000001F0h]
                    00 00
  00000001400D2917: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  00000001400D291C: 48 8D 84 24 E0 01  lea         rax,[rsp+00000000000001E0h]
                    00 00
  00000001400D2924: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  00000001400D2929: 48 8D 84 24 D0 01  lea         rax,[rsp+00000000000001D0h]
                    00 00
  00000001400D2931: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001400D2936: 48 8D 84 24 C0 01  lea         rax,[rsp+00000000000001C0h]
                    00 00
  00000001400D293E: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001400D2943: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  00000001400D294B: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001400D2950: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  00000001400D2958: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001400D295D: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  00000001400D2965: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001400D296A: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  00000001400D2972: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001400D2977: 4C 8D 8C 24 70 01  lea         r9,[rsp+0000000000000170h]
                    00 00
  00000001400D297F: 4C 8D 84 24 60 01  lea         r8,[rsp+0000000000000160h]
                    00 00
  00000001400D2987: 48 8D 15 B2 41 09  lea         rdx,[0000000142166B40h]
                    02
  00000001400D298E: 48 8D 0D 6B 3D CA  lea         rcx,[0000000143D76700h]
                    03
  00000001400D2995: E8 26 CA 1F 00     call        00000001402CF3C0
  00000001400D299A: E9 63 FD FF FF     jmp         00000001400D2702
  00000001400D299F: 48 8B 8C 24 78 02  mov         rcx,qword ptr [rsp+0000000000000278h]
                    00 00
  00000001400D29A7: FF 15 33 30 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D29AD: 90                 nop
  00000001400D29AE: 48 8B 8C 24 88 02  mov         rcx,qword ptr [rsp+0000000000000288h]
                    00 00
  00000001400D29B6: FF 15 24 30 D3 01  call        qword ptr [0000000141E059E0h]
  00000001400D29BC: 48 81 C4 58 02 00  add         rsp,258h
                    00
  00000001400D29C3: 5F                 pop         rdi
  00000001400D29C4: 5E                 pop         rsi
  00000001400D29C5: C3                 ret

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091A5B0: 4C 89 4C 24 20     mov         qword ptr [rsp+20h],r9
  000000014091A5B5: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014091A5BA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091A5BF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091A5C4: 48 81 EC A8 00 00  sub         rsp,0A8h
                    00
  000000014091A5CB: 48 C7 44 24 78 FE  mov         qword ptr [rsp+78h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091A5D4: 48 8D 44 24 48     lea         rax,[rsp+48h]
  000000014091A5D9: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014091A5DE: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091A5E6: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000014091A5EB: FF 15 E7 B3 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A5F1: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000014091A5F9: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091A601: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A604: 48 8B 94 24 80 00  mov         rdx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A60C: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091A614: FF 90 30 01 00 00  call        qword ptr [rax+0000000000000130h]
  000000014091A61A: 0F B6 C0           movzx       eax,al
  000000014091A61D: 85 C0              test        eax,eax
  000000014091A61F: 0F 84 DC 00 00 00  je          000000014091A701
  000000014091A625: 48 8D 44 24 58     lea         rax,[rsp+58h]
  000000014091A62A: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014091A62F: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091A637: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014091A63C: FF 15 96 B3 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A642: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  000000014091A64A: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091A652: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A655: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091A65D: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091A665: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  000000014091A66B: 48 8B 8C 24 B0 00  mov         rcx,qword ptr [rsp+00000000000000B0h]
                    00 00
  000000014091A673: 48 83 C1 38        add         rcx,38h
  000000014091A677: 8B D0              mov         edx,eax
  000000014091A679: E8 92 65 84 FF     call        0000000140160C10
  000000014091A67E: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A681: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000014091A686: 48 8D 44 24 68     lea         rax,[rsp+68h]
  000000014091A68B: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000014091A690: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014091A698: 48 8B 4C 24 70     mov         rcx,qword ptr [rsp+70h]
  000000014091A69D: FF 15 35 B3 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A6A3: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  000000014091A6AB: 48 8B 84 24 F0 00  mov         rax,qword ptr [rsp+00000000000000F0h]
                    00 00
  000000014091A6B3: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014091A6B8: 48 8B 84 24 E8 00  mov         rax,qword ptr [rsp+00000000000000E8h]
                    00 00
  000000014091A6C0: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000014091A6C5: F2 0F 10 84 24 E0  movsd       xmm0,mmword ptr [rsp+00000000000000E0h]
                    00 00 00
  000000014091A6CE: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  000000014091A6D4: 8B 84 24 D8 00 00  mov         eax,dword ptr [rsp+00000000000000D8h]
                    00
  000000014091A6DB: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091A6DF: 44 8B 8C 24 D0 00  mov         r9d,dword ptr [rsp+00000000000000D0h]
                    00 00
  000000014091A6E7: 4C 8B 84 24 90 00  mov         r8,qword ptr [rsp+0000000000000090h]
                    00 00
  000000014091A6EF: 8B 94 24 C0 00 00  mov         edx,dword ptr [rsp+00000000000000C0h]
                    00
  000000014091A6F6: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  000000014091A6FB: E8 40 C4 EF FF     call        0000000140816B40
  000000014091A700: 90                 nop
  000000014091A701: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  000000014091A709: FF 15 D1 B2 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091A70F: 90                 nop
  000000014091A710: 48 8B 8C 24 C8 00  mov         rcx,qword ptr [rsp+00000000000000C8h]
                    00 00
  000000014091A718: FF 15 C2 B2 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091A71E: 48 81 C4 A8 00 00  add         rsp,0A8h
                    00
  000000014091A725: C3                 ret

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091A950: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091A955: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091A95A: 48 83 EC 78        sub         rsp,78h
  000000014091A95E: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091A967: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091A96F: 48 8D 44 24 30     lea         rax,[rsp+30h]
  000000014091A974: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014091A979: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091A981: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014091A986: FF 15 4C B0 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A98C: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014091A991: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A999: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A99C: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  000000014091A9A1: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A9A9: FF 90 30 01 00 00  call        qword ptr [rax+0000000000000130h]
  000000014091A9AF: 0F B6 C0           movzx       eax,al
  000000014091A9B2: 85 C0              test        eax,eax
  000000014091A9B4: 74 69              je          000000014091AA1F
  000000014091A9B6: 48 8D 44 24 40     lea         rax,[rsp+40h]
  000000014091A9BB: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014091A9C0: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091A9C8: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014091A9CD: FF 15 05 B0 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091A9D3: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014091A9D8: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A9E0: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091A9E3: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  000000014091A9E8: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A9F0: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  000000014091A9F6: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091A9FE: 48 83 C1 38        add         rcx,38h
  000000014091AA02: 8B D0              mov         edx,eax
  000000014091AA04: E8 07 62 84 FF     call        0000000140160C10
  000000014091AA09: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091AA0C: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014091AA11: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014091AA16: E8 95 C7 EF FF     call        00000001408171B0
  000000014091AA1B: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091AA1F: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091AA23: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  000000014091AA27: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091AA2F: FF 15 AB AF 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091AA35: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  000000014091AA39: 48 83 C4 78        add         rsp,78h
  000000014091AA3D: C3                 ret

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091AA50: 44 89 44 24 18     mov         dword ptr [rsp+18h],r8d
  000000014091AA55: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  000000014091AA5A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091AA5F: 48 83 EC 78        sub         rsp,78h
  000000014091AA63: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091AA6C: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  000000014091AA75: 48 8D 44 24 30     lea         rax,[rsp+30h]
  000000014091AA7A: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000014091AA7F: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091AA87: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  000000014091AA8C: FF 15 46 AF 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091AA92: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014091AA97: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091AA9F: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091AAA2: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  000000014091AAA7: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091AAAF: FF 90 30 01 00 00  call        qword ptr [rax+0000000000000130h]
  000000014091AAB5: 0F B6 C0           movzx       eax,al
  000000014091AAB8: 85 C0              test        eax,eax
  000000014091AABA: 74 78              je          000000014091AB34
  000000014091AABC: 48 8D 44 24 40     lea         rax,[rsp+40h]
  000000014091AAC1: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014091AAC6: 48 8B 94 24 88 00  mov         rdx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091AACE: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000014091AAD3: FF 15 FF AE 4E 01  call        qword ptr [0000000141E059D8h]
  000000014091AAD9: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000014091AADE: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091AAE6: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091AAE9: 48 8B 54 24 68     mov         rdx,qword ptr [rsp+68h]
  000000014091AAEE: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091AAF6: FF 90 80 01 00 00  call        qword ptr [rax+0000000000000180h]
  000000014091AAFC: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  000000014091AB04: 48 83 C1 38        add         rcx,38h
  000000014091AB08: 8B D0              mov         edx,eax
  000000014091AB0A: E8 01 61 84 FF     call        0000000140160C10
  000000014091AB0F: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091AB12: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000014091AB17: 48 8B 4C 24 28     mov         rcx,qword ptr [rsp+28h]
  000000014091AB1C: 48 83 C1 30        add         rcx,30h
  000000014091AB20: 8B 94 24 90 00 00  mov         edx,dword ptr [rsp+0000000000000090h]
                    00
  000000014091AB27: E8 E4 60 84 FF     call        0000000140160C10
  000000014091AB2C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014091AB2F: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000014091AB34: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  000000014091AB39: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014091AB3E: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  000000014091AB46: FF 15 94 AE 4E 01  call        qword ptr [0000000141E059E0h]
  000000014091AB4C: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000014091AB51: 48 83 C4 78        add         rsp,78h
  000000014091AB55: C3                 ret

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014091C7A0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014091C7A5: 48 83 EC 48        sub         rsp,48h
  000000014091C7A9: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014091C7B2: 48 8B 54 24 50     mov         rdx,qword ptr [rsp+50h]
  000000014091C7B7: 48 83 C2 38        add         rdx,38h
  000000014091C7BB: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C7C0: E8 0B D3 93 FF     call        0000000140259AD0
  000000014091C7C5: 90                 nop
  000000014091C7C6: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  000000014091C7CE: EB 0B              jmp         000000014091C7DB
  000000014091C7D0: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091C7D4: 83 C0 01           add         eax,1
  000000014091C7D7: 89 44 24 20        mov         dword ptr [rsp+20h],eax
  000000014091C7DB: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C7E0: E8 FB 82 6E FF     call        0000000140004AE0
  000000014091C7E5: 39 44 24 20        cmp         dword ptr [rsp+20h],eax
  000000014091C7E9: 7D 1A              jge         000000014091C805
  000000014091C7EB: 8B 54 24 20        mov         edx,dword ptr [rsp+20h]
  000000014091C7EF: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C7F4: E8 17 44 84 FF     call        0000000140160C10
  000000014091C7F9: 48 8B 08           mov         rcx,qword ptr [rax]
  000000014091C7FC: 8B 44 24 20        mov         eax,dword ptr [rsp+20h]
  000000014091C800: 89 41 18           mov         dword ptr [rcx+18h],eax
  000000014091C803: EB CB              jmp         000000014091C7D0
  000000014091C805: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000014091C80A: E8 91 B9 8E FF     call        00000001402081A0
  000000014091C80F: 48 83 C4 48        add         rsp,48h
  000000014091C813: C3                 ret

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140921E90: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140921E95: 48 83 EC 38        sub         rsp,38h
  0000000140921E99: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  0000000140921E9E: E8 3D 02 00 00     call        00000001409220E0
  0000000140921EA3: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140921EA8: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140921EAE: 74 0B              je          0000000140921EBB
  0000000140921EB0: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  0000000140921EB5: 8B 00              mov         eax,dword ptr [rax]
  0000000140921EB7: EB 07              jmp         0000000140921EC0
  0000000140921EB9: EB 05              jmp         0000000140921EC0
  0000000140921EBB: B8 31 01 00 00     mov         eax,131h
  0000000140921EC0: 48 83 C4 38        add         rsp,38h
  0000000140921EC4: C3                 ret

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140921980: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140921985: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014092198A: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  0000000140921991: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000014092199A: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001409219A2: 48 83 C1 10        add         rcx,10h
  00000001409219A6: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001409219AE: FF 15 BC 2A 4E 01  call        qword ptr [0000000141E04470h]
  00000001409219B4: BA 0D 00 00 00     mov         edx,0Dh
  00000001409219B9: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001409219BE: FF 15 DC 2A 4E 01  call        qword ptr [0000000141E044A0h]
  00000001409219C4: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001409219CC: 48 83 C1 10        add         rcx,10h
  00000001409219D0: 41 B8 01 00 00 00  mov         r8d,1
  00000001409219D6: 0F B7 10           movzx       edx,word ptr [rax]
  00000001409219D9: FF 15 01 35 4E 01  call        qword ptr [0000000141E04EE0h]
  00000001409219DF: BA 0A 00 00 00     mov         edx,0Ah
  00000001409219E4: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001409219E9: FF 15 B1 2A 4E 01  call        qword ptr [0000000141E044A0h]
  00000001409219EF: 0F B7 10           movzx       edx,word ptr [rax]
  00000001409219F2: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001409219F7: FF 15 4B 3E 4E 01  call        qword ptr [0000000141E05848h]
  00000001409219FD: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140921A02: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140921A07: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140921A0C: BA 0D 00 00 00     mov         edx,0Dh
  0000000140921A11: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140921A16: FF 15 84 2A 4E 01  call        qword ptr [0000000141E044A0h]
  0000000140921A1C: 0F B7 10           movzx       edx,word ptr [rax]
  0000000140921A1F: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140921A24: FF 15 1E 3E 4E 01  call        qword ptr [0000000141E05848h]
  0000000140921A2A: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140921A2F: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140921A34: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140921A39: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140921A3E: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140921A43: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140921A48: E8 A3 2E 6E FF     call        00000001400048F0
  0000000140921A4D: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140921A55: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140921A5D: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140921A65: BA 0A 00 00 00     mov         edx,0Ah
  0000000140921A6A: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140921A6F: FF 15 2B 2A 4E 01  call        qword ptr [0000000141E044A0h]
  0000000140921A75: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140921A7D: 48 83 C1 10        add         rcx,10h
  0000000140921A81: 41 B9 01 00 00 00  mov         r9d,1
  0000000140921A87: 4C 8B 84 24 88 00  mov         r8,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140921A8F: 0F B7 10           movzx       edx,word ptr [rax]
  0000000140921A92: FF 15 A8 34 4E 01  call        qword ptr [0000000141E04F40h]
  0000000140921A98: 90                 nop
  0000000140921A99: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140921A9E: FF 15 3C 3F 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921AA4: 90                 nop
  0000000140921AA5: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140921AAA: FF 15 30 3F 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921AB0: 90                 nop
  0000000140921AB1: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140921AB6: FF 15 24 3F 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921ABC: 90                 nop
  0000000140921ABD: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140921AC5: FF 15 15 3F 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921ACB: 48 81 C4 98 00 00  add         rsp,98h
                    00
  0000000140921AD2: C3                 ret

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140921AE0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140921AE5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140921AEA: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  0000000140921AF1: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140921AFA: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140921B02: 48 83 C1 18        add         rcx,18h
  0000000140921B06: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140921B0E: FF 15 5C 29 4E 01  call        qword ptr [0000000141E04470h]
  0000000140921B14: BA 0D 00 00 00     mov         edx,0Dh
  0000000140921B19: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140921B1E: FF 15 7C 29 4E 01  call        qword ptr [0000000141E044A0h]
  0000000140921B24: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140921B2C: 48 83 C1 18        add         rcx,18h
  0000000140921B30: 41 B8 01 00 00 00  mov         r8d,1
  0000000140921B36: 0F B7 10           movzx       edx,word ptr [rax]
  0000000140921B39: FF 15 A1 33 4E 01  call        qword ptr [0000000141E04EE0h]
  0000000140921B3F: BA 0A 00 00 00     mov         edx,0Ah
  0000000140921B44: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140921B49: FF 15 51 29 4E 01  call        qword ptr [0000000141E044A0h]
  0000000140921B4F: 0F B7 10           movzx       edx,word ptr [rax]
  0000000140921B52: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140921B57: FF 15 EB 3C 4E 01  call        qword ptr [0000000141E05848h]
  0000000140921B5D: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140921B62: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140921B67: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140921B6C: BA 0D 00 00 00     mov         edx,0Dh
  0000000140921B71: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140921B76: FF 15 24 29 4E 01  call        qword ptr [0000000141E044A0h]
  0000000140921B7C: 0F B7 10           movzx       edx,word ptr [rax]
  0000000140921B7F: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140921B84: FF 15 BE 3C 4E 01  call        qword ptr [0000000141E05848h]
  0000000140921B8A: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140921B8F: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140921B94: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140921B99: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140921B9E: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140921BA3: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140921BA8: E8 43 2D 6E FF     call        00000001400048F0
  0000000140921BAD: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140921BB5: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140921BBD: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140921BC5: BA 0A 00 00 00     mov         edx,0Ah
  0000000140921BCA: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140921BCF: FF 15 CB 28 4E 01  call        qword ptr [0000000141E044A0h]
  0000000140921BD5: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140921BDD: 48 83 C1 18        add         rcx,18h
  0000000140921BE1: 41 B9 01 00 00 00  mov         r9d,1
  0000000140921BE7: 4C 8B 84 24 88 00  mov         r8,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140921BEF: 0F B7 10           movzx       edx,word ptr [rax]
  0000000140921BF2: FF 15 48 33 4E 01  call        qword ptr [0000000141E04F40h]
  0000000140921BF8: 90                 nop
  0000000140921BF9: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140921BFE: FF 15 DC 3D 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921C04: 90                 nop
  0000000140921C05: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140921C0A: FF 15 D0 3D 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921C10: 90                 nop
  0000000140921C11: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140921C16: FF 15 C4 3D 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921C1C: 90                 nop
  0000000140921C1D: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140921C25: FF 15 B5 3D 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921C2B: 48 81 C4 98 00 00  add         rsp,98h
                    00
  0000000140921C32: C3                 ret

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140921C40: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140921C45: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140921C4A: 48 81 EC 98 00 00  sub         rsp,98h
                    00
  0000000140921C51: 48 C7 44 24 58 FE  mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140921C5A: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140921C62: 48 83 C1 20        add         rcx,20h
  0000000140921C66: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140921C6E: FF 15 FC 27 4E 01  call        qword ptr [0000000141E04470h]
  0000000140921C74: BA 0D 00 00 00     mov         edx,0Dh
  0000000140921C79: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  0000000140921C7E: FF 15 1C 28 4E 01  call        qword ptr [0000000141E044A0h]
  0000000140921C84: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140921C8C: 48 83 C1 20        add         rcx,20h
  0000000140921C90: 41 B8 01 00 00 00  mov         r8d,1
  0000000140921C96: 0F B7 10           movzx       edx,word ptr [rax]
  0000000140921C99: FF 15 41 32 4E 01  call        qword ptr [0000000141E04EE0h]
  0000000140921C9F: BA 0A 00 00 00     mov         edx,0Ah
  0000000140921CA4: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  0000000140921CA9: FF 15 F1 27 4E 01  call        qword ptr [0000000141E044A0h]
  0000000140921CAF: 0F B7 10           movzx       edx,word ptr [rax]
  0000000140921CB2: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140921CB7: FF 15 8B 3B 4E 01  call        qword ptr [0000000141E05848h]
  0000000140921CBD: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140921CC2: 48 8B 44 24 60     mov         rax,qword ptr [rsp+60h]
  0000000140921CC7: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  0000000140921CCC: BA 0D 00 00 00     mov         edx,0Dh
  0000000140921CD1: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140921CD6: FF 15 C4 27 4E 01  call        qword ptr [0000000141E044A0h]
  0000000140921CDC: 0F B7 10           movzx       edx,word ptr [rax]
  0000000140921CDF: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140921CE4: FF 15 5E 3B 4E 01  call        qword ptr [0000000141E05848h]
  0000000140921CEA: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140921CEF: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140921CF4: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  0000000140921CF9: 4C 8B 44 24 68     mov         r8,qword ptr [rsp+68h]
  0000000140921CFE: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  0000000140921D03: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140921D08: E8 E3 2B 6E FF     call        00000001400048F0
  0000000140921D0D: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  0000000140921D15: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  0000000140921D1D: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  0000000140921D25: BA 0A 00 00 00     mov         edx,0Ah
  0000000140921D2A: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  0000000140921D2F: FF 15 6B 27 4E 01  call        qword ptr [0000000141E044A0h]
  0000000140921D35: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140921D3D: 48 83 C1 20        add         rcx,20h
  0000000140921D41: 41 B9 01 00 00 00  mov         r9d,1
  0000000140921D47: 4C 8B 84 24 88 00  mov         r8,qword ptr [rsp+0000000000000088h]
                    00 00
  0000000140921D4F: 0F B7 10           movzx       edx,word ptr [rax]
  0000000140921D52: FF 15 E8 31 4E 01  call        qword ptr [0000000141E04F40h]
  0000000140921D58: 90                 nop
  0000000140921D59: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140921D5E: FF 15 7C 3C 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921D64: 90                 nop
  0000000140921D65: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  0000000140921D6A: FF 15 70 3C 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921D70: 90                 nop
  0000000140921D71: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140921D76: FF 15 64 3C 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921D7C: 90                 nop
  0000000140921D7D: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140921D85: FF 15 55 3C 4E 01  call        qword ptr [0000000141E059E0h]
  0000000140921D8B: 48 81 C4 98 00 00  add         rsp,98h
                    00
  0000000140921D92: C3                 ret

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001409FF590: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001409FF594: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001409FF599: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  00000001409FF59E: 8B 44 24 10        mov         eax,dword ptr [rsp+10h]
  00000001409FF5A2: 89 41 1C           mov         dword ptr [rcx+1Ch],eax
  00000001409FF5A5: C3                 ret
  00000001409FF5A6: CC                 int         3
  00000001409FF5A7: CC                 int         3
  00000001409FF5A8: CC                 int         3
  00000001409FF5A9: CC                 int         3
  00000001409FF5AA: CC                 int         3
  00000001409FF5AB: CC                 int         3
  00000001409FF5AC: CC                 int         3
  00000001409FF5AD: CC                 int         3
  00000001409FF5AE: CC                 int         3
  00000001409FF5AF: CC                 int         3
  00000001409FF5B0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001409FF5B5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001409FF5BA: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  00000001409FF5BF: 48

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D8040: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400D8044: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D8049: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  00000001400D804E: 8B 44 24 10        mov         eax,dword ptr [rsp+10h]
  00000001400D8052: 89 41 24           mov         dword ptr [rcx+24h],eax
  00000001400D8055: C3                 ret
  00000001400D8056: CC                 int         3
  00000001400D8057: CC                 int         3
  00000001400D8058: CC                 int         3
  00000001400D8059: CC                 int         3
  00000001400D805A: CC                 int         3
  00000001400D805B: CC                 int         3
  00000001400D805C: CC                 int         3
  00000001400D805D: CC                 int         3
  00000001400D805E: CC                 int         3
  00000001400D805F: CC                 int         3
  00000001400D8060: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D8065: 48 83 EC 38        sub         rsp,38h
  00000001400D8069: 48
  00000001400D806A: C7
  00000001400D806B: 44 24 20           and         al,20h
  00000001400D806E: FE
  00000001400D806F: FF

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140917170: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140917174: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140917179: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  000000014091717E: 0F B6 44 24 10     movzx       eax,byte ptr [rsp+10h]
  0000000140917183: 88 41 31           mov         byte ptr [rcx+31h],al
  0000000140917186: C3                 ret
  0000000140917187: CC                 int         3
  0000000140917188: CC                 int         3
  0000000140917189: CC                 int         3
  000000014091718A: CC                 int         3
  000000014091718B: CC                 int         3
  000000014091718C: CC                 int         3
  000000014091718D: CC                 int         3
  000000014091718E: CC                 int         3
  000000014091718F: CC                 int         3
  0000000140917190: 88 54 24 10        mov         byte ptr [rsp+10h],dl
  0000000140917194: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140917199: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  000000014091719E: 0F B6 44 24 10     movzx       eax,byte ptr [rsp+10h]
  00000001409171A3: 88 41 32           mov         byte ptr [rcx+32h],al
  00000001409171A6: C3                 ret
  00000001409171A7: CC                 int         3
  00000001409171A8: CC                 int         3
  00000001409171A9: CC                 int         3
  00000001409171AA: CC                 int         3
  00000001409171AB: CC                 int         3
  00000001409171AC: CC                 int         3
  00000001409171AD: CC                 int         3
  00000001409171AE: CC                 int         3
  00000001409171AF: CC                 int         3

  Summary

     1E02000 .text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014010F1E0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  000000014010F1E4: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014010F1E9: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  000000014010F1EE: 8B 44 24 10        mov         eax,dword ptr [rsp+10h]
  000000014010F1F2: 89 81 80 00 00 00  mov         dword ptr [rcx+0000000000000080h],eax
  000000014010F1F8: C3                 ret
  000000014010F1F9: CC                 int         3
  000000014010F1FA: CC                 int         3
  000000014010F1FB: CC                 int         3
  000000014010F1FC: CC                 int         3
  000000014010F1FD: CC                 int         3
  000000014010F1FE: CC                 int         3
  000000014010F1FF: CC                 int         3

  Summary

     1E02000 .text

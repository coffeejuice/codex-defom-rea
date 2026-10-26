Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D75A0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400D75A5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D75AA: 53                 push        rbx
  00000001400D75AB: 48 81 EC D0 00 00  sub         rsp,0D0h
                    00
  00000001400D75B2: 48 C7 44 24 70 FE  mov         qword ptr [rsp+70h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400D75BB: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001400D75C0: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D75C8: 48 63 48 40        movsxd      rcx,dword ptr [rax+40h]
  00000001400D75CC: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D75D4: 0F B6 44 08 70     movzx       eax,byte ptr [rax+rcx+70h]
  00000001400D75D9: 85 C0              test        eax,eax
  00000001400D75DB: 0F 85 80 01 00 00  jne         00000001400D7761
  00000001400D75E1: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001400D75E6: E8 05 1F 83 00     call        00000001409094F0
  00000001400D75EB: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001400D75F0: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000001400D75F5: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400D75FD: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001400D7602: FF 15 48 CC D2 01  call        qword ptr [0000000141E04250h]
  00000001400D7608: 48 8B D8           mov         rbx,rax
  00000001400D760B: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001400D7610: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D7618: E8 53 41 FD FF     call        00000001400AB770
  00000001400D761D: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001400D7625: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001400D762D: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001400D7635: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001400D763A: FF 15 10 CC D2 01  call        qword ptr [0000000141E04250h]
  00000001400D7640: 44 0F B7 00        movzx       r8d,word ptr [rax]
  00000001400D7644: 48 8B 94 24 E8 00  mov         rdx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001400D764C: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001400D7651: E8 1A D3 F2 FF     call        0000000140004970
  00000001400D7656: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000001400D765E: 48 8B 84 24 98 00  mov         rax,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001400D7666: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001400D766E: 4C 8B 84 24 90 00  mov         r8,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001400D7676: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400D767E: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D7683: E8 68 D2 F2 FF     call        00000001400048F0
  00000001400D7688: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001400D7690: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001400D7698: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001400D76A0: 44 0F B7 03        movzx       r8d,word ptr [rbx]
  00000001400D76A4: 48 8B 94 24 B0 00  mov         rdx,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400D76AC: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400D76B1: E8 BA D2 F2 FF     call        0000000140004970
  00000001400D76B6: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000001400D76BE: 48 8B 84 24 B8 00  mov         rax,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001400D76C6: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  00000001400D76CE: 4C 8B 84 24 80 00  mov         r8,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400D76D6: 48 8B 94 24 C0 00  mov         rdx,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000001400D76DE: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400D76E3: E8 08 D2 F2 FF     call        00000001400048F0
  00000001400D76E8: 90                 nop
  00000001400D76E9: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400D76EE: FF 15 EC E2 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D76F4: 90                 nop
  00000001400D76F5: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D76FA: FF 15 E0 E2 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7700: 90                 nop
  00000001400D7701: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001400D7706: FF 15 D4 E2 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D770C: 90                 nop
  00000001400D770D: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001400D7712: FF 15 C8 E2 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7718: 90                 nop
  00000001400D7719: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001400D771E: FF 15 BC E2 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7724: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D772C: 48 63 48 40        movsxd      rcx,dword ptr [rax+40h]
  00000001400D7730: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D7738: 48 8D 54 C8 58     lea         rdx,[rax+rcx*8+58h]
  00000001400D773D: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  00000001400D7742: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D774A: 8B 48 40           mov         ecx,dword ptr [rax+40h]
  00000001400D774D: E8 3E DB 83 00     call        0000000140915290
  00000001400D7752: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000001400D7756: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400D775B: FF 15 7F E2 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7761: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001400D7766: 88 44 24 68        mov         byte ptr [rsp+68h],al
  00000001400D776A: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001400D7772: FF 15 68 E2 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7778: 8A 44 24 68        mov         al,byte ptr [rsp+68h]
  00000001400D777C: 48 81 C4 D0 00 00  add         rsp,0D0h
                    00
  00000001400D7783: 5B                 pop         rbx
  00000001400D7784: C3                 ret

  Summary

     1E02000 .text

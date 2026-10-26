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
  00000001400D7785: CC                 int         3
  00000001400D7786: CC                 int         3
  00000001400D7787: CC                 int         3
  00000001400D7788: CC                 int         3
  00000001400D7789: CC                 int         3
  00000001400D778A: CC                 int         3
  00000001400D778B: CC                 int         3
  00000001400D778C: CC                 int         3
  00000001400D778D: CC                 int         3
  00000001400D778E: CC                 int         3
  00000001400D778F: CC                 int         3
  00000001400D7790: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001400D7795: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400D7799: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D779E: 48 83 EC 38        sub         rsp,38h
  00000001400D77A2: 48 C7 44 24 28 FE  mov         qword ptr [rsp+28h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400D77AB: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001400D77B0: 83 7C 24 48 FF     cmp         dword ptr [rsp+48h],0FFFFFFFFh
  00000001400D77B5: 7E 3B              jle         00000001400D77F2
  00000001400D77B7: 83 7C 24 48 03     cmp         dword ptr [rsp+48h],3
  00000001400D77BC: 7D 34              jge         00000001400D77F2
  00000001400D77BE: 48 63 4C 24 48     movsxd      rcx,dword ptr [rsp+48h]
  00000001400D77C3: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400D77C8: 0F B6 44 08 70     movzx       eax,byte ptr [rax+rcx+70h]
  00000001400D77CD: 85 C0              test        eax,eax
  00000001400D77CF: 75 21              jne         00000001400D77F2
  00000001400D77D1: 48 63 4C 24 48     movsxd      rcx,dword ptr [rsp+48h]
  00000001400D77D6: 48 8B 44 24 40     mov         rax,qword ptr [rsp+40h]
  00000001400D77DB: 48 8D 54 C8 58     lea         rdx,[rax+rcx*8+58h]
  00000001400D77E0: 4C 8B 44 24 50     mov         r8,qword ptr [rsp+50h]
  00000001400D77E5: 8B 4C 24 48        mov         ecx,dword ptr [rsp+48h]
  00000001400D77E9: E8 72 DB 83 00     call        0000000140915360
  00000001400D77EE: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000001400D77F2: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001400D77F7: 88 44 24 21        mov         byte ptr [rsp+21h],al
  00000001400D77FB: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  00000001400D7800: FF 15 DA E1 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7806: 8A 44 24 21        mov         al,byte ptr [rsp+21h]
  00000001400D780A: 48 83 C4 38        add         rsp,38h
  00000001400D780E: C3                 ret
  00000001400D780F: CC                 int         3
  00000001400D7810: CC                 int         3
  00000001400D7811: CC                 int         3
  00000001400D7812: CC                 int         3
  00000001400D7813: CC                 int         3
  00000001400D7814: CC                 int         3
  00000001400D7815: CC                 int         3
  00000001400D7816: CC                 int         3
  00000001400D7817: CC                 int         3
  00000001400D7818: CC                 int         3
  00000001400D7819: CC                 int         3
  00000001400D781A: CC                 int         3
  00000001400D781B: CC                 int         3
  00000001400D781C: CC                 int         3
  00000001400D781D: CC                 int         3
  00000001400D781E: CC                 int         3
  00000001400D781F: CC                 int         3
  00000001400D7820: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001400D7825: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D782A: 53                 push        rbx
  00000001400D782B: 48 81 EC D0 00 00  sub         rsp,0D0h
                    00
  00000001400D7832: 48 C7 44 24 78 FE  mov         qword ptr [rsp+78h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400D783B: C6 44 24 28 00     mov         byte ptr [rsp+28h],0
  00000001400D7840: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001400D7845: E8 A6 1C 83 00     call        00000001409094F0
  00000001400D784A: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  00000001400D7852: 48 8B 84 24 80 00  mov         rax,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001400D785A: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001400D7862: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001400D7867: FF 15 E3 C9 D2 01  call        qword ptr [0000000141E04250h]
  00000001400D786D: 48 8B D8           mov         rbx,rax
  00000001400D7870: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000001400D7875: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D787D: E8 EE 3E FD FF     call        00000001400AB770
  00000001400D7882: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001400D788A: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001400D7892: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  00000001400D789A: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  00000001400D789F: FF 15 AB C9 D2 01  call        qword ptr [0000000141E04250h]
  00000001400D78A5: 44 0F B7 00        movzx       r8d,word ptr [rax]
  00000001400D78A9: 48 8B 94 24 E8 00  mov         rdx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001400D78B1: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D78B6: E8 B5 D0 F2 FF     call        0000000140004970
  00000001400D78BB: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001400D78C3: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001400D78CB: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001400D78D3: 4C 8B 84 24 98 00  mov         r8,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001400D78DB: 48 8B 94 24 A8 00  mov         rdx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001400D78E3: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400D78E8: E8 03 D0 F2 FF     call        00000001400048F0
  00000001400D78ED: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  00000001400D78F5: 48 8B 84 24 B0 00  mov         rax,qword ptr [rsp+00000000000000B0h]
                    00 00
  00000001400D78FD: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000001400D7905: 44 0F B7 03        movzx       r8d,word ptr [rbx]
  00000001400D7909: 48 8B 94 24 B8 00  mov         rdx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001400D7911: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001400D7916: E8 55 D0 F2 FF     call        0000000140004970
  00000001400D791B: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  00000001400D7923: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  00000001400D792B: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  00000001400D7933: 4C 8B 84 24 88 00  mov         r8,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001400D793B: 48 8B 94 24 C8 00  mov         rdx,qword ptr [rsp+00000000000000C8h]
                    00 00
  00000001400D7943: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001400D7948: E8 A3 CF F2 FF     call        00000001400048F0
  00000001400D794D: 90                 nop
  00000001400D794E: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001400D7953: FF 15 87 E0 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7959: 90                 nop
  00000001400D795A: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  00000001400D795F: FF 15 7B E0 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7965: 90                 nop
  00000001400D7966: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001400D796B: FF 15 6F E0 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7971: 90                 nop
  00000001400D7972: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001400D7977: FF 15 63 E0 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D797D: 90                 nop
  00000001400D797E: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001400D7983: FF 15 57 E0 D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7989: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001400D798E: E8 2D 08 00 00     call        00000001400D81C0
  00000001400D7993: 90                 nop
  00000001400D7994: 48 8B 8C 24 E0 00  mov         rcx,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D799C: 48 83 C1 40        add         rcx,40h
  00000001400D79A0: 4C 8D 44 24 30     lea         r8,[rsp+30h]
  00000001400D79A5: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001400D79AA: E8 31 DD 83 00     call        00000001409156E0
  00000001400D79AF: 88 44 24 28        mov         byte ptr [rsp+28h],al
  00000001400D79B3: 0F B6 44 24 28     movzx       eax,byte ptr [rsp+28h]
  00000001400D79B8: 85 C0              test        eax,eax
  00000001400D79BA: 74 76              je          00000001400D7A32
  00000001400D79BC: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D79C4: 48 63 48 40        movsxd      rcx,dword ptr [rax+40h]
  00000001400D79C8: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D79D0: 48 8D 4C C8 58     lea         rcx,[rax+rcx*8+58h]
  00000001400D79D5: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001400D79DA: E8 51 08 00 00     call        00000001400D8230
  00000001400D79DF: 4C 8B 9C 24 E0 00  mov         r11,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D79E7: 49 63 4B 40        movsxd      rcx,dword ptr [r11+40h]
  00000001400D79EB: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D79F3: C6 44 08 70 00     mov         byte ptr [rax+rcx+70h],0
  00000001400D79F8: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D7A00: 48 63 48 40        movsxd      rcx,dword ptr [rax+40h]
  00000001400D7A04: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D7A0C: 48 8D 4C C8 58     lea         rcx,[rax+rcx*8+58h]
  00000001400D7A11: E8 2A DB 9D 00     call        0000000140AB5540
  00000001400D7A16: 4C 8B D8           mov         r11,rax
  00000001400D7A19: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D7A21: 48 63 48 40        movsxd      rcx,dword ptr [rax+40h]
  00000001400D7A25: 48 8B 84 24 E0 00  mov         rax,qword ptr [rsp+00000000000000E0h]
                    00 00
  00000001400D7A2D: 44 89 5C 88 48     mov         dword ptr [rax+rcx*4+48h],r11d
  00000001400D7A32: 0F B6 44 24 28     movzx       eax,byte ptr [rsp+28h]
  00000001400D7A37: 88 44 24 70        mov         byte ptr [rsp+70h],al
  00000001400D7A3B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001400D7A40: E8 BB 4C FE FF     call        00000001400BC700
  00000001400D7A45: 90                 nop
  00000001400D7A46: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001400D7A4B: FF 15 8F DF D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7A51: 90                 nop
  00000001400D7A52: 48 8B 8C 24 E8 00  mov         rcx,qword ptr [rsp+00000000000000E8h]
                    00 00
  00000001400D7A5A: FF 15 80 DF D2 01  call        qword ptr [0000000141E059E0h]
  00000001400D7A60: 8A 44 24 70        mov         al,byte ptr [rsp+70h]
  00000001400D7A64: 48 81 C4 D0 00 00  add         rsp,0D0h
                    00
  00000001400D7A6B: 5B                 pop         rbx
  00000001400D7A6C: C3                 ret
  00000001400D7A6D: CC                 int         3
  00000001400D7A6E: CC                 int         3
  00000001400D7A6F: CC                 int         3
  00000001400D7A70: CC                 int         3
  00000001400D7A71: CC                 int         3
  00000001400D7A72: CC                 int         3
  00000001400D7A73: CC                 int         3
  00000001400D7A74: CC                 int         3
  00000001400D7A75: CC                 int         3
  00000001400D7A76: CC                 int         3
  00000001400D7A77: CC                 int         3
  00000001400D7A78: CC                 int         3
  00000001400D7A79: CC                 int         3
  00000001400D7A7A: CC                 int         3
  00000001400D7A7B: CC                 int         3
  00000001400D7A7C: CC                 int         3
  00000001400D7A7D: CC                 int         3
  00000001400D7A7E: CC                 int         3
  00000001400D7A7F: CC                 int         3
  00000001400D7A80: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001400D7A85: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  00000001400D7A89: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001400D7A8E: 48 83 EC 48        sub         rsp,48h
  00000001400D7A92: 48 C7 44 24 38 FE  mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000001400D7A9B: C6 44 24 20 00     mov         byte ptr [rsp+20h],0
  00000001400D7AA0: 83 7C 24 58 FF     cmp         dword ptr [rsp+58h],0FFFFFFFFh
  00000001400D7AA5: 0F 8E 82 00 00 00  jle         00000001400D7B2D
  00000001400D7AAB: 83 7C 24 58 03     cmp         dword ptr [rsp+58h],3
  00000001400D7AB0: 7D 7B              jge         00000001400D7B2D
  00000001400D7AB2: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400D7AB7: E8 04 07 00 00     call        00000001400D81C0
  00000001400D7ABC: 90                 nop
  00000001400D7ABD: 4C 8B 44 24 60     mov         r8,qword ptr [rsp+60h]
  00000001400D7AC2: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001400D7AC7: 8B 4C 24 58        mov         ecx,dword ptr [rsp+58h]
  00000001400D7ACB: E8 90 D8 83 00     call        0000000140915360
  00000001400D7AD0: 88 44 24 20        mov         byte ptr [rsp+20h],al
  00000001400D7AD4: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001400D7AD9: 85 C0              test        eax,eax
  00000001400D7ADB: 74 46              je          00000001400D7B23
  00000001400D7ADD: 48 63 4C 24 58     movsxd      rcx,dword ptr [rsp+58h]
  00000001400D7AE2: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400D7AE7: 48 8D 4C C8 58     lea         rcx,[rax+rcx*8+58h]
  00000001400D7AEC: E8 4F DA 9D 00     call        0000000140AB5540
  00000001400D7AF1: 48 85 C0           test        rax,rax
  00000001400D7AF4: 76 2D              jbe         00000001400D7B23
  00000001400D7AF6: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400D7AFB: 48 63 48 40        movsxd      rcx,dword ptr [rax+40h]
  00000001400D7AFF: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400D7B04: 48 8D 4C C8 58     lea         rcx,[rax+rcx*8+58h]
  00000001400D7B09: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001400D7B0E: E8 1D 07 00 00     call        00000001400D8230
  00000001400D7B13: 4C 63 5C 24 58     movsxd      r11,dword ptr [rsp+58h]
  00000001400D7B18: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  00000001400D7B1D: 42 C6 44 18 70 00  mov         byte ptr [rax+r11+70h],0
  00000001400D7B23: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001400D7B28: E8 D3 4B FE FF     call        00000001400BC700
  00000001400D7B2D: 0F B6 44 24 20     movzx       eax,byte ptr [rsp+20h]
  00000001400D7B32: 88 44 24 30        mov         byte ptr [rsp+30h],al
  00000001400D7B36: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  00000001400D7B3B: FF
  00000001400D7B3C: 15
  00000001400D7B3D: 9F                 lahf
  00000001400D7B3E: DE D2              fcomp5      st(2)

  Summary

     1E02000 .text

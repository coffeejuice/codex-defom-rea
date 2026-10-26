Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407AE210: 44 88 44 24 18     mov         byte ptr [rsp+18h],r8b
  00000001407AE215: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001407AE21A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407AE21F: 48 81 EC E8 01 00  sub         rsp,1E8h
                    00
  00000001407AE226: 48 C7 84 24 40 01  mov         qword ptr [rsp+0000000000000140h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001407AE232: C7 84 24 38 01 00  mov         dword ptr [rsp+0000000000000138h],0
                    00 00 00 00 00
  00000001407AE23D: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000001407AE242: 48 8B 8C 24 F8 01  mov         rcx,qword ptr [rsp+00000000000001F8h]
                    00 00
  00000001407AE24A: FF 15 68 E3 65 01  call        qword ptr [0000000141E0C5B8h]
  00000001407AE250: 90                 nop
  00000001407AE251: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407AE256: FF 15 DC E2 65 01  call        qword ptr [0000000141E0C538h]
  00000001407AE25C: 0F B6 C0           movzx       eax,al
  00000001407AE25F: 85 C0              test        eax,eax
  00000001407AE261: 0F 85 0C 05 00 00  jne         00000001407AE773
  00000001407AE267: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001407AE26C: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407AE271: FF 15 39 E3 65 01  call        qword ptr [0000000141E0C5B0h]
  00000001407AE277: 90                 nop
  00000001407AE278: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001407AE27D: FF 15 B5 E2 65 01  call        qword ptr [0000000141E0C538h]
  00000001407AE283: 0F B6 C0           movzx       eax,al
  00000001407AE286: 85 C0              test        eax,eax
  00000001407AE288: 0F 85 87 04 00 00  jne         00000001407AE715
  00000001407AE28E: 48 8D 54 24 58     lea         rdx,[rsp+58h]
  00000001407AE293: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001407AE298: FF 15 0A E3 65 01  call        qword ptr [0000000141E0C5A8h]
  00000001407AE29E: 48 89 84 24 48 01  mov         qword ptr [rsp+0000000000000148h],rax
                    00 00
  00000001407AE2A6: 48 8B 84 24 48 01  mov         rax,qword ptr [rsp+0000000000000148h]
                    00 00
  00000001407AE2AE: 48 89 84 24 50 01  mov         qword ptr [rsp+0000000000000150h],rax
                    00 00
  00000001407AE2B6: 48 8D 15 83 46 F0  lea         rdx,[00000001426B2940h]
                    01
  00000001407AE2BD: 48 8B 8C 24 50 01  mov         rcx,qword ptr [rsp+0000000000000150h]
                    00 00
  00000001407AE2C5: FF 15 9D 61 65 01  call        qword ptr [0000000141E04468h]
  00000001407AE2CB: 88 44 24 50        mov         byte ptr [rsp+50h],al
  00000001407AE2CF: 48 8D 4C 24 58     lea         rcx,[rsp+58h]
  00000001407AE2D4: FF 15 06 77 65 01  call        qword ptr [0000000141E059E0h]
  00000001407AE2DA: 44 0F B6 5C 24 50  movzx       r11d,byte ptr [rsp+50h]
  00000001407AE2E0: 45 85 DB           test        r11d,r11d
  00000001407AE2E3: 74 48              je          00000001407AE32D
  00000001407AE2E5: 48 8D 44 24 60     lea         rax,[rsp+60h]
  00000001407AE2EA: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  00000001407AE2EF: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001407AE2F4: 48 8B 4C 24 68     mov         rcx,qword ptr [rsp+68h]
  00000001407AE2F9: FF 15 99 E3 65 01  call        qword ptr [0000000141E0C698h]
  00000001407AE2FF: 48 89 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],rax
                    00 00
  00000001407AE307: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE30F: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407AE312: 48 8B 94 24 58 01  mov         rdx,qword ptr [rsp+0000000000000158h]
                    00 00
  00000001407AE31A: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE322: FF 90 30 02 00 00  call        qword ptr [rax+0000000000000230h]
  00000001407AE328: E9 E8 03 00 00     jmp         00000001407AE715
  00000001407AE32D: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  00000001407AE332: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001407AE337: FF 15 6B E2 65 01  call        qword ptr [0000000141E0C5A8h]
  00000001407AE33D: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  00000001407AE345: 48 8B 84 24 60 01  mov         rax,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001407AE34D: 48 89 84 24 68 01  mov         qword ptr [rsp+0000000000000168h],rax
                    00 00
  00000001407AE355: 48 8D 15 EC 45 F0  lea         rdx,[00000001426B2948h]
                    01
  00000001407AE35C: 48 8B 8C 24 68 01  mov         rcx,qword ptr [rsp+0000000000000168h]
                    00 00
  00000001407AE364: FF 15 FE 60 65 01  call        qword ptr [0000000141E04468h]
  00000001407AE36A: 88 44 24 70        mov         byte ptr [rsp+70h],al
  00000001407AE36E: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  00000001407AE373: FF 15 67 76 65 01  call        qword ptr [0000000141E059E0h]
  00000001407AE379: 44 0F B6 5C 24 70  movzx       r11d,byte ptr [rsp+70h]
  00000001407AE37F: 45 85 DB           test        r11d,r11d
  00000001407AE382: 74 51              je          00000001407AE3D5
  00000001407AE384: 48 8D 84 24 80 00  lea         rax,[rsp+0000000000000080h]
                    00 00
  00000001407AE38C: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001407AE394: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001407AE399: 48 8B 8C 24 88 00  mov         rcx,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001407AE3A1: FF 15 F1 E2 65 01  call        qword ptr [0000000141E0C698h]
  00000001407AE3A7: 48 89 84 24 70 01  mov         qword ptr [rsp+0000000000000170h],rax
                    00 00
  00000001407AE3AF: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE3B7: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407AE3BA: 48 8B 94 24 70 01  mov         rdx,qword ptr [rsp+0000000000000170h]
                    00 00
  00000001407AE3C2: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE3CA: FF 90 40 02 00 00  call        qword ptr [rax+0000000000000240h]
  00000001407AE3D0: E9 40 03 00 00     jmp         00000001407AE715
  00000001407AE3D5: 48 8D 94 24 98 00  lea         rdx,[rsp+0000000000000098h]
                    00 00
  00000001407AE3DD: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001407AE3E2: FF 15 C0 E1 65 01  call        qword ptr [0000000141E0C5A8h]
  00000001407AE3E8: 48 89 84 24 78 01  mov         qword ptr [rsp+0000000000000178h],rax
                    00 00
  00000001407AE3F0: 48 8B 84 24 78 01  mov         rax,qword ptr [rsp+0000000000000178h]
                    00 00
  00000001407AE3F8: 48 89 84 24 80 01  mov         qword ptr [rsp+0000000000000180h],rax
                    00 00
  00000001407AE400: 48 8D 15 4D 45 F0  lea         rdx,[00000001426B2954h]
                    01
  00000001407AE407: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001407AE40F: FF 15 53 60 65 01  call        qword ptr [0000000141E04468h]
  00000001407AE415: 88 84 24 90 00 00  mov         byte ptr [rsp+0000000000000090h],al
                    00
  00000001407AE41C: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  00000001407AE424: FF 15 B6 75 65 01  call        qword ptr [0000000141E059E0h]
  00000001407AE42A: 44 0F B6 9C 24 90  movzx       r11d,byte ptr [rsp+0000000000000090h]
                    00 00 00
  00000001407AE433: 45 85 DB           test        r11d,r11d
  00000001407AE436: 0F 84 D9 02 00 00  je          00000001407AE715
  00000001407AE43C: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE444: E8 F7 99 C5 00     call        0000000141407E40
  00000001407AE449: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001407AE44E: 48 83 7C 24 30 00  cmp         qword ptr [rsp+30h],0
  00000001407AE454: 0F 84 BB 02 00 00  je          00000001407AE715
  00000001407AE45A: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001407AE45F: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407AE464: E8 07 01 B6 FF     call        000000014030E570
  00000001407AE469: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407AE46E: E8 3D 16 B7 FF     call        000000014031FAB0
  00000001407AE473: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001407AE478: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001407AE47E: 0F 84 91 02 00 00  je          00000001407AE715
  00000001407AE484: 48 8D 84 24 A0 00  lea         rax,[rsp+00000000000000A0h]
                    00 00
  00000001407AE48C: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  00000001407AE494: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001407AE499: E8 C2 A2 C2 FF     call        00000001403D8760
  00000001407AE49E: 48 8B D0           mov         rdx,rax
  00000001407AE4A1: 48 8B 8C 24 B8 00  mov         rcx,qword ptr [rsp+00000000000000B8h]
                    00 00
  00000001407AE4A9: E8 62 0C 00 00     call        00000001407AF110
  00000001407AE4AE: 48 89 84 24 88 01  mov         qword ptr [rsp+0000000000000188h],rax
                    00 00
  00000001407AE4B6: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE4BE: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407AE4C1: 48 8B 94 24 88 01  mov         rdx,qword ptr [rsp+0000000000000188h]
                    00 00
  00000001407AE4C9: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE4D1: FF 90 B0 01 00 00  call        qword ptr [rax+00000000000001B0h]
  00000001407AE4D7: 48 8D 84 24 C0 00  lea         rax,[rsp+00000000000000C0h]
                    00 00
  00000001407AE4DF: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  00000001407AE4E7: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001407AE4EC: E8 2F A2 C2 FF     call        00000001403D8720
  00000001407AE4F1: 48 8B D0           mov         rdx,rax
  00000001407AE4F4: 48 8B 8C 24 F0 00  mov         rcx,qword ptr [rsp+00000000000000F0h]
                    00 00
  00000001407AE4FC: E8 FF D1 FF FF     call        00000001407AB700
  00000001407AE501: 48 89 84 24 90 01  mov         qword ptr [rsp+0000000000000190h],rax
                    00 00
  00000001407AE509: 48 8B 84 24 F0 01  mov         rax,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE511: 48 8B 00           mov         rax,qword ptr [rax]
  00000001407AE514: 48 8B 94 24 90 01  mov         rdx,qword ptr [rsp+0000000000000190h]
                    00 00
  00000001407AE51C: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE524: FF 90 A8 01 00 00  call        qword ptr [rax+00000000000001A8h]
  00000001407AE52A: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  00000001407AE52F: E8 3C 12 B6 FF     call        000000014030F770
  00000001407AE534: 48 8B D0           mov         rdx,rax
  00000001407AE537: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001407AE53C: FF 15 96 74 65 01  call        qword ptr [0000000141E059D8h]
  00000001407AE542: 90                 nop
  00000001407AE543: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001407AE548: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE550: E8 AB AB C5 00     call        0000000141409100
  00000001407AE555: 90                 nop
  00000001407AE556: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001407AE55B: FF 15 67 5A 65 01  call        qword ptr [0000000141E03FC8h]
  00000001407AE561: 0F B6 C0           movzx       eax,al
  00000001407AE564: 85 C0              test        eax,eax
  00000001407AE566: 0F 85 A2 00 00 00  jne         00000001407AE60E
  00000001407AE56C: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001407AE571: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  00000001407AE579: FF 15 39 5B 65 01  call        qword ptr [0000000141E040B8h]
  00000001407AE57F: 48 89 84 24 98 01  mov         qword ptr [rsp+0000000000000198h],rax
                    00 00
  00000001407AE587: 48 8B 84 24 98 01  mov         rax,qword ptr [rsp+0000000000000198h]
                    00 00
  00000001407AE58F: 48 89 84 24 A0 01  mov         qword ptr [rsp+00000000000001A0h],rax
                    00 00
  00000001407AE597: 8B 84 24 38 01 00  mov         eax,dword ptr [rsp+0000000000000138h]
                    00
  00000001407AE59E: 83 C8 01           or          eax,1
  00000001407AE5A1: 89 84 24 38 01 00  mov         dword ptr [rsp+0000000000000138h],eax
                    00
  00000001407AE5A8: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000001407AE5AD: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  00000001407AE5B5: FF 15 FD 5A 65 01  call        qword ptr [0000000141E040B8h]
  00000001407AE5BB: 48 89 84 24 A8 01  mov         qword ptr [rsp+00000000000001A8h],rax
                    00 00
  00000001407AE5C3: 48 8B 84 24 A8 01  mov         rax,qword ptr [rsp+00000000000001A8h]
                    00 00
  00000001407AE5CB: 48 89 84 24 B0 01  mov         qword ptr [rsp+00000000000001B0h],rax
                    00 00
  00000001407AE5D3: 8B 84 24 38 01 00  mov         eax,dword ptr [rsp+0000000000000138h]
                    00
  00000001407AE5DA: 83 C8 02           or          eax,2
  00000001407AE5DD: 89 84 24 38 01 00  mov         dword ptr [rsp+0000000000000138h],eax
                    00
  00000001407AE5E4: 48 8B 94 24 A0 01  mov         rdx,qword ptr [rsp+00000000000001A0h]
                    00 00
  00000001407AE5EC: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000001407AE5F4: FF 15 D6 5A 65 01  call        qword ptr [0000000141E040D0h]
  00000001407AE5FA: 0F B6 C0           movzx       eax,al
  00000001407AE5FD: 85 C0              test        eax,eax
  00000001407AE5FF: 74 0D              je          00000001407AE60E
  00000001407AE601: C7 84 24 B8 01 00  mov         dword ptr [rsp+00000000000001B8h],1
                    00 01 00 00 00
  00000001407AE60C: EB 0B              jmp         00000001407AE619
  00000001407AE60E: C7 84 24 B8 01 00  mov         dword ptr [rsp+00000000000001B8h],0
                    00 00 00 00 00
  00000001407AE619: 0F B6 84 24 B8 01  movzx       eax,byte ptr [rsp+00000000000001B8h]
                    00 00
  00000001407AE621: 88 84 24 F8 00 00  mov         byte ptr [rsp+00000000000000F8h],al
                    00
  00000001407AE628: 8B 84 24 38 01 00  mov         eax,dword ptr [rsp+0000000000000138h]
                    00
  00000001407AE62F: 83 E0 02           and         eax,2
  00000001407AE632: 85 C0              test        eax,eax
  00000001407AE634: 74 17              je          00000001407AE64D
  00000001407AE636: 83 A4 24 38 01 00  and         dword ptr [rsp+0000000000000138h],0FFFFFFFDh
                    00 FD
  00000001407AE63E: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  00000001407AE646: FF 15 94 73 65 01  call        qword ptr [0000000141E059E0h]
  00000001407AE64C: 90                 nop
  00000001407AE64D: 8B 84 24 38 01 00  mov         eax,dword ptr [rsp+0000000000000138h]
                    00
  00000001407AE654: 83 E0 01           and         eax,1
  00000001407AE657: 85 C0              test        eax,eax
  00000001407AE659: 74 16              je          00000001407AE671
  00000001407AE65B: 83 A4 24 38 01 00  and         dword ptr [rsp+0000000000000138h],0FFFFFFFEh
                    00 FE
  00000001407AE663: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  00000001407AE66B: FF 15 6F 73 65 01  call        qword ptr [0000000141E059E0h]
  00000001407AE671: 0F B6 84 24 F8 00  movzx       eax,byte ptr [rsp+00000000000000F8h]
                    00 00
  00000001407AE679: 85 C0              test        eax,eax
  00000001407AE67B: 74 40              je          00000001407AE6BD
  00000001407AE67D: 48 8D 84 24 10 01  lea         rax,[rsp+0000000000000110h]
                    00 00
  00000001407AE685: 48 89 84 24 18 01  mov         qword ptr [rsp+0000000000000118h],rax
                    00 00
  00000001407AE68D: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000001407AE692: 48 8B 8C 24 18 01  mov         rcx,qword ptr [rsp+0000000000000118h]
                    00 00
  00000001407AE69A: FF 15 38 73 65 01  call        qword ptr [0000000141E059D8h]
  00000001407AE6A0: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  00000001407AE6A8: 48 8B 94 24 C0 01  mov         rdx,qword ptr [rsp+00000000000001C0h]
                    00 00
  00000001407AE6B0: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE6B8: E8 F3 A9 C5 00     call        00000001414090B0
  00000001407AE6BD: 48 8D 84 24 20 01  lea         rax,[rsp+0000000000000120h]
                    00 00
  00000001407AE6C5: 48 89 84 24 28 01  mov         qword ptr [rsp+0000000000000128h],rax
                    00 00
  00000001407AE6CD: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000001407AE6D2: 48 8B 8C 24 28 01  mov         rcx,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001407AE6DA: FF 15 F8 72 65 01  call        qword ptr [0000000141E059D8h]
  00000001407AE6E0: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  00000001407AE6E8: 48 8B 94 24 C8 01  mov         rdx,qword ptr [rsp+00000000000001C8h]
                    00 00
  00000001407AE6F0: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001407AE6F8: E8 C3 F7 FF FF     call        00000001407ADEC0
  00000001407AE6FD: 90                 nop
  00000001407AE6FE: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  00000001407AE703: FF 15 D7 72 65 01  call        qword ptr [0000000141E059E0h]
  00000001407AE709: 90                 nop
  00000001407AE70A: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  00000001407AE70F: FF 15 CB 72 65 01  call        qword ptr [0000000141E059E0h]
  00000001407AE715: 48 8D 94 24 30 01  lea         rdx,[rsp+0000000000000130h]
                    00 00
  00000001407AE71D: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407AE722: FF 15 70 DE 65 01  call        qword ptr [0000000141E0C598h]
  00000001407AE728: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
  00000001407AE730: 48 8B 84 24 D0 01  mov         rax,qword ptr [rsp+00000000000001D0h]
                    00 00
  00000001407AE738: 48 89 84 24 D8 01  mov         qword ptr [rsp+00000000000001D8h],rax
                    00 00
  00000001407AE740: 48 8B 94 24 D8 01  mov         rdx,qword ptr [rsp+00000000000001D8h]
                    00 00
  00000001407AE748: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407AE74D: FF 15 0D DE 65 01  call        qword ptr [0000000141E0C560h]
  00000001407AE753: 90                 nop
  00000001407AE754: 48 8D 8C 24 30 01  lea         rcx,[rsp+0000000000000130h]
                    00 00
  00000001407AE75C: FF 15 46 DF 65 01  call        qword ptr [0000000141E0C6A8h]
  00000001407AE762: 90                 nop
  00000001407AE763: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000001407AE768: FF 15 42 DF 65 01  call        qword ptr [0000000141E0C6B0h]
  00000001407AE76E: E9 DE FA FF FF     jmp         00000001407AE251
  00000001407AE773: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  00000001407AE778: FF 15 2A DF 65 01  call        qword ptr [0000000141E0C6A8h]
  00000001407AE77E: 90                 nop
  00000001407AE77F: 48 8B 8C 24 F8 01  mov         rcx,qword ptr [rsp+00000000000001F8h]
                    00 00
  00000001407AE787: FF 15 23 DF 65 01  call        qword ptr [0000000141E0C6B0h]
  00000001407AE78D: 48 81 C4 E8 01 00  add         rsp,1E8h
                    00
  00000001407AE794: C3                 ret

  Summary

     1E02000 .text

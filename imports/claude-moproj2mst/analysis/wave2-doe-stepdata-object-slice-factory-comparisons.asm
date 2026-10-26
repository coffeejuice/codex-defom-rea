
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001411FE3F0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001411FE3F5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001411FE3FA: 56                 push        rsi
  00000001411FE3FB: 57                 push        rdi
  00000001411FE3FC: 48 81 EC 08 01 00  sub         rsp,108h
                    00
  00000001411FE403: 48 C7 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001411FE40F: C6 44 24 50 01     mov         byte ptr [rsp+50h],1
  00000001411FE414: C6 44 24 70 01     mov         byte ptr [rsp+70h],1
  00000001411FE419: C6 44 24 40 00     mov         byte ptr [rsp+40h],0
  00000001411FE41E: 48 C7 44 24 48 00  mov         qword ptr [rsp+48h],0
                    00 00 00
  00000001411FE427: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE42F: F2 0F 10 00        movsd       xmm0,mmword ptr [rax]
  00000001411FE433: 66 0F 2E 05 95 ED  ucomisd     xmm0,mmword ptr [00000001421AD1D0h]
                    FA 00
  00000001411FE43B: 7A 02              jp          00000001411FE43F
  00000001411FE43D: 74 40              je          00000001411FE47F
  00000001411FE43F: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE447: F2 0F 10 40 18     movsd       xmm0,mmword ptr [rax+18h]
  00000001411FE44C: 66 0F 57 05 DC 58  xorpd       xmm0,xmmword ptr [00000001421D3D30h]
                    FD 00
  00000001411FE454: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE45C: F2 0F 5E 00        divsd       xmm0,mmword ptr [rax]
  00000001411FE460: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411FE466: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411FE46A: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  00000001411FE470: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411FE474: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  00000001411FE47A: E9 CC 00 00 00     jmp         00000001411FE54B
  00000001411FE47F: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE487: F2 0F 10 40 08     movsd       xmm0,mmword ptr [rax+8]
  00000001411FE48C: 66 0F 2E 05 3C ED  ucomisd     xmm0,mmword ptr [00000001421AD1D0h]
                    FA 00
  00000001411FE494: 7A 02              jp          00000001411FE498
  00000001411FE496: 74 3E              je          00000001411FE4D6
  00000001411FE498: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411FE49C: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411FE4A2: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE4AA: F2 0F 10 40 18     movsd       xmm0,mmword ptr [rax+18h]
  00000001411FE4AF: 66 0F 57 05 79 58  xorpd       xmm0,xmmword ptr [00000001421D3D30h]
                    FD 00
  00000001411FE4B7: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE4BF: F2 0F 5E 40 08     divsd       xmm0,mmword ptr [rax+8]
  00000001411FE4C4: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  00000001411FE4CA: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411FE4CE: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  00000001411FE4D4: EB 75              jmp         00000001411FE54B
  00000001411FE4D6: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE4DE: F2 0F 10 40 10     movsd       xmm0,mmword ptr [rax+10h]
  00000001411FE4E3: 66 0F 2E 05 E5 EC  ucomisd     xmm0,mmword ptr [00000001421AD1D0h]
                    FA 00
  00000001411FE4EB: 7A 02              jp          00000001411FE4EF
  00000001411FE4ED: 74 3E              je          00000001411FE52D
  00000001411FE4EF: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411FE4F3: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411FE4F9: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411FE4FD: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  00000001411FE503: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE50B: F2 0F 10 40 18     movsd       xmm0,mmword ptr [rax+18h]
  00000001411FE510: 66 0F 57 05 18 58  xorpd       xmm0,xmmword ptr [00000001421D3D30h]
                    FD 00
  00000001411FE518: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE520: F2 0F 5E 40 10     divsd       xmm0,mmword ptr [rax+10h]
  00000001411FE525: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  00000001411FE52B: EB 1E              jmp         00000001411FE54B
  00000001411FE52D: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411FE531: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001411FE537: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411FE53B: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  00000001411FE541: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001411FE545: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  00000001411FE54B: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE553: F2 0F 10 00        movsd       xmm0,mmword ptr [rax]
  00000001411FE557: F2 0F 11 44 24 58  movsd       mmword ptr [rsp+58h],xmm0
  00000001411FE55D: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE565: F2 0F 10 40 08     movsd       xmm0,mmword ptr [rax+8]
  00000001411FE56A: F2 0F 11 44 24 60  movsd       mmword ptr [rsp+60h],xmm0
  00000001411FE570: 48 8B 84 24 28 01  mov         rax,qword ptr [rsp+0000000000000128h]
                    00 00
  00000001411FE578: F2 0F 10 40 10     movsd       xmm0,mmword ptr [rax+10h]
  00000001411FE57D: F2 0F 11 44 24 68  movsd       mmword ptr [rsp+68h],xmm0
  00000001411FE583: 48 8B 8C 24 20 01  mov         rcx,qword ptr [rsp+0000000000000120h]
                    00 00
  00000001411FE58B: E8 F0 33 FF FF     call        00000001411F1980
  00000001411FE590: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001411FE595: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001411FE59B: 0F 84 68 02 00 00  je          00000001411FE809
  00000001411FE5A1: 48 C7 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],0
                    00 00 00 00 00 00
  00000001411FE5AD: 48 C7 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],0
                    00 00 00 00 00 00
  00000001411FE5B9: C6 84 24 80 00 00  mov         byte ptr [rsp+0000000000000080h],0
                    00 00
  00000001411FE5C1: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001411FE5C6: E8 65 C6 A7 FF     call        0000000140C7AC30
  00000001411FE5CB: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000001411FE5D0: 48 83 7C 24 78 00  cmp         qword ptr [rsp+78h],0
  00000001411FE5D6: 0F 84 2D 02 00 00  je          00000001411FE809
  00000001411FE5DC: C7 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],0
                    00 00 00 00 00
  00000001411FE5E7: EB 11              jmp         00000001411FE5FA
  00000001411FE5E9: 8B 84 24 98 00 00  mov         eax,dword ptr [rsp+0000000000000098h]
                    00
  00000001411FE5F0: 83 C0 01           add         eax,1
  00000001411FE5F3: 89 84 24 98 00 00  mov         dword ptr [rsp+0000000000000098h],eax
                    00
  00000001411FE5FA: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001411FE5FF: 48 83 C1 18        add         rcx,18h
  00000001411FE603: E8 D8 64 E0 FE     call        0000000140004AE0
  00000001411FE608: 39 84 24 98 00 00  cmp         dword ptr [rsp+0000000000000098h],eax
                    00
  00000001411FE60F: 0F 8D E0 00 00 00  jge         00000001411FE6F5
  00000001411FE615: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001411FE61A: 48 83 C1 18        add         rcx,18h
  00000001411FE61E: 8B 94 24 98 00 00  mov         edx,dword ptr [rsp+0000000000000098h]
                    00
  00000001411FE625: E8 E6 25 F6 FE     call        0000000140160C10
  00000001411FE62A: 48 8B 00           mov         rax,qword ptr [rax]
  00000001411FE62D: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001411FE632: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411FE637: E8 F4 60 10 FF     call        0000000140304730
  00000001411FE63C: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  00000001411FE644: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001411FE64C: F2 0F 10 00        movsd       xmm0,mmword ptr [rax]
  00000001411FE650: 66 0F 2E 44 24 20  ucomisd     xmm0,mmword ptr [rsp+20h]
  00000001411FE656: 0F 8A 94 00 00 00  jp          00000001411FE6F0
  00000001411FE65C: 0F 85 8E 00 00 00  jne         00000001411FE6F0
  00000001411FE662: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001411FE66A: F2 0F 10 40 08     movsd       xmm0,mmword ptr [rax+8]
  00000001411FE66F: 66 0F 2E 44 24 28  ucomisd     xmm0,mmword ptr [rsp+28h]
  00000001411FE675: 7A 79              jp          00000001411FE6F0
  00000001411FE677: 75 77              jne         00000001411FE6F0
  00000001411FE679: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001411FE681: F2 0F 10 40 10     movsd       xmm0,mmword ptr [rax+10h]
  00000001411FE686: 66 0F 2E 44 24 30  ucomisd     xmm0,mmword ptr [rsp+30h]
  00000001411FE68C: 7A 62              jp          00000001411FE6F0
  00000001411FE68E: 75 60              jne         00000001411FE6F0
  00000001411FE690: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411FE695: E8 B6 71 F6 FF     call        0000000141165850
  00000001411FE69A: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  00000001411FE6A2: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411FE6AA: F2 0F 10 00        movsd       xmm0,mmword ptr [rax]
  00000001411FE6AE: 66 0F 2E 44 24 58  ucomisd     xmm0,mmword ptr [rsp+58h]
  00000001411FE6B4: 7A 3A              jp          00000001411FE6F0
  00000001411FE6B6: 75 38              jne         00000001411FE6F0
  00000001411FE6B8: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411FE6C0: F2 0F 10 40 08     movsd       xmm0,mmword ptr [rax+8]
  00000001411FE6C5: 66 0F 2E 44 24 60  ucomisd     xmm0,mmword ptr [rsp+60h]
  00000001411FE6CB: 7A 23              jp          00000001411FE6F0
  00000001411FE6CD: 75 21              jne         00000001411FE6F0
  00000001411FE6CF: 48 8B 84 24 90 00  mov         rax,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001411FE6D7: F2 0F 10 40 10     movsd       xmm0,mmword ptr [rax+10h]
  00000001411FE6DC: 66 0F 2E 44 24 68  ucomisd     xmm0,mmword ptr [rsp+68h]
  00000001411FE6E2: 7A 0C              jp          00000001411FE6F0
  00000001411FE6E4: 75 0A              jne         00000001411FE6F0
  00000001411FE6E6: C6 84 24 80 00 00  mov         byte ptr [rsp+0000000000000080h],1
                    00 01
  00000001411FE6EE: EB 05              jmp         00000001411FE6F5
  00000001411FE6F0: E9 F4 FE FF FF     jmp         00000001411FE5E9
  00000001411FE6F5: 0F B6 84 24 80 00  movzx       eax,byte ptr [rsp+0000000000000080h]
                    00 00
  00000001411FE6FD: 85 C0              test        eax,eax
  00000001411FE6FF: 0F 85 04 01 00 00  jne         00000001411FE809
  00000001411FE705: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000001411FE70A: 8B 48 10           mov         ecx,dword ptr [rax+10h]
  00000001411FE70D: 83 C1 01           add         ecx,1
  00000001411FE710: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000001411FE715: 89 48 10           mov         dword ptr [rax+10h],ecx
  00000001411FE718: B9 A8 00 00 00     mov         ecx,0A8h
  00000001411FE71D: E8 9A 03 27 00     call        000000014146EABC
  00000001411FE722: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  00000001411FE72A: 48 83 BC 24 A8 00  cmp         qword ptr [rsp+00000000000000A8h],0
                    00 00 00
  00000001411FE733: 74 1F              je          00000001411FE754
  00000001411FE735: 48 8B 44 24 78     mov         rax,qword ptr [rsp+78h]
  00000001411FE73A: 8B 50 10           mov         edx,dword ptr [rax+10h]
  00000001411FE73D: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001411FE745: E8 26 10 FB FE     call        00000001401AF770
  00000001411FE74A: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  00000001411FE752: EB 0C              jmp         00000001411FE760
  00000001411FE754: 48 C7 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],0
                    00 00 00 00 00 00
  00000001411FE760: 48 8B 84 24 F8 00  mov         rax,qword ptr [rsp+00000000000000F8h]
                    00 00
  00000001411FE768: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  00000001411FE770: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001411FE778: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  00000001411FE77D: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  00000001411FE782: 48 83 C1 18        add         rcx,18h
  00000001411FE786: 48 8D 54 24 48     lea         rdx,[rsp+48h]
  00000001411FE78B: E8 50 F0 52 00     call        000000014172D7E0
  00000001411FE790: 48 8D BC 24 B0 00  lea         rdi,[rsp+00000000000000B0h]
                    00 00
  00000001411FE798: 48 8D 74 24 20     lea         rsi,[rsp+20h]
  00000001411FE79D: B9 18 00 00 00     mov         ecx,18h
  00000001411FE7A2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001411FE7A4: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000001411FE7AC: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411FE7B1: E8 2A 3E FB FE     call        00000001401B25E0
  00000001411FE7B6: 48 8D BC 24 D0 00  lea         rdi,[rsp+00000000000000D0h]
                    00 00
  00000001411FE7BE: 48 8D 74 24 58     lea         rsi,[rsp+58h]
  00000001411FE7C3: B9 18 00 00 00     mov         ecx,18h
  00000001411FE7C8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001411FE7CA: 48 8D 94 24 D0 00  lea         rdx,[rsp+00000000000000D0h]
                    00 00
  00000001411FE7D2: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411FE7D7: E8 94 3D FB FE     call        00000001401B2570
  00000001411FE7DC: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  00000001411FE7E1: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411FE7E6: E8 C5 3C FB FE     call        00000001401B24B0
  00000001411FE7EB: 48 8D 54 24 70     lea         rdx,[rsp+70h]
  00000001411FE7F0: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411FE7F5: E8 D6 3C FB FE     call        00000001401B24D0
  00000001411FE7FA: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001411FE7FF: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  00000001411FE804: E8 F7 5F 10 FF     call        0000000140304800
  00000001411FE809: 48 8B 44 24 48     mov         rax,qword ptr [rsp+48h]
  00000001411FE80E: 48 81 C4 08 01 00  add         rsp,108h
                    00
  00000001411FE815: 5F                 pop         rdi
  00000001411FE816: 5E                 pop         rsi
  00000001411FE817: C3                 ret

  Summary

     1E02000 .text

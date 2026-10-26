
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001401B24B0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001401B24B5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401B24BA: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  00000001401B24BF: 48 8B 44 24 10     mov         rax,qword ptr [rsp+10h]
  00000001401B24C4: 0F B6 00           movzx       eax,byte ptr [rax]
  00000001401B24C7: 88 41 0C           mov         byte ptr [rcx+0Ch],al
  00000001401B24CA: C3                 ret
  00000001401B24CB: CC                 int         3
  00000001401B24CC: CC                 int         3
  00000001401B24CD: CC                 int         3
  00000001401B24CE: CC                 int         3
  00000001401B24CF: CC                 int         3
  00000001401B24D0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001401B24D5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401B24DA: 48 8B 4C 24 08     mov         rcx,qword ptr [rsp+8]
  00000001401B24DF: 48 8B 44 24 10     mov         rax,qword ptr [rsp+10h]
  00000001401B24E4: 0F B6 00           movzx       eax,byte ptr [rax]
  00000001401B24E7: 88 41 0D           mov         byte ptr [rcx+0Dh],al
  00000001401B24EA: C3                 ret
  00000001401B24EB: CC                 int         3
  00000001401B24EC: CC                 int         3
  00000001401B24ED: CC                 int         3
  00000001401B24EE: CC                 int         3
  00000001401B24EF: CC                 int         3
  00000001401B24F0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001401B24F5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401B24FA: 56                 push        rsi
  00000001401B24FB: 57                 push        rdi
  00000001401B24FC: 48 83 EC 68        sub         rsp,68h
  00000001401B2500: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001401B2508: F2 0F 10 00        movsd       xmm0,mmword ptr [rax]
  00000001401B250C: F2 0F 11 44 24 20  movsd       mmword ptr [rsp+20h],xmm0
  00000001401B2512: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001401B251A: F2 0F 10 40 08     movsd       xmm0,mmword ptr [rax+8]
  00000001401B251F: F2 0F 11 44 24 28  movsd       mmword ptr [rsp+28h],xmm0
  00000001401B2525: 48 8B 84 24 88 00  mov         rax,qword ptr [rsp+0000000000000088h]
                    00 00
  00000001401B252D: F2 0F 10 40 10     movsd       xmm0,mmword ptr [rax+10h]
  00000001401B2532: F2 0F 11 44 24 30  movsd       mmword ptr [rsp+30h],xmm0
  00000001401B2538: 48 8D 7C 24 40     lea         rdi,[rsp+40h]
  00000001401B253D: 48 8D 74 24 20     lea         rsi,[rsp+20h]
  00000001401B2542: B9 18 00 00 00     mov         ecx,18h
  00000001401B2547: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401B2549: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001401B254E: 48 8B 8C 24 80 00  mov         rcx,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001401B2556: E8 15 00 00 00     call        00000001401B2570
  00000001401B255B: 48 83 C4 68        add         rsp,68h
  00000001401B255F: 5F                 pop         rdi
  00000001401B2560: 5E                 pop         rsi
  00000001401B2561: C3                 ret
  00000001401B2562: CC                 int         3
  00000001401B2563: CC                 int         3
  00000001401B2564: CC                 int         3
  00000001401B2565: CC                 int         3
  00000001401B2566: CC                 int         3
  00000001401B2567: CC                 int         3
  00000001401B2568: CC                 int         3
  00000001401B2569: CC                 int         3
  00000001401B256A: CC                 int         3
  00000001401B256B: CC                 int         3
  00000001401B256C: CC                 int         3
  00000001401B256D: CC                 int         3
  00000001401B256E: CC                 int         3
  00000001401B256F: CC                 int         3
  00000001401B2570: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001401B2575: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401B257A: 56                 push        rsi
  00000001401B257B: 57                 push        rdi
  00000001401B257C: 48 83 EC 78        sub         rsp,78h
  00000001401B2580: 48 8B 94 24 98 00  mov         rdx,qword ptr [rsp+0000000000000098h]
                    00 00
  00000001401B2588: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000001401B258D: E8 9E 84 78 00     call        000000014093AA30
  00000001401B2592: 48 8D 7C 24 50     lea         rdi,[rsp+50h]
  00000001401B2597: 48 8B F0           mov         rsi,rax
  00000001401B259A: B9 18 00 00 00     mov         ecx,18h
  00000001401B259F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401B25A1: 48 8D 7C 24 20     lea         rdi,[rsp+20h]
  00000001401B25A6: 48 8D 74 24 50     lea         rsi,[rsp+50h]
  00000001401B25AB: B9 18 00 00 00     mov         ecx,18h
  00000001401B25B0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401B25B2: 48 8B BC 24 90 00  mov         rdi,qword ptr [rsp+0000000000000090h]
                    00 00
  00000001401B25BA: 48 8D 74 24 20     lea         rsi,[rsp+20h]
  00000001401B25BF: 48 83 C7 18        add         rdi,18h
  00000001401B25C3: B9 18 00 00 00     mov         ecx,18h
  00000001401B25C8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401B25CA: 48 83 C4 78        add         rsp,78h
  00000001401B25CE: 5F                 pop         rdi
  00000001401B25CF: 5E                 pop         rsi
  00000001401B25D0: C3                 ret
  00000001401B25D1: CC                 int         3
  00000001401B25D2: CC                 int         3
  00000001401B25D3: CC                 int         3
  00000001401B25D4: CC                 int         3
  00000001401B25D5: CC                 int         3
  00000001401B25D6: CC                 int         3
  00000001401B25D7: CC                 int         3
  00000001401B25D8: CC                 int         3
  00000001401B25D9: CC                 int         3
  00000001401B25DA: CC                 int         3
  00000001401B25DB: CC                 int         3
  00000001401B25DC: CC                 int         3
  00000001401B25DD: CC                 int         3
  00000001401B25DE: CC                 int         3
  00000001401B25DF: CC                 int         3
  00000001401B25E0: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001401B25E5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001401B25EA: 56                 push        rsi
  00000001401B25EB: 57                 push        rdi
  00000001401B25EC: 48 83 EC 28        sub         rsp,28h
  00000001401B25F0: 48 8D 3C 24        lea         rdi,[rsp]
  00000001401B25F4: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  00000001401B25F9: B9 18 00 00 00     mov         ecx,18h
  00000001401B25FE: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401B2600: 48 8B 7C 24 40     mov         rdi,qword ptr [rsp+40h]
  00000001401B2605: 48 8D 34 24        lea         rsi,[rsp]
  00000001401B2609: 48 83 C7 30        add         rdi,30h
  00000001401B260D: B9 18 00 00 00     mov         ecx,18h
  00000001401B2612: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401B2614: 48 83 C4 28        add         rsp,28h
  00000001401B2618: 5F                 pop         rdi
  00000001401B2619: 5E                 pop         rsi
  00000001401B261A: C3                 ret

  Summary

     1E02000 .text

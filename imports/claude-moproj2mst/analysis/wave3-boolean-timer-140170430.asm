Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140170430: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140170435: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014017043A: 56                 push        rsi
  000000014017043B: 57                 push        rdi
  000000014017043C: 48 81 EC 88 00 00  sub         rsp,88h
                    00
  0000000140170443: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017044B: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  000000014017044F: C6 80 9E 01 00 00  mov         byte ptr [rax+000000000000019Eh],1
                    01
  0000000140170456: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017045E: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140170462: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  0000000140170467: 0F 84 D6 00 00 00  je          0000000140170543
  000000014017046D: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170475: FF 15 ED B0 C9 01  call        qword ptr [0000000141E0B568h]
  000000014017047B: 48 8B C8           mov         rcx,rax
  000000014017047E: FF 15 7C B5 C9 01  call        qword ptr [0000000141E0BA00h]
  0000000140170484: 48 8D 7C 24 60     lea         rdi,[rsp+60h]
  0000000140170489: 48 8B F0           mov         rsi,rax
  000000014017048C: B9 10 00 00 00     mov         ecx,10h
  0000000140170491: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140170493: 48 8D 7C 24 30     lea         rdi,[rsp+30h]
  0000000140170498: 48 8D 74 24 60     lea         rsi,[rsp+60h]
  000000014017049D: B9 10 00 00 00     mov         ecx,10h
  00000001401704A2: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401704A4: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000001401704A9: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  00000001401704AE: FF 15 7C 3A C9 01  call        qword ptr [0000000141E03F30h]
  00000001401704B4: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  00000001401704B9: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000001401704BE: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401704C6: FF 15 A4 B0 C9 01  call        qword ptr [0000000141E0B570h]
  00000001401704CC: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401704D4: 48 8B 52 28        mov         rdx,qword ptr [rdx+28h]
  00000001401704D8: 48 8D 7C 24 50     lea         rdi,[rsp+50h]
  00000001401704DD: 48 8D 74 24 40     lea         rsi,[rsp+40h]
  00000001401704E2: B9 10 00 00 00     mov         ecx,10h
  00000001401704E7: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401704E9: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401704F1: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401704F5: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  00000001401704F9: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170501: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140170505: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140170509: 48 8B 00           mov         rax,qword ptr [rax]
  000000014017050C: 44 8B 42 54        mov         r8d,dword ptr [rdx+54h]
  0000000140170510: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  0000000140170515: FF 90 10 01 00 00  call        qword ptr [rax+0000000000000110h]
  000000014017051B: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170523: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140170527: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  000000014017052B: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170533: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140170537: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  000000014017053B: 48 8B 00           mov         rax,qword ptr [rax]
  000000014017053E: 33 D2              xor         edx,edx
  0000000140170540: FF 50 08           call        qword ptr [rax+8]
  0000000140170543: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017054B: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014017054F: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140170557: 48 89 41 18        mov         qword ptr [rcx+18h],rax
  000000014017055B: 48 83 BC 24 A8 00  cmp         qword ptr [rsp+00000000000000A8h],0
                    00 00 00
  0000000140170564: 74 1E              je          0000000140170584
  0000000140170566: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014017056E: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140170571: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170579: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140170581: FF 50 08           call        qword ptr [rax+8]
  0000000140170584: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017058C: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140170590: 48 83 C1 40        add         rcx,40h
  0000000140170594: 33 D2              xor         edx,edx
  0000000140170596: E8 85 C3 B4 00     call        0000000140CBC920
  000000014017059B: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401705A3: 48 8B 00           mov         rax,qword ptr [rax]
  00000001401705A6: 33 D2              xor         edx,edx
  00000001401705A8: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401705B0: FF 90 B8 02 00 00  call        qword ptr [rax+00000000000002B8h]
  00000001401705B6: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401705BE: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001401705C2: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001401705CA: 48 89 41 18        mov         qword ptr [rcx+18h],rax
  00000001401705CE: 48 83 BC 24 A8 00  cmp         qword ptr [rsp+00000000000000A8h],0
                    00 00 00
  00000001401705D7: 74 1E              je          00000001401705F7
  00000001401705D9: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001401705E1: 48 8B 00           mov         rax,qword ptr [rax]
  00000001401705E4: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401705EC: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000001401705F4: FF 50 08           call        qword ptr [rax+8]
  00000001401705F7: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401705FF: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140170602: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017060A: FF 90 C8 01 00 00  call        qword ptr [rax+00000000000001C8h]
  0000000140170610: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170618: 48 8B 00           mov         rax,qword ptr [rax]
  000000014017061B: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170623: FF 90 48 05 00 00  call        qword ptr [rax+0000000000000548h]
  0000000140170629: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170631: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140170635: C6 80 9E 01 00 00  mov         byte ptr [rax+000000000000019Eh],0
                    00
  000000014017063C: 48 C7 44 24 20 00  mov         qword ptr [rsp+20h],0
                    00 00 00
  0000000140170645: 48 83 BC 24 A8 00  cmp         qword ptr [rsp+00000000000000A8h],0
                    00 00 00
  000000014017064E: 0F 84 42 01 00 00  je          0000000140170796
  0000000140170654: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  000000014017065C: 48 8B 00           mov         rax,qword ptr [rax]
  000000014017065F: 48 8B 8C 24 A8 00  mov         rcx,qword ptr [rsp+00000000000000A8h]
                    00 00
  0000000140170667: FF 90 00 01 00 00  call        qword ptr [rax+0000000000000100h]
  000000014017066D: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140170672: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  0000000140170678: 0F 84 16 01 00 00  je          0000000140170794
  000000014017067E: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170686: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140170689: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  000000014017068E: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170696: FF 90 D8 01 00 00  call        qword ptr [rax+00000000000001D8h]
  000000014017069C: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401706A4: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001401706A8: 48 83 C1 40        add         rcx,40h
  00000001401706AC: 48 8B D0           mov         rdx,rax
  00000001401706AF: E8 6C C2 B4 00     call        0000000140CBC920
  00000001401706B4: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401706BC: 48 8B 00           mov         rax,qword ptr [rax]
  00000001401706BF: 48 8B 54 24 20     mov         rdx,qword ptr [rsp+20h]
  00000001401706C4: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401706CC: FF 90 F0 01 00 00  call        qword ptr [rax+00000000000001F0h]
  00000001401706D2: 48 8B D0           mov         rdx,rax
  00000001401706D5: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401706DD: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401706E1: 48 89 50 48        mov         qword ptr [rax+48h],rdx
  00000001401706E5: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401706ED: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001401706F1: 48 83 C1 40        add         rcx,40h
  00000001401706F5: E8 F6 A7 AA 00     call        0000000140C1AEF0
  00000001401706FA: 48 85 C0           test        rax,rax
  00000001401706FD: 74 59              je          0000000140170758
  00000001401706FF: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170707: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  000000014017070B: 48 83 C1 40        add         rcx,40h
  000000014017070F: E8 DC A7 AA 00     call        0000000140C1AEF0
  0000000140170714: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  0000000140170719: 48 83 7C 24 70 00  cmp         qword ptr [rsp+70h],0
  000000014017071F: 74 10              je          0000000140170731
  0000000140170721: 48 8B 44 24 70     mov         rax,qword ptr [rsp+70h]
  0000000140170726: 48 83 C0 10        add         rax,10h
  000000014017072A: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000014017072F: EB 09              jmp         000000014017073A
  0000000140170731: 48 C7 44 24 78 00  mov         qword ptr [rsp+78h],0
                    00 00 00
  000000014017073A: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170742: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140170745: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  000000014017074A: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170752: FF 90 38 03 00 00  call        qword ptr [rax+0000000000000338h]
  0000000140170758: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170760: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140170764: 48 83 78 48 00     cmp         qword ptr [rax+48h],0
  0000000140170769: 74 29              je          0000000140170794
  000000014017076B: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  0000000140170773: 48 8B 52 28        mov         rdx,qword ptr [rdx+28h]
  0000000140170777: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017077F: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140170782: 48 8B 52 48        mov         rdx,qword ptr [rdx+48h]
  0000000140170786: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017078E: FF 90 40 03 00 00  call        qword ptr [rax+0000000000000340h]
  0000000140170794: EB 2B              jmp         00000001401707C1
  0000000140170796: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017079E: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001401707A2: 48 83 C1 40        add         rcx,40h
  00000001401707A6: 33 D2              xor         edx,edx
  00000001401707A8: E8 73 C1 B4 00     call        0000000140CBC920
  00000001401707AD: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401707B5: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401707B9: 48 C7 40 48 00 00  mov         qword ptr [rax+48h],0
                    00 00
  00000001401707C1: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001401707C7: 74 3B              je          0000000140170804
  00000001401707C9: 48 8B 44 24 20     mov         rax,qword ptr [rsp+20h]
  00000001401707CE: 48 8B 00           mov         rax,qword ptr [rax]
  00000001401707D1: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001401707D6: FF 50 28           call        qword ptr [rax+28h]
  00000001401707D9: 8B D0              mov         edx,eax
  00000001401707DB: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401707E3: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401707E7: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  00000001401707EB: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  00000001401707F3: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001401707F7: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  00000001401707FB: 48 8B 00           mov         rax,qword ptr [rax]
  00000001401707FE: FF 90 F8 00 00 00  call        qword ptr [rax+00000000000000F8h]
  0000000140170804: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017080C: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140170810: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  0000000140170815: 74 29              je          0000000140170840
  0000000140170817: 48 8B 84 24 A0 00  mov         rax,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017081F: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140170823: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  0000000140170827: 48 8B 8C 24 A0 00  mov         rcx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017082F: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  0000000140170833: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  0000000140170837: 48 8B 00           mov         rax,qword ptr [rax]
  000000014017083A: FF 90 68 01 00 00  call        qword ptr [rax+0000000000000168h]
  0000000140170840: 48 83 BC 24 A8 00  cmp         qword ptr [rsp+00000000000000A8h],0
                    00 00 00
  0000000140170849: 74 1A              je          0000000140170865
  000000014017084B: 4C 8D 05 C6 75 06  lea         r8,[00000001421D7E18h]
                    02
  0000000140170852: 48 8B 94 24 A0 00  mov         rdx,qword ptr [rsp+00000000000000A0h]
                    00 00
  000000014017085A: B9 E8 03 00 00     mov         ecx,3E8h
  000000014017085F: FF 15 13 3C C9 01  call        qword ptr [0000000141E04478h]
  0000000140170865: 48 81 C4 88 00 00  add         rsp,88h
                    00
  000000014017086C: 5F                 pop         rdi
  000000014017086D: 5E                 pop         rsi
  000000014017086E: C3                 ret

  Summary

     1E02000 .text

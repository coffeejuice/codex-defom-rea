Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140183160: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140183165: 56                 push        rsi
  0000000140183166: 57                 push        rdi
  0000000140183167: 48 81 EC 18 03 00  sub         rsp,318h
                    00
  000000014018316E: 48 8B 84 24 30 03  mov         rax,qword ptr [rsp+0000000000000330h]
                    00 00
  0000000140183176: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140183179: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  0000000140183181: FF 90 88 03 00 00  call        qword ptr [rax+0000000000000388h]
  0000000140183187: 48 8B 84 24 30 03  mov         rax,qword ptr [rsp+0000000000000330h]
                    00 00
  000000014018318F: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  0000000140183193: 48 83 78 18 00     cmp         qword ptr [rax+18h],0
  0000000140183198: 0F 84 62 04 00 00  je          0000000140183600
  000000014018319E: 48 8B 84 24 30 03  mov         rax,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001401831A6: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001401831AA: 48 8B 40 18        mov         rax,qword ptr [rax+18h]
  00000001401831AE: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  00000001401831B6: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001401831BA: 48 8B 49 18        mov         rcx,qword ptr [rcx+18h]
  00000001401831BE: 48 8B 00           mov         rax,qword ptr [rax]
  00000001401831C1: FF 90 00 01 00 00  call        qword ptr [rax+0000000000000100h]
  00000001401831C7: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  00000001401831CC: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  00000001401831D2: 0F 84 28 04 00 00  je          0000000140183600
  00000001401831D8: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  00000001401831DD: 48 8D 15 34 51 05  lea         rdx,[00000001421D8318h]
                    02
  00000001401831E4: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  00000001401831EC: E8 8F 1E EA FF     call        0000000140025080
  00000001401831F1: 48 8D BC 24 00 03  lea         rdi,[rsp+0000000000000300h]
                    00 00
  00000001401831F9: 48 8B F0           mov         rsi,rax
  00000001401831FC: B9 10 00 00 00     mov         ecx,10h
  0000000140183201: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183203: 48 8D BC 24 F8 00  lea         rdi,[rsp+00000000000000F8h]
                    00 00
  000000014018320B: 48 8D B4 24 00 03  lea         rsi,[rsp+0000000000000300h]
                    00 00
  0000000140183213: B9 10 00 00 00     mov         ecx,10h
  0000000140183218: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014018321A: 45 33 C0           xor         r8d,r8d
  000000014018321D: 33 D2              xor         edx,edx
  000000014018321F: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  0000000140183224: FF 15 3E 0F C8 01  call        qword ptr [0000000141E04168h]
  000000014018322A: 48 8D BC 24 50 02  lea         rdi,[rsp+0000000000000250h]
                    00 00
  0000000140183232: 48 8B F0           mov         rsi,rax
  0000000140183235: B9 10 00 00 00     mov         ecx,10h
  000000014018323A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014018323C: 45 33 C0           xor         r8d,r8d
  000000014018323F: 33 D2              xor         edx,edx
  0000000140183241: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140183246: FF 15 1C 0F C8 01  call        qword ptr [0000000141E04168h]
  000000014018324C: 48 8D BC 24 40 02  lea         rdi,[rsp+0000000000000240h]
                    00 00
  0000000140183254: 48 8B F0           mov         rsi,rax
  0000000140183257: B9 10 00 00 00     mov         ecx,10h
  000000014018325C: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014018325E: 45 33 C0           xor         r8d,r8d
  0000000140183261: 33 D2              xor         edx,edx
  0000000140183263: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000014018326B: FF 15 F7 0E C8 01  call        qword ptr [0000000141E04168h]
  0000000140183271: 48 8D BC 24 30 02  lea         rdi,[rsp+0000000000000230h]
                    00 00
  0000000140183279: 48 8B F0           mov         rsi,rax
  000000014018327C: B9 10 00 00 00     mov         ecx,10h
  0000000140183281: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183283: 45 33 C0           xor         r8d,r8d
  0000000140183286: 33 D2              xor         edx,edx
  0000000140183288: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140183290: FF 15 D2 0E C8 01  call        qword ptr [0000000141E04168h]
  0000000140183296: 48 8D BC 24 20 02  lea         rdi,[rsp+0000000000000220h]
                    00 00
  000000014018329E: 48 8B F0           mov         rsi,rax
  00000001401832A1: B9 10 00 00 00     mov         ecx,10h
  00000001401832A6: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401832A8: 45 33 C0           xor         r8d,r8d
  00000001401832AB: 33 D2              xor         edx,edx
  00000001401832AD: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  00000001401832B5: FF 15 AD 0E C8 01  call        qword ptr [0000000141E04168h]
  00000001401832BB: 48 8D BC 24 10 02  lea         rdi,[rsp+0000000000000210h]
                    00 00
  00000001401832C3: 48 8B F0           mov         rsi,rax
  00000001401832C6: B9 10 00 00 00     mov         ecx,10h
  00000001401832CB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401832CD: 45 33 C0           xor         r8d,r8d
  00000001401832D0: 33 D2              xor         edx,edx
  00000001401832D2: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000001401832DA: FF 15 88 0E C8 01  call        qword ptr [0000000141E04168h]
  00000001401832E0: 48 8D BC 24 00 02  lea         rdi,[rsp+0000000000000200h]
                    00 00
  00000001401832E8: 48 8B F0           mov         rsi,rax
  00000001401832EB: B9 10 00 00 00     mov         ecx,10h
  00000001401832F0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401832F2: 45 33 C0           xor         r8d,r8d
  00000001401832F5: 33 D2              xor         edx,edx
  00000001401832F7: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001401832FF: FF 15 63 0E C8 01  call        qword ptr [0000000141E04168h]
  0000000140183305: 48 8D BC 24 F0 01  lea         rdi,[rsp+00000000000001F0h]
                    00 00
  000000014018330D: 48 8B F0           mov         rsi,rax
  0000000140183310: B9 10 00 00 00     mov         ecx,10h
  0000000140183315: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183317: 45 33 C0           xor         r8d,r8d
  000000014018331A: 33 D2              xor         edx,edx
  000000014018331C: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  0000000140183324: FF 15 3E 0E C8 01  call        qword ptr [0000000141E04168h]
  000000014018332A: 48 8D BC 24 E0 01  lea         rdi,[rsp+00000000000001E0h]
                    00 00
  0000000140183332: 48 8B F0           mov         rsi,rax
  0000000140183335: B9 10 00 00 00     mov         ecx,10h
  000000014018333A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014018333C: 45 33 C0           xor         r8d,r8d
  000000014018333F: 33 D2              xor         edx,edx
  0000000140183341: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140183349: FF 15 19 0E C8 01  call        qword ptr [0000000141E04168h]
  000000014018334F: 48 8D BC 24 D0 01  lea         rdi,[rsp+00000000000001D0h]
                    00 00
  0000000140183357: 48 8B F0           mov         rsi,rax
  000000014018335A: B9 10 00 00 00     mov         ecx,10h
  000000014018335F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183361: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  0000000140183369: 48 8D B4 24 F8 00  lea         rsi,[rsp+00000000000000F8h]
                    00 00
  0000000140183371: B9 10 00 00 00     mov         ecx,10h
  0000000140183376: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183378: 48 8D 84 24 50 02  lea         rax,[rsp+0000000000000250h]
                    00 00
  0000000140183380: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140183385: 48 8D 84 24 40 02  lea         rax,[rsp+0000000000000240h]
                    00 00
  000000014018338D: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140183392: 48 8D 84 24 30 02  lea         rax,[rsp+0000000000000230h]
                    00 00
  000000014018339A: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014018339F: 48 8D 84 24 20 02  lea         rax,[rsp+0000000000000220h]
                    00 00
  00000001401833A7: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001401833AC: 48 8D 84 24 10 02  lea         rax,[rsp+0000000000000210h]
                    00 00
  00000001401833B4: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001401833B9: 48 8D 84 24 00 02  lea         rax,[rsp+0000000000000200h]
                    00 00
  00000001401833C1: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001401833C6: 48 8D 84 24 F0 01  lea         rax,[rsp+00000000000001F0h]
                    00 00
  00000001401833CE: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001401833D3: 48 8D 84 24 E0 01  lea         rax,[rsp+00000000000001E0h]
                    00 00
  00000001401833DB: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001401833E0: 4C 8D 8C 24 D0 01  lea         r9,[rsp+00000000000001D0h]
                    00 00
  00000001401833E8: 4C 8D 84 24 C0 01  lea         r8,[rsp+00000000000001C0h]
                    00 00
  00000001401833F0: 48 8D 15 31 4F 05  lea         rdx,[00000001421D8328h]
                    02
  00000001401833F7: 48 8D 0D 02 33 BF  lea         rcx,[0000000143D76700h]
                    03
  00000001401833FE: E8 BD BF 14 00     call        00000001402CF3C0
  0000000140183403: 45 33 C0           xor         r8d,r8d
  0000000140183406: 33 D2              xor         edx,edx
  0000000140183408: 48 8D 8C 24 18 01  lea         rcx,[rsp+0000000000000118h]
                    00 00
  0000000140183410: FF 15 52 0D C8 01  call        qword ptr [0000000141E04168h]
  0000000140183416: 48 8D BC 24 F0 02  lea         rdi,[rsp+00000000000002F0h]
                    00 00
  000000014018341E: 48 8B F0           mov         rsi,rax
  0000000140183421: B9 10 00 00 00     mov         ecx,10h
  0000000140183426: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183428: 45 33 C0           xor         r8d,r8d
  000000014018342B: 33 D2              xor         edx,edx
  000000014018342D: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  0000000140183435: FF 15 2D 0D C8 01  call        qword ptr [0000000141E04168h]
  000000014018343B: 48 8D BC 24 E0 02  lea         rdi,[rsp+00000000000002E0h]
                    00 00
  0000000140183443: 48 8B F0           mov         rsi,rax
  0000000140183446: B9 10 00 00 00     mov         ecx,10h
  000000014018344B: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014018344D: 45 33 C0           xor         r8d,r8d
  0000000140183450: 33 D2              xor         edx,edx
  0000000140183452: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  000000014018345A: FF 15 08 0D C8 01  call        qword ptr [0000000141E04168h]
  0000000140183460: 48 8D BC 24 D0 02  lea         rdi,[rsp+00000000000002D0h]
                    00 00
  0000000140183468: 48 8B F0           mov         rsi,rax
  000000014018346B: B9 10 00 00 00     mov         ecx,10h
  0000000140183470: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183472: 45 33 C0           xor         r8d,r8d
  0000000140183475: 33 D2              xor         edx,edx
  0000000140183477: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  000000014018347F: FF 15 E3 0C C8 01  call        qword ptr [0000000141E04168h]
  0000000140183485: 48 8D BC 24 C0 02  lea         rdi,[rsp+00000000000002C0h]
                    00 00
  000000014018348D: 48 8B F0           mov         rsi,rax
  0000000140183490: B9 10 00 00 00     mov         ecx,10h
  0000000140183495: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183497: 45 33 C0           xor         r8d,r8d
  000000014018349A: 33 D2              xor         edx,edx
  000000014018349C: 48 8D 8C 24 58 01  lea         rcx,[rsp+0000000000000158h]
                    00 00
  00000001401834A4: FF 15 BE 0C C8 01  call        qword ptr [0000000141E04168h]
  00000001401834AA: 48 8D BC 24 B0 02  lea         rdi,[rsp+00000000000002B0h]
                    00 00
  00000001401834B2: 48 8B F0           mov         rsi,rax
  00000001401834B5: B9 10 00 00 00     mov         ecx,10h
  00000001401834BA: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401834BC: 45 33 C0           xor         r8d,r8d
  00000001401834BF: 33 D2              xor         edx,edx
  00000001401834C1: 48 8D 8C 24 68 01  lea         rcx,[rsp+0000000000000168h]
                    00 00
  00000001401834C9: FF 15 99 0C C8 01  call        qword ptr [0000000141E04168h]
  00000001401834CF: 48 8D BC 24 A0 02  lea         rdi,[rsp+00000000000002A0h]
                    00 00
  00000001401834D7: 48 8B F0           mov         rsi,rax
  00000001401834DA: B9 10 00 00 00     mov         ecx,10h
  00000001401834DF: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401834E1: 45 33 C0           xor         r8d,r8d
  00000001401834E4: 33 D2              xor         edx,edx
  00000001401834E6: 48 8D 8C 24 78 01  lea         rcx,[rsp+0000000000000178h]
                    00 00
  00000001401834EE: FF 15 74 0C C8 01  call        qword ptr [0000000141E04168h]
  00000001401834F4: 48 8D BC 24 90 02  lea         rdi,[rsp+0000000000000290h]
                    00 00
  00000001401834FC: 48 8B F0           mov         rsi,rax
  00000001401834FF: B9 10 00 00 00     mov         ecx,10h
  0000000140183504: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183506: 45 33 C0           xor         r8d,r8d
  0000000140183509: 33 D2              xor         edx,edx
  000000014018350B: 48 8D 8C 24 88 01  lea         rcx,[rsp+0000000000000188h]
                    00 00
  0000000140183513: FF 15 4F 0C C8 01  call        qword ptr [0000000141E04168h]
  0000000140183519: 48 8D BC 24 80 02  lea         rdi,[rsp+0000000000000280h]
                    00 00
  0000000140183521: 48 8B F0           mov         rsi,rax
  0000000140183524: B9 10 00 00 00     mov         ecx,10h
  0000000140183529: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014018352B: 45 33 C0           xor         r8d,r8d
  000000014018352E: 33 D2              xor         edx,edx
  0000000140183530: 48 8D 8C 24 98 01  lea         rcx,[rsp+0000000000000198h]
                    00 00
  0000000140183538: FF 15 2A 0C C8 01  call        qword ptr [0000000141E04168h]
  000000014018353E: 48 8D BC 24 70 02  lea         rdi,[rsp+0000000000000270h]
                    00 00
  0000000140183546: 48 8B F0           mov         rsi,rax
  0000000140183549: B9 10 00 00 00     mov         ecx,10h
  000000014018354E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183550: 45 33 C0           xor         r8d,r8d
  0000000140183553: 33 D2              xor         edx,edx
  0000000140183555: 48 8D 8C 24 A8 01  lea         rcx,[rsp+00000000000001A8h]
                    00 00
  000000014018355D: FF 15 05 0C C8 01  call        qword ptr [0000000141E04168h]
  0000000140183563: 48 8D BC 24 60 02  lea         rdi,[rsp+0000000000000260h]
                    00 00
  000000014018356B: 48 8B F0           mov         rsi,rax
  000000014018356E: B9 10 00 00 00     mov         ecx,10h
  0000000140183573: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140183575: 48 8D 84 24 F0 02  lea         rax,[rsp+00000000000002F0h]
                    00 00
  000000014018357D: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140183582: 48 8D 84 24 E0 02  lea         rax,[rsp+00000000000002E0h]
                    00 00
  000000014018358A: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014018358F: 48 8D 84 24 D0 02  lea         rax,[rsp+00000000000002D0h]
                    00 00
  0000000140183597: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000014018359C: 48 8D 84 24 C0 02  lea         rax,[rsp+00000000000002C0h]
                    00 00
  00000001401835A4: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  00000001401835A9: 48 8D 84 24 B0 02  lea         rax,[rsp+00000000000002B0h]
                    00 00
  00000001401835B1: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001401835B6: 48 8D 84 24 A0 02  lea         rax,[rsp+00000000000002A0h]
                    00 00
  00000001401835BE: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  00000001401835C3: 48 8D 84 24 90 02  lea         rax,[rsp+0000000000000290h]
                    00 00
  00000001401835CB: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  00000001401835D0: 48 8D 84 24 80 02  lea         rax,[rsp+0000000000000280h]
                    00 00
  00000001401835D8: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001401835DD: 4C 8D 8C 24 70 02  lea         r9,[rsp+0000000000000270h]
                    00 00
  00000001401835E5: 4C 8D 84 24 60 02  lea         r8,[rsp+0000000000000260h]
                    00 00
  00000001401835ED: 48 8D 15 54 4D 05  lea         rdx,[00000001421D8348h]
                    02
  00000001401835F4: 48 8D 0D 05 31 BF  lea         rcx,[0000000143D76700h]
                    03
  00000001401835FB: E8 C0 BD 14 00     call        00000001402CF3C0
  0000000140183600: 48 81 C4 18 03 00  add         rsp,318h
                    00
  0000000140183607: 5F                 pop         rdi
  0000000140183608: 5E                 pop         rsi
  0000000140183609: C3                 ret

  Summary

     1E02000 .text

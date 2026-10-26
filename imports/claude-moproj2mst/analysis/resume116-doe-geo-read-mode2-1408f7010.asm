Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001408F7010: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  00000001408F7015: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  00000001408F701A: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001408F701F: 48 81 EC 78 01 00  sub         rsp,178h
                    00
  00000001408F7026: 48 C7 84 24 58 01  mov         qword ptr [rsp+0000000000000158h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  00000001408F7032: 48 8B 05 F7 6F 26  mov         rax,qword ptr [0000000143B5E030h]
                    03
  00000001408F7039: 48 33 C4           xor         rax,rsp
  00000001408F703C: 48 89 84 24 60 01  mov         qword ptr [rsp+0000000000000160h],rax
                    00 00
  00000001408F7044: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7049: E8 F2 F6 00 00     call        0000000140906740
  00000001408F704E: 90                 nop
  00000001408F704F: C7 44 24 54 63 00  mov         dword ptr [rsp+54h],63h
                    00 00
  00000001408F7057: C6 44 24 30 00     mov         byte ptr [rsp+30h],0
  00000001408F705C: 48 8B 8C 24 88 01  mov         rcx,qword ptr [rsp+0000000000000188h]
                    00 00
  00000001408F7064: E8 37 13 00 00     call        00000001408F83A0
  00000001408F7069: 0F B6 C0           movzx       eax,al
  00000001408F706C: 85 C0              test        eax,eax
  00000001408F706E: 75 1E              jne         00000001408F708E
  00000001408F7070: C6 84 24 50 01 00  mov         byte ptr [rsp+0000000000000150h],0
                    00 00
  00000001408F7078: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F707D: E8 DE F6 00 00     call        0000000140906760
  00000001408F7082: 8A 84 24 50 01 00  mov         al,byte ptr [rsp+0000000000000150h]
                    00
  00000001408F7089: E9 4B 08 00 00     jmp         00000001408F78D9
  00000001408F708E: 48 8D 15 1F 09 EC  lea         rdx,[00000001427B79B4h]
                    01
  00000001408F7095: 48 8B 8C 24 88 01  mov         rcx,qword ptr [rsp+0000000000000188h]
                    00 00
  00000001408F709D: E8 36 8C B7 00     call        000000014146FCD8
  00000001408F70A2: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  00000001408F70A7: 48 83 7C 24 38 00  cmp         qword ptr [rsp+38h],0
  00000001408F70AD: 75 1E              jne         00000001408F70CD
  00000001408F70AF: C6 84 24 51 01 00  mov         byte ptr [rsp+0000000000000151h],0
                    00 00
  00000001408F70B7: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F70BC: E8 9F F6 00 00     call        0000000140906760
  00000001408F70C1: 8A 84 24 51 01 00  mov         al,byte ptr [rsp+0000000000000151h]
                    00
  00000001408F70C8: E9 0C 08 00 00     jmp         00000001408F78D9
  00000001408F70CD: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F70D5: E8 76 FD FF FF     call        00000001408F6E50
  00000001408F70DA: B9 64 00 00 00     mov         ecx,64h
  00000001408F70DF: E8 3C 0F 0D 00     call        00000001409C8020
  00000001408F70E4: 4C 8B D8           mov         r11,rax
  00000001408F70E7: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F70EF: 4C 89 58 10        mov         qword ptr [rax+10h],r11
  00000001408F70F3: C7 44 24 44 FF FF  mov         dword ptr [rsp+44h],0FFFFFFFFh
                    FF FF
  00000001408F70FB: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  00000001408F7103: 4C 8D 4C 24 64     lea         r9,[rsp+64h]
  00000001408F7108: 41 B8 05 00 00 00  mov         r8d,5
  00000001408F710E: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F7113: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7118: E8 D3 F7 00 00     call        00000001409068F0
  00000001408F711D: 85 C0              test        eax,eax
  00000001408F711F: 0F 8E 3B 07 00 00  jle         00000001408F7860
  00000001408F7125: 8B 44 24 54        mov         eax,dword ptr [rsp+54h]
  00000001408F7129: 39 44 24 64        cmp         dword ptr [rsp+64h],eax
  00000001408F712D: 75 0A              jne         00000001408F7139
  00000001408F712F: C6 44 24 30 01     mov         byte ptr [rsp+30h],1
  00000001408F7134: E9 27 07 00 00     jmp         00000001408F7860
  00000001408F7139: 8B 44 24 64        mov         eax,dword ptr [rsp+64h]
  00000001408F713D: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000001408F7141: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F7149: 4C 8D 4C 24 6C     lea         r9,[rsp+6Ch]
  00000001408F714E: 41 B8 05 00 00 00  mov         r8d,5
  00000001408F7154: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F7159: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F715E: E8 8D F7 00 00     call        00000001409068F0
  00000001408F7163: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F716B: 4C 8D 4C 24 4C     lea         r9,[rsp+4Ch]
  00000001408F7170: 41 B8 05 00 00 00  mov         r8d,5
  00000001408F7176: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F717B: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7180: E8 6B F7 00 00     call        00000001409068F0
  00000001408F7185: 83 7C 24 44 00     cmp         dword ptr [rsp+44h],0
  00000001408F718A: 7C 11              jl          00000001408F719D
  00000001408F718C: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  00000001408F7190: 39 44 24 44        cmp         dword ptr [rsp+44h],eax
  00000001408F7194: 74 07              je          00000001408F719D
  00000001408F7196: C6 44 24 48 01     mov         byte ptr [rsp+48h],1
  00000001408F719B: EB 72              jmp         00000001408F720F
  00000001408F719D: C6 44 24 48 00     mov         byte ptr [rsp+48h],0
  00000001408F71A2: 83 7C 24 60 03     cmp         dword ptr [rsp+60h],3
  00000001408F71A7: 75 34              jne         00000001408F71DD
  00000001408F71A9: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F71B1: 8B 50 20           mov         edx,dword ptr [rax+20h]
  00000001408F71B4: 03 54 24 6C        add         edx,dword ptr [rsp+6Ch]
  00000001408F71B8: 6B D2 06           imul        edx,edx,6
  00000001408F71BB: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F71C3: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001408F71C7: E8 44 12 0D 00     call        00000001409C8410
  00000001408F71CC: 4C 8B D8           mov         r11,rax
  00000001408F71CF: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F71D7: 4C 89 58 28        mov         qword ptr [rax+28h],r11
  00000001408F71DB: EB 32              jmp         00000001408F720F
  00000001408F71DD: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F71E5: 8B 50 20           mov         edx,dword ptr [rax+20h]
  00000001408F71E8: 03 54 24 6C        add         edx,dword ptr [rsp+6Ch]
  00000001408F71EC: 6B D2 03           imul        edx,edx,3
  00000001408F71EF: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F71F7: 48 8B 49 28        mov         rcx,qword ptr [rcx+28h]
  00000001408F71FB: E8 10 12 0D 00     call        00000001409C8410
  00000001408F7200: 4C 8B D8           mov         r11,rax
  00000001408F7203: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F720B: 4C 89 58 28        mov         qword ptr [rax+28h],r11
  00000001408F720F: 83 7C 24 60 01     cmp         dword ptr [rsp+60h],1
  00000001408F7214: 0F 85 20 01 00 00  jne         00000001408F733A
  00000001408F721A: C7 44 24 74 00 00  mov         dword ptr [rsp+74h],0
                    00 00
  00000001408F7222: EB 0B              jmp         00000001408F722F
  00000001408F7224: 8B 44 24 74        mov         eax,dword ptr [rsp+74h]
  00000001408F7228: 83 C0 01           add         eax,1
  00000001408F722B: 89 44 24 74        mov         dword ptr [rsp+74h],eax
  00000001408F722F: 8B 44 24 6C        mov         eax,dword ptr [rsp+6Ch]
  00000001408F7233: 39 44 24 74        cmp         dword ptr [rsp+74h],eax
  00000001408F7237: 0F 8D F8 00 00 00  jge         00000001408F7335
  00000001408F723D: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  00000001408F7242: 85 C0              test        eax,eax
  00000001408F7244: 74 2A              je          00000001408F7270
  00000001408F7246: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  00000001408F724E: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  00000001408F7256: 41 B8 0C 00 00 00  mov         r8d,0Ch
  00000001408F725C: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F7261: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7266: E8 85 F6 00 00     call        00000001409068F0
  00000001408F726B: E9 C0 00 00 00     jmp         00000001408F7330
  00000001408F7270: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7278: 8B 40 20           mov         eax,dword ptr [rax+20h]
  00000001408F727B: 03 44 24 74        add         eax,dword ptr [rsp+74h]
  00000001408F727F: 6B C0 03           imul        eax,eax,3
  00000001408F7282: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  00000001408F7286: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  00000001408F728E: 4C 8D 4C 24 70     lea         r9,[rsp+70h]
  00000001408F7293: 41 B8 05 00 00 00  mov         r8d,5
  00000001408F7299: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F729E: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F72A3: E8 48 F6 00 00     call        00000001409068F0
  00000001408F72A8: 4C 63 5C 24 50     movsxd      r11,dword ptr [rsp+50h]
  00000001408F72AD: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F72B5: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F72B9: 4E 8D 0C D8        lea         r9,[rax+r11*8]
  00000001408F72BD: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F72C5: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F72CB: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F72D0: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F72D5: E8 16 F6 00 00     call        00000001409068F0
  00000001408F72DA: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F72DE: 83 C0 01           add         eax,1
  00000001408F72E1: 48 63 C8           movsxd      rcx,eax
  00000001408F72E4: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F72EC: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F72F0: 4C 8D 0C C8        lea         r9,[rax+rcx*8]
  00000001408F72F4: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F72FC: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F7302: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F7307: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F730C: E8 DF F5 00 00     call        00000001409068F0
  00000001408F7311: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F7315: 83 C0 02           add         eax,2
  00000001408F7318: 48 63 C8           movsxd      rcx,eax
  00000001408F731B: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7323: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F7327: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001408F732B: F2 0F 11 04 C8     movsd       mmword ptr [rax+rcx*8],xmm0
  00000001408F7330: E9 EF FE FF FF     jmp         00000001408F7224
  00000001408F7335: E9 9C 04 00 00     jmp         00000001408F77D6
  00000001408F733A: 83 7C 24 60 02     cmp         dword ptr [rsp+60h],2
  00000001408F733F: 0F 85 38 01 00 00  jne         00000001408F747D
  00000001408F7345: C7 44 24 74 00 00  mov         dword ptr [rsp+74h],0
                    00 00
  00000001408F734D: EB 0B              jmp         00000001408F735A
  00000001408F734F: 8B 44 24 74        mov         eax,dword ptr [rsp+74h]
  00000001408F7353: 83 C0 01           add         eax,1
  00000001408F7356: 89 44 24 74        mov         dword ptr [rsp+74h],eax
  00000001408F735A: 8B 44 24 6C        mov         eax,dword ptr [rsp+6Ch]
  00000001408F735E: 39 44 24 74        cmp         dword ptr [rsp+74h],eax
  00000001408F7362: 0F 8D 10 01 00 00  jge         00000001408F7478
  00000001408F7368: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  00000001408F736D: 85 C0              test        eax,eax
  00000001408F736F: 74 2A              je          00000001408F739B
  00000001408F7371: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  00000001408F7379: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  00000001408F7381: 41 B8 0C 00 00 00  mov         r8d,0Ch
  00000001408F7387: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F738C: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7391: E8 5A F5 00 00     call        00000001409068F0
  00000001408F7396: E9 D8 00 00 00     jmp         00000001408F7473
  00000001408F739B: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F73A3: 8B 40 20           mov         eax,dword ptr [rax+20h]
  00000001408F73A6: 03 44 24 74        add         eax,dword ptr [rsp+74h]
  00000001408F73AA: 6B C0 03           imul        eax,eax,3
  00000001408F73AD: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  00000001408F73B1: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  00000001408F73B9: 4C 8D 4C 24 70     lea         r9,[rsp+70h]
  00000001408F73BE: 41 B8 05 00 00 00  mov         r8d,5
  00000001408F73C4: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F73C9: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F73CE: E8 1D F5 00 00     call        00000001409068F0
  00000001408F73D3: 4C 63 5C 24 50     movsxd      r11,dword ptr [rsp+50h]
  00000001408F73D8: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F73E0: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F73E4: 4E 8D 0C D8        lea         r9,[rax+r11*8]
  00000001408F73E8: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F73F0: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F73F6: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F73FB: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7400: E8 EB F4 00 00     call        00000001409068F0
  00000001408F7405: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F7409: 83 C0 01           add         eax,1
  00000001408F740C: 48 63 C8           movsxd      rcx,eax
  00000001408F740F: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7417: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F741B: 4C 8D 0C C8        lea         r9,[rax+rcx*8]
  00000001408F741F: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F7427: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F742D: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F7432: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7437: E8 B4 F4 00 00     call        00000001409068F0
  00000001408F743C: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F7440: 83 C0 02           add         eax,2
  00000001408F7443: 48 63 C8           movsxd      rcx,eax
  00000001408F7446: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F744E: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F7452: 4C 8D 0C C8        lea         r9,[rax+rcx*8]
  00000001408F7456: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F745E: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F7464: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F7469: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F746E: E8 7D F4 00 00     call        00000001409068F0
  00000001408F7473: E9 D7 FE FF FF     jmp         00000001408F734F
  00000001408F7478: E9 59 03 00 00     jmp         00000001408F77D6
  00000001408F747D: 83 7C 24 60 03     cmp         dword ptr [rsp+60h],3
  00000001408F7482: 0F 85 F1 01 00 00  jne         00000001408F7679
  00000001408F7488: C7 44 24 74 00 00  mov         dword ptr [rsp+74h],0
                    00 00
  00000001408F7490: EB 0B              jmp         00000001408F749D
  00000001408F7492: 8B 44 24 74        mov         eax,dword ptr [rsp+74h]
  00000001408F7496: 83 C0 01           add         eax,1
  00000001408F7499: 89 44 24 74        mov         dword ptr [rsp+74h],eax
  00000001408F749D: 8B 44 24 6C        mov         eax,dword ptr [rsp+6Ch]
  00000001408F74A1: 39 44 24 74        cmp         dword ptr [rsp+74h],eax
  00000001408F74A5: 0F 8D C9 01 00 00  jge         00000001408F7674
  00000001408F74AB: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  00000001408F74B0: 85 C0              test        eax,eax
  00000001408F74B2: 74 2A              je          00000001408F74DE
  00000001408F74B4: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  00000001408F74BC: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  00000001408F74C4: 41 B8 0C 00 00 00  mov         r8d,0Ch
  00000001408F74CA: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F74CF: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F74D4: E8 17 F4 00 00     call        00000001409068F0
  00000001408F74D9: E9 91 01 00 00     jmp         00000001408F766F
  00000001408F74DE: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F74E6: 8B 40 20           mov         eax,dword ptr [rax+20h]
  00000001408F74E9: 03 44 24 74        add         eax,dword ptr [rsp+74h]
  00000001408F74ED: 6B C0 06           imul        eax,eax,6
  00000001408F74F0: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  00000001408F74F4: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  00000001408F74FC: 4C 8D 4C 24 70     lea         r9,[rsp+70h]
  00000001408F7501: 41 B8 05 00 00 00  mov         r8d,5
  00000001408F7507: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F750C: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7511: E8 DA F3 00 00     call        00000001409068F0
  00000001408F7516: 4C 63 5C 24 50     movsxd      r11,dword ptr [rsp+50h]
  00000001408F751B: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7523: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F7527: 4E 8D 0C D8        lea         r9,[rax+r11*8]
  00000001408F752B: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F7533: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F7539: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F753E: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7543: E8 A8 F3 00 00     call        00000001409068F0
  00000001408F7548: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F754C: 83 C0 01           add         eax,1
  00000001408F754F: 48 63 C8           movsxd      rcx,eax
  00000001408F7552: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F755A: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F755E: 4C 8D 0C C8        lea         r9,[rax+rcx*8]
  00000001408F7562: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F756A: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F7570: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F7575: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F757A: E8 71 F3 00 00     call        00000001409068F0
  00000001408F757F: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F7583: 83 C0 02           add         eax,2
  00000001408F7586: 48 63 C8           movsxd      rcx,eax
  00000001408F7589: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7591: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F7595: 4C 8D 0C C8        lea         r9,[rax+rcx*8]
  00000001408F7599: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F75A1: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F75A7: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F75AC: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F75B1: E8 3A F3 00 00     call        00000001409068F0
  00000001408F75B6: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F75BA: 83 C0 03           add         eax,3
  00000001408F75BD: 48 63 C8           movsxd      rcx,eax
  00000001408F75C0: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F75C8: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F75CC: 4C 8D 0C C8        lea         r9,[rax+rcx*8]
  00000001408F75D0: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F75D8: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F75DE: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F75E3: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F75E8: E8 03 F3 00 00     call        00000001409068F0
  00000001408F75ED: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F75F1: 83 C0 04           add         eax,4
  00000001408F75F4: 48 63 C8           movsxd      rcx,eax
  00000001408F75F7: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F75FF: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F7603: 4C 8D 0C C8        lea         r9,[rax+rcx*8]
  00000001408F7607: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F760F: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F7615: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F761A: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F761F: E8 CC F2 00 00     call        00000001409068F0
  00000001408F7624: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F762C: 4C 8D 4C 24 58     lea         r9,[rsp+58h]
  00000001408F7631: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F7637: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F763C: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7641: E8 AA F2 00 00     call        00000001409068F0
  00000001408F7646: F2 0F 10 44 24 58  movsd       xmm0,mmword ptr [rsp+58h]
  00000001408F764C: F2 0F 59 05 94 1C  mulsd       xmm0,mmword ptr [00000001426992E8h]
                    DA 01
  00000001408F7654: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F7658: 83 C0 05           add         eax,5
  00000001408F765B: 48 63 C8           movsxd      rcx,eax
  00000001408F765E: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7666: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F766A: F2 0F 11 04 C8     movsd       mmword ptr [rax+rcx*8],xmm0
  00000001408F766F: E9 1E FE FF FF     jmp         00000001408F7492
  00000001408F7674: E9 5D 01 00 00     jmp         00000001408F77D6
  00000001408F7679: 83 7C 24 60 04     cmp         dword ptr [rsp+60h],4
  00000001408F767E: 0F 85 52 01 00 00  jne         00000001408F77D6
  00000001408F7684: C7 44 24 74 00 00  mov         dword ptr [rsp+74h],0
                    00 00
  00000001408F768C: EB 0B              jmp         00000001408F7699
  00000001408F768E: 8B 44 24 74        mov         eax,dword ptr [rsp+74h]
  00000001408F7692: 83 C0 01           add         eax,1
  00000001408F7695: 89 44 24 74        mov         dword ptr [rsp+74h],eax
  00000001408F7699: 8B 44 24 6C        mov         eax,dword ptr [rsp+6Ch]
  00000001408F769D: 39 44 24 74        cmp         dword ptr [rsp+74h],eax
  00000001408F76A1: 0F 8D 2F 01 00 00  jge         00000001408F77D6
  00000001408F76A7: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  00000001408F76AC: 85 C0              test        eax,eax
  00000001408F76AE: 74 2A              je          00000001408F76DA
  00000001408F76B0: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  00000001408F76B8: 4C 8D 8C 24 80 00  lea         r9,[rsp+0000000000000080h]
                    00 00
  00000001408F76C0: 41 B8 0C 00 00 00  mov         r8d,0Ch
  00000001408F76C6: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F76CB: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F76D0: E8 1B F2 00 00     call        00000001409068F0
  00000001408F76D5: E9 F7 00 00 00     jmp         00000001408F77D1
  00000001408F76DA: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F76E2: 8B 40 20           mov         eax,dword ptr [rax+20h]
  00000001408F76E5: 03 44 24 74        add         eax,dword ptr [rsp+74h]
  00000001408F76E9: 6B C0 03           imul        eax,eax,3
  00000001408F76EC: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  00000001408F76F0: C7 44 24 20 78 EC  mov         dword ptr [rsp+20h],0FFFFEC78h
                    FF FF
  00000001408F76F8: 4C 8D 4C 24 70     lea         r9,[rsp+70h]
  00000001408F76FD: 41 B8 05 00 00 00  mov         r8d,5
  00000001408F7703: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F7708: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F770D: E8 DE F1 00 00     call        00000001409068F0
  00000001408F7712: 4C 63 5C 24 50     movsxd      r11,dword ptr [rsp+50h]
  00000001408F7717: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F771F: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F7723: 4E 8D 0C D8        lea         r9,[rax+r11*8]
  00000001408F7727: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F772F: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F7735: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F773A: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F773F: E8 AC F1 00 00     call        00000001409068F0
  00000001408F7744: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F7748: 83 C0 01           add         eax,1
  00000001408F774B: 48 63 C8           movsxd      rcx,eax
  00000001408F774E: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7756: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F775A: 4C 8D 0C C8        lea         r9,[rax+rcx*8]
  00000001408F775E: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F7766: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F776C: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F7771: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F7776: E8 75 F1 00 00     call        00000001409068F0
  00000001408F777B: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F777F: 83 C0 02           add         eax,2
  00000001408F7782: 48 63 C8           movsxd      rcx,eax
  00000001408F7785: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F778D: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F7791: 4C 8D 0C C8        lea         r9,[rax+rcx*8]
  00000001408F7795: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001408F779D: 41 B8 0A 00 00 00  mov         r8d,0Ah
  00000001408F77A3: 48 8B 54 24 38     mov         rdx,qword ptr [rsp+38h]
  00000001408F77A8: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F77AD: E8 3E F1 00 00     call        00000001409068F0
  00000001408F77B2: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  00000001408F77B6: 83 C0 02           add         eax,2
  00000001408F77B9: 48 63 C8           movsxd      rcx,eax
  00000001408F77BC: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F77C4: 48 8B 40 28        mov         rax,qword ptr [rax+28h]
  00000001408F77C8: 66 0F 57 C0        xorpd       xmm0,xmm0
  00000001408F77CC: F2 0F 11 04 C8     movsd       mmword ptr [rax+rcx*8],xmm0
  00000001408F77D1: E9 B8 FE FF FF     jmp         00000001408F768E
  00000001408F77D6: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  00000001408F77DB: 85 C0              test        eax,eax
  00000001408F77DD: 74 08              je          00000001408F77E7
  00000001408F77DF: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  00000001408F77E3: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000001408F77E7: 83 7C 24 4C 00     cmp         dword ptr [rsp+4Ch],0
  00000001408F77EC: 7C 6D              jl          00000001408F785B
  00000001408F77EE: 0F B6 44 24 48     movzx       eax,byte ptr [rsp+48h]
  00000001408F77F3: 85 C0              test        eax,eax
  00000001408F77F5: 75 64              jne         00000001408F785B
  00000001408F77F7: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F77FF: 8B 50 20           mov         edx,dword ptr [rax+20h]
  00000001408F7802: 83 C2 01           add         edx,1
  00000001408F7805: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F780D: 48 63 48 0C        movsxd      rcx,dword ptr [rax+0Ch]
  00000001408F7811: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7819: 48 8B 40 10        mov         rax,qword ptr [rax+10h]
  00000001408F781D: 89 14 88           mov         dword ptr [rax+rcx*4],edx
  00000001408F7820: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7828: 8B 48 0C           mov         ecx,dword ptr [rax+0Ch]
  00000001408F782B: 83 C1 01           add         ecx,1
  00000001408F782E: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7836: 89 48 0C           mov         dword ptr [rax+0Ch],ecx
  00000001408F7839: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7841: 8B 48 20           mov         ecx,dword ptr [rax+20h]
  00000001408F7844: 03 4C 24 6C        add         ecx,dword ptr [rsp+6Ch]
  00000001408F7848: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7850: 89 48 20           mov         dword ptr [rax+20h],ecx
  00000001408F7853: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  00000001408F7857: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  00000001408F785B: E9 9B F8 FF FF     jmp         00000001408F70FB
  00000001408F7860: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  00000001408F7864: 83 E8 01           sub         eax,1
  00000001408F7867: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  00000001408F786B: 83 7C 24 60 00     cmp         dword ptr [rsp+60h],0
  00000001408F7870: 74 07              je          00000001408F7879
  00000001408F7872: 83 7C 24 60 03     cmp         dword ptr [rsp+60h],3
  00000001408F7877: 75 11              jne         00000001408F788A
  00000001408F7879: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7881: C7 40 08 01 00 00  mov         dword ptr [rax+8],1
                    00
  00000001408F7888: EB 0F              jmp         00000001408F7899
  00000001408F788A: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F7892: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  00000001408F7896: 89 41 08           mov         dword ptr [rcx+8],eax
  00000001408F7899: 48 8B 94 24 90 01  mov         rdx,qword ptr [rsp+0000000000000190h]
                    00 00
  00000001408F78A1: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  00000001408F78A9: E8 52 40 00 00     call        00000001408FB900
  00000001408F78AE: 88 44 24 40        mov         byte ptr [rsp+40h],al
  00000001408F78B2: 48 8B 4C 24 38     mov         rcx,qword ptr [rsp+38h]
  00000001408F78B7: E8 6C 82 B7 00     call        000000014146FB28
  00000001408F78BC: 0F B6 44 24 40     movzx       eax,byte ptr [rsp+40h]
  00000001408F78C1: 88 84 24 52 01 00  mov         byte ptr [rsp+0000000000000152h],al
                    00
  00000001408F78C8: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001408F78CD: E8 8E EE 00 00     call        0000000140906760
  00000001408F78D2: 8A 84 24 52 01 00  mov         al,byte ptr [rsp+0000000000000152h]
                    00
  00000001408F78D9: 48 8B 8C 24 60 01  mov         rcx,qword ptr [rsp+0000000000000160h]
                    00 00
  00000001408F78E1: 48 33 CC           xor         rcx,rsp
  00000001408F78E4: E8 27 66 B7 00     call        000000014146DF10
  00000001408F78E9: 48 81 C4 78 01 00  add         rsp,178h
                    00
  00000001408F78F0: C3                 ret

  Summary

     1E02000 .text

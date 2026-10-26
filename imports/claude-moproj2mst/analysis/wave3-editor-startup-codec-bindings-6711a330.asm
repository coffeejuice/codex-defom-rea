Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711A330: 48 85 C9           test        rcx,rcx
  000000006711A333: 0F 84 A2 00 00 00  je          000000006711A3DB
  000000006711A339: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006711A33E: 53                 push        rbx
  000000006711A33F: 48 83 EC 40        sub         rsp,40h
  000000006711A343: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711A34C: 48 8B D9           mov         rbx,rcx
  000000006711A34F: 48 8B 0D CA 9E 1D  mov         rcx,qword ptr [00000000672F4220h]
                    00
  000000006711A356: 48 85 C9           test        rcx,rcx
  000000006711A359: 0F 95 C0           setne       al
  000000006711A35C: 84 C0              test        al,al
  000000006711A35E: 75 15              jne         000000006711A375
  000000006711A360: 48 8D 15 49 FA 11  lea         rdx,[0000000067239DB0h]
                    00
  000000006711A367: 48 8D 0D A2 F4 11  lea         rcx,[0000000067239810h]
                    00
  000000006711A36E: E8 6D 2B F0 FF     call        000000006701CEE0
  000000006711A373: EB 61              jmp         000000006711A3D6
  000000006711A375: 48 8B 49 08        mov         rcx,qword ptr [rcx+8]
  000000006711A379: 48 81 C1 98 00 00  add         rcx,98h
                    00
  000000006711A380: 48 8D 54 24 50     lea         rdx,[rsp+50h]
  000000006711A385: E8 46 1A 03 00     call        000000006714BDD0
  000000006711A38A: 4C 8B 1B           mov         r11,qword ptr [rbx]
  000000006711A38D: 48 8B CB           mov         rcx,rbx
  000000006711A390: 41 FF 53 60        call        qword ptr [r11+60h]
  000000006711A394: 84 C0              test        al,al
  000000006711A396: 75 3E              jne         000000006711A3D6
  000000006711A398: BA 59 00 00 00     mov         edx,59h
  000000006711A39D: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006711A3A2: E8 A9 31 00 00     call        000000006711D550
  000000006711A3A7: 90                 nop
  000000006711A3A8: B8 FD FF 00 00     mov         eax,0FFFDh
  000000006711A3AD: 66 21 44 24 3A     and         word ptr [rsp+3Ah],ax
  000000006711A3B2: 48 8B 0D 67 9E 1D  mov         rcx,qword ptr [00000000672F4220h]
                    00
  000000006711A3B9: 48 85 C9           test        rcx,rcx
  000000006711A3BC: 74 0E              je          000000006711A3CC
  000000006711A3BE: 4C 8D 44 24 28     lea         r8,[rsp+28h]
  000000006711A3C3: 48 8B D1           mov         rdx,rcx
  000000006711A3C6: E8 A5 F1 FF FF     call        0000000067119570
  000000006711A3CB: 90                 nop
  000000006711A3CC: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006711A3D1: E8 AA 31 00 00     call        000000006711D580
  000000006711A3D6: 48 83 C4 40        add         rsp,40h
  000000006711A3DA: 5B                 pop         rbx
  000000006711A3DB: F3 C3              rep ret

  Summary

      1A7000 .text

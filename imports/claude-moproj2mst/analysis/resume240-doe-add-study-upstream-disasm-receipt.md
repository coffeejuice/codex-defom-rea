# CP240 upstream Add Study disassembly receipt

- Target: `evidence/original-binaries/DEF_GUI_MO_64.exe`
- Target SHA-256: `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`
- Tool: `C:\Program Files (x86)\Microsoft Visual Studio\2022\BuildTools\VC\Tools\MSVC\14.44.35207\bin\Hostx64\x64\dumpbin.exe`
- Tool SHA-256: `12a1cd87238bd66dfdb788b4fcdcb91ce4b3f81236aab12a7a29e5ae1d85af50`
- Captured UTC: `2026-10-26T18:17:24.438796+00:00`
- Qualification: DUMPBIN static PE disassembly at image-base VAs; no runtime delivery, branch result, or complete-function claim. Each bounded command and stdout excerpt follows.

## action labels

Command: `dumpbin.exe /disasm:bytes /range:0x140005b4e,0x140005b95 evidence/original-binaries/DEF_GUI_MO_64.exe`

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140005B4E: 45 33 C0           xor         r8d,r8d
  0000000140005B51: 48 8D 15 B8 C9 0E  lea         rdx,[00000001420F2510h]
                    02
  0000000140005B58: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000140005B5D: E8 2E A7 00 00     call        0000000140010290
  0000000140005B62: 48 89 84 24 C0 01  mov         qword ptr [rsp+00000000000001C0h],rax
                    00 00
  0000000140005B6A: 48 8B 84 24 C0 01  mov         rax,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140005B72: 48 89 84 24 C8 01  mov         qword ptr [rsp+00000000000001C8h],rax
                    00 00
  0000000140005B7A: 45 33 C0           xor         r8d,r8d
  0000000140005B7D: 48 8D 15 A4 C9 0E  lea         rdx,[00000001420F2528h]
                    02
  0000000140005B84: 48 8D 4C 24 48     lea         rcx,[rsp+48h]
  0000000140005B89: E8 02 A7 00 00     call        0000000140010290
  0000000140005B8E: 48 89 84 24 D0 01  mov         qword ptr [rsp+00000000000001D0h],rax
                    00 00
```

## QAction triggered connection

Command: `dumpbin.exe /disasm:bytes /range:0x14000689d,0x1400068cc evidence/original-binaries/DEF_GUI_MO_64.exe`

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014000689D: C7 44 24 20 00 00  mov         dword ptr [rsp+20h],0
                    00 00
  00000001400068A5: 4C 8D 0D 34 BF 0E  lea         r9,[00000001420F27E0h]
                    02
  00000001400068AC: 4C 8B 84 24 60 03  mov         r8,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001400068B4: 48 8D 15 45 BF 0E  lea         rdx,[00000001420F2800h]
                    02
  00000001400068BB: 48 8B 8C 24 60 03  mov         rcx,qword ptr [rsp+0000000000000360h]
                    00 00
  00000001400068C3: 48 8B 49 30        mov         rcx,qword ptr [rcx+30h]
  00000001400068C7: FF 15 03 D9 DF 01  call        qword ptr [0000000141E041D0h]
```

## meta dispatch and Type 2 wrapper

Command: `dumpbin.exe /disasm:bytes /range:0x1400d8593,0x1400d859c evidence/original-binaries/DEF_GUI_MO_64.exe`

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400D8593: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001400D8598: E8 83 03 F3 FF     call        0000000140008920
```

## Type 2 wrapper

Command: `dumpbin.exe /disasm:bytes /range:0x140008920,0x140008940 evidence/original-binaries/DEF_GUI_MO_64.exe`

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140008920: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140008925: 48 83 EC 28        sub         rsp,28h
  0000000140008929: BA 02 00 00 00     mov         edx,2
  000000014000892E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  0000000140008933: 48 8B 49 10        mov         rcx,qword ptr [rcx+10h]
  0000000140008937: E8 E4 21 02 00     call        000000014002AB20
  000000014000893C: 48 83 C4 28        add         rsp,28h
  0000000140008940: C3                 ret
```

## guard and project acquisition

Command: `dumpbin.exe /disasm:bytes /range:0x14002ab50,0x14002abcb evidence/original-binaries/DEF_GUI_MO_64.exe`

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014002AB50: C6 44 24 79 01     mov         byte ptr [rsp+79h],1
  000000014002AB55: 0F B6 54 24 79     movzx       edx,byte ptr [rsp+79h]
  000000014002AB5A: 48 8D 0D 57 BB D4  lea         rcx,[0000000143D766B8h]
                    03
  000000014002AB61: E8 BA FD 29 00     call        00000001402CA920
  000000014002AB66: 0F B6 C0           movzx       eax,al
  000000014002AB69: 85 C0              test        eax,eax
  000000014002AB6B: 74 05              je          000000014002AB72
  000000014002AB6D: E9 5A 06 00 00     jmp         000000014002B1CC
  000000014002AB72: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  000000014002AB7A: E8 21 88 FE FF     call        00000001400133A0
  000000014002AB7F: 48 8B C8           mov         rcx,rax
  000000014002AB82: E8 F9 AB 01 00     call        0000000140045780
  000000014002AB87: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000014002AB8C: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  000000014002AB92: 75 05              jne         000000014002AB99
  000000014002AB94: E9 33 06 00 00     jmp         000000014002B1CC
  000000014002AB99: C6 44 24 70 01     mov         byte ptr [rsp+70h],1
  000000014002AB9E: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  000000014002ABA4: 0F 84 60 02 00 00  je          000000014002AE0A
  000000014002ABAA: 83 BC 24 38 03 00  cmp         dword ptr [rsp+0000000000000338h],2
                    00 02
  000000014002ABB2: 74 18              je          000000014002ABCC
  000000014002ABB4: 83 BC 24 38 03 00  cmp         dword ptr [rsp+0000000000000338h],3
                    00 03
  000000014002ABBC: 74 0E              je          000000014002ABCC
  000000014002ABBE: 83 BC 24 38 03 00  cmp         dword ptr [rsp+0000000000000338h],4
                    00 04
  000000014002ABC6: 0F 85 3E 02 00 00  jne         000000014002AE0A
```

## creation branch and successor

Command: `dumpbin.exe /disasm:bytes /range:0x14002ae1e,0x14002ae2a evidence/original-binaries/DEF_GUI_MO_64.exe`

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014002AE1E: 0F B6 44 24 70     movzx       eax,byte ptr [rsp+70h]
  000000014002AE23: 85 C0              test        eax,eax
  000000014002AE25: 0F 85 03 01 00 00  jne         000000014002AF2E
```

## creation call and successor

Command: `dumpbin.exe /disasm:bytes /range:0x14002af2e,0x14002afa2 evidence/original-binaries/DEF_GUI_MO_64.exe`

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014002AF2E: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  000000014002AF36: E8 65 84 FE FF     call        00000001400133A0
  000000014002AF3B: 48 8B C8           mov         rcx,rax
  000000014002AF3E: E8 2D A9 01 00     call        0000000140045870
  000000014002AF43: 8B 94 24 38 03 00  mov         edx,dword ptr [rsp+0000000000000338h]
                    00
  000000014002AF4A: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014002AF4F: E8 AC BA 07 00     call        00000001400A6A00
  000000014002AF54: 48 8B 4C 24 60     mov         rcx,qword ptr [rsp+60h]
  000000014002AF59: E8 62 C4 07 00     call        00000001400A73C0
  000000014002AF5E: 89 44 24 74        mov         dword ptr [rsp+74h],eax
  000000014002AF62: C6 44 24 78 00     mov         byte ptr [rsp+78h],0
  000000014002AF67: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  000000014002AF6F: E8 2C 84 FE FF     call        00000001400133A0
  000000014002AF74: 48 8B C8           mov         rcx,rax
  000000014002AF77: E8 74 A8 01 00     call        00000001400457F0
  000000014002AF7C: 89 84 24 80 01 00  mov         dword ptr [rsp+0000000000000180h],eax
                    00
  000000014002AF83: 44 0F B6 4C 24 78  movzx       r9d,byte ptr [rsp+78h]
  000000014002AF89: 4C 8D 44 24 74     lea         r8,[rsp+74h]
  000000014002AF8E: 48 8D 94 24 80 01  lea         rdx,[rsp+0000000000000180h]
                    00 00
  000000014002AF96: 48 8B 8C 24 30 03  mov         rcx,qword ptr [rsp+0000000000000330h]
                    00 00
  000000014002AF9E: E8 0D FA FF FF     call        000000014002A9B0
```

## Flag stores and zero-flag fallthrough

Command: `dumpbin.exe /disasm:bytes /range:0x14002adbc,0x14002ae50 evidence/original-binaries/DEF_GUI_MO_64.exe`

```text
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014002ADBC: 8B 84 24 A8 00 00  mov         eax,dword ptr [rsp+00000000000000A8h]
                    00
  000000014002ADC3: 39 84 24 28 01 00  cmp         dword ptr [rsp+0000000000000128h],eax
                    00
  000000014002ADCA: 75 1C              jne         000000014002ADE8
  000000014002ADCC: 8B 84 24 2C 01 00  mov         eax,dword ptr [rsp+000000000000012Ch]
                    00
  000000014002ADD3: 39 84 24 20 01 00  cmp         dword ptr [rsp+0000000000000120h],eax
                    00
  000000014002ADDA: 75 11              jne         000000014002ADED
  000000014002ADDC: 0F B6 84 24 38 01  movzx       eax,byte ptr [rsp+0000000000000138h]
                    00 00
  000000014002ADE4: 85 C0              test        eax,eax
  000000014002ADE6: 75 05              jne         000000014002ADED
  000000014002ADE8: C6 44 24 70 00     mov         byte ptr [rsp+70h],0
  000000014002ADED: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  000000014002ADF5: E8 B6 D3 03 00     call        00000001400681B0
  000000014002ADFA: 90                 nop
  000000014002ADFB: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  000000014002AE03: E8 08 6B 00 00     call        0000000140031910
  000000014002AE08: EB 14              jmp         000000014002AE1E
  000000014002AE0A: 48 83 7C 24 60 00  cmp         qword ptr [rsp+60h],0
  000000014002AE10: 74 07              je          000000014002AE19
  000000014002AE12: C6 44 24 70 01     mov         byte ptr [rsp+70h],1
  000000014002AE17: EB 05              jmp         000000014002AE1E
  000000014002AE19: C6 44 24 70 00     mov         byte ptr [rsp+70h],0
  000000014002AE1E: 0F B6 44 24 70     movzx       eax,byte ptr [rsp+70h]
  000000014002AE23: 85 C0              test        eax,eax
  000000014002AE25: 0F 85 03 01 00 00  jne         000000014002AF2E
  000000014002AE2B: 48 8D 8C 24 50 01  lea         rcx,[rsp+0000000000000150h]
                    00 00
  000000014002AE33: FF 15 87 96 DD 01  call        qword ptr [0000000141E044C0h]
  000000014002AE39: 90                 nop
  000000014002AE3A: 45 33 C0           xor         r8d,r8d
  000000014002AE3D: 48 8D 15 6C 53 0D  lea         rdx,[00000001421001B0h]
                    02
  000000014002AE44: 48 8D 8C 24 60 01  lea         rcx,[rsp+0000000000000160h]
                    00 00
  000000014002AE4C: E8 2F 9F FF FF     call        0000000140024D80
```

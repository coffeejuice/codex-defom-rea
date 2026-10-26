Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650494B0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000000650494B5: 57                 push        rdi
  00000000650494B6: 48 83 EC 30        sub         rsp,30h
  00000000650494BA: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000650494C3: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  00000000650494C8: 48 89 6C 24 50     mov         qword ptr [rsp+50h],rbp
  00000000650494CD: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  00000000650494D2: 48 8B F1           mov         rsi,rcx
  00000000650494D5: 33 ED              xor         ebp,ebp
  00000000650494D7: 89 69 08           mov         dword ptr [rcx+8],ebp
  00000000650494DA: 48 89 51 10        mov         qword ptr [rcx+10h],rdx
  00000000650494DE: 48 8D 05 E3 C3 65  lea         rax,[00000000656A58C8h]
                    00
  00000000650494E5: 48 89 01           mov         qword ptr [rcx],rax
  00000000650494E8: 48 83 C1 18        add         rcx,18h
  00000000650494EC: BA 85 FF FF FF     mov         edx,0FFFFFF85h
  00000000650494F1: 44 8B C2           mov         r8d,edx
  00000000650494F4: FF 15 26 D5 64 00  call        qword ptr [0000000065696A20h]
  00000000650494FA: 48 8D 4E 20        lea         rcx,[rsi+20h]
  00000000650494FE: BA 85 FF FF FF     mov         edx,0FFFFFF85h
  0000000065049503: 44 8B C2           mov         r8d,edx
  0000000065049506: FF 15 14 D5 64 00  call        qword ptr [0000000065696A20h]
  000000006504950C: 48 8D 4E 28        lea         rcx,[rsi+28h]
  0000000065049510: BA 85 FF FF FF     mov         edx,0FFFFFF85h
  0000000065049515: 44 8B C2           mov         r8d,edx
  0000000065049518: FF 15 02 D5 64 00  call        qword ptr [0000000065696A20h]
  000000006504951E: 48 8D 5E 30        lea         rbx,[rsi+30h]
  0000000065049522: 8D 7D 02           lea         edi,[rbp+2]
  0000000065049525: 48 8B CB           mov         rcx,rbx
  0000000065049528: FF 15 4A D2 64 00  call        qword ptr [0000000065696778h]
  000000006504952E: 48 83 C3 08        add         rbx,8
  0000000065049532: 83 EF 01           sub         edi,1
  0000000065049535: 79 EE              jns         0000000065049525
  0000000065049537: 66 89 6E 48        mov         word ptr [rsi+48h],bp
  000000006504953B: 66 89 6E 4A        mov         word ptr [rsi+4Ah],bp
  000000006504953F: 48 89 6E 50        mov         qword ptr [rsi+50h],rbp
  0000000065049543: 48 8B 46 10        mov         rax,qword ptr [rsi+10h]
  0000000065049547: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006504954B: 48 83 B9 C0 00 00  cmp         qword ptr [rcx+00000000000000C0h],0
                    00 00
  0000000065049553: 75 0C              jne         0000000065049561
  0000000065049555: 48 89 B1 C0 00 00  mov         qword ptr [rcx+00000000000000C0h],rsi
                    00
  000000006504955C: 48 8B C6           mov         rax,rsi
  000000006504955F: EB 03              jmp         0000000065049564
  0000000065049561: 48 8B C6           mov         rax,rsi
  0000000065049564: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  0000000065049569: 48 8B 6C 24 50     mov         rbp,qword ptr [rsp+50h]
  000000006504956E: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  0000000065049573: 48 83 C4 30        add         rsp,30h
  0000000065049577: 5F                 pop         rdi
  0000000065049578: C3                 ret

  Summary

      695000 .text

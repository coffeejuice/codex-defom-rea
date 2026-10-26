Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006701CC30: 40 57              push        rdi
  000000006701CC32: 48 83 EC 30        sub         rsp,30h
  000000006701CC36: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006701CC3F: 48 89 5C 24 48     mov         qword ptr [rsp+48h],rbx
  000000006701CC44: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  000000006701CC49: 48 8B F2           mov         rsi,rdx
  000000006701CC4C: 8B D9              mov         ebx,ecx
  000000006701CC4E: 33 FF              xor         edi,edi
  000000006701CC50: 89 7C 24 40        mov         dword ptr [rsp+40h],edi
  000000006701CC54: 48 8B 05 DD 6F 2D  mov         rax,qword ptr [00000000672F3C38h]
                    00
  000000006701CC5B: 48 85 C0           test        rax,rax
  000000006701CC5E: 74 04              je          000000006701CC64
  000000006701CC60: FF D0              call        rax
  000000006701CC62: EB 2A              jmp         000000006701CC8E
  000000006701CC64: FF 15 9E B9 18 00  call        qword ptr [00000000671A8608h]
  000000006701CC6A: 4C 8B C6           mov         r8,rsi
  000000006701CC6D: 48 8D 15 C8 F8 18  lea         rdx,[00000000671AC53Ch]
                    00
  000000006701CC74: 48 8D 48 60        lea         rcx,[rax+60h]
  000000006701CC78: FF 15 82 B9 18 00  call        qword ptr [00000000671A8600h]
  000000006701CC7E: FF 15 84 B9 18 00  call        qword ptr [00000000671A8608h]
  000000006701CC84: 48 8D 48 60        lea         rcx,[rax+60h]
  000000006701CC88: FF 15 6A B9 18 00  call        qword ptr [00000000671A85F8h]
  000000006701CC8E: 83 FB 03           cmp         ebx,3
  000000006701CC91: 74 2D              je          000000006701CCC0
  000000006701CC93: 83 FB 01           cmp         ebx,1
  000000006701CC96: 75 24              jne         000000006701CCBC
  000000006701CC98: 48 8D 15 89 F8 18  lea         rdx,[00000000671AC528h]
                    00
  000000006701CC9F: 48 8D 4C 24 50     lea         rcx,[rsp+50h]
  000000006701CCA4: E8 57 FC FF FF     call        000000006701C900
  000000006701CCA9: 90                 nop
  000000006701CCAA: 8B FB              mov         edi,ebx
  000000006701CCAC: 89 5C 24 40        mov         dword ptr [rsp+40h],ebx
  000000006701CCB0: 48 8B C8           mov         rcx,rax
  000000006701CCB3: E8 48 04 01 00     call        000000006702D100
  000000006701CCB8: 84 C0              test        al,al
  000000006701CCBA: 74 04              je          000000006701CCC0
  000000006701CCBC: 32 DB              xor         bl,bl
  000000006701CCBE: EB 02              jmp         000000006701CCC2
  000000006701CCC0: B3 01              mov         bl,1
  000000006701CCC2: 40 F6 C7 01        test        dil,1
  000000006701CCC6: 74 15              je          000000006701CCDD
  000000006701CCC8: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  000000006701CCCD: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006701CCD1: 75 0A              jne         000000006701CCDD
  000000006701CCD3: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  000000006701CCD8: E8 D3 36 00 00     call        00000000670203B0
  000000006701CCDD: 84 DB              test        bl,bl
  000000006701CCDF: 74 0C              je          000000006701CCED
  000000006701CCE1: B9 01 00 00 00     mov         ecx,1
  000000006701CCE6: FF 15 04 B9 18 00  call        qword ptr [00000000671A85F0h]
  000000006701CCEC: CC                 int         3
  000000006701CCED: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000006701CCF2: 48 8B 74 24 58     mov         rsi,qword ptr [rsp+58h]
  000000006701CCF7: 48 83 C4 30        add         rsp,30h
  000000006701CCFB: 5F                 pop         rdi
  000000006701CCFC: C3                 ret

  Summary

      1A7000 .text

; delete helper; bounded range 1407fbc10..1407fbc6d
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001407FBC10: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  00000001407FBC15: 48 83 EC 38        sub         rsp,38h
  00000001407FBC19: 48 8D 15 C4 F2 05  lea         rdx,[000000014385AEE4h]
                    03
  00000001407FBC20: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407FBC25: E8 AE 40 C7 00     call        000000014146FCD8
  00000001407FBC2A: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  00000001407FBC2F: 48 83 7C 24 20 00  cmp         qword ptr [rsp+20h],0
  00000001407FBC35: 75 07              jne         00000001407FBC3E
  00000001407FBC37: B8 01 00 00 00     mov         eax,1
  00000001407FBC3C: EB 2A              jmp         00000001407FBC68
  00000001407FBC3E: 48 8B 4C 24 20     mov         rcx,qword ptr [rsp+20h]
  00000001407FBC43: E8 E0 3E C7 00     call        000000014146FB28
  00000001407FBC48: 48 8B 4C 24 40     mov         rcx,qword ptr [rsp+40h]
  00000001407FBC4D: E8 26 41 C7 00     call        000000014146FD78
  00000001407FBC52: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  00000001407FBC56: 83 7C 24 28 00     cmp         dword ptr [rsp+28h],0
  00000001407FBC5B: 75 09              jne         00000001407FBC66
  00000001407FBC5D: B8 01 00 00 00     mov         eax,1
  00000001407FBC62: EB 04              jmp         00000001407FBC68
  00000001407FBC64: EB 02              jmp         00000001407FBC68
  00000001407FBC66: 33 C0              xor         eax,eax
  00000001407FBC68: 48 83 C4 38        add         rsp,38h
  00000001407FBC6C: C3                 ret
  00000001407FBC6D: CC                 int         3

  Summary

     1E02000 .text


; fopen sets sharing0x40; bounded range 14146fcd8..14146fce3
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146FCD8: 41 B8 40 00 00 00  mov         r8d,40h
  000000014146FCDE: E9 BD FE FF FF     jmp         000000014146FBA0
  000000014146FCE3: CC                 int         3

  Summary

     1E02000 .text


; fsopen narrow byte checks; bounded range 14146fba0..14146fcd5
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146FBA0: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  000000014146FBA5: 48 89 74 24 18     mov         qword ptr [rsp+18h],rsi
  000000014146FBAA: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  000000014146FBAF: 41 54              push        r12
  000000014146FBB1: 48 83 EC 40        sub         rsp,40h
  000000014146FBB5: 48 89 64 24 30     mov         qword ptr [rsp+30h],rsp
  000000014146FBBA: 45 8B E0           mov         r12d,r8d
  000000014146FBBD: 48 8B FA           mov         rdi,rdx
  000000014146FBC0: 48 8B F1           mov         rsi,rcx
  000000014146FBC3: 33 C0              xor         eax,eax
  000000014146FBC5: 48 85 C9           test        rcx,rcx
  000000014146FBC8: 0F 95 C0           setne       al
  000000014146FBCB: 85 C0              test        eax,eax
  000000014146FBCD: 75 27              jne         000000014146FBF6
  000000014146FBCF: E8 60 45 00 00     call        0000000141474134
  000000014146FBD4: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146FBDA: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014146FBE0: 45 33 C9           xor         r9d,r9d
  000000014146FBE3: 45 33 C0           xor         r8d,r8d
  000000014146FBE6: 33 D2              xor         edx,edx
  000000014146FBE8: 33 C9              xor         ecx,ecx
  000000014146FBEA: E8 79 09 00 00     call        0000000141470568
  000000014146FBEF: 33 C0              xor         eax,eax
  000000014146FBF1: E9 C9 00 00 00     jmp         000000014146FCBF
  000000014146FBF6: 33 C0              xor         eax,eax
  000000014146FBF8: 48 85 D2           test        rdx,rdx
  000000014146FBFB: 0F 95 C0           setne       al
  000000014146FBFE: 85 C0              test        eax,eax
  000000014146FC00: 75 27              jne         000000014146FC29
  000000014146FC02: E8 2D 45 00 00     call        0000000141474134
  000000014146FC07: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146FC0D: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014146FC13: 45 33 C9           xor         r9d,r9d
  000000014146FC16: 45 33 C0           xor         r8d,r8d
  000000014146FC19: 33 D2              xor         edx,edx
  000000014146FC1B: 33 C9              xor         ecx,ecx
  000000014146FC1D: E8 46 09 00 00     call        0000000141470568
  000000014146FC22: 33 C0              xor         eax,eax
  000000014146FC24: E9 96 00 00 00     jmp         000000014146FCBF
  000000014146FC29: 33 C0              xor         eax,eax
  000000014146FC2B: 38 02              cmp         byte ptr [rdx],al
  000000014146FC2D: 0F 95 C0           setne       al
  000000014146FC30: 85 C0              test        eax,eax
  000000014146FC32: 75 24              jne         000000014146FC58
  000000014146FC34: E8 FB 44 00 00     call        0000000141474134
  000000014146FC39: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146FC3F: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014146FC45: 45 33 C9           xor         r9d,r9d
  000000014146FC48: 45 33 C0           xor         r8d,r8d
  000000014146FC4B: 33 D2              xor         edx,edx
  000000014146FC4D: 33 C9              xor         ecx,ecx
  000000014146FC4F: E8 14 09 00 00     call        0000000141470568
  000000014146FC54: 33 C0              xor         eax,eax
  000000014146FC56: EB 67              jmp         000000014146FCBF
  000000014146FC58: E8 17 27 01 00     call        0000000141482374
  000000014146FC5D: 48 8B D8           mov         rbx,rax
  000000014146FC60: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000014146FC65: 48 85 C0           test        rax,rax
  000000014146FC68: 75 0F              jne         000000014146FC79
  000000014146FC6A: E8 C5 44 00 00     call        0000000141474134
  000000014146FC6F: C7 00 18 00 00 00  mov         dword ptr [rax],18h
  000000014146FC75: 33 C0              xor         eax,eax
  000000014146FC77: EB 46              jmp         000000014146FCBF
  000000014146FC79: 80 3E 00           cmp         byte ptr [rsi],0
  000000014146FC7C: 75 22              jne         000000014146FCA0
  000000014146FC7E: E8 B1 44 00 00     call        0000000141474134
  000000014146FC83: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146FC89: 48 8D 15 0C 00 00  lea         rdx,[000000014146FC9Ch]
                    00
  000000014146FC90: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000014146FC95: E8 56 28 01 00     call        00000001414824F0
  000000014146FC9A: 90                 nop
  000000014146FC9B: 90                 nop
  000000014146FC9C: 33 C0              xor         eax,eax
  000000014146FC9E: EB 1F              jmp         000000014146FCBF
  000000014146FCA0: 4C 8B C8           mov         r9,rax
  000000014146FCA3: 45 8B C4           mov         r8d,r12d
  000000014146FCA6: 48 8B D7           mov         rdx,rdi
  000000014146FCA9: 48 8B CE           mov         rcx,rsi
  000000014146FCAC: E8 FB 23 01 00     call        00000001414820AC
  000000014146FCB1: 48 8B F8           mov         rdi,rax
  000000014146FCB4: 48 8B CB           mov         rcx,rbx
  000000014146FCB7: E8 6C E0 FF FF     call        000000014146DD28
  000000014146FCBC: 48 8B C7           mov         rax,rdi
  000000014146FCBF: 48 8B 5C 24 58     mov         rbx,qword ptr [rsp+58h]
  000000014146FCC4: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  000000014146FCC9: 48 8B 7C 24 68     mov         rdi,qword ptr [rsp+68h]
  000000014146FCCE: 48 83 C4 40        add         rsp,40h
  000000014146FCD2: 41 5C              pop         r12
  000000014146FCD4: C3                 ret
  000000014146FCD5: CC                 int         3

  Summary

     1E02000 .text


; fclose; bounded range 14146fb28..14146fb9e
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146FB28: 48 89 5C 24 10     mov         qword ptr [rsp+10h],rbx
  000000014146FB2D: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014146FB32: 57                 push        rdi
  000000014146FB33: 48 83 EC 30        sub         rsp,30h
  000000014146FB37: 48 8B D9           mov         rbx,rcx
  000000014146FB3A: 83 CF FF           or          edi,0FFFFFFFFh
  000000014146FB3D: 33 C0              xor         eax,eax
  000000014146FB3F: 48 85 C9           test        rcx,rcx
  000000014146FB42: 0F 95 C0           setne       al
  000000014146FB45: 85 C0              test        eax,eax
  000000014146FB47: 75 24              jne         000000014146FB6D
  000000014146FB49: E8 E6 45 00 00     call        0000000141474134
  000000014146FB4E: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  000000014146FB54: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014146FB5A: 45 33 C9           xor         r9d,r9d
  000000014146FB5D: 45 33 C0           xor         r8d,r8d
  000000014146FB60: 33 D2              xor         edx,edx
  000000014146FB62: 33 C9              xor         ecx,ecx
  000000014146FB64: E8 FF 09 00 00     call        0000000141470568
  000000014146FB69: 8B C7              mov         eax,edi
  000000014146FB6B: EB 26              jmp         000000014146FB93
  000000014146FB6D: F6 41 18 40        test        byte ptr [rcx+18h],40h
  000000014146FB71: 74 06              je          000000014146FB79
  000000014146FB73: 83 61 18 00        and         dword ptr [rcx+18h],0
  000000014146FB77: EB 18              jmp         000000014146FB91
  000000014146FB79: E8 1A E1 FF FF     call        000000014146DC98
  000000014146FB7E: 90                 nop
  000000014146FB7F: 48 8B CB           mov         rcx,rbx
  000000014146FB82: E8 15 FF FF FF     call        000000014146FA9C
  000000014146FB87: 8B F8              mov         edi,eax
  000000014146FB89: 48 8B CB           mov         rcx,rbx
  000000014146FB8C: E8 97 E1 FF FF     call        000000014146DD28
  000000014146FB91: 8B C7              mov         eax,edi
  000000014146FB93: 48 8B 5C 24 48     mov         rbx,qword ptr [rsp+48h]
  000000014146FB98: 48 83 C4 30        add         rsp,30h
  000000014146FB9C: 5F                 pop         rdi
  000000014146FB9D: C3                 ret
  000000014146FB9E: CC                 int         3

  Summary

     1E02000 .text


; remove and forwarding entry; bounded range 14146fd48..14146fd7e
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014146FD48: 48 83 EC 28        sub         rsp,28h
  000000014146FD4C: FF 15 2E 35 99 00  call        qword ptr [0000000141E03280h]
  000000014146FD52: 85 C0              test        eax,eax
  000000014146FD54: 75 08              jne         000000014146FD5E
  000000014146FD56: FF 15 8C 38 99 00  call        qword ptr [0000000141E035E8h]
  000000014146FD5C: EB 02              jmp         000000014146FD60
  000000014146FD5E: 33 C0              xor         eax,eax
  000000014146FD60: 85 C0              test        eax,eax
  000000014146FD62: 74 0C              je          000000014146FD70
  000000014146FD64: 8B C8              mov         ecx,eax
  000000014146FD66: E8 09 44 00 00     call        0000000141474174
  000000014146FD6B: 83 C8 FF           or          eax,0FFFFFFFFh
  000000014146FD6E: EB 02              jmp         000000014146FD72
  000000014146FD70: 33 C0              xor         eax,eax
  000000014146FD72: 48 83 C4 28        add         rsp,28h
  000000014146FD76: C3                 ret
  000000014146FD77: CC                 int         3
  000000014146FD78: E9 CB FF FF FF     jmp         000000014146FD48
  000000014146FD7D: CC                 int         3
  000000014146FD7E: CC                 int         3

  Summary

     1E02000 .text


; rt parser -> narrow sopen; bounded range 1414820ac..141482374
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414820AC: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000001414820B1: 48 89 6C 24 18     mov         qword ptr [rsp+18h],rbp
  00000001414820B6: 56                 push        rsi
  00000001414820B7: 57                 push        rdi
  00000001414820B8: 41 54              push        r12
  00000001414820BA: 41 55              push        r13
  00000001414820BC: 41 57              push        r15
  00000001414820BE: 48 83 EC 30        sub         rsp,30h
  00000001414820C2: 8B 2D 78 E8 27 03  mov         ebp,dword ptr [0000000144700940h]
  00000001414820C8: 49 8B F1           mov         rsi,r9
  00000001414820CB: 45 33 C9           xor         r9d,r9d
  00000001414820CE: 45 33 D2           xor         r10d,r10d
  00000001414820D1: 45 33 DB           xor         r11d,r11d
  00000001414820D4: 80 3A 20           cmp         byte ptr [rdx],20h
  00000001414820D7: 45 8D 79 01        lea         r15d,[r9+1]
  00000001414820DB: 45 8B E0           mov         r12d,r8d
  00000001414820DE: 48 8B DA           mov         rbx,rdx
  00000001414820E1: 4C 8B E9           mov         r13,rcx
  00000001414820E4: 75 08              jne         00000001414820EE
  00000001414820E6: 49 03 DF           add         rbx,r15
  00000001414820E9: 80 3B 20           cmp         byte ptr [rbx],20h
  00000001414820EC: 74 F8              je          00000001414820E6
  00000001414820EE: 80 3B 61           cmp         byte ptr [rbx],61h
  00000001414820F1: 74 3F              je          0000000141482132
  00000001414820F3: 80 3B 72           cmp         byte ptr [rbx],72h
  00000001414820F6: 74 33              je          000000014148212B
  00000001414820F8: 80 3B 77           cmp         byte ptr [rbx],77h
  00000001414820FB: 74 27              je          0000000141482124
  00000001414820FD: E8 32 20 FF FF     call        0000000141474134
  0000000141482102: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  0000000141482108: 45 33 C9           xor         r9d,r9d
  000000014148210B: 45 33 C0           xor         r8d,r8d
  000000014148210E: 33 D2              xor         edx,edx
  0000000141482110: 33 C9              xor         ecx,ecx
  0000000141482112: C7 00 16 00 00 00  mov         dword ptr [rax],16h
  0000000141482118: E8 4B E4 FE FF     call        0000000141470568
  000000014148211D: 33 C0              xor         eax,eax
  000000014148211F: E9 39 02 00 00     jmp         000000014148235D
  0000000141482124: BF 01 03 00 00     mov         edi,301h
  0000000141482129: EB 0C              jmp         0000000141482137
  000000014148212B: 33 FF              xor         edi,edi
  000000014148212D: 41 0B EF           or          ebp,r15d
  0000000141482130: EB 08              jmp         000000014148213A
  0000000141482132: BF 09 01 00 00     mov         edi,109h
  0000000141482137: 83 CD 02           or          ebp,2
  000000014148213A: 49 03 DF           add         rbx,r15
  000000014148213D: 41 8B D7           mov         edx,r15d
  0000000141482140: 8A 03              mov         al,byte ptr [rbx]
  0000000141482142: 84 C0              test        al,al
  0000000141482144: 0F 84 BD 01 00 00  je          0000000141482307
  000000014148214A: 41 B8 00 C0 00 00  mov         r8d,0C000h
  0000000141482150: 85 D2              test        edx,edx
  0000000141482152: 0F 84 F0 00 00 00  je          0000000141482248
  0000000141482158: 0F BE C8           movsx       ecx,al
  000000014148215B: 83 F9 53           cmp         ecx,53h
  000000014148215E: 7F 7E              jg          00000001414821DE
  0000000141482160: 74 6F              je          00000001414821D1
  0000000141482162: 83 E9 20           sub         ecx,20h
  0000000141482165: 0F 84 D0 00 00 00  je          000000014148223B
  000000014148216B: 83 E9 0B           sub         ecx,0Bh
  000000014148216E: 74 4C              je          00000001414821BC
  0000000141482170: 41 2B CF           sub         ecx,r15d
  0000000141482173: 74 42              je          00000001414821B7
  0000000141482175: 83 E9 18           sub         ecx,18h
  0000000141482178: 74 2B              je          00000001414821A5
  000000014148217A: 83 E9 0A           sub         ecx,0Ah
  000000014148217D: 74 1D              je          000000014148219C
  000000014148217F: 83 F9 04           cmp         ecx,4
  0000000141482182: 0F 85 75 FF FF FF  jne         00000001414820FD
  0000000141482188: 45 85 D2           test        r10d,r10d
  000000014148218B: 0F 85 A2 00 00 00  jne         0000000141482233
  0000000141482191: 45 8B D7           mov         r10d,r15d
  0000000141482194: 83 CF 10           or          edi,10h
  0000000141482197: E9 9F 00 00 00     jmp         000000014148223B
  000000014148219C: 0F BA EF 07        bts         edi,7
  00000001414821A0: E9 96 00 00 00     jmp         000000014148223B
  00000001414821A5: 40 F6 C7 40        test        dil,40h
  00000001414821A9: 0F 85 84 00 00 00  jne         0000000141482233
  00000001414821AF: 83 CF 40           or          edi,40h
  00000001414821B2: E9 84 00 00 00     jmp         000000014148223B
  00000001414821B7: 45 8B DF           mov         r11d,r15d
  00000001414821BA: EB 77              jmp         0000000141482233
  00000001414821BC: 40 F6 C7 02        test        dil,2
  00000001414821C0: 75 71              jne         0000000141482233
  00000001414821C2: 83 E7 FE           and         edi,0FFFFFFFEh
  00000001414821C5: 83 E5 FC           and         ebp,0FFFFFFFCh
  00000001414821C8: 83 CF 02           or          edi,2
  00000001414821CB: 0F BA ED 07        bts         ebp,7
  00000001414821CF: EB 6A              jmp         000000014148223B
  00000001414821D1: 45 85 D2           test        r10d,r10d
  00000001414821D4: 75 5D              jne         0000000141482233
  00000001414821D6: 45 8B D7           mov         r10d,r15d
  00000001414821D9: 83 CF 20           or          edi,20h
  00000001414821DC: EB 5D              jmp         000000014148223B
  00000001414821DE: 83 E9 54           sub         ecx,54h
  00000001414821E1: 74 4A              je          000000014148222D
  00000001414821E3: 83 E9 0E           sub         ecx,0Eh
  00000001414821E6: 74 3A              je          0000000141482222
  00000001414821E8: 41 2B CF           sub         ecx,r15d
  00000001414821EB: 74 27              je          0000000141482214
  00000001414821ED: 83 E9 0B           sub         ecx,0Bh
  00000001414821F0: 74 14              je          0000000141482206
  00000001414821F2: 83 F9 06           cmp         ecx,6
  00000001414821F5: 0F 85 02 FF FF FF  jne         00000001414820FD
  00000001414821FB: 41 85 F8           test        r8d,edi
  00000001414821FE: 75 33              jne         0000000141482233
  0000000141482200: 0F BA EF 0E        bts         edi,0Eh
  0000000141482204: EB 35              jmp         000000014148223B
  0000000141482206: 45 85 C9           test        r9d,r9d
  0000000141482209: 75 28              jne         0000000141482233
  000000014148220B: 45 8B CF           mov         r9d,r15d
  000000014148220E: 0F BA F5 0E        btr         ebp,0Eh
  0000000141482212: EB 27              jmp         000000014148223B
  0000000141482214: 45 85 C9           test        r9d,r9d
  0000000141482217: 75 1A              jne         0000000141482233
  0000000141482219: 45 8B CF           mov         r9d,r15d
  000000014148221C: 0F BA ED 0E        bts         ebp,0Eh
  0000000141482220: EB 19              jmp         000000014148223B
  0000000141482222: 41 85 F8           test        r8d,edi
  0000000141482225: 75 0C              jne         0000000141482233
  0000000141482227: 0F BA EF 0F        bts         edi,0Fh
  000000014148222B: EB 0E              jmp         000000014148223B
  000000014148222D: 0F BA E7 0C        bt          edi,0Ch
  0000000141482231: 73 04              jae         0000000141482237
  0000000141482233: 33 D2              xor         edx,edx
  0000000141482235: EB 04              jmp         000000014148223B
  0000000141482237: 0F BA EF 0C        bts         edi,0Ch
  000000014148223B: 49 03 DF           add         rbx,r15
  000000014148223E: 8A 03              mov         al,byte ptr [rbx]
  0000000141482240: 84 C0              test        al,al
  0000000141482242: 0F 85 08 FF FF FF  jne         0000000141482150
  0000000141482248: 45 85 DB           test        r11d,r11d
  000000014148224B: 0F 84 B6 00 00 00  je          0000000141482307
  0000000141482251: EB 03              jmp         0000000141482256
  0000000141482253: 49 03 DF           add         rbx,r15
  0000000141482256: 80 3B 20           cmp         byte ptr [rbx],20h
  0000000141482259: 74 F8              je          0000000141482253
  000000014148225B: 48 8D 0D 5E 44 A8  lea         rcx,[0000000142F066C0h]
                    01
  0000000141482262: 41 B8 03 00 00 00  mov         r8d,3
  0000000141482268: 48 8B D3           mov         rdx,rbx
  000000014148226B: E8 54 EE 00 00     call        00000001414910C4
  0000000141482270: 85 C0              test        eax,eax
  0000000141482272: 0F 85 85 FE FF FF  jne         00000001414820FD
  0000000141482278: 48 83 C3 03        add         rbx,3
  000000014148227C: EB 03              jmp         0000000141482281
  000000014148227E: 49 03 DF           add         rbx,r15
  0000000141482281: 80 3B 20           cmp         byte ptr [rbx],20h
  0000000141482284: 74 F8              je          000000014148227E
  0000000141482286: 80 3B 3D           cmp         byte ptr [rbx],3Dh
  0000000141482289: 0F 85 6E FE FF FF  jne         00000001414820FD
  000000014148228F: 49 03 DF           add         rbx,r15
  0000000141482292: 80 3B 20           cmp         byte ptr [rbx],20h
  0000000141482295: 74 F8              je          000000014148228F
  0000000141482297: 48 8D 15 26 44 A8  lea         rdx,[0000000142F066C4h]
                    01
  000000014148229E: 41 B8 05 00 00 00  mov         r8d,5
  00000001414822A4: 48 8B CB           mov         rcx,rbx
  00000001414822A7: E8 A8 EC 00 00     call        0000000141490F54
  00000001414822AC: 85 C0              test        eax,eax
  00000001414822AE: 75 0A              jne         00000001414822BA
  00000001414822B0: 48 83 C3 05        add         rbx,5
  00000001414822B4: 0F BA EF 12        bts         edi,12h
  00000001414822B8: EB 4D              jmp         0000000141482307
  00000001414822BA: 48 8D 15 0F 44 A8  lea         rdx,[0000000142F066D0h]
                    01
  00000001414822C1: 41 B8 08 00 00 00  mov         r8d,8
  00000001414822C7: 48 8B CB           mov         rcx,rbx
  00000001414822CA: E8 85 EC 00 00     call        0000000141490F54
  00000001414822CF: 85 C0              test        eax,eax
  00000001414822D1: 75 0A              jne         00000001414822DD
  00000001414822D3: 48 83 C3 08        add         rbx,8
  00000001414822D7: 0F BA EF 11        bts         edi,11h
  00000001414822DB: EB 2A              jmp         0000000141482307
  00000001414822DD: 48 8D 15 FC 43 A8  lea         rdx,[0000000142F066E0h]
                    01
  00000001414822E4: 41 B8 07 00 00 00  mov         r8d,7
  00000001414822EA: 48 8B CB           mov         rcx,rbx
  00000001414822ED: E8 62 EC 00 00     call        0000000141490F54
  00000001414822F2: 85 C0              test        eax,eax
  00000001414822F4: 0F 85 03 FE FF FF  jne         00000001414820FD
  00000001414822FA: 48 83 C3 07        add         rbx,7
  00000001414822FE: 0F BA EF 10        bts         edi,10h
  0000000141482302: EB 03              jmp         0000000141482307
  0000000141482304: 49 03 DF           add         rbx,r15
  0000000141482307: 80 3B 20           cmp         byte ptr [rbx],20h
  000000014148230A: 74 F8              je          0000000141482304
  000000014148230C: 80 3B 00           cmp         byte ptr [rbx],0
  000000014148230F: 0F 85 E8 FD FF FF  jne         00000001414820FD
  0000000141482315: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  000000014148231A: 45 8B CC           mov         r9d,r12d
  000000014148231D: 44 8B C7           mov         r8d,edi
  0000000141482320: 49 8B D5           mov         rdx,r13
  0000000141482323: C7 44 24 20 80 01  mov         dword ptr [rsp+20h],180h
                    00 00
  000000014148232B: E8 BC 60 00 00     call        00000001414883EC
  0000000141482330: 85 C0              test        eax,eax
  0000000141482332: 0F 85 E5 FD FF FF  jne         000000014148211D
  0000000141482338: 44 01 3D 81 D7 27  add         dword ptr [00000001446FFAC0h],r15d
                    03
  000000014148233F: 21 46 08           and         dword ptr [rsi+8],eax
  0000000141482342: 8B 44 24 68        mov         eax,dword ptr [rsp+68h]
  0000000141482346: 48 83 26 00        and         qword ptr [rsi],0
  000000014148234A: 48 83 66 10 00     and         qword ptr [rsi+10h],0
  000000014148234F: 48 83 66 28 00     and         qword ptr [rsi+28h],0
  0000000141482354: 89 46 1C           mov         dword ptr [rsi+1Ch],eax
  0000000141482357: 89 6E 18           mov         dword ptr [rsi+18h],ebp
  000000014148235A: 48 8B C6           mov         rax,rsi
  000000014148235D: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  0000000141482362: 48 8B 6C 24 70     mov         rbp,qword ptr [rsp+70h]
  0000000141482367: 48 83 C4 30        add         rsp,30h
  000000014148236B: 41 5F              pop         r15
  000000014148236D: 41 5D              pop         r13
  000000014148236F: 41 5C              pop         r12
  0000000141482371: 5F                 pop         rdi
  0000000141482372: 5E                 pop         rsi
  0000000141482373: C3                 ret
  0000000141482374: 48

  Summary

     1E02000 .text


; sopen wrapper; bounded range 1414883ec..14148841e
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414883EC: 48 83 EC 38        sub         rsp,38h
  00000001414883F0: 41 8B C1           mov         eax,r9d
  00000001414883F3: 44 8B 4C 24 60     mov         r9d,dword ptr [rsp+60h]
  00000001414883F8: 45 8B D0           mov         r10d,r8d
  00000001414883FB: 4C 8B DA           mov         r11,rdx
  00000001414883FE: C7 44 24 28 01 00  mov         dword ptr [rsp+28h],1
                    00 00
  0000000141488406: 48 89 4C 24 20     mov         qword ptr [rsp+20h],rcx
  000000014148840B: 44 8B C0           mov         r8d,eax
  000000014148840E: 41 8B D2           mov         edx,r10d
  0000000141488411: 49 8B CB           mov         rcx,r11
  0000000141488414: E8 9B FE FF FF     call        00000001414882B4
  0000000141488419: 48 83 C4 38        add         rsp,38h
  000000014148841D: C3                 ret
  000000014148841E: CC                 int         3

  Summary

     1E02000 .text


; sopen dispatcher; bounded range 1414882b4..1414883ea
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414882B4: 48 89 74 24 08     mov         qword ptr [rsp+8],rsi
  00000001414882B9: 57                 push        rdi
  00000001414882BA: 48 83 EC 50        sub         rsp,50h
  00000001414882BE: 45 8B D0           mov         r10d,r8d
  00000001414882C1: 4C 8B C1           mov         r8,rcx
  00000001414882C4: 83 64 24 40 00     and         dword ptr [rsp+40h],0
  00000001414882C9: 33 C0              xor         eax,eax
  00000001414882CB: 48 8B B4 24 80 00  mov         rsi,qword ptr [rsp+0000000000000080h]
                    00 00
  00000001414882D3: 48 85 F6           test        rsi,rsi
  00000001414882D6: 0F 95 C0           setne       al
  00000001414882D9: 85 C0              test        eax,eax
  00000001414882DB: 75 28              jne         0000000141488305
  00000001414882DD: E8 52 BE FE FF     call        0000000141474134
  00000001414882E2: BF 16 00 00 00     mov         edi,16h
  00000001414882E7: 89 38              mov         dword ptr [rax],edi
  00000001414882E9: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  00000001414882EF: 45 33 C9           xor         r9d,r9d
  00000001414882F2: 45 33 C0           xor         r8d,r8d
  00000001414882F5: 33 D2              xor         edx,edx
  00000001414882F7: 33 C9              xor         ecx,ecx
  00000001414882F9: E8 6A 82 FE FF     call        0000000141470568
  00000001414882FE: 8B C7              mov         eax,edi
  0000000141488300: E9 DA 00 00 00     jmp         00000001414883DF
  0000000141488305: 83 0E FF           or          dword ptr [rsi],0FFFFFFFFh
  0000000141488308: 33 C0              xor         eax,eax
  000000014148830A: 48 85 C9           test        rcx,rcx
  000000014148830D: 0F 95 C0           setne       al
  0000000141488310: 85 C0              test        eax,eax
  0000000141488312: 75 28              jne         000000014148833C
  0000000141488314: E8 1B BE FE FF     call        0000000141474134
  0000000141488319: BF 16 00 00 00     mov         edi,16h
  000000014148831E: 89 38              mov         dword ptr [rax],edi
  0000000141488320: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  0000000141488326: 45 33 C9           xor         r9d,r9d
  0000000141488329: 45 33 C0           xor         r8d,r8d
  000000014148832C: 33 D2              xor         edx,edx
  000000014148832E: 33 C9              xor         ecx,ecx
  0000000141488330: E8 33 82 FE FF     call        0000000141470568
  0000000141488335: 8B C7              mov         eax,edi
  0000000141488337: E9 A3 00 00 00     jmp         00000001414883DF
  000000014148833C: 8B 8C 24 88 00 00  mov         ecx,dword ptr [rsp+0000000000000088h]
                    00
  0000000141488343: 85 C9              test        ecx,ecx
  0000000141488345: 74 38              je          000000014148837F
  0000000141488347: 41 F7 C1 7F FE FF  test        r9d,0FFFFFE7Fh
                    FF
  000000014148834E: B8 00 00 00 00     mov         eax,0
  0000000141488353: 0F 94 C0           sete        al
  0000000141488356: 85 C0              test        eax,eax
  0000000141488358: 75 25              jne         000000014148837F
  000000014148835A: E8 D5 BD FE FF     call        0000000141474134
  000000014148835F: BF 16 00 00 00     mov         edi,16h
  0000000141488364: 89 38              mov         dword ptr [rax],edi
  0000000141488366: 48 83 64 24 20 00  and         qword ptr [rsp+20h],0
  000000014148836C: 45 33 C9           xor         r9d,r9d
  000000014148836F: 45 33 C0           xor         r8d,r8d
  0000000141488372: 33 D2              xor         edx,edx
  0000000141488374: 33 C9              xor         ecx,ecx
  0000000141488376: E8 ED 81 FE FF     call        0000000141470568
  000000014148837B: 8B C7              mov         eax,edi
  000000014148837D: EB 60              jmp         00000001414883DF
  000000014148837F: 89 4C 24 30        mov         dword ptr [rsp+30h],ecx
  0000000141488383: 44 89 4C 24 28     mov         dword ptr [rsp+28h],r9d
  0000000141488388: 44 89 54 24 20     mov         dword ptr [rsp+20h],r10d
  000000014148838D: 44 8B CA           mov         r9d,edx
  0000000141488390: 48 8B D6           mov         rdx,rsi
  0000000141488393: 48 8D 4C 24 40     lea         rcx,[rsp+40h]
  0000000141488398: E8 27 F6 FF FF     call        00000001414879C4
  000000014148839D: 8B F8              mov         edi,eax
  000000014148839F: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  00000001414883A3: 83 7C 24 40 00     cmp         dword ptr [rsp+40h],0
  00000001414883A8: 74 2C              je          00000001414883D6
  00000001414883AA: 85 C0              test        eax,eax
  00000001414883AC: 74 21              je          00000001414883CF
  00000001414883AE: 48 63 0E           movsxd      rcx,dword ptr [rsi]
  00000001414883B1: 48 8B C1           mov         rax,rcx
  00000001414883B4: 48 C1 F8 05        sar         rax,5
  00000001414883B8: 4C 8D 05 41 BD BC  lea         r8,[0000000145054100h]
                    03
  00000001414883BF: 83 E1 1F           and         ecx,1Fh
  00000001414883C2: 48 6B C9 58        imul        rcx,rcx,58h
  00000001414883C6: 49 8B 04 C0        mov         rax,qword ptr [r8+rax*8]
  00000001414883CA: 80 64 08 08 FE     and         byte ptr [rax+rcx+8],0FEh
  00000001414883CF: 8B 0E              mov         ecx,dword ptr [rsi]
  00000001414883D1: E8 02 45 00 00     call        000000014148C8D8
  00000001414883D6: 85 FF              test        edi,edi
  00000001414883D8: 74 03              je          00000001414883DD
  00000001414883DA: 83 0E FF           or          dword ptr [rsi],0FFFFFFFFh
  00000001414883DD: 8B C7              mov         eax,edi
  00000001414883DF: 48 8B 74 24 60     mov         rsi,qword ptr [rsp+60h]
  00000001414883E4: 48 83 C4 50        add         rsp,50h
  00000001414883E8: 5F                 pop         rdi
  00000001414883E9: C3                 ret
  00000001414883EA: CC                 int         3

  Summary

     1E02000 .text


; sopen leaf -> CreateFileA; bounded range 1414879c4..1414881c2
Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001414879C4: 48 8B C4           mov         rax,rsp
  00000001414879C7: 48 89 58 10        mov         qword ptr [rax+10h],rbx
  00000001414879CB: 4C 89 40 18        mov         qword ptr [rax+18h],r8
  00000001414879CF: 48 89 48 08        mov         qword ptr [rax+8],rcx
  00000001414879D3: 55                 push        rbp
  00000001414879D4: 56                 push        rsi
  00000001414879D5: 57                 push        rdi
  00000001414879D6: 41 54              push        r12
  00000001414879D8: 41 55              push        r13
  00000001414879DA: 41 56              push        r14
  00000001414879DC: 41 57              push        r15
  00000001414879DE: 48 81 EC 80 00 00  sub         rsp,80h
                    00
  00000001414879E5: 33 FF              xor         edi,edi
  00000001414879E7: 41 B4 80           mov         r12b,80h
  00000001414879EA: 41 8B F1           mov         esi,r9d
  00000001414879ED: 48 8B DA           mov         rbx,rdx
  00000001414879F0: C7 40 A8 18 00 00  mov         dword ptr [rax-58h],18h
                    00
  00000001414879F7: 8D 6F 01           lea         ebp,[rdi+1]
  00000001414879FA: 89 78 9C           mov         dword ptr [rax-64h],edi
  00000001414879FD: 40 88 BC 24 D8 00  mov         byte ptr [rsp+00000000000000D8h],dil
                    00 00
  0000000141487A05: 48 89 78 B0        mov         qword ptr [rax-50h],rdi
  0000000141487A09: 45 84 CC           test        r12b,r9b
  0000000141487A0C: 74 08              je          0000000141487A16
  0000000141487A0E: 89 78 B8           mov         dword ptr [rax-48h],edi
  0000000141487A11: 41 B6 10           mov         r14b,10h
  0000000141487A14: EB 07              jmp         0000000141487A1D
  0000000141487A16: 89 6C 24 70        mov         dword ptr [rsp+70h],ebp
  0000000141487A1A: 44 8A F7           mov         r14b,dil
  0000000141487A1D: 48 8D 4C 24 54     lea         rcx,[rsp+54h]
  0000000141487A22: E8 E1 B1 00 00     call        0000000141492C08
  0000000141487A27: 3B C7              cmp         eax,edi
  0000000141487A29: 74 14              je          0000000141487A3F
  0000000141487A2B: 45 33 C9           xor         r9d,r9d
  0000000141487A2E: 45 33 C0           xor         r8d,r8d
  0000000141487A31: 33 D2              xor         edx,edx
  0000000141487A33: 33 C9              xor         ecx,ecx
  0000000141487A35: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  0000000141487A3A: E8 01 8A FE FF     call        0000000141470440
  0000000141487A3F: 0F BA E6 0F        bt          esi,0Fh
  0000000141487A43: 72 15              jb          0000000141487A5A
  0000000141487A45: F7 C6 00 40 07 00  test        esi,74000h
  0000000141487A4B: 75 0A              jne         0000000141487A57
  0000000141487A4D: 81 7C 24 54 00 80  cmp         dword ptr [rsp+54h],8000h
                    00 00
  0000000141487A55: 74 03              je          0000000141487A5A
  0000000141487A57: 45 0A F4           or          r14b,r12b
  0000000141487A5A: 8B CE              mov         ecx,esi
  0000000141487A5C: 41 B8 03 00 00 00  mov         r8d,3
  0000000141487A62: B8 00 00 00 C0     mov         eax,0C0000000h
  0000000141487A67: 41 23 C8           and         ecx,r8d
  0000000141487A6A: BA 00 00 00 80     mov         edx,80000000h
  0000000141487A6F: 3B CF              cmp         ecx,edi
  0000000141487A71: 74 54              je          0000000141487AC7
  0000000141487A73: 2B CD              sub         ecx,ebp
  0000000141487A75: 74 3A              je          0000000141487AB1
  0000000141487A77: 3B CD              cmp         ecx,ebp
  0000000141487A79: 74 31              je          0000000141487AAC
  0000000141487A7B: E8 D4 C6 FE FF     call        0000000141474154
  0000000141487A80: 89 38              mov         dword ptr [rax],edi
  0000000141487A82: 83 0B FF           or          dword ptr [rbx],0FFFFFFFFh
  0000000141487A85: E8 AA C6 FE FF     call        0000000141474134
  0000000141487A8A: BB 16 00 00 00     mov         ebx,16h
  0000000141487A8F: 45 33 C9           xor         r9d,r9d
  0000000141487A92: 45 33 C0           xor         r8d,r8d
  0000000141487A95: 33 D2              xor         edx,edx
  0000000141487A97: 33 C9              xor         ecx,ecx
  0000000141487A99: 48 89 7C 24 20     mov         qword ptr [rsp+20h],rdi
  0000000141487A9E: 89 18              mov         dword ptr [rax],ebx
  0000000141487AA0: E8 C3 8A FE FF     call        0000000141470568
  0000000141487AA5: 8B C3              mov         eax,ebx
  0000000141487AA7: E9 FB 06 00 00     jmp         00000001414881A7
  0000000141487AAC: 44 8B F8           mov         r15d,eax
  0000000141487AAF: EB 19              jmp         0000000141487ACA
  0000000141487AB1: 40 F6 C6 08        test        sil,8
  0000000141487AB5: 74 08              je          0000000141487ABF
  0000000141487AB7: F7 C6 00 00 07 00  test        esi,70000h
  0000000141487ABD: 75 ED              jne         0000000141487AAC
  0000000141487ABF: 41 BF 00 00 00 40  mov         r15d,40000000h
  0000000141487AC5: EB 03              jmp         0000000141487ACA
  0000000141487AC7: 44 8B FA           mov         r15d,edx
  0000000141487ACA: 8B 8C 24 E0 00 00  mov         ecx,dword ptr [rsp+00000000000000E0h]
                    00
  0000000141487AD1: 41 BC 02 00 00 00  mov         r12d,2
  0000000141487AD7: 83 E9 10           sub         ecx,10h
  0000000141487ADA: 74 2C              je          0000000141487B08
  0000000141487ADC: 83 E9 10           sub         ecx,10h
  0000000141487ADF: 74 23              je          0000000141487B04
  0000000141487AE1: 83 E9 10           sub         ecx,10h
  0000000141487AE4: 74 19              je          0000000141487AFF
  0000000141487AE6: 83 E9 10           sub         ecx,10h
  0000000141487AE9: 74 0F              je          0000000141487AFA
  0000000141487AEB: 83 F9 40           cmp         ecx,40h
  0000000141487AEE: 75 8B              jne         0000000141487A7B
  0000000141487AF0: 44 3B FA           cmp         r15d,edx
  0000000141487AF3: 8B CF              mov         ecx,edi
  0000000141487AF5: 0F 94 C1           sete        cl
  0000000141487AF8: EB 10              jmp         0000000141487B0A
  0000000141487AFA: 41 8B C8           mov         ecx,r8d
  0000000141487AFD: EB 0B              jmp         0000000141487B0A
  0000000141487AFF: 41 8B CC           mov         ecx,r12d
  0000000141487B02: EB 06              jmp         0000000141487B0A
  0000000141487B04: 8B CD              mov         ecx,ebp
  0000000141487B06: EB 02              jmp         0000000141487B0A
  0000000141487B08: 8B CF              mov         ecx,edi
  0000000141487B0A: 8B C6              mov         eax,esi
  0000000141487B0C: BA 00 07 00 00     mov         edx,700h
  0000000141487B11: 89 4C 24 48        mov         dword ptr [rsp+48h],ecx
  0000000141487B15: 23 C2              and         eax,edx
  0000000141487B17: 74 46              je          0000000141487B5F
  0000000141487B19: 3D 00 01 00 00     cmp         eax,100h
  0000000141487B1E: 74 38              je          0000000141487B58
  0000000141487B20: 3D 00 02 00 00     cmp         eax,200h
  0000000141487B25: 74 2A              je          0000000141487B51
  0000000141487B27: 3D 00 03 00 00     cmp         eax,300h
  0000000141487B2C: 74 1E              je          0000000141487B4C
  0000000141487B2E: 3D 00 04 00 00     cmp         eax,400h
  0000000141487B33: 74 2A              je          0000000141487B5F
  0000000141487B35: 3D 00 05 00 00     cmp         eax,500h
  0000000141487B3A: 74 26              je          0000000141487B62
  0000000141487B3C: 3D 00 06 00 00     cmp         eax,600h
  0000000141487B41: 74 0E              je          0000000141487B51
  0000000141487B43: 3B C2              cmp         eax,edx
  0000000141487B45: 74 1B              je          0000000141487B62
  0000000141487B47: E9 2F FF FF FF     jmp         0000000141487A7B
  0000000141487B4C: 41 8B EC           mov         ebp,r12d
  0000000141487B4F: EB 11              jmp         0000000141487B62
  0000000141487B51: BD 05 00 00 00     mov         ebp,5
  0000000141487B56: EB 0A              jmp         0000000141487B62
  0000000141487B58: BD 04 00 00 00     mov         ebp,4
  0000000141487B5D: EB 03              jmp         0000000141487B62
  0000000141487B5F: 41 8B E8           mov         ebp,r8d
  0000000141487B62: 0F BA E6 08        bt          esi,8
  0000000141487B66: 41 BD 80 00 00 00  mov         r13d,80h
  0000000141487B6C: 44 89 6C 24 50     mov         dword ptr [rsp+50h],r13d
  0000000141487B71: 73 1E              jae         0000000141487B91
  0000000141487B73: 8A 05 4F 7F 27 03  mov         al,byte ptr [00000001446FFAC8h]
  0000000141487B79: F6 D0              not         al
  0000000141487B7B: 22 84 24 E8 00 00  and         al,byte ptr [rsp+00000000000000E8h]
                    00
  0000000141487B82: A8 80              test        al,80h
  0000000141487B84: 41 8D 45 81        lea         eax,[r13-7Fh]
  0000000141487B88: 44 0F 44 E8        cmove       r13d,eax
  0000000141487B8C: 44 89 6C 24 50     mov         dword ptr [rsp+50h],r13d
  0000000141487B91: 40 F6 C6 40        test        sil,40h
  0000000141487B95: 74 16              je          0000000141487BAD
  0000000141487B97: 41 0F BA ED 1A     bts         r13d,1Ah
  0000000141487B9C: 41 0F BA EF 10     bts         r15d,10h
  0000000141487BA1: 83 C9 04           or          ecx,4
  0000000141487BA4: 44 89 6C 24 50     mov         dword ptr [rsp+50h],r13d
  0000000141487BA9: 89 4C 24 48        mov         dword ptr [rsp+48h],ecx
  0000000141487BAD: 0F BA E6 0C        bt          esi,0Ch
  0000000141487BB1: 73 0A              jae         0000000141487BBD
  0000000141487BB3: 41 0F BA ED 08     bts         r13d,8
  0000000141487BB8: 44 89 6C 24 50     mov         dword ptr [rsp+50h],r13d
  0000000141487BBD: 40 F6 C6 20        test        sil,20h
  0000000141487BC1: 74 07              je          0000000141487BCA
  0000000141487BC3: 41 0F BA ED 1B     bts         r13d,1Bh
  0000000141487BC8: EB 0B              jmp         0000000141487BD5
  0000000141487BCA: 40 F6 C6 10        test        sil,10h
  0000000141487BCE: 74 0A              je          0000000141487BDA
  0000000141487BD0: 41 0F BA ED 1C     bts         r13d,1Ch
  0000000141487BD5: 44 89 6C 24 50     mov         dword ptr [rsp+50h],r13d
  0000000141487BDA: E8 21 4D 00 00     call        000000014148C900
  0000000141487BDF: 89 03              mov         dword ptr [rbx],eax
  0000000141487BE1: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487BE4: 75 21              jne         0000000141487C07
  0000000141487BE6: E8 69 C5 FE FF     call        0000000141474154
  0000000141487BEB: 89 38              mov         dword ptr [rax],edi
  0000000141487BED: 83 0B FF           or          dword ptr [rbx],0FFFFFFFFh
  0000000141487BF0: E8 3F C5 FE FF     call        0000000141474134
  0000000141487BF5: C7 00 18 00 00 00  mov         dword ptr [rax],18h
  0000000141487BFB: E8 34 C5 FE FF     call        0000000141474134
  0000000141487C00: 8B 00              mov         eax,dword ptr [rax]
  0000000141487C02: E9 A0 05 00 00     jmp         00000001414881A7
  0000000141487C07: 48 8B 84 24 C0 00  mov         rax,qword ptr [rsp+00000000000000C0h]
                    00 00
  0000000141487C0F: 44 8B 44 24 48     mov         r8d,dword ptr [rsp+48h]
  0000000141487C14: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000141487C1C: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  0000000141487C21: 4C 8D 4C 24 60     lea         r9,[rsp+60h]
  0000000141487C26: 41 8B D7           mov         edx,r15d
  0000000141487C29: 44 89 6C 24 28     mov         dword ptr [rsp+28h],r13d
  0000000141487C2E: C7 00 01 00 00 00  mov         dword ptr [rax],1
  0000000141487C34: 89 6C 24 20        mov         dword ptr [rsp+20h],ebp
  0000000141487C38: FF 15 D2 B5 97 00  call        qword ptr [0000000141E03210h]
  0000000141487C3E: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000141487C43: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  0000000141487C47: 0F 85 8C 00 00 00  jne         0000000141487CD9
  0000000141487C4D: B9 00 00 00 C0     mov         ecx,0C0000000h
  0000000141487C52: 41 8B C7           mov         eax,r15d
  0000000141487C55: 23 C1              and         eax,ecx
  0000000141487C57: 3B C1              cmp         eax,ecx
  0000000141487C59: 75 43              jne         0000000141487C9E
  0000000141487C5B: B8 01 00 00 00     mov         eax,1
  0000000141487C60: 40 84 F0           test        al,sil
  0000000141487C63: 74 39              je          0000000141487C9E
  0000000141487C65: 44 8B 44 24 48     mov         r8d,dword ptr [rsp+48h]
  0000000141487C6A: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  0000000141487C72: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  0000000141487C77: 41 0F BA F7 1F     btr         r15d,1Fh
  0000000141487C7C: 4C 8D 4C 24 60     lea         r9,[rsp+60h]
  0000000141487C81: 44 89 6C 24 28     mov         dword ptr [rsp+28h],r13d
  0000000141487C86: 41 8B D7           mov         edx,r15d
  0000000141487C89: 89 6C 24 20        mov         dword ptr [rsp+20h],ebp
  0000000141487C8D: FF 15 7D B5 97 00  call        qword ptr [0000000141E03210h]
  0000000141487C93: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000141487C98: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  0000000141487C9C: 75 3B              jne         0000000141487CD9
  0000000141487C9E: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  0000000141487CA1: 4C 8D 2D 58 C4 BC  lea         r13,[0000000145054100h]
                    03
  0000000141487CA8: 48 8B C1           mov         rax,rcx
  0000000141487CAB: 83 E1 1F           and         ecx,1Fh
  0000000141487CAE: 48 C1 F8 05        sar         rax,5
  0000000141487CB2: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141487CB6: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  0000000141487CBB: 80 64 08 08 FE     and         byte ptr [rax+rcx+8],0FEh
  0000000141487CC0: FF 15 22 B9 97 00  call        qword ptr [0000000141E035E8h]
  0000000141487CC6: 8B C8              mov         ecx,eax
  0000000141487CC8: E8 A7 C4 FE FF     call        0000000141474174
  0000000141487CCD: E8 62 C4 FE FF     call        0000000141474134
  0000000141487CD2: 8B 38              mov         edi,dword ptr [rax]
  0000000141487CD4: E9 CC 04 00 00     jmp         00000001414881A5
  0000000141487CD9: 48 8B C8           mov         rcx,rax
  0000000141487CDC: FF 15 FE B6 97 00  call        qword ptr [0000000141E033E0h]
  0000000141487CE2: 3B C7              cmp         eax,edi
  0000000141487CE4: 75 4D              jne         0000000141487D33
  0000000141487CE6: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  0000000141487CE9: 4C 8D 2D 10 C4 BC  lea         r13,[0000000145054100h]
                    03
  0000000141487CF0: 48 8B C1           mov         rax,rcx
  0000000141487CF3: 83 E1 1F           and         ecx,1Fh
  0000000141487CF6: 48 C1 F8 05        sar         rax,5
  0000000141487CFA: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141487CFE: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  0000000141487D03: 80 64 08 08 FE     and         byte ptr [rax+rcx+8],0FEh
  0000000141487D08: FF 15 DA B8 97 00  call        qword ptr [0000000141E035E8h]
  0000000141487D0E: 8B C8              mov         ecx,eax
  0000000141487D10: 8B D8              mov         ebx,eax
  0000000141487D12: E8 5D C4 FE FF     call        0000000141474174
  0000000141487D17: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000141487D1C: FF 15 9E B8 97 00  call        qword ptr [0000000141E035C0h]
  0000000141487D22: 3B DF              cmp         ebx,edi
  0000000141487D24: 75 A7              jne         0000000141487CCD
  0000000141487D26: E8 09 C4 FE FF     call        0000000141474134
  0000000141487D2B: C7 00 0D 00 00 00  mov         dword ptr [rax],0Dh
  0000000141487D31: EB 9A              jmp         0000000141487CCD
  0000000141487D33: 41 3B C4           cmp         eax,r12d
  0000000141487D36: 75 06              jne         0000000141487D3E
  0000000141487D38: 41 80 CE 40        or          r14b,40h
  0000000141487D3C: EB 09              jmp         0000000141487D47
  0000000141487D3E: 83 F8 03           cmp         eax,3
  0000000141487D41: 75 04              jne         0000000141487D47
  0000000141487D43: 41 80 CE 08        or          r14b,8
  0000000141487D47: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000141487D4C: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487D4E: E8 FD 48 00 00     call        000000014148C650
  0000000141487D53: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  0000000141487D56: 4C 8D 2D A3 C3 BC  lea         r13,[0000000145054100h]
                    03
  0000000141487D5D: 48 8B C1           mov         rax,rcx
  0000000141487D60: 83 E1 1F           and         ecx,1Fh
  0000000141487D63: BA 01 00 00 00     mov         edx,1
  0000000141487D68: 48 C1 F8 05        sar         rax,5
  0000000141487D6C: 44 0A F2           or          r14b,dl
  0000000141487D6F: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  0000000141487D74: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141487D78: 44 88 74 08 08     mov         byte ptr [rax+rcx+8],r14b
  0000000141487D7D: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  0000000141487D80: 48 8B C1           mov         rax,rcx
  0000000141487D83: 83 E1 1F           and         ecx,1Fh
  0000000141487D86: 48 C1 F8 05        sar         rax,5
  0000000141487D8A: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141487D8E: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  0000000141487D93: 80 64 08 38 80     and         byte ptr [rax+rcx+38h],80h
  0000000141487D98: 41 8A C6           mov         al,r14b
  0000000141487D9B: 24 48              and         al,48h
  0000000141487D9D: 88 44 24 4C        mov         byte ptr [rsp+4Ch],al
  0000000141487DA1: 0F 85 86 00 00 00  jne         0000000141487E2D
  0000000141487DA7: 45 84 F6           test        r14b,r14b
  0000000141487DAA: 0F 89 C2 02 00 00  jns         0000000141488072
  0000000141487DB0: 41 84 F4           test        r12b,sil
  0000000141487DB3: 74 78              je          0000000141487E2D
  0000000141487DB5: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487DB7: 45 8B C4           mov         r8d,r12d
  0000000141487DBA: 83 CA FF           or          edx,0FFFFFFFFh
  0000000141487DBD: E8 62 B5 FF FF     call        0000000141483324
  0000000141487DC2: 89 44 24 44        mov         dword ptr [rsp+44h],eax
  0000000141487DC6: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487DC9: 75 19              jne         0000000141487DE4
  0000000141487DCB: E8 84 C3 FE FF     call        0000000141474154
  0000000141487DD0: 81 38 83 00 00 00  cmp         dword ptr [rax],83h
  0000000141487DD6: 74 50              je          0000000141487E28
  0000000141487DD8: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487DDA: E8 D5 20 FF FF     call        0000000141479EB4
  0000000141487DDF: E9 E9 FE FF FF     jmp         0000000141487CCD
  0000000141487DE4: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487DE6: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  0000000141487DEB: 41 B8 01 00 00 00  mov         r8d,1
  0000000141487DF1: 40 88 7C 24 40     mov         byte ptr [rsp+40h],dil
  0000000141487DF6: E8 FD 15 00 00     call        00000001414893F8
  0000000141487DFB: 3B C7              cmp         eax,edi
  0000000141487DFD: 75 18              jne         0000000141487E17
  0000000141487DFF: 80 7C 24 40 1A     cmp         byte ptr [rsp+40h],1Ah
  0000000141487E04: 75 11              jne         0000000141487E17
  0000000141487E06: 48 63 54 24 44     movsxd      rdx,dword ptr [rsp+44h]
  0000000141487E0B: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487E0D: E8 8E AB 00 00     call        00000001414929A0
  0000000141487E12: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487E15: 74 C1              je          0000000141487DD8
  0000000141487E17: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487E19: 45 33 C0           xor         r8d,r8d
  0000000141487E1C: 33 D2              xor         edx,edx
  0000000141487E1E: E8 01 B5 FF FF     call        0000000141483324
  0000000141487E23: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487E26: 74 B0              je          0000000141487DD8
  0000000141487E28: BA 01 00 00 00     mov         edx,1
  0000000141487E2D: 45 84 F6           test        r14b,r14b
  0000000141487E30: 0F 89 3C 02 00 00  jns         0000000141488072
  0000000141487E36: B9 00 40 07 00     mov         ecx,74000h
  0000000141487E3B: 85 F1              test        ecx,esi
  0000000141487E3D: 75 10              jne         0000000141487E4F
  0000000141487E3F: 8B 44 24 54        mov         eax,dword ptr [rsp+54h]
  0000000141487E43: 23 C1              and         eax,ecx
  0000000141487E45: 75 06              jne         0000000141487E4D
  0000000141487E47: 0F BA EE 0E        bts         esi,0Eh
  0000000141487E4B: EB 02              jmp         0000000141487E4F
  0000000141487E4D: 0B F0              or          esi,eax
  0000000141487E4F: 8B C6              mov         eax,esi
  0000000141487E51: 23 C1              and         eax,ecx
  0000000141487E53: 3D 00 40 00 00     cmp         eax,4000h
  0000000141487E58: 74 4A              je          0000000141487EA4
  0000000141487E5A: 3D 00 00 01 00     cmp         eax,10000h
  0000000141487E5F: 74 2C              je          0000000141487E8D
  0000000141487E61: 3D 00 40 01 00     cmp         eax,14000h
  0000000141487E66: 74 25              je          0000000141487E8D
  0000000141487E68: 3D 00 00 02 00     cmp         eax,20000h
  0000000141487E6D: 74 2B              je          0000000141487E9A
  0000000141487E6F: 3D 00 40 02 00     cmp         eax,24000h
  0000000141487E74: 74 24              je          0000000141487E9A
  0000000141487E76: 3D 00 00 04 00     cmp         eax,40000h
  0000000141487E7B: 74 07              je          0000000141487E84
  0000000141487E7D: 3D 00 40 04 00     cmp         eax,44000h
  0000000141487E82: 75 28              jne         0000000141487EAC
  0000000141487E84: 88 94 24 D8 00 00  mov         byte ptr [rsp+00000000000000D8h],dl
                    00
  0000000141487E8B: EB 1F              jmp         0000000141487EAC
  0000000141487E8D: B9 01 03 00 00     mov         ecx,301h
  0000000141487E92: 8B C6              mov         eax,esi
  0000000141487E94: 23 C1              and         eax,ecx
  0000000141487E96: 3B C1              cmp         eax,ecx
  0000000141487E98: 75 12              jne         0000000141487EAC
  0000000141487E9A: 44 88 A4 24 D8 00  mov         byte ptr [rsp+00000000000000D8h],r12b
                    00 00
  0000000141487EA2: EB 08              jmp         0000000141487EAC
  0000000141487EA4: 40 88 BC 24 D8 00  mov         byte ptr [rsp+00000000000000D8h],dil
                    00 00
  0000000141487EAC: F7 C6 00 00 07 00  test        esi,70000h
  0000000141487EB2: 0F 84 BA 01 00 00  je          0000000141488072
  0000000141487EB8: 89 7C 24 44        mov         dword ptr [rsp+44h],edi
  0000000141487EBC: 41 F6 C6 40        test        r14b,40h
  0000000141487EC0: 0F 85 AC 01 00 00  jne         0000000141488072
  0000000141487EC6: 41 8B C7           mov         eax,r15d
  0000000141487EC9: 25 00 00 00 C0     and         eax,0C0000000h
  0000000141487ECE: 3D 00 00 00 40     cmp         eax,40000000h
  0000000141487ED3: 0F 84 24 01 00 00  je          0000000141487FFD
  0000000141487ED9: 3D 00 00 00 80     cmp         eax,80000000h
  0000000141487EDE: 74 7C              je          0000000141487F5C
  0000000141487EE0: 3D 00 00 00 C0     cmp         eax,0C0000000h
  0000000141487EE5: 0F 85 87 01 00 00  jne         0000000141488072
  0000000141487EEB: 3B EF              cmp         ebp,edi
  0000000141487EED: 0F 86 7F 01 00 00  jbe         0000000141488072
  0000000141487EF3: 41 3B EC           cmp         ebp,r12d
  0000000141487EF6: 76 0E              jbe         0000000141487F06
  0000000141487EF8: 83 FD 04           cmp         ebp,4
  0000000141487EFB: 76 38              jbe         0000000141487F35
  0000000141487EFD: 83 FD 05           cmp         ebp,5
  0000000141487F00: 0F 85 6C 01 00 00  jne         0000000141488072
  0000000141487F06: 0F BE 8C 24 D8 00  movsx       ecx,byte ptr [rsp+00000000000000D8h]
                    00 00
  0000000141487F0E: 41 B8 01 00 00 00  mov         r8d,1
  0000000141487F14: 8B EF              mov         ebp,edi
  0000000141487F16: 41 2B C8           sub         ecx,r8d
  0000000141487F19: 0F 84 20 01 00 00  je          000000014148803F
  0000000141487F1F: 41 3B C8           cmp         ecx,r8d
  0000000141487F22: 0F 85 50 01 00 00  jne         0000000141488078
  0000000141487F28: C7 44 24 44 FF FE  mov         dword ptr [rsp+44h],0FEFFh
                    00 00
  0000000141487F30: E9 18 01 00 00     jmp         000000014148804D
  0000000141487F35: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487F37: 45 8B C4           mov         r8d,r12d
  0000000141487F3A: 33 D2              xor         edx,edx
  0000000141487F3C: E8 1F 83 00 00     call        0000000141490260
  0000000141487F41: 48 3B C7           cmp         rax,rdi
  0000000141487F44: 74 C0              je          0000000141487F06
  0000000141487F46: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487F48: 45 33 C0           xor         r8d,r8d
  0000000141487F4B: 33 D2              xor         edx,edx
  0000000141487F4D: E8 0E 83 00 00     call        0000000141490260
  0000000141487F52: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  0000000141487F56: 0F 84 7C FE FF FF  je          0000000141487DD8
  0000000141487F5C: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487F5E: 48 8D 54 24 44     lea         rdx,[rsp+44h]
  0000000141487F63: 41 B8 03 00 00 00  mov         r8d,3
  0000000141487F69: E8 8A 14 00 00     call        00000001414893F8
  0000000141487F6E: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487F71: 0F 84 61 FE FF FF  je          0000000141487DD8
  0000000141487F77: 41 3B C4           cmp         eax,r12d
  0000000141487F7A: 74 20              je          0000000141487F9C
  0000000141487F7C: 83 F8 03           cmp         eax,3
  0000000141487F7F: 75 6B              jne         0000000141487FEC
  0000000141487F81: 81 7C 24 44 EF BB  cmp         dword ptr [rsp+44h],0BFBBEFh
                    BF 00
  0000000141487F89: 75 11              jne         0000000141487F9C
  0000000141487F8B: 44 8D 40 FE        lea         r8d,[rax-2]
  0000000141487F8F: 44 88 84 24 D8 00  mov         byte ptr [rsp+00000000000000D8h],r8b
                    00 00
  0000000141487F97: E9 DC 00 00 00     jmp         0000000141488078
  0000000141487F9C: 0F B7 44 24 44     movzx       eax,word ptr [rsp+44h]
  0000000141487FA1: 3D FE FF 00 00     cmp         eax,0FFFEh
  0000000141487FA6: 75 1A              jne         0000000141487FC2
  0000000141487FA8: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487FAA: E8 05 1F FF FF     call        0000000141479EB4
  0000000141487FAF: E8 80 C1 FE FF     call        0000000141474134
  0000000141487FB4: BB 16 00 00 00     mov         ebx,16h
  0000000141487FB9: 89 18              mov         dword ptr [rax],ebx
  0000000141487FBB: 8B FB              mov         edi,ebx
  0000000141487FBD: E9 E3 01 00 00     jmp         00000001414881A5
  0000000141487FC2: 3D FF FE 00 00     cmp         eax,0FEFFh
  0000000141487FC7: 75 23              jne         0000000141487FEC
  0000000141487FC9: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487FCB: 45 33 C0           xor         r8d,r8d
  0000000141487FCE: 41 8B D4           mov         edx,r12d
  0000000141487FD1: E8 4E B3 FF FF     call        0000000141483324
  0000000141487FD6: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487FD9: 0F 84 F9 FD FF FF  je          0000000141487DD8
  0000000141487FDF: 44 88 A4 24 D8 00  mov         byte ptr [rsp+00000000000000D8h],r12b
                    00 00
  0000000141487FE7: E9 86 00 00 00     jmp         0000000141488072
  0000000141487FEC: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141487FEE: 45 33 C0           xor         r8d,r8d
  0000000141487FF1: 33 D2              xor         edx,edx
  0000000141487FF3: E8 2C B3 FF FF     call        0000000141483324
  0000000141487FF8: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141487FFB: EB 3B              jmp         0000000141488038
  0000000141487FFD: 3B EF              cmp         ebp,edi
  0000000141487FFF: 76 71              jbe         0000000141488072
  0000000141488001: 41 3B EC           cmp         ebp,r12d
  0000000141488004: 0F 86 FC FE FF FF  jbe         0000000141487F06
  000000014148800A: 83 FD 04           cmp         ebp,4
  000000014148800D: 0F 87 EA FE FF FF  ja          0000000141487EFD
  0000000141488013: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141488015: 45 8B C4           mov         r8d,r12d
  0000000141488018: 33 D2              xor         edx,edx
  000000014148801A: E8 41 82 00 00     call        0000000141490260
  000000014148801F: 48 3B C7           cmp         rax,rdi
  0000000141488022: 0F 84 DE FE FF FF  je          0000000141487F06
  0000000141488028: 8B 0B              mov         ecx,dword ptr [rbx]
  000000014148802A: 45 33 C0           xor         r8d,r8d
  000000014148802D: 33 D2              xor         edx,edx
  000000014148802F: E8 2C 82 00 00     call        0000000141490260
  0000000141488034: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  0000000141488038: 75 38              jne         0000000141488072
  000000014148803A: E9 99 FD FF FF     jmp         0000000141487DD8
  000000014148803F: C7 44 24 44 EF BB  mov         dword ptr [rsp+44h],0BFBBEFh
                    BF 00
  0000000141488047: 41 BC 03 00 00 00  mov         r12d,3
  000000014148804D: 8B 0B              mov         ecx,dword ptr [rbx]
  000000014148804F: 48 63 C5           movsxd      rax,ebp
  0000000141488052: 45 8B C4           mov         r8d,r12d
  0000000141488055: 48 8D 54 04 44     lea         rdx,[rsp+rax+44h]
  000000014148805A: 44 2B C5           sub         r8d,ebp
  000000014148805D: E8 46 D4 FF FF     call        00000001414854A8
  0000000141488062: 83 F8 FF           cmp         eax,0FFFFFFFFh
  0000000141488065: 0F 84 6D FD FF FF  je          0000000141487DD8
  000000014148806B: 03 E8              add         ebp,eax
  000000014148806D: 44 3B E5           cmp         r12d,ebp
  0000000141488070: 7F DB              jg          000000014148804D
  0000000141488072: 41 B8 01 00 00 00  mov         r8d,1
  0000000141488078: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  000000014148807B: 40 8A AC 24 D8 00  mov         bpl,byte ptr [rsp+00000000000000D8h]
                    00 00
  0000000141488083: 48 8B C1           mov         rax,rcx
  0000000141488086: 83 E1 1F           and         ecx,1Fh
  0000000141488089: 40 80 E5 7F        and         bpl,7Fh
  000000014148808D: 48 C1 F8 05        sar         rax,5
  0000000141488091: 48 6B C9 58        imul        rcx,rcx,58h
  0000000141488095: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  000000014148809A: 80 64 08 38 80     and         byte ptr [rax+rcx+38h],80h
  000000014148809F: 40 08 6C 08 38     or          byte ptr [rax+rcx+38h],bpl
  00000001414880A4: 48 63 13           movsxd      rdx,dword ptr [rbx]
  00000001414880A7: 48 8B C2           mov         rax,rdx
  00000001414880AA: 83 E2 1F           and         edx,1Fh
  00000001414880AD: 48 C1 F8 05        sar         rax,5
  00000001414880B1: 48 6B D2 58        imul        rdx,rdx,58h
  00000001414880B5: 49 8B 4C C5 00     mov         rcx,qword ptr [r13+rax*8]
  00000001414880BA: 8B C6              mov         eax,esi
  00000001414880BC: 80 64 11 38 7F     and         byte ptr [rcx+rdx+38h],7Fh
  00000001414880C1: C1 E8 10           shr         eax,10h
  00000001414880C4: C0 E0 07           shl         al,7
  00000001414880C7: 08 44 11 38        or          byte ptr [rcx+rdx+38h],al
  00000001414880CB: 40 38 7C 24 4C     cmp         byte ptr [rsp+4Ch],dil
  00000001414880D0: 75 21              jne         00000001414880F3
  00000001414880D2: 40 F6 C6 08        test        sil,8
  00000001414880D6: 74 1B              je          00000001414880F3
  00000001414880D8: 48 63 0B           movsxd      rcx,dword ptr [rbx]
  00000001414880DB: 48 8B C1           mov         rax,rcx
  00000001414880DE: 83 E1 1F           and         ecx,1Fh
  00000001414880E1: 48 C1 F8 05        sar         rax,5
  00000001414880E5: 48 6B C9 58        imul        rcx,rcx,58h
  00000001414880E9: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  00000001414880EE: 80 4C 08 08 20     or          byte ptr [rax+rcx+8],20h
  00000001414880F3: 41 8B C7           mov         eax,r15d
  00000001414880F6: 25 00 00 00 C0     and         eax,0C0000000h
  00000001414880FB: 3D 00 00 00 C0     cmp         eax,0C0000000h
  0000000141488100: 0F 85 9F 00 00 00  jne         00000001414881A5
  0000000141488106: 41 84 F0           test        r8b,sil
  0000000141488109: 0F 84 96 00 00 00  je          00000001414881A5
  000000014148810F: 48 8B 4C 24 58     mov         rcx,qword ptr [rsp+58h]
  0000000141488114: FF 15 A6 B4 97 00  call        qword ptr [0000000141E035C0h]
  000000014148811A: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  000000014148811E: 44 8B 44 24 48     mov         r8d,dword ptr [rsp+48h]
  0000000141488123: 48 8B 8C 24 D0 00  mov         rcx,qword ptr [rsp+00000000000000D0h]
                    00 00
  000000014148812B: 48 89 7C 24 30     mov         qword ptr [rsp+30h],rdi
  0000000141488130: 89 44 24 28        mov         dword ptr [rsp+28h],eax
  0000000141488134: 41 0F BA F7 1F     btr         r15d,1Fh
  0000000141488139: 4C 8D 4C 24 60     lea         r9,[rsp+60h]
  000000014148813E: C7 44 24 20 03 00  mov         dword ptr [rsp+20h],3
                    00 00
  0000000141488146: 41 8B D7           mov         edx,r15d
  0000000141488149: FF 15 C1 B0 97 00  call        qword ptr [0000000141E03210h]
  000000014148814F: 48 83 F8 FF        cmp         rax,0FFFFFFFFFFFFFFFFh
  0000000141488153: 75 36              jne         000000014148818B
  0000000141488155: FF 15 8D B4 97 00  call        qword ptr [0000000141E035E8h]
  000000014148815B: 8B C8              mov         ecx,eax
  000000014148815D: E8 12 C0 FE FF     call        0000000141474174
  0000000141488162: 4C 63 1B           movsxd      r11,dword ptr [rbx]
  0000000141488165: 49 8B C3           mov         rax,r11
  0000000141488168: 41 83 E3 1F        and         r11d,1Fh
  000000014148816C: 48 C1 F8 05        sar         rax,5
  0000000141488170: 4D 6B DB 58        imul        r11,r11,58h
  0000000141488174: 49 8B 44 C5 00     mov         rax,qword ptr [r13+rax*8]
  0000000141488179: 42 80 64 18 08 FE  and         byte ptr [rax+r11+8],0FEh
  000000014148817F: 8B 0B              mov         ecx,dword ptr [rbx]
  0000000141488181: E8 7A 45 00 00     call        000000014148C700
  0000000141488186: E9 42 FB FF FF     jmp         0000000141487CCD
  000000014148818B: 48 63 13           movsxd      rdx,dword ptr [rbx]
  000000014148818E: 48 8B CA           mov         rcx,rdx
  0000000141488191: 83 E2 1F           and         edx,1Fh
  0000000141488194: 48 C1 F9 05        sar         rcx,5
  0000000141488198: 48 6B D2 58        imul        rdx,rdx,58h
  000000014148819C: 49 8B 4C CD 00     mov         rcx,qword ptr [r13+rcx*8]
  00000001414881A1: 48 89 04 0A        mov         qword ptr [rdx+rcx],rax
  00000001414881A5: 8B C7              mov         eax,edi
  00000001414881A7: 48 8B 9C 24 C8 00  mov         rbx,qword ptr [rsp+00000000000000C8h]
                    00 00
  00000001414881AF: 48 81 C4 80 00 00  add         rsp,80h
                    00
  00000001414881B6: 41 5F              pop         r15
  00000001414881B8: 41 5E              pop         r14
  00000001414881BA: 41 5D              pop         r13
  00000001414881BC: 41 5C              pop         r12
  00000001414881BE: 5F                 pop         rdi
  00000001414881BF: 5E                 pop         rsi
  00000001414881C0: 5D                 pop         rbp
  00000001414881C1: C3                 ret
  00000001414881C2: CC                 int         3

  Summary

     1E02000 .text


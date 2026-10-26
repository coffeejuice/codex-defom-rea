Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006537AF60: 40 53              push        rbx
  000000006537AF62: 55                 push        rbp
  000000006537AF63: 56                 push        rsi
  000000006537AF64: 57                 push        rdi
  000000006537AF65: 41 54              push        r12
  000000006537AF67: 41 55              push        r13
  000000006537AF69: 41 56              push        r14
  000000006537AF6B: 41 57              push        r15
  000000006537AF6D: 48 83 EC 68        sub         rsp,68h
  000000006537AF71: 48 C7 44 24 48 FE  mov         qword ptr [rsp+48h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006537AF7A: 48 8B F1           mov         rsi,rcx
  000000006537AF7D: 33 DB              xor         ebx,ebx
  000000006537AF7F: 89 9C 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],ebx
                    00
  000000006537AF86: 4C 8B 71 08        mov         r14,qword ptr [rcx+8]
  000000006537AF8A: 39 99 40 02 00 00  cmp         dword ptr [rcx+0000000000000240h],ebx
  000000006537AF90: 0F 84 E3 02 00 00  je          000000006537B279
  000000006537AF96: 48 8D 91 10 02 00  lea         rdx,[rcx+0000000000000210h]
                    00
  000000006537AF9D: 48 81 C1 00 02 00  add         rcx,200h
                    00
  000000006537AFA4: FF 15 7E D3 31 00  call        qword ptr [0000000065698328h]
  000000006537AFAA: 84 C0              test        al,al
  000000006537AFAC: 74 14              je          000000006537AFC2
  000000006537AFAE: 48 8D BE F8 01 00  lea         rdi,[rsi+00000000000001F8h]
                    00
  000000006537AFB5: 48 8B CF           mov         rcx,rdi
  000000006537AFB8: FF 15 2A BA 31 00  call        qword ptr [00000000656969E8h]
  000000006537AFBE: 84 C0              test        al,al
  000000006537AFC0: 74 66              je          000000006537B028
  000000006537AFC2: 48 8B 06           mov         rax,qword ptr [rsi]
  000000006537AFC5: 4C 8D 86 00 02 00  lea         r8,[rsi+0000000000000200h]
                    00
  000000006537AFCC: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006537AFD1: 48 8B CE           mov         rcx,rsi
  000000006537AFD4: FF 50 48           call        qword ptr [rax+48h]
  000000006537AFD7: 90                 nop
  000000006537AFD8: C7 84 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],1
                    00 01 00 00 00
  000000006537AFE3: 48 8D 8E E8 01 00  lea         rcx,[rsi+00000000000001E8h]
                    00
  000000006537AFEA: 48 89 4C 24 50     mov         qword ptr [rsp+50h],rcx
  000000006537AFEF: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000006537AFF4: 48 8D 44 24 50     lea         rax,[rsp+50h]
  000000006537AFF9: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000006537AFFE: 48 8D 86 F0 01 00  lea         rax,[rsi+00000000000001F0h]
                    00
  000000006537B005: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006537B00A: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  000000006537B00F: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006537B014: E8 C7 66 EA FF     call        00000000652216E0
  000000006537B019: 48 8B F8           mov         rdi,rax
  000000006537B01C: BB 03 00 00 00     mov         ebx,3
  000000006537B021: 89 9C 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],ebx
                    00
  000000006537B028: 48 8B D7           mov         rdx,rdi
  000000006537B02B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006537B030: FF 15 BA B9 31 00  call        qword ptr [00000000656969F0h]
  000000006537B036: 90                 nop
  000000006537B037: F6 C3 02           test        bl,2
  000000006537B03A: 74 16              je          000000006537B052
  000000006537B03C: 83 E3 FD           and         ebx,0FFFFFFFDh
  000000006537B03F: 89 9C 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],ebx
                    00
  000000006537B046: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006537B04B: FF 15 0F B7 31 00  call        qword ptr [0000000065696760h]
  000000006537B051: 90                 nop
  000000006537B052: F6 C3 01           test        bl,1
  000000006537B055: 74 0E              je          000000006537B065
  000000006537B057: 83 E3 FE           and         ebx,0FFFFFFFEh
  000000006537B05A: 48 8D 4C 24 30     lea         rcx,[rsp+30h]
  000000006537B05F: FF 15 FB B6 31 00  call        qword ptr [0000000065696760h]
  000000006537B065: 48 8D 94 24 B8 00  lea         rdx,[rsp+00000000000000B8h]
                    00 00
  000000006537B06D: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006537B074: E8 E7 49 04 00     call        00000000653BFA60
  000000006537B079: 90                 nop
  000000006537B07A: 83 CB 04           or          ebx,4
  000000006537B07D: 89 9C 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],ebx
                    00
  000000006537B084: 48 8B D0           mov         rdx,rax
  000000006537B087: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006537B08C: FF 15 3E B9 31 00  call        qword ptr [00000000656969D0h]
  000000006537B092: 84 C0              test        al,al
  000000006537B094: 75 0E              jne         000000006537B0A4
  000000006537B096: F6 86 8C 02 00 00  test        byte ptr [rsi+000000000000028Ch],80h
                    80
  000000006537B09D: 75 05              jne         000000006537B0A4
  000000006537B09F: 40 32 FF           xor         dil,dil
  000000006537B0A2: EB 03              jmp         000000006537B0A7
  000000006537B0A4: 40 B7 01           mov         dil,1
  000000006537B0A7: F6 C3 04           test        bl,4
  000000006537B0AA: 74 0E              je          000000006537B0BA
  000000006537B0AC: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000006537B0B4: FF 15 A6 B6 31 00  call        qword ptr [0000000065696760h]
  000000006537B0BA: 40 84 FF           test        dil,dil
  000000006537B0BD: 74 10              je          000000006537B0CF
  000000006537B0BF: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006537B0C4: FF 15 96 B6 31 00  call        qword ptr [0000000065696760h]
  000000006537B0CA: E9 AA 01 00 00     jmp         000000006537B279
  000000006537B0CF: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  000000006537B0D7: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006537B0DE: E8 CD 48 04 00     call        00000000653BF9B0
  000000006537B0E3: 90                 nop
  000000006537B0E4: 48 8B C8           mov         rcx,rax
  000000006537B0E7: FF 15 FB B8 31 00  call        qword ptr [00000000656969E8h]
  000000006537B0ED: 44 0F B6 F8        movzx       r15d,al
  000000006537B0F1: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000006537B0F9: FF 15 61 B6 31 00  call        qword ptr [0000000065696760h]
  000000006537B0FF: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006537B106: E8 75 2F 00 00     call        000000006537E080
  000000006537B10B: 89 84 24 B8 00 00  mov         dword ptr [rsp+00000000000000B8h],eax
                    00
  000000006537B112: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  000000006537B11A: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006537B121: E8 4A 52 04 00     call        00000000653C0370
  000000006537B126: 90                 nop
  000000006537B127: 48 8B C8           mov         rcx,rax
  000000006537B12A: FF 15 60 B7 31 00  call        qword ptr [0000000065696890h]
  000000006537B130: 44 8B E8           mov         r13d,eax
  000000006537B133: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  000000006537B13B: FF 15 1F B6 31 00  call        qword ptr [0000000065696760h]
  000000006537B141: B2 01              mov         dl,1
  000000006537B143: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006537B14A: FF 15 88 C5 31 00  call        qword ptr [00000000656976D8h]
  000000006537B150: 88 84 24 B0 00 00  mov         byte ptr [rsp+00000000000000B0h],al
                    00
  000000006537B157: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000006537B15C: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006537B163: E8 78 48 04 00     call        00000000653BF9E0
  000000006537B168: 48 8D 96 10 02 00  lea         rdx,[rsi+0000000000000210h]
                    00
  000000006537B16F: 48 8D 8E 00 02 00  lea         rcx,[rsi+0000000000000200h]
                    00
  000000006537B176: FF 15 AC D1 31 00  call        qword ptr [0000000065698328h]
  000000006537B17C: 84 C0              test        al,al
  000000006537B17E: 74 15              je          000000006537B195
  000000006537B180: 48 8D 8E F8 01 00  lea         rcx,[rsi+00000000000001F8h]
                    00
  000000006537B187: FF 15 5B B8 31 00  call        qword ptr [00000000656969E8h]
  000000006537B18D: 84 C0              test        al,al
  000000006537B18F: 0F 84 BB 00 00 00  je          000000006537B250
  000000006537B195: 48 8D 54 24 38     lea         rdx,[rsp+38h]
  000000006537B19A: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006537B1A1: E8 BA 48 04 00     call        00000000653BFA60
  000000006537B1A6: 48 8B F8           mov         rdi,rax
  000000006537B1A9: 48 8D 8E F0 01 00  lea         rcx,[rsi+00000000000001F0h]
                    00
  000000006537B1B0: FF 15 DA B6 31 00  call        qword ptr [0000000065696890h]
  000000006537B1B6: 8B D8              mov         ebx,eax
  000000006537B1B8: 48 8B CF           mov         rcx,rdi
  000000006537B1BB: FF 15 CF B6 31 00  call        qword ptr [0000000065696890h]
  000000006537B1C1: 8B F8              mov         edi,eax
  000000006537B1C3: 2B FB              sub         edi,ebx
  000000006537B1C5: 89 BC 24 C0 00 00  mov         dword ptr [rsp+00000000000000C0h],edi
                    00
  000000006537B1CC: 48 8D 8E E8 01 00  lea         rcx,[rsi+00000000000001E8h]
                    00
  000000006537B1D3: FF 15 B7 B6 31 00  call        qword ptr [0000000065696890h]
  000000006537B1D9: 89 84 24 C8 00 00  mov         dword ptr [rsp+00000000000000C8h],eax
                    00
  000000006537B1E0: 48 8D 94 24 C0 00  lea         rdx,[rsp+00000000000000C0h]
                    00 00
  000000006537B1E8: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  000000006537B1F0: 3B BC 24 B8 00 00  cmp         edi,dword ptr [rsp+00000000000000B8h]
                    00
  000000006537B1F7: 48 0F 4D D1        cmovge      rdx,rcx
  000000006537B1FB: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000006537B203: 3B 02              cmp         eax,dword ptr [rdx]
  000000006537B205: 48 0F 4C CA        cmovl       rcx,rdx
  000000006537B209: 8B 19              mov         ebx,dword ptr [rcx]
  000000006537B20B: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  000000006537B210: FF 15 4A B5 31 00  call        qword ptr [0000000065696760h]
  000000006537B216: 45 85 ED           test        r13d,r13d
  000000006537B219: 7E 13              jle         000000006537B22E
  000000006537B21B: 45 8B C5           mov         r8d,r13d
  000000006537B21E: 8B D3              mov         edx,ebx
  000000006537B220: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006537B227: E8 64 53 04 00     call        00000000653C0590
  000000006537B22C: EB 22              jmp         000000006537B250
  000000006537B22E: 45 84 FF           test        r15b,r15b
  000000006537B231: 74 0F              je          000000006537B242
  000000006537B233: 48 8D 8E E8 01 00  lea         rcx,[rsi+00000000000001E8h]
                    00
  000000006537B23A: FF 15 50 B6 31 00  call        qword ptr [0000000065696890h]
  000000006537B240: 8B D8              mov         ebx,eax
  000000006537B242: 8B D3              mov         edx,ebx
  000000006537B244: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006537B24B: E8 60 4E 04 00     call        00000000653C00B0
  000000006537B250: 0F B6 94 24 B0 00  movzx       edx,byte ptr [rsp+00000000000000B0h]
                    00 00
  000000006537B258: 48 8B 8E E0 01 00  mov         rcx,qword ptr [rsi+00000000000001E0h]
                    00
  000000006537B25F: FF 15 73 C4 31 00  call        qword ptr [00000000656976D8h]
  000000006537B265: 49 8B CE           mov         rcx,r14
  000000006537B268: E8 43 A0 CE FF     call        00000000650652B0
  000000006537B26D: 90                 nop
  000000006537B26E: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006537B273: FF 15 E7 B4 31 00  call        qword ptr [0000000065696760h]
  000000006537B279: 48 83 C4 68        add         rsp,68h
  000000006537B27D: 41 5F              pop         r15
  000000006537B27F: 41 5E              pop         r14
  000000006537B281: 41 5D              pop         r13
  000000006537B283: 41 5C              pop         r12
  000000006537B285: 5F                 pop         rdi
  000000006537B286: 5E                 pop         rsi
  000000006537B287: 5D                 pop         rbp
  000000006537B288: 5B                 pop         rbx
  000000006537B289: C3                 ret

  Summary

      695000 .text

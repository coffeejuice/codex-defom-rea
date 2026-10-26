Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065068FE0: 89 54 24 10        mov         dword ptr [rsp+10h],edx
  0000000065068FE4: 55                 push        rbp
  0000000065068FE5: 56                 push        rsi
  0000000065068FE6: 57                 push        rdi
  0000000065068FE7: 48 83 EC 40        sub         rsp,40h
  0000000065068FEB: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065068FF4: 48 89 5C 24 60     mov         qword ptr [rsp+60h],rbx
  0000000065068FF9: 41 0F B6 E8        movzx       ebp,r8b
  0000000065068FFD: 44 8B D2           mov         r10d,edx
  0000000065069000: 48 8B F9           mov         rdi,rcx
  0000000065069003: 83 FA 20           cmp         edx,20h
  0000000065069006: 7D 11              jge         0000000065069019
  0000000065069008: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  000000006506900C: 44 8B 48 08        mov         r9d,dword ptr [rax+8]
  0000000065069010: 41 0F A3 D1        bt          r9d,edx
  0000000065069014: 0F 92 C0           setb        al
  0000000065069017: EB 31              jmp         000000006506904A
  0000000065069019: 4C 8B 49 08        mov         r9,qword ptr [rcx+8]
  000000006506901D: 8D 42 E0           lea         eax,[rdx-20h]
  0000000065069020: 48 98              cdqe
  0000000065069022: 4C 8B C0           mov         r8,rax
  0000000065069025: 49 C1 E8 05        shr         r8,5
  0000000065069029: 42 8D 0C C5 00 00  lea         ecx,[r8*8+0000000000000000h]
                    00 00
  0000000065069031: C0 E1 02           shl         cl,2
  0000000065069034: 2A C1              sub         al,cl
  0000000065069036: 0F B6 D0           movzx       edx,al
  0000000065069039: 49 63 C0           movsxd      rax,r8d
  000000006506903C: 41 8B 8C 81 C8 01  mov         ecx,dword ptr [r9+rax*4+00000000000001C8h]
                    00 00
  0000000065069044: 0F A3 D1           bt          ecx,edx
  0000000065069047: 0F 92 C0           setb        al
  000000006506904A: 40 3A C5           cmp         al,bpl
  000000006506904D: 0F 84 9E 06 00 00  je          00000000650696F1
  0000000065069053: 48 8B 77 08        mov         rsi,qword ptr [rdi+8]
  0000000065069057: 41 83 FA 08        cmp         r10d,8
  000000006506905B: 75 3D              jne         000000006506909A
  000000006506905D: 40 84 ED           test        bpl,bpl
  0000000065069060: 74 38              je          000000006506909A
  0000000065069062: 48 8D 15 3F 22 64  lea         rdx,[00000000656AB2A8h]
                    00
  0000000065069069: 48 8B CF           mov         rcx,rdi
  000000006506906C: FF 15 EE DC 62 00  call        qword ptr [0000000065696D60h]
  0000000065069072: 84 C0              test        al,al
  0000000065069074: 75 1F              jne         0000000065069095
  0000000065069076: 48 8D 4F 10        lea         rcx,[rdi+10h]
  000000006506907A: 48 8B 01           mov         rax,qword ptr [rcx]
  000000006506907D: FF 50 10           call        qword ptr [rax+10h]
  0000000065069080: 0F BA A6 D8 01 00  bt          dword ptr [rsi+00000000000001D8h],19h
                    00 19
  0000000065069088: 0F 82 63 06 00 00  jb          00000000650696F1
  000000006506908E: 44 8B 54 24 68     mov         r10d,dword ptr [rsp+68h]
  0000000065069093: EB 05              jmp         000000006506909A
  0000000065069095: 44 8B 54 24 68     mov         r10d,dword ptr [rsp+68h]
  000000006506909A: 48 8B 57 20        mov         rdx,qword ptr [rdi+20h]
  000000006506909E: 41 83 FA 20        cmp         r10d,20h
  00000000650690A2: 7D 2D              jge         00000000650690D1
  00000000650690A4: 40 84 ED           test        bpl,bpl
  00000000650690A7: 74 13              je          00000000650690BC
  00000000650690A9: 41 8B CA           mov         ecx,r10d
  00000000650690AC: 41 B8 01 00 00 00  mov         r8d,1
  00000000650690B2: 41 8B C0           mov         eax,r8d
  00000000650690B5: D3 E0              shl         eax,cl
  00000000650690B7: 09 42 08           or          dword ptr [rdx+8],eax
  00000000650690BA: EB 66              jmp         0000000065069122
  00000000650690BC: 41 8B CA           mov         ecx,r10d
  00000000650690BF: 41 B8 01 00 00 00  mov         r8d,1
  00000000650690C5: 41 8B C0           mov         eax,r8d
  00000000650690C8: D3 E0              shl         eax,cl
  00000000650690CA: F7 D0              not         eax
  00000000650690CC: 21 42 08           and         dword ptr [rdx+8],eax
  00000000650690CF: EB 51              jmp         0000000065069122
  00000000650690D1: 41 8D 42 E0        lea         eax,[r10-20h]
  00000000650690D5: 48 63 C8           movsxd      rcx,eax
  00000000650690D8: 48 8B C1           mov         rax,rcx
  00000000650690DB: 48 C1 E8 05        shr         rax,5
  00000000650690DF: 40 84 ED           test        bpl,bpl
  00000000650690E2: 74 1F              je          0000000065069103
  00000000650690E4: 48 63 D0           movsxd      rdx,eax
  00000000650690E7: C1 E0 03           shl         eax,3
  00000000650690EA: C0 E0 02           shl         al,2
  00000000650690ED: 2A C8              sub         cl,al
  00000000650690EF: 41 B8 01 00 00 00  mov         r8d,1
  00000000650690F5: 41 8B C0           mov         eax,r8d
  00000000650690F8: D3 E0              shl         eax,cl
  00000000650690FA: 09 84 96 C8 01 00  or          dword ptr [rsi+rdx*4+00000000000001C8h],eax
                    00
  0000000065069101: EB 1F              jmp         0000000065069122
  0000000065069103: 48 63 D0           movsxd      rdx,eax
  0000000065069106: C1 E0 03           shl         eax,3
  0000000065069109: C0 E0 02           shl         al,2
  000000006506910C: 2A C8              sub         cl,al
  000000006506910E: 41 B8 01 00 00 00  mov         r8d,1
  0000000065069114: 41 8B C0           mov         eax,r8d
  0000000065069117: D3 E0              shl         eax,cl
  0000000065069119: F7 D0              not         eax
  000000006506911B: 21 84 96 C8 01 00  and         dword ptr [rsi+rdx*4+00000000000001C8h],eax
                    00
  0000000065069122: 41 8D 42 FE        lea         eax,[r10-2]
  0000000065069126: 3D 80 00 00 00     cmp         eax,80h
  000000006506912B: 0F 87 C0 05 00 00  ja          00000000650696F1
  0000000065069131: 48 98              cdqe
  0000000065069133: 48 8D 15 C6 6E F9  lea         rdx,[0000000065000000h]
                    FF
  000000006506913A: 0F B6 84 02 4C 97  movzx       eax,byte ptr [rdx+rax+000000000006974Ch]
                    06 00
  0000000065069142: 8B 8C 82 00 97 06  mov         ecx,dword ptr [rdx+rax*4+0000000000069700h]
                    00
  0000000065069149: 48 03 CA           add         rcx,rdx
  000000006506914C: FF E1              jmp         rcx
  000000006506914E: 40 84 ED           test        bpl,bpl
  0000000065069151: 74 24              je          0000000065069177
  0000000065069153: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065069157: 8B 88 CC 01 00 00  mov         ecx,dword ptr [rax+00000000000001CCh]
  000000006506915D: C1 E9 0F           shr         ecx,0Fh
  0000000065069160: 80 E1 01           and         cl,1
  0000000065069163: 75 55              jne         00000000650691BA
  0000000065069165: 41 B0 01           mov         r8b,1
  0000000065069168: BA 4F 00 00 00     mov         edx,4Fh
  000000006506916D: 48 8B CF           mov         rcx,rdi
  0000000065069170: E8 6B FE FF FF     call        0000000065068FE0
  0000000065069175: EB 43              jmp         00000000650691BA
  0000000065069177: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006506917B: 0F B6 40 0C        movzx       eax,byte ptr [rax+0Ch]
  000000006506917F: A8 01              test        al,1
  0000000065069181: 75 28              jne         00000000650691AB
  0000000065069183: 48 8B CF           mov         rcx,rdi
  0000000065069186: FF 15 DC D6 62 00  call        qword ptr [0000000065696868h]
  000000006506918C: 48 85 C0           test        rax,rax
  000000006506918F: 74 1A              je          00000000650691AB
  0000000065069191: 48 8B CF           mov         rcx,rdi
  0000000065069194: FF 15 CE D6 62 00  call        qword ptr [0000000065696868h]
  000000006506919A: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  000000006506919E: 8B 81 CC 01 00 00  mov         eax,dword ptr [rcx+00000000000001CCh]
  00000000650691A4: C1 E8 0F           shr         eax,0Fh
  00000000650691A7: 24 01              and         al,1
  00000000650691A9: 75 0F              jne         00000000650691BA
  00000000650691AB: 45 33 C0           xor         r8d,r8d
  00000000650691AE: 41 8D 50 4F        lea         edx,[r8+4Fh]
  00000000650691B2: 48 8B CF           mov         rcx,rdi
  00000000650691B5: E8 26 FE FF FF     call        0000000065068FE0
  00000000650691BA: BA 98 00 00 00     mov         edx,98h
  00000000650691BF: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650691C4: FF 15 4E D6 62 00  call        qword ptr [0000000065696818h]
  00000000650691CA: 90                 nop
  00000000650691CB: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000650691D0: 48 8B CF           mov         rcx,rdi
  00000000650691D3: FF 15 37 D6 62 00  call        qword ptr [0000000065696810h]
  00000000650691D9: 90                 nop
  00000000650691DA: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000650691DF: FF 15 23 D6 62 00  call        qword ptr [0000000065696808h]
  00000000650691E5: E9 07 05 00 00     jmp         00000000650696F1
  00000000650691EA: 40 0F B6 D5        movzx       edx,bpl
  00000000650691EE: 48 8B CE           mov         rcx,rsi
  00000000650691F1: E8 8A 24 03 00     call        000000006509B680
  00000000650691F6: 33 DB              xor         ebx,ebx
  00000000650691F8: 48 83 C6 18        add         rsi,18h
  00000000650691FC: 48 8B CE           mov         rcx,rsi
  00000000650691FF: FF 15 B3 D6 62 00  call        qword ptr [00000000656968B8h]
  0000000065069205: 85 C0              test        eax,eax
  0000000065069207: 0F 8E E4 04 00 00  jle         00000000650696F1
  000000006506920D: 0F 1F 00           nop         dword ptr [rax]
  0000000065069210: 8B D3              mov         edx,ebx
  0000000065069212: 48 8B CE           mov         rcx,rsi
  0000000065069215: FF 15 25 D7 62 00  call        qword ptr [0000000065696940h]
  000000006506921B: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006506921E: E8 3D FE F9 FF     call        0000000065009060
  0000000065069223: 48 85 C0           test        rax,rax
  0000000065069226: 74 36              je          000000006506925E
  0000000065069228: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  000000006506922C: F6 41 0C 01        test        byte ptr [rcx+0Ch],1
  0000000065069230: 75 2C              jne         000000006506925E
  0000000065069232: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065069236: 8B 91 CC 01 00 00  mov         edx,dword ptr [rcx+00000000000001CCh]
  000000006506923C: 0F BA E2 0E        bt          edx,0Eh
  0000000065069240: 72 1C              jb          000000006506925E
  0000000065069242: C1 EA 0F           shr         edx,0Fh
  0000000065069245: 80 E2 01           and         dl,1
  0000000065069248: 40 3A D5           cmp         dl,bpl
  000000006506924B: 74 11              je          000000006506925E
  000000006506924D: 44 0F B6 C5        movzx       r8d,bpl
  0000000065069251: BA 4F 00 00 00     mov         edx,4Fh
  0000000065069256: 48 8B C8           mov         rcx,rax
  0000000065069259: E8 82 FD FF FF     call        0000000065068FE0
  000000006506925E: FF C3              inc         ebx
  0000000065069260: 48 8B CE           mov         rcx,rsi
  0000000065069263: FF 15 4F D6 62 00  call        qword ptr [00000000656968B8h]
  0000000065069269: 3B D8              cmp         ebx,eax
  000000006506926B: 7C A3              jl          0000000065069210
  000000006506926D: E9 7F 04 00 00     jmp         00000000650696F1
  0000000065069272: 83 66 20 DF        and         dword ptr [rsi+20h],0FFFFFFDFh
  0000000065069276: 33 DB              xor         ebx,ebx
  0000000065069278: 40 84 ED           test        bpl,bpl
  000000006506927B: 0F 94 C3           sete        bl
  000000006506927E: 83 E3 01           and         ebx,1
  0000000065069281: C1 E3 05           shl         ebx,5
  0000000065069284: 09 5E 20           or          dword ptr [rsi+20h],ebx
  0000000065069287: E9 65 04 00 00     jmp         00000000650696F1
  000000006506928C: 83 66 20 BF        and         dword ptr [rsi+20h],0FFFFFFBFh
  0000000065069290: 33 DB              xor         ebx,ebx
  0000000065069292: 40 84 ED           test        bpl,bpl
  0000000065069295: 0F 94 C3           sete        bl
  0000000065069298: 83 E3 01           and         ebx,1
  000000006506929B: C1 E3 06           shl         ebx,6
  000000006506929E: 09 5E 20           or          dword ptr [rsi+20h],ebx
  00000000650692A1: E9 4B 04 00 00     jmp         00000000650696F1
  00000000650692A6: 40 84 ED           test        bpl,bpl
  00000000650692A9: 75 27              jne         00000000650692D2
  00000000650692AB: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  00000000650692AF: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000650692B2: C1 E9 0F           shr         ecx,0Fh
  00000000650692B5: F6 C1 01           test        cl,1
  00000000650692B8: 74 08              je          00000000650692C2
  00000000650692BA: 48 8B CF           mov         rcx,rdi
  00000000650692BD: E8 5E 93 FB FF     call        0000000065022620
  00000000650692C2: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  00000000650692C6: 81 60 10 FF FF FC  and         dword ptr [rax+10h],0FFFCFFFFh
                    FF
  00000000650692CD: E9 82 00 00 00     jmp         0000000065069354
  00000000650692D2: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  00000000650692D6: F6 40 12 03        test        byte ptr [rax+12h],3
  00000000650692DA: 75 78              jne         0000000065069354
  00000000650692DC: 48 8B CF           mov         rcx,rdi
  00000000650692DF: FF 15 83 D5 62 00  call        qword ptr [0000000065696868h]
  00000000650692E5: 48 85 C0           test        rax,rax
  00000000650692E8: 74 52              je          000000006506933C
  00000000650692EA: 48 8B C8           mov         rcx,rax
  00000000650692ED: E8 6E 38 FF FF     call        000000006505CB60
  00000000650692F2: 48 85 C0           test        rax,rax
  00000000650692F5: 74 45              je          000000006506933C
  00000000650692F7: 66 0F 1F 84 00 00  nop         word ptr [rax+rax+0000000000000000h]
                    00 00 00
  0000000065069300: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065069304: 0F BA A1 CC 01 00  bt          dword ptr [rcx+00000000000001CCh],8
                    00 08
  000000006506930C: 72 1D              jb          000000006506932B
  000000006506930E: 48 8B C8           mov         rcx,rax
  0000000065069311: FF 15 51 D5 62 00  call        qword ptr [0000000065696868h]
  0000000065069317: 48 85 C0           test        rax,rax
  000000006506931A: 74 20              je          000000006506933C
  000000006506931C: 48 8B C8           mov         rcx,rax
  000000006506931F: E8 3C 38 FF FF     call        000000006505CB60
  0000000065069324: 48 85 C0           test        rax,rax
  0000000065069327: 75 D7              jne         0000000065069300
  0000000065069329: EB 11              jmp         000000006506933C
  000000006506932B: 0F BA A1 CC 01 00  bt          dword ptr [rcx+00000000000001CCh],8
                    00 08
  0000000065069333: 73 07              jae         000000006506933C
  0000000065069335: B9 01 00 00 00     mov         ecx,1
  000000006506933A: EB 05              jmp         0000000065069341
  000000006506933C: B9 02 00 00 00     mov         ecx,2
  0000000065069341: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065069345: C1 E1 10           shl         ecx,10h
  0000000065069348: 33 48 10           xor         ecx,dword ptr [rax+10h]
  000000006506934B: 81 E1 00 00 03 00  and         ecx,30000h
  0000000065069351: 31 48 10           xor         dword ptr [rax+10h],ecx
  0000000065069354: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065069358: 0F BA A0 C8 01 00  bt          dword ptr [rax+00000000000001C8h],1Ch
                    00 1C
  0000000065069360: 0F 83 8B 03 00 00  jae         00000000650696F1
  0000000065069366: 48 8B CE           mov         rcx,rsi
  0000000065069369: E8 42 5F 03 00     call        000000006509F2B0
  000000006506936E: E9 7E 03 00 00     jmp         00000000650696F1
  0000000065069373: BA 6D 00 00 00     mov         edx,6Dh
  0000000065069378: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  000000006506937D: FF 15 95 D4 62 00  call        qword ptr [0000000065696818h]
  0000000065069383: 90                 nop
  0000000065069384: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000065069389: 48 8B CF           mov         rcx,rdi
  000000006506938C: FF 15 7E D4 62 00  call        qword ptr [0000000065696810h]
  0000000065069392: 90                 nop
  0000000065069393: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000065069398: FF 15 6A D4 62 00  call        qword ptr [0000000065696808h]
  000000006506939E: E9 4E 03 00 00     jmp         00000000650696F1
  00000000650693A3: 48 8B CE           mov         rcx,rsi
  00000000650693A6: E8 65 DC F9 FF     call        0000000065007010
  00000000650693AB: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  00000000650693B0: 33 DB              xor         ebx,ebx
  00000000650693B2: 40 84 ED           test        bpl,bpl
  00000000650693B5: 74 52              je          0000000065069409
  00000000650693B7: 48 8B 4F 20        mov         rcx,qword ptr [rdi+20h]
  00000000650693BB: 48 39 19           cmp         qword ptr [rcx],rbx
  00000000650693BE: 75 49              jne         0000000065069409
  00000000650693C0: 48 8B CF           mov         rcx,rdi
  00000000650693C3: E8 D8 66 FF FF     call        000000006505FAA0
  00000000650693C8: 84 C0              test        al,al
  00000000650693CA: 74 3D              je          0000000065069409
  00000000650693CC: 48 8B 54 24 78     mov         rdx,qword ptr [rsp+78h]
  00000000650693D1: 48 8B 42 20        mov         rax,qword ptr [rdx+20h]
  00000000650693D5: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000650693D8: C1 E9 0E           shr         ecx,0Eh
  00000000650693DB: F6 C1 01           test        cl,1
  00000000650693DE: 74 29              je          0000000065069409
  00000000650693E0: 48 8B 4A 08        mov         rcx,qword ptr [rdx+8]
  00000000650693E4: E8 87 40 FF FF     call        000000006505D470
  00000000650693E9: 48 8B D8           mov         rbx,rax
  00000000650693EC: 48 85 C0           test        rax,rax
  00000000650693EF: 74 18              je          0000000065069409
  00000000650693F1: 48 8B 10           mov         rdx,qword ptr [rax]
  00000000650693F4: 48 8B C8           mov         rcx,rax
  00000000650693F7: FF 52 68           call        qword ptr [rdx+68h]
  00000000650693FA: 4C 8B 1B           mov         r11,qword ptr [rbx]
  00000000650693FD: 33 D2              xor         edx,edx
  00000000650693FF: 48 8B CB           mov         rcx,rbx
  0000000065069402: 41 FF 93 90 00 00  call        qword ptr [r11+0000000000000090h]
                    00
  0000000065069409: FF 15 11 D4 62 00  call        qword ptr [0000000065696820h]
  000000006506940F: B9 04 00 00 00     mov         ecx,4
  0000000065069414: FF 15 DE D5 62 00  call        qword ptr [00000000656969F8h]
  000000006506941A: 84 C0              test        al,al
  000000006506941C: 75 20              jne         000000006506943E
  000000006506941E: 48 8B CF           mov         rcx,rdi
  0000000065069421: FF 15 41 D4 62 00  call        qword ptr [0000000065696868h]
  0000000065069427: 48 85 C0           test        rax,rax
  000000006506942A: 74 12              je          000000006506943E
  000000006506942C: 48 8B CF           mov         rcx,rdi
  000000006506942F: FF 15 33 D4 62 00  call        qword ptr [0000000065696868h]
  0000000065069435: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065069439: E8 82 36 FA FF     call        000000006500CAC0
  000000006506943E: 40 84 ED           test        bpl,bpl
  0000000065069441: 74 22              je          0000000065069465
  0000000065069443: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065069447: 48 83 38 00        cmp         qword ptr [rax],0
  000000006506944B: 75 18              jne         0000000065069465
  000000006506944D: 48 8B 47 08        mov         rax,qword ptr [rdi+8]
  0000000065069451: 0F BA A0 C8 01 00  bt          dword ptr [rax+00000000000001C8h],1Ch
                    00 1C
  0000000065069459: 73 0A              jae         0000000065069465
  000000006506945B: 33 D2              xor         edx,edx
  000000006506945D: 48 8B CE           mov         rcx,rsi
  0000000065069460: E8 EB F9 FF FF     call        0000000065068E50
  0000000065069465: 48 85 DB           test        rbx,rbx
  0000000065069468: 0F 84 83 02 00 00  je          00000000650696F1
  000000006506946E: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065069472: F6 40 08 01        test        byte ptr [rax+8],1
  0000000065069476: 0F 85 75 02 00 00  jne         00000000650696F1
  000000006506947C: 48 8B 4C 24 78     mov         rcx,qword ptr [rsp+78h]
  0000000065069481: 48 8B 41 20        mov         rax,qword ptr [rcx+20h]
  0000000065069485: F6 40 08 01        test        byte ptr [rax+8],1
  0000000065069489: 0F 85 62 02 00 00  jne         00000000650696F1
  000000006506948F: 8B 40 08           mov         eax,dword ptr [rax+8]
  0000000065069492: C1 E8 0E           shr         eax,0Eh
  0000000065069495: A8 01              test        al,1
  0000000065069497: 0F 84 54 02 00 00  je          00000000650696F1
  000000006506949D: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650694A0: 48 8B D1           mov         rdx,rcx
  00000000650694A3: 48 8B CB           mov         rcx,rbx
  00000000650694A6: FF 90 90 00 00 00  call        qword ptr [rax+0000000000000090h]
  00000000650694AC: E9 40 02 00 00     jmp         00000000650696F1
  00000000650694B1: 48 8B CE           mov         rcx,rsi
  00000000650694B4: E8 27 CA FF FF     call        0000000065065EE0
  00000000650694B9: 40 84 ED           test        bpl,bpl
  00000000650694BC: 74 18              je          00000000650694D6
  00000000650694BE: 41 B0 01           mov         r8b,1
  00000000650694C1: BA 64 00 00 00     mov         edx,64h
  00000000650694C6: 48 8B CF           mov         rcx,rdi
  00000000650694C9: E8 12 FB FF FF     call        0000000065068FE0
  00000000650694CE: 48 8B CE           mov         rcx,rsi
  00000000650694D1: E8 EA 35 FA FF     call        000000006500CAC0
  00000000650694D6: 48 8B CE           mov         rcx,rsi
  00000000650694D9: E8 02 CA FF FF     call        0000000065065EE0
  00000000650694DE: E9 0E 02 00 00     jmp         00000000650696F1
  00000000650694E3: 48 8B CE           mov         rcx,rsi
  00000000650694E6: E8 F5 C9 FF FF     call        0000000065065EE0
  00000000650694EB: 48 8B CE           mov         rcx,rsi
  00000000650694EE: E8 BD 5D 03 00     call        000000006509F2B0
  00000000650694F3: E9 F9 01 00 00     jmp         00000000650696F1
  00000000650694F8: 48 8B CE           mov         rcx,rsi
  00000000650694FB: E8 10 DB F9 FF     call        0000000065007010
  0000000065069500: 48 8B F0           mov         rsi,rax
  0000000065069503: 48 8B 48 08        mov         rcx,qword ptr [rax+8]
  0000000065069507: E8 24 3F FF FF     call        000000006505D430
  000000006506950C: 48 8B D8           mov         rbx,rax
  000000006506950F: 48 85 C0           test        rax,rax
  0000000065069512: 75 2A              jne         000000006506953E
  0000000065069514: 40 84 ED           test        bpl,bpl
  0000000065069517: 74 10              je          0000000065069529
  0000000065069519: 48 8B CF           mov         rcx,rdi
  000000006506951C: E8 7F 65 FF FF     call        000000006505FAA0
  0000000065069521: 84 C0              test        al,al
  0000000065069523: 0F 84 C8 01 00 00  je          00000000650696F1
  0000000065069529: 48 8B 4E 08        mov         rcx,qword ptr [rsi+8]
  000000006506952D: E8 3E 3F FF FF     call        000000006505D470
  0000000065069532: 48 8B D8           mov         rbx,rax
  0000000065069535: 48 85 C0           test        rax,rax
  0000000065069538: 0F 84 B3 01 00 00  je          00000000650696F1
  000000006506953E: 40 84 ED           test        bpl,bpl
  0000000065069541: 74 56              je          0000000065069599
  0000000065069543: 48 8B CF           mov         rcx,rdi
  0000000065069546: E8 55 65 FF FF     call        000000006505FAA0
  000000006506954B: 84 C0              test        al,al
  000000006506954D: 0F 84 9E 01 00 00  je          00000000650696F1
  0000000065069553: 48 8B CB           mov         rcx,rbx
  0000000065069556: E8 D5 7F 48 00     call        00000000654F1530
  000000006506955B: 48 3B C6           cmp         rax,rsi
  000000006506955E: 0F 84 8D 01 00 00  je          00000000650696F1
  0000000065069564: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065069568: F6 40 08 01        test        byte ptr [rax+8],1
  000000006506956C: 0F 85 7F 01 00 00  jne         00000000650696F1
  0000000065069572: 48 8B 46 20        mov         rax,qword ptr [rsi+20h]
  0000000065069576: 8B 48 08           mov         ecx,dword ptr [rax+8]
  0000000065069579: C1 E9 0E           shr         ecx,0Eh
  000000006506957C: F6 C1 01           test        cl,1
  000000006506957F: 0F 84 6C 01 00 00  je          00000000650696F1
  0000000065069585: 48 8B 03           mov         rax,qword ptr [rbx]
  0000000065069588: 48 8B D6           mov         rdx,rsi
  000000006506958B: 48 8B CB           mov         rcx,rbx
  000000006506958E: FF 90 90 00 00 00  call        qword ptr [rax+0000000000000090h]
  0000000065069594: E9 58 01 00 00     jmp         00000000650696F1
  0000000065069599: 48 8B CB           mov         rcx,rbx
  000000006506959C: E8 8F 7F 48 00     call        00000000654F1530
  00000000650695A1: 48 3B C6           cmp         rax,rsi
  00000000650695A4: 0F 85 47 01 00 00  jne         00000000650696F1
  00000000650695AA: 48 8B 03           mov         rax,qword ptr [rbx]
  00000000650695AD: 48 8B CB           mov         rcx,rbx
  00000000650695B0: FF 50 68           call        qword ptr [rax+68h]
  00000000650695B3: 4C 8B 1B           mov         r11,qword ptr [rbx]
  00000000650695B6: 33 D2              xor         edx,edx
  00000000650695B8: 48 8B CB           mov         rcx,rbx
  00000000650695BB: 41 FF 93 90 00 00  call        qword ptr [r11+0000000000000090h]
                    00
  00000000650695C2: E9 2A 01 00 00     jmp         00000000650696F1
  00000000650695C7: 48 8B CE           mov         rcx,rsi
  00000000650695CA: E8 B1 EA FF FF     call        0000000065068080
  00000000650695CF: 48 8B CE           mov         rcx,rsi
  00000000650695D2: E8 F9 7B FF FF     call        00000000650611D0
  00000000650695D7: 48 8B CE           mov         rcx,rsi
  00000000650695DA: E8 61 63 FF FF     call        000000006505F940
  00000000650695DF: E9 0D 01 00 00     jmp         00000000650696F1
  00000000650695E4: 40 84 ED           test        bpl,bpl
  00000000650695E7: 0F 84 04 01 00 00  je          00000000650696F1
  00000000650695ED: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  00000000650695F1: 8B 48 08           mov         ecx,dword ptr [rax+8]
  00000000650695F4: C1 E9 0F           shr         ecx,0Fh
  00000000650695F7: F6 C1 01           test        cl,1
  00000000650695FA: 0F 84 F1 00 00 00  je          00000000650696F1
  0000000065069600: 48 8B CE           mov         rcx,rsi
  0000000065069603: E8 28 01 03 00     call        0000000065099730
  0000000065069608: 48 8B CE           mov         rcx,rsi
  000000006506960B: E8 E0 04 03 00     call        0000000065099AF0
  0000000065069610: E9 DC 00 00 00     jmp         00000000650696F1
  0000000065069615: 48 8B CE           mov         rcx,rsi
  0000000065069618: E8 D3 62 FA FF     call        000000006500F8F0
  000000006506961D: 48 85 C0           test        rax,rax
  0000000065069620: 0F 84 CB 00 00 00  je          00000000650696F1
  0000000065069626: 40 84 ED           test        bpl,bpl
  0000000065069629: 74 10              je          000000006506963B
  000000006506962B: 48 8B D7           mov         rdx,rdi
  000000006506962E: 48 8B C8           mov         rcx,rax
  0000000065069631: E8 8A 6D FF FF     call        00000000650603C0
  0000000065069636: E9 B6 00 00 00     jmp         00000000650696F1
  000000006506963B: 48 89 7C 24 78     mov         qword ptr [rsp+78h],rdi
  0000000065069640: 48 8D 48 30        lea         rcx,[rax+30h]
  0000000065069644: 48 8D 54 24 78     lea         rdx,[rsp+78h]
  0000000065069649: E8 22 8C 2F 00     call        0000000065362270
  000000006506964E: E9 9E 00 00 00     jmp         00000000650696F1
  0000000065069653: 40 84 ED           test        bpl,bpl
  0000000065069656: 0F 84 95 00 00 00  je          00000000650696F1
  000000006506965C: 41 B0 01           mov         r8b,1
  000000006506965F: BA 09 00 00 00     mov         edx,9
  0000000065069664: 48 8B CF           mov         rcx,rdi
  0000000065069667: E8 74 F9 FF FF     call        0000000065068FE0
  000000006506966C: 48 8B CE           mov         rcx,rsi
  000000006506966F: E8 CC E2 02 00     call        0000000065097940
  0000000065069674: EB 7B              jmp         00000000650696F1
  0000000065069676: 40 84 ED           test        bpl,bpl
  0000000065069679: 74 76              je          00000000650696F1
  000000006506967B: 48 8B CE           mov         rcx,rsi
  000000006506967E: E8 8D E7 02 00     call        0000000065097E10
  0000000065069683: EB 6C              jmp         00000000650696F1
  0000000065069685: C7 44 24 28 80 00  mov         dword ptr [rsp+28h],80h
                    00 00
  000000006506968D: C7 44 24 2C 81 00  mov         dword ptr [rsp+2Ch],81h
                    00 00
  0000000065069695: C7 44 24 30 82 00  mov         dword ptr [rsp+30h],82h
                    00 00
  000000006506969D: 40 84 ED           test        bpl,bpl
  00000000650696A0: 74 4F              je          00000000650696F1
  00000000650696A2: 33 DB              xor         ebx,ebx
  00000000650696A4: 8B 4C 9C 28        mov         ecx,dword ptr [rsp+rbx*4+28h]
  00000000650696A8: 41 3B CA           cmp         ecx,r10d
  00000000650696AB: 74 3B              je          00000000650696E8
  00000000650696AD: 48 8B 57 20        mov         rdx,qword ptr [rdi+20h]
  00000000650696B1: 83 F9 20           cmp         ecx,20h
  00000000650696B4: 7D 0C              jge         00000000650696C2
  00000000650696B6: 41 8B C0           mov         eax,r8d
  00000000650696B9: D3 E0              shl         eax,cl
  00000000650696BB: F7 D0              not         eax
  00000000650696BD: 21 42 08           and         dword ptr [rdx+8],eax
  00000000650696C0: EB 26              jmp         00000000650696E8
  00000000650696C2: 8D 41 E0           lea         eax,[rcx-20h]
  00000000650696C5: 48 63 C8           movsxd      rcx,eax
  00000000650696C8: 48 8B C1           mov         rax,rcx
  00000000650696CB: 48 C1 E8 05        shr         rax,5
  00000000650696CF: 48 63 D0           movsxd      rdx,eax
  00000000650696D2: C1 E0 03           shl         eax,3
  00000000650696D5: C0 E0 02           shl         al,2
  00000000650696D8: 2A C8              sub         cl,al
  00000000650696DA: 41 8B C0           mov         eax,r8d
  00000000650696DD: D3 E0              shl         eax,cl
  00000000650696DF: F7 D0              not         eax
  00000000650696E1: 21 84 96 C8 01 00  and         dword ptr [rsi+rdx*4+00000000000001C8h],eax
                    00
  00000000650696E8: 48 FF C3           inc         rbx
  00000000650696EB: 48 83 FB 03        cmp         rbx,3
  00000000650696EF: 7C B3              jl          00000000650696A4
  00000000650696F1: 48 8B 5C 24 60     mov         rbx,qword ptr [rsp+60h]
  00000000650696F6: 48 83 C4 40        add         rsp,40h
  00000000650696FA: 5F                 pop         rdi
  00000000650696FB: 5E                 pop         rsi
  00000000650696FC: 5D                 pop         rbp
  00000000650696FD: C3                 ret
  00000000650696FE: 66 90              nop
  0000000065069700: 73 93              jae         0000000065069695
  0000000065069702: 06
  0000000065069703: 00 D6              add         dh,dl
  0000000065069705: 94                 xchg        eax,esp
  0000000065069706: 06
  0000000065069707: 00 15 96 06 00 B1  add         byte ptr [0000000016069DA3h],dl
  000000006506970D: 94                 xchg        eax,esp
  000000006506970E: 06
  000000006506970F: 00 E3              add         bl,ah
  0000000065069711: 94                 xchg        eax,esp
  0000000065069712: 06
  0000000065069713: 00 EB              add         bl,ch
  0000000065069715: 94                 xchg        eax,esp
  0000000065069716: 06
  0000000065069717: 00 F8              add         al,bh
  0000000065069719: 94                 xchg        eax,esp
  000000006506971A: 06
  000000006506971B: 00 8C 92 06 00 72  add         byte ptr [rdx+rdx*4+FFFFFFFF92720006h],cl
                    92
  0000000065069722: 06
  0000000065069723: 00 A6 92 06 00 4E  add         byte ptr [rsi+000000004E000692h],ah
  0000000065069729: 91                 xchg        eax,ecx
  000000006506972A: 06
  000000006506972B: 00 EA              add         dl,ch
  000000006506972D: 91                 xchg        eax,ecx
  000000006506972E: 06
  000000006506972F: 00 C7              add         bh,al
  0000000065069731: 95                 xchg        eax,ebp
  0000000065069732: 06
  0000000065069733: 00 A3 93 06 00 E4  add         byte ptr [rbx+FFFFFFFFE4000693h],ah
  0000000065069739: 95                 xchg        eax,ebp
  000000006506973A: 06
  000000006506973B: 00 53 96           add         byte ptr [rbx-6Ah],dl
  000000006506973E: 06
  000000006506973F: 00 76 96           add         byte ptr [rsi-6Ah],dh
  0000000065069742: 06
  0000000065069743: 00 85 96 06 00 F1  add         byte ptr [rbp+FFFFFFFFF1000696h],al
  0000000065069749: 96                 xchg        eax,esi
  000000006506974A: 06
  000000006506974B: 00 00              add         byte ptr [rax],al
  000000006506974D: 12 01              adc         al,byte ptr [rcx]
  000000006506974F: 02 12              add         dl,byte ptr [rdx]
  0000000065069751: 12 03              adc         al,byte ptr [rbx]
  0000000065069753: 04 05              add         al,5
  0000000065069755: 12 12              adc         dl,byte ptr [rdx]
  0000000065069757: 12 06              adc         al,byte ptr [rsi]
  0000000065069759: 12 12              adc         dl,byte ptr [rdx]
  000000006506975B: 12 12              adc         dl,byte ptr [rdx]
  000000006506975D: 12 12              adc         dl,byte ptr [rdx]
  000000006506975F: 12 12              adc         dl,byte ptr [rdx]
  0000000065069761: 12 12              adc         dl,byte ptr [rdx]
  0000000065069763: 12 12              adc         dl,byte ptr [rdx]
  0000000065069765: 12 12              adc         dl,byte ptr [rdx]
  0000000065069767: 12 12              adc         dl,byte ptr [rdx]
  0000000065069769: 12 12              adc         dl,byte ptr [rdx]
  000000006506976B: 12 12              adc         dl,byte ptr [rdx]
  000000006506976D: 12 12              adc         dl,byte ptr [rdx]
  000000006506976F: 12 12              adc         dl,byte ptr [rdx]
  0000000065069771: 07
  0000000065069772: 12 12              adc         dl,byte ptr [rdx]
  0000000065069774: 12 12              adc         dl,byte ptr [rdx]
  0000000065069776: 12 12              adc         dl,byte ptr [rdx]
  0000000065069778: 12 12              adc         dl,byte ptr [rdx]
  000000006506977A: 12 12              adc         dl,byte ptr [rdx]
  000000006506977C: 12 12              adc         dl,byte ptr [rdx]
  000000006506977E: 12 12              adc         dl,byte ptr [rdx]
  0000000065069780: 12 12              adc         dl,byte ptr [rdx]
  0000000065069782: 12 12              adc         dl,byte ptr [rdx]
  0000000065069784: 08 12              or          byte ptr [rdx],dl
  0000000065069786: 12 12              adc         dl,byte ptr [rdx]
  0000000065069788: 12 12              adc         dl,byte ptr [rdx]
  000000006506978A: 12 12              adc         dl,byte ptr [rdx]
  000000006506978C: 12 12              adc         dl,byte ptr [rdx]
  000000006506978E: 12 12              adc         dl,byte ptr [rdx]
  0000000065069790: 09 12              or          dword ptr [rdx],edx
  0000000065069792: 12 12              adc         dl,byte ptr [rdx]
  0000000065069794: 12 12              adc         dl,byte ptr [rdx]
  0000000065069796: 12 12              adc         dl,byte ptr [rdx]
  0000000065069798: 0A 0B              or          cl,byte ptr [rbx]
  000000006506979A: 0C 12              or          al,12h
  000000006506979C: 12 12              adc         dl,byte ptr [rdx]
  000000006506979E: 12 12              adc         dl,byte ptr [rdx]
  00000000650697A0: 12 12              adc         dl,byte ptr [rdx]
  00000000650697A2: 12 12              adc         dl,byte ptr [rdx]
  00000000650697A4: 12 12              adc         dl,byte ptr [rdx]
  00000000650697A6: 12 12              adc         dl,byte ptr [rdx]
  00000000650697A8: 12 12              adc         dl,byte ptr [rdx]
  00000000650697AA: 12 12              adc         dl,byte ptr [rdx]
  00000000650697AC: 12 12              adc         dl,byte ptr [rdx]
  00000000650697AE: 0D 12 12 0E 12     or          eax,120E1212h
  00000000650697B3: 12 12              adc         dl,byte ptr [rdx]
  00000000650697B5: 12 12              adc         dl,byte ptr [rdx]
  00000000650697B7: 12 12              adc         dl,byte ptr [rdx]
  00000000650697B9: 12 12              adc         dl,byte ptr [rdx]
  00000000650697BB: 12 12              adc         dl,byte ptr [rdx]
  00000000650697BD: 12 12              adc         dl,byte ptr [rdx]
  00000000650697BF: 12 12              adc         dl,byte ptr [rdx]
  00000000650697C1: 12 0F              adc         cl,byte ptr [rdi]
  00000000650697C3: 10 12              adc         byte ptr [rdx],dl
  00000000650697C5: 12 12              adc         dl,byte ptr [rdx]
  00000000650697C7: 12 12              adc         dl,byte ptr [rdx]
  00000000650697C9: 12 11              adc         dl,byte ptr [rcx]
  00000000650697CB: 11 11              adc         dword ptr [rcx],edx

  Summary

      695000 .text

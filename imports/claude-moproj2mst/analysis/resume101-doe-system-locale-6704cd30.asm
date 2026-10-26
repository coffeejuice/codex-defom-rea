Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006704CD30: 40 53              push        rbx
  000000006704CD32: 56                 push        rsi
  000000006704CD33: 57                 push        rdi
  000000006704CD34: 48 81 EC 60 01 00  sub         rsp,160h
                    00
  000000006704CD3B: 48 C7 84 24 20 01  mov         qword ptr [rsp+0000000000000120h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000006704CD47: 48 8B 1D E2 6F 2A  mov         rbx,qword ptr [00000000672F3D30h]
                    00
  000000006704CD4E: 48 85 DB           test        rbx,rbx
  000000006704CD51: 75 08              jne         000000006704CD5B
  000000006704CD53: E8 B8 FE FF FF     call        000000006704CC10
  000000006704CD58: 48 8B D8           mov         rbx,rax
  000000006704CD5B: 48 83 3D D5 6F 2A  cmp         qword ptr [00000000672F3D38h],0
                    00 00
  000000006704CD63: 75 0C              jne         000000006704CD71
  000000006704CD65: E8 46 FF FF FF     call        000000006704CCB0
  000000006704CD6A: 48 89 05 C7 6F 2A  mov         qword ptr [00000000672F3D38h],rax
                    00
  000000006704CD71: 8B 4C 24 70        mov         ecx,dword ptr [rsp+70h]
  000000006704CD75: 81 E1 00 00 00 C0  and         ecx,0C0000000h
  000000006704CD7B: 0F BA F1 1E        btr         ecx,1Eh
  000000006704CD7F: 0F BA E9 1F        bts         ecx,1Fh
  000000006704CD83: 89 4C 24 70        mov         dword ptr [rsp+70h],ecx
  000000006704CD87: 33 F6              xor         esi,esi
  000000006704CD89: 48 89 74 24 68     mov         qword ptr [rsp+68h],rsi
  000000006704CD8E: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006704CD91: 4C 8D 4C 24 68     lea         r9,[rsp+68h]
  000000006704CD96: 44 8D 46 23        lea         r8d,[rsi+23h]
  000000006704CD9A: 48 8D 94 24 C8 00  lea         rdx,[rsp+00000000000000C8h]
                    00 00
  000000006704CDA2: 48 8B CB           mov         rcx,rbx
  000000006704CDA5: FF 50 08           call        qword ptr [rax+8]
  000000006704CDA8: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  000000006704CDB0: E8 BB B5 0E 00     call        0000000067138370
  000000006704CDB5: 4C 8B 1B           mov         r11,qword ptr [rbx]
  000000006704CDB8: 48 8D 94 24 E8 00  lea         rdx,[rsp+00000000000000E8h]
                    00 00
  000000006704CDC0: 48 8B CB           mov         rcx,rbx
  000000006704CDC3: 41 FF 53 10        call        qword ptr [r11+10h]
  000000006704CDC7: 0F B7 10           movzx       edx,word ptr [rax]
  000000006704CDCA: B8 87 01 00 00     mov         eax,187h
  000000006704CDCF: 48 8B 0D 62 6F 2A  mov         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704CDD6: 66 3B D0           cmp         dx,ax
  000000006704CDD9: 75 05              jne         000000006704CDE0
  000000006704CDDB: 48 8B D1           mov         rdx,rcx
  000000006704CDDE: EB 15              jmp         000000006704CDF5
  000000006704CDE0: 0F B7 C2           movzx       eax,dx
  000000006704CDE3: 48 8D 14 C0        lea         rdx,[rax+rax*8]
  000000006704CDE7: 48 C1 E2 04        shl         rdx,4
  000000006704CDEB: 48 8D 05 4E 52 16  lea         rax,[00000000671B2040h]
                    00
  000000006704CDF2: 48 03 D0           add         rdx,rax
  000000006704CDF5: 41 B8 90 00 00 00  mov         r8d,90h
  000000006704CDFB: E8 9E BC 12 00     call        0000000067178A9E
  000000006704CE00: 44 8B 9C 24 C0 00  mov         r11d,dword ptr [rsp+00000000000000C0h]
                    00 00
  000000006704CE08: 41 81 E3 00 00 00  and         r11d,0C0000000h
                    C0
  000000006704CE0F: 41 0F BA F3 1E     btr         r11d,1Eh
  000000006704CE14: 41 0F BA EB 1F     bts         r11d,1Fh
  000000006704CE19: 44 89 9C 24 C0 00  mov         dword ptr [rsp+00000000000000C0h],r11d
                    00 00
  000000006704CE21: 48 89 B4 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rsi
                    00 00
  000000006704CE29: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006704CE2C: 4C 8D 8C 24 B8 00  lea         r9,[rsp+00000000000000B8h]
                    00 00
  000000006704CE34: 45 33 C0           xor         r8d,r8d
  000000006704CE37: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  000000006704CE3C: 48 8B CB           mov         rcx,rbx
  000000006704CE3F: FF 50 08           call        qword ptr [rax+8]
  000000006704CE42: 90                 nop
  000000006704CE43: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CE48: E8 93 B7 0E 00     call        00000000671385E0
  000000006704CE4D: 84 C0              test        al,al
  000000006704CE4F: 75 1A              jne         000000006704CE6B
  000000006704CE51: 33 D2              xor         edx,edx
  000000006704CE53: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CE58: E8 B3 DE 0E 00     call        000000006713AD10
  000000006704CE5D: 48 8B 0D D4 6E 2A  mov         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704CE64: 66 89 01           mov         word ptr [rcx],ax
  000000006704CE67: 66 89 71 02        mov         word ptr [rcx+2],si
  000000006704CE6B: 8B 84 24 90 00 00  mov         eax,dword ptr [rsp+0000000000000090h]
                    00
  000000006704CE72: 25 00 00 00 C0     and         eax,0C0000000h
  000000006704CE77: 0F BA F0 1E        btr         eax,1Eh
  000000006704CE7B: 0F BA E8 1F        bts         eax,1Fh
  000000006704CE7F: 89 84 24 90 00 00  mov         dword ptr [rsp+0000000000000090h],eax
                    00
  000000006704CE86: 48 89 B4 24 88 00  mov         qword ptr [rsp+0000000000000088h],rsi
                    00 00
  000000006704CE8E: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006704CE91: 4C 8D 8C 24 88 00  lea         r9,[rsp+0000000000000088h]
                    00 00
  000000006704CE99: 41 B8 01 00 00 00  mov         r8d,1
  000000006704CE9F: 48 8D 94 24 00 01  lea         rdx,[rsp+0000000000000100h]
                    00 00
  000000006704CEA7: 48 8B CB           mov         rcx,rbx
  000000006704CEAA: FF 50 08           call        qword ptr [rax+8]
  000000006704CEAD: 90                 nop
  000000006704CEAE: 48 8B D0           mov         rdx,rax
  000000006704CEB1: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CEB6: E8 D5 C7 0E 00     call        0000000067139690
  000000006704CEBB: 90                 nop
  000000006704CEBC: 48 8D 8C 24 00 01  lea         rcx,[rsp+0000000000000100h]
                    00 00
  000000006704CEC4: E8 A7 B4 0E 00     call        0000000067138370
  000000006704CEC9: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CECE: E8 0D B7 0E 00     call        00000000671385E0
  000000006704CED3: 84 C0              test        al,al
  000000006704CED5: 75 1B              jne         000000006704CEF2
  000000006704CED7: 33 D2              xor         edx,edx
  000000006704CED9: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CEDE: E8 2D DE 0E 00     call        000000006713AD10
  000000006704CEE3: 48 8B 0D 4E 6E 2A  mov         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704CEEA: 66 89 41 04        mov         word ptr [rcx+4],ax
  000000006704CEEE: 66 89 71 02        mov         word ptr [rcx+2],si
  000000006704CEF2: 8B 84 24 B0 00 00  mov         eax,dword ptr [rsp+00000000000000B0h]
                    00
  000000006704CEF9: 25 00 00 00 C0     and         eax,0C0000000h
  000000006704CEFE: 0F BA F0 1E        btr         eax,1Eh
  000000006704CF02: 0F BA E8 1F        bts         eax,1Fh
  000000006704CF06: 89 84 24 B0 00 00  mov         dword ptr [rsp+00000000000000B0h],eax
                    00
  000000006704CF0D: 48 89 B4 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rsi
                    00 00
  000000006704CF15: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006704CF18: 4C 8D 8C 24 A8 00  lea         r9,[rsp+00000000000000A8h]
                    00 00
  000000006704CF20: 41 B8 21 00 00 00  mov         r8d,21h
  000000006704CF26: 48 8D 94 24 38 01  lea         rdx,[rsp+0000000000000138h]
                    00 00
  000000006704CF2E: 48 8B CB           mov         rcx,rbx
  000000006704CF31: FF 50 08           call        qword ptr [rax+8]
  000000006704CF34: 90                 nop
  000000006704CF35: 48 8B D0           mov         rdx,rax
  000000006704CF38: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CF3D: E8 4E C7 0E 00     call        0000000067139690
  000000006704CF42: 90                 nop
  000000006704CF43: 48 8D 8C 24 38 01  lea         rcx,[rsp+0000000000000138h]
                    00 00
  000000006704CF4B: E8 20 B4 0E 00     call        0000000067138370
  000000006704CF50: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CF55: E8 86 B6 0E 00     call        00000000671385E0
  000000006704CF5A: 84 C0              test        al,al
  000000006704CF5C: 75 17              jne         000000006704CF75
  000000006704CF5E: 33 D2              xor         edx,edx
  000000006704CF60: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CF65: E8 A6 DD 0E 00     call        000000006713AD10
  000000006704CF6A: 48 8B 0D C7 6D 2A  mov         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704CF71: 66 89 41 02        mov         word ptr [rcx+2],ax
  000000006704CF75: 8B 84 24 80 00 00  mov         eax,dword ptr [rsp+0000000000000080h]
                    00
  000000006704CF7C: 25 00 00 00 C0     and         eax,0C0000000h
  000000006704CF81: 0F BA F0 1E        btr         eax,1Eh
  000000006704CF85: 0F BA E8 1F        bts         eax,1Fh
  000000006704CF89: 89 84 24 80 00 00  mov         dword ptr [rsp+0000000000000080h],eax
                    00
  000000006704CF90: 48 89 74 24 78     mov         qword ptr [rsp+78h],rsi
  000000006704CF95: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006704CF98: 4C 8D 4C 24 78     lea         r9,[rsp+78h]
  000000006704CF9D: 41 B8 02 00 00 00  mov         r8d,2
  000000006704CFA3: 48 8D 94 24 D8 00  lea         rdx,[rsp+00000000000000D8h]
                    00 00
  000000006704CFAB: 48 8B CB           mov         rcx,rbx
  000000006704CFAE: FF 50 08           call        qword ptr [rax+8]
  000000006704CFB1: 90                 nop
  000000006704CFB2: 48 8B D0           mov         rdx,rax
  000000006704CFB5: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CFBA: E8 D1 C6 0E 00     call        0000000067139690
  000000006704CFBF: 90                 nop
  000000006704CFC0: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  000000006704CFC8: E8 A3 B3 0E 00     call        0000000067138370
  000000006704CFCD: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CFD2: E8 09 B6 0E 00     call        00000000671385E0
  000000006704CFD7: 84 C0              test        al,al
  000000006704CFD9: 75 42              jne         000000006704D01D
  000000006704CFDB: 48 8D 94 24 80 01  lea         rdx,[rsp+0000000000000180h]
                    00 00
  000000006704CFE3: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704CFE8: E8 43 CC 0E 00     call        0000000067139C30
  000000006704CFED: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006704CFF0: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  000000006704CFF4: 0F B7 00           movzx       eax,word ptr [rax]
  000000006704CFF7: 48 8B 0D 3A 6D 2A  mov         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704CFFE: 66 89 41 06        mov         word ptr [rcx+6],ax
  000000006704D002: 48 8B 84 24 80 01  mov         rax,qword ptr [rsp+0000000000000180h]
                    00 00
  000000006704D00A: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006704D00E: 75 0D              jne         000000006704D01D
  000000006704D010: 48 8B 8C 24 80 01  mov         rcx,qword ptr [rsp+0000000000000180h]
                    00 00
  000000006704D018: E8 B3 30 02 00     call        00000000670700D0
  000000006704D01D: 8B 84 24 A0 00 00  mov         eax,dword ptr [rsp+00000000000000A0h]
                    00
  000000006704D024: 25 00 00 00 C0     and         eax,0C0000000h
  000000006704D029: 0F BA F0 1E        btr         eax,1Eh
  000000006704D02D: 0F BA E8 1F        bts         eax,1Fh
  000000006704D031: 89 84 24 A0 00 00  mov         dword ptr [rsp+00000000000000A0h],eax
                    00
  000000006704D038: 48 89 B4 24 98 00  mov         qword ptr [rsp+0000000000000098h],rsi
                    00 00
  000000006704D040: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006704D043: 4C 8D 8C 24 98 00  lea         r9,[rsp+0000000000000098h]
                    00 00
  000000006704D04B: 41 B8 03 00 00 00  mov         r8d,3
  000000006704D051: 48 8D 94 24 F0 00  lea         rdx,[rsp+00000000000000F0h]
                    00 00
  000000006704D059: 48 8B CB           mov         rcx,rbx
  000000006704D05C: FF 50 08           call        qword ptr [rax+8]
  000000006704D05F: 90                 nop
  000000006704D060: 48 8B D0           mov         rdx,rax
  000000006704D063: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D068: E8 23 C6 0E 00     call        0000000067139690
  000000006704D06D: 90                 nop
  000000006704D06E: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  000000006704D076: E8 F5 B2 0E 00     call        0000000067138370
  000000006704D07B: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D080: E8 5B B5 0E 00     call        00000000671385E0
  000000006704D085: 84 C0              test        al,al
  000000006704D087: 75 42              jne         000000006704D0CB
  000000006704D089: 48 8D 94 24 88 01  lea         rdx,[rsp+0000000000000188h]
                    00 00
  000000006704D091: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D096: E8 95 CB 0E 00     call        0000000067139C30
  000000006704D09B: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006704D09E: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  000000006704D0A2: 0F B7 00           movzx       eax,word ptr [rax]
  000000006704D0A5: 48 8B 0D 8C 6C 2A  mov         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704D0AC: 66 89 41 08        mov         word ptr [rcx+8],ax
  000000006704D0B0: 48 8B 84 24 88 01  mov         rax,qword ptr [rsp+0000000000000188h]
                    00 00
  000000006704D0B8: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006704D0BC: 75 0D              jne         000000006704D0CB
  000000006704D0BE: 48 8B 8C 24 88 01  mov         rcx,qword ptr [rsp+0000000000000188h]
                    00 00
  000000006704D0C6: E8 05 30 02 00     call        00000000670700D0
  000000006704D0CB: 8B 44 24 40        mov         eax,dword ptr [rsp+40h]
  000000006704D0CF: 25 00 00 00 C0     and         eax,0C0000000h
  000000006704D0D4: 0F BA F0 1E        btr         eax,1Eh
  000000006704D0D8: 0F BA E8 1F        bts         eax,1Fh
  000000006704D0DC: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000006704D0E0: 48 89 74 24 38     mov         qword ptr [rsp+38h],rsi
  000000006704D0E5: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006704D0E8: 4C 8D 4C 24 38     lea         r9,[rsp+38h]
  000000006704D0ED: 41 B8 04 00 00 00  mov         r8d,4
  000000006704D0F3: 48 8D 94 24 10 01  lea         rdx,[rsp+0000000000000110h]
                    00 00
  000000006704D0FB: 48 8B CB           mov         rcx,rbx
  000000006704D0FE: FF 50 08           call        qword ptr [rax+8]
  000000006704D101: 90                 nop
  000000006704D102: 48 8B D0           mov         rdx,rax
  000000006704D105: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D10A: E8 81 C5 0E 00     call        0000000067139690
  000000006704D10F: 90                 nop
  000000006704D110: 48 8D 8C 24 10 01  lea         rcx,[rsp+0000000000000110h]
                    00 00
  000000006704D118: E8 53 B2 0E 00     call        0000000067138370
  000000006704D11D: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D122: E8 B9 B4 0E 00     call        00000000671385E0
  000000006704D127: 84 C0              test        al,al
  000000006704D129: 75 42              jne         000000006704D16D
  000000006704D12B: 48 8D 94 24 90 01  lea         rdx,[rsp+0000000000000190h]
                    00 00
  000000006704D133: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D138: E8 F3 CA 0E 00     call        0000000067139C30
  000000006704D13D: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006704D140: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  000000006704D144: 0F B7 00           movzx       eax,word ptr [rax]
  000000006704D147: 48 8B 0D EA 6B 2A  mov         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704D14E: 66 89 41 0E        mov         word ptr [rcx+0Eh],ax
  000000006704D152: 48 8B 84 24 90 01  mov         rax,qword ptr [rsp+0000000000000190h]
                    00 00
  000000006704D15A: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006704D15E: 75 0D              jne         000000006704D16D
  000000006704D160: 48 8B 8C 24 90 01  mov         rcx,qword ptr [rsp+0000000000000190h]
                    00 00
  000000006704D168: E8 63 2F 02 00     call        00000000670700D0
  000000006704D16D: 8B 44 24 50        mov         eax,dword ptr [rsp+50h]
  000000006704D171: 25 00 00 00 C0     and         eax,0C0000000h
  000000006704D176: 0F BA F0 1E        btr         eax,1Eh
  000000006704D17A: 0F BA E8 1F        bts         eax,1Fh
  000000006704D17E: 89 44 24 50        mov         dword ptr [rsp+50h],eax
  000000006704D182: 48 89 74 24 48     mov         qword ptr [rsp+48h],rsi
  000000006704D187: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006704D18A: 4C 8D 4C 24 48     lea         r9,[rsp+48h]
  000000006704D18F: 41 B8 05 00 00 00  mov         r8d,5
  000000006704D195: 48 8D 94 24 28 01  lea         rdx,[rsp+0000000000000128h]
                    00 00
  000000006704D19D: 48 8B CB           mov         rcx,rbx
  000000006704D1A0: FF 50 08           call        qword ptr [rax+8]
  000000006704D1A3: 90                 nop
  000000006704D1A4: 48 8B D0           mov         rdx,rax
  000000006704D1A7: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D1AC: E8 DF C4 0E 00     call        0000000067139690
  000000006704D1B1: 90                 nop
  000000006704D1B2: 48 8D 8C 24 28 01  lea         rcx,[rsp+0000000000000128h]
                    00 00
  000000006704D1BA: E8 B1 B1 0E 00     call        0000000067138370
  000000006704D1BF: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D1C4: E8 17 B4 0E 00     call        00000000671385E0
  000000006704D1C9: 84 C0              test        al,al
  000000006704D1CB: 75 42              jne         000000006704D20F
  000000006704D1CD: 48 8D 94 24 98 01  lea         rdx,[rsp+0000000000000198h]
                    00 00
  000000006704D1D5: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D1DA: E8 51 CA 0E 00     call        0000000067139C30
  000000006704D1DF: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006704D1E2: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  000000006704D1E6: 0F B7 00           movzx       eax,word ptr [rax]
  000000006704D1E9: 48 8B 0D 48 6B 2A  mov         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704D1F0: 66 89 41 10        mov         word ptr [rcx+10h],ax
  000000006704D1F4: 48 8B 84 24 98 01  mov         rax,qword ptr [rsp+0000000000000198h]
                    00 00
  000000006704D1FC: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006704D200: 75 0D              jne         000000006704D20F
  000000006704D202: 48 8B 8C 24 98 01  mov         rcx,qword ptr [rsp+0000000000000198h]
                    00 00
  000000006704D20A: E8 C1 2E 02 00     call        00000000670700D0
  000000006704D20F: 8B 44 24 60        mov         eax,dword ptr [rsp+60h]
  000000006704D213: 25 00 00 00 C0     and         eax,0C0000000h
  000000006704D218: 0F BA F0 1E        btr         eax,1Eh
  000000006704D21C: 0F BA E8 1F        bts         eax,1Fh
  000000006704D220: 89 44 24 60        mov         dword ptr [rsp+60h],eax
  000000006704D224: 48 89 74 24 58     mov         qword ptr [rsp+58h],rsi
  000000006704D229: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006704D22C: 4C 8D 4C 24 58     lea         r9,[rsp+58h]
  000000006704D231: 41 B8 17 00 00 00  mov         r8d,17h
  000000006704D237: 48 8D 94 24 48 01  lea         rdx,[rsp+0000000000000148h]
                    00 00
  000000006704D23F: 48 8B CB           mov         rcx,rbx
  000000006704D242: FF 50 08           call        qword ptr [rax+8]
  000000006704D245: 90                 nop
  000000006704D246: 48 8B D0           mov         rdx,rax
  000000006704D249: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D24E: E8 3D C4 0E 00     call        0000000067139690
  000000006704D253: 90                 nop
  000000006704D254: 48 8D 8C 24 48 01  lea         rcx,[rsp+0000000000000148h]
                    00 00
  000000006704D25C: E8 0F B1 0E 00     call        0000000067138370
  000000006704D261: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D266: E8 75 B3 0E 00     call        00000000671385E0
  000000006704D26B: 84 C0              test        al,al
  000000006704D26D: 75 3A              jne         000000006704D2A9
  000000006704D26F: 48 8D 54 24 30     lea         rdx,[rsp+30h]
  000000006704D274: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D279: E8 B2 C9 0E 00     call        0000000067139C30
  000000006704D27E: 48 8B 08           mov         rcx,qword ptr [rax]
  000000006704D281: 48 8B 41 10        mov         rax,qword ptr [rcx+10h]
  000000006704D285: 0F B7 00           movzx       eax,word ptr [rax]
  000000006704D288: 48 8B 0D A9 6A 2A  mov         rcx,qword ptr [00000000672F3D38h]
                    00
  000000006704D28F: 66 89 41 12        mov         word ptr [rcx+12h],ax
  000000006704D293: 48 8B 44 24 30     mov         rax,qword ptr [rsp+30h]
  000000006704D298: F0 83 00 FF        lock add    dword ptr [rax],0FFFFFFFFh
  000000006704D29C: 75 0B              jne         000000006704D2A9
  000000006704D29E: 48 8B 4C 24 30     mov         rcx,qword ptr [rsp+30h]
  000000006704D2A3: E8 28 2E 02 00     call        00000000670700D0
  000000006704D2A8: 90                 nop
  000000006704D2A9: 48 8D 4C 24 20     lea         rcx,[rsp+20h]
  000000006704D2AE: E8 BD B0 0E 00     call        0000000067138370
  000000006704D2B3: 48 81 C4 60 01 00  add         rsp,160h
                    00
  000000006704D2BA: 5F                 pop         rdi
  000000006704D2BB: 5E                 pop         rsi
  000000006704D2BC: 5B                 pop         rbx
  000000006704D2BD: C3                 ret

  Summary

      1A7000 .text

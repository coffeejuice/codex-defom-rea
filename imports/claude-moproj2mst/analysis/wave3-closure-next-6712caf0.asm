Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006712CAF0: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000006712CAF5: 53                 push        rbx
  000000006712CAF6: 56                 push        rsi
  000000006712CAF7: 57                 push        rdi
  000000006712CAF8: 48 81 EC D0 01 00  sub         rsp,1D0h
                    00
  000000006712CAFF: 48 C7 84 24 30 01  mov         qword ptr [rsp+0000000000000130h],0FFFFFFFFFFFFFFFEh
                    00 00 FE FF FF FF
  000000006712CB0B: 4C 8B CA           mov         r9,rdx
  000000006712CB0E: 48 8B D9           mov         rbx,rcx
  000000006712CB11: 0F B7 4A 10        movzx       ecx,word ptr [rdx+10h]
  000000006712CB15: 8D 41 FF           lea         eax,[rcx-1]
  000000006712CB18: 83 F8 46           cmp         eax,46h
  000000006712CB1B: 0F 87 A4 03 00 00  ja          000000006712CEC5
  000000006712CB21: 48 98              cdqe
  000000006712CB23: 48 8D 15 D6 34 ED  lea         rdx,[0000000067000000h]
                    FF
  000000006712CB2A: 0F B6 84 02 08 CF  movzx       eax,byte ptr [rdx+rax+000000000012CF08h]
                    12 00
  000000006712CB32: 44 8B 84 82 EC CE  mov         r8d,dword ptr [rdx+rax*4+000000000012CEECh]
                    12 00
  000000006712CB3A: 4C 03 C2           add         r8,rdx
  000000006712CB3D: 41 FF E0           jmp         r8
  000000006712CB40: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712CB43: 49 8B D1           mov         rdx,r9
  000000006712CB46: 48 8B CB           mov         rcx,rbx
  000000006712CB49: FF 50 30           call        qword ptr [rax+30h]
  000000006712CB4C: E9 8A 03 00 00     jmp         000000006712CEDB
  000000006712CB51: 48 8B 4B 08        mov         rcx,qword ptr [rbx+8]
  000000006712CB55: E8 F6 FB FF FF     call        000000006712C750
  000000006712CB5A: E9 7C 03 00 00     jmp         000000006712CEDB
  000000006712CB5F: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712CB62: 49 8B D1           mov         rdx,r9
  000000006712CB65: 48 8B CB           mov         rcx,rbx
  000000006712CB68: FF 50 38           call        qword ptr [rax+38h]
  000000006712CB6B: E9 6B 03 00 00     jmp         000000006712CEDB
  000000006712CB70: 48 85 DB           test        rbx,rbx
  000000006712CB73: 0F 84 62 03 00 00  je          000000006712CEDB
  000000006712CB79: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712CB7C: BA 01 00 00 00     mov         edx,1
  000000006712CB81: 48 8B CB           mov         rcx,rbx
  000000006712CB84: FF 50 18           call        qword ptr [rax+18h]
  000000006712CB87: E9 4F 03 00 00     jmp         000000006712CEDB
  000000006712CB8C: 49 8B 41 18        mov         rax,qword ptr [r9+18h]
  000000006712CB90: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000006712CB95: 41 8B 41 20        mov         eax,dword ptr [r9+20h]
  000000006712CB99: 89 44 24 78        mov         dword ptr [rsp+78h],eax
  000000006712CB9D: C7 44 24 7C 01 00  mov         dword ptr [rsp+7Ch],1
                    00 00
  000000006712CBA5: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006712CBA9: 48 8B 78 58        mov         rdi,qword ptr [rax+58h]
  000000006712CBAD: 48 89 BC 24 F8 01  mov         qword ptr [rsp+00000000000001F8h],rdi
                    00 00
  000000006712CBB5: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  000000006712CBBA: 48 89 48 58        mov         qword ptr [rax+58h],rcx
  000000006712CBBE: 49 8B 01           mov         rax,qword ptr [r9]
  000000006712CBC1: 48 8B D3           mov         rdx,rbx
  000000006712CBC4: 49 8B C9           mov         rcx,r9
  000000006712CBC7: FF 50 08           call        qword ptr [rax+8]
  000000006712CBCA: 90                 nop
  000000006712CBCB: 8B 44 24 7C        mov         eax,dword ptr [rsp+7Ch]
  000000006712CBCF: 83 F8 01           cmp         eax,1
  000000006712CBD2: 75 0C              jne         000000006712CBE0
  000000006712CBD4: 48 8B 43 08        mov         rax,qword ptr [rbx+8]
  000000006712CBD8: 48 89 78 58        mov         qword ptr [rax+58h],rdi
  000000006712CBDC: 8B 44 24 7C        mov         eax,dword ptr [rsp+7Ch]
  000000006712CBE0: 48 85 FF           test        rdi,rdi
  000000006712CBE3: 0F 84 F2 02 00 00  je          000000006712CEDB
  000000006712CBE9: 89 47 0C           mov         dword ptr [rdi+0Ch],eax
  000000006712CBEC: E9 EA 02 00 00     jmp         000000006712CEDB
  000000006712CBF1: 48 8B 73 08        mov         rsi,qword ptr [rbx+8]
  000000006712CBF5: 48 8B 46 40        mov         rax,qword ptr [rsi+40h]
  000000006712CBF9: 48 8B 78 20        mov         rdi,qword ptr [rax+20h]
  000000006712CBFD: 48 85 FF           test        rdi,rdi
  000000006712CC00: 0F 84 D5 02 00 00  je          000000006712CEDB
  000000006712CC06: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006712CC09: 4C 8B C3           mov         r8,rbx
  000000006712CC0C: 48 8D 94 24 F8 01  lea         rdx,[rsp+00000000000001F8h]
                    00 00
  000000006712CC14: 48 8B CF           mov         rcx,rdi
  000000006712CC17: FF 90 90 00 00 00  call        qword ptr [rax+0000000000000090h]
  000000006712CC1D: 90                 nop
  000000006712CC1E: 48 8B 84 24 F8 01  mov         rax,qword ptr [rsp+00000000000001F8h]
                    00 00
  000000006712CC26: 8B 50 08           mov         edx,dword ptr [rax+8]
  000000006712CC29: 39 50 0C           cmp         dword ptr [rax+0Ch],edx
  000000006712CC2C: 0F 84 84 02 00 00  je          000000006712CEB6
  000000006712CC32: 81 4E 20 00 01 00  or          dword ptr [rsi+20h],100h
                    00
  000000006712CC39: 48 8B 07           mov         rax,qword ptr [rdi]
  000000006712CC3C: 48 8B D3           mov         rdx,rbx
  000000006712CC3F: 48 8B CF           mov         rcx,rdi
  000000006712CC42: FF 90 88 00 00 00  call        qword ptr [rax+0000000000000088h]
  000000006712CC48: 81 66 20 FF FE FF  and         dword ptr [rsi+20h],0FFFFFEFFh
                    FF
  000000006712CC4F: B9 08 00 00 00     mov         ecx,8
  000000006712CC54: E8 2D BE 04 00     call        0000000067178A86
  000000006712CC59: 48 8B F8           mov         rdi,rax
  000000006712CC5C: 48 89 84 24 08 02  mov         qword ptr [rsp+0000000000000208h],rax
                    00 00
  000000006712CC64: 48 85 C0           test        rax,rax
  000000006712CC67: 74 2F              je          000000006712CC98
  000000006712CC69: 48 8B 8C 24 F8 01  mov         rcx,qword ptr [rsp+00000000000001F8h]
                    00 00
  000000006712CC71: 48 89 08           mov         qword ptr [rax],rcx
  000000006712CC74: 48 8B 8C 24 F8 01  mov         rcx,qword ptr [rsp+00000000000001F8h]
                    00 00
  000000006712CC7C: F0 83 01 01        lock add    dword ptr [rcx],1
  000000006712CC80: 48 8B 00           mov         rax,qword ptr [rax]
  000000006712CC83: F6 40 10 01        test        byte ptr [rax+10h],1
  000000006712CC87: 75 0B              jne         000000006712CC94
  000000006712CC89: 8B 50 04           mov         edx,dword ptr [rax+4]
  000000006712CC8C: 48 8B CF           mov         rcx,rdi
  000000006712CC8F: E8 CC DE FF FF     call        000000006712AB60
  000000006712CC94: 33 C0              xor         eax,eax
  000000006712CC96: EB 05              jmp         000000006712CC9D
  000000006712CC98: 33 C0              xor         eax,eax
  000000006712CC9A: 48 8B F8           mov         rdi,rax
  000000006712CC9D: 48 89 BC 24 00 02  mov         qword ptr [rsp+0000000000000200h],rdi
                    00 00
  000000006712CCA5: 48 8D 8C 24 00 02  lea         rcx,[rsp+0000000000000200h]
                    00 00
  000000006712CCAD: 48 89 8C 24 80 00  mov         qword ptr [rsp+0000000000000080h],rcx
                    00 00
  000000006712CCB5: 48 8D 0D A0 DF 10  lea         rcx,[000000006723AC5Ch]
                    00
  000000006712CCBC: 48 89 8C 24 88 00  mov         qword ptr [rsp+0000000000000088h],rcx
                    00 00
  000000006712CCC4: 48 89 44 24 70     mov         qword ptr [rsp+70h],rax
  000000006712CCC9: 48 89 44 24 78     mov         qword ptr [rsp+78h],rax
  000000006712CCCE: 48 89 84 24 D0 00  mov         qword ptr [rsp+00000000000000D0h],rax
                    00 00
  000000006712CCD6: 48 89 84 24 D8 00  mov         qword ptr [rsp+00000000000000D8h],rax
                    00 00
  000000006712CCDE: 48 89 84 24 B0 00  mov         qword ptr [rsp+00000000000000B0h],rax
                    00 00
  000000006712CCE6: 48 89 84 24 B8 00  mov         qword ptr [rsp+00000000000000B8h],rax
                    00 00
  000000006712CCEE: 48 89 84 24 F0 00  mov         qword ptr [rsp+00000000000000F0h],rax
                    00 00
  000000006712CCF6: 48 89 84 24 F8 00  mov         qword ptr [rsp+00000000000000F8h],rax
                    00 00
  000000006712CCFE: 48 89 84 24 90 00  mov         qword ptr [rsp+0000000000000090h],rax
                    00 00
  000000006712CD06: 48 89 84 24 98 00  mov         qword ptr [rsp+0000000000000098h],rax
                    00 00
  000000006712CD0E: 48 89 84 24 A0 00  mov         qword ptr [rsp+00000000000000A0h],rax
                    00 00
  000000006712CD16: 48 89 84 24 A8 00  mov         qword ptr [rsp+00000000000000A8h],rax
                    00 00
  000000006712CD1E: 48 89 84 24 C0 00  mov         qword ptr [rsp+00000000000000C0h],rax
                    00 00
  000000006712CD26: 48 89 84 24 C8 00  mov         qword ptr [rsp+00000000000000C8h],rax
                    00 00
  000000006712CD2E: 48 89 84 24 E0 00  mov         qword ptr [rsp+00000000000000E0h],rax
                    00 00
  000000006712CD36: 48 89 84 24 E8 00  mov         qword ptr [rsp+00000000000000E8h],rax
                    00 00
  000000006712CD3E: 48 89 84 24 00 01  mov         qword ptr [rsp+0000000000000100h],rax
                    00 00
  000000006712CD46: 48 89 84 24 08 01  mov         qword ptr [rsp+0000000000000108h],rax
                    00 00
  000000006712CD4E: 0F 28 44 24 70     movaps      xmm0,xmmword ptr [rsp+70h]
  000000006712CD53: 66 0F 7F 84 24 10  movdqa      xmmword ptr [rsp+0000000000000110h],xmm0
                    01 00 00
  000000006712CD5C: 0F 28 8C 24 D0 00  movaps      xmm1,xmmword ptr [rsp+00000000000000D0h]
                    00 00
  000000006712CD64: 66 0F 7F 8C 24 B0  movdqa      xmmword ptr [rsp+00000000000001B0h],xmm1
                    01 00 00
  000000006712CD6D: 0F 28 84 24 B0 00  movaps      xmm0,xmmword ptr [rsp+00000000000000B0h]
                    00 00
  000000006712CD75: 66 0F 7F 84 24 50  movdqa      xmmword ptr [rsp+0000000000000150h],xmm0
                    01 00 00
  000000006712CD7E: 0F 28 8C 24 F0 00  movaps      xmm1,xmmword ptr [rsp+00000000000000F0h]
                    00 00
  000000006712CD86: 66 0F 7F 8C 24 90  movdqa      xmmword ptr [rsp+0000000000000190h],xmm1
                    01 00 00
  000000006712CD8F: 0F 28 84 24 90 00  movaps      xmm0,xmmword ptr [rsp+0000000000000090h]
                    00 00
  000000006712CD97: 66 0F 7F 84 24 70  movdqa      xmmword ptr [rsp+0000000000000170h],xmm0
                    01 00 00
  000000006712CDA0: 0F 28 8C 24 A0 00  movaps      xmm1,xmmword ptr [rsp+00000000000000A0h]
                    00 00
  000000006712CDA8: 66 0F 7F 8C 24 20  movdqa      xmmword ptr [rsp+0000000000000120h],xmm1
                    01 00 00
  000000006712CDB1: 0F 28 84 24 C0 00  movaps      xmm0,xmmword ptr [rsp+00000000000000C0h]
                    00 00
  000000006712CDB9: 66 0F 7F 84 24 40  movdqa      xmmword ptr [rsp+0000000000000140h],xmm0
                    01 00 00
  000000006712CDC2: 0F 28 8C 24 E0 00  movaps      xmm1,xmmword ptr [rsp+00000000000000E0h]
                    00 00
  000000006712CDCA: 66 0F 7F 8C 24 60  movdqa      xmmword ptr [rsp+0000000000000160h],xmm1
                    01 00 00
  000000006712CDD3: 0F 28 84 24 00 01  movaps      xmm0,xmmword ptr [rsp+0000000000000100h]
                    00 00
  000000006712CDDB: 66 0F 7F 84 24 80  movdqa      xmmword ptr [rsp+0000000000000180h],xmm0
                    01 00 00
  000000006712CDE4: 0F 28 8C 24 80 00  movaps      xmm1,xmmword ptr [rsp+0000000000000080h]
                    00 00
  000000006712CDEC: 66 0F 7F 8C 24 A0  movdqa      xmmword ptr [rsp+00000000000001A0h],xmm1
                    01 00 00
  000000006712CDF5: 48 89 84 24 80 00  mov         qword ptr [rsp+0000000000000080h],rax
                    00 00
  000000006712CDFD: 48 89 84 24 88 00  mov         qword ptr [rsp+0000000000000088h],rax
                    00 00
  000000006712CE05: 0F 28 84 24 80 00  movaps      xmm0,xmmword ptr [rsp+0000000000000080h]
                    00 00
  000000006712CE0D: 66 0F 7F 84 24 C0  movdqa      xmmword ptr [rsp+00000000000001C0h],xmm0
                    01 00 00
  000000006712CE16: 48 8D 84 24 10 01  lea         rax,[rsp+0000000000000110h]
                    00 00
  000000006712CE1E: 48 89 44 24 68     mov         qword ptr [rsp+68h],rax
  000000006712CE23: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  000000006712CE2B: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  000000006712CE30: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  000000006712CE38: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  000000006712CE3D: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  000000006712CE45: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  000000006712CE4A: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  000000006712CE52: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000006712CE57: 48 8D 84 24 20 01  lea         rax,[rsp+0000000000000120h]
                    00 00
  000000006712CE5F: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  000000006712CE64: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  000000006712CE6C: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  000000006712CE71: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  000000006712CE79: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  000000006712CE7E: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  000000006712CE86: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  000000006712CE8B: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  000000006712CE93: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  000000006712CE98: 4C 8D 8C 24 C0 01  lea         r9,[rsp+00000000000001C0h]
                    00 00
  000000006712CEA0: 41 B8 02 00 00 00  mov         r8d,2
  000000006712CEA6: 48 8D 15 CB F9 10  lea         rdx,[000000006723C878h]
                    00
  000000006712CEAD: 48 8B CB           mov         rcx,rbx
  000000006712CEB0: E8 2B 4F FF FF     call        0000000067121DE0
  000000006712CEB5: 90                 nop
  000000006712CEB6: 48 8D 8C 24 F8 01  lea         rcx,[rsp+00000000000001F8h]
                    00 00
  000000006712CEBE: E8 6D F8 F1 FF     call        000000006704C730
  000000006712CEC3: EB 16              jmp         000000006712CEDB
  000000006712CEC5: B8 E8 03 00 00     mov         eax,3E8h
  000000006712CECA: 66 3B C8           cmp         cx,ax
  000000006712CECD: 72 10              jb          000000006712CEDF
  000000006712CECF: 48 8B 03           mov         rax,qword ptr [rbx]
  000000006712CED2: 49 8B D1           mov         rdx,r9
  000000006712CED5: 48 8B CB           mov         rcx,rbx
  000000006712CED8: FF 50 40           call        qword ptr [rax+40h]
  000000006712CEDB: B0 01              mov         al,1
  000000006712CEDD: EB 02              jmp         000000006712CEE1
  000000006712CEDF: 32 C0              xor         al,al
  000000006712CEE1: 48 81 C4 D0 01 00  add         rsp,1D0h
                    00
  000000006712CEE8: 5F                 pop         rdi
  000000006712CEE9: 5E                 pop         rsi
  000000006712CEEA: 5B                 pop         rbx
  000000006712CEEB: C3                 ret
  000000006712CEEC: 40 CB              retf
  000000006712CEEE: 12 00              adc         al,byte ptr [rax]
  000000006712CEF0: F1
  000000006712CEF1: CB                 retf
  000000006712CEF2: 12 00              adc         al,byte ptr [rax]
  000000006712CEF4: 8C CB              mov         ebx,cs
  000000006712CEF6: 12 00              adc         al,byte ptr [rax]
  000000006712CEF8: 70 CB              jo          000000006712CEC5
  000000006712CEFA: 12 00              adc         al,byte ptr [rax]
  000000006712CEFC: 51                 push        rcx
  000000006712CEFD: CB                 retf
  000000006712CEFE: 12 00              adc         al,byte ptr [rax]
  000000006712CF00: 5F                 pop         rdi
  000000006712CF01: CB                 retf
  000000006712CF02: 12 00              adc         al,byte ptr [rax]
  000000006712CF04: C5 CE 12 00        vmovsldup   ymm0,ymmword ptr [rax]
  000000006712CF08: 00 06              add         byte ptr [rsi],al
  000000006712CF0A: 06
  000000006712CF0B: 06
  000000006712CF0C: 06
  000000006712CF0D: 06
  000000006712CF0E: 06
  000000006712CF0F: 06
  000000006712CF10: 06
  000000006712CF11: 06
  000000006712CF12: 06
  000000006712CF13: 06
  000000006712CF14: 06
  000000006712CF15: 06
  000000006712CF16: 06
  000000006712CF17: 06
  000000006712CF18: 06
  000000006712CF19: 06
  000000006712CF1A: 06
  000000006712CF1B: 06
  000000006712CF1C: 06
  000000006712CF1D: 01 06              add         dword ptr [rsi],eax
  000000006712CF1F: 06
  000000006712CF20: 06
  000000006712CF21: 06
  000000006712CF22: 06
  000000006712CF23: 06
  000000006712CF24: 06
  000000006712CF25: 06
  000000006712CF26: 06
  000000006712CF27: 06
  000000006712CF28: 06
  000000006712CF29: 06
  000000006712CF2A: 06
  000000006712CF2B: 06
  000000006712CF2C: 06
  000000006712CF2D: 06
  000000006712CF2E: 06
  000000006712CF2F: 06
  000000006712CF30: 06
  000000006712CF31: 06
  000000006712CF32: 02 06              add         al,byte ptr [rsi]
  000000006712CF34: 06
  000000006712CF35: 06
  000000006712CF36: 06
  000000006712CF37: 06
  000000006712CF38: 06
  000000006712CF39: 06
  000000006712CF3A: 06
  000000006712CF3B: 03 06              add         eax,dword ptr [rsi]
  000000006712CF3D: 06
  000000006712CF3E: 06
  000000006712CF3F: 06
  000000006712CF40: 06
  000000006712CF41: 06
  000000006712CF42: 06
  000000006712CF43: 06
  000000006712CF44: 06
  000000006712CF45: 06
  000000006712CF46: 06
  000000006712CF47: 06
  000000006712CF48: 06
  000000006712CF49: 06
  000000006712CF4A: 04 05              add         al,5
  000000006712CF4C: 05
  000000006712CF4D: 05
  000000006712CF4E: 05

  Summary

      1A7000 .text

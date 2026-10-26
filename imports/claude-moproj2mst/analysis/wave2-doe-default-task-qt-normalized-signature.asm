
Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  000000006711FF60: 4C 8B DC           mov         r11,rsp
  000000006711FF63: 49 89 4B 08        mov         qword ptr [r11+8],rcx
  000000006711FF67: 53                 push        rbx
  000000006711FF68: 55                 push        rbp
  000000006711FF69: 56                 push        rsi
  000000006711FF6A: 57                 push        rdi
  000000006711FF6B: 41 54              push        r12
  000000006711FF6D: 41 55              push        r13
  000000006711FF6F: 48 81 EC 68 01 00  sub         rsp,168h
                    00
  000000006711FF76: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  000000006711FF7F: 48 8B DA           mov         rbx,rdx
  000000006711FF82: 4C 8B E9           mov         r13,rcx
  000000006711FF85: 33 ED              xor         ebp,ebp
  000000006711FF87: 41 89 6B 10        mov         dword ptr [r11+10h],ebp
  000000006711FF8B: 48 8D 05 6E 09 1D  lea         rax,[00000000672F0900h]
                    00
  000000006711FF92: 49 89 43 18        mov         qword ptr [r11+18h],rax
  000000006711FF96: F0 83 05 62 09 1D  lock add    dword ptr [00000000672F0900h],1
                    00 01
  000000006711FF9E: 48 85 D2           test        rdx,rdx
  000000006711FFA1: 0F 84 D6 01 00 00  je          000000006712017D
  000000006711FFA7: 40 38 2A           cmp         byte ptr [rdx],bpl
  000000006711FFAA: 0F 84 CD 01 00 00  je          000000006712017D
  000000006711FFB0: 33 C0              xor         eax,eax
  000000006711FFB2: 49 83 CC FF        or          r12,0FFFFFFFFFFFFFFFFh
  000000006711FFB6: 49 8B CC           mov         rcx,r12
  000000006711FFB9: 48 8B FA           mov         rdi,rdx
  000000006711FFBC: F2 AE              repne scas  byte ptr [rdi]
  000000006711FFBE: 48 F7 D1           not         rcx
  000000006711FFC1: 48 8D 79 FF        lea         rdi,[rcx-1]
  000000006711FFC5: 89 4C 24 44        mov         dword ptr [rsp+44h],ecx
  000000006711FFC9: 81 F9 00 01 00 00  cmp         ecx,100h
  000000006711FFCF: 7E 29              jle         000000006711FFFA
  000000006711FFD1: 48 63 C9           movsxd      rcx,ecx
  000000006711FFD4: E8 C7 03 F0 FF     call        00000000670203A0
  000000006711FFD9: 48 8B F0           mov         rsi,rax
  000000006711FFDC: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  000000006711FFE1: 48 85 C0           test        rax,rax
  000000006711FFE4: 75 0A              jne         000000006711FFF0
  000000006711FFE6: E8 C5 BF EF FF     call        000000006701BFB0
  000000006711FFEB: 48 8B 74 24 48     mov         rsi,qword ptr [rsp+48h]
  000000006711FFF0: 8B 44 24 44        mov         eax,dword ptr [rsp+44h]
  000000006711FFF4: 89 44 24 40        mov         dword ptr [rsp+40h],eax
  000000006711FFF8: EB 12              jmp         000000006712000C
  000000006711FFFA: 48 8D 74 24 50     lea         rsi,[rsp+50h]
  000000006711FFFF: 48 89 74 24 48     mov         qword ptr [rsp+48h],rsi
  0000000067120004: C7 44 24 40 00 01  mov         dword ptr [rsp+40h],100h
                    00 00
  000000006712000C: 48 8B D6           mov         rdx,rsi
  000000006712000F: 48 8B CB           mov         rcx,rbx
  0000000067120012: E8 99 E6 FF FF     call        000000006711E6B0
  0000000067120017: 48 8B 94 24 B0 01  mov         rdx,qword ptr [rsp+00000000000001B0h]
                    00 00
  000000006712001F: 8B 02              mov         eax,dword ptr [rdx]
  0000000067120021: 83 F8 01           cmp         eax,1
  0000000067120024: 75 0D              jne         0000000067120033
  0000000067120026: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  000000006712002E: 3B 78 04           cmp         edi,dword ptr [rax+4]
  0000000067120031: 7E 17              jle         000000006712004A
  0000000067120033: 8B D7              mov         edx,edi
  0000000067120035: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  000000006712003D: E8 CE D6 F0 FF     call        000000006702D710
  0000000067120042: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  000000006712004A: 8B DD              mov         ebx,ebp
  000000006712004C: 0F B6 0E           movzx       ecx,byte ptr [rsi]
  000000006712004F: 84 C9              test        cl,cl
  0000000067120051: 0F 84 D9 00 00 00  je          0000000067120130
  0000000067120057: 83 FD 01           cmp         ebp,1
  000000006712005A: 0F 85 9D 00 00 00  jne         00000000671200FD
  0000000067120060: 48 8B FE           mov         rdi,rsi
  0000000067120063: 84 C9              test        cl,cl
  0000000067120065: 74 26              je          000000006712008D
  0000000067120067: 85 DB              test        ebx,ebx
  0000000067120069: 75 0B              jne         0000000067120076
  000000006712006B: 0F B6 07           movzx       eax,byte ptr [rdi]
  000000006712006E: 3C 2C              cmp         al,2Ch
  0000000067120070: 74 1B              je          000000006712008D
  0000000067120072: 3C 29              cmp         al,29h
  0000000067120074: 74 17              je          000000006712008D
  0000000067120076: 0F B6 07           movzx       eax,byte ptr [rdi]
  0000000067120079: 3C 3C              cmp         al,3Ch
  000000006712007B: 75 02              jne         000000006712007F
  000000006712007D: FF C3              inc         ebx
  000000006712007F: 3C 3E              cmp         al,3Eh
  0000000067120081: 75 02              jne         0000000067120085
  0000000067120083: FF CB              dec         ebx
  0000000067120085: 48 FF C7           inc         rdi
  0000000067120088: 80 3F 00           cmp         byte ptr [rdi],0
  000000006712008B: 75 DA              jne         0000000067120067
  000000006712008D: 4C 8B C7           mov         r8,rdi
  0000000067120090: 4C 2B C6           sub         r8,rsi
  0000000067120093: 48 8B D6           mov         rdx,rsi
  0000000067120096: 48 8D 0D DB A5 11  lea         rcx,[000000006723A678h]
                    00
  000000006712009D: FF 15 A5 85 08 00  call        qword ptr [00000000671A8648h]
  00000000671200A3: 85 C0              test        eax,eax
  00000000671200A5: 74 4E              je          00000000671200F5
  00000000671200A7: C6 44 24 20 01     mov         byte ptr [rsp+20h],1
  00000000671200AC: 45 33 C9           xor         r9d,r9d
  00000000671200AF: 4C 8B C7           mov         r8,rdi
  00000000671200B2: 48 8B D6           mov         rdx,rsi
  00000000671200B5: 48 8D 8C 24 B8 01  lea         rcx,[rsp+00000000000001B8h]
                    00 00
  00000000671200BD: E8 4E EF FF FF     call        000000006711F010
  00000000671200C2: 90                 nop
  00000000671200C3: 48 8B D0           mov         rdx,rax
  00000000671200C6: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  00000000671200CE: E8 ED D8 F0 FF     call        000000006702D9C0
  00000000671200D3: 90                 nop
  00000000671200D4: 48 8B 84 24 B8 01  mov         rax,qword ptr [rsp+00000000000001B8h]
                    00 00
  00000000671200DC: 41 8B CC           mov         ecx,r12d
  00000000671200DF: F0 0F C1 08        lock xadd   dword ptr [rax],ecx
  00000000671200E3: 83 C1 FF           add         ecx,0FFFFFFFFh
  00000000671200E6: 75 0D              jne         00000000671200F5
  00000000671200E8: 48 8B 8C 24 B8 01  mov         rcx,qword ptr [rsp+00000000000001B8h]
                    00 00
  00000000671200F0: E8 BB 02 F0 FF     call        00000000670203B0
  00000000671200F5: 48 8B F7           mov         rsi,rdi
  00000000671200F8: 80 3F 00           cmp         byte ptr [rdi],0
  00000000671200FB: 74 2B              je          0000000067120128
  00000000671200FD: 0F B6 16           movzx       edx,byte ptr [rsi]
  0000000067120100: 80 FA 28           cmp         dl,28h
  0000000067120103: 75 02              jne         0000000067120107
  0000000067120105: FF C5              inc         ebp
  0000000067120107: 80 FA 29           cmp         dl,29h
  000000006712010A: 75 02              jne         000000006712010E
  000000006712010C: FF CD              dec         ebp
  000000006712010E: 48 8D 8C 24 B0 01  lea         rcx,[rsp+00000000000001B0h]
                    00 00
  0000000067120116: E8 95 DB F0 FF     call        000000006702DCB0
  000000006712011B: 48 FF C6           inc         rsi
  000000006712011E: 8A 0E              mov         cl,byte ptr [rsi]
  0000000067120120: 84 C9              test        cl,cl
  0000000067120122: 0F 85 2F FF FF FF  jne         0000000067120057
  0000000067120128: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000067120130: 49 89 45 00        mov         qword ptr [r13],rax
  0000000067120134: F0 83 00 01        lock add    dword ptr [rax],1
  0000000067120138: C7 84 24 A8 01 00  mov         dword ptr [rsp+00000000000001A8h],1
                    00 01 00 00 00
  0000000067120143: 48 8D 44 24 50     lea         rax,[rsp+50h]
  0000000067120148: 48 8B 4C 24 48     mov         rcx,qword ptr [rsp+48h]
  000000006712014D: 48 3B C8           cmp         rcx,rax
  0000000067120150: 74 06              je          0000000067120158
  0000000067120152: E8 59 02 F0 FF     call        00000000670203B0
  0000000067120157: 90                 nop
  0000000067120158: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000067120160: F0 44 0F C1 20     lock xadd   dword ptr [rax],r12d
  0000000067120165: 41 83 C4 FF        add         r12d,0FFFFFFFFh
  0000000067120169: 75 0D              jne         0000000067120178
  000000006712016B: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000067120173: E8 38 02 F0 FF     call        00000000670203B0
  0000000067120178: 49 8B C5           mov         rax,r13
  000000006712017B: EB 41              jmp         00000000671201BE
  000000006712017D: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  0000000067120185: 48 89 01           mov         qword ptr [rcx],rax
  0000000067120188: F0 83 00 01        lock add    dword ptr [rax],1
  000000006712018C: C7 84 24 A8 01 00  mov         dword ptr [rsp+00000000000001A8h],1
                    00 01 00 00 00
  0000000067120197: 48 8B 84 24 B0 01  mov         rax,qword ptr [rsp+00000000000001B0h]
                    00 00
  000000006712019F: 49 83 CC FF        or          r12,0FFFFFFFFFFFFFFFFh
  00000000671201A3: F0 44 0F C1 20     lock xadd   dword ptr [rax],r12d
  00000000671201A8: 41 83 C4 FF        add         r12d,0FFFFFFFFh
  00000000671201AC: 75 0D              jne         00000000671201BB
  00000000671201AE: 48 8B 8C 24 B0 01  mov         rcx,qword ptr [rsp+00000000000001B0h]
                    00 00
  00000000671201B6: E8 F5 01 F0 FF     call        00000000670203B0
  00000000671201BB: 49 8B C5           mov         rax,r13
  00000000671201BE: 48 81 C4 68 01 00  add         rsp,168h
                    00
  00000000671201C5: 41 5D              pop         r13
  00000000671201C7: 41 5C              pop         r12
  00000000671201C9: 5F                 pop         rdi
  00000000671201CA: 5E                 pop         rsi
  00000000671201CB: 5D                 pop         rbp
  00000000671201CC: 5B                 pop         rbx
  00000000671201CD: C3                 ret
  00000000671201CE: CC                 int         3
  00000000671201CF: CC                 int         3

  Summary

      1A7000 .text

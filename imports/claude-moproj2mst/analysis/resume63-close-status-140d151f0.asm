
Dump of file evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D151F0: 44 88 4C 24 20     mov         byte ptr [rsp+20h],r9b
  0000000140D151F5: 4C 89 44 24 18     mov         qword ptr [rsp+18h],r8
  0000000140D151FA: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D151FF: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D15204: 48 83 EC 48        sub         rsp,48h
  0000000140D15208: 48 C7 44 24 30 FE  mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000140D15211: 8B 05 A1 E9 99 03  mov         eax,dword ptr [00000001446B3BB8h]
  0000000140D15217: 83 E0 01           and         eax,1
  0000000140D1521A: 85 C0              test        eax,eax
  0000000140D1521C: 75 29              jne         0000000140D15247
  0000000140D1521E: 8B 05 94 E9 99 03  mov         eax,dword ptr [00000001446B3BB8h]
  0000000140D15224: 83 C8 01           or          eax,1
  0000000140D15227: 89 05 8B E9 99 03  mov         dword ptr [00000001446B3BB8h],eax
  0000000140D1522D: 48 8D 0D 7C E9 99  lea         rcx,[00000001446B3BB0h]
                    03
  0000000140D15234: FF 15 86 F2 0E 01  call        qword ptr [0000000141E044C0h]
  0000000140D1523A: 48 8D 0D DF 64 0E  lea         rcx,[0000000141DFB720h]
                    01
  0000000140D15241: E8 F2 9A 75 00     call        000000014146ED38
  0000000140D15246: 90                 nop
  0000000140D15247: 0F B6 44 24 68     movzx       eax,byte ptr [rsp+68h]
  0000000140D1524C: 85 C0              test        eax,eax
  0000000140D1524E: 74 13              je          0000000140D15263
  0000000140D15250: 48 8B 44 24 50     mov         rax,qword ptr [rsp+50h]
  0000000140D15255: 48 8B 00           mov         rax,qword ptr [rax]
  0000000140D15258: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D1525D: FF 90 C0 01 00 00  call        qword ptr [rax+00000000000001C0h]
  0000000140D15263: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140D15268: 48 8D 0D 41 E9 99  lea         rcx,[00000001446B3BB0h]
                    03
  0000000140D1526F: FF 15 93 EC 0E 01  call        qword ptr [0000000141E03F08h]
  0000000140D15275: 0F B6 C0           movzx       eax,al
  0000000140D15278: 85 C0              test        eax,eax
  0000000140D1527A: 74 05              je          0000000140D15281
  0000000140D1527C: E9 83 00 00 00     jmp         0000000140D15304
  0000000140D15281: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  0000000140D15286: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D1528B: FF 15 07 4B 0F 01  call        qword ptr [0000000141E09D98h]
  0000000140D15291: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140D15296: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D1529B: FF 15 EF 4A 0F 01  call        qword ptr [0000000141E09D90h]
  0000000140D152A1: 48 8B 54 24 58     mov         rdx,qword ptr [rsp+58h]
  0000000140D152A6: 48 8D 0D 03 E9 99  lea         rcx,[00000001446B3BB0h]
                    03
  0000000140D152AD: FF 15 BD F1 0E 01  call        qword ptr [0000000141E04470h]
  0000000140D152B3: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140D152B8: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D152BD: FF 15 C5 4A 0F 01  call        qword ptr [0000000141E09D88h]
  0000000140D152C3: 90                 nop
  0000000140D152C4: 41 B9 01 00 00 00  mov         r9d,1
  0000000140D152CA: 45 33 C0           xor         r8d,r8d
  0000000140D152CD: BA 0B 00 00 00     mov         edx,0Bh
  0000000140D152D2: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140D152D7: FF 15 A3 4A 0F 01  call        qword ptr [0000000141E09D80h]
  0000000140D152DD: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  0000000140D152E2: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D152E7: FF 15 8B 4A 0F 01  call        qword ptr [0000000141E09D78h]
  0000000140D152ED: 48 8B 4C 24 50     mov         rcx,qword ptr [rsp+50h]
  0000000140D152F2: FF 15 78 4A 0F 01  call        qword ptr [0000000141E09D70h]
  0000000140D152F8: 90                 nop
  0000000140D152F9: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  0000000140D152FE: FF 15 84 27 0F 01  call        qword ptr [0000000141E07A88h]
  0000000140D15304: 48 83 C4 48        add         rsp,48h
  0000000140D15308: C3                 ret

  Summary

     1E02000 .text

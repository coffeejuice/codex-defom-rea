Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140D78220: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140D78225: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000140D7822A: 56                 push        rsi
  0000000140D7822B: 57                 push        rdi
  0000000140D7822C: 48 81 EC A8 01 00  sub         rsp,1A8h
                    00
  0000000140D78233: 48 8B 8C 24 C0 01  mov         rcx,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140D7823B: 48 8B 84 24 C8 01  mov         rax,qword ptr [rsp+00000000000001C8h]
                    00 00
  0000000140D78243: 48 89 41 50        mov         qword ptr [rcx+50h],rax
  0000000140D78247: 48 8B 8C 24 C0 01  mov         rcx,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140D7824F: E8 DC 98 FC FF     call        0000000140D41B30
  0000000140D78254: 83 F8 01           cmp         eax,1
  0000000140D78257: 75 0F              jne         0000000140D78268
  0000000140D78259: 48 8B 8C 24 C0 01  mov         rcx,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140D78261: E8 2A 02 00 00     call        0000000140D78490
  0000000140D78266: EB 21              jmp         0000000140D78289
  0000000140D78268: 48 8B 8C 24 C0 01  mov         rcx,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140D78270: E8 BB 98 FC FF     call        0000000140D41B30
  0000000140D78275: 83 F8 02           cmp         eax,2
  0000000140D78278: 75 0F              jne         0000000140D78289
  0000000140D7827A: 33 D2              xor         edx,edx
  0000000140D7827C: 48 8B 8C 24 C0 01  mov         rcx,qword ptr [rsp+00000000000001C0h]
                    00 00
  0000000140D78284: E8 67 27 00 00     call        0000000140D7A9F0
  0000000140D78289: 45 33 C0           xor         r8d,r8d
  0000000140D7828C: 33 D2              xor         edx,edx
  0000000140D7828E: 48 8D 4C 24 60     lea         rcx,[rsp+60h]
  0000000140D78293: FF 15 CF BE 08 01  call        qword ptr [0000000141E04168h]
  0000000140D78299: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  0000000140D782A1: 48 8B F0           mov         rsi,rax
  0000000140D782A4: B9 10 00 00 00     mov         ecx,10h
  0000000140D782A9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D782AB: 45 33 C0           xor         r8d,r8d
  0000000140D782AE: 33 D2              xor         edx,edx
  0000000140D782B0: 48 8D 4C 24 70     lea         rcx,[rsp+70h]
  0000000140D782B5: FF 15 AD BE 08 01  call        qword ptr [0000000141E04168h]
  0000000140D782BB: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140D782C3: 48 8B F0           mov         rsi,rax
  0000000140D782C6: B9 10 00 00 00     mov         ecx,10h
  0000000140D782CB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D782CD: 45 33 C0           xor         r8d,r8d
  0000000140D782D0: 33 D2              xor         edx,edx
  0000000140D782D2: 48 8D 8C 24 80 00  lea         rcx,[rsp+0000000000000080h]
                    00 00
  0000000140D782DA: FF 15 88 BE 08 01  call        qword ptr [0000000141E04168h]
  0000000140D782E0: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140D782E8: 48 8B F0           mov         rsi,rax
  0000000140D782EB: B9 10 00 00 00     mov         ecx,10h
  0000000140D782F0: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D782F2: 45 33 C0           xor         r8d,r8d
  0000000140D782F5: 33 D2              xor         edx,edx
  0000000140D782F7: 48 8D 8C 24 90 00  lea         rcx,[rsp+0000000000000090h]
                    00 00
  0000000140D782FF: FF 15 63 BE 08 01  call        qword ptr [0000000141E04168h]
  0000000140D78305: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  0000000140D7830D: 48 8B F0           mov         rsi,rax
  0000000140D78310: B9 10 00 00 00     mov         ecx,10h
  0000000140D78315: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D78317: 45 33 C0           xor         r8d,r8d
  0000000140D7831A: 33 D2              xor         edx,edx
  0000000140D7831C: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  0000000140D78324: FF 15 3E BE 08 01  call        qword ptr [0000000141E04168h]
  0000000140D7832A: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  0000000140D78332: 48 8B F0           mov         rsi,rax
  0000000140D78335: B9 10 00 00 00     mov         ecx,10h
  0000000140D7833A: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D7833C: 45 33 C0           xor         r8d,r8d
  0000000140D7833F: 33 D2              xor         edx,edx
  0000000140D78341: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  0000000140D78349: FF 15 19 BE 08 01  call        qword ptr [0000000141E04168h]
  0000000140D7834F: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  0000000140D78357: 48 8B F0           mov         rsi,rax
  0000000140D7835A: B9 10 00 00 00     mov         ecx,10h
  0000000140D7835F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D78361: 45 33 C0           xor         r8d,r8d
  0000000140D78364: 33 D2              xor         edx,edx
  0000000140D78366: 48 8D 8C 24 C0 00  lea         rcx,[rsp+00000000000000C0h]
                    00 00
  0000000140D7836E: FF 15 F4 BD 08 01  call        qword ptr [0000000141E04168h]
  0000000140D78374: 48 8D BC 24 30 01  lea         rdi,[rsp+0000000000000130h]
                    00 00
  0000000140D7837C: 48 8B F0           mov         rsi,rax
  0000000140D7837F: B9 10 00 00 00     mov         ecx,10h
  0000000140D78384: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D78386: 45 33 C0           xor         r8d,r8d
  0000000140D78389: 33 D2              xor         edx,edx
  0000000140D7838B: 48 8D 8C 24 D0 00  lea         rcx,[rsp+00000000000000D0h]
                    00 00
  0000000140D78393: FF 15 CF BD 08 01  call        qword ptr [0000000141E04168h]
  0000000140D78399: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  0000000140D783A1: 48 8B F0           mov         rsi,rax
  0000000140D783A4: B9 10 00 00 00     mov         ecx,10h
  0000000140D783A9: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D783AB: 45 33 C0           xor         r8d,r8d
  0000000140D783AE: 33 D2              xor         edx,edx
  0000000140D783B0: 48 8D 8C 24 E0 00  lea         rcx,[rsp+00000000000000E0h]
                    00 00
  0000000140D783B8: FF 15 AA BD 08 01  call        qword ptr [0000000141E04168h]
  0000000140D783BE: 48 8D BC 24 10 01  lea         rdi,[rsp+0000000000000110h]
                    00 00
  0000000140D783C6: 48 8B F0           mov         rsi,rax
  0000000140D783C9: B9 10 00 00 00     mov         ecx,10h
  0000000140D783CE: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D783D0: 45 33 C0           xor         r8d,r8d
  0000000140D783D3: 33 D2              xor         edx,edx
  0000000140D783D5: 48 8D 8C 24 F0 00  lea         rcx,[rsp+00000000000000F0h]
                    00 00
  0000000140D783DD: FF 15 85 BD 08 01  call        qword ptr [0000000141E04168h]
  0000000140D783E3: 48 8D BC 24 00 01  lea         rdi,[rsp+0000000000000100h]
                    00 00
  0000000140D783EB: 48 8B F0           mov         rsi,rax
  0000000140D783EE: B9 10 00 00 00     mov         ecx,10h
  0000000140D783F3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140D783F5: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140D783FD: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140D78402: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  0000000140D7840A: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140D7840F: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  0000000140D78417: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140D7841C: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  0000000140D78424: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140D78429: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  0000000140D78431: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140D78436: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  0000000140D7843E: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140D78443: 48 8D 84 24 30 01  lea         rax,[rsp+0000000000000130h]
                    00 00
  0000000140D7844B: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140D78450: 48 8D 84 24 20 01  lea         rax,[rsp+0000000000000120h]
                    00 00
  0000000140D78458: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140D7845D: 4C 8D 8C 24 10 01  lea         r9,[rsp+0000000000000110h]
                    00 00
  0000000140D78465: 4C 8D 84 24 00 01  lea         r8,[rsp+0000000000000100h]
                    00 00
  0000000140D7846D: 48 8D 15 54 6D CC  lea         rdx,[0000000142A3F1C8h]
                    01
  0000000140D78474: 48 8D 0D 85 E2 FF  lea         rcx,[0000000143D76700h]
                    02
  0000000140D7847B: E8 40 6F 55 FF     call        00000001402CF3C0
  0000000140D78480: 48 81 C4 A8 01 00  add         rsp,1A8h
                    00
  0000000140D78487: 5F                 pop         rdi
  0000000140D78488: 5E                 pop         rsi
  0000000140D78489: C3                 ret

  Summary

     1E02000 .text

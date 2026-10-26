Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140130830: 48 89 54 24 10     mov         qword ptr [rsp+10h],rdx
  0000000140130835: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  000000014013083A: 56                 push        rsi
  000000014013083B: 57                 push        rdi
  000000014013083C: 48 81 EC D8 01 00  sub         rsp,1D8h
                    00
  0000000140130843: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014013084B: 48 83 C1 38        add         rcx,38h
  000000014013084F: 48 8B 94 24 F8 01  mov         rdx,qword ptr [rsp+00000000000001F8h]
                    00 00
  0000000140130857: E8 C4 C0 B8 00     call        0000000140CBC920
  000000014013085C: 48 C7 44 24 60 00  mov         qword ptr [rsp+60h],0
                    00 00 00
  0000000140130865: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  000000014013086D: 48 83 C1 38        add         rcx,38h
  0000000140130871: E8 7A A6 AE 00     call        0000000140C1AEF0
  0000000140130876: 48 85 C0           test        rax,rax
  0000000140130879: 74 1E              je          0000000140130899
  000000014013087B: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  0000000140130883: 48 83 C1 38        add         rcx,38h
  0000000140130887: E8 64 A6 AE 00     call        0000000140C1AEF0
  000000014013088C: 48 8B C8           mov         rcx,rax
  000000014013088F: E8 DC EE F2 FF     call        000000014005F770
  0000000140130894: 48 89 44 24 60     mov         qword ptr [rsp+60h],rax
  0000000140130899: 48 8B 54 24 60     mov         rdx,qword ptr [rsp+60h]
  000000014013089E: 48 8B 8C 24 F0 01  mov         rcx,qword ptr [rsp+00000000000001F0h]
                    00 00
  00000001401308A6: E8 75 72 24 00     call        0000000140377B20
  00000001401308AB: 4C 8D 44 24 60     lea         r8,[rsp+60h]
  00000001401308B0: 48 8D 15 91 7F 07  lea         rdx,[00000001421A8848h]
                    02
  00000001401308B7: 48 8D 8C 24 08 01  lea         rcx,[rsp+0000000000000108h]
                    00 00
  00000001401308BF: E8 BC 47 EF FF     call        0000000140025080
  00000001401308C4: 48 8D BC 24 C0 01  lea         rdi,[rsp+00000000000001C0h]
                    00 00
  00000001401308CC: 48 8B F0           mov         rsi,rax
  00000001401308CF: B9 10 00 00 00     mov         ecx,10h
  00000001401308D4: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401308D6: 48 8D BC 24 F8 00  lea         rdi,[rsp+00000000000000F8h]
                    00 00
  00000001401308DE: 48 8D B4 24 C0 01  lea         rsi,[rsp+00000000000001C0h]
                    00 00
  00000001401308E6: B9 10 00 00 00     mov         ecx,10h
  00000001401308EB: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401308ED: 45 33 C0           xor         r8d,r8d
  00000001401308F0: 33 D2              xor         edx,edx
  00000001401308F2: 48 8D 4C 24 68     lea         rcx,[rsp+68h]
  00000001401308F7: FF 15 6B 38 CD 01  call        qword ptr [0000000141E04168h]
  00000001401308FD: 48 8D BC 24 B0 01  lea         rdi,[rsp+00000000000001B0h]
                    00 00
  0000000140130905: 48 8B F0           mov         rsi,rax
  0000000140130908: B9 10 00 00 00     mov         ecx,10h
  000000014013090D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014013090F: 45 33 C0           xor         r8d,r8d
  0000000140130912: 33 D2              xor         edx,edx
  0000000140130914: 48 8D 4C 24 78     lea         rcx,[rsp+78h]
  0000000140130919: FF 15 49 38 CD 01  call        qword ptr [0000000141E04168h]
  000000014013091F: 48 8D BC 24 A0 01  lea         rdi,[rsp+00000000000001A0h]
                    00 00
  0000000140130927: 48 8B F0           mov         rsi,rax
  000000014013092A: B9 10 00 00 00     mov         ecx,10h
  000000014013092F: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140130931: 45 33 C0           xor         r8d,r8d
  0000000140130934: 33 D2              xor         edx,edx
  0000000140130936: 48 8D 8C 24 88 00  lea         rcx,[rsp+0000000000000088h]
                    00 00
  000000014013093E: FF 15 24 38 CD 01  call        qword ptr [0000000141E04168h]
  0000000140130944: 48 8D BC 24 90 01  lea         rdi,[rsp+0000000000000190h]
                    00 00
  000000014013094C: 48 8B F0           mov         rsi,rax
  000000014013094F: B9 10 00 00 00     mov         ecx,10h
  0000000140130954: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140130956: 45 33 C0           xor         r8d,r8d
  0000000140130959: 33 D2              xor         edx,edx
  000000014013095B: 48 8D 8C 24 98 00  lea         rcx,[rsp+0000000000000098h]
                    00 00
  0000000140130963: FF 15 FF 37 CD 01  call        qword ptr [0000000141E04168h]
  0000000140130969: 48 8D BC 24 80 01  lea         rdi,[rsp+0000000000000180h]
                    00 00
  0000000140130971: 48 8B F0           mov         rsi,rax
  0000000140130974: B9 10 00 00 00     mov         ecx,10h
  0000000140130979: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  000000014013097B: 45 33 C0           xor         r8d,r8d
  000000014013097E: 33 D2              xor         edx,edx
  0000000140130980: 48 8D 8C 24 A8 00  lea         rcx,[rsp+00000000000000A8h]
                    00 00
  0000000140130988: FF 15 DA 37 CD 01  call        qword ptr [0000000141E04168h]
  000000014013098E: 48 8D BC 24 70 01  lea         rdi,[rsp+0000000000000170h]
                    00 00
  0000000140130996: 48 8B F0           mov         rsi,rax
  0000000140130999: B9 10 00 00 00     mov         ecx,10h
  000000014013099E: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401309A0: 45 33 C0           xor         r8d,r8d
  00000001401309A3: 33 D2              xor         edx,edx
  00000001401309A5: 48 8D 8C 24 B8 00  lea         rcx,[rsp+00000000000000B8h]
                    00 00
  00000001401309AD: FF 15 B5 37 CD 01  call        qword ptr [0000000141E04168h]
  00000001401309B3: 48 8D BC 24 60 01  lea         rdi,[rsp+0000000000000160h]
                    00 00
  00000001401309BB: 48 8B F0           mov         rsi,rax
  00000001401309BE: B9 10 00 00 00     mov         ecx,10h
  00000001401309C3: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401309C5: 45 33 C0           xor         r8d,r8d
  00000001401309C8: 33 D2              xor         edx,edx
  00000001401309CA: 48 8D 8C 24 C8 00  lea         rcx,[rsp+00000000000000C8h]
                    00 00
  00000001401309D2: FF 15 90 37 CD 01  call        qword ptr [0000000141E04168h]
  00000001401309D8: 48 8D BC 24 50 01  lea         rdi,[rsp+0000000000000150h]
                    00 00
  00000001401309E0: 48 8B F0           mov         rsi,rax
  00000001401309E3: B9 10 00 00 00     mov         ecx,10h
  00000001401309E8: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  00000001401309EA: 45 33 C0           xor         r8d,r8d
  00000001401309ED: 33 D2              xor         edx,edx
  00000001401309EF: 48 8D 8C 24 D8 00  lea         rcx,[rsp+00000000000000D8h]
                    00 00
  00000001401309F7: FF 15 6B 37 CD 01  call        qword ptr [0000000141E04168h]
  00000001401309FD: 48 8D BC 24 40 01  lea         rdi,[rsp+0000000000000140h]
                    00 00
  0000000140130A05: 48 8B F0           mov         rsi,rax
  0000000140130A08: B9 10 00 00 00     mov         ecx,10h
  0000000140130A0D: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140130A0F: 45 33 C0           xor         r8d,r8d
  0000000140130A12: 33 D2              xor         edx,edx
  0000000140130A14: 48 8D 8C 24 E8 00  lea         rcx,[rsp+00000000000000E8h]
                    00 00
  0000000140130A1C: FF 15 46 37 CD 01  call        qword ptr [0000000141E04168h]
  0000000140130A22: 48 8D BC 24 30 01  lea         rdi,[rsp+0000000000000130h]
                    00 00
  0000000140130A2A: 48 8B F0           mov         rsi,rax
  0000000140130A2D: B9 10 00 00 00     mov         ecx,10h
  0000000140130A32: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140130A34: 48 8D BC 24 20 01  lea         rdi,[rsp+0000000000000120h]
                    00 00
  0000000140130A3C: 48 8D B4 24 F8 00  lea         rsi,[rsp+00000000000000F8h]
                    00 00
  0000000140130A44: B9 10 00 00 00     mov         ecx,10h
  0000000140130A49: F3 A4              rep movs    byte ptr [rdi],byte ptr [rsi]
  0000000140130A4B: 48 8D 84 24 B0 01  lea         rax,[rsp+00000000000001B0h]
                    00 00
  0000000140130A53: 48 89 44 24 58     mov         qword ptr [rsp+58h],rax
  0000000140130A58: 48 8D 84 24 A0 01  lea         rax,[rsp+00000000000001A0h]
                    00 00
  0000000140130A60: 48 89 44 24 50     mov         qword ptr [rsp+50h],rax
  0000000140130A65: 48 8D 84 24 90 01  lea         rax,[rsp+0000000000000190h]
                    00 00
  0000000140130A6D: 48 89 44 24 48     mov         qword ptr [rsp+48h],rax
  0000000140130A72: 48 8D 84 24 80 01  lea         rax,[rsp+0000000000000180h]
                    00 00
  0000000140130A7A: 48 89 44 24 40     mov         qword ptr [rsp+40h],rax
  0000000140130A7F: 48 8D 84 24 70 01  lea         rax,[rsp+0000000000000170h]
                    00 00
  0000000140130A87: 48 89 44 24 38     mov         qword ptr [rsp+38h],rax
  0000000140130A8C: 48 8D 84 24 60 01  lea         rax,[rsp+0000000000000160h]
                    00 00
  0000000140130A94: 48 89 44 24 30     mov         qword ptr [rsp+30h],rax
  0000000140130A99: 48 8D 84 24 50 01  lea         rax,[rsp+0000000000000150h]
                    00 00
  0000000140130AA1: 48 89 44 24 28     mov         qword ptr [rsp+28h],rax
  0000000140130AA6: 48 8D 84 24 40 01  lea         rax,[rsp+0000000000000140h]
                    00 00
  0000000140130AAE: 48 89 44 24 20     mov         qword ptr [rsp+20h],rax
  0000000140130AB3: 4C 8D 8C 24 30 01  lea         r9,[rsp+0000000000000130h]
                    00 00
  0000000140130ABB: 4C 8D 84 24 20 01  lea         r8,[rsp+0000000000000120h]
                    00 00
  0000000140130AC3: 48 8D 15 8E 7D 07  lea         rdx,[00000001421A8858h]
                    02
  0000000140130ACA: 48 8D 0D 2F 5C C4  lea         rcx,[0000000143D76700h]
                    03
  0000000140130AD1: E8 EA E8 19 00     call        00000001402CF3C0
  0000000140130AD6: 48 81 C4 D8 01 00  add         rsp,1D8h
                    00
  0000000140130ADD: 5F                 pop         rdi
  0000000140130ADE: 5E                 pop         rsi
  0000000140130ADF: C3                 ret

  Summary

     1E02000 .text

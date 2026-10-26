Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000650467C0: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  00000000650467C5: 48 89 74 24 10     mov         qword ptr [rsp+10h],rsi
  00000000650467CA: 57                 push        rdi
  00000000650467CB: 48 83 EC 20        sub         rsp,20h
  00000000650467CF: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000650467D3: 48 8B F2           mov         rsi,rdx
  00000000650467D6: 48 8B F9           mov         rdi,rcx
  00000000650467D9: F6 83 9C 00 00 00  test        byte ptr [rbx+000000000000009Ch],2
                    02
  00000000650467E0: 74 60              je          0000000065046842
  00000000650467E2: 48 8B CA           mov         rcx,rdx
  00000000650467E5: FF 15 7D 02 65 00  call        qword ptr [0000000065696A68h]
  00000000650467EB: 83 E8 0E           sub         eax,0Eh
  00000000650467EE: 0F 84 85 01 00 00  je          0000000065046979
  00000000650467F4: 83 E8 38           sub         eax,38h
  00000000650467F7: 0F 84 A3 00 00 00  je          00000000650468A0
  00000000650467FD: 83 E8 01           sub         eax,1
  0000000065046800: 74 50              je          0000000065046852
  0000000065046802: 83 E8 01           sub         eax,1
  0000000065046805: 74 15              je          000000006504681C
  0000000065046807: 83 F8 04           cmp         eax,4
  000000006504680A: 74 17              je          0000000065046823
  000000006504680C: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065046811: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000065046816: 48 83 C4 20        add         rsp,20h
  000000006504681A: 5F                 pop         rdi
  000000006504681B: C3                 ret
  000000006504681C: 83 A3 9C 00 00 00  and         dword ptr [rbx+000000000000009Ch],0FFFFFFFBh
                    FB
  0000000065046823: 48 8B CF           mov         rcx,rdi
  0000000065046826: FF 15 3C 00 65 00  call        qword ptr [0000000065696868h]
  000000006504682C: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  0000000065046830: 8B 41 08           mov         eax,dword ptr [rcx+8]
  0000000065046833: C1 E8 0F           shr         eax,0Fh
  0000000065046836: A8 01              test        al,1
  0000000065046838: 74 08              je          0000000065046842
  000000006504683A: 48 8B CF           mov         rcx,rdi
  000000006504683D: E8 9E F1 FF FF     call        00000000650459E0
  0000000065046842: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065046847: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000006504684C: 48 83 C4 20        add         rsp,20h
  0000000065046850: 5F                 pop         rdi
  0000000065046851: C3                 ret
  0000000065046852: 48 8B CE           mov         rcx,rsi
  0000000065046855: FF 15 3D 08 65 00  call        qword ptr [0000000065697098h]
  000000006504685B: 48 8B C8           mov         rcx,rax
  000000006504685E: FF 15 F4 FF 64 00  call        qword ptr [0000000065696858h]
  0000000065046864: 84 C0              test        al,al
  0000000065046866: 74 DA              je          0000000065046842
  0000000065046868: 48 8B CE           mov         rcx,rsi
  000000006504686B: FF 15 27 08 65 00  call        qword ptr [0000000065697098h]
  0000000065046871: 48 3B 83 B8 00 00  cmp         rax,qword ptr [rbx+00000000000000B8h]
                    00
  0000000065046878: 75 0B              jne         0000000065046885
  000000006504687A: 48 C7 83 B8 00 00  mov         qword ptr [rbx+00000000000000B8h],0
                    00 00 00 00 00
  0000000065046885: 48 8D 4F 10        lea         rcx,[rdi+10h]
  0000000065046889: 48 8B D0           mov         rdx,rax
  000000006504688C: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065046891: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000065046896: 48 83 C4 20        add         rsp,20h
  000000006504689A: 5F                 pop         rdi
  000000006504689B: E9 40 D1 FF FF     jmp         00000000650439E0
  00000000650468A0: 8B 83 9C 00 00 00  mov         eax,dword ptr [rbx+000000000000009Ch]
  00000000650468A6: A8 01              test        al,1
  00000000650468A8: 74 98              je          0000000065046842
  00000000650468AA: A8 08              test        al,8
  00000000650468AC: 74 94              je          0000000065046842
  00000000650468AE: 48 8B CE           mov         rcx,rsi
  00000000650468B1: FF 15 E1 07 65 00  call        qword ptr [0000000065697098h]
  00000000650468B7: 48 8B C8           mov         rcx,rax
  00000000650468BA: FF 15 98 FF 64 00  call        qword ptr [0000000065696858h]
  00000000650468C0: 84 C0              test        al,al
  00000000650468C2: 0F 84 7A FF FF FF  je          0000000065046842
  00000000650468C8: 48 8B CE           mov         rcx,rsi
  00000000650468CB: FF 15 C7 07 65 00  call        qword ptr [0000000065697098h]
  00000000650468D1: 48 8B F0           mov         rsi,rax
  00000000650468D4: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  00000000650468D8: F6 41 0C 01        test        byte ptr [rcx+0Ch],1
  00000000650468DC: 0F 85 60 FF FF FF  jne         0000000065046842
  00000000650468E2: 48 8D 0D 6F 16 6C  lea         rcx,[0000000065707F58h]
                    00
  00000000650468E9: 48 8B D0           mov         rdx,rax
  00000000650468EC: FF 15 16 01 65 00  call        qword ptr [0000000065696A08h]
  00000000650468F2: 48 85 C0           test        rax,rax
  00000000650468F5: 74 3F              je          0000000065046936
  00000000650468F7: 48 8B CE           mov         rcx,rsi
  00000000650468FA: FF 15 68 FF 64 00  call        qword ptr [0000000065696868h]
  0000000065046900: 48 8D 0D 29 96 6C  lea         rcx,[000000006570FF30h]
                    00
  0000000065046907: 48 8B D0           mov         rdx,rax
  000000006504690A: FF 15 F8 00 65 00  call        qword ptr [0000000065696A08h]
  0000000065046910: 48 85 C0           test        rax,rax
  0000000065046913: 75 21              jne         0000000065046936
  0000000065046915: 48 8D 4F 10        lea         rcx,[rdi+10h]
  0000000065046919: 48 89 B3 B8 00 00  mov         qword ptr [rbx+00000000000000B8h],rsi
                    00
  0000000065046920: 48 8B 01           mov         rax,qword ptr [rcx]
  0000000065046923: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065046928: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  000000006504692D: 48 83 C4 20        add         rsp,20h
  0000000065046931: 5F                 pop         rdi
  0000000065046932: 48 FF 60 58        jmp         qword ptr [rax+58h]
  0000000065046936: 48 8D 0D EB 33 6C  lea         rcx,[0000000065709D28h]
                    00
  000000006504693D: 48 8B D6           mov         rdx,rsi
  0000000065046940: FF 15 C2 00 65 00  call        qword ptr [0000000065696A08h]
  0000000065046946: 48 85 C0           test        rax,rax
  0000000065046949: 0F 85 F3 FE FF FF  jne         0000000065046842
  000000006504694F: 48 8B 1F           mov         rbx,qword ptr [rdi]
  0000000065046952: 48 8B D6           mov         rdx,rsi
  0000000065046955: 48 8B CF           mov         rcx,rdi
  0000000065046958: E8 D3 CF FF FF     call        0000000065043930
  000000006504695D: 48 8B CF           mov         rcx,rdi
  0000000065046960: 48 8B D0           mov         rdx,rax
  0000000065046963: 48 8B 43 58        mov         rax,qword ptr [rbx+58h]
  0000000065046967: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  000000006504696C: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000065046971: 48 83 C4 20        add         rsp,20h
  0000000065046975: 5F                 pop         rdi
  0000000065046976: 48 FF E0           jmp         rax
  0000000065046979: F6 83 9C 00 00 00  test        byte ptr [rbx+000000000000009Ch],4
                    04
  0000000065046980: 0F 84 B4 FE FF FF  je          000000006504683A
  0000000065046986: 48 8D 56 18        lea         rdx,[rsi+18h]
  000000006504698A: 48 8B CB           mov         rcx,rbx
  000000006504698D: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  0000000065046992: 48 8B 74 24 38     mov         rsi,qword ptr [rsp+38h]
  0000000065046997: 48 83 C4 20        add         rsp,20h
  000000006504699B: 5F                 pop         rdi
  000000006504699C: E9 DF E1 FF FF     jmp         0000000065044B80

  Summary

      695000 .text

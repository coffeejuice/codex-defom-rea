Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065097940: 48 89 5C 24 08     mov         qword ptr [rsp+8],rbx
  0000000065097945: 57                 push        rdi
  0000000065097946: 48 83 EC 20        sub         rsp,20h
  000000006509794A: 48 8B 79 08        mov         rdi,qword ptr [rcx+8]
  000000006509794E: 48 8B D9           mov         rbx,rcx
  0000000065097951: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  0000000065097955: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  0000000065097959: 74 74              je          00000000650979CF
  000000006509795B: BA 78 00 00 00     mov         edx,78h
  0000000065097960: 48 8B CF           mov         rcx,rdi
  0000000065097963: E8 A8 66 FC FF     call        000000006505E010
  0000000065097968: 84 C0              test        al,al
  000000006509796A: 74 63              je          00000000650979CF
  000000006509796C: 0F BA A3 54 01 00  bt          dword ptr [rbx+0000000000000154h],0Bh
                    00 0B
  0000000065097974: 73 59              jae         00000000650979CF
  0000000065097976: 0F BA A3 D8 01 00  bt          dword ptr [rbx+00000000000001D8h],11h
                    00 11
  000000006509797E: 72 25              jb          00000000650979A5
  0000000065097980: 48 83 3D E0 34 93  cmp         qword ptr [00000000659CAE68h],0
                    00 00
  0000000065097988: 74 1B              je          00000000650979A5
  000000006509798A: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  000000006509798E: BA EC FF FF FF     mov         edx,0FFFFFFECh
  0000000065097993: 48 8B 18           mov         rbx,qword ptr [rax]
  0000000065097996: 48 8B CB           mov         rcx,rbx
  0000000065097999: FF 15 21 21 60 00  call        qword ptr [0000000065699AC0h]
  000000006509799F: 0F BA E8 13        bts         eax,13h
  00000000650979A3: EB 19              jmp         00000000650979BE
  00000000650979A5: 48 8B 47 20        mov         rax,qword ptr [rdi+20h]
  00000000650979A9: BA EC FF FF FF     mov         edx,0FFFFFFECh
  00000000650979AE: 48 8B 18           mov         rbx,qword ptr [rax]
  00000000650979B1: 48 8B CB           mov         rcx,rbx
  00000000650979B4: FF 15 06 21 60 00  call        qword ptr [0000000065699AC0h]
  00000000650979BA: 0F BA F0 13        btr         eax,13h
  00000000650979BE: 44 8B C0           mov         r8d,eax
  00000000650979C1: BA EC FF FF FF     mov         edx,0FFFFFFECh
  00000000650979C6: 48 8B CB           mov         rcx,rbx
  00000000650979C9: FF 15 F9 20 60 00  call        qword ptr [0000000065699AC8h]
  00000000650979CF: 48 8B 5C 24 30     mov         rbx,qword ptr [rsp+30h]
  00000000650979D4: 48 83 C4 20        add         rsp,20h
  00000000650979D8: 5F                 pop         rdi
  00000000650979D9: C3                 ret

  Summary

      695000 .text

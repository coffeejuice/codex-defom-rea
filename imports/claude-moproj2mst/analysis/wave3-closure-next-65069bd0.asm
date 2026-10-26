Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065069BD0: 48 89 5C 24 20     mov         qword ptr [rsp+20h],rbx
  0000000065069BD5: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065069BDA: 55                 push        rbp
  0000000065069BDB: 48 83 EC 20        sub         rsp,20h
  0000000065069BDF: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  0000000065069BE3: 0F B6 EA           movzx       ebp,dl
  0000000065069BE6: 84 D2              test        dl,dl
  0000000065069BE8: 74 33              je          0000000065069C1D
  0000000065069BEA: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  0000000065069BEE: F6 40 0C 01        test        byte ptr [rax+0Ch],1
  0000000065069BF2: 75 29              jne         0000000065069C1D
  0000000065069BF4: 48 8B CB           mov         rcx,rbx
  0000000065069BF7: FF 15 6B CC 62 00  call        qword ptr [0000000065696868h]
  0000000065069BFD: 48 85 C0           test        rax,rax
  0000000065069C00: 74 1B              je          0000000065069C1D
  0000000065069C02: 48 8B CB           mov         rcx,rbx
  0000000065069C05: FF 15 5D CC 62 00  call        qword ptr [0000000065696868h]
  0000000065069C0B: 48 8B 48 20        mov         rcx,qword ptr [rax+20h]
  0000000065069C0F: 8B 41 08           mov         eax,dword ptr [rcx+8]
  0000000065069C12: C1 E8 0A           shr         eax,0Ah
  0000000065069C15: A8 01              test        al,1
  0000000065069C17: 0F 85 16 01 00 00  jne         0000000065069D33
  0000000065069C1D: 48 8B 43 20        mov         rax,qword ptr [rbx+20h]
  0000000065069C21: 8B 50 08           mov         edx,dword ptr [rax+8]
  0000000065069C24: C1 EA 0A           shr         edx,0Ah
  0000000065069C27: 80 E2 01           and         dl,1
  0000000065069C2A: 40 3A EA           cmp         bpl,dl
  0000000065069C2D: 0F 85 00 01 00 00  jne         0000000065069D33

  Summary

      695000 .text

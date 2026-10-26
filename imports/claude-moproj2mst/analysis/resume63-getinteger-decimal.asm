Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  00000000670C5609: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000670C5611: 48 8B CE           mov         rcx,rsi
  00000000670C5614: 66 44 89 A4 24 A0  mov         word ptr [rsp+00000000000000A0h],r12w
                    00 00 00
  00000000670C561D: 45 8B EC           mov         r13d,r12d
  00000000670C5620: E8 5B DE FF FF     call        00000000670C3480
  00000000670C5625: 84 C0              test        al,al
  00000000670C5627: 0F 84 73 03 00 00  je          00000000670C59A0
  00000000670C562D: 48 8D 54 24 26     lea         rdx,[rsp+26h]
  00000000670C5632: 48 8D 8E D0 00 00  lea         rcx,[rsi+00000000000000D0h]
                    00
  00000000670C5639: E8 02 52 F8 FF     call        000000006704A840
  00000000670C563E: 0F B7 00           movzx       eax,word ptr [rax]
  00000000670C5641: 66 39 84 24 A0 00  cmp         word ptr [rsp+00000000000000A0h],ax
                    00 00
  00000000670C5649: 74 5F              je          00000000670C56AA
  00000000670C564B: 48 8D 54 24 2E     lea         rdx,[rsp+2Eh]
  00000000670C5650: 48 8D 8E D0 00 00  lea         rcx,[rsi+00000000000000D0h]
                    00
  00000000670C5657: E8 24 52 F8 FF     call        000000006704A880
  00000000670C565C: 0F B7 00           movzx       eax,word ptr [rax]
  00000000670C565F: 66 39 84 24 A0 00  cmp         word ptr [rsp+00000000000000A0h],ax
                    00 00
  00000000670C5667: 74 41              je          00000000670C56AA
  00000000670C5669: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000000670C5671: E8 FA 47 FA FF     call        0000000067069E70
  00000000670C5676: 84 C0              test        al,al
  00000000670C5678: 75 1A              jne         00000000670C5694
  00000000670C567A: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000670C5682: 48 8B CE           mov         rcx,rsi
  00000000670C5685: E8 66 CA FF FF     call        00000000670C20F0
  00000000670C568A: B8 01 00 00 00     mov         eax,1
  00000000670C568F: E9 2B FE FF FF     jmp         00000000670C54BF
  00000000670C5694: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000000670C569C: E8 5F 48 FA FF     call        0000000067069F00
  00000000670C56A1: 41 BD 01 00 00 00  mov         r13d,1
  00000000670C56A7: 48 63 F8           movsxd      rdi,eax
  00000000670C56AA: 66 44 89 A4 24 B0  mov         word ptr [rsp+00000000000000B0h],r12w
                    00 00 00
  00000000670C56B3: 66 66 66 66 66 0F  nop         word ptr [rax+rax+0000000000000000h]
                    1F 84 00 00 00 00
                    00
  00000000670C56C0: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000000670C56C8: 48 8B CE           mov         rcx,rsi
  00000000670C56CB: E8 B0 DD FF FF     call        00000000670C3480
  00000000670C56D0: 84 C0              test        al,al
  00000000670C56D2: 0F 84 B8 00 00 00  je          00000000670C5790
  00000000670C56D8: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000000670C56E0: E8 8B 47 FA FF     call        0000000067069E70
  00000000670C56E5: 84 C0              test        al,al
  00000000670C56E7: 74 1D              je          00000000670C5706
  00000000670C56E9: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000000670C56F1: E8 0A 48 FA FF     call        0000000067069F00
  00000000670C56F6: 41 FF C5           inc         r13d
  00000000670C56F9: 48 63 C8           movsxd      rcx,eax
  00000000670C56FC: 48 8D 04 BF        lea         rax,[rdi+rdi*4]
  00000000670C5700: 48 8D 3C 41        lea         rdi,[rcx+rax*2]
  00000000670C5704: EB BA              jmp         00000000670C56C0
  00000000670C5706: 45 33 C0           xor         r8d,r8d
  00000000670C5709: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000670C570E: 41 8D 50 01        lea         edx,[r8+1]
  00000000670C5712: E8 89 7E F8 FF     call        000000006704D5A0
  00000000670C5717: 48 8D 8E D0 00 00  lea         rcx,[rsi+00000000000000D0h]
                    00
  00000000670C571E: E8 1D 3C F8 FF     call        0000000067049340
  00000000670C5723: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000670C5728: 48 8B D8           mov         rbx,rax
  00000000670C572B: E8 10 3C F8 FF     call        0000000067049340
  00000000670C5730: 48 3B D8           cmp         rbx,rax
  00000000670C5733: 75 29              jne         00000000670C575E
  00000000670C5735: 48 8D 94 24 B8 00  lea         rdx,[rsp+00000000000000B8h]
                    00 00
  00000000670C573D: 48 8D 8E D0 00 00  lea         rcx,[rsi+00000000000000D0h]
                    00
  00000000670C5744: E8 F7 4B F8 FF     call        000000006704A340
  00000000670C5749: 48 8D 54 24 40     lea         rdx,[rsp+40h]
  00000000670C574E: 48 8D 4C 24 38     lea         rcx,[rsp+38h]
  00000000670C5753: 8B 18              mov         ebx,dword ptr [rax]
  00000000670C5755: E8 E6 4B F8 FF     call        000000006704A340
  00000000670C575A: 3B 18              cmp         ebx,dword ptr [rax]
  00000000670C575C: 74 22              je          00000000670C5780
  00000000670C575E: 48 8D 54 24 2A     lea         rdx,[rsp+2Ah]
  00000000670C5763: 48 8D 8E D0 00 00  lea         rcx,[rsi+00000000000000D0h]
                    00
  00000000670C576A: E8 11 50 F8 FF     call        000000006704A780
  00000000670C576F: 0F B7 00           movzx       eax,word ptr [rax]
  00000000670C5772: 66 39 84 24 B0 00  cmp         word ptr [rsp+00000000000000B0h],ax
                    00 00
  00000000670C577A: 0F 84 40 FF FF FF  je          00000000670C56C0
  00000000670C5780: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000000670C5788: 48 8B CE           mov         rcx,rsi
  00000000670C578B: E8 60 C9 FF FF     call        00000000670C20F0
  00000000670C5790: 45 85 ED           test        r13d,r13d
  00000000670C5793: 0F 84 07 02 00 00  je          00000000670C59A0
  00000000670C5799: 48 8D 54 24 20     lea         rdx,[rsp+20h]
  00000000670C579E: 48 8D 8E D0 00 00  lea         rcx,[rsi+00000000000000D0h]
                    00
  00000000670C57A5: E8 96 50 F8 FF     call        000000006704A840
  00000000670C57AA: 0F B7 00           movzx       eax,word ptr [rax]
  00000000670C57AD: 66 39 84 24 A0 00  cmp         word ptr [rsp+00000000000000A0h],ax
                    00 00
  00000000670C57B5: 0F 85 AE 01 00 00  jne         00000000670C5969
  00000000670C57BB: 48 85 FF           test        rdi,rdi
  00000000670C57BE: 0F 8E A5 01 00 00  jle         00000000670C5969
  00000000670C57C4: 48 F7 DF           neg         rdi
  00000000670C57C7: E9 9D 01 00 00     jmp         00000000670C5969
  00000000670C57CC: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000000670C57D4: 48 8B CE           mov         rcx,rsi
  00000000670C57D7: 66 44 89 A4 24 B0  mov         word ptr [rsp+00000000000000B0h],r12w
                    00 00 00
  00000000670C57E0: 66 44 89 A4 24 A0  mov         word ptr [rsp+00000000000000A0h],r12w
                    00 00 00
  00000000670C57E9: E8 92 DC FF FF     call        00000000670C3480
  00000000670C57EE: 84 C0              test        al,al
  00000000670C57F0: 0F 84 C4 FC FF FF  je          00000000670C54BA
  00000000670C57F6: 66 83 BC 24 B0 00  cmp         word ptr [rsp+00000000000000B0h],30h
                    00 00 30
  00000000670C57FF: 0F 85 B5 FC FF FF  jne         00000000670C54BA
  00000000670C5805: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000670C580D: 48 8B CE           mov         rcx,rsi
  00000000670C5810: E8 6B DC FF FF     call        00000000670C3480
  00000000670C5815: 84 C0              test        al,al
  00000000670C5817: 74 55              je          00000000670C586E
  00000000670C5819: 0F 1F 80 00 00 00  nop         dword ptr [rax+0000000000000000h]
                    00
  00000000670C5820: 48 8D 54 24 24     lea         rdx,[rsp+24h]
  00000000670C5825: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000000670C582D: E8 1E 4E FA FF     call        000000006706A650
  00000000670C5832: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000670C583A: 0F B7 08           movzx       ecx,word ptr [rax]
  00000000670C583D: 83 C1 D0           add         ecx,0FFFFFFD0h
  00000000670C5840: 83 F9 07           cmp         ecx,7
  00000000670C5843: 77 18              ja          00000000670C585D
  00000000670C5845: 48 63 C1           movsxd      rax,ecx
  00000000670C5848: 48 8B CE           mov         rcx,rsi
  00000000670C584B: 41 FF C4           inc         r12d
  00000000670C584E: 48 8D 3C F8        lea         rdi,[rax+rdi*8]
  00000000670C5852: E8 29 DC FF FF     call        00000000670C3480
  00000000670C5857: 84 C0              test        al,al
  00000000670C5859: 75 C5              jne         00000000670C5820
  00000000670C585B: EB 08              jmp         00000000670C5865
  00000000670C585D: 48 8B CE           mov         rcx,rsi
  00000000670C5860: E8 8B C8 FF FF     call        00000000670C20F0
  00000000670C5865: 45 85 E4           test        r12d,r12d
  00000000670C5868: 0F 85 FB 00 00 00  jne         00000000670C5969
  00000000670C586E: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000000670C5876: 48 8B CE           mov         rcx,rsi
  00000000670C5879: E8 72 C8 FF FF     call        00000000670C20F0
  00000000670C587E: B8 01 00 00 00     mov         eax,1
  00000000670C5883: E9 37 FC FF FF     jmp         00000000670C54BF
  00000000670C5888: 48 8D 94 24 B8 00  lea         rdx,[rsp+00000000000000B8h]
                    00 00
  00000000670C5890: 48 8B CE           mov         rcx,rsi
  00000000670C5893: 66 44 89 A4 24 B8  mov         word ptr [rsp+00000000000000B8h],r12w
                    00 00 00
  00000000670C589C: 66 44 89 A4 24 B0  mov         word ptr [rsp+00000000000000B0h],r12w
                    00 00 00
  00000000670C58A5: 66 44 89 A4 24 A0  mov         word ptr [rsp+00000000000000A0h],r12w
                    00 00 00
  00000000670C58AE: E8 CD DB FF FF     call        00000000670C3480
  00000000670C58B3: 84 C0              test        al,al
  00000000670C58B5: 0F 84 FF FB FF FF  je          00000000670C54BA
  00000000670C58BB: 66 83 BC 24 B8 00  cmp         word ptr [rsp+00000000000000B8h],30h
                    00 00 30
  00000000670C58C4: 0F 85 F0 FB FF FF  jne         00000000670C54BA
  00000000670C58CA: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000000670C58D2: 48 8B CE           mov         rcx,rsi
  00000000670C58D5: E8 A6 DB FF FF     call        00000000670C3480
  00000000670C58DA: 84 C0              test        al,al
  00000000670C58DC: 0F 84 D8 FB FF FF  je          00000000670C54BA
  00000000670C58E2: 48 8D 54 24 28     lea         rdx,[rsp+28h]
  00000000670C58E7: 48 8D 8C 24 B0 00  lea         rcx,[rsp+00000000000000B0h]
                    00 00
  00000000670C58EF: E8 5C 4D FA FF     call        000000006706A650
  00000000670C58F4: 66 83 38 62        cmp         word ptr [rax],62h
  00000000670C58F8: 0F 85 BC FB FF FF  jne         00000000670C54BA
  00000000670C58FE: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000670C5906: 48 8B CE           mov         rcx,rsi
  00000000670C5909: E8 72 DB FF FF     call        00000000670C3480
  00000000670C590E: 84 C0              test        al,al
  00000000670C5910: 74 6E              je          00000000670C5980
  00000000670C5912: 48 8D 54 24 2C     lea         rdx,[rsp+2Ch]
  00000000670C5917: 48 8D 8C 24 A0 00  lea         rcx,[rsp+00000000000000A0h]
                    00 00
  00000000670C591F: E8 2C 4D FA FF     call        000000006706A650
  00000000670C5924: 0F B7 00           movzx       eax,word ptr [rax]
  00000000670C5927: 83 F8 30           cmp         eax,30h
  00000000670C592A: 74 05              je          00000000670C5931
  00000000670C592C: 83 F8 31           cmp         eax,31h
  00000000670C592F: 75 23              jne         00000000670C5954
  00000000670C5931: 83 C0 D0           add         eax,0FFFFFFD0h
  00000000670C5934: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000670C593C: 41 FF C4           inc         r12d
  00000000670C593F: 48 63 C8           movsxd      rcx,eax
  00000000670C5942: 48 8D 3C 79        lea         rdi,[rcx+rdi*2]
  00000000670C5946: 48 8B CE           mov         rcx,rsi
  00000000670C5949: E8 32 DB FF FF     call        00000000670C3480
  00000000670C594E: 84 C0              test        al,al
  00000000670C5950: 75 C0              jne         00000000670C5912
  00000000670C5952: EB 10              jmp         00000000670C5964
  00000000670C5954: 48 8D 94 24 A0 00  lea         rdx,[rsp+00000000000000A0h]
                    00 00
  00000000670C595C: 48 8B CE           mov         rcx,rsi
  00000000670C595F: E8 8C C7 FF FF     call        00000000670C20F0
  00000000670C5964: 45 85 E4           test        r12d,r12d
  00000000670C5967: 74 17              je          00000000670C5980
  00000000670C5969: 48 8B 84 24 A8 00  mov         rax,qword ptr [rsp+00000000000000A8h]
                    00 00
  00000000670C5971: 48 85 C0           test        rax,rax
  00000000670C5974: 74 03              je          00000000670C5979
  00000000670C5976: 48 89 38           mov         qword ptr [rax],rdi
  00000000670C5979: 33 C0              xor         eax,eax
  00000000670C597B: E9 3F FB FF FF     jmp         00000000670C54BF
  00000000670C5980: 48 8D 94 24 B0 00  lea         rdx,[rsp+00000000000000B0h]
                    00 00
  00000000670C5988: 48 8B CE           mov         rcx,rsi
  00000000670C598B: E8 60 C7 FF FF     call        00000000670C20F0
  00000000670C5990: 48 8D 94 24 B8 00  lea         rdx,[rsp+00000000000000B8h]
                    00 00
  00000000670C5998: 48 8B CE           mov         rcx,rsi
  00000000670C599B: E8 50 C7 FF FF     call        00000000670C20F0
  00000000670C59A0: B8 01 00 00 00     mov         eax,1
  00000000670C59A5: E9 15 FB FF FF     jmp         00000000670C54BF
  00000000670C59AA: CC                 int         3

  Summary

      1A7000 .text

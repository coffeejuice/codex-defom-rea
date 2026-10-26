Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  0000000065402A70: 48 89 4C 24 08     mov         qword ptr [rsp+8],rcx
  0000000065402A75: 53                 push        rbx
  0000000065402A76: 48 83 EC 30        sub         rsp,30h
  0000000065402A7A: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  0000000065402A83: 48 8B D9           mov         rbx,rcx
  0000000065402A86: E8 55 F6 FA FF     call        00000000653B20E0
  0000000065402A8B: 90                 nop
  0000000065402A8C: 48 8D 05 75 87 30  lea         rax,[000000006570B208h]
                    00
  0000000065402A93: 48 89 03           mov         qword ptr [rbx],rax
  0000000065402A96: 48 8D 8B 08 02 00  lea         rcx,[rbx+0000000000000208h]
                    00
  0000000065402A9D: 48 C7 01 00 00 00  mov         qword ptr [rcx],0
                    00
  0000000065402AA4: FF 15 2E 3E 29 00  call        qword ptr [00000000656968D8h]
  0000000065402AAA: 90                 nop
  0000000065402AAB: 48 8B 05 16 3E 29  mov         rax,qword ptr [00000000656968C8h]
                    00
  0000000065402AB2: 48 89 83 10 02 00  mov         qword ptr [rbx+0000000000000210h],rax
                    00
  0000000065402AB9: 48 8B 0D 08 3E 29  mov         rcx,qword ptr [00000000656968C8h]
                    00
  0000000065402AC0: FF 15 FA 3D 29 00  call        qword ptr [00000000656968C0h]
  0000000065402AC6: C6 83 1C 02 00 00  mov         byte ptr [rbx+000000000000021Ch],1
                    01
  0000000065402ACD: C6 83 1D 02 00 00  mov         byte ptr [rbx+000000000000021Dh],1
                    01
  0000000065402AD4: C6 83 1E 02 00 00  mov         byte ptr [rbx+000000000000021Eh],1
                    01
  0000000065402ADB: C6 83 1F 02 00 00  mov         byte ptr [rbx+000000000000021Fh],0
                    00
  0000000065402AE2: C7 83 20 02 00 00  mov         dword ptr [rbx+0000000000000220h],0
                    00 00 00 00
  0000000065402AEC: C6 83 24 02 00 00  mov         byte ptr [rbx+0000000000000224h],0
                    00
  0000000065402AF3: 48 8B C3           mov         rax,rbx
  0000000065402AF6: 48 83 C4 30        add         rsp,30h
  0000000065402AFA: 5B                 pop         rbx
  0000000065402AFB: C3                 ret

  Summary

      695000 .text

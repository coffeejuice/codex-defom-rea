Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  000000006505D610: 40 53              push        rbx
  000000006505D612: 48 83 EC 20        sub         rsp,20h
  000000006505D616: 48 8B 41 08        mov         rax,qword ptr [rcx+8]
  000000006505D61A: 48 8B DA           mov         rbx,rdx
  000000006505D61D: C7 44 24 30 00 00  mov         dword ptr [rsp+30h],0
                    00 00
  000000006505D625: 48 8B 90 88 00 00  mov         rdx,qword ptr [rax+0000000000000088h]
                    00
  000000006505D62C: 48 8B CB           mov         rcx,rbx
  000000006505D62F: 48 85 D2           test        rdx,rdx
  000000006505D632: 75 0F              jne         000000006505D643
  000000006505D634: FF 15 2E 91 63 00  call        qword ptr [0000000065696768h]
  000000006505D63A: 48 8B C3           mov         rax,rbx
  000000006505D63D: 48 83 C4 20        add         rsp,20h
  000000006505D641: 5B                 pop         rbx
  000000006505D642: C3                 ret
  000000006505D643: 48 83 C2 38        add         rdx,38h
  000000006505D647: FF 15 A3 93 63 00  call        qword ptr [00000000656969F0h]
  000000006505D64D: 48 8B C3           mov         rax,rbx
  000000006505D650: 48 83 C4 20        add         rsp,20h
  000000006505D654: 5B                 pop         rbx
  000000006505D655: C3                 ret

  Summary

      695000 .text

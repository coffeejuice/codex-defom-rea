Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\original-binaries\QtGui4_64.dll

File Type: DLL

  00000000653FEAF0: 85 D2              test        edx,edx
  00000000653FEAF2: 78 49              js          00000000653FEB3D
  00000000653FEAF4: 53                 push        rbx
  00000000653FEAF5: 48 83 EC 40        sub         rsp,40h
  00000000653FEAF9: 48 C7 44 24 20 FE  mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
                    FF FF FF
  00000000653FEB02: 48 8B 59 08        mov         rbx,qword ptr [rcx+8]
  00000000653FEB06: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000653FEB0B: FF 15 47 87 29 00  call        qword ptr [0000000065697258h]
  00000000653FEB11: 90                 nop
  00000000653FEB12: 48 8D 8B 30 02 00  lea         rcx,[rbx+0000000000000230h]
                    00
  00000000653FEB19: 48 8B D0           mov         rdx,rax
  00000000653FEB1C: FF 15 6E 7C 29 00  call        qword ptr [0000000065696790h]
  00000000653FEB22: 90                 nop
  00000000653FEB23: 48 8D 4C 24 28     lea         rcx,[rsp+28h]
  00000000653FEB28: FF 15 B2 7B 29 00  call        qword ptr [00000000656966E0h]
  00000000653FEB2E: 4C 8B 1B           mov         r11,qword ptr [rbx]
  00000000653FEB31: 48 8B CB           mov         rcx,rbx
  00000000653FEB34: 41 FF 53 30        call        qword ptr [r11+30h]
  00000000653FEB38: 48 83 C4 40        add         rsp,40h
  00000000653FEB3C: 5B                 pop         rbx
  00000000653FEB3D: F3 C3              rep ret

  Summary

      695000 .text

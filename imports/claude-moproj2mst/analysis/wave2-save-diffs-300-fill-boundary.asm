Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140EB5630: mov         dword ptr [rsp+18h],r8d
  0000000140EB5635: mov         dword ptr [rsp+10h],edx
  0000000140EB5639: mov         qword ptr [rsp+8],rcx
  0000000140EB563E: sub         rsp,58h
  0000000140EB5642: mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
  0000000140EB564B: mov         rcx,qword ptr [rsp+60h]
  0000000140EB5650: call        0000000140EB5500
  0000000140EB5655: mov         rcx,qword ptr [rsp+60h]
  0000000140EB565A: call        0000000140130EA0
  0000000140EB565F: movzx       eax,al
  0000000140EB5662: test        eax,eax
  0000000140EB5664: je          0000000140EB56A7
  0000000140EB5666: mov         rcx,qword ptr [rsp+60h]
  0000000140EB566B: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EB5672: mov         edx,8
  0000000140EB5677: mov         rcx,qword ptr [rcx+8]
  0000000140EB567B: call        qword ptr [0000000141E064D0h]
  0000000140EB5681: mov         dl,1
  0000000140EB5683: mov         rcx,rax
  0000000140EB5686: call        qword ptr [0000000141E08E20h]
  0000000140EB568C: mov         rcx,qword ptr [rsp+60h]
  0000000140EB5691: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EB5698: mov         rcx,qword ptr [rcx+0000000000000B70h]
  0000000140EB569F: call        qword ptr [0000000141E0BBB0h]
  0000000140EB56A5: jmp         0000000140EB56E6
  0000000140EB56A7: mov         rcx,qword ptr [rsp+60h]
  0000000140EB56AC: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EB56B3: mov         edx,8
  0000000140EB56B8: mov         rcx,qword ptr [rcx+8]
  0000000140EB56BC: call        qword ptr [0000000141E064D0h]
  0000000140EB56C2: xor         edx,edx
  0000000140EB56C4: mov         rcx,rax
  0000000140EB56C7: call        qword ptr [0000000141E08E20h]
  0000000140EB56CD: mov         rcx,qword ptr [rsp+60h]
  0000000140EB56D2: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140EB56D9: mov         rcx,qword ptr [rcx+0000000000000B70h]
  0000000140EB56E0: call        qword ptr [0000000141E0BBB0h]
  0000000140EB56E6: mov         rcx,qword ptr [rsp+60h]
  0000000140EB56EB: mov         eax,dword ptr [rsp+68h]
  0000000140EB56EF: mov         dword ptr [rcx+00000000000000B8h],eax
  0000000140EB56F5: mov         rcx,qword ptr [rsp+60h]
  0000000140EB56FA: mov         eax,dword ptr [rsp+70h]
  0000000140EB56FE: mov         dword ptr [rcx+00000000000000BCh],eax
  0000000140EB5704: mov         rcx,qword ptr [rsp+60h]
  0000000140EB5709: call        0000000140ECF340
  0000000140EB570E: movzx       eax,al
  0000000140EB5711: cmp         eax,1
  0000000140EB5714: jne         0000000140EB5729
  0000000140EB5716: mov         rax,qword ptr [rsp+60h]
  0000000140EB571B: cmp         dword ptr [rax+00000000000000B8h],8
  0000000140EB5722: je          0000000140EB5729
  0000000140EB5724: mov         rax,qword ptr [rsp+60h]
  0000000140EB5729: mov         rcx,qword ptr [rsp+60h]
  0000000140EB572E: call        0000000140130F90
  0000000140EB5733: mov         qword ptr [rsp+28h],rax
  0000000140EB5738: cmp         qword ptr [rsp+28h],0
  0000000140EB573E: jne         0000000140EB574A
  0000000140EB5740: jmp         0000000140EB58A3
  0000000140EB5745: jmp         0000000140EB58A3
  0000000140EB574A: mov         rdx,qword ptr [rsp+28h]
  0000000140EB574F: mov         rcx,qword ptr [rsp+60h]
  0000000140EB5754: call        0000000140EB80C0
  0000000140EB5759: mov         dword ptr [rsp+20h],0
  0000000140EB5761: jmp         0000000140EB576E
  0000000140EB5763: mov         eax,dword ptr [rsp+20h]
  0000000140EB5767: add         eax,1
  0000000140EB576A: mov         dword ptr [rsp+20h],eax
  0000000140EB576E: cmp         dword ptr [rsp+20h],6
  0000000140EB5773: jge         0000000140EB57DE
  0000000140EB5775: movsxd      rcx,dword ptr [rsp+20h]
  0000000140EB577A: mov         rax,qword ptr [rsp+60h]
  0000000140EB577F: cmp         qword ptr [rax+rcx*8+00000000000000C8h],0
  0000000140EB5788: jne         0000000140EB57DC
  0000000140EB578A: mov         ecx,78h
  0000000140EB578F: call        000000014146EABC
  0000000140EB5794: mov         qword ptr [rsp+38h],rax
  0000000140EB5799: cmp         qword ptr [rsp+38h],0
  0000000140EB579F: je          0000000140EB57B2
  0000000140EB57A1: mov         rcx,qword ptr [rsp+38h]
  0000000140EB57A6: call        0000000140898DC0
  0000000140EB57AB: mov         qword ptr [rsp+48h],rax
  0000000140EB57B0: jmp         0000000140EB57BB
  0000000140EB57B2: mov         qword ptr [rsp+48h],0
  0000000140EB57BB: mov         rax,qword ptr [rsp+48h]
  0000000140EB57C0: mov         qword ptr [rsp+30h],rax
  0000000140EB57C5: movsxd      rdx,dword ptr [rsp+20h]
  0000000140EB57CA: mov         rcx,qword ptr [rsp+60h]
  0000000140EB57CF: mov         rax,qword ptr [rsp+30h]
  0000000140EB57D4: mov         qword ptr [rcx+rdx*8+00000000000000C8h],rax
  0000000140EB57DC: jmp         0000000140EB5763
  0000000140EB57DE: mov         rdx,qword ptr [rsp+28h]
  0000000140EB57E3: add         rdx,3C8h
  0000000140EB57EA: mov         rcx,qword ptr [rsp+60h]
  0000000140EB57EF: mov         rcx,qword ptr [rcx+00000000000000C8h]
  0000000140EB57F6: call        000000014089AD60
  0000000140EB57FB: mov         rdx,qword ptr [rsp+28h]
  0000000140EB5800: add         rdx,4C0h
  0000000140EB5807: mov         rcx,qword ptr [rsp+60h]
  0000000140EB580C: mov         rcx,qword ptr [rcx+00000000000000D0h]
  0000000140EB5813: call        000000014089AD60
  0000000140EB5818: mov         rdx,qword ptr [rsp+28h]
  0000000140EB581D: add         rdx,538h
  0000000140EB5824: mov         rcx,qword ptr [rsp+60h]
  0000000140EB5829: mov         rcx,qword ptr [rcx+00000000000000D8h]
  0000000140EB5830: call        000000014089AD60
  0000000140EB5835: mov         rdx,qword ptr [rsp+28h]
  0000000140EB583A: add         rdx,5B0h
  0000000140EB5841: mov         rcx,qword ptr [rsp+60h]
  0000000140EB5846: mov         rcx,qword ptr [rcx+00000000000000E0h]
  0000000140EB584D: call        000000014089AD60
  0000000140EB5852: mov         rdx,qword ptr [rsp+28h]
  0000000140EB5857: add         rdx,1E8h
  0000000140EB585E: mov         rcx,qword ptr [rsp+60h]
  0000000140EB5863: mov         rcx,qword ptr [rcx+00000000000000E8h]
  0000000140EB586A: call        000000014089AD60
  0000000140EB586F: mov         rdx,qword ptr [rsp+28h]
  0000000140EB5874: add         rdx,9E0h
  0000000140EB587B: mov         rcx,qword ptr [rsp+60h]
  0000000140EB5880: mov         rcx,qword ptr [rcx+00000000000000F0h]
  0000000140EB5887: call        000000014089AD60
  0000000140EB588C: mov         r11,qword ptr [rsp+60h]
  0000000140EB5891: mov         rax,qword ptr [rsp+28h]
  0000000140EB5896: mov         eax,dword ptr [rax+00000000000009DCh]
  0000000140EB589C: mov         dword ptr [r11+0000000000000138h],eax
  0000000140EB58A3: add         rsp,58h
  0000000140EB58A7: ret
  0000000140EB58A8: int         3

  Summary

     1E02000 .text

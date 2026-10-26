Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file C:\Users\alext\Projects\codex-deform-rea\imports\claude-moproj2mst\evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  000000014082A360: mov         byte ptr [rsp+10h],dl
  000000014082A364: mov         qword ptr [rsp+8],rcx
  000000014082A369: sub         rsp,218h
  000000014082A370: mov         qword ptr [rsp+0000000000000208h],0FFFFFFFFFFFFFFFEh
  000000014082A37C: mov         dword ptr [rsp+0000000000000190h],0
  000000014082A387: mov         dword ptr [rsp+00000000000001CCh],0
  000000014082A392: mov         dword ptr [rsp+00000000000001C8h],0
  000000014082A39D: mov         qword ptr [rsp+00000000000001C0h],0
  000000014082A3A9: mov         qword ptr [rsp+00000000000001B8h],0
  000000014082A3B5: mov         dword ptr [rsp+0000000000000190h],0
  000000014082A3C0: jmp         000000014082A3D3
  000000014082A3C2: mov         eax,dword ptr [rsp+0000000000000190h]
  000000014082A3C9: add         eax,1
  000000014082A3CC: mov         dword ptr [rsp+0000000000000190h],eax
  000000014082A3D3: cmp         dword ptr [rsp+0000000000000190h],4Fh
  000000014082A3DB: jge         000000014082A402
  000000014082A3DD: movsxd      rdx,dword ptr [rsp+0000000000000190h]
  000000014082A3E5: movsxd      rcx,dword ptr [rsp+0000000000000190h]
  000000014082A3ED: mov         rax,qword ptr [rsp+0000000000000220h]
  000000014082A3F5: mov         eax,dword ptr [rax+rdx*4+00000000000015F8h]
  000000014082A3FC: mov         dword ptr [rsp+rcx*4+50h],eax
  000000014082A400: jmp         000000014082A3C2
  000000014082A402: xor         edx,edx
  000000014082A404: lea         rcx,[rsp+0000000000000198h]
  000000014082A40C: call        000000014084E8E0
  000000014082A411: nop
  000000014082A412: lea         rcx,[rsp+0000000000000198h]
  000000014082A41A: call        000000014084E920
  000000014082A41F: mov         rdx,qword ptr [rsp+0000000000000220h]
  000000014082A427: lea         rcx,[rsp+0000000000000198h]
  000000014082A42F: call        00000001408316D0
  000000014082A434: movzx       eax,byte ptr [rsp+0000000000000228h]
  000000014082A43C: test        eax,eax
  000000014082A43E: je          000000014082A478
  000000014082A440: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A448: add         rcx,1328h
  000000014082A44F: call        0000000140B172D0
  000000014082A454: mov         qword ptr [rsp+00000000000001C0h],rax
  000000014082A45C: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A464: add         rcx,1328h
  000000014082A46B: call        00000001401CC700
  000000014082A470: mov         qword ptr [rsp+00000000000001B8h],rax
  000000014082A478: mov         dword ptr [rsp+00000000000001C8h],0
  000000014082A483: jmp         000000014082A496
  000000014082A485: mov         eax,dword ptr [rsp+00000000000001C8h]
  000000014082A48C: add         eax,1
  000000014082A48F: mov         dword ptr [rsp+00000000000001C8h],eax
  000000014082A496: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A49E: add         rcx,1380h
  000000014082A4A5: call        00000001407D3070
  000000014082A4AA: cmp         dword ptr [rsp+00000000000001C8h],eax
  000000014082A4B1: jge         000000014082A5FF
  000000014082A4B7: mov         edx,dword ptr [rsp+00000000000001C8h]
  000000014082A4BE: add         edx,1
  000000014082A4C1: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A4C9: add         rcx,1380h
  000000014082A4D0: call        000000014084ECA0
  000000014082A4D5: mov         qword ptr [rsp+00000000000001D0h],rax
  000000014082A4DD: movzx       eax,byte ptr [rsp+0000000000000228h]
  000000014082A4E5: test        eax,eax
  000000014082A4E7: je          000000014082A5FA
  000000014082A4ED: cmp         qword ptr [rsp+00000000000001D0h],0
  000000014082A4F6: je          000000014082A5FA
  000000014082A4FC: mov         rcx,qword ptr [rsp+00000000000001D0h]
  000000014082A504: call        0000000140834FA0
  000000014082A509: movzx       eax,al
  000000014082A50C: test        eax,eax
  000000014082A50E: je          000000014082A5FA
  000000014082A514: cmp         qword ptr [rsp+00000000000001C0h],0
  000000014082A51D: je          000000014082A5FA
  000000014082A523: cmp         qword ptr [rsp+00000000000001B8h],0
  000000014082A52C: je          000000014082A5FA
  000000014082A532: lea         rcx,[rsp+0000000000000198h]
  000000014082A53A: call        000000014084EAA0
  000000014082A53F: mov         qword ptr [rsp+00000000000001D8h],rax
  000000014082A547: mov         byte ptr [rsp+00000000000001FCh],1
  000000014082A54F: mov         byte ptr [rsp+00000000000001FDh],0
  000000014082A557: mov         byte ptr [rsp+00000000000001FEh],1
  000000014082A55F: mov         byte ptr [rsp+00000000000001FFh],1
  000000014082A567: mov         byte ptr [rsp+0000000000000200h],1
  000000014082A56F: mov         byte ptr [rsp+0000000000000201h],1
  000000014082A577: mov         r8,qword ptr [rsp+0000000000000220h]
  000000014082A57F: add         r8,15F8h
  000000014082A586: lea         rax,[rsp+00000000000001FCh]
  000000014082A58E: mov         qword ptr [rsp+40h],rax
  000000014082A593: lea         rax,[rsp+00000000000001FDh]
  000000014082A59B: mov         qword ptr [rsp+38h],rax
  000000014082A5A0: lea         rax,[rsp+00000000000001FEh]
  000000014082A5A8: mov         qword ptr [rsp+30h],rax
  000000014082A5AD: lea         rax,[rsp+00000000000001FFh]
  000000014082A5B5: mov         qword ptr [rsp+28h],rax
  000000014082A5BA: lea         rax,[rsp+0000000000000200h]
  000000014082A5C2: mov         qword ptr [rsp+20h],rax
  000000014082A5C7: lea         r9,[rsp+0000000000000201h]
  000000014082A5CF: mov         rdx,qword ptr [rsp+00000000000001D0h]
  000000014082A5D7: mov         rcx,qword ptr [rsp+00000000000001D8h]
  000000014082A5DF: call        0000000140835430
  000000014082A5E4: mov         rcx,qword ptr [rsp+00000000000001D8h]
  000000014082A5EC: mov         rax,qword ptr [rsp+00000000000001D0h]
  000000014082A5F4: mov         eax,dword ptr [rax+8]
  000000014082A5F7: mov         dword ptr [rcx+8],eax
  000000014082A5FA: jmp         000000014082A485
  000000014082A5FF: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A607: call        00000001408296C0
  000000014082A60C: mov         dword ptr [rsp+00000000000001C8h],0
  000000014082A617: jmp         000000014082A62A
  000000014082A619: mov         eax,dword ptr [rsp+00000000000001C8h]
  000000014082A620: add         eax,1
  000000014082A623: mov         dword ptr [rsp+00000000000001C8h],eax
  000000014082A62A: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A632: add         rcx,1380h
  000000014082A639: call        00000001407D3070
  000000014082A63E: cmp         dword ptr [rsp+00000000000001C8h],eax
  000000014082A645: jge         000000014082A9D6
  000000014082A64B: mov         edx,dword ptr [rsp+00000000000001C8h]
  000000014082A652: add         edx,1
  000000014082A655: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A65D: add         rcx,1380h
  000000014082A664: call        000000014084ECA0
  000000014082A669: mov         qword ptr [rsp+00000000000001E0h],rax
  000000014082A671: cmp         qword ptr [rsp+00000000000001E0h],0
  000000014082A67A: je          000000014082A9D1
  000000014082A680: mov         rcx,qword ptr [rsp+00000000000001E0h]
  000000014082A688: call        0000000140834FA0
  000000014082A68D: movzx       eax,al
  000000014082A690: test        eax,eax
  000000014082A692: je          000000014082A9D1
  000000014082A698: mov         rdx,qword ptr [rsp+0000000000000220h]
  000000014082A6A0: add         rdx,15F8h
  000000014082A6A7: lea         r8,[rsp+50h]
  000000014082A6AC: mov         rcx,qword ptr [rsp+00000000000001E0h]
  000000014082A6B4: call        000000014086D7F0
  000000014082A6B9: movzx       eax,byte ptr [rsp+0000000000000228h]
  000000014082A6C1: test        eax,eax
  000000014082A6C3: je          000000014082A9D1
  000000014082A6C9: cmp         qword ptr [rsp+00000000000001C0h],0
  000000014082A6D2: je          000000014082A9D1
  000000014082A6D8: cmp         qword ptr [rsp+00000000000001B8h],0
  000000014082A6E1: je          000000014082A9D1
  000000014082A6E7: mov         rax,qword ptr [rsp+00000000000001E0h]
  000000014082A6EF: mov         edx,dword ptr [rax+8]
  000000014082A6F2: lea         rcx,[rsp+0000000000000198h]
  000000014082A6FA: call        000000014084ED10
  000000014082A6FF: mov         qword ptr [rsp+00000000000001E8h],rax
  000000014082A707: cmp         qword ptr [rsp+00000000000001E8h],0
  000000014082A710: je          000000014082A9D1
  000000014082A716: xor         r8d,r8d
  000000014082A719: mov         edx,31h
  000000014082A71E: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A726: call        000000014082F8C0
  000000014082A72B: movzx       eax,al
  000000014082A72E: test        eax,eax
  000000014082A730: je          000000014082A9D1
  000000014082A736: mov         dword ptr [rsp+00000000000001CCh],0
  000000014082A741: jmp         000000014082A754
  000000014082A743: mov         eax,dword ptr [rsp+00000000000001CCh]
  000000014082A74A: add         eax,1
  000000014082A74D: mov         dword ptr [rsp+00000000000001CCh],eax
  000000014082A754: mov         rax,qword ptr [rsp+0000000000000220h]
  000000014082A75C: mov         eax,dword ptr [rax+00000000000016BCh]
  000000014082A762: cmp         dword ptr [rsp+00000000000001CCh],eax
  000000014082A769: jge         000000014082A81E
  000000014082A76F: mov         dword ptr [rsp+0000000000000190h],0
  000000014082A77A: jmp         000000014082A78D
  000000014082A77C: mov         eax,dword ptr [rsp+0000000000000190h]
  000000014082A783: add         eax,1
  000000014082A786: mov         dword ptr [rsp+0000000000000190h],eax
  000000014082A78D: mov         rax,qword ptr [rsp+00000000000001E0h]
  000000014082A795: mov         eax,dword ptr [rax+0000000000000884h]
  000000014082A79B: cmp         dword ptr [rsp+0000000000000190h],eax
  000000014082A7A2: jge         000000014082A819
  000000014082A7A4: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A7AC: mov         eax,dword ptr [rsp+0000000000000190h]
  000000014082A7B3: imul        eax,dword ptr [rcx+00000000000016BCh]
  000000014082A7BA: add         eax,dword ptr [rsp+00000000000001CCh]
  000000014082A7C1: movsxd      rcx,eax
  000000014082A7C4: mov         rax,qword ptr [rsp+00000000000001E0h]
  000000014082A7CC: mov         rax,qword ptr [rax+0000000000000A58h]
  000000014082A7D3: xorpd       xmm0,xmm0
  000000014082A7D7: movsd       mmword ptr [rax+rcx*8],xmm0
  000000014082A7DC: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A7E4: mov         eax,dword ptr [rsp+0000000000000190h]
  000000014082A7EB: imul        eax,dword ptr [rcx+00000000000016BCh]
  000000014082A7F2: add         eax,dword ptr [rsp+00000000000001CCh]
  000000014082A7F9: movsxd      rcx,eax
  000000014082A7FC: mov         rax,qword ptr [rsp+00000000000001E0h]
  000000014082A804: mov         rax,qword ptr [rax+0000000000000A60h]
  000000014082A80B: xorpd       xmm0,xmm0
  000000014082A80F: movsd       mmword ptr [rax+rcx*8],xmm0
  000000014082A814: jmp         000000014082A77C
  000000014082A819: jmp         000000014082A743
  000000014082A81E: mov         dword ptr [rsp+00000000000001CCh],0
  000000014082A829: jmp         000000014082A83C
  000000014082A82B: mov         eax,dword ptr [rsp+00000000000001CCh]
  000000014082A832: add         eax,1
  000000014082A835: mov         dword ptr [rsp+00000000000001CCh],eax
  000000014082A83C: mov         eax,dword ptr [rsp+0000000000000114h]
  000000014082A843: cmp         dword ptr [rsp+00000000000001CCh],eax
  000000014082A84A: jge         000000014082A9D1
  000000014082A850: movsxd      rcx,dword ptr [rsp+00000000000001CCh]
  000000014082A858: mov         rax,qword ptr [rsp+00000000000001C0h]
  000000014082A860: mov         eax,dword ptr [rax+rcx*4]
  000000014082A863: mov         dword ptr [rsp+00000000000001F8h],eax
  000000014082A86A: movsxd      rcx,dword ptr [rsp+00000000000001CCh]
  000000014082A872: mov         rax,qword ptr [rsp+00000000000001B8h]
  000000014082A87A: mov         eax,dword ptr [rax+rcx*4]
  000000014082A87D: mov         dword ptr [rsp+00000000000001F4h],eax
  000000014082A884: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A88C: add         rcx,1328h
  000000014082A893: mov         edx,dword ptr [rsp+00000000000001F8h]
  000000014082A89A: call        00000001408BC900
  000000014082A89F: mov         dword ptr [rsp+00000000000001F0h],eax
  000000014082A8A6: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A8AE: add         rcx,1328h
  000000014082A8B5: mov         edx,dword ptr [rsp+00000000000001F8h]
  000000014082A8BC: call        00000001408BC880
  000000014082A8C1: test        eax,eax
  000000014082A8C3: jge         000000014082A8CA
  000000014082A8C5: jmp         000000014082A82B
  000000014082A8CA: mov         dword ptr [rsp+0000000000000190h],0
  000000014082A8D5: jmp         000000014082A8E8
  000000014082A8D7: mov         eax,dword ptr [rsp+0000000000000190h]
  000000014082A8DE: add         eax,1
  000000014082A8E1: mov         dword ptr [rsp+0000000000000190h],eax
  000000014082A8E8: mov         rax,qword ptr [rsp+00000000000001E0h]
  000000014082A8F0: mov         eax,dword ptr [rax+0000000000000884h]
  000000014082A8F6: cmp         dword ptr [rsp+0000000000000190h],eax
  000000014082A8FD: jge         000000014082A9CC
  000000014082A903: mov         eax,dword ptr [rsp+0000000000000190h]
  000000014082A90A: imul        eax,dword ptr [rsp+0000000000000114h]
  000000014082A912: add         eax,dword ptr [rsp+00000000000001F4h]
  000000014082A919: movsxd      r8,eax
  000000014082A91C: mov         rdx,qword ptr [rsp+00000000000001E8h]
  000000014082A924: mov         rdx,qword ptr [rdx+0000000000000A58h]
  000000014082A92B: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A933: mov         eax,dword ptr [rsp+0000000000000190h]
  000000014082A93A: imul        eax,dword ptr [rcx+00000000000016BCh]
  000000014082A941: add         eax,dword ptr [rsp+00000000000001F0h]
  000000014082A948: movsxd      rcx,eax
  000000014082A94B: mov         rax,qword ptr [rsp+00000000000001E0h]
  000000014082A953: mov         rax,qword ptr [rax+0000000000000A58h]
  000000014082A95A: movsd       xmm0,mmword ptr [rdx+r8*8]
  000000014082A960: movsd       mmword ptr [rax+rcx*8],xmm0
  000000014082A965: mov         eax,dword ptr [rsp+0000000000000190h]
  000000014082A96C: imul        eax,dword ptr [rsp+0000000000000114h]
  000000014082A974: add         eax,dword ptr [rsp+00000000000001F4h]
  000000014082A97B: movsxd      r8,eax
  000000014082A97E: mov         rdx,qword ptr [rsp+00000000000001E8h]
  000000014082A986: mov         rdx,qword ptr [rdx+0000000000000A60h]
  000000014082A98D: mov         rcx,qword ptr [rsp+0000000000000220h]
  000000014082A995: mov         eax,dword ptr [rsp+0000000000000190h]
  000000014082A99C: imul        eax,dword ptr [rcx+00000000000016BCh]
  000000014082A9A3: add         eax,dword ptr [rsp+00000000000001F0h]
  000000014082A9AA: movsxd      rcx,eax
  000000014082A9AD: mov         rax,qword ptr [rsp+00000000000001E0h]
  000000014082A9B5: mov         rax,qword ptr [rax+0000000000000A60h]
  000000014082A9BC: movsd       xmm0,mmword ptr [rdx+r8*8]
  000000014082A9C2: movsd       mmword ptr [rax+rcx*8],xmm0
  000000014082A9C7: jmp         000000014082A8D7
  000000014082A9CC: jmp         000000014082A82B
  000000014082A9D1: jmp         000000014082A619
  000000014082A9D6: lea         rcx,[rsp+0000000000000198h]
  000000014082A9DE: call        000000014084ED90
  000000014082A9E3: nop
  000000014082A9E4: lea         rcx,[rsp+0000000000000198h]
  000000014082A9EC: call        000000014084E960
  000000014082A9F1: add         rsp,218h
  000000014082A9F8: ret
  000000014082A9F9: int         3

  Summary

     1E02000 .text

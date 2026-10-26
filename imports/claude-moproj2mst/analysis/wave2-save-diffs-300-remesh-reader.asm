Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  00000001400AB170: mov         qword ptr [rsp+8],rcx
  00000001400AB175: sub         rsp,168h
  00000001400AB17C: mov         qword ptr [rsp+0000000000000128h],0FFFFFFFFFFFFFFFEh
  00000001400AB188: mov         rax,qword ptr [0000000143B5E030h]
  00000001400AB18F: xor         rax,rsp
  00000001400AB192: mov         qword ptr [rsp+0000000000000158h],rax
  00000001400AB19A: lea         rcx,[rsp+0000000000000120h]
  00000001400AB1A2: call        0000000140906740
  00000001400AB1A7: nop
  00000001400AB1A8: mov         byte ptr [rsp+000000000000011Ch],0
  00000001400AB1B0: lea         rdx,[0000000142142194h]
  00000001400AB1B7: lea         rcx,[0000000142142180h]
  00000001400AB1BE: call        000000014146FCD8
  00000001400AB1C3: mov         qword ptr [rsp+0000000000000110h],rax
  00000001400AB1CB: cmp         qword ptr [rsp+0000000000000110h],0
  00000001400AB1D4: je          00000001400AB24B
  00000001400AB1D6: mov         dword ptr [rsp+20h],0FFFFEC78h
  00000001400AB1DE: lea         r9,[rsp+40h]
  00000001400AB1E3: mov         r8d,2
  00000001400AB1E9: mov         rdx,qword ptr [rsp+0000000000000110h]
  00000001400AB1F1: lea         rcx,[rsp+0000000000000120h]
  00000001400AB1F9: call        00000001409068F0
  00000001400AB1FE: mov         r8d,50h
  00000001400AB204: lea         rdx,[0000000142142170h]
  00000001400AB20B: lea         rcx,[rsp+40h]
  00000001400AB210: call        0000000141470380
  00000001400AB215: test        eax,eax
  00000001400AB217: jne         00000001400AB223
  00000001400AB219: mov         byte ptr [rsp+000000000000011Ch],1
  00000001400AB221: jmp         00000001400AB23E
  00000001400AB223: mov         r8d,50h
  00000001400AB229: lea         rdx,[0000000142142164h]
  00000001400AB230: lea         rcx,[rsp+40h]
  00000001400AB235: call        0000000141470380
  00000001400AB23A: test        eax,eax
  00000001400AB23C: jne         00000001400AB1D6
  00000001400AB23E: mov         rcx,qword ptr [rsp+0000000000000110h]
  00000001400AB246: call        0000000141470298
  00000001400AB24B: movzx       eax,byte ptr [rsp+000000000000011Ch]
  00000001400AB253: test        eax,eax
  00000001400AB255: jne         00000001400AB2B6
  00000001400AB257: cmp         qword ptr [rsp+0000000000000110h],0
  00000001400AB260: je          00000001400AB26F
  00000001400AB262: mov         rcx,qword ptr [rsp+0000000000000110h]
  00000001400AB26A: call        000000014146FB28
  00000001400AB26F: lea         rdx,[0000000142142194h]
  00000001400AB276: lea         rcx,[0000000142142150h]
  00000001400AB27D: call        000000014146FCD8
  00000001400AB282: mov         qword ptr [rsp+0000000000000110h],rax
  00000001400AB28A: cmp         qword ptr [rsp+0000000000000110h],0
  00000001400AB293: jne         00000001400AB2B6
  00000001400AB295: mov         byte ptr [rsp+0000000000000124h],0
  00000001400AB29D: lea         rcx,[rsp+0000000000000120h]
  00000001400AB2A5: call        0000000140906760
  00000001400AB2AA: mov         al,byte ptr [rsp+0000000000000124h]
  00000001400AB2B1: jmp         00000001400AB6D0
  00000001400AB2B6: mov         dword ptr [rsp+20h],0FFFFEC78h
  00000001400AB2BE: lea         r9,[rsp+40h]
  00000001400AB2C3: mov         r8d,2
  00000001400AB2C9: mov         rdx,qword ptr [rsp+0000000000000110h]
  00000001400AB2D1: lea         rcx,[rsp+0000000000000120h]
  00000001400AB2D9: call        00000001409068F0
  00000001400AB2DE: mov         r8d,50h
  00000001400AB2E4: lea         rdx,[0000000142142128h]
  00000001400AB2EB: lea         rcx,[rsp+40h]
  00000001400AB2F0: call        0000000141470380
  00000001400AB2F5: test        eax,eax
  00000001400AB2F7: jne         00000001400AB39A
  00000001400AB2FD: mov         dword ptr [rsp+20h],0
  00000001400AB305: lea         r9,[rsp+0000000000000118h]
  00000001400AB30D: mov         r8d,5
  00000001400AB313: mov         rdx,qword ptr [rsp+0000000000000110h]
  00000001400AB31B: lea         rcx,[rsp+0000000000000120h]
  00000001400AB323: call        00000001409068F0
  00000001400AB328: mov         r11,qword ptr [rsp+0000000000000170h]
  00000001400AB330: mov         eax,dword ptr [rsp+0000000000000118h]
  00000001400AB337: mov         dword ptr [r11],eax
  00000001400AB33A: cmp         dword ptr [rsp+0000000000000118h],0
  00000001400AB342: jge         00000001400AB351
  00000001400AB344: mov         dword ptr [rsp+0000000000000130h],0
  00000001400AB34F: jmp         00000001400AB384
  00000001400AB351: cmp         dword ptr [rsp+0000000000000118h],5
  00000001400AB359: jle         00000001400AB368
  00000001400AB35B: mov         dword ptr [rsp+0000000000000134h],5
  00000001400AB366: jmp         00000001400AB376
  00000001400AB368: mov         eax,dword ptr [rsp+0000000000000118h]
  00000001400AB36F: mov         dword ptr [rsp+0000000000000134h],eax
  00000001400AB376: mov         eax,dword ptr [rsp+0000000000000134h]
  00000001400AB37D: mov         dword ptr [rsp+0000000000000130h],eax
  00000001400AB384: mov         rcx,qword ptr [rsp+0000000000000170h]
  00000001400AB38C: mov         eax,dword ptr [rsp+0000000000000130h]
  00000001400AB393: mov         dword ptr [rcx],eax
  00000001400AB395: jmp         00000001400AB688
  00000001400AB39A: mov         r8d,50h
  00000001400AB3A0: lea         rdx,[0000000142142100h]
  00000001400AB3A7: lea         rcx,[rsp+40h]
  00000001400AB3AC: call        0000000141470380
  00000001400AB3B1: test        eax,eax
  00000001400AB3B3: jne         00000001400AB414
  00000001400AB3B5: mov         dword ptr [rsp+20h],0
  00000001400AB3BD: lea         r9,[rsp+0000000000000118h]
  00000001400AB3C5: mov         r8d,5
  00000001400AB3CB: mov         rdx,qword ptr [rsp+0000000000000110h]
  00000001400AB3D3: lea         rcx,[rsp+0000000000000120h]
  00000001400AB3DB: call        00000001409068F0
  00000001400AB3E0: cmp         dword ptr [rsp+0000000000000118h],0
  00000001400AB3E8: je          00000001400AB3F4
  00000001400AB3EA: mov         byte ptr [rsp+0000000000000138h],1
  00000001400AB3F2: jmp         00000001400AB3FC
  00000001400AB3F4: mov         byte ptr [rsp+0000000000000138h],0
  00000001400AB3FC: mov         rcx,qword ptr [rsp+0000000000000170h]
  00000001400AB404: movzx       eax,byte ptr [rsp+0000000000000138h]
  00000001400AB40C: mov         byte ptr [rcx+4],al
  00000001400AB40F: jmp         00000001400AB688
  00000001400AB414: mov         r8d,50h
  00000001400AB41A: lea         rdx,[00000001421420F0h]
  00000001400AB421: lea         rcx,[rsp+40h]
  00000001400AB426: call        0000000141470380
  00000001400AB42B: test        eax,eax
  00000001400AB42D: jne         00000001400AB463
  00000001400AB42F: mov         r9,qword ptr [rsp+0000000000000170h]
  00000001400AB437: add         r9,8
  00000001400AB43B: mov         dword ptr [rsp+20h],0
  00000001400AB443: mov         r8d,0Ah
  00000001400AB449: mov         rdx,qword ptr [rsp+0000000000000110h]
  00000001400AB451: lea         rcx,[rsp+0000000000000120h]
  00000001400AB459: call        00000001409068F0
  00000001400AB45E: jmp         00000001400AB688
  00000001400AB463: mov         r8d,50h
  00000001400AB469: lea         rdx,[00000001421420D0h]
  00000001400AB470: lea         rcx,[rsp+40h]
  00000001400AB475: call        0000000141470380
  00000001400AB47A: test        eax,eax
  00000001400AB47C: jne         00000001400AB4B2
  00000001400AB47E: mov         r9,qword ptr [rsp+0000000000000170h]
  00000001400AB486: add         r9,10h
  00000001400AB48A: mov         dword ptr [rsp+20h],0
  00000001400AB492: mov         r8d,4
  00000001400AB498: mov         rdx,qword ptr [rsp+0000000000000110h]
  00000001400AB4A0: lea         rcx,[rsp+0000000000000120h]
  00000001400AB4A8: call        00000001409068F0
  00000001400AB4AD: jmp         00000001400AB688
  00000001400AB4B2: mov         r8d,50h
  00000001400AB4B8: lea         rdx,[00000001421420A8h]
  00000001400AB4BF: lea         rcx,[rsp+40h]
  00000001400AB4C4: call        0000000141470380
  00000001400AB4C9: test        eax,eax
  00000001400AB4CB: jne         00000001400AB52C
  00000001400AB4CD: mov         dword ptr [rsp+20h],0
  00000001400AB4D5: lea         r9,[rsp+0000000000000118h]
  00000001400AB4DD: mov         r8d,5
  00000001400AB4E3: mov         rdx,qword ptr [rsp+0000000000000110h]
  00000001400AB4EB: lea         rcx,[rsp+0000000000000120h]
  00000001400AB4F3: call        00000001409068F0
  00000001400AB4F8: cmp         dword ptr [rsp+0000000000000118h],0
  00000001400AB500: je          00000001400AB50C
  00000001400AB502: mov         byte ptr [rsp+0000000000000139h],1
  00000001400AB50A: jmp         00000001400AB514
  00000001400AB50C: mov         byte ptr [rsp+0000000000000139h],0
  00000001400AB514: mov         rcx,qword ptr [rsp+0000000000000170h]
  00000001400AB51C: movzx       eax,byte ptr [rsp+0000000000000139h]
  00000001400AB524: mov         byte ptr [rcx+18h],al
  00000001400AB527: jmp         00000001400AB688
  00000001400AB52C: mov         r8d,50h
  00000001400AB532: lea         rdx,[0000000142142080h]
  00000001400AB539: lea         rcx,[rsp+40h]
  00000001400AB53E: call        0000000141470380
  00000001400AB543: test        eax,eax
  00000001400AB545: jne         00000001400AB613
  00000001400AB54B: mov         r9,qword ptr [rsp+0000000000000170h]
  00000001400AB553: add         r9,20h
  00000001400AB557: mov         dword ptr [rsp+20h],0
  00000001400AB55F: mov         r8d,0Ah
  00000001400AB565: mov         rdx,qword ptr [rsp+0000000000000110h]
  00000001400AB56D: lea         rcx,[rsp+0000000000000120h]
  00000001400AB575: call        00000001409068F0
  00000001400AB57A: mov         r11,qword ptr [rsp+0000000000000170h]
  00000001400AB582: movsd       xmm1,mmword ptr [r11+20h]
  00000001400AB588: movsd       xmm0,mmword ptr [00000001421249E0h]
  00000001400AB590: comisd      xmm0,xmm1
  00000001400AB594: jbe         00000001400AB5A9
  00000001400AB596: movsd       xmm0,mmword ptr [00000001421249E0h]
  00000001400AB59E: movsd       mmword ptr [rsp+0000000000000140h],xmm0
  00000001400AB5A7: jmp         00000001400AB5FB
  00000001400AB5A9: mov         rax,qword ptr [rsp+0000000000000170h]
  00000001400AB5B1: movsd       xmm0,mmword ptr [rax+20h]
  00000001400AB5B6: comisd      xmm0,mmword ptr [0000000142142078h]
  00000001400AB5BE: jbe         00000001400AB5D3
  00000001400AB5C0: movsd       xmm0,mmword ptr [0000000142142078h]
  00000001400AB5C8: movsd       mmword ptr [rsp+0000000000000148h],xmm0
  00000001400AB5D1: jmp         00000001400AB5E9
  00000001400AB5D3: mov         rax,qword ptr [rsp+0000000000000170h]
  00000001400AB5DB: movsd       xmm0,mmword ptr [rax+20h]
  00000001400AB5E0: movsd       mmword ptr [rsp+0000000000000148h],xmm0
  00000001400AB5E9: movsd       xmm0,mmword ptr [rsp+0000000000000148h]
  00000001400AB5F2: movsd       mmword ptr [rsp+0000000000000140h],xmm0
  00000001400AB5FB: mov         rax,qword ptr [rsp+0000000000000170h]
  00000001400AB603: movsd       xmm0,mmword ptr [rsp+0000000000000140h]
  00000001400AB60C: movsd       mmword ptr [rax+20h],xmm0
  00000001400AB611: jmp         00000001400AB688
  00000001400AB613: mov         r8d,50h
  00000001400AB619: lea         rdx,[0000000142142058h]
  00000001400AB620: lea         rcx,[rsp+40h]
  00000001400AB625: call        0000000141470380
  00000001400AB62A: test        eax,eax
  00000001400AB62C: jne         00000001400AB688
  00000001400AB62E: mov         dword ptr [rsp+20h],0
  00000001400AB636: lea         r9,[rsp+0000000000000118h]
  00000001400AB63E: mov         r8d,5
  00000001400AB644: mov         rdx,qword ptr [rsp+0000000000000110h]
  00000001400AB64C: lea         rcx,[rsp+0000000000000120h]
  00000001400AB654: call        00000001409068F0
  00000001400AB659: cmp         dword ptr [rsp+0000000000000118h],0
  00000001400AB661: je          00000001400AB66D
  00000001400AB663: mov         byte ptr [rsp+0000000000000150h],1
  00000001400AB66B: jmp         00000001400AB675
  00000001400AB66D: mov         byte ptr [rsp+0000000000000150h],0
  00000001400AB675: mov         rcx,qword ptr [rsp+0000000000000170h]
  00000001400AB67D: movzx       eax,byte ptr [rsp+0000000000000150h]
  00000001400AB685: mov         byte ptr [rcx+28h],al
  00000001400AB688: mov         r8d,50h
  00000001400AB68E: lea         rdx,[0000000142142164h]
  00000001400AB695: lea         rcx,[rsp+40h]
  00000001400AB69A: call        0000000141470380
  00000001400AB69F: test        eax,eax
  00000001400AB6A1: jne         00000001400AB2B6
  00000001400AB6A7: mov         rcx,qword ptr [rsp+0000000000000110h]
  00000001400AB6AF: call        000000014146FB28
  00000001400AB6B4: mov         byte ptr [rsp+0000000000000125h],1
  00000001400AB6BC: lea         rcx,[rsp+0000000000000120h]
  00000001400AB6C4: call        0000000140906760
  00000001400AB6C9: mov         al,byte ptr [rsp+0000000000000125h]
  00000001400AB6D0: mov         rcx,qword ptr [rsp+0000000000000158h]
  00000001400AB6D8: xor         rcx,rsp
  00000001400AB6DB: call        000000014146DF10
  00000001400AB6E0: add         rsp,168h
  00000001400AB6E7: ret
  00000001400AB6E8: int         3

  Summary

     1E02000 .text

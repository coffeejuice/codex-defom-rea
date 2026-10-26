Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\gui_mo\DEF_GUI_MO_64.exe

File Type: EXECUTABLE IMAGE

  0000000140ED01B0: mov         byte ptr [rsp+10h],dl
  0000000140ED01B4: mov         qword ptr [rsp+8],rcx
  0000000140ED01B9: sub         rsp,78h
  0000000140ED01BD: mov         qword ptr [rsp+40h],0FFFFFFFFFFFFFFFEh
  0000000140ED01C6: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED01CE: call        0000000140130F90
  0000000140ED01D3: mov         qword ptr [rsp+20h],rax
  0000000140ED01D8: cmp         qword ptr [rsp+20h],0
  0000000140ED01DE: jne         0000000140ED01E5
  0000000140ED01E0: jmp         0000000140ED0655
  0000000140ED01E5: movzx       eax,byte ptr [rsp+0000000000000088h]
  0000000140ED01ED: test        eax,eax
  0000000140ED01EF: je          0000000140ED0565
  0000000140ED01F5: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED01FD: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED0204: mov         rcx,qword ptr [rcx+0000000000000AA0h]
  0000000140ED020B: call        qword ptr [0000000141E0BFB0h]
  0000000140ED0211: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0219: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED0220: mov         rcx,qword ptr [rcx+0000000000000AB0h]
  0000000140ED0227: call        qword ptr [0000000141E0BFB0h]
  0000000140ED022D: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0235: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED023C: mov         rcx,qword ptr [rcx+0000000000000AC0h]
  0000000140ED0243: call        qword ptr [0000000141E0BFB0h]
  0000000140ED0249: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0251: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED0258: mov         rcx,qword ptr [rcx+0000000000000AD0h]
  0000000140ED025F: call        qword ptr [0000000141E0BFB0h]
  0000000140ED0265: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED026D: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED0274: mov         rcx,qword ptr [rcx+0000000000000AA8h]
  0000000140ED027B: call        qword ptr [0000000141E0BFB0h]
  0000000140ED0281: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0289: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED0290: mov         rcx,qword ptr [rcx+0000000000000AB8h]
  0000000140ED0297: call        qword ptr [0000000141E0BFB0h]
  0000000140ED029D: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED02A5: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED02AC: mov         rcx,qword ptr [rcx+0000000000000AC8h]
  0000000140ED02B3: call        qword ptr [0000000141E0BFB0h]
  0000000140ED02B9: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED02C1: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED02C8: mov         rcx,qword ptr [rcx+0000000000000AD8h]
  0000000140ED02CF: call        qword ptr [0000000141E0BFB0h]
  0000000140ED02D5: mov         rax,qword ptr [rsp+20h]
  0000000140ED02DA: cmp         dword ptr [rax+00000000000002E8h],4
  0000000140ED02E1: je          0000000140ED0346
  0000000140ED02E3: mov         rax,qword ptr [rsp+20h]
  0000000140ED02E8: mov         dword ptr [rax+00000000000002E8h],4
  0000000140ED02F2: mov         rax,qword ptr [rsp+20h]
  0000000140ED02F7: movsd       xmm0,mmword ptr [0000000142101520h]
  0000000140ED02FF: movsd       mmword ptr [rax+00000000000002F0h],xmm0
  0000000140ED0307: mov         rax,qword ptr [rsp+20h]
  0000000140ED030C: movsd       xmm0,mmword ptr [00000001421D3C70h]
  0000000140ED0314: movsd       mmword ptr [rax+00000000000002F8h],xmm0
  0000000140ED031C: mov         rax,qword ptr [rsp+20h]
  0000000140ED0321: movsd       xmm0,mmword ptr [00000001421249E0h]
  0000000140ED0329: movsd       mmword ptr [rax+0000000000000300h],xmm0
  0000000140ED0331: mov         rax,qword ptr [rsp+20h]
  0000000140ED0336: movsd       xmm0,mmword ptr [00000001421249E0h]
  0000000140ED033E: movsd       mmword ptr [rax+0000000000000308h],xmm0
  0000000140ED0346: mov         rax,qword ptr [rsp+20h]
  0000000140ED034B: cvttsd2si   eax,mmword ptr [rax+00000000000002F0h]
  0000000140ED0353: mov         dword ptr [rsp+28h],eax
  0000000140ED0357: cmp         dword ptr [rsp+28h],0
  0000000140ED035C: jl          0000000140ED0365
  0000000140ED035E: cmp         dword ptr [rsp+28h],2
  0000000140ED0363: jle         0000000140ED0382
  0000000140ED0365: mov         rax,qword ptr [rsp+20h]
  0000000140ED036A: movsd       xmm0,mmword ptr [0000000142101520h]
  0000000140ED0372: movsd       mmword ptr [rax+00000000000002F0h],xmm0
  0000000140ED037A: mov         dword ptr [rsp+28h],2
  0000000140ED0382: cmp         dword ptr [rsp+28h],0
  0000000140ED0387: jne         0000000140ED03A9
  0000000140ED0389: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0391: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED0398: xor         edx,edx
  0000000140ED039A: mov         rcx,qword ptr [rcx+0000000000000AA8h]
  0000000140ED03A1: call        qword ptr [0000000141E0B4C8h]
  0000000140ED03A7: jmp         0000000140ED03F4
  0000000140ED03A9: cmp         dword ptr [rsp+28h],1
  0000000140ED03AE: jne         0000000140ED03D3
  0000000140ED03B0: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED03B8: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED03BF: mov         edx,1
  0000000140ED03C4: mov         rcx,qword ptr [rcx+0000000000000AA8h]
  0000000140ED03CB: call        qword ptr [0000000141E0B4C8h]
  0000000140ED03D1: jmp         0000000140ED03F4
  0000000140ED03D3: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED03DB: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED03E2: mov         edx,2
  0000000140ED03E7: mov         rcx,qword ptr [rcx+0000000000000AA8h]
  0000000140ED03EE: call        qword ptr [0000000141E0B4C8h]
  0000000140ED03F4: mov         rax,qword ptr [rsp+20h]
  0000000140ED03F9: cvttsd2si   eax,mmword ptr [rax+00000000000002F8h]
  0000000140ED0401: mov         dword ptr [rsp+28h],eax
  0000000140ED0405: cmp         dword ptr [rsp+28h],0
  0000000140ED040A: jg          0000000140ED0429
  0000000140ED040C: mov         rax,qword ptr [rsp+20h]
  0000000140ED0411: movsd       xmm0,mmword ptr [00000001421249E0h]
  0000000140ED0419: movsd       mmword ptr [rax+00000000000002F8h],xmm0
  0000000140ED0421: mov         dword ptr [rsp+28h],1
  0000000140ED0429: mov         r8d,0Ah
  0000000140ED042F: mov         edx,dword ptr [rsp+28h]
  0000000140ED0433: lea         rcx,[rsp+30h]
  0000000140ED0438: call        qword ptr [0000000141E03DC0h]
  0000000140ED043E: mov         qword ptr [rsp+48h],rax
  0000000140ED0443: mov         rax,qword ptr [rsp+48h]
  0000000140ED0448: mov         qword ptr [rsp+50h],rax
  0000000140ED044D: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0455: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED045C: mov         rdx,qword ptr [rsp+50h]
  0000000140ED0461: mov         rcx,qword ptr [rcx+0000000000000AB8h]
  0000000140ED0468: call        qword ptr [0000000141E066B8h]
  0000000140ED046E: nop
  0000000140ED046F: lea         rcx,[rsp+30h]
  0000000140ED0474: call        qword ptr [0000000141E059E0h]
  0000000140ED047A: mov         rax,qword ptr [rsp+20h]
  0000000140ED047F: cvttsd2si   eax,mmword ptr [rax+0000000000000300h]
  0000000140ED0487: mov         dword ptr [rsp+28h],eax
  0000000140ED048B: mov         r8d,0Ah
  0000000140ED0491: mov         edx,dword ptr [rsp+28h]
  0000000140ED0495: lea         rcx,[rsp+38h]
  0000000140ED049A: call        qword ptr [0000000141E03DC0h]
  0000000140ED04A0: mov         qword ptr [rsp+58h],rax
  0000000140ED04A5: mov         rax,qword ptr [rsp+58h]
  0000000140ED04AA: mov         qword ptr [rsp+60h],rax
  0000000140ED04AF: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED04B7: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED04BE: mov         rdx,qword ptr [rsp+60h]
  0000000140ED04C3: mov         rcx,qword ptr [rcx+0000000000000AC8h]
  0000000140ED04CA: call        qword ptr [0000000141E066B8h]
  0000000140ED04D0: nop
  0000000140ED04D1: lea         rcx,[rsp+38h]
  0000000140ED04D6: call        qword ptr [0000000141E059E0h]
  0000000140ED04DC: mov         rax,qword ptr [rsp+20h]
  0000000140ED04E1: cvttsd2si   eax,mmword ptr [rax+0000000000000308h]
  0000000140ED04E9: mov         dword ptr [rsp+28h],eax
  0000000140ED04ED: cmp         dword ptr [rsp+28h],1
  0000000140ED04F2: je          0000000140ED0518
  0000000140ED04F4: cmp         dword ptr [rsp+28h],5
  0000000140ED04F9: je          0000000140ED0518
  0000000140ED04FB: mov         rax,qword ptr [rsp+20h]
  0000000140ED0500: movsd       xmm0,mmword ptr [00000001421249E0h]
  0000000140ED0508: movsd       mmword ptr [rax+0000000000000308h],xmm0
  0000000140ED0510: mov         dword ptr [rsp+28h],1
  0000000140ED0518: cmp         dword ptr [rsp+28h],1
  0000000140ED051D: jne         0000000140ED053F
  0000000140ED051F: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0527: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED052E: xor         edx,edx
  0000000140ED0530: mov         rcx,qword ptr [rcx+0000000000000AD8h]
  0000000140ED0537: call        qword ptr [0000000141E0B4C8h]
  0000000140ED053D: jmp         0000000140ED0560
  0000000140ED053F: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0547: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED054E: mov         edx,1
  0000000140ED0553: mov         rcx,qword ptr [rcx+0000000000000AD8h]
  0000000140ED055A: call        qword ptr [0000000141E0B4C8h]
  0000000140ED0560: jmp         0000000140ED0655
  0000000140ED0565: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED056D: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED0574: mov         rcx,qword ptr [rcx+0000000000000AA0h]
  0000000140ED057B: call        qword ptr [0000000141E0BBB0h]
  0000000140ED0581: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0589: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED0590: mov         rcx,qword ptr [rcx+0000000000000AB0h]
  0000000140ED0597: call        qword ptr [0000000141E0BBB0h]
  0000000140ED059D: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED05A5: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED05AC: mov         rcx,qword ptr [rcx+0000000000000AC0h]
  0000000140ED05B3: call        qword ptr [0000000141E0BBB0h]
  0000000140ED05B9: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED05C1: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED05C8: mov         rcx,qword ptr [rcx+0000000000000AD0h]
  0000000140ED05CF: call        qword ptr [0000000141E0BBB0h]
  0000000140ED05D5: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED05DD: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED05E4: mov         rcx,qword ptr [rcx+0000000000000AA8h]
  0000000140ED05EB: call        qword ptr [0000000141E0BBB0h]
  0000000140ED05F1: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED05F9: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED0600: mov         rcx,qword ptr [rcx+0000000000000AB8h]
  0000000140ED0607: call        qword ptr [0000000141E0BBB0h]
  0000000140ED060D: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0615: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED061C: mov         rcx,qword ptr [rcx+0000000000000AC8h]
  0000000140ED0623: call        qword ptr [0000000141E0BBB0h]
  0000000140ED0629: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000140ED0631: mov         rcx,qword ptr [rcx+0000000000000140h]
  0000000140ED0638: mov         rcx,qword ptr [rcx+0000000000000AD8h]
  0000000140ED063F: call        qword ptr [0000000141E0BBB0h]
  0000000140ED0645: mov         r11,qword ptr [rsp+20h]
  0000000140ED064A: mov         dword ptr [r11+00000000000002E8h],0
  0000000140ED0655: add         rsp,78h
  0000000140ED0659: ret
  0000000140ED065A: int         3

  Summary

     1E02000 .text

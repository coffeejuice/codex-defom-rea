Microsoft (R) COFF/PE Dumper Version 14.44.35228.0
Copyright (C) Microsoft Corporation.  All rights reserved.


Dump of file evidence\original-binaries\QtCore4_64.dll

File Type: DLL

  0000000067074F90: mov         rax,rsp
  0000000067074F93: mov         qword ptr [rax+8],rcx
  0000000067074F97: push        rbp
  0000000067074F98: push        rsi
  0000000067074F99: push        rdi
  0000000067074F9A: push        r12
  0000000067074F9C: push        r13
  0000000067074F9E: push        r14
  0000000067074FA0: push        r15
  0000000067074FA2: sub         rsp,110h
  0000000067074FA9: mov         qword ptr [rsp+00000000000000F0h],0FFFFFFFFFFFFFFFEh
  0000000067074FB5: mov         qword ptr [rax+18h],rbx
  0000000067074FB9: movaps      xmmword ptr [rax-48h],xmm6
  0000000067074FBD: mov         r15,r8
  0000000067074FC0: mov         rbx,rdx
  0000000067074FC3: mov         rsi,rcx
  0000000067074FC6: xor         r8d,r8d
  0000000067074FC9: lea         edx,[r8+1]
  0000000067074FCD: lea         rcx,[rsp+68h]
  0000000067074FD2: call        000000006704D5A0
  0000000067074FD7: test        rbx,rbx
  0000000067074FDA: je          0000000067075F00
  0000000067074FE0: cmp         byte ptr [rbx],0
  0000000067074FE3: je          0000000067075F00
  0000000067074FE9: lea         r9,[00000000672F0B30h]
  0000000067074FF0: mov         qword ptr [rsp+40h],r9
  0000000067074FF5: lock add    dword ptr [00000000672F0B30h],1
  0000000067074FFD: xor         r12d,r12d
  0000000067075000: or          rbp,0FFFFFFFFFFFFFFFFh
  0000000067075004: lea         r14d,[r12+20h]
  0000000067075009: lea         r13,[0000000067000000h]
  0000000067075010: mov         r8d,r12d
  0000000067075013: mov         rdi,r12
  0000000067075016: movzx       eax,byte ptr [rbx]
  0000000067075019: test        al,al
  000000006707501B: je          0000000067075031
  000000006707501D: nop         dword ptr [rax]
  0000000067075020: cmp         al,25h
  0000000067075022: je          0000000067075031
  0000000067075024: inc         r8d
  0000000067075027: inc         rdi
  000000006707502A: mov         al,byte ptr [rdi+rbx]
  000000006707502D: test        al,al
  000000006707502F: jne         0000000067075020
  0000000067075031: mov         rcx,qword ptr [00000000672F3E10h]
  0000000067075038: test        rcx,rcx
  000000006707503B: je          0000000067075086
  000000006707503D: mov         qword ptr [rsp+20h],r12
  0000000067075042: mov         r9d,r8d
  0000000067075045: mov         r8,rbx
  0000000067075048: lea         rdx,[rsp+00000000000000A8h]
  0000000067075050: call        0000000067072460
  0000000067075055: nop
  0000000067075056: mov         rdx,rax
  0000000067075059: lea         rcx,[rsp+40h]
  000000006707505E: call        0000000067070410
  0000000067075063: nop
  0000000067075064: mov         rax,qword ptr [rsp+00000000000000A8h]
  000000006707506C: mov         ecx,ebp
  000000006707506E: lock xadd   dword ptr [rax],ecx
  0000000067075072: add         ecx,0FFFFFFFFh
  0000000067075075: jne         00000000670750C5
  0000000067075077: mov         rcx,qword ptr [rsp+00000000000000A8h]
  000000006707507F: call        00000000670700D0
  0000000067075084: jmp         00000000670750C5
  0000000067075086: mov         rdx,rbx
  0000000067075089: lea         rcx,[rsp+00000000000000B0h]
  0000000067075091: call        00000000670741E0
  0000000067075096: nop
  0000000067075097: mov         rdx,rax
  000000006707509A: lea         rcx,[rsp+40h]
  000000006707509F: call        0000000067070410
  00000000670750A4: nop
  00000000670750A5: mov         rax,qword ptr [rsp+00000000000000B0h]
  00000000670750AD: mov         ecx,ebp
  00000000670750AF: lock xadd   dword ptr [rax],ecx
  00000000670750B3: add         ecx,0FFFFFFFFh
  00000000670750B6: jne         00000000670750C5
  00000000670750B8: mov         rcx,qword ptr [rsp+00000000000000B0h]
  00000000670750C0: call        00000000670700D0
  00000000670750C5: add         rbx,rdi
  00000000670750C8: cmp         byte ptr [rbx],0
  00000000670750CB: je          0000000067075EB9
  00000000670750D1: mov         rsi,rbx
  00000000670750D4: inc         rbx
  00000000670750D7: movzx       eax,byte ptr [rbx]
  00000000670750DA: test        al,al
  00000000670750DC: je          0000000067075E57
  00000000670750E2: cmp         al,25h
  00000000670750E4: jne         0000000067075153
  00000000670750E6: mov         esi,25h
  00000000670750EB: mov         word ptr [rsp+74h],si
  00000000670750F0: mov         rax,qword ptr [rsp+40h]
  00000000670750F5: mov         ecx,dword ptr [rax]
  00000000670750F7: mov         rax,qword ptr [rsp+40h]
  00000000670750FC: cmp         ecx,1
  00000000670750FF: jne         000000006707510B
  0000000067075101: mov         ecx,dword ptr [rax+8]
  0000000067075104: inc         ecx
  0000000067075106: cmp         ecx,dword ptr [rax+4]
  0000000067075109: jle         000000006707512F
  000000006707510B: mov         ecx,dword ptr [rax+8]
  000000006707510E: inc         ecx
  0000000067075110: add         ecx,ecx
  0000000067075112: mov         edx,r14d
  0000000067075115: call        000000006702CAA0
  000000006707511A: movsxd      rdx,eax
  000000006707511D: shr         rdx,1
  0000000067075120: lea         rcx,[rsp+40h]
  0000000067075125: call        00000000670701A0
  000000006707512A: mov         rax,qword ptr [rsp+40h]
  000000006707512F: movsxd      rdx,dword ptr [rax+8]
  0000000067075133: mov         rcx,qword ptr [rax+10h]
  0000000067075137: mov         word ptr [rcx+rdx*2],si
  000000006707513B: inc         dword ptr [rax+8]
  000000006707513E: movsxd      rdx,dword ptr [rax+8]
  0000000067075142: mov         rcx,qword ptr [rax+10h]
  0000000067075146: mov         word ptr [rcx+rdx*2],r12w
  000000006707514B: inc         rbx
  000000006707514E: jmp         0000000067075010
  0000000067075153: xor         edi,edi
  0000000067075155: mov         r14d,edi
  0000000067075158: nop         dword ptr [rax+rax+0000000000000000h]
  0000000067075160: movsx       eax,byte ptr [rbx]
  0000000067075163: add         eax,0FFFFFFE0h
  0000000067075166: cmp         eax,10h
  0000000067075169: ja          00000000670751A1
  000000006707516B: cdqe
  000000006707516D: mov         ecx,dword ptr [r13+rax*4+0000000000075F84h]
  0000000067075175: add         rcx,r13
  0000000067075178: jmp         rcx
  000000006707517A: or          r14d,1
  000000006707517E: jmp         000000006707519C
  0000000067075180: or          r14d,2
  0000000067075184: jmp         000000006707519C
  0000000067075186: or          r14d,4
  000000006707518A: jmp         000000006707519C
  000000006707518C: or          r14d,8
  0000000067075190: jmp         000000006707519C
  0000000067075192: or          r14d,10h
  0000000067075196: jmp         000000006707519C
  0000000067075198: or          r14d,20h
  000000006707519C: inc         rbx
  000000006707519F: jmp         0000000067075160
  00000000670751A1: movzx       ecx,byte ptr [rbx]
  00000000670751A4: test        cl,cl
  00000000670751A6: je          0000000067075DCA
  00000000670751AC: mov         r13d,ebp
  00000000670751AF: lea         eax,[rcx-30h]
  00000000670751B2: cmp         al,9
  00000000670751B4: ja          0000000067075323
  00000000670751BA: lea         rax,[00000000672F0B30h]
  00000000670751C1: mov         qword ptr [rsp+58h],rax
  00000000670751C6: lock add    dword ptr [00000000672F0B30h],1
  00000000670751CE: movzx       eax,byte ptr [rbx]
  00000000670751D1: test        al,al
  00000000670751D3: je          0000000067075270
  00000000670751D9: nop         dword ptr [rax+0000000000000000h]
  00000000670751E0: lea         ecx,[rax-30h]
  00000000670751E3: cmp         cl,9
  00000000670751E6: ja          0000000067075270
  00000000670751EC: mov         byte ptr [rsp+62h],al
  00000000670751F0: inc         rbx
  00000000670751F3: movzx       ecx,al
  00000000670751F6: mov         word ptr [rsp+64h],cx
  00000000670751FB: mov         rax,qword ptr [rsp+58h]
  0000000067075200: mov         ecx,dword ptr [rax]
  0000000067075202: mov         rax,qword ptr [rsp+58h]
  0000000067075207: cmp         ecx,1
  000000006707520A: jne         0000000067075216
  000000006707520C: mov         ecx,dword ptr [rax+8]
  000000006707520F: inc         ecx
  0000000067075211: cmp         ecx,dword ptr [rax+4]
  0000000067075214: jle         000000006707523C
  0000000067075216: mov         ecx,dword ptr [rax+8]
  0000000067075219: inc         ecx
  000000006707521B: add         ecx,ecx
  000000006707521D: mov         edx,20h
  0000000067075222: call        000000006702CAA0
  0000000067075227: movsxd      rdx,eax
  000000006707522A: shr         rdx,1
  000000006707522D: lea         rcx,[rsp+58h]
  0000000067075232: call        00000000670701A0
  0000000067075237: mov         rax,qword ptr [rsp+58h]
  000000006707523C: movsxd      rdx,dword ptr [rax+8]
  0000000067075240: mov         rcx,qword ptr [rax+10h]
  0000000067075244: movzx       eax,word ptr [rsp+64h]
  0000000067075249: mov         word ptr [rcx+rdx*2],ax
  000000006707524D: mov         rax,qword ptr [rsp+58h]
  0000000067075252: inc         dword ptr [rax+8]
  0000000067075255: mov         rax,qword ptr [rsp+58h]
  000000006707525A: movsxd      rdx,dword ptr [rax+8]
  000000006707525E: mov         rcx,qword ptr [rax+10h]
  0000000067075262: mov         word ptr [rcx+rdx*2],di
  0000000067075266: mov         al,byte ptr [rbx]
  0000000067075268: test        al,al
  000000006707526A: jne         00000000670751E0
  0000000067075270: lea         rcx,[rsp+00000000000000D0h]
  0000000067075278: call        000000006704D4F0
  000000006707527D: lea         rcx,[rsp+00000000000000D0h]
  0000000067075285: call        0000000067049340
  000000006707528A: mov         rcx,rax
  000000006707528D: mov         dword ptr [rsp+20h],edi
  0000000067075291: lea         r9,[rsp+0000000000000158h]
  0000000067075299: mov         r8d,0Ah
  000000006707529F: lea         rdx,[rsp+58h]
  00000000670752A4: call        000000006704C460
  00000000670752A9: mov         r13,rax
  00000000670752AC: cmp         byte ptr [rsp+0000000000000158h],0
  00000000670752B4: jne         00000000670752F2
  00000000670752B6: xor         r8d,r8d
  00000000670752B9: lea         edx,[r8+1]
  00000000670752BD: lea         rcx,[rsp+00000000000000E8h]
  00000000670752C5: call        000000006704D5A0
  00000000670752CA: lea         rcx,[rsp+00000000000000E8h]
  00000000670752D2: call        0000000067049340
  00000000670752D7: mov         rcx,rax
  00000000670752DA: mov         dword ptr [rsp+20h],edi
  00000000670752DE: xor         r9d,r9d
  00000000670752E1: lea         r8d,[r9+0Ah]
  00000000670752E5: lea         rdx,[rsp+58h]
  00000000670752EA: call        000000006704C460
  00000000670752EF: mov         r13,rax
  00000000670752F2: cmp         r13,0FFFFFFFF80000000h
  00000000670752F9: jl          0000000067075304
  00000000670752FB: cmp         r13,7FFFFFFFh
  0000000067075302: jle         0000000067075307
  0000000067075304: mov         r13,rdi
  0000000067075307: mov         rax,qword ptr [rsp+58h]
  000000006707530C: mov         ecx,ebp
  000000006707530E: lock xadd   dword ptr [rax],ecx
  0000000067075312: add         ecx,0FFFFFFFFh
  0000000067075315: jne         000000006707533B
  0000000067075317: mov         rcx,qword ptr [rsp+58h]
  000000006707531C: call        00000000670700D0
  0000000067075321: jmp         000000006707533B
  0000000067075323: cmp         cl,2Ah
  0000000067075326: jne         000000006707533B
  0000000067075328: add         r15,8
  000000006707532C: mov         eax,dword ptr [r15-8]
  0000000067075330: test        eax,eax
  0000000067075332: cmovs       eax,ebp
  0000000067075335: mov         r13d,eax
  0000000067075338: inc         rbx
  000000006707533B: movzx       eax,byte ptr [rbx]
  000000006707533E: test        al,al
  0000000067075340: je          0000000067075D3A
  0000000067075346: mov         r12d,ebp
  0000000067075349: cmp         al,2Eh
  000000006707534B: jne         00000000670754DC
  0000000067075351: inc         rbx
  0000000067075354: movzx       ecx,byte ptr [rbx]
  0000000067075357: lea         eax,[rcx-30h]
  000000006707535A: cmp         al,9
  000000006707535C: ja          00000000670754C4
  0000000067075362: lea         rax,[00000000672F0B30h]
  0000000067075369: mov         qword ptr [rsp+50h],rax
  000000006707536E: lock add    dword ptr [00000000672F0B30h],1
  0000000067075376: movzx       eax,byte ptr [rbx]
  0000000067075379: test        al,al
  000000006707537B: je          0000000067075411
  0000000067075381: lea         ecx,[rax-30h]
  0000000067075384: cmp         cl,9
  0000000067075387: ja          0000000067075411
  000000006707538D: mov         byte ptr [rsp+66h],al
  0000000067075391: inc         rbx
  0000000067075394: movzx       ecx,al
  0000000067075397: mov         word ptr [rsp+60h],cx
  000000006707539C: mov         rax,qword ptr [rsp+50h]
  00000000670753A1: mov         ecx,dword ptr [rax]
  00000000670753A3: mov         rax,qword ptr [rsp+50h]
  00000000670753A8: cmp         ecx,1
  00000000670753AB: jne         00000000670753B7
  00000000670753AD: mov         ecx,dword ptr [rax+8]
  00000000670753B0: inc         ecx
  00000000670753B2: cmp         ecx,dword ptr [rax+4]
  00000000670753B5: jle         00000000670753DD
  00000000670753B7: mov         ecx,dword ptr [rax+8]
  00000000670753BA: inc         ecx
  00000000670753BC: add         ecx,ecx
  00000000670753BE: mov         edx,20h
  00000000670753C3: call        000000006702CAA0
  00000000670753C8: movsxd      rdx,eax
  00000000670753CB: shr         rdx,1
  00000000670753CE: lea         rcx,[rsp+50h]
  00000000670753D3: call        00000000670701A0
  00000000670753D8: mov         rax,qword ptr [rsp+50h]
  00000000670753DD: movsxd      rdx,dword ptr [rax+8]
  00000000670753E1: mov         rcx,qword ptr [rax+10h]
  00000000670753E5: movzx       eax,word ptr [rsp+60h]
  00000000670753EA: mov         word ptr [rcx+rdx*2],ax
  00000000670753EE: mov         rax,qword ptr [rsp+50h]
  00000000670753F3: inc         dword ptr [rax+8]
  00000000670753F6: mov         rax,qword ptr [rsp+50h]
  00000000670753FB: movsxd      rdx,dword ptr [rax+8]
  00000000670753FF: mov         rcx,qword ptr [rax+10h]
  0000000067075403: mov         word ptr [rcx+rdx*2],di
  0000000067075407: mov         al,byte ptr [rbx]
  0000000067075409: test        al,al
  000000006707540B: jne         0000000067075381
  0000000067075411: lea         rcx,[rsp+00000000000000D8h]
  0000000067075419: call        000000006704D4F0
  000000006707541E: lea         rcx,[rsp+00000000000000D8h]
  0000000067075426: call        0000000067049340
  000000006707542B: mov         rcx,rax
  000000006707542E: mov         dword ptr [rsp+20h],edi
  0000000067075432: lea         r9,[rsp+0000000000000168h]
  000000006707543A: mov         r8d,0Ah
  0000000067075440: lea         rdx,[rsp+50h]
  0000000067075445: call        000000006704C460
  000000006707544A: mov         r12,rax
  000000006707544D: cmp         byte ptr [rsp+0000000000000168h],0
  0000000067075455: jne         0000000067075493
  0000000067075457: xor         r8d,r8d
  000000006707545A: lea         edx,[r8+1]
  000000006707545E: lea         rcx,[rsp+00000000000000E0h]
  0000000067075466: call        000000006704D5A0
  000000006707546B: lea         rcx,[rsp+00000000000000E0h]
  0000000067075473: call        0000000067049340
  0000000067075478: mov         rcx,rax
  000000006707547B: mov         dword ptr [rsp+20h],edi
  000000006707547F: xor         r9d,r9d
  0000000067075482: lea         r8d,[r9+0Ah]
  0000000067075486: lea         rdx,[rsp+50h]
  000000006707548B: call        000000006704C460
  0000000067075490: mov         r12,rax
  0000000067075493: cmp         r12,0FFFFFFFF80000000h
  000000006707549A: jl          00000000670754A5
  000000006707549C: cmp         r12,7FFFFFFFh
  00000000670754A3: jle         00000000670754A8
  00000000670754A5: mov         r12,rdi
  00000000670754A8: mov         rax,qword ptr [rsp+50h]
  00000000670754AD: mov         ecx,ebp
  00000000670754AF: lock xadd   dword ptr [rax],ecx
  00000000670754B3: add         ecx,0FFFFFFFFh
  00000000670754B6: jne         00000000670754DC
  00000000670754B8: mov         rcx,qword ptr [rsp+50h]
  00000000670754BD: call        00000000670700D0
  00000000670754C2: jmp         00000000670754DC
  00000000670754C4: cmp         cl,2Ah
  00000000670754C7: jne         00000000670754DC
  00000000670754C9: add         r15,8
  00000000670754CD: mov         eax,dword ptr [r15-8]
  00000000670754D1: test        eax,eax
  00000000670754D3: cmovs       eax,ebp
  00000000670754D6: mov         r12d,eax
  00000000670754D9: inc         rbx
  00000000670754DC: movzx       eax,byte ptr [rbx]
  00000000670754DF: test        al,al
  00000000670754E1: je          0000000067075CAA
  00000000670754E7: mov         edx,edi
  00000000670754E9: movsx       eax,al
  00000000670754EC: add         eax,0FFFFFFB4h
  00000000670754EF: cmp         eax,2Eh
  00000000670754F2: ja          000000006707556D
  00000000670754F4: cdqe
  00000000670754F6: lea         r8,[0000000067000000h]
  00000000670754FD: movzx       eax,byte ptr [r8+rax+0000000000075FE4h]
  0000000067075506: mov         ecx,dword ptr [r8+rax*4+0000000000075FC8h]
  000000006707550E: add         rcx,r8
  0000000067075511: jmp         rcx
  0000000067075513: inc         rbx
  0000000067075516: cmp         byte ptr [rbx],68h
  0000000067075519: jne         0000000067075525
  000000006707551B: mov         edx,1
  0000000067075520: inc         rbx
  0000000067075523: jmp         0000000067075574
  0000000067075525: mov         edx,2
  000000006707552A: jmp         0000000067075574
  000000006707552C: inc         rbx
  000000006707552F: cmp         byte ptr [rbx],6Ch
  0000000067075532: jne         000000006707553E
  0000000067075534: mov         edx,4
  0000000067075539: inc         rbx
  000000006707553C: jmp         0000000067075574
  000000006707553E: mov         edx,3
  0000000067075543: jmp         0000000067075574
  0000000067075545: inc         rbx
  0000000067075548: mov         edx,5
  000000006707554D: jmp         0000000067075574
  000000006707554F: inc         rbx
  0000000067075552: mov         edx,6
  0000000067075557: jmp         0000000067075574
  0000000067075559: inc         rbx
  000000006707555C: mov         edx,7
  0000000067075561: jmp         0000000067075574
  0000000067075563: inc         rbx
  0000000067075566: mov         edx,8
  000000006707556B: jmp         0000000067075574
  000000006707556D: lea         r8,[0000000067000000h]
  0000000067075574: cmp         byte ptr [rbx],0
  0000000067075577: je          0000000067075C1A
  000000006707557D: lea         r9,[00000000672F0B30h]
  0000000067075584: mov         qword ptr [rsp+48h],r9
  0000000067075589: lock add    dword ptr [00000000672F0B30h],1
  0000000067075591: movsx       edi,byte ptr [rbx]
  0000000067075594: lea         eax,[rdi-41h]
  0000000067075597: cmp         eax,37h
  000000006707559A: ja          0000000067075BB1
  00000000670755A0: cdqe
  00000000670755A2: movzx       eax,byte ptr [r8+rax+0000000000076034h]
  00000000670755AB: mov         ecx,dword ptr [r8+rax*4+0000000000076014h]
  00000000670755B3: add         rcx,r8
  00000000670755B6: jmp         rcx
  00000000670755B8: cmp         edx,8
  00000000670755BB: ja          000000006707561D
  00000000670755BD: movsxd      rax,edx
  00000000670755C0: mov         ecx,dword ptr [r8+rax*4+000000000007606Ch]
  00000000670755C8: add         rcx,r8
  00000000670755CB: jmp         rcx
  00000000670755CD: add         r15,8
  00000000670755D1: movsxd      rdi,dword ptr [r15-8]
  00000000670755D5: jmp         000000006707561F
  00000000670755D7: add         r15,8
  00000000670755DB: movsxd      rdi,dword ptr [r15-8]
  00000000670755DF: jmp         000000006707561F
  00000000670755E1: add         r15,8
  00000000670755E5: movsxd      rdi,dword ptr [r15-8]
  00000000670755E9: jmp         000000006707561F
  00000000670755EB: add         r15,8
  00000000670755EF: movsxd      rdi,dword ptr [r15-8]
  00000000670755F3: jmp         000000006707561F
  00000000670755F5: add         r15,8
  00000000670755F9: mov         rdi,qword ptr [r15-8]
  00000000670755FD: jmp         000000006707561F
  00000000670755FF: add         r15,8
  0000000067075603: movsxd      rdi,dword ptr [r15-8]
  0000000067075607: jmp         000000006707561F
  0000000067075609: add         r15,8
  000000006707560D: mov         rdi,qword ptr [r15-8]
  0000000067075611: jmp         000000006707561F
  0000000067075613: add         r15,8
  0000000067075617: movsxd      rdi,dword ptr [r15-8]
  000000006707561B: jmp         000000006707561F
  000000006707561D: xor         edi,edi
  000000006707561F: lea         rcx,[rsp+68h]
  0000000067075624: call        0000000067049340
  0000000067075629: mov         rcx,rax
  000000006707562C: mov         dword ptr [rsp+30h],r14d
  0000000067075631: mov         dword ptr [rsp+28h],r13d
  0000000067075636: mov         dword ptr [rsp+20h],0Ah
  000000006707563E: mov         r9d,r12d
  0000000067075641: mov         r8,rdi
  0000000067075644: lea         rdx,[rsp+0000000000000098h]
  000000006707564C: call        000000006704EEF0
  0000000067075651: mov         rdi,rax
  0000000067075654: mov         rcx,qword ptr [rax]
  0000000067075657: lock add    dword ptr [rcx],1
  000000006707565B: mov         ecx,ebp
  000000006707565D: lock xadd   dword ptr [00000000672F0B30h],ecx
  0000000067075665: add         ecx,0FFFFFFFFh
  0000000067075668: jne         0000000067075676
  000000006707566A: lea         rcx,[00000000672F0B30h]
  0000000067075671: call        00000000670700D0
  0000000067075676: mov         rax,qword ptr [rdi]
  0000000067075679: mov         qword ptr [rsp+48h],rax
  000000006707567E: mov         rax,qword ptr [rsp+0000000000000098h]
  0000000067075686: mov         ecx,ebp
  0000000067075688: lock xadd   dword ptr [rax],ecx
  000000006707568C: add         ecx,0FFFFFFFFh
  000000006707568F: jne         000000006707569E
  0000000067075691: mov         rcx,qword ptr [rsp+0000000000000098h]
  0000000067075699: call        00000000670700D0
  000000006707569E: inc         rbx
  00000000670756A1: jmp         0000000067075ABF
  00000000670756A6: cmp         edx,7
  00000000670756A9: ja          00000000670756F7
  00000000670756AB: movsxd      rax,edx
  00000000670756AE: mov         ecx,dword ptr [r8+rax*4+0000000000076090h]
  00000000670756B6: add         rcx,r8
  00000000670756B9: jmp         rcx
  00000000670756BB: add         r15,8
  00000000670756BF: mov         esi,dword ptr [r15-8]
  00000000670756C3: jmp         00000000670756F9
  00000000670756C5: add         r15,8
  00000000670756C9: mov         esi,dword ptr [r15-8]
  00000000670756CD: jmp         00000000670756F9
  00000000670756CF: add         r15,8
  00000000670756D3: mov         esi,dword ptr [r15-8]
  00000000670756D7: jmp         00000000670756F9
  00000000670756D9: add         r15,8
  00000000670756DD: mov         esi,dword ptr [r15-8]
  00000000670756E1: jmp         00000000670756F9
  00000000670756E3: add         r15,8
  00000000670756E7: mov         rsi,qword ptr [r15-8]
  00000000670756EB: jmp         00000000670756F9
  00000000670756ED: add         r15,8
  00000000670756F1: mov         rsi,qword ptr [r15-8]
  00000000670756F5: jmp         00000000670756F9
  00000000670756F7: xor         esi,esi
  00000000670756F9: lea         eax,[rdi-41h]
  00000000670756FC: cmp         al,19h
  00000000670756FE: ja          0000000067075704
  0000000067075700: or          r14d,40h
  0000000067075704: mov         ebp,0Ah
  0000000067075709: lea         eax,[rdi-41h]
  000000006707570C: cmp         al,19h
  000000006707570E: ja          0000000067075714
  0000000067075710: add         dil,20h
  0000000067075714: cmp         dil,6Fh
  0000000067075718: je          000000006707572D
  000000006707571A: cmp         dil,75h
  000000006707571E: je          0000000067075732
  0000000067075720: cmp         dil,78h
  0000000067075724: jne         0000000067075732
  0000000067075726: mov         ebp,10h
  000000006707572B: jmp         0000000067075732
  000000006707572D: mov         ebp,8
  0000000067075732: lea         rcx,[rsp+68h]
  0000000067075737: call        0000000067049340
  000000006707573C: mov         rcx,rax
  000000006707573F: mov         dword ptr [rsp+30h],r14d
  0000000067075744: mov         dword ptr [rsp+28h],r13d
  0000000067075749: mov         dword ptr [rsp+20h],ebp
  000000006707574D: mov         r9d,r12d
  0000000067075750: mov         r8,rsi
  0000000067075753: lea         rdx,[rsp+00000000000000B8h]
  000000006707575B: call        000000006704EF60
  0000000067075760: mov         rdi,rax
  0000000067075763: mov         rcx,qword ptr [rax]
  0000000067075766: lock add    dword ptr [rcx],1
  000000006707576A: or          rbp,0FFFFFFFFFFFFFFFFh
  000000006707576E: mov         ecx,ebp
  0000000067075770: lock xadd   dword ptr [00000000672F0B30h],ecx
  0000000067075778: add         ecx,ebp
  000000006707577A: jne         0000000067075788
  000000006707577C: lea         rcx,[00000000672F0B30h]
  0000000067075783: call        00000000670700D0
  0000000067075788: mov         rax,qword ptr [rdi]
  000000006707578B: mov         qword ptr [rsp+48h],rax
  0000000067075790: mov         rax,qword ptr [rsp+00000000000000B8h]
  0000000067075798: mov         ecx,ebp
  000000006707579A: lock xadd   dword ptr [rax],ecx
  000000006707579E: add         ecx,0FFFFFFFFh
  00000000670757A1: jne         00000000670757B0
  00000000670757A3: mov         rcx,qword ptr [rsp+00000000000000B8h]
  00000000670757AB: call        00000000670700D0
  00000000670757B0: inc         rbx
  00000000670757B3: jmp         0000000067075ABF
  00000000670757B8: cmp         edx,5
  00000000670757BB: jne         00000000670757C9
  00000000670757BD: add         r15,8
  00000000670757C1: movsd       xmm6,mmword ptr [r15-8]
  00000000670757C7: jmp         00000000670757D3
  00000000670757C9: add         r15,8
  00000000670757CD: movsd       xmm6,mmword ptr [r15-8]
  00000000670757D3: lea         eax,[rdi-41h]
  00000000670757D6: cmp         al,19h
  00000000670757D8: ja          00000000670757DE
  00000000670757DA: or          r14d,40h
  00000000670757DE: mov         esi,1
  00000000670757E3: lea         eax,[rdi-41h]
  00000000670757E6: cmp         al,19h
  00000000670757E8: ja          00000000670757EE
  00000000670757EA: add         dil,20h
  00000000670757EE: movsx       ecx,dil
  00000000670757F2: sub         ecx,61h
  00000000670757F5: je          000000006707580D
  00000000670757F7: sub         ecx,4
  00000000670757FA: je          000000006707580B
  00000000670757FC: sub         ecx,esi
  00000000670757FE: je          000000006707580D
  0000000067075800: cmp         ecx,esi
  0000000067075802: jne         000000006707580D
  0000000067075804: mov         esi,2
  0000000067075809: jmp         000000006707580D
  000000006707580B: xor         esi,esi
  000000006707580D: lea         rcx,[rsp+68h]
  0000000067075812: call        0000000067049340
  0000000067075817: mov         rcx,rax
  000000006707581A: mov         dword ptr [rsp+30h],r14d
  000000006707581F: mov         dword ptr [rsp+28h],r13d
  0000000067075824: mov         dword ptr [rsp+20h],esi
  0000000067075828: mov         r9d,r12d
  000000006707582B: movapd      xmm2,xmm6
  000000006707582F: lea         rdx,[rsp+0000000000000080h]
  0000000067075837: call        000000006704EE70
  000000006707583C: mov         rdi,rax
  000000006707583F: mov         rcx,qword ptr [rax]
  0000000067075842: lock add    dword ptr [rcx],1
  0000000067075846: mov         ecx,ebp
  0000000067075848: lock xadd   dword ptr [00000000672F0B30h],ecx
  0000000067075850: add         ecx,0FFFFFFFFh
  0000000067075853: jne         0000000067075861
  0000000067075855: lea         rcx,[00000000672F0B30h]
  000000006707585C: call        00000000670700D0
  0000000067075861: mov         rax,qword ptr [rdi]
  0000000067075864: mov         qword ptr [rsp+48h],rax
  0000000067075869: mov         rax,qword ptr [rsp+0000000000000080h]
  0000000067075871: mov         ecx,ebp
  0000000067075873: lock xadd   dword ptr [rax],ecx
  0000000067075877: add         ecx,0FFFFFFFFh
  000000006707587A: jne         0000000067075889
  000000006707587C: mov         rcx,qword ptr [rsp+0000000000000080h]
  0000000067075884: call        00000000670700D0
  0000000067075889: inc         rbx
  000000006707588C: jmp         0000000067075ABF
  0000000067075891: cmp         edx,3
  0000000067075894: jne         00000000670758AB
  0000000067075896: add         r15,8
  000000006707589A: movzx       edx,word ptr [r15-8]
  000000006707589F: lea         rcx,[rsp+48h]
  00000000670758A4: call        0000000067072B10
  00000000670758A9: jmp         00000000670758C3
  00000000670758AB: add         r15,8
  00000000670758AF: movzx       edx,byte ptr [r15-8]
  00000000670758B4: mov         word ptr [rsp+70h],dx
  00000000670758B9: lea         rcx,[rsp+48h]
  00000000670758BE: call        0000000067072B10
  00000000670758C3: inc         rbx
  00000000670758C6: jmp         0000000067075ABF
  00000000670758CB: cmp         edx,3
  00000000670758CE: jne         000000006707591E
  00000000670758D0: add         r15,8
  00000000670758D4: mov         rsi,qword ptr [r15-8]
  00000000670758D8: mov         rdi,rsi
  00000000670758DB: cmp         word ptr [rsi],0
  00000000670758DF: je          00000000670758EB
  00000000670758E1: add         rdi,2
  00000000670758E5: cmp         word ptr [rdi],0
  00000000670758E9: jne         00000000670758E1
  00000000670758EB: sub         rdi,rsi
  00000000670758EE: sar         rdi,1
  00000000670758F1: mov         edx,edi
  00000000670758F3: lea         rcx,[rsp+48h]
  00000000670758F8: call        00000000670729C0
  00000000670758FD: test        edi,edi
  00000000670758FF: je          0000000067075983
  0000000067075905: movsxd      r8,edi
  0000000067075908: add         r8,r8
  000000006707590B: mov         rdx,rsi
  000000006707590E: mov         rcx,qword ptr [rsp+48h]
  0000000067075913: mov         rcx,qword ptr [rcx+10h]
  0000000067075917: call        0000000067178A9E
  000000006707591C: jmp         0000000067075983
  000000006707591E: add         r15,8
  0000000067075922: mov         r8d,ebp
  0000000067075925: mov         rdx,qword ptr [r15-8]
  0000000067075929: lea         rcx,[rsp+00000000000000C8h]
  0000000067075931: call        0000000067074350
  0000000067075936: mov         rdi,rax
  0000000067075939: mov         rcx,qword ptr [rax]
  000000006707593C: lock add    dword ptr [rcx],1
  0000000067075940: mov         ecx,ebp
  0000000067075942: lock xadd   dword ptr [00000000672F0B30h],ecx
  000000006707594A: add         ecx,0FFFFFFFFh
  000000006707594D: jne         000000006707595B
  000000006707594F: lea         rcx,[00000000672F0B30h]
  0000000067075956: call        00000000670700D0
  000000006707595B: mov         rax,qword ptr [rdi]
  000000006707595E: mov         qword ptr [rsp+48h],rax
  0000000067075963: mov         rax,qword ptr [rsp+00000000000000C8h]
  000000006707596B: mov         ecx,ebp
  000000006707596D: lock xadd   dword ptr [rax],ecx
  0000000067075971: add         ecx,0FFFFFFFFh
  0000000067075974: jne         0000000067075983
  0000000067075976: mov         rcx,qword ptr [rsp+00000000000000C8h]
  000000006707597E: call        00000000670700D0
  0000000067075983: cmp         r12d,0FFFFFFFFh
  0000000067075987: je          00000000670759A1
  0000000067075989: mov         rax,qword ptr [rsp+48h]
  000000006707598E: cmp         r12d,dword ptr [rax+8]
  0000000067075992: jge         00000000670759A1
  0000000067075994: mov         edx,r12d
  0000000067075997: lea         rcx,[rsp+48h]
  000000006707599C: call        00000000670729C0
  00000000670759A1: inc         rbx
  00000000670759A4: jmp         0000000067075ABF
  00000000670759A9: add         r15,8
  00000000670759AD: mov         rdi,qword ptr [r15-8]
  00000000670759B1: or          r14d,1
  00000000670759B5: lea         rcx,[rsp+68h]
  00000000670759BA: call        0000000067049340
  00000000670759BF: mov         rcx,rax
  00000000670759C2: mov         dword ptr [rsp+30h],r14d
  00000000670759C7: mov         dword ptr [rsp+28h],r13d
  00000000670759CC: mov         dword ptr [rsp+20h],10h
  00000000670759D4: mov         r9d,r12d
  00000000670759D7: mov         r8,rdi
  00000000670759DA: lea         rdx,[rsp+00000000000000C0h]
  00000000670759E2: call        000000006704EF60
  00000000670759E7: mov         rdi,rax
  00000000670759EA: mov         rcx,qword ptr [rax]
  00000000670759ED: lock add    dword ptr [rcx],1
  00000000670759F1: mov         ecx,ebp
  00000000670759F3: lock xadd   dword ptr [00000000672F0B30h],ecx
  00000000670759FB: add         ecx,0FFFFFFFFh
  00000000670759FE: jne         0000000067075A0C
  0000000067075A00: lea         rcx,[00000000672F0B30h]
  0000000067075A07: call        00000000670700D0
  0000000067075A0C: mov         rax,qword ptr [rdi]
  0000000067075A0F: mov         qword ptr [rsp+48h],rax
  0000000067075A14: mov         rax,qword ptr [rsp+00000000000000C0h]
  0000000067075A1C: mov         ecx,ebp
  0000000067075A1E: lock xadd   dword ptr [rax],ecx
  0000000067075A22: add         ecx,0FFFFFFFFh
  0000000067075A25: jne         0000000067075A34
  0000000067075A27: mov         rcx,qword ptr [rsp+00000000000000C0h]
  0000000067075A2F: call        00000000670700D0
  0000000067075A34: inc         rbx
  0000000067075A37: jmp         0000000067075ABF
  0000000067075A3C: sub         edx,1
  0000000067075A3F: je          0000000067075AAA
  0000000067075A41: sub         edx,1
  0000000067075A44: je          0000000067075A95
  0000000067075A46: sub         edx,1
  0000000067075A49: je          0000000067075A81
  0000000067075A4B: cmp         edx,1
  0000000067075A4E: je          0000000067075A64
  0000000067075A50: add         r15,8
  0000000067075A54: mov         rdx,qword ptr [r15-8]
  0000000067075A58: mov         rax,qword ptr [rsp+40h]
  0000000067075A5D: mov         ecx,dword ptr [rax+8]
  0000000067075A60: mov         dword ptr [rdx],ecx
  0000000067075A62: jmp         0000000067075ABC
  0000000067075A64: add         r15,8
  0000000067075A68: mov         rdx,qword ptr [r15-8]
  0000000067075A6C: mov         rax,qword ptr [rsp+40h]
  0000000067075A71: mov         ecx,dword ptr [rax+8]
  0000000067075A74: mov         dword ptr [rsp+78h],ecx
  0000000067075A78: mov         eax,dword ptr [rsp+78h]
  0000000067075A7C: mov         qword ptr [rdx],rax
  0000000067075A7F: jmp         0000000067075ABC
  0000000067075A81: add         r15,8
  0000000067075A85: mov         rdx,qword ptr [r15-8]
  0000000067075A89: mov         rax,qword ptr [rsp+40h]
  0000000067075A8E: mov         ecx,dword ptr [rax+8]
  0000000067075A91: mov         dword ptr [rdx],ecx
  0000000067075A93: jmp         0000000067075ABC
  0000000067075A95: add         r15,8
  0000000067075A99: mov         rdx,qword ptr [r15-8]
  0000000067075A9D: mov         rax,qword ptr [rsp+40h]
  0000000067075AA2: mov         ecx,dword ptr [rax+8]
  0000000067075AA5: mov         word ptr [rdx],cx
  0000000067075AA8: jmp         0000000067075ABC
  0000000067075AAA: add         r15,8
  0000000067075AAE: mov         rdx,qword ptr [r15-8]
  0000000067075AB2: mov         rax,qword ptr [rsp+40h]
  0000000067075AB7: mov         ecx,dword ptr [rax+8]
  0000000067075ABA: mov         byte ptr [rdx],cl
  0000000067075ABC: inc         rbx
  0000000067075ABF: test        r14b,4
  0000000067075AC3: je          0000000067075B20
  0000000067075AC5: mov         r14d,20h
  0000000067075ACB: mov         word ptr [rsp+72h],r14w
  0000000067075AD1: mov         byte ptr [rsp+20h],0
  0000000067075AD6: movzx       r9d,r14w
  0000000067075ADA: mov         r8d,r13d
  0000000067075ADD: lea         rdx,[rsp+0000000000000088h]
  0000000067075AE5: lea         rcx,[rsp+48h]
  0000000067075AEA: call        0000000067074780
  0000000067075AEF: nop
  0000000067075AF0: mov         rdx,rax
  0000000067075AF3: lea         rcx,[rsp+40h]
  0000000067075AF8: call        0000000067070410
  0000000067075AFD: nop
  0000000067075AFE: mov         rax,qword ptr [rsp+0000000000000088h]
  0000000067075B06: mov         ecx,ebp
  0000000067075B08: lock xadd   dword ptr [rax],ecx
  0000000067075B0C: add         ecx,0FFFFFFFFh
  0000000067075B0F: jne         0000000067075B7A
  0000000067075B11: mov         rcx,qword ptr [rsp+0000000000000088h]
  0000000067075B19: call        00000000670700D0
  0000000067075B1E: jmp         0000000067075B7A
  0000000067075B20: mov         r14d,20h
  0000000067075B26: mov         word ptr [rsp+76h],r14w
  0000000067075B2C: mov         byte ptr [rsp+20h],0
  0000000067075B31: movzx       r9d,r14w
  0000000067075B35: mov         r8d,r13d
  0000000067075B38: lea         rdx,[rsp+0000000000000090h]
  0000000067075B40: lea         rcx,[rsp+48h]
  0000000067075B45: call        00000000670748B0
  0000000067075B4A: nop
  0000000067075B4B: mov         rdx,rax
  0000000067075B4E: lea         rcx,[rsp+40h]
  0000000067075B53: call        0000000067070410
  0000000067075B58: nop
  0000000067075B59: mov         rax,qword ptr [rsp+0000000000000090h]
  0000000067075B61: mov         ecx,ebp
  0000000067075B63: lock xadd   dword ptr [rax],ecx
  0000000067075B67: add         ecx,0FFFFFFFFh
  0000000067075B6A: jne         0000000067075B7A
  0000000067075B6C: mov         rcx,qword ptr [rsp+0000000000000090h]
  0000000067075B74: call        00000000670700D0
  0000000067075B79: nop
  0000000067075B7A: mov         rax,qword ptr [rsp+48h]
  0000000067075B7F: mov         ecx,ebp
  0000000067075B81: lock xadd   dword ptr [rax],ecx
  0000000067075B85: add         ecx,0FFFFFFFFh
  0000000067075B88: mov         r12d,0
  0000000067075B8E: lea         r13,[0000000067000000h]
  0000000067075B95: jne         0000000067075010
  0000000067075B9B: mov         rcx,qword ptr [rsp+48h]
  0000000067075BA0: call        00000000670700D0
  0000000067075BA5: lea         r13,[0000000067000000h]
  0000000067075BAC: jmp         0000000067075010
  0000000067075BB1: cmp         rsi,rbx
  0000000067075BB4: je          0000000067075BE1
  0000000067075BB6: nop         word ptr [rax+rax+0000000000000000h]
  0000000067075BC0: movzx       edx,byte ptr [rsi]
  0000000067075BC3: mov         word ptr [rsp+7Ch],dx
  0000000067075BC8: lea         rcx,[rsp+40h]
  0000000067075BCD: call        00000000670705B0
  0000000067075BD2: inc         rsi
  0000000067075BD5: cmp         rsi,rbx
  0000000067075BD8: jne         0000000067075BC0
  0000000067075BDA: lea         r9,[00000000672F0B30h]
  0000000067075BE1: mov         eax,ebp
  0000000067075BE3: lock xadd   dword ptr [00000000672F0B30h],eax
  0000000067075BEB: add         eax,0FFFFFFFFh
  0000000067075BEE: mov         r12d,0
  0000000067075BF4: lea         r13,[0000000067000000h]
  0000000067075BFB: lea         r14d,[r12+20h]
  0000000067075C00: jne         0000000067075010
  0000000067075C06: mov         rcx,r9
  0000000067075C09: call        00000000670700D0
  0000000067075C0E: lea         r13,[0000000067000000h]
  0000000067075C15: jmp         0000000067075010
  0000000067075C1A: test        rsi,rsi
  0000000067075C1D: je          0000000067075EB9
  0000000067075C23: xor         eax,eax
  0000000067075C25: mov         rcx,rbp
  0000000067075C28: mov         rdi,rsi
  0000000067075C2B: repne scas  byte ptr [rdi]
  0000000067075C2D: not         rcx
  0000000067075C30: lea         rdi,[rcx-1]
  0000000067075C34: mov         rax,qword ptr [rsp+40h]
  0000000067075C39: mov         ecx,dword ptr [rax]
  0000000067075C3B: mov         rax,qword ptr [rsp+40h]
  0000000067075C40: cmp         ecx,1
  0000000067075C43: jne         0000000067075C4F
  0000000067075C45: mov         ecx,dword ptr [rax+8]
  0000000067075C48: add         ecx,edi
  0000000067075C4A: cmp         ecx,dword ptr [rax+4]
  0000000067075C4D: jle         0000000067075C76
  0000000067075C4F: mov         eax,dword ptr [rax+8]
  0000000067075C52: lea         ecx,[rdi+rax]
  0000000067075C55: add         ecx,ecx
  0000000067075C57: mov         edx,20h
  0000000067075C5C: call        000000006702CAA0
  0000000067075C61: movsxd      rdx,eax
  0000000067075C64: shr         rdx,1
  0000000067075C67: lea         rcx,[rsp+40h]
  0000000067075C6C: call        00000000670701A0
  0000000067075C71: mov         rax,qword ptr [rsp+40h]
  0000000067075C76: movsxd      rdx,dword ptr [rax+8]
  0000000067075C7A: mov         rcx,qword ptr [rax+10h]
  0000000067075C7E: lea         rdx,[rcx+rdx*2]
  0000000067075C82: nop         word ptr [rax+rax+0000000000000000h]
  0000000067075C90: movzx       ecx,byte ptr [rsi]
  0000000067075C93: mov         word ptr [rdx],cx
  0000000067075C96: add         rdx,2
  0000000067075C9A: inc         rsi
  0000000067075C9D: test        cx,cx
  0000000067075CA0: jne         0000000067075C90
  0000000067075CA2: add         dword ptr [rax+8],edi
  0000000067075CA5: jmp         0000000067075EBE
  0000000067075CAA: test        rsi,rsi
  0000000067075CAD: je          0000000067075EB9
  0000000067075CB3: xor         eax,eax
  0000000067075CB5: mov         rcx,rbp
  0000000067075CB8: mov         rdi,rsi
  0000000067075CBB: repne scas  byte ptr [rdi]
  0000000067075CBD: not         rcx
  0000000067075CC0: dec         rcx
  0000000067075CC3: mov         rdi,rcx
  0000000067075CC6: mov         rax,qword ptr [rsp+40h]
  0000000067075CCB: mov         edx,dword ptr [rax]
  0000000067075CCD: mov         rax,qword ptr [rsp+40h]
  0000000067075CD2: cmp         edx,1
  0000000067075CD5: jne         0000000067075CE1
  0000000067075CD7: mov         edx,dword ptr [rax+8]
  0000000067075CDA: add         edx,ecx
  0000000067075CDC: cmp         edx,dword ptr [rax+4]
  0000000067075CDF: jle         0000000067075D05
  0000000067075CE1: add         ecx,dword ptr [rax+8]
  0000000067075CE4: add         ecx,ecx
  0000000067075CE6: mov         edx,20h
  0000000067075CEB: call        000000006702CAA0
  0000000067075CF0: movsxd      rdx,eax
  0000000067075CF3: shr         rdx,1
  0000000067075CF6: lea         rcx,[rsp+40h]
  0000000067075CFB: call        00000000670701A0
  0000000067075D00: mov         rax,qword ptr [rsp+40h]
  0000000067075D05: movsxd      rdx,dword ptr [rax+8]
  0000000067075D09: mov         rcx,qword ptr [rax+10h]
  0000000067075D0D: lea         rdx,[rcx+rdx*2]
  0000000067075D11: nop         word ptr [rax+rax+0000000000000000h]
  0000000067075D20: movzx       ecx,byte ptr [rsi]
  0000000067075D23: mov         word ptr [rdx],cx
  0000000067075D26: add         rdx,2
  0000000067075D2A: inc         rsi
  0000000067075D2D: test        cx,cx
  0000000067075D30: jne         0000000067075D20
  0000000067075D32: add         dword ptr [rax+8],edi
  0000000067075D35: jmp         0000000067075EBE
  0000000067075D3A: test        rsi,rsi
  0000000067075D3D: je          0000000067075EB9
  0000000067075D43: xor         eax,eax
  0000000067075D45: mov         rcx,rbp
  0000000067075D48: mov         rdi,rsi
  0000000067075D4B: repne scas  byte ptr [rdi]
  0000000067075D4D: not         rcx
  0000000067075D50: dec         rcx
  0000000067075D53: mov         rdi,rcx
  0000000067075D56: mov         rax,qword ptr [rsp+40h]
  0000000067075D5B: mov         edx,dword ptr [rax]
  0000000067075D5D: mov         rax,qword ptr [rsp+40h]
  0000000067075D62: cmp         edx,1
  0000000067075D65: jne         0000000067075D71
  0000000067075D67: mov         edx,dword ptr [rax+8]
  0000000067075D6A: add         edx,ecx
  0000000067075D6C: cmp         edx,dword ptr [rax+4]
  0000000067075D6F: jle         0000000067075D95
  0000000067075D71: add         ecx,dword ptr [rax+8]
  0000000067075D74: add         ecx,ecx
  0000000067075D76: mov         edx,20h
  0000000067075D7B: call        000000006702CAA0
  0000000067075D80: movsxd      rdx,eax
  0000000067075D83: shr         rdx,1
  0000000067075D86: lea         rcx,[rsp+40h]
  0000000067075D8B: call        00000000670701A0
  0000000067075D90: mov         rax,qword ptr [rsp+40h]
  0000000067075D95: movsxd      rdx,dword ptr [rax+8]
  0000000067075D99: mov         rcx,qword ptr [rax+10h]
  0000000067075D9D: lea         rdx,[rcx+rdx*2]
  0000000067075DA1: nop         word ptr [rax+rax+0000000000000000h]
  0000000067075DB0: movzx       ecx,byte ptr [rsi]
  0000000067075DB3: mov         word ptr [rdx],cx
  0000000067075DB6: add         rdx,2
  0000000067075DBA: inc         rsi
  0000000067075DBD: test        cx,cx
  0000000067075DC0: jne         0000000067075DB0
  0000000067075DC2: add         dword ptr [rax+8],edi
  0000000067075DC5: jmp         0000000067075EBE
  0000000067075DCA: test        rsi,rsi
  0000000067075DCD: je          0000000067075EB9
  0000000067075DD3: xor         eax,eax
  0000000067075DD5: mov         rcx,rbp
  0000000067075DD8: mov         rdi,rsi
  0000000067075DDB: repne scas  byte ptr [rdi]
  0000000067075DDD: not         rcx
  0000000067075DE0: dec         rcx
  0000000067075DE3: mov         rdi,rcx
  0000000067075DE6: mov         rax,qword ptr [rsp+40h]
  0000000067075DEB: mov         edx,dword ptr [rax]
  0000000067075DED: mov         rax,qword ptr [rsp+40h]
  0000000067075DF2: cmp         edx,1
  0000000067075DF5: jne         0000000067075E01
  0000000067075DF7: mov         edx,dword ptr [rax+8]
  0000000067075DFA: add         edx,ecx
  0000000067075DFC: cmp         edx,dword ptr [rax+4]
  0000000067075DFF: jle         0000000067075E25
  0000000067075E01: add         ecx,dword ptr [rax+8]
  0000000067075E04: add         ecx,ecx
  0000000067075E06: mov         edx,20h
  0000000067075E0B: call        000000006702CAA0
  0000000067075E10: movsxd      rdx,eax
  0000000067075E13: shr         rdx,1
  0000000067075E16: lea         rcx,[rsp+40h]
  0000000067075E1B: call        00000000670701A0
  0000000067075E20: mov         rax,qword ptr [rsp+40h]
  0000000067075E25: movsxd      rdx,dword ptr [rax+8]
  0000000067075E29: mov         rcx,qword ptr [rax+10h]
  0000000067075E2D: lea         rdx,[rcx+rdx*2]
  0000000067075E31: nop         word ptr [rax+rax+0000000000000000h]
  0000000067075E40: movzx       ecx,byte ptr [rsi]
  0000000067075E43: mov         word ptr [rdx],cx
  0000000067075E46: add         rdx,2
  0000000067075E4A: inc         rsi
  0000000067075E4D: test        cx,cx
  0000000067075E50: jne         0000000067075E40
  0000000067075E52: add         dword ptr [rax+8],edi
  0000000067075E55: jmp         0000000067075EBE
  0000000067075E57: mov         rax,qword ptr [rsp+40h]
  0000000067075E5C: mov         ecx,dword ptr [rax]
  0000000067075E5E: mov         rax,qword ptr [rsp+40h]
  0000000067075E63: cmp         ecx,1
  0000000067075E66: jne         0000000067075E72
  0000000067075E68: mov         ecx,dword ptr [rax+8]
  0000000067075E6B: inc         ecx
  0000000067075E6D: cmp         ecx,dword ptr [rax+4]
  0000000067075E70: jle         0000000067075E96
  0000000067075E72: mov         ecx,dword ptr [rax+8]
  0000000067075E75: inc         ecx
  0000000067075E77: add         ecx,ecx
  0000000067075E79: mov         edx,r14d
  0000000067075E7C: call        000000006702CAA0
  0000000067075E81: movsxd      rdx,eax
  0000000067075E84: shr         rdx,1
  0000000067075E87: lea         rcx,[rsp+40h]
  0000000067075E8C: call        00000000670701A0
  0000000067075E91: mov         rax,qword ptr [rsp+40h]
  0000000067075E96: movsxd      rdx,dword ptr [rax+8]
  0000000067075E9A: mov         rcx,qword ptr [rax+10h]
  0000000067075E9E: mov         edi,25h
  0000000067075EA3: mov         word ptr [rcx+rdx*2],di
  0000000067075EA7: inc         dword ptr [rax+8]
  0000000067075EAA: movsxd      rdx,dword ptr [rax+8]
  0000000067075EAE: mov         rcx,qword ptr [rax+10h]
  0000000067075EB2: mov         word ptr [rcx+rdx*2],r12w
  0000000067075EB7: jmp         0000000067075EBE
  0000000067075EB9: mov         rax,qword ptr [rsp+40h]
  0000000067075EBE: lock add    dword ptr [rax],1
  0000000067075EC2: mov         rdi,qword ptr [rsp+0000000000000150h]
  0000000067075ECA: mov         rcx,qword ptr [rdi]
  0000000067075ECD: mov         eax,ebp
  0000000067075ECF: lock xadd   dword ptr [rcx],eax
  0000000067075ED3: add         eax,0FFFFFFFFh
  0000000067075ED6: jne         0000000067075EE0
  0000000067075ED8: mov         rcx,qword ptr [rdi]
  0000000067075EDB: call        00000000670700D0
  0000000067075EE0: mov         rax,qword ptr [rsp+40h]
  0000000067075EE5: mov         qword ptr [rdi],rax
  0000000067075EE8: lock xadd   dword ptr [rax],ebp
  0000000067075EEC: add         ebp,0FFFFFFFFh
  0000000067075EEF: jne         0000000067075EFB
  0000000067075EF1: mov         rcx,qword ptr [rsp+40h]
  0000000067075EF6: call        00000000670700D0
  0000000067075EFB: mov         rax,rdi
  0000000067075EFE: jmp         0000000067075F61
  0000000067075F00: or          rbx,0FFFFFFFFFFFFFFFFh
  0000000067075F04: mov         r8d,ebx
  0000000067075F07: lea         rdx,[00000000671F3C40h]
  0000000067075F0E: lea         rcx,[rsp+00000000000000A0h]
  0000000067075F16: call        00000000670741E0
  0000000067075F1B: mov         rdi,rax
  0000000067075F1E: mov         rcx,qword ptr [rax]
  0000000067075F21: lock add    dword ptr [rcx],1
  0000000067075F25: mov         rdx,qword ptr [rsi]
  0000000067075F28: mov         ecx,ebx
  0000000067075F2A: lock xadd   dword ptr [rdx],ecx
  0000000067075F2E: add         ecx,ebx
  0000000067075F30: jne         0000000067075F3A
  0000000067075F32: mov         rcx,qword ptr [rsi]
  0000000067075F35: call        00000000670700D0
  0000000067075F3A: mov         rax,qword ptr [rdi]
  0000000067075F3D: mov         qword ptr [rsi],rax
  0000000067075F40: mov         rax,qword ptr [rsp+00000000000000A0h]
  0000000067075F48: lock xadd   dword ptr [rax],ebx
  0000000067075F4C: add         ebx,0FFFFFFFFh
  0000000067075F4F: jne         0000000067075F5E
  0000000067075F51: mov         rcx,qword ptr [rsp+00000000000000A0h]
  0000000067075F59: call        00000000670700D0
  0000000067075F5E: mov         rax,rsi
  0000000067075F61: mov         rbx,qword ptr [rsp+0000000000000160h]
  0000000067075F69: movaps      xmm6,xmmword ptr [rsp+0000000000000100h]
  0000000067075F71: add         rsp,110h
  0000000067075F78: pop         r15
  0000000067075F7A: pop         r14
  0000000067075F7C: pop         r13
  0000000067075F7E: pop         r12
  0000000067075F80: pop         rdi
  0000000067075F81: pop         rsi
  0000000067075F82: pop         rbp
  0000000067075F83: ret
  0000000067075F84: mov         word ptr [rcx+7],ss
  0000000067075F87: add         byte ptr [rcx+FFFFFFFFA1000751h],ah
  0000000067075F8D: push        rcx
  0000000067075F8E: 07
  0000000067075F8F: add         byte ptr [rdx+51h],bh
  0000000067075F92: 07
  0000000067075F93: add         byte ptr [rcx+FFFFFFFFA1000751h],ah
  0000000067075F99: push        rcx
  0000000067075F9A: 07
  0000000067075F9B: add         byte ptr [rcx+FFFFFFFF98000751h],ah
  0000000067075FA1: push        rcx
  0000000067075FA2: 07
  0000000067075FA3: add         byte ptr [rcx+FFFFFFFFA1000751h],ah
  0000000067075FA9: push        rcx
  0000000067075FAA: 07
  0000000067075FAB: add         byte ptr [rcx+FFFFFFFF92000751h],ah
  0000000067075FB1: push        rcx
  0000000067075FB2: 07
  0000000067075FB3: add         byte ptr [rcx+FFFFFFFF86000751h],ah
  0000000067075FB9: push        rcx
  0000000067075FBA: 07
  0000000067075FBB: add         byte ptr [rcx+FFFFFFFFA1000751h],ah
  0000000067075FC1: push        rcx
  0000000067075FC2: 07
  0000000067075FC3: add         byte ptr [rax+0000000045000751h],al
  0000000067075FC9: push        rbp
  0000000067075FCA: 07
  0000000067075FCB: add         byte ptr [rcx+55h],bl
  0000000067075FCE: 07
  0000000067075FCF: add         byte ptr [rbx],dl
  0000000067075FD1: push        rbp
  0000000067075FD2: 07
  0000000067075FD3: add         byte ptr [rdi+55h],cl
  0000000067075FD6: 07
  0000000067075FD7: add         byte ptr [rdx*2+0000000055630007h],ch
  0000000067075FDE: 07
  0000000067075FDF: add         byte ptr [rbp+rdx*2+7],dh
  0000000067075FE3: add         byte ptr [rax],al
  0000000067075FE5: 06
  0000000067075FE6: 06
  0000000067075FE7: 06
  0000000067075FE8: 06
  0000000067075FE9: 06
  0000000067075FEA: 06
  0000000067075FEB: 06
  0000000067075FEC: 06
  0000000067075FED: 06
  0000000067075FEE: 06
  0000000067075FEF: 06
  0000000067075FF0: 06
  0000000067075FF1: 06
  0000000067075FF2: add         dword ptr [rsi],eax
  0000000067075FF4: 06
  0000000067075FF5: 06
  0000000067075FF6: 06
  0000000067075FF7: 06
  0000000067075FF8: 06
  0000000067075FF9: 06
  0000000067075FFA: 06
  0000000067075FFB: 06
  0000000067075FFC: 06
  0000000067075FFD: 06
  0000000067075FFE: 06
  0000000067075FFF: 06
  0000000067076000: add         al,byte ptr [rsi]
  0000000067076002: add         eax,dword ptr [rsi]
  0000000067076004: add         al,6
  0000000067076006: 06
  0000000067076007: 06
  0000000067076008: 06
  0000000067076009: 06
  000000006707600A: 06
  000000006707600B: 06
  000000006707600C: add         eax,6060606h
  0000000067076011: 06
  0000000067076012: add         dword ptr [rax+00000000000757B8h],edx
  0000000067076018: cmps        byte ptr [rsi],byte ptr [rdi]
  0000000067076019: push        rsi
  000000006707601A: 07
  000000006707601B: add         byte ptr [rcx+FFFFFFFFB8000758h],dl
  0000000067076021: push        rbp
  0000000067076022: 07
  0000000067076023: add         byte ptr [rdx+rbx*2],bh
  0000000067076026: 07
  0000000067076027: add         byte ptr [rcx+FFFFFFFFCB000759h],ch
  000000006707602D: pop         rax
  000000006707602E: 07
  000000006707602F: add         byte ptr [rcx+000000000000075Bh],dh
  0000000067076035: 07
  0000000067076036: 07
  0000000067076037: 07
  0000000067076038: add         byte ptr [rax],al
  000000006707603A: add         byte ptr [rdi],al
  000000006707603C: 07
  000000006707603D: 07
  000000006707603E: 07
  000000006707603F: 07
  0000000067076040: 07
  0000000067076041: 07
  0000000067076042: 07
  0000000067076043: 07
  0000000067076044: 07
  0000000067076045: 07
  0000000067076046: 07
  0000000067076047: 07
  0000000067076048: 07
  0000000067076049: 07
  000000006707604A: 07
  000000006707604B: add         dword ptr [rdi],eax
  000000006707604D: 07
  000000006707604E: 07
  000000006707604F: 07
  0000000067076050: 07
  0000000067076051: 07
  0000000067076052: 07
  0000000067076053: 07
  0000000067076054: add         byte ptr [rdi],al
  0000000067076056: add         al,byte ptr [rbx]
  0000000067076058: add         byte ptr [rax],al
  000000006707605A: add         byte ptr [rdi],al
  000000006707605C: add         eax,dword ptr [rdi]
  000000006707605E: 07
  000000006707605F: 07
  0000000067076060: 07
  0000000067076061: add         al,1
  0000000067076063: add         eax,7060707h
  0000000067076068: add         dword ptr [rdi],eax
  000000006707606A: 07
  000000006707606B: add         ebp,ecx
  000000006707606D: push        rbp
  000000006707606E: 07
  000000006707606F: add         bh,dl
  0000000067076071: push        rbp
  0000000067076072: 07
  0000000067076073: add         cl,ah
  0000000067076075: push        rbp
  0000000067076076: 07
  0000000067076077: add         bl,ch
  0000000067076079: push        rbp
  000000006707607A: 07
  000000006707607B: add         ch,dh
  000000006707607D: push        rbp
  000000006707607E: 07
  000000006707607F: add         byte ptr [00000000660767DBh],bl
  0000000067076085: push        rbp
  0000000067076086: 07
  0000000067076087: add         byte ptr [rcx],cl
  0000000067076089: push        rsi
  000000006707608A: 07
  000000006707608B: add         byte ptr [rbx],dl
  000000006707608D: push        rsi
  000000006707608E: 07
  000000006707608F: add         byte ptr [rbx+FFFFFFFFC5000756h],bh
  0000000067076095: push        rsi
  0000000067076096: 07
  0000000067076097: add         bh,cl
  0000000067076099: push        rsi
  000000006707609A: 07
  000000006707609B: add         cl,bl
  000000006707609D: push        rsi
  000000006707609E: 07
  000000006707609F: add         bl,ah
  00000000670760A1: push        rsi
  00000000670760A2: 07
  00000000670760A3: add         bh,dh
  00000000670760A5: push        rsi
  00000000670760A6: 07
  00000000670760A7: add         bh,dh
  00000000670760A9: push        rsi
  00000000670760AA: 07
  00000000670760AB: add         ch,ch
  00000000670760AD: push        rsi
  00000000670760AE: 07
  00000000670760AF: add         byte ptr [rax-77h],cl
  00000000670760B2: and         al,8
  00000000670760B5: push        rbx
  00000000670760B6: sub         rsp,30h
  00000000670760BA: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  00000000670760C3: mov         rbx,rcx
  00000000670760C6: mov         dword ptr [rsp+48h],0
  00000000670760CE: lea         rax,[00000000672F0B30h]
  00000000670760D5: mov         qword ptr [rcx],rax
  00000000670760D8: lock add    dword ptr [00000000672F0B30h],1
  00000000670760E0: mov         dword ptr [rsp+48h],1
  00000000670760E8: movsxd      rdx,edx
  00000000670760EB: call        0000000067070CD0
  00000000670760F0: mov         rax,rbx
  00000000670760F3: add         rsp,30h
  00000000670760F7: pop         rbx
  00000000670760F8: ret
  00000000670760F9: int         3
  00000000670760FA: int         3
  00000000670760FB: int         3
  00000000670760FC: int         3
  00000000670760FD: int         3
  00000000670760FE: int         3
  00000000670760FF: int         3
  0000000067076100: mov         qword ptr [rsp+8],rcx
  0000000067076105: push        rbx
  0000000067076106: sub         rsp,30h
  000000006707610A: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  0000000067076113: mov         rbx,rcx
  0000000067076116: mov         dword ptr [rsp+48h],0
  000000006707611E: lea         rax,[00000000672F0B30h]
  0000000067076125: mov         qword ptr [rcx],rax
  0000000067076128: lock add    dword ptr [00000000672F0B30h],1
  0000000067076130: mov         dword ptr [rsp+48h],1
  0000000067076138: mov         edx,edx
  000000006707613A: call        0000000067070D80
  000000006707613F: mov         rax,rbx
  0000000067076142: add         rsp,30h
  0000000067076146: pop         rbx
  0000000067076147: ret
  0000000067076148: int         3
  0000000067076149: int         3
  000000006707614A: int         3
  000000006707614B: int         3
  000000006707614C: int         3
  000000006707614D: int         3
  000000006707614E: int         3
  000000006707614F: int         3
  0000000067076150: push        rbp
  0000000067076152: push        rsi
  0000000067076153: push        rdi
  0000000067076154: push        r12
  0000000067076156: push        r13
  0000000067076158: sub         rsp,40h
  000000006707615C: mov         qword ptr [rsp+38h],0FFFFFFFFFFFFFFFEh
  0000000067076165: mov         qword ptr [rsp+78h],rbx
  000000006707616A: mov         rsi,rdx
  000000006707616D: mov         r12,rcx
  0000000067076170: cmp         dword ptr [rcx+20h],1
  0000000067076174: jne         0000000067076204
  000000006707617A: lea         rax,[00000000672F0900h]
  0000000067076181: mov         qword ptr [rsp+20h],rax
  0000000067076186: lock add    dword ptr [00000000672F0900h],1
  000000006707618E: lea         rdx,[rsp+20h]
  0000000067076193: call        000000006702F680
  0000000067076198: mov         r11,qword ptr [rsp+20h]
  000000006707619D: or          r8d,0FFFFFFFFh
  00000000670761A1: mov         rdx,qword ptr [r11+10h]
  00000000670761A5: lea         rcx,[rsp+28h]
  00000000670761AA: call        00000000670741E0
  00000000670761AF: mov         rdi,rax
  00000000670761B2: mov         rcx,qword ptr [rax]
  00000000670761B5: lock add    dword ptr [rcx],1
  00000000670761B9: mov         rdx,qword ptr [rsi]
  00000000670761BC: lock add    dword ptr [rdx],0FFFFFFFFh
  00000000670761C0: jne         00000000670761CA
  00000000670761C2: mov         rcx,qword ptr [rsi]
  00000000670761C5: call        00000000670700D0
  00000000670761CA: mov         rax,qword ptr [rdi]
  00000000670761CD: mov         qword ptr [rsi],rax
  00000000670761D0: mov         rax,qword ptr [rsp+28h]
  00000000670761D5: lock add    dword ptr [rax],0FFFFFFFFh
  00000000670761D9: jne         00000000670761E6
  00000000670761DB: mov         rcx,qword ptr [rsp+28h]
  00000000670761E0: call        00000000670700D0
  00000000670761E5: nop
  00000000670761E6: mov         rax,qword ptr [rsp+20h]
  00000000670761EB: lock add    dword ptr [rax],0FFFFFFFFh
  00000000670761EF: jne         0000000067076380
  00000000670761F5: mov         rcx,qword ptr [rsp+20h]
  00000000670761FA: call        00000000670203B0
  00000000670761FF: jmp         0000000067076380
  0000000067076204: xor         ebp,ebp
  0000000067076206: mov         dword ptr [rsp+70h],ebp
  000000006707620A: lea         rdx,[rsp+70h]
  000000006707620F: call        00000000670A98F0
  0000000067076214: mov         edi,dword ptr [rsp+70h]
  0000000067076218: cmp         edi,0FFFFFFFFh
  000000006707621B: jne         000000006707622A
  000000006707621D: mov         rcx,rsi
  0000000067076220: call        0000000067021650
  0000000067076225: jmp         0000000067076380
  000000006707622A: test        edi,edi
  000000006707622C: je          0000000067076335
  0000000067076232: test        dil,1
  0000000067076236: je          0000000067076255
  0000000067076238: mov         rcx,rsi
  000000006707623B: call        0000000067021650
  0000000067076240: mov         edx,2
  0000000067076245: mov         rcx,r12
  0000000067076248: call        00000000670A97E0
  000000006707624D: mov         rax,r12
  0000000067076250: jmp         0000000067076383
  0000000067076255: mov         dword ptr [rsp+0000000000000080h],100000h
  0000000067076260: shr         edi,1
  0000000067076262: je          000000006707630C
  0000000067076268: nop         dword ptr [rax+rax+0000000000000000h]
  0000000067076270: mov         ecx,edi
  0000000067076272: sub         ecx,ebp
  0000000067076274: mov         dword ptr [rsp+0000000000000088h],ecx
  000000006707627B: lea         rax,[rsp+0000000000000080h]
  0000000067076283: lea         rdx,[rsp+0000000000000088h]
  000000006707628B: cmp         ecx,100000h
  0000000067076291: cmovbe      rax,rdx
  0000000067076295: mov         ebx,dword ptr [rax]
  0000000067076297: lea         r13d,[rbx+rbp]
  000000006707629B: mov         edx,r13d
  000000006707629E: mov         rcx,rsi
  00000000670762A1: call        00000000670729C0
  00000000670762A6: mov         r11,qword ptr [rsi]
  00000000670762A9: mov         eax,dword ptr [r11]
  00000000670762AC: cmp         eax,1
  00000000670762AF: jne         00000000670762BE
  00000000670762B1: mov         rcx,qword ptr [rsi]
  00000000670762B4: lea         rax,[rcx+1Ah]
  00000000670762B8: cmp         qword ptr [rcx+10h],rax
  00000000670762BC: je          00000000670762CC
  00000000670762BE: mov         rax,qword ptr [rsi]
  00000000670762C1: mov         edx,dword ptr [rax+8]
  00000000670762C4: mov         rcx,rsi
  00000000670762C7: call        00000000670701A0
  00000000670762CC: mov         rax,qword ptr [rsi]
  00000000670762CF: add         ebx,ebx
  00000000670762D1: lea         edx,[rbp+rbp]
  00000000670762D5: add         rdx,qword ptr [rax+10h]
  00000000670762D9: mov         r8d,ebx
  00000000670762DC: mov         rcx,r12
  00000000670762DF: call        00000000670A99F0
  00000000670762E4: cmp         eax,ebx
  00000000670762E6: jne         00000000670762F2
  00000000670762E8: mov         ebp,r13d
  00000000670762EB: cmp         r13d,edi
  00000000670762EE: jb          0000000067076270
  00000000670762F0: jmp         000000006707630C
  00000000670762F2: mov         rcx,rsi
  00000000670762F5: call        0000000067021650
  00000000670762FA: mov         edx,1
  00000000670762FF: mov         rcx,r12
  0000000067076302: call        00000000670A97E0
  0000000067076307: mov         rax,r12
  000000006707630A: jmp         0000000067076383
  000000006707630C: cmp         dword ptr [r12+1Ch],0
  0000000067076312: jne         0000000067076380
  0000000067076314: mov         rcx,rsi
  0000000067076317: call        0000000067175A10
  000000006707631C: test        edi,edi
  000000006707631E: je          0000000067076380
  0000000067076320: movzx       ecx,word ptr [rax]
  0000000067076323: rol         cx,8
  0000000067076327: mov         word ptr [rax],cx
  000000006707632A: add         rax,2
  000000006707632E: add         edi,0FFFFFFFFh
  0000000067076331: jne         0000000067076320
  0000000067076333: jmp         0000000067076380
  0000000067076335: or          r8d,0FFFFFFFFh
  0000000067076339: lea         rdx,[00000000671F3C40h]
  0000000067076340: lea         rcx,[rsp+30h]
  0000000067076345: call        00000000670741E0
  000000006707634A: mov         rdi,rax
  000000006707634D: mov         rcx,qword ptr [rax]
  0000000067076350: lock add    dword ptr [rcx],1
  0000000067076354: mov         rdx,qword ptr [rsi]
  0000000067076357: lock add    dword ptr [rdx],0FFFFFFFFh
  000000006707635B: jne         0000000067076365
  000000006707635D: mov         rcx,qword ptr [rsi]
  0000000067076360: call        00000000670700D0
  0000000067076365: mov         rax,qword ptr [rdi]
  0000000067076368: mov         qword ptr [rsi],rax
  000000006707636B: mov         rax,qword ptr [rsp+30h]
  0000000067076370: lock add    dword ptr [rax],0FFFFFFFFh
  0000000067076374: jne         0000000067076380
  0000000067076376: mov         rcx,qword ptr [rsp+30h]
  000000006707637B: call        00000000670700D0
  0000000067076380: mov         rax,r12
  0000000067076383: mov         rbx,qword ptr [rsp+78h]
  0000000067076388: add         rsp,40h
  000000006707638C: pop         r13
  000000006707638E: pop         r12
  0000000067076390: pop         rdi
  0000000067076391: pop         rsi
  0000000067076392: pop         rbp
  0000000067076393: ret
  0000000067076394: int         3
  0000000067076395: int         3
  0000000067076396: int         3
  0000000067076397: int         3
  0000000067076398: int         3
  0000000067076399: int         3
  000000006707639A: int         3
  000000006707639B: int         3
  000000006707639C: int         3
  000000006707639D: int         3
  000000006707639E: int         3
  000000006707639F: int         3
  00000000670763A0: mov         qword ptr [rsp+10h],rbx
  00000000670763A5: mov         qword ptr [rsp+18h],rsi
  00000000670763AA: push        rdi
  00000000670763AB: sub         rsp,20h
  00000000670763AF: mov         rdi,r8
  00000000670763B2: mov         rbx,rdx
  00000000670763B5: mov         rsi,rcx
  00000000670763B8: test        r8,r8
  00000000670763BB: jne         00000000670763DB
  00000000670763BD: xor         eax,eax
  00000000670763BF: mov         qword ptr [rdx],rax
  00000000670763C2: mov         dword ptr [rdx+8],eax
  00000000670763C5: mov         dword ptr [rdx+0Ch],eax
  00000000670763C8: mov         rax,rdx
  00000000670763CB: mov         rbx,qword ptr [rsp+38h]
  00000000670763D0: mov         rsi,qword ptr [rsp+40h]
  00000000670763D5: add         rsp,20h
  00000000670763D9: pop         rdi
  00000000670763DA: ret
  00000000670763DB: mov         rax,qword ptr [r8]
  00000000670763DE: mov         qword ptr [rsp+30h],rbp
  00000000670763E3: mov         ebp,dword ptr [rax+8]
  00000000670763E6: mov         rax,qword ptr [rcx]
  00000000670763E9: test        rax,rax
  00000000670763EC: jne         00000000670763F7
  00000000670763EE: mov         r8,qword ptr [00000000672F0B40h]
  00000000670763F5: jmp         0000000067076406
  00000000670763F7: movsxd      rdx,dword ptr [rcx+8]
  00000000670763FB: mov         rax,qword ptr [rax]
  00000000670763FE: mov         rcx,qword ptr [rax+10h]
  0000000067076402: lea         r8,[rcx+rdx*2]
  0000000067076406: mov         r9d,dword ptr [rsi+0Ch]
  000000006707640A: mov         edx,ebp
  000000006707640C: mov         rcx,rdi
  000000006707640F: call        0000000067072CA0
  0000000067076414: mov         eax,dword ptr [rsi+0Ch]
  0000000067076417: mov         rsi,qword ptr [rsp+40h]
  000000006707641C: mov         dword ptr [rbx+0Ch],eax
  000000006707641F: mov         dword ptr [rbx+8],ebp
  0000000067076422: mov         rbp,qword ptr [rsp+30h]
  0000000067076427: mov         rax,rbx
  000000006707642A: mov         qword ptr [rbx],rdi
  000000006707642D: mov         rbx,qword ptr [rsp+38h]
  0000000067076432: add         rsp,20h
  0000000067076436: pop         rdi
  0000000067076437: ret
  0000000067076438: int         3
  0000000067076439: int         3
  000000006707643A: int         3
  000000006707643B: int         3
  000000006707643C: int         3
  000000006707643D: int         3
  000000006707643E: int         3
  000000006707643F: int         3
  0000000067076440: mov         qword ptr [rsp+18h],rbx
  0000000067076445: push        rdi
  0000000067076446: sub         rsp,30h
  000000006707644A: mov         rax,qword ptr [rdx]
  000000006707644D: mov         rdi,rdx
  0000000067076450: mov         rbx,rcx
  0000000067076453: cmp         rax,rcx
  0000000067076456: jne         0000000067076476
  0000000067076458: mov         r8,rcx
  000000006707645B: lea         rdx,[rsp+20h]
  0000000067076460: mov         rcx,rdi
  0000000067076463: call        00000000670763A0
  0000000067076468: mov         rax,rbx
  000000006707646B: mov         rbx,qword ptr [rsp+50h]
  0000000067076470: add         rsp,30h
  0000000067076474: pop         rdi
  0000000067076475: ret
  0000000067076476: test        rax,rax
  0000000067076479: je          0000000067076507
  000000006707647F: mov         rax,qword ptr [rcx]
  0000000067076482: mov         qword ptr [rsp+40h],rbp
  0000000067076487: mov         qword ptr [rsp+48h],rsi
  000000006707648C: movsxd      rsi,dword ptr [rax+8]
  0000000067076490: mov         eax,dword ptr [rdx+0Ch]
  0000000067076493: lea         edx,[rsi+rax]
  0000000067076496: call        00000000670729C0
  000000006707649B: mov         rax,qword ptr [rdi]
  000000006707649E: movsxd      rbp,dword ptr [rdi+0Ch]
  00000000670764A2: test        rax,rax
  00000000670764A5: jne         00000000670764B0
  00000000670764A7: mov         rdi,qword ptr [00000000672F0B40h]
  00000000670764AE: jmp         00000000670764BF
  00000000670764B0: mov         rax,qword ptr [rax]
  00000000670764B3: movsxd      rdx,dword ptr [rdi+8]
  00000000670764B7: mov         rcx,qword ptr [rax+10h]
  00000000670764BB: lea         rdi,[rcx+rdx*2]
  00000000670764BF: mov         rax,qword ptr [rbx]
  00000000670764C2: mov         ecx,dword ptr [rax]
  00000000670764C4: cmp         ecx,1
  00000000670764C7: jne         00000000670764D6
  00000000670764C9: mov         rcx,qword ptr [rbx]
  00000000670764CC: lea         rax,[rcx+1Ah]
  00000000670764D0: cmp         qword ptr [rcx+10h],rax
  00000000670764D4: je          00000000670764E4
  00000000670764D6: mov         rax,qword ptr [rbx]
  00000000670764D9: mov         rcx,rbx
  00000000670764DC: mov         edx,dword ptr [rax+8]
  00000000670764DF: call        00000000670701A0
  00000000670764E4: mov         rax,qword ptr [rbx]
  00000000670764E7: mov         r8,rbp
  00000000670764EA: mov         rdx,rdi
  00000000670764ED: mov         rcx,qword ptr [rax+10h]
  00000000670764F1: add         r8,r8
  00000000670764F4: lea         rcx,[rcx+rsi*2]
  00000000670764F8: call        0000000067178A9E
  00000000670764FD: mov         rsi,qword ptr [rsp+48h]
  0000000067076502: mov         rbp,qword ptr [rsp+40h]
  0000000067076507: mov         rax,rbx
  000000006707650A: mov         rbx,qword ptr [rsp+50h]
  000000006707650F: add         rsp,30h
  0000000067076513: pop         rdi
  0000000067076514: ret
  0000000067076515: int         3
  0000000067076516: int         3
  0000000067076517: int         3
  0000000067076518: int         3
  0000000067076519: int         3
  000000006707651A: int         3
  000000006707651B: int         3
  000000006707651C: int         3
  000000006707651D: int         3
  000000006707651E: int         3
  000000006707651F: int         3
  0000000067076520: mov         r11,rsp
  0000000067076523: mov         qword ptr [r11+10h],rbx
  0000000067076527: mov         qword ptr [r11+18h],rbp
  000000006707652B: push        rsi
  000000006707652C: push        rdi
  000000006707652D: push        r12
  000000006707652F: push        r13
  0000000067076531: push        r15
  0000000067076533: sub         rsp,460h
  000000006707653A: xor         eax,eax
  000000006707653C: or          r15d,0FFFFFFFFh
  0000000067076540: cmp         edx,1F4h
  0000000067076546: mov         esi,r9d
  0000000067076549: mov         rbp,r8
  000000006707654C: mov         r12d,edx
  000000006707654F: mov         ebx,eax
  0000000067076551: mov         r13,rcx
  0000000067076554: jle         0000000067076618
  000000006707655A: cmp         r9d,5
  000000006707655E: jle         0000000067076618
  0000000067076564: mov         qword ptr [rsp+40h],rax
  0000000067076569: lea         rax,[00000000672F0B30h]
  0000000067076570: mov         qword ptr [r11+8],r14
  0000000067076574: mov         qword ptr [rsp+48h],rax
  0000000067076579: lock add    dword ptr [00000000672F0B30h],1
  0000000067076581: mov         r9d,dword ptr [rsp+00000000000004B0h]
  0000000067076589: mov         edx,esi
  000000006707658B: mov         rcx,rbp
  000000006707658E: mov         dword ptr [rsp+50h],r9d
  0000000067076593: mov         qword ptr [r11+FFFFFFFFFFFFFCD0h],r8
  000000006707659A: lea         r8,[rsp+58h]
  000000006707659F: mov         dword ptr [rsp+0000000000000160h],esi
  00000000670765A6: call        000000006706D570
  00000000670765AB: mov         esi,dword ptr [rsp+0000000000000160h]
  00000000670765B2: mov         rbp,qword ptr [rsp+0000000000000158h]
  00000000670765BA: mov         r14d,dword ptr [rsp+50h]
  00000000670765BF: xor         edi,edi
  00000000670765C1: lea         r8d,[r15+1]
  00000000670765C5: lea         rax,[rsp+58h]
  00000000670765CA: mov         dword ptr [rsp+30h],r14d
  00000000670765CF: test        r8d,r8d
  00000000670765D2: mov         qword ptr [rsp+28h],rax
  00000000670765D7: mov         r9,rbp
  00000000670765DA: mov         edx,r12d
  00000000670765DD: mov         rcx,r13
  00000000670765E0: cmovs       r8d,edi
  00000000670765E4: mov         dword ptr [rsp+20h],esi
  00000000670765E8: call        000000006706AA80
  00000000670765ED: mov         r15d,eax
  00000000670765F0: cmp         eax,0FFFFFFFFh
  00000000670765F3: je          00000000670765F9
  00000000670765F5: inc         ebx
  00000000670765F7: jmp         00000000670765C1
  00000000670765F9: mov         rcx,qword ptr [rsp+48h]
  00000000670765FE: lock add    dword ptr [rcx],0FFFFFFFFh
  0000000067076602: mov         r14,qword ptr [rsp+0000000000000490h]
  000000006707660A: jne         0000000067076661
  000000006707660C: mov         rcx,qword ptr [rsp+48h]
  0000000067076611: call        00000000670700D0
  0000000067076616: jmp         0000000067076661
  0000000067076618: mov         edi,dword ptr [rsp+00000000000004B0h]
  000000006707661F: mov         dword ptr [rsp+28h],edi
  0000000067076623: mov         dword ptr [rsp+20h],r9d
  0000000067076628: mov         r9,r8
  000000006707662B: xor         r8d,r8d
  000000006707662E: call        000000006706DB00
  0000000067076633: cmp         eax,0FFFFFFFFh
  0000000067076636: je          0000000067076661
  0000000067076638: nop         dword ptr [rax+rax+0000000000000000h]
  0000000067076640: lea         r8d,[rax+1]
  0000000067076644: mov         r9,rbp
  0000000067076647: mov         edx,r12d
  000000006707664A: mov         rcx,r13
  000000006707664D: mov         dword ptr [rsp+28h],edi
  0000000067076651: inc         ebx
  0000000067076653: mov         dword ptr [rsp+20h],esi
  0000000067076657: call        000000006706DB00
  000000006707665C: cmp         eax,0FFFFFFFFh
  000000006707665F: jne         0000000067076640
  0000000067076661: lea         r11,[rsp+0000000000000460h]
  0000000067076669: mov         eax,ebx
  000000006707666B: mov         rbx,qword ptr [r11+38h]
  000000006707666F: mov         rbp,qword ptr [r11+40h]
  0000000067076673: mov         rsp,r11
  0000000067076676: pop         r15
  0000000067076678: pop         r13
  000000006707667A: pop         r12
  000000006707667C: pop         rdi
  000000006707667D: pop         rsi
  000000006707667E: ret
  000000006707667F: int         3
  0000000067076680: cmp         rdx,r8
  0000000067076683: je          00000000670766DC
  0000000067076685: mov         qword ptr [rsp+10h],rbx
  000000006707668A: push        rsi
  000000006707668B: sub         rsp,20h
  000000006707668F: mov         rbx,r8
  0000000067076692: mov         rsi,rdx
  0000000067076695: mov         qword ptr [rsp+30h],rdi
  000000006707669A: nop         word ptr [rax+rax]
  00000000670766A0: mov         rdi,qword ptr [rbx-8]
  00000000670766A4: sub         rbx,8
  00000000670766A8: test        rdi,rdi
  00000000670766AB: je          00000000670766C8
  00000000670766AD: mov         rdx,qword ptr [rdi+8]
  00000000670766B1: lock add    dword ptr [rdx],0FFFFFFFFh
  00000000670766B5: jne         00000000670766C0
  00000000670766B7: mov         rcx,qword ptr [rdi+8]
  00000000670766BB: call        00000000670700D0
  00000000670766C0: mov         rcx,rdi
  00000000670766C3: call        0000000067178956
  00000000670766C8: cmp         rbx,rsi
  00000000670766CB: jne         00000000670766A0
  00000000670766CD: mov         rdi,qword ptr [rsp+30h]
  00000000670766D2: mov         rbx,qword ptr [rsp+38h]
  00000000670766D7: add         rsp,20h
  00000000670766DB: pop         rsi
  00000000670766DC: rep ret
  00000000670766DE: int         3
  00000000670766DF: int         3
  00000000670766E0: mov         qword ptr [rsp+10h],rdx
  00000000670766E5: push        rdi
  00000000670766E6: sub         rsp,30h
  00000000670766EA: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  00000000670766F3: mov         qword ptr [rsp+40h],rbx
  00000000670766F8: mov         qword ptr [rsp+58h],rsi
  00000000670766FD: mov         rdi,r9
  0000000067076700: mov         rsi,r8
  0000000067076703: mov         rbx,rdx
  0000000067076706: mov         qword ptr [rsp+50h],rdx
  000000006707670B: nop         dword ptr [rax+rax]
  0000000067076710: cmp         rbx,rsi
  0000000067076713: je          000000006707674D
  0000000067076715: mov         ecx,10h
  000000006707671A: call        0000000067178A86
  000000006707671F: test        rax,rax
  0000000067076722: je          0000000067076739
  0000000067076724: mov         rdx,qword ptr [rdi]
  0000000067076727: mov         ecx,dword ptr [rdx]
  0000000067076729: mov         dword ptr [rax],ecx
  000000006707672B: mov         rcx,qword ptr [rdx+8]
  000000006707672F: mov         qword ptr [rax+8],rcx
  0000000067076733: lock add    dword ptr [rcx],1
  0000000067076737: jmp         000000006707673B
  0000000067076739: xor         eax,eax
  000000006707673B: mov         qword ptr [rbx],rax
  000000006707673E: add         rbx,8
  0000000067076742: mov         qword ptr [rsp+50h],rbx
  0000000067076747: add         rdi,8
  000000006707674B: jmp         0000000067076710
  000000006707674D: mov         rbx,qword ptr [rsp+40h]
  0000000067076752: mov         rsi,qword ptr [rsp+58h]
  0000000067076757: add         rsp,30h
  000000006707675B: pop         rdi
  000000006707675C: ret
  000000006707675D: int         3
  000000006707675E: int         3
  000000006707675F: int         3
  0000000067076760: push        rbx
  0000000067076762: sub         rsp,40h
  0000000067076766: mov         rax,qword ptr [00000000672F3BA0h]
  000000006707676D: xor         rax,rsp
  0000000067076770: mov         qword ptr [rsp+30h],rax
  0000000067076775: mov         r10d,edx
  0000000067076778: mov         rbx,rcx
  000000006707677B: lea         r9,[rsp+28h]
  0000000067076780: lea         rdx,[rsp+20h]
  0000000067076785: lea         r8,[rsp+24h]
  000000006707678A: mov         ecx,r10d
  000000006707678D: mov         dword ptr [rsp+20h],0
  0000000067076795: call        000000006706A2C0
  000000006707679A: mov         r8d,dword ptr [rsp+20h]
  000000006707679F: mov         rdx,rax
  00000000670767A2: mov         rcx,rbx
  00000000670767A5: call        00000000670743C0
  00000000670767AA: mov         rax,rbx
  00000000670767AD: mov         rcx,qword ptr [rsp+30h]
  00000000670767B2: xor         rcx,rsp
  00000000670767B5: call        0000000067178C10
  00000000670767BA: add         rsp,40h
  00000000670767BE: pop         rbx
  00000000670767BF: ret
  00000000670767C0: test        r8d,r8d
  00000000670767C3: js          0000000067076A4E
  00000000670767C9: mov         dword ptr [rsp+10h],edx
  00000000670767CD: push        rbx
  00000000670767CE: push        rdi
  00000000670767CF: push        r13
  00000000670767D1: sub         rsp,50h
  00000000670767D5: mov         r13,rcx
  00000000670767D8: mov         rcx,qword ptr [rcx]
  00000000670767DB: mov         qword ptr [rsp+28h],r15
  00000000670767E0: mov         r15d,dword ptr [rcx+8]
  00000000670767E4: movsxd      rdi,r8d
  00000000670767E7: mov         ebx,edx
  00000000670767E9: mov         eax,r15d
  00000000670767EC: mov         qword ptr [rsp+0000000000000088h],rcx
  00000000670767F4: sub         eax,edi
  00000000670767F6: cmp         eax,2
  00000000670767F9: jl          0000000067076A41
  00000000670767FF: mov         qword ptr [rsp+40h],rsi
  0000000067076804: mov         qword ptr [rsp+38h],r12
  0000000067076809: xor         r12d,r12d
  000000006707680C: cmp         edi,r15d
  000000006707680F: mov         rsi,rdi
  0000000067076812: mov         r10d,r12d
  0000000067076815: mov         dword ptr [rsp+0000000000000080h],r12d
  000000006707681D: mov         r11,r12
  0000000067076820: mov         qword ptr [rsp+70h],r12
  0000000067076825: jge         0000000067076A37
  000000006707682B: mov         qword ptr [rsp+48h],rbp
  0000000067076830: mov         qword ptr [rsp+30h],r14
  0000000067076835: mov         r8d,0FC00h
  000000006707683B: lea         r14d,[rdi-1]
  000000006707683F: lea         rbp,[0000000067000000h]
  0000000067076846: nop         word ptr [rax+rax+0000000000000000h]
  0000000067076850: mov         r9,qword ptr [rcx+10h]
  0000000067076854: mov         ecx,0D800h
  0000000067076859: movzx       edx,word ptr [r9+rsi*2]
  000000006707685E: movzx       eax,dx
  0000000067076861: and         ax,r8w
  0000000067076865: cmp         ax,cx
  0000000067076868: jne         00000000670768A6
  000000006707686A: lea         eax,[r15-1]
  000000006707686E: cmp         edi,eax
  0000000067076870: jge         00000000670768A6
  0000000067076872: movzx       r8d,word ptr [r9+rsi*2+2]
  0000000067076878: mov         ecx,0FC00h
  000000006707687D: movzx       eax,r8w
  0000000067076881: and         ax,cx
  0000000067076884: mov         ecx,0DC00h
  0000000067076889: cmp         ax,cx
  000000006707688C: jne         00000000670768A6
  000000006707688E: mov         ecx,edx
  0000000067076890: movzx       eax,r8w
  0000000067076894: inc         edi
  0000000067076896: shl         ecx,0Ah
  0000000067076899: inc         r14d
  000000006707689C: inc         rsi
  000000006707689F: lea         edx,[rcx+rax+FFFFFFFFFCA02400h]
  00000000670768A6: cmp         edx,11000h
  00000000670768AC: jae         00000000670768CD
  00000000670768AE: mov         eax,edx
  00000000670768B0: shr         rax,5
  00000000670768B4: movzx       ecx,word ptr [rbp+rax*2+00000000001F4050h]
  00000000670768BC: mov         eax,edx
  00000000670768BE: and         eax,1Fh
  00000000670768C1: add         ecx,eax
  00000000670768C3: movzx       ecx,word ptr [rbp+rcx*2+00000000001F4050h]
  00000000670768CB: jmp         00000000670768F0
  00000000670768CD: lea         eax,[rdx+FFFFFFFFFFFEF000h]
  00000000670768D3: shr         eax,8
  00000000670768D6: add         eax,880h
  00000000670768DB: movzx       ecx,word ptr [rbp+rax*2+00000000001F4050h]
  00000000670768E3: movzx       eax,dl
  00000000670768E6: add         ecx,eax
  00000000670768E8: mov         cx,word ptr [rbp+rcx*2+00000000001F4050h]
  00000000670768F0: movzx       eax,cx
  00000000670768F3: mov         ecx,0F00h
  00000000670768F8: lea         r8,[rax+rax*4]
  00000000670768FC: mov         ax,word ptr [rbp+r8*4+00000000001FF7B4h]
  0000000067076905: test        cx,ax
  0000000067076908: je          00000000670769E7
  000000006707690E: shr         eax,8
  0000000067076911: and         eax,0Fh
  0000000067076914: cmp         eax,ebx
  0000000067076916: jg          00000000670769E7
  000000006707691C: movzx       ebp,byte ptr [rbp+r8*4+00000000001FF7B3h]
  0000000067076925: cmp         r12d,r14d
  0000000067076928: je          0000000067076933
  000000006707692A: cmp         ebp,r10d
  000000006707692D: jle         00000000670769BD
  0000000067076933: movzx       ecx,word ptr [r9+r11*2]
  0000000067076938: call        000000006706F480
  000000006707693D: movzx       ebx,ax
  0000000067076940: test        ax,ax
  0000000067076943: je          00000000670769B8
  0000000067076945: cmp         r12d,r15d
  0000000067076948: jl          0000000067076957
  000000006707694A: mov         edx,r12d
  000000006707694D: mov         rcx,r13
  0000000067076950: call        0000000067072A90
  0000000067076955: jmp         0000000067076983
  0000000067076957: mov         rax,qword ptr [rsp+0000000000000088h]
  000000006707695F: mov         eax,dword ptr [rax]
  0000000067076961: cmp         eax,1
  0000000067076964: jne         0000000067076974
  0000000067076966: mov         rcx,qword ptr [r13]
  000000006707696A: lea         rax,[rcx+1Ah]
  000000006707696E: cmp         qword ptr [rcx+10h],rax
  0000000067076972: je          0000000067076983
  0000000067076974: mov         rax,qword ptr [r13]
  0000000067076978: mov         rcx,r13
  000000006707697B: mov         edx,dword ptr [rax+8]
  000000006707697E: call        00000000670701A0
  0000000067076983: mov         rax,qword ptr [r13]
  0000000067076987: movsxd      rdx,r12d
  000000006707698A: mov         r8d,1
  0000000067076990: mov         rcx,qword ptr [rax+10h]
  0000000067076994: mov         word ptr [rcx+rdx*2],bx
  0000000067076998: mov         edx,edi
  000000006707699A: mov         rcx,r13
  000000006707699D: call        0000000067072E60
  00000000670769A2: mov         r10d,dword ptr [rsp+0000000000000080h]
  00000000670769AA: mov         r11,qword ptr [rsp+70h]
  00000000670769AF: lea         rbp,[0000000067000000h]
  00000000670769B6: jmp         0000000067076A0A
  00000000670769B8: mov         r11,qword ptr [rsp+70h]
  00000000670769BD: test        ebp,ebp
  00000000670769BF: jne         00000000670769CC
  00000000670769C1: mov         r12d,edi
  00000000670769C4: mov         r11,rsi
  00000000670769C7: mov         qword ptr [rsp+70h],rsi
  00000000670769CC: mov         r10d,ebp
  00000000670769CF: mov         dword ptr [rsp+0000000000000080h],ebp
  00000000670769D6: inc         edi
  00000000670769D8: inc         r14d
  00000000670769DB: lea         rbp,[0000000067000000h]
  00000000670769E2: inc         rsi
  00000000670769E5: jmp         0000000067076A0A
  00000000670769E7: or          r11,0FFFFFFFFFFFFFFFFh
  00000000670769EB: mov         r10d,0FFh
  00000000670769F1: or          r12d,0FFFFFFFFh
  00000000670769F5: inc         edi
  00000000670769F7: inc         r14d
  00000000670769FA: mov         qword ptr [rsp+70h],r11
  00000000670769FF: mov         dword ptr [rsp+0000000000000080h],r10d
  0000000067076A07: inc         rsi
  0000000067076A0A: mov         rcx,qword ptr [r13]
  0000000067076A0E: mov         ebx,dword ptr [rsp+78h]
  0000000067076A12: mov         r8d,0FC00h
  0000000067076A18: mov         r15d,dword ptr [rcx+8]
  0000000067076A1C: mov         qword ptr [rsp+0000000000000088h],rcx
  0000000067076A24: cmp         edi,r15d
  0000000067076A27: jl          0000000067076850
  0000000067076A2D: mov         r14,qword ptr [rsp+30h]
  0000000067076A32: mov         rbp,qword ptr [rsp+48h]
  0000000067076A37: mov         r12,qword ptr [rsp+38h]
  0000000067076A3C: mov         rsi,qword ptr [rsp+40h]
  0000000067076A41: mov         r15,qword ptr [rsp+28h]
  0000000067076A46: add         rsp,50h
  0000000067076A4A: pop         r13
  0000000067076A4C: pop         rdi
  0000000067076A4D: pop         rbx
  0000000067076A4E: rep ret
  0000000067076A50: push        rbx
  0000000067076A52: push        r12
  0000000067076A54: push        r13
  0000000067076A56: push        r15
  0000000067076A58: sub         rsp,28h
  0000000067076A5C: mov         rax,qword ptr [rcx]
  0000000067076A5F: mov         ebx,r8d
  0000000067076A62: mov         r15d,edx
  0000000067076A65: mov         r13d,dword ptr [rax+8]
  0000000067076A69: mov         r12,rcx
  0000000067076A6C: dec         r13d
  0000000067076A6F: cmp         r8d,r13d
  0000000067076A72: jge         0000000067076DBD
  0000000067076A78: mov         qword ptr [rsp+50h],rbp
  0000000067076A7D: mov         qword ptr [rsp+58h],rsi
  0000000067076A82: mov         qword ptr [rsp+60h],rdi
  0000000067076A87: mov         qword ptr [rsp+20h],r14
  0000000067076A8C: mov         ecx,0FC00h
  0000000067076A91: mov         esi,0DC00h
  0000000067076A96: mov         r9d,0D800h
  0000000067076A9C: lea         r11,[0000000067000000h]
  0000000067076AA3: nop         word ptr [rax+rax+0000000000000000h]
  0000000067076AB0: mov         r10,qword ptr [r12]
  0000000067076AB4: movsxd      r14,ebx
  0000000067076AB7: lea         ebp,[rbx+1]
  0000000067076ABA: mov         r8,qword ptr [r10+10h]
  0000000067076ABE: movsxd      rdx,ebp
  0000000067076AC1: movzx       edi,word ptr [r8+r14*2]
  0000000067076AC6: movzx       eax,di
  0000000067076AC9: and         ax,cx
  0000000067076ACC: cmp         ax,r9w
  0000000067076AD0: jne         0000000067076AFE
  0000000067076AD2: movzx       r9d,word ptr [r8+rdx*2]
  0000000067076AD7: movzx       eax,r9w
  0000000067076ADB: and         ax,cx
  0000000067076ADE: cmp         ax,si
  0000000067076AE1: jne         0000000067076AFE
  0000000067076AE3: mov         ecx,edi
  0000000067076AE5: movzx       eax,r9w
  0000000067076AE9: shl         ecx,0Ah
  0000000067076AEC: cmp         edx,r13d
  0000000067076AEF: lea         edi,[rcx+rax+FFFFFFFFFCA02400h]
  0000000067076AF6: jge         0000000067076DA9
  0000000067076AFC: inc         edx
  0000000067076AFE: mov         r9d,0FC00h
  0000000067076B04: movsxd      rcx,edx
  0000000067076B07: movzx       esi,word ptr [r8+rcx*2]
  0000000067076B0C: movzx       eax,si
  0000000067076B0F: and         ax,r9w
  0000000067076B13: mov         r9d,0D800h
  0000000067076B19: cmp         ax,r9w
  0000000067076B1D: jne         0000000067076B52
  0000000067076B1F: cmp         edx,r13d
  0000000067076B22: jge         0000000067076B52
  0000000067076B24: movzx       r9d,word ptr [r8+rcx*2+2]
  0000000067076B2A: mov         ecx,0FC00h
  0000000067076B2F: movzx       eax,r9w
  0000000067076B33: and         ax,cx
  0000000067076B36: mov         ecx,0DC00h
  0000000067076B3B: cmp         ax,cx
  0000000067076B3E: jne         0000000067076B52
  0000000067076B40: mov         ecx,esi
  0000000067076B42: movzx       eax,r9w
  0000000067076B46: shl         ecx,0Ah
  0000000067076B49: inc         edx
  0000000067076B4B: lea         esi,[rcx+rax+FFFFFFFFFCA02400h]
  0000000067076B52: cmp         esi,11000h
  0000000067076B58: jae         0000000067076B7B
  0000000067076B5A: mov         eax,esi
  0000000067076B5C: shr         rax,5
  0000000067076B60: movzx       ecx,word ptr [r11+rax*2+00000000001F4050h]
  0000000067076B69: mov         eax,esi
  0000000067076B6B: and         eax,1Fh
  0000000067076B6E: add         ecx,eax
  0000000067076B70: movzx       ecx,word ptr [r11+rcx*2+00000000001F4050h]
  0000000067076B79: jmp         0000000067076BA1
  0000000067076B7B: lea         eax,[rsi+FFFFFFFFFFFEF000h]
  0000000067076B81: shr         eax,8
  0000000067076B84: add         eax,880h
  0000000067076B89: movzx       ecx,word ptr [r11+rax*2+00000000001F4050h]
  0000000067076B92: movzx       eax,sil
  0000000067076B96: add         ecx,eax
  0000000067076B98: mov         cx,word ptr [r11+rcx*2+00000000001F4050h]
  0000000067076BA1: movzx       eax,cx
  0000000067076BA4: mov         r9d,0F00h
  0000000067076BAA: lea         rcx,[rax+rax*4]
  0000000067076BAE: mov         ax,word ptr [r11+rcx*4+00000000001FF7B4h]
  0000000067076BB7: test        r9w,ax
  0000000067076BBB: je          0000000067076D69
  0000000067076BC1: shr         eax,8
  0000000067076BC4: and         eax,0Fh
  0000000067076BC7: cmp         eax,r15d
  0000000067076BCA: jg          0000000067076D69
  0000000067076BD0: movzx       r8d,byte ptr [r11+rcx*4+00000000001FF7B3h]
  0000000067076BD9: test        r8w,r8w
  0000000067076BDD: je          0000000067076D69
  0000000067076BE3: cmp         edi,11000h
  0000000067076BE9: jae         0000000067076C0C
  0000000067076BEB: mov         eax,edi
  0000000067076BED: shr         rax,5
  0000000067076BF1: movzx       ecx,word ptr [r11+rax*2+00000000001F4050h]
  0000000067076BFA: mov         eax,edi
  0000000067076BFC: and         eax,1Fh
  0000000067076BFF: add         ecx,eax
  0000000067076C01: movzx       ecx,word ptr [r11+rcx*2+00000000001F4050h]
  0000000067076C0A: jmp         0000000067076C32
  0000000067076C0C: lea         eax,[rdi+FFFFFFFFFFFEF000h]
  0000000067076C12: shr         eax,8
  0000000067076C15: add         eax,880h
  0000000067076C1A: movzx       ecx,word ptr [r11+rax*2+00000000001F4050h]
  0000000067076C23: movzx       eax,dil
  0000000067076C27: add         ecx,eax
  0000000067076C29: mov         cx,word ptr [r11+rcx*2+00000000001F4050h]
  0000000067076C32: movzx       eax,cx
  0000000067076C35: lea         rcx,[rax+rax*4]
  0000000067076C39: mov         ax,word ptr [r11+rcx*4+00000000001FF7B4h]
  0000000067076C42: test        r9w,ax
  0000000067076C46: je          0000000067076D51
  0000000067076C4C: shr         eax,8
  0000000067076C4F: and         eax,0Fh
  0000000067076C52: cmp         eax,r15d
  0000000067076C55: jg          0000000067076D51
  0000000067076C5B: movzx       eax,byte ptr [r11+rcx*4+00000000001FF7B3h]
  0000000067076C64: cmp         ax,r8w
  0000000067076C68: jbe         0000000067076D51
  0000000067076C6E: mov         eax,dword ptr [r10]
  0000000067076C71: cmp         eax,1
  0000000067076C74: jne         0000000067076C84
  0000000067076C76: mov         rcx,qword ptr [r12]
  0000000067076C7A: lea         rax,[rcx+1Ah]
  0000000067076C7E: cmp         qword ptr [rcx+10h],rax
  0000000067076C82: je          0000000067076C9A
  0000000067076C84: mov         rax,qword ptr [r12]
  0000000067076C88: mov         rcx,r12
  0000000067076C8B: mov         edx,dword ptr [rax+8]
  0000000067076C8E: call        00000000670701A0
  0000000067076C93: lea         r11,[0000000067000000h]
  0000000067076C9A: mov         rax,qword ptr [r12]
  0000000067076C9E: mov         rdx,qword ptr [rax+10h]
  0000000067076CA2: cmp         esi,10000h
  0000000067076CA8: jae         0000000067076CC3
  0000000067076CAA: mov         word ptr [rdx+r14*2],si
  0000000067076CAF: mov         r8d,2840h
  0000000067076CB5: mov         r9d,3FFh
  0000000067076CBB: mov         r10d,2400h
  0000000067076CC1: jmp         0000000067076CF4
  0000000067076CC3: mov         eax,esi
  0000000067076CC5: mov         r8d,2840h
  0000000067076CCB: mov         r9d,3FFh
  0000000067076CD1: shr         eax,0Ah
  0000000067076CD4: and         si,r9w
  0000000067076CD8: mov         r10d,2400h
  0000000067076CDE: sub         ax,r8w
  0000000067076CE2: sub         si,r10w
  0000000067076CE6: mov         word ptr [rdx+r14*2],ax
  0000000067076CEB: movsxd      rax,ebp
  0000000067076CEE: inc         ebp
  0000000067076CF0: mov         word ptr [rdx+rax*2],si
  0000000067076CF4: cmp         edi,10000h
  0000000067076CFA: jae         0000000067076D05
  0000000067076CFC: movsxd      rax,ebp
  0000000067076CFF: mov         word ptr [rdx+rax*2],di
  0000000067076D03: jmp         0000000067076D22
  0000000067076D05: mov         eax,edi
  0000000067076D07: movsxd      rcx,ebp
  0000000067076D0A: and         di,r9w
  0000000067076D0E: shr         eax,0Ah
  0000000067076D11: sub         ax,r8w
  0000000067076D15: sub         di,r10w
  0000000067076D19: mov         word ptr [rdx+rcx*2],ax
  0000000067076D1D: mov         word ptr [rdx+rcx*2+2],di
  0000000067076D22: test        ebx,ebx
  0000000067076D24: jle         0000000067076D78
  0000000067076D26: dec         ebx
  0000000067076D28: test        ebx,ebx
  0000000067076D2A: jle         0000000067076D84
  0000000067076D2C: mov         rax,qword ptr [r12]
  0000000067076D30: movsxd      rdx,ebx
  0000000067076D33: mov         esi,0DC00h
  0000000067076D38: mov         rcx,qword ptr [rax+10h]
  0000000067076D3C: movzx       eax,word ptr [rcx+rdx*2]
  0000000067076D40: mov         ecx,0FC00h
  0000000067076D45: and         ax,cx
  0000000067076D48: cmp         ax,si
  0000000067076D4B: jne         0000000067076D9A
  0000000067076D4D: dec         ebx
  0000000067076D4F: jmp         0000000067076D9A
  0000000067076D51: mov         ebx,ebp
  0000000067076D53: cmp         edi,10000h
  0000000067076D59: jb          0000000067076D90
  0000000067076D5B: inc         ebx
  0000000067076D5D: mov         ecx,0FC00h
  0000000067076D62: mov         esi,0DC00h
  0000000067076D67: jmp         0000000067076D9A
  0000000067076D69: lea         ebx,[rdx+1]
  0000000067076D6C: mov         ecx,0FC00h
  0000000067076D71: mov         esi,0DC00h
  0000000067076D76: jmp         0000000067076D9A
  0000000067076D78: mov         ecx,0FC00h
  0000000067076D7D: mov         esi,0DC00h
  0000000067076D82: jmp         0000000067076D9A
  0000000067076D84: mov         ecx,0FC00h
  0000000067076D89: mov         esi,0DC00h
  0000000067076D8E: jmp         0000000067076D9A
  0000000067076D90: mov         ecx,0FC00h
  0000000067076D95: mov         esi,0DC00h
  0000000067076D9A: cmp         ebx,r13d
  0000000067076D9D: mov         r9d,0D800h
  0000000067076DA3: jl          0000000067076AB0
  0000000067076DA9: mov         rdi,qword ptr [rsp+60h]
  0000000067076DAE: mov         rsi,qword ptr [rsp+58h]
  0000000067076DB3: mov         rbp,qword ptr [rsp+50h]
  0000000067076DB8: mov         r14,qword ptr [rsp+20h]
  0000000067076DBD: add         rsp,28h
  0000000067076DC1: pop         r15
  0000000067076DC3: pop         r13
  0000000067076DC5: pop         r12
  0000000067076DC7: pop         rbx
  0000000067076DC8: ret
  0000000067076DC9: int         3
  0000000067076DCA: int         3
  0000000067076DCB: int         3
  0000000067076DCC: int         3
  0000000067076DCD: int         3
  0000000067076DCE: int         3
  0000000067076DCF: int         3
  0000000067076DD0: mov         qword ptr [rsp+8],rcx
  0000000067076DD5: push        rdi
  0000000067076DD6: sub         rsp,30h
  0000000067076DDA: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  0000000067076DE3: mov         qword ptr [rsp+48h],rbx
  0000000067076DE8: mov         rdi,rdx
  0000000067076DEB: mov         rbx,rcx
  0000000067076DEE: mov         qword ptr [rcx],0
  0000000067076DF5: lea         rax,[00000000672F0B30h]
  0000000067076DFC: mov         qword ptr [rcx+8],rax
  0000000067076E00: lock add    dword ptr [00000000672F0B30h],1
  0000000067076E08: cmp         rcx,rdx
  0000000067076E0B: je          0000000067076E4F
  0000000067076E0D: mov         rax,qword ptr [rdx+8]
  0000000067076E11: lock add    dword ptr [rax],1
  0000000067076E15: mov         rcx,qword ptr [rcx+8]
  0000000067076E19: lock add    dword ptr [rcx],0FFFFFFFFh
  0000000067076E1D: jne         0000000067076E28
  0000000067076E1F: mov         rcx,qword ptr [rbx+8]
  0000000067076E23: call        00000000670700D0
  0000000067076E28: mov         rax,qword ptr [rdi+8]
  0000000067076E2C: mov         qword ptr [rbx+8],rax
  0000000067076E30: mov         eax,dword ptr [rdi+10h]
  0000000067076E33: mov         dword ptr [rbx+10h],eax
  0000000067076E36: lea         rcx,[rbx+18h]
  0000000067076E3A: lea         rdx,[rdi+18h]
  0000000067076E3E: mov         r8d,400h
  0000000067076E44: call        0000000067178A9E
  0000000067076E49: nop
  0000000067076E4A: mov         rax,rbx
  0000000067076E4D: jmp         0000000067076E52
  0000000067076E4F: mov         rax,rbx
  0000000067076E52: mov         rbx,qword ptr [rsp+48h]
  0000000067076E57: add         rsp,30h
  0000000067076E5B: pop         rdi
  0000000067076E5C: ret
  0000000067076E5D: int         3
  0000000067076E5E: int         3
  0000000067076E5F: int         3
  0000000067076E60: push        rbx
  0000000067076E62: sub         rsp,30h
  0000000067076E66: mov         rbx,rcx
  0000000067076E69: test        edx,edx
  0000000067076E6B: js          0000000067076EAC
  0000000067076E6D: mov         rax,qword ptr [rcx]
  0000000067076E70: mov         r10d,dword ptr [rax+8]
  0000000067076E74: cmp         edx,r10d
  0000000067076E77: jg          0000000067076EAC
  0000000067076E79: lea         eax,[rdx+r8]
  0000000067076E7D: cmp         eax,r10d
  0000000067076E80: jle         0000000067076E88
  0000000067076E82: mov         r8d,r10d
  0000000067076E85: sub         r8d,edx
  0000000067076E88: mov         eax,dword ptr [rsp+60h]
  0000000067076E8C: mov         dword ptr [rsp+48h],edx
  0000000067076E90: lea         rdx,[rsp+48h]
  0000000067076E95: mov         dword ptr [rsp+28h],eax
  0000000067076E99: mov         qword ptr [rsp+20h],r9
  0000000067076E9E: mov         r9d,r8d
  0000000067076EA1: mov         r8d,1
  0000000067076EA7: call        0000000067073200
  0000000067076EAC: mov         rax,rbx
  0000000067076EAF: add         rsp,30h
  0000000067076EB3: pop         rbx
  0000000067076EB4: ret
  0000000067076EB5: int         3
  0000000067076EB6: int         3
  0000000067076EB7: int         3
  0000000067076EB8: int         3
  0000000067076EB9: int         3
  0000000067076EBA: int         3
  0000000067076EBB: int         3
  0000000067076EBC: int         3
  0000000067076EBD: int         3
  0000000067076EBE: int         3
  0000000067076EBF: int         3
  0000000067076EC0: mov         word ptr [rsp+20h],r9w
  0000000067076EC6: push        rbx
  0000000067076EC7: sub         rsp,30h
  0000000067076ECB: mov         rbx,rcx
  0000000067076ECE: test        edx,edx
  0000000067076ED0: js          0000000067076F16
  0000000067076ED2: mov         rax,qword ptr [rcx]
  0000000067076ED5: mov         ecx,dword ptr [rax+8]
  0000000067076ED8: cmp         edx,ecx
  0000000067076EDA: jg          0000000067076F16
  0000000067076EDC: lea         eax,[rdx+r8]
  0000000067076EE0: cmp         eax,ecx
  0000000067076EE2: jle         0000000067076EEA
  0000000067076EE4: mov         r8d,ecx
  0000000067076EE7: sub         r8d,edx
  0000000067076EEA: mov         dword ptr [rsp+48h],edx
  0000000067076EEE: lea         rax,[rsp+58h]
  0000000067076EF3: mov         r9d,r8d
  0000000067076EF6: lea         rdx,[rsp+48h]
  0000000067076EFB: mov         r8d,1
  0000000067076F01: mov         rcx,rbx
  0000000067076F04: mov         dword ptr [rsp+28h],1
  0000000067076F0C: mov         qword ptr [rsp+20h],rax
  0000000067076F11: call        0000000067073200
  0000000067076F16: mov         rax,rbx
  0000000067076F19: add         rsp,30h
  0000000067076F1D: pop         rbx
  0000000067076F1E: ret
  0000000067076F1F: int         3
  0000000067076F20: sub         rsp,38h
  0000000067076F24: mov         rax,qword ptr [r8]
  0000000067076F27: mov         r10d,r9d
  0000000067076F2A: mov         r8d,dword ptr [rax+8]
  0000000067076F2E: mov         r9,qword ptr [rax+10h]
  0000000067076F32: mov         rax,qword ptr [rdx]
  0000000067076F35: mov         rdx,qword ptr [rax+10h]
  0000000067076F39: mov         dword ptr [rsp+28h],r10d
  0000000067076F3E: mov         dword ptr [rsp+20h],r8d
  0000000067076F43: mov         r8d,dword ptr [rax+8]
  0000000067076F47: call        00000000670734D0
  0000000067076F4C: add         rsp,38h
  0000000067076F50: ret
  0000000067076F51: int         3
  0000000067076F52: int         3
  0000000067076F53: int         3
  0000000067076F54: int         3
  0000000067076F55: int         3
  0000000067076F56: int         3
  0000000067076F57: int         3
  0000000067076F58: int         3
  0000000067076F59: int         3
  0000000067076F5A: int         3
  0000000067076F5B: int         3
  0000000067076F5C: int         3
  0000000067076F5D: int         3
  0000000067076F5E: int         3
  0000000067076F5F: int         3
  0000000067076F60: mov         dword ptr [rsp+20h],r9d
  0000000067076F65: mov         qword ptr [rsp+18h],r8
  0000000067076F6A: push        rbp
  0000000067076F6B: push        rsi
  0000000067076F6C: push        r12
  0000000067076F6E: push        r13
  0000000067076F70: push        r14
  0000000067076F72: push        r15
  0000000067076F74: mov         eax,1038h
  0000000067076F79: call        0000000067179570
  0000000067076F7E: sub         rsp,rax
  0000000067076F81: mov         r14,qword ptr [r8]
  0000000067076F84: mov         r15d,r9d
  0000000067076F87: mov         rbp,r8
  0000000067076F8A: mov         r13d,dword ptr [r14+8]
  0000000067076F8E: movzx       esi,dx
  0000000067076F91: mov         r12,rcx
  0000000067076F94: test        r13d,r13d
  0000000067076F97: jne         0000000067076FB3
  0000000067076F99: mov         r8d,r9d
  0000000067076F9C: call        0000000067072FD0
  0000000067076FA1: add         rsp,1038h
  0000000067076FA8: pop         r15
  0000000067076FAA: pop         r14
  0000000067076FAC: pop         r13
  0000000067076FAE: pop         r12
  0000000067076FB0: pop         rsi
  0000000067076FB1: pop         rbp
  0000000067076FB2: ret
  0000000067076FB3: cmp         r13d,1
  0000000067076FB7: jne         0000000067076FDB
  0000000067076FB9: mov         rax,qword ptr [r14+10h]
  0000000067076FBD: movzx       edx,si
  0000000067076FC0: movzx       r8d,word ptr [rax]
  0000000067076FC4: call        00000000670736C0
  0000000067076FC9: add         rsp,1038h
  0000000067076FD0: pop         r15
  0000000067076FD2: pop         r14
  0000000067076FD4: pop         r13
  0000000067076FD6: pop         r12
  0000000067076FD8: pop         rsi
  0000000067076FD9: pop         rbp
  0000000067076FDA: ret
  0000000067076FDB: mov         rax,qword ptr [rcx]
  0000000067076FDE: cmp         dword ptr [rax+8],0
  0000000067076FE2: jne         0000000067076FF9
  0000000067076FE4: mov         rax,rcx
  0000000067076FE7: add         rsp,1038h
  0000000067076FEE: pop         r15
  0000000067076FF0: pop         r14
  0000000067076FF2: pop         r13
  0000000067076FF4: pop         r12
  0000000067076FF6: pop         rsi
  0000000067076FF7: pop         rbp
  0000000067076FF8: ret
  0000000067076FF9: mov         qword ptr [rsp+0000000000001070h],rbx
  0000000067077001: lea         r8,[0000000067000000h]
  0000000067077008: mov         qword ptr [rsp+0000000000001030h],rdi
  0000000067077010: cmp         r9d,1
  0000000067077014: je          0000000067077045
  0000000067077016: movzx       edx,si
  0000000067077019: mov         rax,rdx
  000000006707701C: and         edx,1Fh
  000000006707701F: shr         rax,5
  0000000067077023: movzx       ecx,word ptr [r8+rax*2+00000000001F4050h]
  000000006707702C: add         rcx,rdx
  000000006707702F: movzx       eax,word ptr [r8+rcx*2+00000000001F4050h]
  0000000067077038: lea         rcx,[rax+rax*4]
  000000006707703C: add         si,word ptr [r8+rcx*4+00000000001FF7BEh]
  0000000067077045: xor         ebx,ebx
  0000000067077047: nop         word ptr [rax+rax+0000000000000000h]
  0000000067077050: xor         edi,edi
  0000000067077052: movsxd      r10,ebx
  0000000067077055: cmp         r15d,1
  0000000067077059: jne         000000006707709E
  000000006707705B: mov         r8,qword ptr [r12]
  000000006707705F: lea         rcx,[r10+r10]
  0000000067077063: lea         rdx,[rsp+30h]
  0000000067077068: mov         r9d,dword ptr [r8+8]
  000000006707706C: nop         dword ptr [rax]
  0000000067077070: cmp         ebx,r9d
  0000000067077073: jge         0000000067077122
  0000000067077079: mov         rax,qword ptr [r8+10h]
  000000006707707D: cmp         word ptr [rcx+rax],si
  0000000067077081: jne         000000006707708B
  0000000067077083: mov         dword ptr [rdx],ebx
  0000000067077085: inc         edi
  0000000067077087: add         rdx,4
  000000006707708B: inc         ebx
  000000006707708D: add         rcx,2
  0000000067077091: cmp         edi,3FFh
  0000000067077097: jb          0000000067077070
  0000000067077099: jmp         0000000067077122
  000000006707709E: mov         rbp,qword ptr [r12]
  00000000670770A2: add         r10,r10
  00000000670770A5: lea         r9,[rsp+30h]
  00000000670770AA: mov         r11d,dword ptr [rbp+8]
  00000000670770AE: lea         r15,[0000000067000000h]
  00000000670770B5: cmp         ebx,r11d
  00000000670770B8: jge         0000000067077112
  00000000670770BA: mov         rax,qword ptr [rbp+10h]
  00000000670770BE: movzx       r8d,word ptr [r10+rax]
  00000000670770C3: mov         rdx,r8
  00000000670770C6: mov         rax,r8
  00000000670770C9: shr         rax,5
  00000000670770CD: and         edx,1Fh
  00000000670770D0: movzx       ecx,word ptr [r15+rax*2+00000000001F4050h]
  00000000670770D9: add         rcx,rdx
  00000000670770DC: movzx       eax,word ptr [r15+rcx*2+00000000001F4050h]
  00000000670770E5: lea         rcx,[rax+rax*4]
  00000000670770E9: movzx       eax,word ptr [r15+rcx*4+00000000001FF7BEh]
  00000000670770F2: add         ax,r8w
  00000000670770F6: cmp         ax,si
  00000000670770F9: jne         0000000067077104
  00000000670770FB: mov         dword ptr [r9],ebx
  00000000670770FE: inc         edi
  0000000067077100: add         r9,4
  0000000067077104: inc         ebx
  0000000067077106: add         r10,2
  000000006707710A: cmp         edi,3FFh
  0000000067077110: jb          00000000670770B5
  0000000067077112: mov         r15d,dword ptr [rsp+0000000000001088h]
  000000006707711A: mov         rbp,qword ptr [rsp+0000000000001080h]
  0000000067077122: test        edi,edi
  0000000067077124: je          0000000067077165
  0000000067077126: mov         rax,qword ptr [r14+10h]
  000000006707712A: lea         rdx,[rsp+30h]
  000000006707712F: mov         r9d,1
  0000000067077135: mov         r8d,edi
  0000000067077138: mov         rcx,r12
  000000006707713B: mov         dword ptr [rsp+28h],r13d
  0000000067077140: mov         qword ptr [rsp+20h],rax
  0000000067077145: call        0000000067073200
  000000006707714A: cmp         ebx,0FFFFFFFFh
  000000006707714D: je          0000000067077165
  000000006707714F: mov         r14,qword ptr [rbp]
  0000000067077153: mov         r13d,dword ptr [r14+8]
  0000000067077157: lea         eax,[r13-1]
  000000006707715B: imul        eax,edi
  000000006707715E: add         ebx,eax
  0000000067077160: jmp         0000000067077050
  0000000067077165: mov         rdi,qword ptr [rsp+0000000000001030h]
  000000006707716D: mov         rbx,qword ptr [rsp+0000000000001070h]
  0000000067077175: mov         rax,r12
  0000000067077178: add         rsp,1038h
  000000006707717F: pop         r15
  0000000067077181: pop         r14
  0000000067077183: pop         r13
  0000000067077185: pop         r12
  0000000067077187: pop         rsi
  0000000067077188: pop         rbp
  0000000067077189: ret
  000000006707718A: int         3
  000000006707718B: int         3
  000000006707718C: int         3
  000000006707718D: int         3
  000000006707718E: int         3
  000000006707718F: int         3
  0000000067077190: sub         rsp,38h
  0000000067077194: mov         rax,qword ptr [rdx]
  0000000067077197: mov         r10d,r8d
  000000006707719A: mov         r9d,dword ptr [rax+8]
  000000006707719E: mov         r8,qword ptr [rax+10h]
  00000000670771A2: mov         rax,qword ptr [rcx]
  00000000670771A5: mov         edx,dword ptr [rax+8]
  00000000670771A8: mov         rcx,qword ptr [rax+10h]
  00000000670771AC: mov         dword ptr [rsp+20h],r10d
  00000000670771B1: call        0000000067076520
  00000000670771B6: add         rsp,38h
  00000000670771BA: ret
  00000000670771BB: int         3
  00000000670771BC: int         3
  00000000670771BD: int         3
  00000000670771BE: int         3
  00000000670771BF: int         3
  00000000670771C0: sub         rsp,38h
  00000000670771C4: mov         rax,qword ptr [rdx]
  00000000670771C7: mov         r11d,dword ptr [rdx+0Ch]
  00000000670771CB: mov         r9d,r8d
  00000000670771CE: mov         r10,rcx
  00000000670771D1: test        rax,rax
  00000000670771D4: jne         00000000670771DF
  00000000670771D6: mov         r8,qword ptr [00000000672F0B40h]
  00000000670771DD: jmp         00000000670771EE
  00000000670771DF: mov         rax,qword ptr [rax]
  00000000670771E2: movsxd      rdx,dword ptr [rdx+8]
  00000000670771E6: mov         rcx,qword ptr [rax+10h]
  00000000670771EA: lea         r8,[rcx+rdx*2]
  00000000670771EE: mov         rax,qword ptr [r10]
  00000000670771F1: mov         dword ptr [rsp+20h],r9d
  00000000670771F6: mov         r9d,r11d
  00000000670771F9: mov         edx,dword ptr [rax+8]
  00000000670771FC: mov         rcx,qword ptr [rax+10h]
  0000000067077200: call        0000000067076520
  0000000067077205: add         rsp,38h
  0000000067077209: ret
  000000006707720A: int         3
  000000006707720B: int         3
  000000006707720C: int         3
  000000006707720D: int         3
  000000006707720E: int         3
  000000006707720F: int         3
  0000000067077210: mov         qword ptr [rsp+10h],rdx
  0000000067077215: push        rbp
  0000000067077216: push        rsi
  0000000067077217: push        rdi
  0000000067077218: push        r12
  000000006707721A: push        r13
  000000006707721C: push        r14
  000000006707721E: push        r15
  0000000067077220: sub         rsp,30h
  0000000067077224: mov         qword ptr [rsp+20h],0FFFFFFFFFFFFFFFEh
  000000006707722D: mov         qword ptr [rsp+0000000000000088h],rbx
  0000000067077235: mov         r14,rdx
  0000000067077238: mov         r10,rcx
  000000006707723B: mov         dword ptr [rsp+70h],0
  0000000067077243: mov         rax,qword ptr [rcx]
  0000000067077246: movsxd      r8,dword ptr [rax+8]
  000000006707724A: test        r8d,r8d
  000000006707724D: jne         000000006707725E
  000000006707724F: mov         qword ptr [rdx],rax
  0000000067077252: lock add    dword ptr [rax],1
  0000000067077256: mov         rax,rdx
  0000000067077259: jmp         00000000670775CF
  000000006707725E: mov         r9,qword ptr [rax+10h]
  0000000067077262: mov         rsi,r9
  0000000067077265: mov         rax,r8
  0000000067077268: lea         rbp,[r9+r8*2]
  000000006707726C: lea         r15,[0000000067000000h]
  0000000067077273: movzx       eax,word ptr [rsi]
  0000000067077276: lea         ecx,[rax-9]
  0000000067077279: cmp         cx,4
  000000006707727D: jbe         00000000670772C1
  000000006707727F: movzx       ecx,ax
  0000000067077282: shr         rcx,5
  0000000067077286: movzx       edx,word ptr [r15+rcx*2+00000000001F4050h]
  000000006707728F: movzx       ecx,ax
  0000000067077292: and         ecx,1Fh
  0000000067077295: add         rdx,rcx
  0000000067077298: movzx       eax,word ptr [r15+rdx*2+00000000001F4050h]
  00000000670772A1: lea         rax,[rax+rax*4]
  00000000670772A5: movzx       ecx,byte ptr [r15+rax*4+00000000001FF7B0h]
  00000000670772AE: mov         eax,1
  00000000670772B3: shl         eax,cl
  00000000670772B5: test        eax,380h
  00000000670772BA: setne       al
  00000000670772BD: test        al,al
  00000000670772BF: je          00000000670772D0
  00000000670772C1: add         rsi,2
  00000000670772C5: cmp         rsi,rbp
  00000000670772C8: je          00000000670775BA
  00000000670772CE: jmp         0000000067077273
  00000000670772D0: movzx       edx,word ptr [rbp-2]
  00000000670772D4: cmp         dx,9
  00000000670772D8: jb          00000000670772E0
  00000000670772DA: cmp         dx,0Dh
  00000000670772DE: jbe         0000000067077322
  00000000670772E0: movzx       eax,dx
  00000000670772E3: shr         rax,5
  00000000670772E7: movzx       ecx,word ptr [r15+rax*2+00000000001F4050h]
  00000000670772F0: movzx       eax,dx
  00000000670772F3: and         eax,1Fh
  00000000670772F6: add         rcx,rax
  00000000670772F9: movzx       eax,word ptr [r15+rcx*2+00000000001F4050h]
  0000000067077302: lea         rax,[rax+rax*4]
  0000000067077306: movzx       ecx,byte ptr [r15+rax*4+00000000001FF7B0h]
  000000006707730F: mov         eax,1
  0000000067077314: shl         eax,cl
  0000000067077316: test        eax,380h
  000000006707731B: setne       al
  000000006707731E: test        al,al
  0000000067077320: je          0000000067077328
  0000000067077322: add         rbp,0FFFFFFFFFFFFFFFEh
  0000000067077326: jmp         00000000670772D0
  0000000067077328: mov         r13,rsi
  000000006707732B: add         rsi,2
  000000006707732F: cmp         rsi,rbp
  0000000067077332: je          0000000067077597
  0000000067077338: nop         dword ptr [rax+rax+0000000000000000h]
  0000000067077340: movzx       eax,word ptr [rsi]
  0000000067077343: lea         ecx,[rax-9]
  0000000067077346: cmp         cx,4
  000000006707734A: jbe         000000006707738F
  000000006707734C: movzx       ecx,ax
  000000006707734F: shr         rcx,5
  0000000067077353: movzx       edx,word ptr [r15+rcx*2+00000000001F4050h]
  000000006707735C: movzx       ecx,ax
  000000006707735F: and         ecx,1Fh
  0000000067077362: add         rdx,rcx
  0000000067077365: movzx       ecx,word ptr [r15+rdx*2+00000000001F4050h]
  000000006707736E: lea         rcx,[rcx+rcx*4]
  0000000067077372: movzx       ecx,byte ptr [r15+rcx*4+00000000001FF7B0h]
  000000006707737B: mov         edx,1
  0000000067077380: shl         edx,cl
  0000000067077382: test        edx,380h
  0000000067077388: setne       cl
  000000006707738B: test        cl,cl
  000000006707738D: je          00000000670773E7
  000000006707738F: cmp         ax,20h
  0000000067077393: jne         0000000067077407
  0000000067077395: add         rsi,2
  0000000067077399: movzx       eax,word ptr [rsi]
  000000006707739C: lea         ecx,[rax-9]
  000000006707739F: cmp         cx,4
  00000000670773A3: jbe         00000000670773F9
  00000000670773A5: movzx       ecx,ax
  00000000670773A8: shr         rcx,5
  00000000670773AC: movzx       edx,word ptr [r15+rcx*2+00000000001F4050h]
  00000000670773B5: movzx       ecx,ax
  00000000670773B8: and         ecx,1Fh
  00000000670773BB: add         rdx,rcx
  00000000670773BE: movzx       eax,word ptr [r15+rdx*2+00000000001F4050h]
  00000000670773C7: lea         rax,[rax+rax*4]
  00000000670773CB: movzx       ecx,byte ptr [r15+rax*4+00000000001FF7B0h]
  00000000670773D4: mov         eax,1
  00000000670773D9: shl         eax,cl
  00000000670773DB: test        eax,380h
  00000000670773E0: setne       al
  00000000670773E3: test        al,al
  00000000670773E5: jne         00000000670773F9
  00000000670773E7: add         rsi,2
  00000000670773EB: cmp         rsi,rbp
  00000000670773EE: jne         0000000067077340
  00000000670773F4: jmp         0000000067077597
  00000000670773F9: mov         r12,rsi
  00000000670773FC: sub         r12,r13
  00000000670773FF: sar         r12,1
  0000000067077402: dec         r12d
  0000000067077405: jmp         0000000067077410
  0000000067077407: mov         r12,rsi
  000000006707740A: sub         r12,r13
  000000006707740D: sar         r12,1
  0000000067077410: mov         rdx,rbp
  0000000067077413: sub         rdx,rsi
  0000000067077416: sar         rdx,1
  0000000067077419: add         edx,r12d
  000000006707741C: xor         r8d,r8d
  000000006707741F: lea         rcx,[rsp+0000000000000080h]
  0000000067077427: call        000000006706B670
  000000006707742C: nop
  000000006707742D: mov         rbx,qword ptr [rsp+0000000000000080h]
  0000000067077435: mov         rdi,qword ptr [rbx+10h]
  0000000067077439: lea         eax,[r12+r12]
  000000006707743D: movsxd      r8,eax
  0000000067077440: mov         rdx,r13
  0000000067077443: mov         rcx,rdi
  0000000067077446: call        0000000067178A9E
  000000006707744B: movsxd      r11,r12d
  000000006707744E: lea         r9,[rdi+r11*2]
  0000000067077452: sub         rbp,2
  0000000067077456: mov         r10d,20h
  000000006707745C: nop         dword ptr [rax]
  0000000067077460: mov         word ptr [r9],r10w
  0000000067077464: add         r9,2
  0000000067077468: nop         dword ptr [rax+rax+0000000000000000h]
  0000000067077470: add         rsi,2
  0000000067077474: movzx       eax,word ptr [rsi]
  0000000067077477: lea         ecx,[rax-9]
  000000006707747A: cmp         cx,4
  000000006707747E: jbe         0000000067077470
  0000000067077480: movzx       ecx,ax
  0000000067077483: shr         rcx,5
  0000000067077487: movzx       edx,word ptr [r15+rcx*2+00000000001F4050h]
  0000000067077490: movzx       ecx,ax
  0000000067077493: and         ecx,1Fh
  0000000067077496: add         rdx,rcx
  0000000067077499: movzx       ecx,word ptr [r15+rdx*2+00000000001F4050h]
  00000000670774A2: lea         rcx,[rcx+rcx*4]
  00000000670774A6: movzx       ecx,byte ptr [r15+rcx*4+00000000001FF7B0h]
  00000000670774AF: mov         edx,1
  00000000670774B4: shl         edx,cl
  00000000670774B6: test        edx,380h
  00000000670774BC: setne       cl
  00000000670774BF: test        cl,cl
  00000000670774C1: jne         0000000067077470
  00000000670774C3: cmp         rsi,rbp
  00000000670774C6: je          000000006707753B
  00000000670774C8: nop         dword ptr [rax+rax+0000000000000000h]
  00000000670774D0: mov         word ptr [r9],ax
  00000000670774D4: add         r9,2
  00000000670774D8: add         rsi,2
  00000000670774DC: mov         ax,word ptr [rsi]
  00000000670774DF: cmp         rsi,rbp
  00000000670774E2: je          000000006707753B
  00000000670774E4: lea         ecx,[rax-9]
  00000000670774E7: cmp         cx,4
  00000000670774EB: jbe         0000000067077460
  00000000670774F1: movzx       r8d,ax
  00000000670774F5: mov         rcx,r8
  00000000670774F8: shr         rcx,5
  00000000670774FC: movzx       edx,word ptr [r15+rcx*2+00000000001F4050h]
  0000000067077505: and         r8d,1Fh
  0000000067077509: add         rdx,r8
  000000006707750C: movzx       ecx,word ptr [r15+rdx*2+00000000001F4050h]
  0000000067077515: lea         rdx,[rcx+rcx*4]
  0000000067077519: movzx       ecx,byte ptr [r15+rdx*4+00000000001FF7B0h]
  0000000067077522: mov         edx,1
  0000000067077527: shl         edx,cl
  0000000067077529: test        edx,380h
  000000006707752F: setne       cl
  0000000067077532: test        cl,cl
  0000000067077534: je          00000000670774D0
  0000000067077536: jmp         0000000067077460
  000000006707753B: mov         word ptr [r9],ax
  000000006707753F: add         r9,2
  0000000067077543: sub         r9,qword ptr [rbx+10h]
  0000000067077547: sar         r9,1
  000000006707754A: cmp         r9d,dword ptr [rbx+8]
  000000006707754E: jge         0000000067077568
  0000000067077550: mov         edx,r9d
  0000000067077553: lea         rcx,[rsp+0000000000000080h]
  000000006707755B: call        00000000670729C0
  0000000067077560: mov         rbx,qword ptr [rsp+0000000000000080h]
  0000000067077568: mov         qword ptr [r14],rbx
  000000006707756B: lock add    dword ptr [rbx],1
  000000006707756F: mov         dword ptr [rsp+70h],1
  0000000067077577: mov         rax,qword ptr [rsp+0000000000000080h]
  000000006707757F: lock add    dword ptr [rax],0FFFFFFFFh
  0000000067077583: jne         0000000067077592
  0000000067077585: mov         rcx,qword ptr [rsp+0000000000000080h]
  000000006707758D: call        00000000670700D0
  0000000067077592: mov         rax,r14
  0000000067077595: jmp         00000000670775CF
  0000000067077597: sub         rsi,r13
  000000006707759A: sar         rsi,1
  000000006707759D: sub         r13,r9
  00000000670775A0: sar         r13,1
  00000000670775A3: mov         r9d,esi
  00000000670775A6: mov         r8d,r13d
  00000000670775A9: mov         rdx,r14
  00000000670775AC: mov         rcx,r10
  00000000670775AF: call        0000000067073FA0
  00000000670775B4: nop
  00000000670775B5: mov         rax,r14
  00000000670775B8: jmp         00000000670775CF
  00000000670775BA: lock add    dword ptr [00000000672F0B50h],1
  00000000670775C2: lea         rax,[00000000672F0B50h]
  00000000670775C9: mov         qword ptr [r14],rax
  00000000670775CC: mov         rax,r14
  00000000670775CF: mov         rbx,qword ptr [rsp+0000000000000088h]
  00000000670775D7: add         rsp,30h
  00000000670775DB: pop         r15
  00000000670775DD: pop         r14
  00000000670775DF: pop         r13
  00000000670775E1: pop         r12
  00000000670775E3: pop         rdi
  00000000670775E4: pop         rsi
  00000000670775E5: pop         rbp
  00000000670775E6: ret
  00000000670775E7: int         3
  00000000670775E8: int         3
  00000000670775E9: int         3
  00000000670775EA: int         3
  00000000670775EB: int         3
  00000000670775EC: int         3
  00000000670775ED: int         3
  00000000670775EE: int         3
  00000000670775EF: int         3
  00000000670775F0: 48

  Summary

      1A7000 .text

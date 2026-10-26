140919de0: mov         byte ptr [rsp+20h],r9b
140919de5: mov         dword ptr [rsp+18h],r8d
140919dea: mov         qword ptr [rsp+10h],rdx
140919def: mov         qword ptr [rsp+8],rcx
140919df4: sub         rsp,0C8h
140919dfb: mov         qword ptr [rsp+090],0FFFFFFFFFFFFFFFEh
140919e07: mov         qword ptr [rsp+20h],0
140919e10: lea         rax,[rsp+30h]
140919e15: mov         qword ptr [rsp+38h],rax
140919e1a: mov         rdx,qword ptr [rsp+0D8]
140919e22: mov         rcx,qword ptr [rsp+38h]
140919e27: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140919e2d: mov         qword ptr [rsp+098],rax
140919e35: mov         rax,qword ptr [rsp+0D0]
140919e3d: mov         rax,qword ptr [rax]
140919e40: mov         rdx,qword ptr [rsp+098]
140919e48: mov         rcx,qword ptr [rsp+0D0]
140919e50: call        qword ptr [rax+0000000000000130h]
140919e56: movzx       eax,al
140919e59: test        eax,eax
140919e5b: jne         140919FEB
140919e61: mov         ecx,48h
140919e66: call        14146EABC
140919e6b: mov         qword ptr [rsp+48h],rax
140919e70: cmp         qword ptr [rsp+48h],0
140919e76: je          140919E93
140919e78: mov         edx,dword ptr [rsp+0E0]
140919e7f: mov         rcx,qword ptr [rsp+48h]
140919e84: call        140816900
140919e89: mov         qword ptr [rsp+0A0],rax
140919e91: jmp         140919E9F
140919e93: mov         qword ptr [rsp+0A0],0
140919e9f: mov         rax,qword ptr [rsp+0A0]
140919ea7: mov         qword ptr [rsp+40h],rax
140919eac: mov         rax,qword ptr [rsp+40h]
140919eb1: mov         qword ptr [rsp+20h],rax
140919eb6: mov         rcx,qword ptr [rsp+0D0]
140919ebe: add         rcx,38h
140919ec2: lea         rdx,[rsp+20h]
140919ec7: call        14172D7E0
140919ecc: lea         rax,[rsp+50h]
140919ed1: mov         qword ptr [rsp+58h],rax
140919ed6: mov         rdx,qword ptr [rsp+0D8]
140919ede: mov         rcx,qword ptr [rsp+58h]
140919ee3: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140919ee9: mov         qword ptr [rsp+0A8],rax
140919ef1: mov         rdx,qword ptr [rsp+0A8]
140919ef9: mov         rcx,qword ptr [rsp+20h]
140919efe: call        140DFBD00
140919f03: mov         rax,qword ptr [rsp+0D0]
140919f0b: mov         rax,qword ptr [rax]
140919f0e: mov         rcx,qword ptr [rsp+0D0]
140919f16: call        qword ptr [rax+00000000000001E8h]
140919f1c: mov         dword ptr [rsp+2Ch],eax
140919f20: movzx       eax,byte ptr [rsp+0E8]
140919f28: test        eax,eax
140919f2a: je          140919F3A
140919f2c: mov         edx,dword ptr [rsp+2Ch]
140919f30: mov         rcx,qword ptr [rsp+20h]
140919f35: call        1409FF590
140919f3a: lea         rax,[rsp+60h]
140919f3f: mov         qword ptr [rsp+68h],rax
140919f44: mov         rdx,qword ptr [rsp+0D8]
140919f4c: mov         rcx,qword ptr [rsp+68h]
140919f51: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
140919f57: mov         qword ptr [rsp+0B0],rax
140919f5f: mov         rax,qword ptr [rsp+0D0]
140919f67: mov         rax,qword ptr [rax]
140919f6a: mov         rdx,qword ptr [rsp+0B0]
140919f72: mov         rcx,qword ptr [rsp+0D0]
140919f7a: call        qword ptr [rax+0000000000000180h]
140919f80: mov         r11d,eax
140919f83: mov         rax,qword ptr [rsp+0D0]
140919f8b: mov         dword ptr [rax+74h],r11d
140919f8f: mov         dword ptr [rsp+28h],0
140919f97: jmp         140919FA4
140919f99: mov         eax,dword ptr [rsp+28h]
140919f9d: add         eax,1
140919fa0: mov         dword ptr [rsp+28h],eax
140919fa4: cmp         dword ptr [rsp+28h],3
140919fa9: jge         140919FBF
140919fab: movsxd      rcx,dword ptr [rsp+28h]
140919fb0: mov         rax,qword ptr [rsp+0D0]
140919fb8: mov         byte ptr [rax+rcx+70h],1
140919fbd: jmp         140919F99
140919fbf: mov         rax,qword ptr [rsp+0D0]
140919fc7: cmp         dword ptr [rax+40h],2
140919fcb: jne         140919FE9
140919fcd: lea         rdx,[rsp+70h]
140919fd2: mov         rcx,qword ptr [rsp+0D0]
140919fda: call        14091D110
140919fdf: lea         rcx,[rsp+70h]
140919fe4: call        1400BC700
140919fe9: jmp         14091A052
140919feb: lea         rax,[rsp+78h]
140919ff0: mov         qword ptr [rsp+080],rax
140919ff8: mov         rdx,qword ptr [rsp+0D8]
14091a000: mov         rcx,qword ptr [rsp+080]
14091a008: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
14091a00e: mov         qword ptr [rsp+0B8],rax
14091a016: mov         rax,qword ptr [rsp+0D0]
14091a01e: mov         rax,qword ptr [rax]
14091a021: mov         rdx,qword ptr [rsp+0B8]
14091a029: mov         rcx,qword ptr [rsp+0D0]
14091a031: call        qword ptr [rax+0000000000000180h]
14091a037: mov         rcx,qword ptr [rsp+0D0]
14091a03f: add         rcx,38h
14091a043: mov         edx,eax
14091a045: call        140160C10
14091a04a: mov         rax,qword ptr [rax]
14091a04d: mov         qword ptr [rsp+20h],rax
14091a052: mov         rax,qword ptr [rsp+20h]
14091a057: mov         qword ptr [rsp+088],rax
14091a05f: mov         rcx,qword ptr [rsp+0D8]
14091a067: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091a06d: mov         rax,qword ptr [rsp+088]
14091a075: add         rsp,0C8h
14091a07c: ret
1400d25a0: mov         qword ptr [rsp+20h],r9
1400d25a5: mov         dword ptr [rsp+18h],r8d
1400d25aa: mov         qword ptr [rsp+10h],rdx
1400d25af: mov         qword ptr [rsp+8],rcx
1400d25b4: push        rsi
1400d25b5: push        rdi
1400d25b6: sub         rsp,258h
1400d25bd: mov         qword ptr [rsp+220],0FFFFFFFFFFFFFFFEh
1400d25c9: lea         rax,[rsp+68h]
1400d25ce: mov         qword ptr [rsp+70h],rax
1400d25d3: mov         rdx,qword ptr [rsp+278]
1400d25db: mov         rcx,qword ptr [rsp+70h]
1400d25e0: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
1400d25e6: mov         qword ptr [rsp+228],rax
1400d25ee: mov         rax,qword ptr [rsp+270]
1400d25f6: mov         rax,qword ptr [rax]
1400d25f9: mov         rdx,qword ptr [rsp+228]
1400d2601: mov         rcx,qword ptr [rsp+270]
1400d2609: call        qword ptr [rax+0000000000000130h]
1400d260f: movzx       eax,al
1400d2612: test        eax,eax
1400d2614: je          1400D299F
1400d261a: lea         rax,[rsp+78h]
1400d261f: mov         qword ptr [rsp+080],rax
1400d2627: lea         rax,[rsp+088]
1400d262f: mov         qword ptr [rsp+090],rax
1400d2637: mov         rdx,qword ptr [rsp+288]
1400d263f: mov         rcx,qword ptr [rsp+080]
1400d2647: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
1400d264d: mov         qword ptr [rsp+230],rax
1400d2655: mov         rax,qword ptr [rsp+230]
1400d265d: mov         qword ptr [rsp+238],rax
1400d2665: mov         rdx,qword ptr [rsp+278]
1400d266d: mov         rcx,qword ptr [rsp+090]
1400d2675: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
1400d267b: mov         qword ptr [rsp+240],rax
1400d2683: mov         rax,qword ptr [rsp+2B0]
1400d268b: mov         qword ptr [rsp+40h],rax
1400d2690: mov         rax,qword ptr [rsp+2A8]
1400d2698: mov         qword ptr [rsp+38h],rax
1400d269d: movsd       xmm0,mmword ptr [rsp+2A0]
1400d26a6: movsd       mmword ptr [rsp+30h],xmm0
1400d26ac: mov         eax,dword ptr [rsp+298]
1400d26b3: mov         dword ptr [rsp+28h],eax
1400d26b7: mov         eax,dword ptr [rsp+290]
1400d26be: mov         dword ptr [rsp+20h],eax
1400d26c2: mov         r9,qword ptr [rsp+238]
1400d26ca: mov         r8d,dword ptr [rsp+280]
1400d26d2: mov         rdx,qword ptr [rsp+240]
1400d26da: mov         rcx,qword ptr [rsp+270]
1400d26e2: call        14091A5B0
1400d26e7: mov         rcx,qword ptr [rsp+2A8]
1400d26ef: call        140004AE0
1400d26f4: mov         dword ptr [rsp+64h],eax
1400d26f8: mov         dword ptr [rsp+60h],0
1400d2700: jmp         1400D270D
1400d2702: mov         eax,dword ptr [rsp+60h]
1400d2706: add         eax,1
1400d2709: mov         dword ptr [rsp+60h],eax
1400d270d: mov         eax,dword ptr [rsp+64h]
1400d2711: cmp         dword ptr [rsp+60h],eax
1400d2715: jge         1400D299F
1400d271b: mov         byte ptr [rsp+138],1
1400d2723: lea         r8,[rsp+138]
1400d272b: lea         rdx,[0000000142166B34h]
1400d2732: lea         rcx,[rsp+128]
1400d273a: call        140025080
1400d273f: lea         rdi,[rsp+210]
1400d2747: mov         rsi,rax
1400d274a: mov         ecx,10h
1400d274f: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d2751: lea         rdi,[rsp+118]
1400d2759: lea         rsi,[rsp+210]
1400d2761: mov         ecx,10h
1400d2766: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d2768: mov         edx,dword ptr [rsp+60h]
1400d276c: mov         rcx,qword ptr [rsp+2A8]
1400d2774: call        1400250C0
1400d2779: mov         r8,rax
1400d277c: lea         rdx,[0000000142163514h]
1400d2783: lea         rcx,[rsp+150]
1400d278b: call        140025080
1400d2790: lea         rdi,[rsp+200]
1400d2798: mov         rsi,rax
1400d279b: mov         ecx,10h
1400d27a0: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d27a2: lea         rdi,[rsp+140]
1400d27aa: lea         rsi,[rsp+200]
1400d27b2: mov         ecx,10h
1400d27b7: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d27b9: xor         r8d,r8d
1400d27bc: xor         edx,edx
1400d27be: lea         rcx,[rsp+098]
1400d27c6: call        qword ptr [141E04168h]
1400d27cc: lea         rdi,[rsp+1F0]
1400d27d4: mov         rsi,rax
1400d27d7: mov         ecx,10h
1400d27dc: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d27de: xor         r8d,r8d
1400d27e1: xor         edx,edx
1400d27e3: lea         rcx,[rsp+0A8]
1400d27eb: call        qword ptr [141E04168h]
1400d27f1: lea         rdi,[rsp+1E0]
1400d27f9: mov         rsi,rax
1400d27fc: mov         ecx,10h
1400d2801: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d2803: xor         r8d,r8d
1400d2806: xor         edx,edx
1400d2808: lea         rcx,[rsp+0B8]
1400d2810: call        qword ptr [141E04168h]
1400d2816: lea         rdi,[rsp+1D0]
1400d281e: mov         rsi,rax
1400d2821: mov         ecx,10h
1400d2826: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d2828: xor         r8d,r8d
1400d282b: xor         edx,edx
1400d282d: lea         rcx,[rsp+0C8]
1400d2835: call        qword ptr [141E04168h]
1400d283b: lea         rdi,[rsp+1C0]
1400d2843: mov         rsi,rax
1400d2846: mov         ecx,10h
1400d284b: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d284d: xor         r8d,r8d
1400d2850: xor         edx,edx
1400d2852: lea         rcx,[rsp+0D8]
1400d285a: call        qword ptr [141E04168h]
1400d2860: lea         rdi,[rsp+1B0]
1400d2868: mov         rsi,rax
1400d286b: mov         ecx,10h
1400d2870: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d2872: xor         r8d,r8d
1400d2875: xor         edx,edx
1400d2877: lea         rcx,[rsp+0E8]
1400d287f: call        qword ptr [141E04168h]
1400d2885: lea         rdi,[rsp+1A0]
1400d288d: mov         rsi,rax
1400d2890: mov         ecx,10h
1400d2895: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d2897: xor         r8d,r8d
1400d289a: xor         edx,edx
1400d289c: lea         rcx,[rsp+0F8]
1400d28a4: call        qword ptr [141E04168h]
1400d28aa: lea         rdi,[rsp+190]
1400d28b2: mov         rsi,rax
1400d28b5: mov         ecx,10h
1400d28ba: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d28bc: xor         r8d,r8d
1400d28bf: xor         edx,edx
1400d28c1: lea         rcx,[rsp+108]
1400d28c9: call        qword ptr [141E04168h]
1400d28cf: lea         rdi,[rsp+180]
1400d28d7: mov         rsi,rax
1400d28da: mov         ecx,10h
1400d28df: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d28e1: lea         rdi,[rsp+170]
1400d28e9: lea         rsi,[rsp+118]
1400d28f1: mov         ecx,10h
1400d28f6: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d28f8: lea         rdi,[rsp+160]
1400d2900: lea         rsi,[rsp+140]
1400d2908: mov         ecx,10h
1400d290d: rep movs    byte ptr [rdi],byte ptr [rsi]
1400d290f: lea         rax,[rsp+1F0]
1400d2917: mov         qword ptr [rsp+58h],rax
1400d291c: lea         rax,[rsp+1E0]
1400d2924: mov         qword ptr [rsp+50h],rax
1400d2929: lea         rax,[rsp+1D0]
1400d2931: mov         qword ptr [rsp+48h],rax
1400d2936: lea         rax,[rsp+1C0]
1400d293e: mov         qword ptr [rsp+40h],rax
1400d2943: lea         rax,[rsp+1B0]
1400d294b: mov         qword ptr [rsp+38h],rax
1400d2950: lea         rax,[rsp+1A0]
1400d2958: mov         qword ptr [rsp+30h],rax
1400d295d: lea         rax,[rsp+190]
1400d2965: mov         qword ptr [rsp+28h],rax
1400d296a: lea         rax,[rsp+180]
1400d2972: mov         qword ptr [rsp+20h],rax
1400d2977: lea         r9,[rsp+170]
1400d297f: lea         r8,[rsp+160]
1400d2987: lea         rdx,[0000000142166B40h]
1400d298e: lea         rcx,[0000000143D76700h]
1400d2995: call        1402CF3C0
1400d299a: jmp         1400D2702
1400d299f: mov         rcx,qword ptr [rsp+278]
1400d29a7: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1400d29ad: nop
1400d29ae: mov         rcx,qword ptr [rsp+288]
1400d29b6: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
1400d29bc: add         rsp,258h
1400d29c3: pop         rdi
1400d29c4: pop         rsi
1400d29c5: ret
14091a5b0: mov         qword ptr [rsp+20h],r9
14091a5b5: mov         dword ptr [rsp+18h],r8d
14091a5ba: mov         qword ptr [rsp+10h],rdx
14091a5bf: mov         qword ptr [rsp+8],rcx
14091a5c4: sub         rsp,0A8h
14091a5cb: mov         qword ptr [rsp+78h],0FFFFFFFFFFFFFFFEh
14091a5d4: lea         rax,[rsp+48h]
14091a5d9: mov         qword ptr [rsp+50h],rax
14091a5de: mov         rdx,qword ptr [rsp+0B8]
14091a5e6: mov         rcx,qword ptr [rsp+50h]
14091a5eb: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
14091a5f1: mov         qword ptr [rsp+080],rax
14091a5f9: mov         rax,qword ptr [rsp+0B0]
14091a601: mov         rax,qword ptr [rax]
14091a604: mov         rdx,qword ptr [rsp+080]
14091a60c: mov         rcx,qword ptr [rsp+0B0]
14091a614: call        qword ptr [rax+0000000000000130h]
14091a61a: movzx       eax,al
14091a61d: test        eax,eax
14091a61f: je          14091A701
14091a625: lea         rax,[rsp+58h]
14091a62a: mov         qword ptr [rsp+60h],rax
14091a62f: mov         rdx,qword ptr [rsp+0B8]
14091a637: mov         rcx,qword ptr [rsp+60h]
14091a63c: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
14091a642: mov         qword ptr [rsp+088],rax
14091a64a: mov         rax,qword ptr [rsp+0B0]
14091a652: mov         rax,qword ptr [rax]
14091a655: mov         rdx,qword ptr [rsp+088]
14091a65d: mov         rcx,qword ptr [rsp+0B0]
14091a665: call        qword ptr [rax+0000000000000180h]
14091a66b: mov         rcx,qword ptr [rsp+0B0]
14091a673: add         rcx,38h
14091a677: mov         edx,eax
14091a679: call        140160C10
14091a67e: mov         rax,qword ptr [rax]
14091a681: mov         qword ptr [rsp+40h],rax
14091a686: lea         rax,[rsp+68h]
14091a68b: mov         qword ptr [rsp+70h],rax
14091a690: mov         rdx,qword ptr [rsp+0C8]
14091a698: mov         rcx,qword ptr [rsp+70h]
14091a69d: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
14091a6a3: mov         qword ptr [rsp+090],rax
14091a6ab: mov         rax,qword ptr [rsp+0F0]
14091a6b3: mov         qword ptr [rsp+38h],rax
14091a6b8: mov         rax,qword ptr [rsp+0E8]
14091a6c0: mov         qword ptr [rsp+30h],rax
14091a6c5: movsd       xmm0,mmword ptr [rsp+0E0]
14091a6ce: movsd       mmword ptr [rsp+28h],xmm0
14091a6d4: mov         eax,dword ptr [rsp+0D8]
14091a6db: mov         dword ptr [rsp+20h],eax
14091a6df: mov         r9d,dword ptr [rsp+0D0]
14091a6e7: mov         r8,qword ptr [rsp+090]
14091a6ef: mov         edx,dword ptr [rsp+0C0]
14091a6f6: mov         rcx,qword ptr [rsp+40h]
14091a6fb: call        140816B40
14091a700: nop
14091a701: mov         rcx,qword ptr [rsp+0B8]
14091a709: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091a70f: nop
14091a710: mov         rcx,qword ptr [rsp+0C8]
14091a718: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091a71e: add         rsp,0A8h
14091a725: ret
14091a950: mov         qword ptr [rsp+10h],rdx
14091a955: mov         qword ptr [rsp+8],rcx
14091a95a: sub         rsp,78h
14091a95e: mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
14091a967: mov         dword ptr [rsp+20h],0
14091a96f: lea         rax,[rsp+30h]
14091a974: mov         qword ptr [rsp+38h],rax
14091a979: mov         rdx,qword ptr [rsp+088]
14091a981: mov         rcx,qword ptr [rsp+38h]
14091a986: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
14091a98c: mov         qword ptr [rsp+60h],rax
14091a991: mov         rax,qword ptr [rsp+080]
14091a999: mov         rax,qword ptr [rax]
14091a99c: mov         rdx,qword ptr [rsp+60h]
14091a9a1: mov         rcx,qword ptr [rsp+080]
14091a9a9: call        qword ptr [rax+0000000000000130h]
14091a9af: movzx       eax,al
14091a9b2: test        eax,eax
14091a9b4: je          14091AA1F
14091a9b6: lea         rax,[rsp+40h]
14091a9bb: mov         qword ptr [rsp+48h],rax
14091a9c0: mov         rdx,qword ptr [rsp+088]
14091a9c8: mov         rcx,qword ptr [rsp+48h]
14091a9cd: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
14091a9d3: mov         qword ptr [rsp+68h],rax
14091a9d8: mov         rax,qword ptr [rsp+080]
14091a9e0: mov         rax,qword ptr [rax]
14091a9e3: mov         rdx,qword ptr [rsp+68h]
14091a9e8: mov         rcx,qword ptr [rsp+080]
14091a9f0: call        qword ptr [rax+0000000000000180h]
14091a9f6: mov         rcx,qword ptr [rsp+080]
14091a9fe: add         rcx,38h
14091aa02: mov         edx,eax
14091aa04: call        140160C10
14091aa09: mov         rax,qword ptr [rax]
14091aa0c: mov         qword ptr [rsp+28h],rax
14091aa11: mov         rcx,qword ptr [rsp+28h]
14091aa16: call        1408171B0
14091aa1b: mov         dword ptr [rsp+20h],eax
14091aa1f: mov         eax,dword ptr [rsp+20h]
14091aa23: mov         dword ptr [rsp+50h],eax
14091aa27: mov         rcx,qword ptr [rsp+088]
14091aa2f: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091aa35: mov         eax,dword ptr [rsp+50h]
14091aa39: add         rsp,78h
14091aa3d: ret
14091aa50: mov         dword ptr [rsp+18h],r8d
14091aa55: mov         qword ptr [rsp+10h],rdx
14091aa5a: mov         qword ptr [rsp+8],rcx
14091aa5f: sub         rsp,78h
14091aa63: mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
14091aa6c: mov         qword ptr [rsp+20h],0
14091aa75: lea         rax,[rsp+30h]
14091aa7a: mov         qword ptr [rsp+38h],rax
14091aa7f: mov         rdx,qword ptr [rsp+088]
14091aa87: mov         rcx,qword ptr [rsp+38h]
14091aa8c: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
14091aa92: mov         qword ptr [rsp+60h],rax
14091aa97: mov         rax,qword ptr [rsp+080]
14091aa9f: mov         rax,qword ptr [rax]
14091aaa2: mov         rdx,qword ptr [rsp+60h]
14091aaa7: mov         rcx,qword ptr [rsp+080]
14091aaaf: call        qword ptr [rax+0000000000000130h]
14091aab5: movzx       eax,al
14091aab8: test        eax,eax
14091aaba: je          14091AB34
14091aabc: lea         rax,[rsp+40h]
14091aac1: mov         qword ptr [rsp+48h],rax
14091aac6: mov         rdx,qword ptr [rsp+088]
14091aace: mov         rcx,qword ptr [rsp+48h]
14091aad3: call        qword ptr [141E059D8h] ; QtCore4_64.dll!??0QString@@QEAA@AEBV0@@Z
14091aad9: mov         qword ptr [rsp+68h],rax
14091aade: mov         rax,qword ptr [rsp+080]
14091aae6: mov         rax,qword ptr [rax]
14091aae9: mov         rdx,qword ptr [rsp+68h]
14091aaee: mov         rcx,qword ptr [rsp+080]
14091aaf6: call        qword ptr [rax+0000000000000180h]
14091aafc: mov         rcx,qword ptr [rsp+080]
14091ab04: add         rcx,38h
14091ab08: mov         edx,eax
14091ab0a: call        140160C10
14091ab0f: mov         rax,qword ptr [rax]
14091ab12: mov         qword ptr [rsp+28h],rax
14091ab17: mov         rcx,qword ptr [rsp+28h]
14091ab1c: add         rcx,30h
14091ab20: mov         edx,dword ptr [rsp+090]
14091ab27: call        140160C10
14091ab2c: mov         rax,qword ptr [rax]
14091ab2f: mov         qword ptr [rsp+20h],rax
14091ab34: mov         rax,qword ptr [rsp+20h]
14091ab39: mov         qword ptr [rsp+50h],rax
14091ab3e: mov         rcx,qword ptr [rsp+088]
14091ab46: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
14091ab4c: mov         rax,qword ptr [rsp+50h]
14091ab51: add         rsp,78h
14091ab55: ret
14091c7a0: mov         qword ptr [rsp+8],rcx
14091c7a5: sub         rsp,48h
14091c7a9: mov         qword ptr [rsp+30h],0FFFFFFFFFFFFFFFEh
14091c7b2: mov         rdx,qword ptr [rsp+50h]
14091c7b7: add         rdx,38h
14091c7bb: lea         rcx,[rsp+28h]
14091c7c0: call        140259AD0
14091c7c5: nop
14091c7c6: mov         dword ptr [rsp+20h],0
14091c7ce: jmp         14091C7DB
14091c7d0: mov         eax,dword ptr [rsp+20h]
14091c7d4: add         eax,1
14091c7d7: mov         dword ptr [rsp+20h],eax
14091c7db: lea         rcx,[rsp+28h]
14091c7e0: call        140004AE0
14091c7e5: cmp         dword ptr [rsp+20h],eax
14091c7e9: jge         14091C805
14091c7eb: mov         edx,dword ptr [rsp+20h]
14091c7ef: lea         rcx,[rsp+28h]
14091c7f4: call        140160C10
14091c7f9: mov         rcx,qword ptr [rax]
14091c7fc: mov         eax,dword ptr [rsp+20h]
14091c800: mov         dword ptr [rcx+18h],eax
14091c803: jmp         14091C7D0
14091c805: lea         rcx,[rsp+28h]
14091c80a: call        1402081A0
14091c80f: add         rsp,48h
14091c813: ret
140921e90: mov         qword ptr [rsp+8],rcx
140921e95: sub         rsp,38h
140921e99: mov         rcx,qword ptr [rsp+40h]
140921e9e: call        1409220E0
140921ea3: mov         qword ptr [rsp+20h],rax
140921ea8: cmp         qword ptr [rsp+20h],0
140921eae: je          140921EBB
140921eb0: mov         rax,qword ptr [rsp+20h]
140921eb5: mov         eax,dword ptr [rax]
140921eb7: jmp         140921EC0
140921eb9: jmp         140921EC0
140921ebb: mov         eax,131h
140921ec0: add         rsp,38h
140921ec4: ret
140921980: mov         qword ptr [rsp+10h],rdx
140921985: mov         qword ptr [rsp+8],rcx
14092198a: sub         rsp,98h
140921991: mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
14092199a: mov         rcx,qword ptr [rsp+0A0]
1409219a2: add         rcx,10h
1409219a6: mov         rdx,qword ptr [rsp+0A8]
1409219ae: call        qword ptr [141E04470h] ; QtCore4_64.dll!??4QString@@QEAAAEAV0@AEBV0@@Z
1409219b4: mov         edx,0Dh
1409219b9: lea         rcx,[rsp+20h]
1409219be: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
1409219c4: mov         rcx,qword ptr [rsp+0A0]
1409219cc: add         rcx,10h
1409219d0: mov         r8d,1
1409219d6: movzx       edx,word ptr [rax]
1409219d9: call        qword ptr [141E04EE0h] ; QtCore4_64.dll!?remove@QString@@QEAAAEAV1@VQChar@@W4CaseSensitivity@Qt@@@Z
1409219df: mov         edx,0Ah
1409219e4: lea         rcx,[rsp+30h]
1409219e9: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
1409219ef: movzx       edx,word ptr [rax]
1409219f2: lea         rcx,[rsp+28h]
1409219f7: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
1409219fd: mov         qword ptr [rsp+60h],rax
140921a02: mov         rax,qword ptr [rsp+60h]
140921a07: mov         qword ptr [rsp+68h],rax
140921a0c: mov         edx,0Dh
140921a11: lea         rcx,[rsp+40h]
140921a16: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140921a1c: movzx       edx,word ptr [rax]
140921a1f: lea         rcx,[rsp+38h]
140921a24: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140921a2a: mov         qword ptr [rsp+70h],rax
140921a2f: mov         rax,qword ptr [rsp+70h]
140921a34: mov         qword ptr [rsp+78h],rax
140921a39: mov         r8,qword ptr [rsp+68h]
140921a3e: mov         rdx,qword ptr [rsp+78h]
140921a43: lea         rcx,[rsp+48h]
140921a48: call        1400048F0
140921a4d: mov         qword ptr [rsp+080],rax
140921a55: mov         rax,qword ptr [rsp+080]
140921a5d: mov         qword ptr [rsp+088],rax
140921a65: mov         edx,0Ah
140921a6a: lea         rcx,[rsp+50h]
140921a6f: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140921a75: mov         rcx,qword ptr [rsp+0A0]
140921a7d: add         rcx,10h
140921a81: mov         r9d,1
140921a87: mov         r8,qword ptr [rsp+088]
140921a8f: movzx       edx,word ptr [rax]
140921a92: call        qword ptr [141E04F40h] ; QtCore4_64.dll!?replace@QString@@QEAAAEAV1@VQChar@@AEBV1@W4CaseSensitivity@Qt@@@Z
140921a98: nop
140921a99: lea         rcx,[rsp+48h]
140921a9e: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921aa4: nop
140921aa5: lea         rcx,[rsp+38h]
140921aaa: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921ab0: nop
140921ab1: lea         rcx,[rsp+28h]
140921ab6: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921abc: nop
140921abd: mov         rcx,qword ptr [rsp+0A8]
140921ac5: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921acb: add         rsp,98h
140921ad2: ret
140921ae0: mov         qword ptr [rsp+10h],rdx
140921ae5: mov         qword ptr [rsp+8],rcx
140921aea: sub         rsp,98h
140921af1: mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
140921afa: mov         rcx,qword ptr [rsp+0A0]
140921b02: add         rcx,18h
140921b06: mov         rdx,qword ptr [rsp+0A8]
140921b0e: call        qword ptr [141E04470h] ; QtCore4_64.dll!??4QString@@QEAAAEAV0@AEBV0@@Z
140921b14: mov         edx,0Dh
140921b19: lea         rcx,[rsp+20h]
140921b1e: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140921b24: mov         rcx,qword ptr [rsp+0A0]
140921b2c: add         rcx,18h
140921b30: mov         r8d,1
140921b36: movzx       edx,word ptr [rax]
140921b39: call        qword ptr [141E04EE0h] ; QtCore4_64.dll!?remove@QString@@QEAAAEAV1@VQChar@@W4CaseSensitivity@Qt@@@Z
140921b3f: mov         edx,0Ah
140921b44: lea         rcx,[rsp+30h]
140921b49: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140921b4f: movzx       edx,word ptr [rax]
140921b52: lea         rcx,[rsp+28h]
140921b57: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140921b5d: mov         qword ptr [rsp+60h],rax
140921b62: mov         rax,qword ptr [rsp+60h]
140921b67: mov         qword ptr [rsp+68h],rax
140921b6c: mov         edx,0Dh
140921b71: lea         rcx,[rsp+40h]
140921b76: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140921b7c: movzx       edx,word ptr [rax]
140921b7f: lea         rcx,[rsp+38h]
140921b84: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140921b8a: mov         qword ptr [rsp+70h],rax
140921b8f: mov         rax,qword ptr [rsp+70h]
140921b94: mov         qword ptr [rsp+78h],rax
140921b99: mov         r8,qword ptr [rsp+68h]
140921b9e: mov         rdx,qword ptr [rsp+78h]
140921ba3: lea         rcx,[rsp+48h]
140921ba8: call        1400048F0
140921bad: mov         qword ptr [rsp+080],rax
140921bb5: mov         rax,qword ptr [rsp+080]
140921bbd: mov         qword ptr [rsp+088],rax
140921bc5: mov         edx,0Ah
140921bca: lea         rcx,[rsp+50h]
140921bcf: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140921bd5: mov         rcx,qword ptr [rsp+0A0]
140921bdd: add         rcx,18h
140921be1: mov         r9d,1
140921be7: mov         r8,qword ptr [rsp+088]
140921bef: movzx       edx,word ptr [rax]
140921bf2: call        qword ptr [141E04F40h] ; QtCore4_64.dll!?replace@QString@@QEAAAEAV1@VQChar@@AEBV1@W4CaseSensitivity@Qt@@@Z
140921bf8: nop
140921bf9: lea         rcx,[rsp+48h]
140921bfe: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921c04: nop
140921c05: lea         rcx,[rsp+38h]
140921c0a: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921c10: nop
140921c11: lea         rcx,[rsp+28h]
140921c16: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921c1c: nop
140921c1d: mov         rcx,qword ptr [rsp+0A8]
140921c25: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921c2b: add         rsp,98h
140921c32: ret
140921c40: mov         qword ptr [rsp+10h],rdx
140921c45: mov         qword ptr [rsp+8],rcx
140921c4a: sub         rsp,98h
140921c51: mov         qword ptr [rsp+58h],0FFFFFFFFFFFFFFFEh
140921c5a: mov         rcx,qword ptr [rsp+0A0]
140921c62: add         rcx,20h
140921c66: mov         rdx,qword ptr [rsp+0A8]
140921c6e: call        qword ptr [141E04470h] ; QtCore4_64.dll!??4QString@@QEAAAEAV0@AEBV0@@Z
140921c74: mov         edx,0Dh
140921c79: lea         rcx,[rsp+20h]
140921c7e: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140921c84: mov         rcx,qword ptr [rsp+0A0]
140921c8c: add         rcx,20h
140921c90: mov         r8d,1
140921c96: movzx       edx,word ptr [rax]
140921c99: call        qword ptr [141E04EE0h] ; QtCore4_64.dll!?remove@QString@@QEAAAEAV1@VQChar@@W4CaseSensitivity@Qt@@@Z
140921c9f: mov         edx,0Ah
140921ca4: lea         rcx,[rsp+30h]
140921ca9: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140921caf: movzx       edx,word ptr [rax]
140921cb2: lea         rcx,[rsp+28h]
140921cb7: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140921cbd: mov         qword ptr [rsp+60h],rax
140921cc2: mov         rax,qword ptr [rsp+60h]
140921cc7: mov         qword ptr [rsp+68h],rax
140921ccc: mov         edx,0Dh
140921cd1: lea         rcx,[rsp+40h]
140921cd6: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140921cdc: movzx       edx,word ptr [rax]
140921cdf: lea         rcx,[rsp+38h]
140921ce4: call        qword ptr [141E05848h] ; QtCore4_64.dll!??0QString@@QEAA@VQChar@@@Z
140921cea: mov         qword ptr [rsp+70h],rax
140921cef: mov         rax,qword ptr [rsp+70h]
140921cf4: mov         qword ptr [rsp+78h],rax
140921cf9: mov         r8,qword ptr [rsp+68h]
140921cfe: mov         rdx,qword ptr [rsp+78h]
140921d03: lea         rcx,[rsp+48h]
140921d08: call        1400048F0
140921d0d: mov         qword ptr [rsp+080],rax
140921d15: mov         rax,qword ptr [rsp+080]
140921d1d: mov         qword ptr [rsp+088],rax
140921d25: mov         edx,0Ah
140921d2a: lea         rcx,[rsp+50h]
140921d2f: call        qword ptr [141E044A0h] ; QtCore4_64.dll!??0QChar@@QEAA@H@Z
140921d35: mov         rcx,qword ptr [rsp+0A0]
140921d3d: add         rcx,20h
140921d41: mov         r9d,1
140921d47: mov         r8,qword ptr [rsp+088]
140921d4f: movzx       edx,word ptr [rax]
140921d52: call        qword ptr [141E04F40h] ; QtCore4_64.dll!?replace@QString@@QEAAAEAV1@VQChar@@AEBV1@W4CaseSensitivity@Qt@@@Z
140921d58: nop
140921d59: lea         rcx,[rsp+48h]
140921d5e: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921d64: nop
140921d65: lea         rcx,[rsp+38h]
140921d6a: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921d70: nop
140921d71: lea         rcx,[rsp+28h]
140921d76: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921d7c: nop
140921d7d: mov         rcx,qword ptr [rsp+0A8]
140921d85: call        qword ptr [141E059E0h] ; QtCore4_64.dll!??1QString@@QEAA@XZ
140921d8b: add         rsp,98h
140921d92: ret
1409ff590: mov         dword ptr [rsp+10h],edx
1409ff594: mov         qword ptr [rsp+8],rcx
1409ff599: mov         rcx,qword ptr [rsp+8]
1409ff59e: mov         eax,dword ptr [rsp+10h]
1409ff5a2: mov         dword ptr [rcx+1Ch],eax
1409ff5a5: ret
1409ff5a6: int         3
1409ff5a7: int         3
1409ff5a8: int         3
1409ff5a9: int         3
1409ff5aa: int         3
1409ff5ab: int         3
1409ff5ac: int         3
1409ff5ad: int         3
1409ff5ae: int         3
1409ff5af: int         3
1409ff5b0: mov         qword ptr [rsp+10h],rdx
1409ff5b5: mov         qword ptr [rsp+8],rcx
1409ff5ba: mov         rcx,qword ptr [rsp+8]
1400d8040: mov         dword ptr [rsp+10h],edx
1400d8044: mov         qword ptr [rsp+8],rcx
1400d8049: mov         rcx,qword ptr [rsp+8]
1400d804e: mov         eax,dword ptr [rsp+10h]
1400d8052: mov         dword ptr [rcx+24h],eax
1400d8055: ret
1400d8056: int         3
1400d8057: int         3
1400d8058: int         3
1400d8059: int         3
1400d805a: int         3
1400d805b: int         3
1400d805c: int         3
1400d805d: int         3
1400d805e: int         3
1400d805f: int         3
1400d8060: mov         qword ptr [rsp+8],rcx
1400d8065: sub         rsp,38h
1400d806b: and         al,20h
140917170: mov         byte ptr [rsp+10h],dl
140917174: mov         qword ptr [rsp+8],rcx
140917179: mov         rcx,qword ptr [rsp+8]
14091717e: movzx       eax,byte ptr [rsp+10h]
140917183: mov         byte ptr [rcx+31h],al
140917186: ret
140917187: int         3
140917188: int         3
140917189: int         3
14091718a: int         3
14091718b: int         3
14091718c: int         3
14091718d: int         3
14091718e: int         3
14091718f: int         3
140917190: mov         byte ptr [rsp+10h],dl
140917194: mov         qword ptr [rsp+8],rcx
140917199: mov         rcx,qword ptr [rsp+8]
14091719e: movzx       eax,byte ptr [rsp+10h]
1409171a3: mov         byte ptr [rcx+32h],al
1409171a6: ret
1409171a7: int         3
1409171a8: int         3
1409171a9: int         3
1409171aa: int         3
1409171ab: int         3
1409171ac: int         3
1409171ad: int         3
1409171ae: int         3
1409171af: int         3
14010f1e0: mov         dword ptr [rsp+10h],edx
14010f1e4: mov         qword ptr [rsp+8],rcx
14010f1e9: mov         rcx,qword ptr [rsp+8]
14010f1ee: mov         eax,dword ptr [rsp+10h]
14010f1f2: mov         dword ptr [rcx+0000000000000080h],eax
14010f1f8: ret
14010f1f9: int         3
14010f1fa: int         3
14010f1fb: int         3
14010f1fc: int         3
14010f1fd: int         3
14010f1fe: int         3
14010f1ff: int         3

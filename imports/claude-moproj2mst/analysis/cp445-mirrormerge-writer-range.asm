140e23d80  48894c2408               MOV qword ptr [RSP + 0x8],RCX
140e23d85  53                       PUSH RBX
140e23d86  4881ec60030000           SUB RSP,0x360
140e23d8d  48c7842400020000feffffff MOV qword ptr [RSP + 0x200],-0x2
140e23d99  0f29b42450030000         MOVAPS xmmword ptr [RSP + 0x350],XMM6
140e23da1  488b8c2470030000         MOV RCX,qword ptr [RSP + 0x370]
140e23da9  e802b57cff               CALL 0x1405ef2b0
140e23dae  4889442458               MOV qword ptr [RSP + 0x58],RAX
140e23db3  48837c245800             CMP qword ptr [RSP + 0x58],0x0
140e23db9  7505                     JNZ 0x140e23dc0
140e23dbb  e9e20c0000               JMP 0x140e24aa2
140e23dc0  488b4c2458               MOV RCX,qword ptr [RSP + 0x58]
140e23dc5  e8a6b923ff               CALL 0x14005f770
140e23dca  4889442450               MOV qword ptr [RSP + 0x50],RAX
140e23dcf  48837c245000             CMP qword ptr [RSP + 0x50],0x0
140e23dd5  7505                     JNZ 0x140e23ddc
140e23dd7  e9c60c0000               JMP 0x140e24aa2
140e23ddc  488d8c2400010000         LEA RCX,[RSP + 0x100]
140e23de4  ff156604fe00             CALL qword ptr [0x141e04250]
140e23dea  488bd8                   MOV RBX,RAX
140e23ded  488d942408010000         LEA RDX,[RSP + 0x108]
140e23df5  488b4c2458               MOV RCX,qword ptr [RSP + 0x58]
140e23dfa  e811d322ff               CALL 0x140051110
140e23dff  4889842408020000         MOV qword ptr [RSP + 0x208],RAX
140e23e07  488b842408020000         MOV RAX,qword ptr [RSP + 0x208]
140e23e0f  4889842410020000         MOV qword ptr [RSP + 0x210],RAX
140e23e17  440fb703                 MOVZX R8D,word ptr [RBX]
140e23e1b  488b942410020000         MOV RDX,qword ptr [RSP + 0x210]
140e23e23  488d8c2410010000         LEA RCX,[RSP + 0x110]
140e23e2b  e8400b1eff               CALL 0x140004970
140e23e30  4889842418020000         MOV qword ptr [RSP + 0x218],RAX
140e23e38  488b842418020000         MOV RAX,qword ptr [RSP + 0x218]
140e23e40  4889842420020000         MOV qword ptr [RSP + 0x220],RAX
140e23e48  4c8d0501b9c901           LEA R8,[0x142abf750]
140e23e4f  488b942420020000         MOV RDX,qword ptr [RSP + 0x220]
140e23e57  488d8c2418010000         LEA RCX,[RSP + 0x118]
140e23e5f  e8ace21dff               CALL 0x140002110
140e23e64  4889842428020000         MOV qword ptr [RSP + 0x228],RAX
140e23e6c  488b842428020000         MOV RAX,qword ptr [RSP + 0x228]
140e23e74  4889842430020000         MOV qword ptr [RSP + 0x230],RAX
140e23e7c  488b942430020000         MOV RDX,qword ptr [RSP + 0x230]
140e23e84  488d8c2498000000         LEA RCX,[RSP + 0x98]
140e23e8c  ff151e16fe00             CALL qword ptr [0x141e054b0]
140e23e92  90                       NOP
140e23e93  488d8c2418010000         LEA RCX,[RSP + 0x118]
140e23e9b  ff153f1bfe00             CALL qword ptr [0x141e059e0]
140e23ea1  90                       NOP
140e23ea2  488d8c2410010000         LEA RCX,[RSP + 0x110]
140e23eaa  ff15301bfe00             CALL qword ptr [0x141e059e0]
140e23eb0  90                       NOP
140e23eb1  488d8c2408010000         LEA RCX,[RSP + 0x108]
140e23eb9  ff15211bfe00             CALL qword ptr [0x141e059e0]
140e23ebf  488d4c2468               LEA RCX,[RSP + 0x68]
140e23ec4  ff15f605fe00             CALL qword ptr [0x141e044c0]
140e23eca  90                       NOP
140e23ecb  488d942498000000         LEA RDX,[RSP + 0x98]
140e23ed3  488d8c24b0000000         LEA RCX,[RSP + 0xb0]
140e23edb  ff15f700fe00             CALL qword ptr [0x141e03fd8]
140e23ee1  90                       NOP
140e23ee2  48c744244800000000       MOV qword ptr [RSP + 0x48],0x0
140e23eeb  488d842420010000         LEA RAX,[RSP + 0x120]
140e23ef3  4889842428010000         MOV qword ptr [RSP + 0x128],RAX
140e23efb  ba02000000               MOV EDX,0x2
140e23f00  488b8c2428010000         MOV RCX,qword ptr [RSP + 0x128]
140e23f08  e8530b1eff               CALL 0x140004a60
140e23f0d  488bd0                   MOV RDX,RAX
140e23f10  488d8c24b0000000         LEA RCX,[RSP + 0xb0]
140e23f18  ff15c200fe00             CALL qword ptr [0x141e03fe0]
140e23f1e  0fb6c0                   MOVZX EAX,AL
140e23f21  85c0                     TEST EAX,EAX
140e23f23  7569                     JNZ 0x140e23f8e
140e23f25  488d8c24d8000000         LEA RCX,[RSP + 0xd8]
140e23f2d  ff158d05fe00             CALL qword ptr [0x141e044c0]
140e23f33  90                       NOP
140e23f34  488d8c24d0000000         LEA RCX,[RSP + 0xd0]
140e23f3c  ff157e05fe00             CALL qword ptr [0x141e044c0]
140e23f42  488d8c24d0000000         LEA RCX,[RSP + 0xd0]
140e23f4a  ff15901afe00             CALL qword ptr [0x141e059e0]
140e23f50  90                       NOP
140e23f51  488d8c24d8000000         LEA RCX,[RSP + 0xd8]
140e23f59  ff15811afe00             CALL qword ptr [0x141e059e0]
140e23f5f  90                       NOP
140e23f60  488d8c24b0000000         LEA RCX,[RSP + 0xb0]
140e23f68  ff15b200fe00             CALL qword ptr [0x141e04020]
140e23f6e  90                       NOP
140e23f6f  488d4c2468               LEA RCX,[RSP + 0x68]
140e23f74  ff15661afe00             CALL qword ptr [0x141e059e0]
140e23f7a  90                       NOP
140e23f7b  488d8c2498000000         LEA RCX,[RSP + 0x98]
140e23f83  ff15571afe00             CALL qword ptr [0x141e059e0]
140e23f89  e9140b0000               JMP 0x140e24aa2
140e23f8e  488d9424b0000000         LEA RDX,[RSP + 0xb0]
140e23f96  488d4c2478               LEA RCX,[RSP + 0x78]
140e23f9b  ff1537fefd00             CALL qword ptr [0x141e03dd8]
140e23fa1  90                       NOP
140e23fa2  488b4c2450               MOV RCX,qword ptr [RSP + 0x50]
140e23fa7  e8f481e5ff               CALL 0x140c7c1a0
140e23fac  89842490000000           MOV dword ptr [RSP + 0x90],EAX
140e23fb3  488d942430010000         LEA RDX,[RSP + 0x130]
140e23fbb  488b8c2470030000         MOV RCX,qword ptr [RSP + 0x370]
140e23fc3  e8f80a0000               CALL 0x140e24ac0
140e23fc8  4889842438020000         MOV qword ptr [RSP + 0x238],RAX
140e23fd0  488b842438020000         MOV RAX,qword ptr [RSP + 0x238]
140e23fd8  4889842440020000         MOV qword ptr [RSP + 0x240],RAX
140e23fe0  488d8c2438010000         LEA RCX,[RSP + 0x138]
140e23fe8  ff156202fe00             CALL qword ptr [0x141e04250]
140e23fee  488bd8                   MOV RBX,RAX
140e23ff1  488d942440010000         LEA RDX,[RSP + 0x140]
140e23ff9  488b4c2458               MOV RCX,qword ptr [RSP + 0x58]
140e23ffe  e80dd122ff               CALL 0x140051110
140e24003  4889842448020000         MOV qword ptr [RSP + 0x248],RAX
140e2400b  488b842448020000         MOV RAX,qword ptr [RSP + 0x248]
140e24013  4889842450020000         MOV qword ptr [RSP + 0x250],RAX
140e2401b  440fb703                 MOVZX R8D,word ptr [RBX]
140e2401f  488b942450020000         MOV RDX,qword ptr [RSP + 0x250]
140e24027  488d8c2448010000         LEA RCX,[RSP + 0x148]
140e2402f  e83c091eff               CALL 0x140004970
140e24034  4889842458020000         MOV qword ptr [RSP + 0x258],RAX
140e2403c  488b842458020000         MOV RAX,qword ptr [RSP + 0x258]
140e24044  4889842460020000         MOV qword ptr [RSP + 0x260],RAX
140e2404c  4c8b842440020000         MOV R8,qword ptr [RSP + 0x240]
140e24054  488b942460020000         MOV RDX,qword ptr [RSP + 0x260]
140e2405c  488d8c2450010000         LEA RCX,[RSP + 0x150]
140e24064  e887081eff               CALL 0x1400048f0
140e24069  4889842468020000         MOV qword ptr [RSP + 0x268],RAX
140e24071  488b842468020000         MOV RAX,qword ptr [RSP + 0x268]
140e24079  4889842470020000         MOV qword ptr [RSP + 0x270],RAX
140e24081  488b942470020000         MOV RDX,qword ptr [RSP + 0x270]
140e24089  488d8c2458010000         LEA RCX,[RSP + 0x158]
140e24091  ff151914fe00             CALL qword ptr [0x141e054b0]
140e24097  4889842478020000         MOV qword ptr [RSP + 0x278],RAX
140e2409f  488b842478020000         MOV RAX,qword ptr [RSP + 0x278]
140e240a7  4889842480020000         MOV qword ptr [RSP + 0x280],RAX
140e240af  488b942480020000         MOV RDX,qword ptr [RSP + 0x280]
140e240b7  488d4c2468               LEA RCX,[RSP + 0x68]
140e240bc  ff15ae03fe00             CALL qword ptr [0x141e04470]
140e240c2  90                       NOP
140e240c3  488d8c2458010000         LEA RCX,[RSP + 0x158]
140e240cb  ff150f19fe00             CALL qword ptr [0x141e059e0]
140e240d1  90                       NOP
140e240d2  488d8c2450010000         LEA RCX,[RSP + 0x150]
140e240da  ff150019fe00             CALL qword ptr [0x141e059e0]
140e240e0  90                       NOP
140e240e1  488d8c2448010000         LEA RCX,[RSP + 0x148]
140e240e9  ff15f118fe00             CALL qword ptr [0x141e059e0]
140e240ef  90                       NOP
140e240f0  488d8c2440010000         LEA RCX,[RSP + 0x140]
140e240f8  ff15e218fe00             CALL qword ptr [0x141e059e0]
140e240fe  90                       NOP
140e240ff  488d8c2430010000         LEA RCX,[RSP + 0x130]
140e24107  ff15d318fe00             CALL qword ptr [0x141e059e0]
140e2410d  488d4c2468               LEA RCX,[RSP + 0x68]
140e24112  ff15b0fefd00             CALL qword ptr [0x141e03fc8]
140e24118  0fb6c0                   MOVZX EAX,AL
140e2411b  85c0                     TEST EAX,EAX
140e2411d  7411                     JZ 0x140e24130
140e2411f  488d053ab6c901           LEA RAX,[0x142abf760]
140e24126  4889842488020000         MOV qword ptr [RSP + 0x288],RAX
140e2412e  eb13                     JMP 0x140e24143
140e24130  488d4c2468               LEA RCX,[RSP + 0x68]
140e24135  ff15bd16fe00             CALL qword ptr [0x141e057f8]
140e2413b  4889842488020000         MOV qword ptr [RSP + 0x288],RAX
140e24143  b903000000               MOV ECX,0x3
140e24148  e8e3ddafff               CALL 0x140921f30
140e2414d  488bd0                   MOV RDX,RAX
140e24150  488d4c2478               LEA RCX,[RSP + 0x78]
140e24155  ff1585fcfd00             CALL qword ptr [0x141e03de0]
140e2415b  488d15b27fc901           LEA RDX,[0x142abc114]
140e24162  488bc8                   MOV RCX,RAX
140e24165  ff1575fcfd00             CALL qword ptr [0x141e03de0]
140e2416b  ba01000000               MOV EDX,0x1
140e24170  488bc8                   MOV RCX,RAX
140e24173  ff156ffcfd00             CALL qword ptr [0x141e03de8]
140e24179  488b151802fe00           MOV RDX,qword ptr [0x141e04398]
140e24180  488bc8                   MOV RCX,RAX
140e24183  e838da21ff               CALL 0x140041bc0
140e24188  488b942488020000         MOV RDX,qword ptr [RSP + 0x288]
140e24190  488bc8                   MOV RCX,RAX
140e24193  ff1547fcfd00             CALL qword ptr [0x141e03de0]
140e24199  488b15f801fe00           MOV RDX,qword ptr [0x141e04398]
140e241a0  488bc8                   MOV RCX,RAX
140e241a3  e818da21ff               CALL 0x140041bc0
140e241a8  ba01000000               MOV EDX,0x1
140e241ad  488b8c2470030000         MOV RCX,qword ptr [RSP + 0x370]
140e241b5  e8765b0000               CALL 0x140e29d30
140e241ba  660f28f0                 MOVAPD XMM6,XMM0
140e241be  488d15a3b5c901           LEA RDX,[0x142abf768]
140e241c5  488d4c2478               LEA RCX,[RSP + 0x78]
140e241ca  ff1510fcfd00             CALL qword ptr [0x141e03de0]
140e241d0  488d159d80c901           LEA RDX,[0x142abc274]
140e241d7  488bc8                   MOV RCX,RAX
140e241da  ff1500fcfd00             CALL qword ptr [0x141e03de0]
140e241e0  660f28ce                 MOVAPD XMM1,XMM6
140e241e4  488bc8                   MOV RCX,RAX
140e241e7  ff15f316fe00             CALL qword ptr [0x141e058e0]
140e241ed  488b15a401fe00           MOV RDX,qword ptr [0x141e04398]
140e241f4  488bc8                   MOV RCX,RAX
140e241f7  e8c4d921ff               CALL 0x140041bc0
140e241fc  c78424e000000000000000   MOV dword ptr [RSP + 0xe0],0x0
140e24207  eb11                     JMP 0x140e2421a
140e24209  8b8424e0000000           MOV EAX,dword ptr [RSP + 0xe0]
140e24210  83c001                   ADD EAX,0x1
140e24213  898424e0000000           MOV dword ptr [RSP + 0xe0],EAX
140e2421a  8b842490000000           MOV EAX,dword ptr [RSP + 0x90]
140e24221  398424e0000000           CMP dword ptr [RSP + 0xe0],EAX
140e24228  0f8d30080000             JGE 0x140e24a5e
140e2422e  8b8424e0000000           MOV EAX,dword ptr [RSP + 0xe0]
140e24235  83c001                   ADD EAX,0x1
140e24238  89842460010000           MOV dword ptr [RSP + 0x160],EAX
140e2423f  488d942460010000         LEA RDX,[RSP + 0x160]
140e24247  488b4c2450               MOV RCX,qword ptr [RSP + 0x50]
140e2424c  e8ffeae5ff               CALL 0x140c82d50
140e24251  48898424e8000000         MOV qword ptr [RSP + 0xe8],RAX
140e24259  4883bc24e800000000       CMP qword ptr [RSP + 0xe8],0x0
140e24262  753a                     JNZ 0x140e2429e
140e24264  488d4c2478               LEA RCX,[RSP + 0x78]
140e24269  ff1589fbfd00             CALL qword ptr [0x141e03df8]
140e2426f  90                       NOP
140e24270  488d8c24b0000000         LEA RCX,[RSP + 0xb0]
140e24278  ff15a2fdfd00             CALL qword ptr [0x141e04020]
140e2427e  90                       NOP
140e2427f  488d4c2468               LEA RCX,[RSP + 0x68]
140e24284  ff155617fe00             CALL qword ptr [0x141e059e0]
140e2428a  90                       NOP
140e2428b  488d8c2498000000         LEA RCX,[RSP + 0x98]
140e24293  ff154717fe00             CALL qword ptr [0x141e059e0]
140e24299  e904080000               JMP 0x140e24aa2
140e2429e  488b8424e8000000         MOV RAX,qword ptr [RSP + 0xe8]
140e242a6  480548030000             ADD RAX,0x348
140e242ac  4889442448               MOV qword ptr [RSP + 0x48],RAX
140e242b1  c78424f000000000000000   MOV dword ptr [RSP + 0xf0],0x0
140e242bc  488b4c2448               MOV RCX,qword ptr [RSP + 0x48]
140e242c1  e85a514200               CALL 0x141249420
140e242c6  898424f0000000           MOV dword ptr [RSP + 0xf0],EAX
140e242cd  83bc24f000000000         CMP dword ptr [RSP + 0xf0],0x0
140e242d5  7f05                     JG 0x140e242dc
140e242d7  e92dffffff               JMP 0x140e24209
140e242dc  8b9c24e0000000           MOV EBX,dword ptr [RSP + 0xe0]
140e242e3  83c301                   ADD EBX,0x1
140e242e6  488d158bb4c901           LEA RDX,[0x142abf778]
140e242ed  488d4c2478               LEA RCX,[RSP + 0x78]
140e242f2  ff15e8fafd00             CALL qword ptr [0x141e03de0]
140e242f8  488d158d81c901           LEA RDX,[0x142abc48c]
140e242ff  488bc8                   MOV RCX,RAX
140e24302  ff15d8fafd00             CALL qword ptr [0x141e03de0]
140e24308  8bd3                     MOV EDX,EBX
140e2430a  488bc8                   MOV RCX,RAX
140e2430d  ff15d5fafd00             CALL qword ptr [0x141e03de8]
140e24313  488d150288c901           LEA RDX,[0x142abcb1c]
140e2431a  488bc8                   MOV RCX,RAX
140e2431d  ff15bdfafd00             CALL qword ptr [0x141e03de0]
140e24323  8b9424f0000000           MOV EDX,dword ptr [RSP + 0xf0]
140e2432a  488bc8                   MOV RCX,RAX
140e2432d  ff15b5fafd00             CALL qword ptr [0x141e03de8]
140e24333  488b155e00fe00           MOV RDX,qword ptr [0x141e04398]
140e2433a  488bc8                   MOV RCX,RAX
140e2433d  e87ed821ff               CALL 0x140041bc0
140e24342  c78424f400000000000000   MOV dword ptr [RSP + 0xf4],0x0
140e2434d  eb11                     JMP 0x140e24360
140e2434f  8b8424f4000000           MOV EAX,dword ptr [RSP + 0xf4]
140e24356  83c001                   ADD EAX,0x1
140e24359  898424f4000000           MOV dword ptr [RSP + 0xf4],EAX
140e24360  8b8424f0000000           MOV EAX,dword ptr [RSP + 0xf0]
140e24367  398424f4000000           CMP dword ptr [RSP + 0xf4],EAX
140e2436e  0f8de5060000             JGE 0x140e24a59
140e24374  48638c24f4000000         MOVSXD RCX,dword ptr [RSP + 0xf4]
140e2437c  4869c908010000           IMUL RCX,RCX,0x108
140e24383  488b442448               MOV RAX,qword ptr [RSP + 0x48]
140e24388  4803c1                   ADD RAX,RCX
140e2438b  48898424f8000000         MOV qword ptr [RSP + 0xf8],RAX
140e24393  c784246401000000000000   MOV dword ptr [RSP + 0x164],0x0
140e2439e  488d942464010000         LEA RDX,[RSP + 0x164]
140e243a6  488b8c24f8000000         MOV RCX,qword ptr [RSP + 0xf8]
140e243ae  e89d3a4200               CALL 0x141247e50
140e243b3  660f6ec0                 MOVD XMM0,EAX
140e243b7  f30fe6c0                 CVTDQ2PD XMM0,XMM0
140e243bb  f20f11442470             MOVSD qword ptr [RSP + 0x70],XMM0
140e243c1  c784246801000001000000   MOV dword ptr [RSP + 0x168],0x1
140e243cc  488d942468010000         LEA RDX,[RSP + 0x168]
140e243d4  488b8c24f8000000         MOV RCX,qword ptr [RSP + 0xf8]
140e243dc  e86f3a4200               CALL 0x141247e50
140e243e1  660f6ec0                 MOVD XMM0,EAX
140e243e5  f30fe6c0                 CVTDQ2PD XMM0,XMM0
140e243e9  f20f118424c8000000       MOVSD qword ptr [RSP + 0xc8],XMM0
140e243f2  488b8424f8000000         MOV RAX,qword ptr [RSP + 0xf8]
140e243fa  f20f1080e0000000         MOVSD XMM0,qword ptr [RAX + 0xe0]
140e24402  f20f11842488000000       MOVSD qword ptr [RSP + 0x88],XMM0
140e2440b  488b8424f8000000         MOV RAX,qword ptr [RSP + 0xf8]
140e24413  f20f1080e8000000         MOVSD XMM0,qword ptr [RAX + 0xe8]
140e2441b  f20f118424a8000000       MOVSD qword ptr [RSP + 0xa8],XMM0
140e24424  488b8424f8000000         MOV RAX,qword ptr [RSP + 0xf8]
140e2442c  f20f1080f0000000         MOVSD XMM0,qword ptr [RAX + 0xf0]
140e24434  f20f11442460             MOVSD qword ptr [RSP + 0x60],XMM0
140e2443a  488b8424f8000000         MOV RAX,qword ptr [RSP + 0xf8]
140e24442  f20f1080c0000000         MOVSD XMM0,qword ptr [RAX + 0xc0]
140e2444a  f20f118424a0000000       MOVSD qword ptr [RSP + 0xa0],XMM0
140e24453  488b8424f8000000         MOV RAX,qword ptr [RSP + 0xf8]
140e2445b  f20f1080c8000000         MOVSD XMM0,qword ptr [RAX + 0xc8]
140e24463  f20f11442440             MOVSD qword ptr [RSP + 0x40],XMM0
140e24469  488b8424f8000000         MOV RAX,qword ptr [RSP + 0xf8]
140e24471  f20f1080d0000000         MOVSD XMM0,qword ptr [RAX + 0xd0]
140e24479  f20f118424c0000000       MOVSD qword ptr [RSP + 0xc0],XMM0
140e24482  b220                     MOV DL,0x20
140e24484  488d8c24f2010000         LEA RCX,[RSP + 0x1f2]
140e2448c  e8cfe31dff               CALL 0x140002860
140e24491  0fb610                   MOVZX EDX,byte ptr [RAX]
140e24494  488d8c24f0010000         LEA RCX,[RSP + 0x1f0]
140e2449c  ff15b6fefd00             CALL qword ptr [0x141e04358]
140e244a2  b220                     MOV DL,0x20
140e244a4  488d8c24da010000         LEA RCX,[RSP + 0x1da]
140e244ac  e8afe31dff               CALL 0x140002860
140e244b1  0fb610                   MOVZX EDX,byte ptr [RAX]
140e244b4  488d8c24d8010000         LEA RCX,[RSP + 0x1d8]
140e244bc  ff1596fefd00             CALL qword ptr [0x141e04358]
140e244c2  b220                     MOV DL,0x20
140e244c4  488d8c24c2010000         LEA RCX,[RSP + 0x1c2]
140e244cc  e88fe31dff               CALL 0x140002860
140e244d1  0fb610                   MOVZX EDX,byte ptr [RAX]
140e244d4  488d8c24c0010000         LEA RCX,[RSP + 0x1c0]
140e244dc  ff1576fefd00             CALL qword ptr [0x141e04358]
140e244e2  b220                     MOV DL,0x20
140e244e4  488d8c24aa010000         LEA RCX,[RSP + 0x1aa]
140e244ec  e86fe31dff               CALL 0x140002860
140e244f1  0fb610                   MOVZX EDX,byte ptr [RAX]
140e244f4  488d8c24a8010000         LEA RCX,[RSP + 0x1a8]
140e244fc  ff1556fefd00             CALL qword ptr [0x141e04358]
140e24502  b220                     MOV DL,0x20
140e24504  488d8c2492010000         LEA RCX,[RSP + 0x192]
140e2450c  e84fe31dff               CALL 0x140002860
140e24511  0fb610                   MOVZX EDX,byte ptr [RAX]
140e24514  488d8c2490010000         LEA RCX,[RSP + 0x190]
140e2451c  ff1536fefd00             CALL qword ptr [0x141e04358]
140e24522  b220                     MOV DL,0x20
140e24524  488d8c247a010000         LEA RCX,[RSP + 0x17a]
140e2452c  e82fe31dff               CALL 0x140002860
140e24531  0fb610                   MOVZX EDX,byte ptr [RAX]
140e24534  488d8c2478010000         LEA RCX,[RSP + 0x178]
140e2453c  ff1516fefd00             CALL qword ptr [0x141e04358]
140e24542  488b8c24f8000000         MOV RCX,qword ptr [RSP + 0xf8]
140e2454a  e861394200               CALL 0x141247eb0
140e2454f  0fb6d8                   MOVZX EBX,AL
140e24552  488d1533aec901           LEA RDX,[0x142abf38c]
140e24559  488d8c24e8010000         LEA RCX,[RSP + 0x1e8]
140e24561  ff1519fffd00             CALL qword ptr [0x141e04480]
140e24567  4889842490020000         MOV qword ptr [RSP + 0x290],RAX
140e2456f  488b842490020000         MOV RAX,qword ptr [RSP + 0x290]
140e24577  4889842498020000         MOV qword ptr [RSP + 0x298],RAX
140e2457f  488d8424f0010000         LEA RAX,[RSP + 0x1f0]
140e24587  4889442430               MOV qword ptr [RSP + 0x30],RAX
140e2458c  c744242807000000         MOV dword ptr [RSP + 0x28],0x7
140e24594  c644242067               MOV byte ptr [RSP + 0x20],0x67
140e24599  4533c9                   XOR R9D,R9D
140e2459c  f20f109424c0000000       MOVSD XMM2,qword ptr [RSP + 0xc0]
140e245a5  488d9424f8010000         LEA RDX,[RSP + 0x1f8]
140e245ad  488b8c2498020000         MOV RCX,qword ptr [RSP + 0x298]
140e245b5  ff155501fe00             CALL qword ptr [0x141e04710]
140e245bb  48898424a0020000         MOV qword ptr [RSP + 0x2a0],RAX
140e245c3  488b8424a0020000         MOV RAX,qword ptr [RSP + 0x2a0]
140e245cb  48898424a8020000         MOV qword ptr [RSP + 0x2a8],RAX
140e245d3  488d1522a8c901           LEA RDX,[0x142abedfc]
140e245da  488d8c24d0010000         LEA RCX,[RSP + 0x1d0]
140e245e2  ff1598fefd00             CALL qword ptr [0x141e04480]
140e245e8  48898424b0020000         MOV qword ptr [RSP + 0x2b0],RAX
140e245f0  488b8424b0020000         MOV RAX,qword ptr [RSP + 0x2b0]
140e245f8  48898424b8020000         MOV qword ptr [RSP + 0x2b8],RAX
140e24600  488d8424d8010000         LEA RAX,[RSP + 0x1d8]
140e24608  4889442430               MOV qword ptr [RSP + 0x30],RAX
140e2460d  c744242807000000         MOV dword ptr [RSP + 0x28],0x7
140e24615  c644242067               MOV byte ptr [RSP + 0x20],0x67
140e2461a  4533c9                   XOR R9D,R9D
140e2461d  f20f10542440             MOVSD XMM2,qword ptr [RSP + 0x40]
140e24623  488d9424e0010000         LEA RDX,[RSP + 0x1e0]
140e2462b  488b8c24b8020000         MOV RCX,qword ptr [RSP + 0x2b8]
140e24633  ff15d700fe00             CALL qword ptr [0x141e04710]
140e24639  48898424c0020000         MOV qword ptr [RSP + 0x2c0],RAX
140e24641  488b8424c0020000         MOV RAX,qword ptr [RSP + 0x2c0]
140e24649  48898424c8020000         MOV qword ptr [RSP + 0x2c8],RAX
140e24651  488d155ca6c901           LEA RDX,[0x142abecb4]
140e24658  488d8c24b8010000         LEA RCX,[RSP + 0x1b8]
140e24660  ff151afefd00             CALL qword ptr [0x141e04480]
140e24666  48898424d0020000         MOV qword ptr [RSP + 0x2d0],RAX
140e2466e  488b8424d0020000         MOV RAX,qword ptr [RSP + 0x2d0]
140e24676  48898424d8020000         MOV qword ptr [RSP + 0x2d8],RAX
140e2467e  488d8424c0010000         LEA RAX,[RSP + 0x1c0]
140e24686  4889442430               MOV qword ptr [RSP + 0x30],RAX
140e2468b  c744242807000000         MOV dword ptr [RSP + 0x28],0x7
140e24693  c644242067               MOV byte ptr [RSP + 0x20],0x67
140e24698  4533c9                   XOR R9D,R9D
140e2469b  f20f109424a0000000       MOVSD XMM2,qword ptr [RSP + 0xa0]
140e246a4  488d9424c8010000         LEA RDX,[RSP + 0x1c8]
140e246ac  488b8c24d8020000         MOV RCX,qword ptr [RSP + 0x2d8]
140e246b4  ff155600fe00             CALL qword ptr [0x141e04710]
140e246ba  48898424e0020000         MOV qword ptr [RSP + 0x2e0],RAX
140e246c2  488b8424e0020000         MOV RAX,qword ptr [RSP + 0x2e0]
140e246ca  48898424e8020000         MOV qword ptr [RSP + 0x2e8],RAX
140e246d2  488d15fba3c901           LEA RDX,[0x142abead4]
140e246d9  488d8c24a0010000         LEA RCX,[RSP + 0x1a0]
140e246e1  ff1599fdfd00             CALL qword ptr [0x141e04480]
140e246e7  48898424f0020000         MOV qword ptr [RSP + 0x2f0],RAX
140e246ef  488b8424f0020000         MOV RAX,qword ptr [RSP + 0x2f0]
140e246f7  48898424f8020000         MOV qword ptr [RSP + 0x2f8],RAX
140e246ff  488d8424a8010000         LEA RAX,[RSP + 0x1a8]
140e24707  4889442430               MOV qword ptr [RSP + 0x30],RAX
140e2470c  c744242807000000         MOV dword ptr [RSP + 0x28],0x7
140e24714  c644242067               MOV byte ptr [RSP + 0x20],0x67
140e24719  4533c9                   XOR R9D,R9D
140e2471c  f20f109424a8000000       MOVSD XMM2,qword ptr [RSP + 0xa8]
140e24725  488d9424b0010000         LEA RDX,[RSP + 0x1b0]
140e2472d  488b8c24f8020000         MOV RCX,qword ptr [RSP + 0x2f8]
140e24735  ff15d5fffd00             CALL qword ptr [0x141e04710]
140e2473b  4889842400030000         MOV qword ptr [RSP + 0x300],RAX
140e24743  488b842400030000         MOV RAX,qword ptr [RSP + 0x300]
140e2474b  4889842408030000         MOV qword ptr [RSP + 0x308],RAX
140e24753  488d155aa0c901           LEA RDX,[0x142abe7b4]
140e2475a  488d8c2488010000         LEA RCX,[RSP + 0x188]
140e24762  ff1518fdfd00             CALL qword ptr [0x141e04480]
140e24768  4889842410030000         MOV qword ptr [RSP + 0x310],RAX
140e24770  488b842410030000         MOV RAX,qword ptr [RSP + 0x310]
140e24778  4889842418030000         MOV qword ptr [RSP + 0x318],RAX
140e24780  488d842490010000         LEA RAX,[RSP + 0x190]
140e24788  4889442430               MOV qword ptr [RSP + 0x30],RAX
140e2478d  c744242807000000         MOV dword ptr [RSP + 0x28],0x7
140e24795  c644242067               MOV byte ptr [RSP + 0x20],0x67
140e2479a  4533c9                   XOR R9D,R9D
140e2479d  f20f109424a8000000       MOVSD XMM2,qword ptr [RSP + 0xa8]
140e247a6  488d942498010000         LEA RDX,[RSP + 0x198]
140e247ae  488b8c2418030000         MOV RCX,qword ptr [RSP + 0x318]
140e247b6  ff1554fffd00             CALL qword ptr [0x141e04710]
140e247bc  4889842420030000         MOV qword ptr [RSP + 0x320],RAX
140e247c4  488b842420030000         MOV RAX,qword ptr [RSP + 0x320]
140e247cc  4889842428030000         MOV qword ptr [RSP + 0x328],RAX
140e247d4  488d15019dc901           LEA RDX,[0x142abe4dc]
140e247db  488d8c2470010000         LEA RCX,[RSP + 0x170]
140e247e3  ff1597fcfd00             CALL qword ptr [0x141e04480]
140e247e9  4889842430030000         MOV qword ptr [RSP + 0x330],RAX
140e247f1  488b842430030000         MOV RAX,qword ptr [RSP + 0x330]
140e247f9  4889842438030000         MOV qword ptr [RSP + 0x338],RAX
140e24801  488d842478010000         LEA RAX,[RSP + 0x178]
140e24809  4889442430               MOV qword ptr [RSP + 0x30],RAX
140e2480e  c744242807000000         MOV dword ptr [RSP + 0x28],0x7
140e24816  c644242067               MOV byte ptr [RSP + 0x20],0x67
140e2481b  4533c9                   XOR R9D,R9D
140e2481e  f20f10942488000000       MOVSD XMM2,qword ptr [RSP + 0x88]
140e24827  488d942480010000         LEA RDX,[RSP + 0x180]
140e2482f  488b8c2438030000         MOV RCX,qword ptr [RSP + 0x338]
140e24837  ff15d3fefd00             CALL qword ptr [0x141e04710]
140e2483d  4889842440030000         MOV qword ptr [RSP + 0x340],RAX
140e24845  488b842440030000         MOV RAX,qword ptr [RSP + 0x340]
140e2484d  4889842448030000         MOV qword ptr [RSP + 0x348],RAX
140e24855  488b8c24f8000000         MOV RCX,qword ptr [RSP + 0xf8]
140e2485d  e88e39e0ff               CALL 0x140c281f0
140e24862  8bd0                     MOV EDX,EAX
140e24864  488d4c2478               LEA RCX,[RSP + 0x78]
140e24869  ff1579f5fd00             CALL qword ptr [0x141e03de8]
140e2486f  488d156e92c901           LEA RDX,[0x142abdae4]
140e24876  488bc8                   MOV RCX,RAX
140e24879  ff1561f5fd00             CALL qword ptr [0x141e03de0]
140e2487f  f20f104c2470             MOVSD XMM1,qword ptr [RSP + 0x70]
140e24885  488bc8                   MOV RCX,RAX
140e24888  ff155210fe00             CALL qword ptr [0x141e058e0]
140e2488e  488d15ff92c901           LEA RDX,[0x142abdb94]
140e24895  488bc8                   MOV RCX,RAX
140e24898  ff1542f5fd00             CALL qword ptr [0x141e03de0]
140e2489e  f20f108c24c8000000       MOVSD XMM1,qword ptr [RSP + 0xc8]
140e248a7  488bc8                   MOV RCX,RAX
140e248aa  ff153010fe00             CALL qword ptr [0x141e058e0]
140e248b0  488d157596c901           LEA RDX,[0x142abdf2c]
140e248b7  488bc8                   MOV RCX,RAX
140e248ba  ff1520f5fd00             CALL qword ptr [0x141e03de0]
140e248c0  488b942448030000         MOV RDX,qword ptr [RSP + 0x348]
140e248c8  488bc8                   MOV RCX,RAX
140e248cb  ff151ff5fd00             CALL qword ptr [0x141e03df0]
140e248d1  488d15e49dc901           LEA RDX,[0x142abe6bc]
140e248d8  488bc8                   MOV RCX,RAX
140e248db  ff15fff4fd00             CALL qword ptr [0x141e03de0]
140e248e1  488b942428030000         MOV RDX,qword ptr [RSP + 0x328]
140e248e9  488bc8                   MOV RCX,RAX
140e248ec  ff15fef4fd00             CALL qword ptr [0x141e03df0]
140e248f2  488d15839fc901           LEA RDX,[0x142abe87c]
140e248f9  488bc8                   MOV RCX,RAX
140e248fc  ff15def4fd00             CALL qword ptr [0x141e03de0]
140e24902  488b942408030000         MOV RDX,qword ptr [RSP + 0x308]
140e2490a  488bc8                   MOV RCX,RAX
140e2490d  ff15ddf4fd00             CALL qword ptr [0x141e03df0]
140e24913  488d15daa2c901           LEA RDX,[0x142abebf4]
140e2491a  488bc8                   MOV RCX,RAX
140e2491d  ff15bdf4fd00             CALL qword ptr [0x141e03de0]
140e24923  488b9424e8020000         MOV RDX,qword ptr [RSP + 0x2e8]
140e2492b  488bc8                   MOV RCX,RAX
140e2492e  ff15bcf4fd00             CALL qword ptr [0x141e03df0]
140e24934  488d15d9a3c901           LEA RDX,[0x142abed14]
140e2493b  488bc8                   MOV RCX,RAX
140e2493e  ff159cf4fd00             CALL qword ptr [0x141e03de0]
140e24944  488b9424c8020000         MOV RDX,qword ptr [RSP + 0x2c8]
140e2494c  488bc8                   MOV RCX,RAX
140e2494f  ff159bf4fd00             CALL qword ptr [0x141e03df0]
140e24955  488d15c8a9c901           LEA RDX,[0x142abf324]
140e2495c  488bc8                   MOV RCX,RAX
140e2495f  ff157bf4fd00             CALL qword ptr [0x141e03de0]
140e24965  488b9424a8020000         MOV RDX,qword ptr [RSP + 0x2a8]
140e2496d  488bc8                   MOV RCX,RAX
140e24970  ff157af4fd00             CALL qword ptr [0x141e03df0]
140e24976  488d151faac901           LEA RDX,[0x142abf39c]
140e2497d  488bc8                   MOV RCX,RAX
140e24980  ff155af4fd00             CALL qword ptr [0x141e03de0]
140e24986  8bd3                     MOV EDX,EBX
140e24988  488bc8                   MOV RCX,RAX
140e2498b  ff1557f4fd00             CALL qword ptr [0x141e03de8]
140e24991  488b1500fafd00           MOV RDX,qword ptr [0x141e04398]
140e24998  488bc8                   MOV RCX,RAX
140e2499b  e820d221ff               CALL 0x140041bc0
140e249a0  90                       NOP
140e249a1  488d8c2480010000         LEA RCX,[RSP + 0x180]
140e249a9  ff153110fe00             CALL qword ptr [0x141e059e0]
140e249af  90                       NOP
140e249b0  488d8c2470010000         LEA RCX,[RSP + 0x170]
140e249b8  ff152210fe00             CALL qword ptr [0x141e059e0]
140e249be  90                       NOP
140e249bf  488d8c2498010000         LEA RCX,[RSP + 0x198]
140e249c7  ff151310fe00             CALL qword ptr [0x141e059e0]
140e249cd  90                       NOP
140e249ce  488d8c2488010000         LEA RCX,[RSP + 0x188]
140e249d6  ff150410fe00             CALL qword ptr [0x141e059e0]
140e249dc  90                       NOP
140e249dd  488d8c24b0010000         LEA RCX,[RSP + 0x1b0]
140e249e5  ff15f50ffe00             CALL qword ptr [0x141e059e0]
140e249eb  90                       NOP
140e249ec  488d8c24a0010000         LEA RCX,[RSP + 0x1a0]
140e249f4  ff15e60ffe00             CALL qword ptr [0x141e059e0]
140e249fa  90                       NOP
140e249fb  488d8c24c8010000         LEA RCX,[RSP + 0x1c8]
140e24a03  ff15d70ffe00             CALL qword ptr [0x141e059e0]
140e24a09  90                       NOP
140e24a0a  488d8c24b8010000         LEA RCX,[RSP + 0x1b8]
140e24a12  ff15c80ffe00             CALL qword ptr [0x141e059e0]
140e24a18  90                       NOP
140e24a19  488d8c24e0010000         LEA RCX,[RSP + 0x1e0]
140e24a21  ff15b90ffe00             CALL qword ptr [0x141e059e0]
140e24a27  90                       NOP
140e24a28  488d8c24d0010000         LEA RCX,[RSP + 0x1d0]
140e24a30  ff15aa0ffe00             CALL qword ptr [0x141e059e0]
140e24a36  90                       NOP
140e24a37  488d8c24f8010000         LEA RCX,[RSP + 0x1f8]
140e24a3f  ff159b0ffe00             CALL qword ptr [0x141e059e0]
140e24a45  90                       NOP
140e24a46  488d8c24e8010000         LEA RCX,[RSP + 0x1e8]
140e24a4e  ff158c0ffe00             CALL qword ptr [0x141e059e0]
140e24a54  e9f6f8ffff               JMP 0x140e2434f
140e24a59  e9abf7ffff               JMP 0x140e24209
140e24a5e  488d8c24b0000000         LEA RCX,[RSP + 0xb0]
140e24a66  ff15acf5fd00             CALL qword ptr [0x141e04018]
140e24a6c  90                       NOP
140e24a6d  488d4c2478               LEA RCX,[RSP + 0x78]
140e24a72  ff1580f3fd00             CALL qword ptr [0x141e03df8]
140e24a78  90                       NOP
140e24a79  488d8c24b0000000         LEA RCX,[RSP + 0xb0]
140e24a81  ff1599f5fd00             CALL qword ptr [0x141e04020]
140e24a87  90                       NOP
140e24a88  488d4c2468               LEA RCX,[RSP + 0x68]
140e24a8d  ff154d0ffe00             CALL qword ptr [0x141e059e0]
140e24a93  90                       NOP
140e24a94  488d8c2498000000         LEA RCX,[RSP + 0x98]
140e24a9c  ff153e0ffe00             CALL qword ptr [0x141e059e0]
140e24aa2  0f28b42450030000         MOVAPS XMM6,xmmword ptr [RSP + 0x350]
140e24aaa  4881c460030000           ADD RSP,0x360
140e24ab1  5b                       POP RBX
140e24ab2  c3                       RET
140e24ab3  cc                       INT3
140e24ab4  cc                       INT3
140e24ab5  cc                       INT3
140e24ab6  cc                       INT3
140e24ab7  cc                       INT3
140e24ab8  cc                       INT3
140e24ab9  cc                       INT3
140e24aba  cc                       INT3
140e24abb  cc                       INT3
140e24abc  cc                       INT3
140e24abd  cc                       INT3
140e24abe  cc                       INT3
140e24abf  cc                       INT3
140e24ac0  4889542410               MOV qword ptr [RSP + 0x10],RDX
140e24ac5  48894c2408               MOV qword ptr [RSP + 0x8],RCX
140e24aca  4883ec48                 SUB RSP,0x48
140e24ace  48c7442430feffffff       MOV qword ptr [RSP + 0x30],-0x2
140e24ad7  c744242800000000         MOV dword ptr [RSP + 0x28],0x0
140e24adf  488d15a2acc901           LEA RDX,[0x142abf788]
140e24ae6  488d4c2420               LEA RCX,[RSP + 0x20]
140e24aeb  ff158ff9fd00             CALL qword ptr [0x141e04480]
140e24af1  90                       NOP
140e24af2  488d542420               LEA RDX,[RSP + 0x20]
140e24af7  488b4c2458               MOV RCX,qword ptr [RSP + 0x58]
140e24afc  ff15d60efe00             CALL qword ptr [0x141e059d8]
140e24b02  8b442428                 MOV EAX,dword ptr [RSP + 0x28]
140e24b06  83c801                   OR EAX,0x1
140e24b09  89442428                 MOV dword ptr [RSP + 0x28],EAX
140e24b0d  488d4c2420               LEA RCX,[RSP + 0x20]
140e24b12  ff15c80efe00             CALL qword ptr [0x141e059e0]
140e24b18  488b442458               MOV RAX,qword ptr [RSP + 0x58]
140e24b1d  4883c448                 ADD RSP,0x48
140e24b21  c3                       RET
140e24b22  cc                       INT3
140e24b23  cc                       INT3
140e24b24  cc                       INT3
140e24b25  cc                       INT3
140e24b26  cc                       INT3
140e24b27  cc                       INT3
140e24b28  cc                       INT3
140e24b29  cc                       INT3
140e24b2a  cc                       INT3
140e24b2b  cc                       INT3
140e24b2c  cc                       INT3
140e24b2d  cc                       INT3
140e24b2e  cc                       INT3
140e24b2f  cc                       INT3

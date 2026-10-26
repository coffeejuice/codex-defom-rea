
void FUN_1411e2870(longlong param_1)

{
  char cVar1;
  longlong lVar2;
  undefined8 uVar3;
  longlong lVar4;
  undefined2 *puVar5;
  undefined1 auStack_158 [8];
  longlong lStack_150;
  int iStack_148;
  undefined1 auStack_140 [8];
  undefined1 *puStack_138;
  undefined1 auStack_130 [8];
  undefined1 *puStack_128;
  undefined1 auStack_120 [8];
  undefined1 *puStack_118;
  undefined1 auStack_110 [8];
  undefined1 auStack_108 [8];
  undefined1 *puStack_100;
  undefined1 auStack_f8 [8];
  undefined1 *puStack_f0;
  undefined1 auStack_e8 [8];
  undefined1 auStack_e0 [8];
  undefined1 auStack_d8 [8];
  undefined1 *puStack_d0;
  undefined1 auStack_c8 [8];
  undefined1 *puStack_c0;
  undefined1 auStack_b8 [8];
  undefined1 *puStack_b0;
  undefined1 auStack_a8 [8];
  undefined1 auStack_a0 [8];
  undefined1 *puStack_98;
  undefined1 auStack_90 [8];
  undefined1 *puStack_88;
  undefined1 auStack_80 [8];
  undefined8 uStack_78;
  undefined8 uStack_70;
  undefined8 uStack_68;
  undefined8 uStack_60;
  undefined8 uStack_58;
  undefined8 uStack_50;
  undefined8 uStack_48;
  undefined8 uStack_40;
  undefined8 uStack_38;
  undefined8 uStack_30;
  undefined8 uStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  
  uStack_78 = 0xfffffffffffffffe;
  lVar2 = FUN_140130af0(param_1);
  if (lVar2 != 0) {
    uVar3 = func_0x000140051230(lVar2);
    lVar4 = FUN_14146f8fc(uVar3,0,0x143579b60,0x143a76500,0);
    if ((lVar4 != 0) && (lVar4 = func_0x000140025010(lVar4), lVar4 != 0)) {
      _currentText_QComboBox__QEBA_AVQString__XZ
                (*(undefined8 *)(*(longlong *)(param_1 + 0x48) + 0x18),auStack_158);
      cVar1 = __8QString__QEBA_NPEBD_Z(auStack_158,&UNK_142cd9a28);
      if (cVar1 == '\0') {
        puStack_128 = auStack_130;
        puStack_118 = auStack_120;
        uVar3 = func_0x000140004a60(puStack_128,0);
        puVar5 = (undefined2 *)__0QChar__QEAA_D_Z(puStack_118,0x28);
        uStack_68 = _section_QString__QEBA_AV1_VQChar__HHV__QFlags_W4SectionFlag_QString_____Z
                              (auStack_158,auStack_110,*puVar5,1,1,uVar3);
        uStack_60 = uStack_68;
        __4QString__QEAAAEAV0_AEBV0__Z(auStack_158,uStack_68);
        __1QString__QEAA_XZ(auStack_110);
        puStack_100 = auStack_108;
        puStack_f0 = auStack_f8;
        uVar3 = func_0x000140004a60(puStack_100,0);
        puVar5 = (undefined2 *)__0QChar__QEAA_D_Z(puStack_f0,0x29);
        uStack_58 = _section_QString__QEBA_AV1_VQChar__HHV__QFlags_W4SectionFlag_QString_____Z
                              (auStack_158,auStack_e8,*puVar5,0,0,uVar3);
        uStack_50 = uStack_58;
        __4QString__QEAAAEAV0_AEBV0__Z(auStack_158,uStack_58);
        __1QString__QEAA_XZ(auStack_e8);
        iStack_148 = _toInt_QString__QEBAHPEA_NH_Z(auStack_158,0,10);
        func_0x0001413fd9d0(lVar4,iStack_148);
      }
      else {
        iStack_148 = FUN_140060530(lVar2);
        iStack_148 = iStack_148 + 1;
        lStack_150 = FUN_140057ba0(lVar2);
        func_0x0001413fd9d0(lVar4,*(undefined4 *)(lStack_150 + 8));
        func_0x00014146fec0(lStack_150 + 0x828,&UNK_142cd9a38);
        puStack_138 = auStack_140;
        uStack_70 = __0QString__QEAA_PEBD_Z(puStack_138,lStack_150 + 0x828);
        FUN_1400585e0(lVar2,iStack_148 + -1,uStack_70);
      }
      uStack_48 = _currentText_QComboBox__QEBA_AVQString__XZ
                            (*(undefined8 *)(*(longlong *)(param_1 + 0x48) + 0x30),auStack_e0);
      uStack_40 = uStack_48;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_158,uStack_48);
      __1QString__QEAA_XZ(auStack_e0);
      cVar1 = __8QString__QEBA_NPEBD_Z(auStack_158,&UNK_142cd9a48);
      if (cVar1 == '\0') {
        puStack_c0 = auStack_c8;
        puStack_b0 = auStack_b8;
        uVar3 = func_0x000140004a60(puStack_c0,0);
        puVar5 = (undefined2 *)__0QChar__QEAA_D_Z(puStack_b0,0x28);
        uStack_30 = _section_QString__QEBA_AV1_VQChar__HHV__QFlags_W4SectionFlag_QString_____Z
                              (auStack_158,auStack_a8,*puVar5,1,1,uVar3);
        uStack_28 = uStack_30;
        __4QString__QEAAAEAV0_AEBV0__Z(auStack_158,uStack_30);
        __1QString__QEAA_XZ(auStack_a8);
        puStack_98 = auStack_a0;
        puStack_88 = auStack_90;
        uVar3 = func_0x000140004a60(puStack_98,0);
        puVar5 = (undefined2 *)__0QChar__QEAA_D_Z(puStack_88,0x29);
        uStack_20 = _section_QString__QEBA_AV1_VQChar__HHV__QFlags_W4SectionFlag_QString_____Z
                              (auStack_158,auStack_80,*puVar5,0,0,uVar3);
        uStack_18 = uStack_20;
        __4QString__QEAAAEAV0_AEBV0__Z(auStack_158,uStack_20);
        __1QString__QEAA_XZ(auStack_80);
        iStack_148 = _toInt_QString__QEBAHPEA_NH_Z(auStack_158,0,10);
        func_0x000141371aa0(lVar4,iStack_148);
      }
      else {
        iStack_148 = FUN_140060530(lVar2);
        iStack_148 = iStack_148 + 1;
        lStack_150 = FUN_140057ba0(lVar2);
        func_0x000141371aa0(lVar4,*(undefined4 *)(lStack_150 + 8));
        func_0x00014146fec0(lStack_150 + 0x828,&UNK_142cd7c34);
        puStack_d0 = auStack_d8;
        uStack_38 = __0QString__QEAA_PEBD_Z(puStack_d0,lStack_150 + 0x828);
        FUN_1400585e0(lVar2,iStack_148 + -1,uStack_38);
      }
      __1QString__QEAA_XZ(auStack_158);
    }
  }
  return;
}


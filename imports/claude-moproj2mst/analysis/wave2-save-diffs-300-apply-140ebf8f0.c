
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ebf8f0(longlong *param_1,longlong param_2)

{
  char cVar1;
  int iVar2;
  undefined4 uVar3;
  longlong lVar4;
  undefined1 *puVar5;
  undefined8 uVar6;
  double dVar7;
  undefined4 uVar8;
  undefined4 uStack_278;
  undefined4 uStack_274;
  undefined1 auStack_270 [40];
  undefined4 uStack_248;
  char cStack_244;
  undefined1 auStack_240 [8];
  undefined1 auStack_238 [8];
  int iStack_230;
  undefined1 auStack_228 [8];
  undefined1 auStack_220 [8];
  undefined1 auStack_218 [8];
  undefined1 auStack_210 [8];
  undefined1 auStack_208 [8];
  char cStack_200;
  undefined1 auStack_1f8 [8];
  undefined1 auStack_1f0 [8];
  undefined1 auStack_1e8 [8];
  undefined1 auStack_1e0 [8];
  undefined1 auStack_1d8 [8];
  undefined1 auStack_1d0 [8];
  undefined1 auStack_1c8 [8];
  undefined1 auStack_1c0 [2];
  undefined1 auStack_1be [6];
  undefined1 auStack_1b8 [8];
  undefined1 auStack_1b0 [8];
  undefined1 auStack_1a8 [8];
  undefined1 auStack_1a0 [8];
  undefined1 auStack_198 [8];
  undefined1 auStack_190 [8];
  undefined1 auStack_188 [8];
  undefined1 auStack_180 [8];
  undefined1 auStack_178 [8];
  undefined1 auStack_170 [8];
  undefined1 auStack_168 [8];
  undefined1 auStack_160 [4];
  uint uStack_15c;
  undefined8 uStack_158;
  undefined8 uStack_150;
  undefined8 uStack_148;
  undefined8 uStack_140;
  undefined8 uStack_138;
  undefined8 uStack_130;
  undefined8 uStack_128;
  undefined8 uStack_120;
  undefined8 uStack_118;
  undefined8 uStack_110;
  undefined8 uStack_108;
  undefined8 uStack_100;
  undefined8 uStack_f8;
  undefined8 uStack_f0;
  undefined8 uStack_e8;
  undefined8 uStack_e0;
  undefined8 uStack_d8;
  undefined8 uStack_d0;
  undefined8 uStack_c8;
  undefined8 uStack_c0;
  undefined8 uStack_b8;
  undefined4 uStack_b0;
  undefined8 uStack_a8;
  undefined8 uStack_a0;
  undefined8 uStack_98;
  undefined8 uStack_90;
  undefined8 uStack_88;
  undefined8 uStack_80;
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
  undefined8 uStack_10;
  
  uStack_158 = 0xfffffffffffffffe;
  uStack_15c = 0;
  uStack_248 = 0;
  __0QString__QEAA_XZ(auStack_240);
  __0QDoubleValidator__QEAA_PEAVQObject___Z(auStack_270,param_1);
  uStack_150 = _text_QLineEdit__QEBA_AVQString__XZ
                         (*(undefined8 *)(param_1[0x28] + 0x548),auStack_220);
  uStack_148 = uStack_150;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_150);
  __1QString__QEAA_XZ(auStack_220);
  iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                    (auStack_270,auStack_240,&uStack_248);
  if (iVar2 == 0) {
    __1QDoubleValidator__UEAA_XZ(auStack_270);
    __1QString__QEAA_XZ(auStack_240);
    return;
  }
  uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
  *(undefined8 *)(param_2 + 0x178) = uVar6;
  uStack_140 = _text_QLineEdit__QEBA_AVQString__XZ
                         (*(undefined8 *)(param_1[0x28] + 0x580),auStack_218);
  uStack_138 = uStack_140;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_140);
  __1QString__QEAA_XZ(auStack_218);
  iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                    (auStack_270,auStack_240,&uStack_248);
  if (iVar2 == 0) {
    __1QDoubleValidator__UEAA_XZ(auStack_270);
    __1QString__QEAA_XZ(auStack_240);
    return;
  }
  uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
  *(undefined8 *)(param_2 + 0x180) = uVar6;
  uStack_130 = _text_QLineEdit__QEBA_AVQString__XZ
                         (*(undefined8 *)(param_1[0x28] + 0x590),auStack_210);
  uStack_128 = uStack_130;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_130);
  __1QString__QEAA_XZ(auStack_210);
  iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                    (auStack_270,auStack_240,&uStack_248);
  if (iVar2 == 0) {
    __1QDoubleValidator__UEAA_XZ(auStack_270);
    __1QString__QEAA_XZ(auStack_240);
    return;
  }
  uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
  *(undefined8 *)(param_2 + 0x188) = uVar6;
  cVar1 = FUN_140130ee0(param_1);
  if (cVar1 != '\0') {
    uStack_120 = _text_QLineEdit__QEBA_AVQString__XZ
                           (*(undefined8 *)(param_1[0x28] + 0x5a0),auStack_208);
    uStack_118 = uStack_120;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_120);
    __1QString__QEAA_XZ(auStack_208);
    iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_270,auStack_240,&uStack_248);
    if (iVar2 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_270);
      __1QString__QEAA_XZ(auStack_240);
      return;
    }
    uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
    *(undefined8 *)(param_2 + 400) = uVar6;
  }
  uVar6 = (**(code **)(*param_1 + 0x198))(param_1);
  lVar4 = FUN_140c7ae80(uVar6,*(undefined4 *)(param_2 + 0x168));
  cStack_244 = '\x01';
  if ((lVar4 == 0) ||
     ((((*(int *)(lVar4 + 0x21c8) != 3 && (*(int *)(lVar4 + 0x21c8) != 4)) &&
       (*(int *)(lVar4 + 0x21c8) != 7)) &&
      (((*(int *)(lVar4 + 0x21c8) != 8 && (*(int *)(lVar4 + 0x21c8) != 9)) &&
       (*(int *)(lVar4 + 0x21c8) != 10)))))) goto code_r0x000140ec0273;
  uStack_110 = _text_QLineEdit__QEBA_AVQString__XZ
                         (*(undefined8 *)(param_1[0x28] + 0x620),auStack_1f8);
  uStack_15c = uStack_15c | 1;
  uStack_108 = uStack_110;
  dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(uStack_110,0);
  dVar7 = (double)FUN_141470a78(*(double *)(param_2 + 0x198) - dVar7);
  if (_UNK_142157420 < dVar7) {
code_r0x000140ec000a:
    uStack_b0 = 1;
  }
  else {
    uStack_100 = _text_QLineEdit__QEBA_AVQString__XZ
                           (*(undefined8 *)(param_1[0x28] + 0x630),auStack_1f0);
    uStack_15c = uStack_15c | 2;
    uStack_f8 = uStack_100;
    dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(uStack_100,0);
    dVar7 = (double)FUN_141470a78(*(double *)(param_2 + 0x1a0) - dVar7);
    if (_UNK_142157420 < dVar7) goto code_r0x000140ec000a;
    uStack_f0 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(param_1[0x28] + 0x640),auStack_1e8);
    uStack_15c = uStack_15c | 4;
    uStack_e8 = uStack_f0;
    dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(uStack_f0,0);
    dVar7 = (double)FUN_141470a78(*(double *)(param_2 + 0x1a8) - dVar7);
    if (_UNK_142157420 < dVar7) goto code_r0x000140ec000a;
    uStack_e0 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(param_1[0x28] + 0x5e8),auStack_1e0);
    uStack_15c = uStack_15c | 8;
    uStack_d8 = uStack_e0;
    dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(uStack_e0,0);
    dVar7 = (double)FUN_141470a78(*(double *)(param_2 + 0x1b0) - dVar7);
    if (_UNK_142157420 < dVar7) goto code_r0x000140ec000a;
    uStack_d0 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(param_1[0x28] + 0x5f8),auStack_1d8);
    uStack_15c = uStack_15c | 0x10;
    uStack_c8 = uStack_d0;
    dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(uStack_d0,0);
    dVar7 = (double)FUN_141470a78(*(double *)(param_2 + 0x1b8) - dVar7);
    if (_UNK_142157420 < dVar7) goto code_r0x000140ec000a;
    uStack_c0 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(param_1[0x28] + 0x608),auStack_1d0);
    uStack_15c = uStack_15c | 0x20;
    uStack_b8 = uStack_c0;
    dVar7 = (double)_toDouble_QString__QEBANPEA_N_Z(uStack_c0,0);
    dVar7 = (double)FUN_141470a78(*(double *)(param_2 + 0x1c0) - dVar7);
    if (_UNK_142157420 < dVar7) goto code_r0x000140ec000a;
    uStack_b0 = 0;
  }
  cStack_200 = (char)uStack_b0;
  if ((uStack_15c & 0x20) != 0) {
    uStack_15c = uStack_15c & 0xffffffdf;
    __1QString__QEAA_XZ(auStack_1d0);
  }
  if ((uStack_15c & 0x10) != 0) {
    uStack_15c = uStack_15c & 0xffffffef;
    __1QString__QEAA_XZ(auStack_1d8);
  }
  if ((uStack_15c & 8) != 0) {
    uStack_15c = uStack_15c & 0xfffffff7;
    __1QString__QEAA_XZ(auStack_1e0);
  }
  if ((uStack_15c & 4) != 0) {
    uStack_15c = uStack_15c & 0xfffffffb;
    __1QString__QEAA_XZ(auStack_1e8);
  }
  if ((uStack_15c & 2) != 0) {
    uStack_15c = uStack_15c & 0xfffffffd;
    __1QString__QEAA_XZ(auStack_1f0);
  }
  if ((uStack_15c & 1) != 0) {
    uStack_15c = uStack_15c & 0xfffffffe;
    __1QString__QEAA_XZ(auStack_1f8);
  }
  if (cStack_200 != '\0') {
    FUN_1402ca7a0(0x143d766b8,0);
    FUN_140f34710(auStack_238,&UNK_142b14b48,0);
    puVar5 = (undefined1 *)func_0x000140002860(auStack_1be,0x20);
    __0QChar__QEAA_UQLatin1Char___Z(auStack_1c0,*puVar5);
    __0QString__QEAA_PEBD_Z(auStack_1b8,lVar4 + 0x828);
    uStack_a8 = FUN_140f34710(auStack_1c8,&UNK_142b14b60,0);
    puVar5 = auStack_1c0;
    uStack_a0 = uStack_a8;
    _arg_QString__QEBA_AV1_AEBV1_HAEBVQChar___Z(uStack_a8,auStack_228,auStack_1b8,0,puVar5);
    uVar8 = (undefined4)((ulonglong)puVar5 >> 0x20);
    __1QString__QEAA_XZ(auStack_1c8);
    __1QString__QEAA_XZ(auStack_1b8);
    uVar6 = FUN_140031680(auStack_1b0,0x10000,0x100);
    uVar3 = func_0x0001413ee240(uVar6);
    iStack_230 = _question_QMessageBox__SAHPEAVQWidget__AEBVQString__1HHH_Z
                           (param_1,auStack_238,auStack_228,0x4000,CONCAT44(uVar8,uVar3),0);
    if (iStack_230 == 0x10000) {
      cStack_244 = '\0';
    }
    __1QString__QEAA_XZ(auStack_228);
    __1QString__QEAA_XZ(auStack_238);
  }
code_r0x000140ec0273:
  if (cStack_244 != '\0') {
    uStack_98 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(param_1[0x28] + 0x620),auStack_1a8);
    uStack_90 = uStack_98;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_98);
    __1QString__QEAA_XZ(auStack_1a8);
    iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_270,auStack_240,&uStack_248);
    if (iVar2 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_270);
      __1QString__QEAA_XZ(auStack_240);
      return;
    }
    uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
    *(undefined8 *)(param_2 + 0x198) = uVar6;
    uStack_88 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(param_1[0x28] + 0x630),auStack_1a0);
    uStack_80 = uStack_88;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_88);
    __1QString__QEAA_XZ(auStack_1a0);
    iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_270,auStack_240,&uStack_248);
    if (iVar2 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_270);
      __1QString__QEAA_XZ(auStack_240);
      return;
    }
    uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
    *(undefined8 *)(param_2 + 0x1a0) = uVar6;
    cVar1 = FUN_140130ee0(param_1);
    if (cVar1 != '\0') {
      uStack_78 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(param_1[0x28] + 0x640),auStack_198);
      uStack_70 = uStack_78;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_78);
      __1QString__QEAA_XZ(auStack_198);
      iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                        (auStack_270,auStack_240,&uStack_248);
      if (iVar2 == 0) {
        __1QDoubleValidator__UEAA_XZ(auStack_270);
        __1QString__QEAA_XZ(auStack_240);
        return;
      }
      uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
      *(undefined8 *)(param_2 + 0x1a8) = uVar6;
    }
    uStack_68 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(param_1[0x28] + 0x5e8),auStack_190);
    uStack_60 = uStack_68;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_68);
    __1QString__QEAA_XZ(auStack_190);
    iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_270,auStack_240,&uStack_248);
    if (iVar2 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_270);
      __1QString__QEAA_XZ(auStack_240);
      return;
    }
    uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
    *(undefined8 *)(param_2 + 0x1b0) = uVar6;
    uStack_58 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(param_1[0x28] + 0x5f8),auStack_188);
    uStack_50 = uStack_58;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_58);
    __1QString__QEAA_XZ(auStack_188);
    iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_270,auStack_240,&uStack_248);
    if (iVar2 == 0) {
      __1QDoubleValidator__UEAA_XZ(auStack_270);
      __1QString__QEAA_XZ(auStack_240);
      return;
    }
    uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
    *(undefined8 *)(param_2 + 0x1b8) = uVar6;
    cVar1 = FUN_140130ee0(param_1);
    if (cVar1 != '\0') {
      uStack_48 = _text_QLineEdit__QEBA_AVQString__XZ
                            (*(undefined8 *)(param_1[0x28] + 0x608),auStack_180);
      uStack_40 = uStack_48;
      __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_48);
      __1QString__QEAA_XZ(auStack_180);
      iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                        (auStack_270,auStack_240,&uStack_248);
      if (iVar2 == 0) {
        __1QDoubleValidator__UEAA_XZ(auStack_270);
        __1QString__QEAA_XZ(auStack_240);
        return;
      }
      uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
      *(undefined8 *)(param_2 + 0x1c0) = uVar6;
    }
  }
  uStack_38 = _text_QLineEdit__QEBA_AVQString__XZ
                        (*(undefined8 *)(param_1[0x28] + 0x648),auStack_178);
  uStack_30 = uStack_38;
  __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_38);
  __1QString__QEAA_XZ(auStack_178);
  iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                    (auStack_270,auStack_240,&uStack_248);
  if (iVar2 == 0) {
    __1QDoubleValidator__UEAA_XZ(auStack_270);
    __1QString__QEAA_XZ(auStack_240);
  }
  else {
    uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
    *(undefined8 *)(param_2 + 0x170) = uVar6;
    uStack_28 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(param_1[0x28] + 0x7d8),auStack_170);
    uStack_20 = uStack_28;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_240,uStack_28);
    __1QString__QEAA_XZ(auStack_170);
    iVar2 = _validate_QDoubleValidator__UEBA_AW4State_QValidator__AEAVQString__AEAH_Z
                      (auStack_270,auStack_240,&uStack_248);
    if (iVar2 == 0) {
      uStack_18 = _qDebug__YA_AVQDebug__XZ(auStack_168);
      uStack_10 = uStack_18;
      uVar6 = __6QDebug__QEAAAEAV0_PEBD_Z(uStack_18,&UNK_142b14bf4);
      uVar6 = __6QDebug__QEAAAEAV0_AEBVQString___Z(uVar6,auStack_240);
      __6QDebug__QEAAAEAV0_PEBD_Z(uVar6,&UNK_142b14c00);
      __1QDebug__QEAA_XZ(auStack_168);
    }
    uVar6 = _toDouble_QString__QEBANPEA_N_Z(auStack_240,0);
    *(undefined8 *)(param_2 + 0x298) = uVar6;
    FUN_140eb6210(param_1,&uStack_274,&uStack_278);
    *(undefined4 *)(param_2 + 0x288) = uStack_274;
    *(undefined4 *)(param_2 + 0x28c) = uStack_278;
    uVar3 = _currentIndex_QComboBox__QEBAHXZ(*(undefined8 *)(param_1[0x28] + 0x7c0));
    *(undefined4 *)(param_2 + 0x290) = uVar3;
    FUN_140eb6da0(param_1,0);
    FUN_140eb6da0(param_1,1);
    auStack_160[0] = 0;
    FUN_140ec09a0(param_1,auStack_160);
    FUN_140eb71e0(param_1,0);
    __1QDoubleValidator__UEAA_XZ(auStack_270);
    __1QString__QEAA_XZ(auStack_240);
  }
  return;
}


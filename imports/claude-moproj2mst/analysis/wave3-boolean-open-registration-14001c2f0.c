
longlong FUN_14001c2f0(longlong param_1,undefined8 param_2)

{
  char cVar1;
  undefined2 *puVar2;
  undefined8 uVar3;
  longlong lVar4;
  ulonglong uVar5;
  undefined1 auStack_240 [8];
  longlong lStack_238;
  int iStack_230;
  undefined1 auStack_228 [40];
  undefined1 auStack_200 [8];
  undefined1 auStack_1f8 [16];
  undefined1 auStack_1e8 [8];
  undefined1 auStack_1e0 [8];
  undefined1 auStack_1d8 [8];
  longlong lStack_1d0;
  undefined1 auStack_1c8 [8];
  undefined1 auStack_1c0 [8];
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
  undefined1 auStack_160 [8];
  undefined1 auStack_158 [8];
  undefined1 auStack_150 [8];
  longlong lStack_148;
  undefined1 uStack_140;
  char cStack_13f;
  undefined1 auStack_13e [6];
  undefined1 auStack_138 [8];
  undefined1 auStack_130 [8];
  undefined1 auStack_128 [8];
  longlong lStack_120;
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
  undefined8 uStack_b0;
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
  
  uStack_118 = 0xfffffffffffffffe;
  FUN_140819590(auStack_1e0,param_2);
  uStack_110 = FUN_140819090(auStack_1c0,param_2);
  uStack_108 = uStack_110;
  FUN_140002110(auStack_1e8,uStack_110,&UNK_1420fa550);
  __1QString__QEAA_XZ(auStack_1c0);
  puVar2 = (undefined2 *)_separator_QDir__SA_AVQChar__XZ(auStack_1b8);
  uStack_100 = FUN_140002110(auStack_1b0,auStack_1e0,&UNK_1420fa558);
  uStack_f8 = uStack_100;
  uStack_f0 = _cleanPath_QDir__SA_AVQString__AEBV2__Z(auStack_1a8,uStack_100);
  uStack_e8 = uStack_f0;
  uStack_e0 = FUN_140004970(auStack_1a0,uStack_f0,*puVar2);
  uStack_d8 = uStack_e0;
  uStack_d0 = FUN_1400048f0(auStack_198,uStack_e0,auStack_1e8);
  uStack_c8 = uStack_d0;
  __0QDir__QEAA_AEBVQString___Z(auStack_240,uStack_d0);
  __1QString__QEAA_XZ(auStack_198);
  __1QString__QEAA_XZ(auStack_1a0);
  __1QString__QEAA_XZ(auStack_1a8);
  __1QString__QEAA_XZ(auStack_1b0);
  cVar1 = _exists_QDir__QEBA_NXZ(auStack_240);
  if (cVar1 != '\0') {
    uStack_c0 = _toLatin1_QString__QEBA_AVQByteArray__XZ(auStack_1e8,auStack_188);
    uStack_b8 = uStack_c0;
    uVar3 = _data_QByteArray__QEAAPEADXZ(uStack_c0);
    uVar3 = __0QLatin1String__QEAA_PEBD_Z(auStack_190,uVar3);
    uStack_b0 = FUN_140002110(auStack_180,auStack_1e0,&UNK_1420fa560);
    uStack_a8 = uStack_b0;
    uStack_a0 = _cleanPath_QDir__SA_AVQString__AEBV2__Z(auStack_178,uStack_b0);
    uStack_98 = uStack_a0;
    uStack_90 = FUN_140ca4240(param_1,auStack_170,uStack_a0,uVar3);
    uStack_88 = uStack_90;
    uStack_80 = FUN_140819090(auStack_168,uStack_90);
    uStack_78 = uStack_80;
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1e8,uStack_80);
    __1QString__QEAA_XZ(auStack_168);
    __1QString__QEAA_XZ(auStack_170);
    __1QString__QEAA_XZ(auStack_178);
    __1QString__QEAA_XZ(auStack_180);
    __1QByteArray__QEAA_XZ(auStack_188);
  }
  uStack_70 = FUN_140002110(auStack_160,auStack_1e0,&UNK_1420fa568);
  uStack_68 = uStack_70;
  uStack_60 = _cleanPath_QDir__SA_AVQString__AEBV2__Z(auStack_158,uStack_70);
  uStack_58 = uStack_60;
  uStack_50 = FUN_140819090(auStack_150,param_2);
  uVar5 = 0;
  uStack_48 = uStack_50;
  FUN_14012de80(auStack_228,uStack_50,auStack_1e8,uStack_58,0);
  __1QString__QEAA_XZ(auStack_150);
  __1QString__QEAA_XZ(auStack_158);
  __1QString__QEAA_XZ(auStack_160);
  iStack_230 = _exec_QDialog__QEAAHXZ(auStack_228);
  if (iStack_230 == 0) {
    lStack_148 = 0;
    FUN_14012e180(auStack_228);
    __1QDir__QEAA_XZ(auStack_240);
    __1QString__QEAA_XZ(auStack_1e8);
    __1QString__QEAA_XZ(auStack_1e0);
  }
  else {
    __4QString__QEAAAEAV0_AEBV0__Z(auStack_1e8,auStack_200);
    __0QString__QEAA_AEBV0__Z(auStack_1d8,auStack_1f8);
    lStack_238 = 0;
    uVar3 = func_0x000140c9b7c0(param_1);
    lVar4 = FUN_140c78e80(uVar3,auStack_1e8);
    if (lVar4 != 0) {
      lStack_1d0 = FUN_14146f8fc(lVar4,0,0x143569448,0x143569470,uVar5 & 0xffffffff00000000);
      if (lStack_1d0 != 0) {
        lStack_238 = FUN_140044a90(lStack_1d0);
        uVar3 = func_0x000140025010(*(undefined8 *)(param_1 + 0x218));
        _setEnabled_QAction__QEAAX_N_Z(uVar3,0);
      }
    }
    if (lStack_238 != 0) {
      uStack_140 = 1;
      FUN_14008a5c0(lStack_238,auStack_1e8,auStack_1d8,&uStack_140);
      puVar2 = (undefined2 *)_separator_QDir__SA_AVQChar__XZ(auStack_13e);
      uStack_40 = FUN_140004970(auStack_138,auStack_1d8,*puVar2);
      uStack_38 = uStack_40;
      uStack_30 = FUN_1400048f0(auStack_130,uStack_40,auStack_1e8);
      uStack_28 = uStack_30;
      cStack_13f = FUN_1400b12c0(auStack_1c8,param_2,lStack_238,uStack_30);
      __1QString__QEAA_XZ(auStack_130);
      __1QString__QEAA_XZ(auStack_138);
      if (cStack_13f != '\0') {
        FUN_1400980d0(lStack_238);
        uStack_20 = FUN_14008a4b0(lStack_238,auStack_128);
        uStack_18 = uStack_20;
        FUN_14001dc00(param_1,uStack_20);
        __1QString__QEAA_XZ(auStack_128);
      }
    }
    if (*(longlong *)(param_1 + 0x478) != 0) {
      FUN_140cf6210(*(undefined8 *)(param_1 + 0x478));
    }
    lStack_120 = lStack_238;
    __1QString__QEAA_XZ(auStack_1d8);
    FUN_14012e180(auStack_228);
    __1QDir__QEAA_XZ(auStack_240);
    __1QString__QEAA_XZ(auStack_1e8);
    __1QString__QEAA_XZ(auStack_1e0);
    lStack_148 = lStack_120;
  }
  return lStack_148;
}


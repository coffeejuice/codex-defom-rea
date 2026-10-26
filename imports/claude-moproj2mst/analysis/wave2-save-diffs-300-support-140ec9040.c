
void FUN_140ec9040(longlong param_1)

{
  undefined1 auVar1 [16];
  char cVar2;
  int iVar3;
  undefined8 uVar4;
  undefined1 *puVar5;
  longlong lVar6;
  undefined1 *puVar7;
  undefined1 auStack_1e8 [8];
  undefined1 auStack_1e0 [16];
  int iStack_1d0;
  longlong lStack_1c8;
  longlong lStack_1c0;
  int iStack_1b8;
  undefined1 auStack_1b0 [8];
  undefined1 auStack_1a8 [8];
  undefined1 auStack_1a0 [8];
  undefined1 auStack_198 [8];
  undefined1 auStack_190 [8];
  undefined1 auStack_188 [2];
  undefined1 auStack_186 [6];
  undefined1 auStack_180 [8];
  undefined1 auStack_178 [8];
  undefined1 auStack_170 [8];
  undefined1 auStack_168 [8];
  undefined1 auStack_160 [8];
  undefined1 auStack_158 [8];
  undefined1 auStack_150 [8];
  undefined1 auStack_148 [8];
  undefined1 auStack_140 [8];
  undefined1 auStack_138 [8];
  undefined1 auStack_130 [8];
  undefined1 auStack_128 [8];
  undefined1 auStack_120 [8];
  ulonglong uStack_118;
  longlong lStack_110;
  longlong lStack_108;
  undefined1 auStack_100 [64];
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
  longlong lStack_28;
  
  uStack_c0 = 0xfffffffffffffffe;
  FUN_1400653e0(auStack_1e8);
  FUN_1410da4d0(auStack_1e0);
  cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x14f0));
  if (cVar2 == '\0') {
    iVar3 = FUN_1407fbc10(&UNK_142b14f20);
    if (iVar3 == 0) {
      puVar5 = (undefined1 *)func_0x000140002860(auStack_186,0x20);
      __0QChar__QEAA_UQLatin1Char___Z(auStack_188,*puVar5);
      __0QString__QEAA_PEBD_Z(auStack_180,&UNK_142b14f48);
      uStack_98 = FUN_140f34710(auStack_190,&UNK_142b14f28,0);
      uStack_90 = uStack_98;
      uStack_88 = _arg_QString__QEBA_AV1_AEBV1_HAEBVQChar___Z
                            (uStack_98,auStack_178,auStack_180,0,auStack_188);
      uStack_80 = uStack_88;
      FUN_1402d0bd0(0x143d76350,uStack_88,2000,0);
      __1QString__QEAA_XZ(auStack_178);
      __1QString__QEAA_XZ(auStack_190);
      __1QString__QEAA_XZ(auStack_180);
    }
  }
  else {
    __0QString__QEAA_PEBD_Z(auStack_1a8,&UNK_142b14f10);
    uStack_b8 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x14c0),auStack_1b0);
    uStack_b0 = uStack_b8;
    uVar4 = FUN_140cca3c0(auStack_1e8,uStack_b8);
    FUN_140cca3c0(uVar4,auStack_1a8);
    __1QString__QEAA_XZ(auStack_1b0);
    __1QString__QEAA_XZ(auStack_1a8);
    uStack_a8 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x14e0),auStack_1a0);
    uStack_a0 = uStack_a8;
    FUN_140cca3c0(auStack_1e8,uStack_a8);
    __1QString__QEAA_XZ(auStack_1a0);
    __0QString__QEAA_PEBD_Z(auStack_198,&UNK_142b14f18);
    FUN_1410da9e0(auStack_1e0,auStack_198,auStack_1e8);
    __1QString__QEAA_XZ(auStack_198);
  }
  cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1480));
  if (cVar2 == '\0') {
    FUN_1407fbc10(&UNK_142b14f60);
  }
  else {
    FUN_1400aced0(auStack_1e8);
    uStack_78 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1510),auStack_170);
    uStack_70 = uStack_78;
    FUN_140cca3c0(auStack_1e8,uStack_78);
    __1QString__QEAA_XZ(auStack_170);
    __0QString__QEAA_PEBD_Z(auStack_168,&UNK_142b14f50);
    FUN_1410da9e0(auStack_1e0,auStack_168,auStack_1e8);
    __1QString__QEAA_XZ(auStack_168);
  }
  cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1538));
  if (cVar2 == '\0') {
    FUN_1407fbc10(&UNK_142b14f88);
  }
  else {
    FUN_1400aced0(auStack_1e8);
    __0QString__QEAA_PEBD_Z(auStack_158,&UNK_142b14f6c);
    uStack_68 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1550),auStack_160);
    uStack_60 = uStack_68;
    uVar4 = FUN_140cca3c0(auStack_1e8,uStack_68);
    FUN_140cca3c0(uVar4,auStack_158);
    __1QString__QEAA_XZ(auStack_160);
    __1QString__QEAA_XZ(auStack_158);
    __0QString__QEAA_PEBD_Z(auStack_148,&UNK_142b14f70);
    uStack_58 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1560),auStack_150);
    uStack_50 = uStack_58;
    uVar4 = FUN_140cca3c0(auStack_1e8,uStack_58);
    FUN_140cca3c0(uVar4,auStack_148);
    __1QString__QEAA_XZ(auStack_150);
    __1QString__QEAA_XZ(auStack_148);
    uStack_48 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1570),auStack_140);
    uStack_40 = uStack_48;
    FUN_140cca3c0(auStack_1e8,uStack_48);
    __1QString__QEAA_XZ(auStack_140);
    __0QString__QEAA_PEBD_Z(auStack_138,&UNK_142b14f78);
    FUN_1410da9e0(auStack_1e0,auStack_138,auStack_1e8);
    __1QString__QEAA_XZ(auStack_138);
  }
  cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x15c0));
  if (cVar2 == '\0') {
    FUN_1407fbc10(&UNK_142b14fa8);
  }
  else {
    FUN_1400aced0(auStack_1e8);
    uStack_38 = _text_QLineEdit__QEBA_AVQString__XZ
                          (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x15b8),auStack_130);
    uStack_30 = uStack_38;
    FUN_140cca3c0(auStack_1e8,uStack_38);
    __1QString__QEAA_XZ(auStack_130);
    __0QString__QEAA_PEBD_Z(auStack_128,&UNK_142b14f98);
    FUN_1410da9e0(auStack_1e0,auStack_128,auStack_1e8);
    __1QString__QEAA_XZ(auStack_128);
  }
  cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x15c8));
  if (cVar2 == '\0') {
    FUN_1407fbc10(&UNK_142b14fc8);
  }
  else {
    FUN_1400aced0(auStack_1e8);
    __0QString__QEAA_PEBD_Z(auStack_120,&UNK_142b14fb8);
    FUN_1410da9e0(auStack_1e0,auStack_120,auStack_1e8);
    __1QString__QEAA_XZ(auStack_120);
  }
  iStack_1d0 = _count_QListWidget__QEBAHXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1610))
  ;
  cVar2 = _isChecked_QAbstractButton__QEBA_NXZ
                    (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x15f0));
  if ((cVar2 == '\0') || (iStack_1d0 < 1)) {
    FUN_1407fbc10(&UNK_142b14fd8);
    FUN_1407fbc10(&UNK_142b14fe8);
  }
  else {
    uVar4 = _currentItem_QListWidget__QEBAPEAVQListWidgetItem__XZ
                      (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1610));
    FUN_140ecdee0(param_1,uVar4);
    uStack_118 = (ulonglong)iStack_1d0;
    auVar1._8_8_ = 0;
    auVar1._0_8_ = uStack_118;
    uVar4 = SUB168(ZEXT816(0x40) * auVar1,0);
    if (SUB168(ZEXT816(0x40) * auVar1,8) != 0) {
      uVar4 = 0xffffffffffffffff;
    }
    lStack_108 = func_0x00014146c25c(uVar4);
    if (lStack_108 == 0) {
      lStack_28 = 0;
    }
    else {
      FUN_1400020c0(lStack_108,0x40,uStack_118 & 0xffffffff,&UNK_1410da450);
      lStack_28 = lStack_108;
    }
    lStack_110 = lStack_28;
    lStack_1c8 = lStack_28;
    if (lStack_28 != 0) {
      for (iStack_1b8 = 0; iStack_1b8 < iStack_1d0; iStack_1b8 = iStack_1b8 + 1) {
        lStack_1c0 = _item_QListWidget__QEBAPEAVQListWidgetItem__H_Z
                               (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1610),iStack_1b8)
        ;
        puVar5 = (undefined1 *)(lStack_1c0 + 0x30);
        puVar7 = auStack_100;
        for (lVar6 = 0x40; lVar6 != 0; lVar6 = lVar6 + -1) {
          *puVar7 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar7 = puVar7 + 1;
        }
        puVar5 = auStack_100;
        puVar7 = (undefined1 *)(lStack_1c8 + (longlong)iStack_1b8 * 0x40);
        for (lVar6 = 0x40; lVar6 != 0; lVar6 = lVar6 + -1) {
          *puVar7 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar7 = puVar7 + 1;
        }
      }
      FUN_1410db030(auStack_1e0,lStack_1c8,iStack_1d0);
    }
  }
  FUN_140f34760(auStack_1e0);
  FUN_1400048d0(auStack_1e8);
  return;
}


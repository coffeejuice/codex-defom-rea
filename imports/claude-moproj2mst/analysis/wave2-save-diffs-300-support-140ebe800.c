
void FUN_140ebe800(longlong *param_1)

{
  char cVar1;
  undefined8 uVar2;
  longlong lVar3;
  longlong lStack_58;
  int aiStack_50 [2];
  undefined1 auStack_48 [8];
  undefined1 auStack_40 [8];
  undefined1 auStack_38 [8];
  undefined1 auStack_30 [8];
  undefined1 auStack_28 [8];
  undefined1 auStack_20 [8];
  undefined1 auStack_18 [8];
  undefined8 uStack_10;
  
  uStack_10 = 0xfffffffffffffffe;
  lStack_58 = 0;
  __0QString__QEAA_XZ(auStack_48);
  FUN_140eb62e0(param_1,aiStack_50);
  uVar2 = (**(code **)(*param_1 + 0x198))(param_1);
  lVar3 = FUN_140c7ae80(uVar2,aiStack_50[0]);
  _setCurrentIndex_QComboBox__QEAAXH_Z(*(undefined8 *)(param_1[0x28] + 0x838),aiStack_50[0] + -1);
  if (lVar3 != 0) {
    lStack_58 = lVar3 + 0x2160;
  }
  if (lStack_58 == 0) {
    __0QString__QEAA_PEBD_Z(auStack_40,&UNK_142b14aa0);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x888),auStack_40);
    __1QString__QEAA_XZ(auStack_40);
    __0QString__QEAA_PEBD_Z(auStack_38,&UNK_142b14aa4);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x890),auStack_38);
    __1QString__QEAA_XZ(auStack_38);
    cVar1 = FUN_140130ee0(param_1);
    if (cVar1 != '\0') {
      __0QString__QEAA_PEBD_Z(auStack_30,&UNK_142b14aa8);
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x898),auStack_30);
      __1QString__QEAA_XZ(auStack_30);
    }
    __0QString__QEAA_PEBD_Z(auStack_28,&UNK_142b14aac);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x848),auStack_28);
    __1QString__QEAA_XZ(auStack_28);
    __0QString__QEAA_PEBD_Z(auStack_20,&UNK_142b14ab0);
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x850),auStack_20);
    __1QString__QEAA_XZ(auStack_20);
    cVar1 = FUN_140130ee0(param_1);
    if (cVar1 != '\0') {
      __0QString__QEAA_PEBD_Z(auStack_18,&UNK_142b14ab4);
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x858),auStack_18);
      __1QString__QEAA_XZ(auStack_18);
    }
  }
  else {
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_48,&UNK_142b14a70,*(undefined8 *)(lStack_58 + 0x888))
    ;
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x888),auStack_48);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_48,&UNK_142b14a78,*(undefined8 *)(lStack_58 + 0x890))
    ;
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x890),auStack_48);
    cVar1 = FUN_140130ee0(param_1);
    if (cVar1 != '\0') {
      _sprintf_QString__QEAAAEAV1_PEBDZZ
                (auStack_48,&UNK_142b14a80,*(undefined8 *)(lStack_58 + 0x898));
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x898),auStack_48);
    }
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_48,&UNK_142b14a88,*(undefined8 *)(lStack_58 + 0x8a0))
    ;
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x848),auStack_48);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_48,&UNK_142b14a90,*(undefined8 *)(lStack_58 + 0x8a8))
    ;
    _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x850),auStack_48);
    cVar1 = FUN_140130ee0(param_1);
    if (cVar1 != '\0') {
      _sprintf_QString__QEAAAEAV1_PEBDZZ
                (auStack_48,&UNK_142b14a98,*(undefined8 *)(lStack_58 + 0x8b0));
      _setText_QLineEdit__QEAAXAEBVQString___Z(*(undefined8 *)(param_1[0x28] + 0x858),auStack_48);
    }
  }
  __1QString__QEAA_XZ(auStack_48);
  return;
}


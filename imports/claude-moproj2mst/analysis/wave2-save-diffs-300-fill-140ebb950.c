
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_140ebb950(longlong param_1,longlong param_2)

{
  char cVar1;
  int iStack_48;
  undefined1 auStack_40 [8];
  int iStack_38;
  undefined1 auStack_30 [8];
  undefined1 uStack_28;
  undefined4 uStack_24;
  undefined8 uStack_20;
  int iStack_18;
  undefined1 uStack_14;
  bool bStack_13;
  
  uStack_20 = 0xfffffffffffffffe;
  __0QString__QEAA_XZ(auStack_30);
  __0QString__QEAA_XZ(auStack_40);
  cVar1 = FUN_140130ea0(param_1);
  if (cVar1 == '\0') {
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x468));
    _setCurrentIndex_QStackedWidget__QEAAXH_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x358),1);
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x4b0));
  }
  else {
    _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x468));
    _setCurrentIndex_QStackedWidget__QEAAXH_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x358),0);
    _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x4b0));
  }
  iStack_48 = func_0x0001408da6d0(param_2);
  iStack_38 = iStack_48;
  if (*(int *)(param_2 + 0x1c8) == 3) {
    iStack_48 = 2;
    iStack_38 = 3;
  }
  iStack_18 = iStack_48;
  if (iStack_48 == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f0),1);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f8),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x300),0);
  }
  else if (iStack_48 == 1) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f0),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f8),1);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x300),0);
  }
  else if (iStack_48 == 2) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f0),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2f8),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x300),1);
  }
  else {
    FUN_141472790(&UNK_142b14770);
  }
  FUN_140ecc770(param_1,iStack_48);
  if (iStack_38 != 3) {
    uStack_24 = FUN_14089b4d0(param_2 + 0x1e8);
    FUN_140eccaa0(param_1,&uStack_24);
  }
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b147b4,*(undefined8 *)(param_2 + 0x1d0));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x248),auStack_30);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b147bc,*(undefined8 *)(param_2 + 0x1e0));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x200),auStack_30);
  if (*(int *)(param_2 + 0x1c8) == 1) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x318),1);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 800),0);
  }
  else if (*(int *)(param_2 + 0x1c8) == 2) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x318),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 800),1);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x318),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 800),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x308),0);
  }
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b147c4,*(undefined8 *)(param_2 + 0x260));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x348),auStack_30);
  cVar1 = FUN_140130ee0(param_1);
  if (cVar1 != '\0') {
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b147cc,*(undefined8 *)(param_2 + 0x2c8));
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x3c0),auStack_30);
  }
  cVar1 = FUN_140130ee0(param_1);
  if (cVar1 == '\0') {
    uStack_28 = *(double *)(param_2 + 0x2c0) != _UNK_1421ad1d0;
    bStack_13 = !(bool)uStack_28;
    uStack_14 = uStack_28;
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x370),bStack_13);
  }
  else {
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b147d4,*(undefined8 *)(param_2 + 0x2c0));
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x3f8),auStack_30);
  }
  cVar1 = FUN_140130ea0(param_1);
  if (cVar1 != '\0') {
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b147dc,*(undefined8 *)(param_2 + 0x278));
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x4f8),auStack_30);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b147e4,*(undefined8 *)(param_2 + 0x280));
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x500),auStack_30);
  }
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b147ec,*(undefined8 *)(param_2 + 0x270));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x450),auStack_30);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b147f4,*(undefined8 *)(param_2 + 0x2a0));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x430),auStack_30);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b147fc,*(undefined8 *)(param_2 + 0x2a8));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x438),auStack_30);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b14804,*(undefined8 *)(param_2 + 0x1d0));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x288),auStack_30);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b1480c,*(undefined8 *)(param_2 + 0x270));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2a0),auStack_30);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b14814,*(undefined8 *)(param_2 + 0x2a0));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2c0),auStack_30);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b1481c,*(undefined8 *)(param_2 + 0x2a8));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x2c8),auStack_30);
  _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b14824,*(undefined8 *)(param_2 + 0x268));
  _setText_QLineEdit__QEAAXAEBVQString___Z
            (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x4c8),auStack_30);
  cVar1 = FUN_140130ea0(param_1);
  if (cVar1 != '\0') {
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b1482c,*(undefined8 *)(param_2 + 0x2b0));
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x490),auStack_30);
    _sprintf_QString__QEAAAEAV1_PEBDZZ(auStack_30,&UNK_142b14834,*(undefined8 *)(param_2 + 0x2b8));
    _setText_QLineEdit__QEAAXAEBVQString___Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x480),auStack_30);
  }
  __1QString__QEAA_XZ(auStack_40);
  __1QString__QEAA_XZ(auStack_30);
  return;
}


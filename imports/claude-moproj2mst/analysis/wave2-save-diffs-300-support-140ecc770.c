
void FUN_140ecc770(longlong param_1,int param_2)

{
  longlong lVar1;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 auStack_10 [4];
  
  lVar1 = FUN_140130f90(param_1);
  if (param_2 == 1) {
    _setCurrentIndex_QStackedWidget__QEAAXH_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1d0),1);
    _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x410));
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x228),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1e8),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x268),0);
    if (lVar1 != 0) {
      uStack_20 = 1;
      func_0x0001408da6f0(lVar1,&uStack_20);
    }
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x308),1);
    lVar1 = _sender_QObject__IEBAPEAV1_XZ(param_1);
    if (lVar1 != 0) {
      uStack_1c = FUN_14089b4d0(*(undefined8 *)(param_1 + 0xe8));
      FUN_140eccaa0(param_1,&uStack_1c);
    }
  }
  else if (param_2 == 0) {
    _setCurrentIndex_QStackedWidget__QEAAXH_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1d0),0);
    _show_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x410));
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x228),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1e8),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x268),0);
    if (lVar1 != 0) {
      uStack_18 = 0;
      func_0x0001408da6f0(lVar1,&uStack_18);
    }
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x308),1);
    lVar1 = _sender_QObject__IEBAPEAV1_XZ(param_1);
    if (lVar1 != 0) {
      uStack_14 = FUN_14089b4d0(*(undefined8 *)(param_1 + 0xe8));
      FUN_140eccaa0(param_1,&uStack_14);
    }
  }
  else {
    _setCurrentIndex_QStackedWidget__QEAAXH_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1d0),2);
    _hide_QWidget__QEAAXXZ(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x410));
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x228),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x1e8),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x268),1);
    if (lVar1 != 0) {
      auStack_10[0] = 1;
      func_0x0001408da6f0(lVar1,auStack_10);
    }
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x318),0);
    _setChecked_QAbstractButton__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 800),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0x308),0);
  }
  return;
}


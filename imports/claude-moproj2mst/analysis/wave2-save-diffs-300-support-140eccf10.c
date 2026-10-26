
void FUN_140eccf10(longlong param_1,int param_2)

{
  longlong lVar1;
  
  if (param_2 == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe70),1);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe80),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe78),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe88),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe60),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe68),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe98),0);
  }
  else {
    if (param_2 == 1) {
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe70),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe80),1);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe78),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe88),0);
    }
    else if (param_2 == 2) {
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe70),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe80),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe78),1);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe88),0);
    }
    else if (param_2 == 3) {
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe70),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe80),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe78),0);
      _setChecked_QAbstractButton__QEAAX_N_Z
                (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe88),1);
    }
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe60),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe68),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe98),1);
  }
  lVar1 = FUN_140130f90(param_1);
  if (lVar1 != 0) {
    *(undefined4 *)(param_1 + 0x15c) = 3;
    if (param_2 == 0) {
      *(undefined4 *)(lVar1 + 0x5b4) = 0;
      *(undefined4 *)(lVar1 + 0x5c0) = 0;
      *(undefined4 *)(*(longlong *)(param_1 + 200 + (longlong)*(int *)(param_1 + 0x15c) * 8) + 0x10)
           = 0;
    }
    else if (param_2 == 1) {
      *(undefined4 *)(lVar1 + 0x5c0) = 3;
      *(undefined4 *)(*(longlong *)(param_1 + 200 + (longlong)*(int *)(param_1 + 0x15c) * 8) + 0x10)
           = 3;
    }
    else if (param_2 == 2) {
      *(undefined4 *)(lVar1 + 0x5c0) = 0xc;
      *(undefined4 *)(*(longlong *)(param_1 + 200 + (longlong)*(int *)(param_1 + 0x15c) * 8) + 0x10)
           = 0xc;
    }
    else if (param_2 == 3) {
      *(undefined4 *)(lVar1 + 0x5c0) = 1;
      *(undefined4 *)(*(longlong *)(param_1 + 200 + (longlong)*(int *)(param_1 + 0x15c) * 8) + 0x10)
           = 1;
    }
  }
  return;
}


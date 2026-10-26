
void FUN_140eccdd0(longlong param_1,int param_2)

{
  if (param_2 == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe08),1);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe28),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe18),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe20),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xde8),0);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe08),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe28),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe18),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xe20),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xde8),1);
  }
  return;
}


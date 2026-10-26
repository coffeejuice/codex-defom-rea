
void FUN_140eccb50(longlong param_1,int param_2)

{
  if (param_2 == 0) {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcf8),1);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd08),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd00),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd18),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xce0),0);
  }
  else {
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xcf8),0);
    _setChecked_QAbstractButton__QEAAX_N_Z
              (*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd08),1);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd00),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xd18),0);
    _setEnabled_QWidget__QEAAX_N_Z(*(undefined8 *)(*(longlong *)(param_1 + 0x140) + 0xce0),1);
  }
  return;
}


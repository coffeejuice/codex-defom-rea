
void FUN_1414036e0(longlong *param_1)

{
  longlong lVar1;
  
  lVar1 = (**(code **)(*param_1 + 0x68))(param_1);
  param_1[2] = lVar1;
  if (param_1[2] != 0) {
    func_0x000141411700(param_1[2],param_1);
  }
  lVar1 = (**(code **)(*param_1 + 0x70))(param_1);
  param_1[3] = lVar1;
  if (param_1[3] != 0) {
    func_0x000141412100(param_1[3],param_1);
  }
  (**(code **)(*param_1 + 0x60))(param_1);
  FUN_1414091b0(param_1);
  return;
}


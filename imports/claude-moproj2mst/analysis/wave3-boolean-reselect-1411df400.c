
void FUN_1411df400(undefined8 param_1,undefined8 param_2)

{
  int iVar1;
  int iVar2;
  longlong lVar3;
  undefined8 uVar4;
  longlong lVar5;
  int iStack_1c;
  undefined1 uStack_10;
  undefined1 uStack_f;
  undefined1 uStack_e;
  undefined1 uStack_d;
  undefined1 uStack_c;
  undefined1 uStack_b;
  char cStack_a;
  undefined1 auStack_9 [9];
  
  FUN_1405eea60(param_1,param_2);
  FUN_1405eeab0(param_1);
  lVar3 = FUN_1405ef3a0(param_1);
  if (lVar3 != 0) {
    uVar4 = func_0x000140051230(param_2);
    lVar5 = FUN_14146f8fc(uVar4,0,0x143579b60,0x143a76500,0);
    if ((lVar5 != 0) && (lVar5 = func_0x000140025010(lVar5), lVar5 != 0)) {
      iVar1 = func_0x00014142ff20(lVar5);
      func_0x000140cbc690(lVar5);
      for (iStack_1c = 0; iVar2 = FUN_1400c3a50(lVar3), iStack_1c < iVar2; iStack_1c = iStack_1c + 1
          ) {
        lVar5 = FUN_1400c3c60(lVar3,iStack_1c);
        if (lVar5 != 0) {
          uStack_b = 0;
          uStack_f = 0;
          uStack_c = 0;
          uStack_d = 0;
          uStack_10 = 0;
          auStack_9[0] = 0;
          uStack_e = 0;
          uVar4 = FUN_1400c0530(lVar5);
          cStack_a = FUN_14004d600(uVar4);
          uVar4 = FUN_1400c0530(lVar5);
          iVar2 = func_0x00014004d560(uVar4);
          if (iVar2 == iVar1) {
            uStack_c = 1;
            uStack_e = 1;
          }
          else if (cStack_a == '\0') {
            uStack_b = 1;
          }
          func_0x0001400c0760(lVar5,&uStack_b);
          func_0x0001400c07f0(lVar5,&uStack_f);
          func_0x0001400c3080(lVar5,&uStack_c);
          func_0x0001400c3030(lVar5,&uStack_d);
          func_0x0001400c30d0(lVar5,&uStack_10);
          func_0x0001400c3170(lVar5,auStack_9);
          func_0x0001400c31c0(lVar5,&uStack_e);
        }
      }
    }
  }
  return;
}


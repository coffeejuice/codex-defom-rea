// DB array index tables (packet p1-03): the indices DBREAD/GENDB use into the MAR and A address
// tables of DB_STEP_DATA, one set per dimension. Generated from the decompile of the four fill
// functions (stores kept in the binary's order).
#include <defmodel/step/db_step.hpp>

#include <algorithm>

namespace def::model {

// @src 1401c6e60  2D MAR indices (stores in the binary's order)
void init_mar_index_2d()
{
    g_mar_index[0] = 0; // 1406e81fc
    g_mar_index[1] = 1; // 1406e8200
    g_mar_index[2] = 2; // 1406e8204
    g_mar_index[3] = 3; // 1406e8208
    g_mar_index[4] = 6; // 1406e820c
    g_mar_index[5] = 7; // 1406e8210
    g_mar_index[6] = 8; // 1406e8214
    g_mar_index[7] = 9; // 1406e8218
    g_mar_index[8] = 10; // 1406e821c
    g_mar_index[9] = 11; // 1406e8220
    g_mar_index[10] = 12; // 1406e8224
    g_mar_index[11] = 13; // 1406e8228
    g_mar_index[12] = 4; // 1406e822c
    g_mar_index[13] = 5; // 1406e8230
    g_mar_index[14] = 14; // 1406e8234
    g_mar_index[15] = 15; // 1406e8238
    g_mar_index[16] = 16; // 1406e823c
    g_mar_index[17] = 17; // 1406e8240
    g_mar_index[18] = 18; // 1406e8244
    g_mar_index[19] = 19; // 1406e8248
    g_mar_index[20] = 69; // 1406e824c
    g_mar_index[21] = 20; // 1406e8250
    g_mar_index[22] = 21; // 1406e8254
    g_mar_index[23] = 61; // 1406e8258
    g_mar_index[24] = 70; // 1406e825c
    g_mar_index[25] = 71; // 1406e8260
    g_mar_index[26] = 72; // 1406e8264
    g_mar_index[27] = 73; // 1406e8268
    g_mar_index[28] = 76; // 1406e826c
    g_mar_index[29] = 77; // 1406e8270
    g_mar_index[30] = 78; // 1406e8274
    g_mar_index[31] = 79; // 1406e8278
    g_mar_index[32] = 80; // 1406e827c
    g_mar_index[33] = 81; // 1406e8280
    g_mar_index[34] = 82; // 1406e8284
    g_mar_index[35] = 83; // 1406e8288
    g_mar_index[36] = 74; // 1406e828c
    g_mar_index[37] = 75; // 1406e8290
    g_mar_index[38] = 84; // 1406e8294
    g_mar_index[39] = 85; // 1406e8298
    g_mar_index[40] = 86; // 1406e829c
    g_mar_index[41] = 87; // 1406e82a0
    g_mar_index[42] = 88; // 1406e82a4
    g_mar_index[43] = 89; // 1406e82a8
    g_mar_index[44] = 132; // 1406e82ac
    g_mar_index[45] = 133; // 1406e82b0
    g_mar_index[46] = 134; // 1406e82b4
    g_mar_index[47] = 135; // 1406e82b8
    g_mar_index[48] = 136; // 1406e82bc
    g_mar_index[49] = 137; // 1406e82c0
    g_mar_index[50] = 90; // 1406e82c4
    g_mar_index[51] = 91; // 1406e82c8
    g_mar_index[52] = 131; // 1406e82cc
}

// @src 1401c7080  3D MAR indices (stores in the binary's order)
void init_mar_index_3d()
{
    g_mar_index[0] = 0; // 1406e81fc
    g_mar_index[1] = 1; // 1406e8200
    g_mar_index[2] = 2; // 1406e8204
    g_mar_index[3] = 3; // 1406e8208
    g_mar_index[4] = 4; // 1406e820c
    g_mar_index[5] = 5; // 1406e8210
    g_mar_index[6] = 6; // 1406e8214
    g_mar_index[7] = 7; // 1406e8218
    g_mar_index[8] = 8; // 1406e821c
    g_mar_index[9] = 9; // 1406e8220
    g_mar_index[10] = 10; // 1406e8224
    g_mar_index[11] = 11; // 1406e8228
    g_mar_index[12] = 14; // 1406e822c
    g_mar_index[13] = 15; // 1406e8230
    g_mar_index[14] = 17; // 1406e8234
    g_mar_index[15] = 18; // 1406e8238
    g_mar_index[16] = 19; // 1406e823c
    g_mar_index[17] = 20; // 1406e8240
    g_mar_index[18] = 21; // 1406e8244
    g_mar_index[19] = 22; // 1406e8248
    g_mar_index[20] = 69; // 1406e824c
    g_mar_index[21] = 24; // 1406e8250
    g_mar_index[22] = 25; // 1406e8254
    g_mar_index[23] = 57; // 1406e8258
    g_mar_index[24] = 70; // 1406e825c
    g_mar_index[25] = 71; // 1406e8260
    g_mar_index[26] = 72; // 1406e8264
    g_mar_index[27] = 73; // 1406e8268
    g_mar_index[28] = 74; // 1406e826c
    g_mar_index[29] = 75; // 1406e8270
    g_mar_index[30] = 76; // 1406e8274
    g_mar_index[31] = 77; // 1406e8278
    g_mar_index[32] = 78; // 1406e827c
    g_mar_index[33] = 79; // 1406e8280
    g_mar_index[34] = 80; // 1406e8284
    g_mar_index[35] = 81; // 1406e8288
    g_mar_index[36] = 84; // 1406e828c
    g_mar_index[37] = 85; // 1406e8290
    g_mar_index[38] = 87; // 1406e8294
    g_mar_index[39] = 88; // 1406e8298
    g_mar_index[40] = 89; // 1406e829c
    g_mar_index[41] = 90; // 1406e82a0
    g_mar_index[42] = 91; // 1406e82a4
    g_mar_index[43] = 92; // 1406e82a8
    g_mar_index[44] = 131; // 1406e82ac
    g_mar_index[45] = 132; // 1406e82b0
    g_mar_index[46] = 133; // 1406e82b4
    g_mar_index[47] = 134; // 1406e82b8
    g_mar_index[48] = 135; // 1406e82bc
    g_mar_index[49] = 136; // 1406e82c0
    g_mar_index[50] = 94; // 1406e82c4
    g_mar_index[51] = 95; // 1406e82c8
    g_mar_index[52] = 127; // 1406e82cc
}

// @src 1401c72a0  2D A indices (stores in the binary's order)
void init_a_index_2d()
{
    g_a_index[0] = 0; // 1406e82d0
    g_a_index[1] = 1; // 1406e82d4
    g_a_index[2] = 2; // 1406e82d8
    g_a_index[3] = 3; // 1406e82dc
    g_a_index[4] = 4; // 1406e82e0
    g_a_index[5] = 5; // 1406e82e4
    g_a_index[6] = 6; // 1406e82e8
    g_a_index[7] = 7; // 1406e82ec
    g_a_index[8] = 8; // 1406e82f0
    g_a_index[9] = 9; // 1406e82f4
    g_a_index[10] = 10; // 1406e82f8
    g_a_index[11] = 11; // 1406e82fc
    g_a_index[12] = 12; // 1406e8300
    g_a_index[13] = 13; // 1406e8304
    g_a_index[14] = 15; // 1406e8308
    g_a_index[15] = 16; // 1406e830c
    g_a_index[16] = 17; // 1406e8310
    g_a_index[17] = 18; // 1406e8314
    g_a_index[18] = 20; // 1406e8318
    g_a_index[19] = 21; // 1406e831c
    g_a_index[20] = 22; // 1406e8320
    g_a_index[21] = 23; // 1406e8324
    g_a_index[22] = 24; // 1406e8328
    g_a_index[23] = 25; // 1406e832c
    g_a_index[24] = 26; // 1406e8330
    g_a_index[25] = 27; // 1406e8334
    g_a_index[26] = 28; // 1406e8338
    g_a_index[27] = 29; // 1406e833c
    g_a_index[28] = 30; // 1406e8340
    g_a_index[29] = 31; // 1406e8344
    g_a_index[30] = 32; // 1406e8348
    g_a_index[31] = 33; // 1406e834c
    g_a_index[32] = 34; // 1406e8350
    g_a_index[33] = 35; // 1406e8354
    g_a_index[34] = 37; // 1406e8358
    g_a_index[35] = 38; // 1406e835c
    g_a_index[36] = 39; // 1406e8360
    g_a_index[37] = 40; // 1406e8364
    g_a_index[38] = 41; // 1406e8368
    g_a_index[39] = 42; // 1406e836c
    g_a_index[40] = 43; // 1406e8370
    g_a_index[41] = 44; // 1406e8374
    g_a_index[42] = 45; // 1406e8378
    g_a_index[43] = 47; // 1406e837c
    g_a_index[44] = 48; // 1406e8380
    g_a_index[45] = 50; // 1406e8384
    g_a_index[46] = 51; // 1406e8388
    g_a_index[47] = 53; // 1406e838c
    g_a_index[48] = 54; // 1406e8390
    g_a_index[49] = 55; // 1406e8394
    g_a_index[50] = 52; // 1406e8398
    g_a_index[51] = 58; // 1406e839c
    g_a_index[52] = 46; // 1406e83a0
    g_a_index[53] = 59; // 1406e83a4
    g_a_index[54] = 60; // 1406e83a8
    g_a_index[55] = 61; // 1406e83ac
    g_a_index[56] = 62; // 1406e83b0
    g_a_index[57] = 63; // 1406e83b4
    g_a_index[58] = 139; // 1406e83b8
    g_a_index[59] = 140; // 1406e83bc
    g_a_index[60] = 141; // 1406e83c0
    g_a_index[61] = 142; // 1406e83c4
    g_a_index[62] = 143; // 1406e83c8
    g_a_index[63] = 144; // 1406e83cc
    g_a_index[64] = 145; // 1406e83d0
    g_a_index[65] = 146; // 1406e83d4
    g_a_index[66] = 147; // 1406e83d8
    g_a_index[67] = 148; // 1406e83dc
    g_a_index[68] = 149; // 1406e83e0
    g_a_index[69] = 150; // 1406e83e4
    g_a_index[70] = 151; // 1406e83e8
    g_a_index[71] = 152; // 1406e83ec
    g_a_index[72] = 153; // 1406e83f0
    g_a_index[73] = 155; // 1406e83f4
    g_a_index[74] = 156; // 1406e83f8
    g_a_index[75] = 157; // 1406e83fc
    g_a_index[76] = 158; // 1406e8400
    g_a_index[77] = 160; // 1406e8404
    g_a_index[78] = 161; // 1406e8408
    g_a_index[79] = 162; // 1406e840c
    g_a_index[80] = 163; // 1406e8410
    g_a_index[81] = 164; // 1406e8414
    g_a_index[82] = 165; // 1406e8418
    g_a_index[83] = 166; // 1406e841c
    g_a_index[84] = 167; // 1406e8420
    g_a_index[85] = 168; // 1406e8424
    g_a_index[86] = 169; // 1406e8428
    g_a_index[87] = 170; // 1406e842c
    g_a_index[88] = 171; // 1406e8430
    g_a_index[89] = 172; // 1406e8434
    g_a_index[90] = 173; // 1406e8438
    g_a_index[91] = 174; // 1406e843c
    g_a_index[92] = 175; // 1406e8440
    g_a_index[93] = 177; // 1406e8444
    g_a_index[94] = 178; // 1406e8448
    g_a_index[95] = 179; // 1406e844c
    g_a_index[96] = 180; // 1406e8450
    g_a_index[97] = 181; // 1406e8454
    g_a_index[98] = 182; // 1406e8458
    g_a_index[99] = 183; // 1406e845c
    g_a_index[100] = 184; // 1406e8460
    g_a_index[101] = 185; // 1406e8464
    g_a_index[102] = 187; // 1406e8468
    g_a_index[103] = 188; // 1406e846c
    g_a_index[104] = 190; // 1406e8470
    g_a_index[105] = 191; // 1406e8474
    g_a_index[106] = 193; // 1406e8478
    g_a_index[107] = 194; // 1406e847c
    g_a_index[108] = 195; // 1406e8480
    g_a_index[109] = 192; // 1406e8484
    g_a_index[110] = 198; // 1406e8488
    g_a_index[111] = 269; // 1406e848c
    g_a_index[112] = 270; // 1406e8490
    g_a_index[113] = 271; // 1406e8494
    g_a_index[114] = 272; // 1406e8498
    g_a_index[115] = 189; // 1406e849c
    g_a_index[116] = 273; // 1406e84a0
    g_a_index[117] = 274; // 1406e84a4
    g_a_index[118] = 275; // 1406e84a8
    g_a_index[119] = 276; // 1406e84ac
    g_a_index[120] = 277; // 1406e84b0
    g_a_index[121] = 278; // 1406e84b4
    g_a_index[122] = 186; // 1406e84b8
    g_a_index[123] = 199; // 1406e84bc
    g_a_index[124] = 200; // 1406e84c0
    g_a_index[125] = 201; // 1406e84c4
    g_a_index[126] = 202; // 1406e84c8
    g_a_index[127] = 203; // 1406e84cc
    g_a_index[128] = 280; // 1406e84d0
}

// @src 1401c77c0  3D A indices (stores in the binary's order)
void init_a_index_3d()
{
    g_a_index[0] = 0; // 1406e82d0
    g_a_index[1] = 1; // 1406e82d4
    g_a_index[2] = 2; // 1406e82d8
    g_a_index[3] = 3; // 1406e82dc
    g_a_index[4] = 4; // 1406e82e0
    g_a_index[5] = 5; // 1406e82e4
    g_a_index[7] = 7; // 1406e82ec
    g_a_index[6] = 6; // 1406e82e8
    g_a_index[8] = 8; // 1406e82f0
    g_a_index[9] = 9; // 1406e82f4
    g_a_index[10] = 10; // 1406e82f8
    g_a_index[11] = 11; // 1406e82fc
    g_a_index[12] = 12; // 1406e8300
    g_a_index[13] = 13; // 1406e8304
    g_a_index[14] = 15; // 1406e8308
    g_a_index[15] = 16; // 1406e830c
    g_a_index[16] = 17; // 1406e8310
    g_a_index[17] = 18; // 1406e8314
    g_a_index[18] = 20; // 1406e8318
    g_a_index[19] = 21; // 1406e831c
    g_a_index[20] = 22; // 1406e8320
    g_a_index[21] = 23; // 1406e8324
    g_a_index[22] = 24; // 1406e8328
    g_a_index[23] = 25; // 1406e832c
    g_a_index[24] = 26; // 1406e8330
    g_a_index[25] = 27; // 1406e8334
    g_a_index[26] = 28; // 1406e8338
    g_a_index[27] = 29; // 1406e833c
    g_a_index[28] = 30; // 1406e8340
    g_a_index[29] = 31; // 1406e8344
    g_a_index[30] = 32; // 1406e8348
    g_a_index[31] = 33; // 1406e834c
    g_a_index[32] = 34; // 1406e8350
    g_a_index[33] = 35; // 1406e8354
    g_a_index[34] = 37; // 1406e8358
    g_a_index[35] = 38; // 1406e835c
    g_a_index[36] = 40; // 1406e8360
    g_a_index[37] = 41; // 1406e8364
    g_a_index[38] = 42; // 1406e8368
    g_a_index[39] = 43; // 1406e836c
    g_a_index[40] = 44; // 1406e8370
    g_a_index[41] = 45; // 1406e8374
    g_a_index[42] = 46; // 1406e8378
    g_a_index[43] = 60; // 1406e837c
    g_a_index[44] = 48; // 1406e8380
    g_a_index[45] = 49; // 1406e8384
    g_a_index[46] = 50; // 1406e8388
    g_a_index[47] = 51; // 1406e838c
    g_a_index[48] = 52; // 1406e8390
    g_a_index[49] = 53; // 1406e8394
    g_a_index[50] = 54; // 1406e8398
    g_a_index[51] = 55; // 1406e839c
    g_a_index[52] = 47; // 1406e83a0
    g_a_index[53] = 56; // 1406e83a4
    g_a_index[54] = 57; // 1406e83a8
    g_a_index[55] = 58; // 1406e83ac
    g_a_index[56] = 59; // 1406e83b0
    g_a_index[57] = 61; // 1406e83b4
    g_a_index[58] = 139; // 1406e83b8
    g_a_index[59] = 140; // 1406e83bc
    g_a_index[60] = 141; // 1406e83c0
    g_a_index[61] = 142; // 1406e83c4
    g_a_index[62] = 143; // 1406e83c8
    g_a_index[63] = 144; // 1406e83cc
    g_a_index[64] = 145; // 1406e83d0
    g_a_index[65] = 146; // 1406e83d4
    g_a_index[66] = 147; // 1406e83d8
    g_a_index[67] = 148; // 1406e83dc
    g_a_index[68] = 149; // 1406e83e0
    g_a_index[69] = 150; // 1406e83e4
    g_a_index[70] = 151; // 1406e83e8
    g_a_index[71] = 152; // 1406e83ec
    g_a_index[72] = 153; // 1406e83f0
    g_a_index[73] = 155; // 1406e83f4
    g_a_index[74] = 156; // 1406e83f8
    g_a_index[75] = 157; // 1406e83fc
    g_a_index[76] = 158; // 1406e8400
    g_a_index[77] = 160; // 1406e8404
    g_a_index[78] = 161; // 1406e8408
    g_a_index[79] = 162; // 1406e840c
    g_a_index[80] = 163; // 1406e8410
    g_a_index[81] = 164; // 1406e8414
    g_a_index[82] = 165; // 1406e8418
    g_a_index[83] = 166; // 1406e841c
    g_a_index[84] = 167; // 1406e8420
    g_a_index[85] = 168; // 1406e8424
    g_a_index[86] = 169; // 1406e8428
    g_a_index[87] = 170; // 1406e842c
    g_a_index[88] = 171; // 1406e8430
    g_a_index[89] = 172; // 1406e8434
    g_a_index[90] = 173; // 1406e8438
    g_a_index[91] = 174; // 1406e843c
    g_a_index[92] = 175; // 1406e8440
    g_a_index[93] = 177; // 1406e8444
    g_a_index[94] = 178; // 1406e8448
    g_a_index[95] = 180; // 1406e844c
    g_a_index[96] = 181; // 1406e8450
    g_a_index[97] = 182; // 1406e8454
    g_a_index[98] = 183; // 1406e8458
    g_a_index[99] = 184; // 1406e845c
    g_a_index[100] = 185; // 1406e8460
    g_a_index[101] = 186; // 1406e8464
    g_a_index[102] = 200; // 1406e8468
    g_a_index[103] = 188; // 1406e846c
    g_a_index[104] = 189; // 1406e8470
    g_a_index[105] = 190; // 1406e8474
    g_a_index[106] = 191; // 1406e8478
    g_a_index[107] = 192; // 1406e847c
    g_a_index[108] = 193; // 1406e8480
    g_a_index[109] = 194; // 1406e8484
    g_a_index[110] = 195; // 1406e8488
    g_a_index[111] = 266; // 1406e848c
    g_a_index[112] = 267; // 1406e8490
    g_a_index[113] = 268; // 1406e8494
    g_a_index[114] = 269; // 1406e8498
    g_a_index[115] = 270; // 1406e849c
    g_a_index[116] = 271; // 1406e84a0
    g_a_index[117] = 274; // 1406e84a4
    g_a_index[118] = 275; // 1406e84a8
    g_a_index[119] = 276; // 1406e84ac
    g_a_index[120] = 277; // 1406e84b0
    g_a_index[121] = 278; // 1406e84b4
    g_a_index[122] = 187; // 1406e84b8
    g_a_index[123] = 196; // 1406e84bc
    g_a_index[124] = 197; // 1406e84c0
    g_a_index[125] = 198; // 1406e84c4
    g_a_index[126] = 199; // 1406e84c8
    g_a_index[127] = 201; // 1406e84cc
    g_a_index[128] = 280; // 1406e84d0
}

// @src 1401c7ce0  fill the 2D tables unless they are already
void select_index_tables_2d()
{
    if (g_index_tables_dim != 2) {
        init_mar_index_2d();
        init_a_index_2d();
        g_index_tables_dim = 2;
    }
}

// @src 1401c7d10  fill the 3D tables unless they are already
void select_index_tables_3d()
{
    if (g_index_tables_dim != 3) {
        init_mar_index_3d();
        init_a_index_3d();
        g_index_tables_dim = 3;
    }
}

// @src 1401c7d40  2 selects 2D, anything else 3D
void select_index_tables(const std::int32_t* dim)
{
    if (*dim == 2) {
        select_index_tables_2d();
    } else {
        select_index_tables_3d();
    }
}

void reset_globals_p1_03()
{
    g_index_tables_dim = 0;
    std::fill(std::begin(g_mar_index), std::end(g_mar_index), 0);
    std::fill(std::begin(g_a_index), std::end(g_a_index), 0);
}

} // namespace def::model

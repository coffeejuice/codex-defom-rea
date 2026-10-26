# CP121: stock QSize conversion closes; the actual style owner remains an input

The non-QSize conversion branch left by CP120 has a concrete stock Qt path. Conversion to QSize delegates from QtGui to QtCore, whose selected branch accepts QSizeF or returns failure; that branch supplies no Boolean Apply callback. The invalid-role branch still depends on the actual widget/application style, with further proxy-style callbacks even for the built-in Windows style. No same-tree timer delivery or nested Boolean Apply before Open return is established. Keep public first-Open/PostPending guards.

Only this report was written. Bounded immutable PE reads and DUMPBIN 14.44.35228.0 were used. No GUI, original execution, source edits, build/tests, target opening, REA import or mutable database access occurred. CP120's one successful connection diagnostic was not repeated.

## Destination21 is a bounded conversion, not an arbitrary custom-type callback

At6553c23e, QStyledItemDelegate::sizeHint loads imported QVariant::handler (IAT65696ae8), dereferences the handler pointer and calls handler+30 at6553c259 with destination type21. Two stock producers can now be distinguished:

- QtCore's exported handler variable672f2048 initially points to table6723df40. That table+30 is6713c6b0.
- QtGui's initializer65693d00 reads the imported variable, saves its old pointer to659c9918 at3d0a and writes table656a6b70 at3d18. That table+30 is650572a0. Its teardown65694850 restores the saved pointer at485e. These are source effects, not observations of initialization or the current pointer during first Open.

For destination21, the QtGui conversion dispatcher uses byte table650577d4[21-2]=10 and target table650577a8[10]=65057779. That block calls imported qcoreVariantHandler at65057779, then its returned table+30 at6505778a with the original conversion arguments. The accessor67138340 consists of LEA6723df40 and RET67138347; it does not return an application-selected handler.

QtCore6713c6b0 uses its own byte dispatch table6713d738[21-1]=18 and target6713d6b4[18]=6713cb21. This selected destination21 branch masks the source QVariant type and accepts only22 atcb29. Other types return false throughcb2e/cb30 ->d69d. Source22 calls6700b190 atcb43, copies the resulting QSize atcb48/cb4b, and returns true throughd682 ->d69d. The bounded6700b190..6700b226 leaf contains floating-point rounding, integer stores and returns atb1f9/b225, with no call instruction. It is the QSizeF-to-QSize path, not a generic user-type conversion callback.

Consequently, when the actual handler is either of these stock tables, the sizeHint non-QSize branch does not reach Boolean Apply through conversion, irrespective of an unsupported source type. This does not admit an unobserved handler pointer. CP120's stored-role value and possible later item/delegate/handler changes remain actual-state prerequisites.

## Style selection has an explicit receiver and global fallback

The invalid-role suffix in6553c1c0 obtains a style as follows. A valid option version>=3/type12 with non-null widget+78 calls QWidget::style6505d660 at6553c2c5. That getter reads widget-private+88, then its extra-data+20; a non-null value returns directly. Otherwise it calls QApplication::style65026100 at6505d67d. The delegate's no-widget/other-option branch also calls QApplication::style at6553c2d1.

QApplication::style first reads global659c9688 at65026111 and returns the existing pointer through6355. Only when it is null does it enter the initialization path, subject to byte659c9668. That path calls QStyleFactory::create652a8d20 at650261b9 and stores the returned style at659c9688 at61be; later fallback keys can retry at6200/6205. The factory has built-in key branches plus a plugin-loader virtual leg at652a8f59/8f6d. Static code does not select the live style key, plugin result or existing global pointer for T's delivered callback.

The delegate finally calls the selected style's +e0 at6553c303, content type0x18. The receiver is that style object. It is neither the SfTreeView T nor the widget area's current Boolean editor E, and equal slot offsets on their different tables do not imply equal methods.

## Concrete Windows-style continuation narrows the next source callback

One recoverable conditional producer is the factory's Windows branch:652a8d76 calls public QWindowsStyle constructor6535abb0. That constructor installs public table656f1f58 at6535abed/abf4. Its +e0=65359280 (sizeFromContents) and +d8=6535a5f0 (pixelMetric). Do not confuse this public table with private implementation table656f21f0 installed by6535aa00.

For content0x18,65359280 selects fallback653595e3 and calls QCommonStyle::sizeFromContents652b6680 at653595fd. CommonStyle's content table652b6e5c[24] selects652b6d8e. Null option, version<4 or type!=12 returns through6e36. Otherwise it constructs rectangles and calls item-layout helper652b3b30 at652b6dec, then combines their extents and returns a QSize. This does not select an application editor.

The helper calls size helper652b3670 with selectors1,0,10 at3b69/3bc0/3c1e. Further concrete callback boundaries use the style-private+90 proxy object: for example text measurement calls its virtual+d8 with metric0x46 at652b37c0, while the conditional check-size branch uses metrics0x26/0x25 at652b36e2/36fb. Layout also uses that proxy's+d8 at3cb4/3ce1/3d13. Actual option features, proxy/table and returned metrics must therefore be bound before claiming the whole built-in layout route has no application effect. The inspected selected bodies contain no direct native Boolean Apply/add call; complete transitive absence through style/proxy/font/widget callbacks is not claimed.

## Later style writes inspected do not populate the same-tree owner

The two stylesheet calls in the already-reached dock refresh method116ed0, at1173e9/117430, use a local widget from1171df ->1171eb ->1171f0 ->[rsp+78]. Their literals14219aa38/14219aa68 are `QPushButton {background:yellow; font:bold}` and `QPushButton {}`. These calls do not establish a write through the known tree fields UI+50/+1c0. Likewise the eleven direct stylesheet setters in dock UI setup11e220 use UI fields20,28,30,38,d0,e0,e8,f0,f8,100,118, not50/1c0. This bounds those direct call sites only; it does not exclude inherited application styles, an indirect property change or a later writer elsewhere.

## Exact remaining occurrence and the next bounded action

Source callback selection is now narrower than CP120: a stock handler cannot introduce Apply on destination21; the invalid-role route needs the actual style/proxy, not another search for item-data+8 inheritance. The next static action is to follow the selected proxy+d8 target after recovering its producer, or identify a real same-T override writer. The minimal passive state observation for this branch would retain T and its option widget, actual handler pointer, widget-extra+20 or returned application style/table, style+e0 target and any proxy+90/table used at37c0. Observing these pointers alone still would not establish a Boolean effect.

The first temporal gap remains CP116's actual live timer record ->67142315 target T ->matching654a8956 ->widget-data predicate8978 ->layout8998, nested within a complete first Open span. No retained record binds that occurrence to the inspected delegate/style call. A positive Boolean-creation frame additionally needs actual widget-area UpdateData ->current E Apply1411e2870 ->E+38's resolved OPI O ->normal add return1411e295a or1411e2bcb with O. Style pointer, pixel size and timer-ID equality cannot substitute for these owner/lifetime and nested-return relations. Zero-slot postLoad creation1411df0e9 remains separately attributed. Use the corrected Open completion boundaries in CP120; retained Forming Editor004 is not this Boolean frame.

## Reproduction bounds

Use the original module hashes and verified DUMPBIN path in CP120/CP116. QtCore4_64.dll SHA256 is bcf0074c7e9fba3bcafdaa38742931f309889cbdbacffbca32af83db237fc160. Code-byte hashes below bind unwind regions or explicitly labeled slices; dispatch data embedded after a method is not executable merely because DUMPBIN disassembles it.

| Module and range, end exclusive | SHA256 |
| --- | --- |
|QtGui650572a0..6505781f|0d74b6fcc6fd1bc2685a3dd5f014a3480fc4addfbccf139fb67cf019a44c1f8b|
|QtCore6713c6b0..6713dba2, includes dispatch data|a60f9f3297dad7024000e095d878b1915bf80bac58c4c4005471ba804a34e3d4|
|QtCore6713cb21..6713cb53 selected branch|18a4de7d6033b66eda1183588940c2abf60b30b4cd350fe0b6fca96af712a118|
|QtCore67138340..67138348 accessor leaf|fbb977df8b7c6f861d0bac8e82ba846ea4244931b67cf97895ecd8411884b767|
|QtGui65693d00..65693d2d initialization leaf|f8fefe371598ca5d33014023a48d071e68d53c976fe434c4a3a9e5d30ebbac86|
|QtGui6505d660..6505d687|5febf883ca4ae8ddd403c286d74fc9b423a1a46b75b22aae97e064fc67169d14|
|QtGui65026100..6502635d|ea7464e2d66013baa3bb4fee73ac39e4567450b25dc238ef0ef33fc1237b7619|
|QtGui652a8d20..652a8f9c|9d052644703fa4503265f01c734dd6a5030951a748acf18e8156e8be6da7d4d4|
|QtGui6535abb0..6535ac00|4ae284155d9e853b45cfbe23fd462e5f1d9b0f53aeb75ab8a317a0271ca70236|
|QtGui65359280..65359718|df37bfca168770c1a3818e9566a8c0d08a9b77c5e9bde16ff8249fe1acf54e30|
|QtGui652b6d8e..652b6e5c selected branch/epilogue/padding|6f0bc81eae9680920f9be3a651005b53ad1e75e4461c94e22b2da7de6150c007|
|QtGui652b3b30..652b4366|169e639a413126a5d1ec28af41f604439f2b709d3ea59424dcfc8f64e024d642|
|QtGui652b3670..652b39dd|7350205bfc778f86048b4552cde930faa06e6fab8c63e8468845ab6b149b1318|
|EXE140116ed0..140117890|916cee3cc8e621c52caa034e367f0c30675f9adf079faf665fe890d06d14bd92|

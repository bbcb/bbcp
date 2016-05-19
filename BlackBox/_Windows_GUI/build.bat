@echo off
@echo DevCompiler.CompileThis WinApi WinOle WinCtl WinCmc WinOleAut WinOleCtl WinDlg WinOleDlg WinMM WinSql WinRpc WinNet WinGL WinGLAux WinGLUtil Kernel Files HostFiles StdLoader Math SMath Log Strings Dates Meta Dialog Services Fonts Ports Printers Stores Converters Sequencers > tmp.txt
@echo DevCompiler.CompileThis Models Views Controllers Properties Printing Mechanisms Containers Documents Windows StdCFrames Controls StdInterpreter StdDialog StdApi StdCmds HostRegistry HostFonts HostPorts OleData HostMechanisms HostWindows HostPrinters HostClipboard HostCFrames HostDialog HostCmds HostMenus HostPictures >> tmp.txt
@echo DevCompiler.CompileThis TextModels TextRulers TextSetters TextViews TextControllers TextMappers StdLog TextCmds StdHeaders FormModels FormViews FormControllers FormGen FormCmds StdFolds StdLinks DevCommanders StdTables StdTabViews HostTabFrames StdViewSizer DevMarkers DevDebug DevHeapSpy DevSearch DevSubTool DevAlienTool >> tmp.txt
@echo DevCompiler.CompileThis HostPackedFiles DevPacker DevDependencies DevReferences DevCmds DevInspector HostTextConv HostBitmaps HostMail StdMenuTool StdClocks StdStamps StdDebug StdCoder StdScrollers Out DevCPM DevCPT DevCPS DevCPB DevCPP DevCPE DevCPH DevCPL486 DevCPC486 DevCPV486 DevSelectors DevCompiler DevBrowser DevLinker DevRBrowser >> tmp.txt
@echo DevCompiler.CompileThis OleStorage OleServer OleClient CtlT CtlC OleViews StdETHConv In XYplane Init Config Integers DevTypeLibs DevComInterfaceGen DevComDebug DevProfiler DevLinkChk DevMsgSpy DevAnalyzer CommStreams CommTCP CommV24 >> tmp.txt
@echo DevCompiler.CompileThis SqlDrivers SqlOdbc SqlOdbc3 SqlDB SqlControls SqlBrowser SqlObxDB SqlObxUI SqlObxNets SqlObxViews SqlObxGen SqlObxExt SqlObxInit SqlObxTab >> tmp.txt
@echo DevCompiler.CompileThis ComTools ComEnumRect ComObject ComConnect ComAggregate ComKoala ComKoalaTst ComKoalaExe ComKoalaDll ComEnum ComPhoneBook ComPhoneBookActiveX ComPhoneBookClient >> tmp.txt
@echo DevCompiler.CompileThis XhtmlWriters XhtmlStdFileWriters XhtmlEntitySets XhtmlTextTableMarkers XhtmlExporter >> tmp.txt
@echo DevCompiler.CompileThis CtlStdType CtlOffice CtlVBIDE CtlOfficeBinder CtlMSForms CtlExcel8 CtlWord8 CtlGraph8 CtlPowerPoint8 CtlOutlook8 CtlDAO35 CtlAccess8 CtlExcel9 CtlWord9 CtlGraph9 CtlPowerPoint9 CtlOutlook9 CtlDAO36 CtlADODB CtlAccess9 CtlExcel5 >> tmp.txt
@echo DevCompiler.CompileThis Console HostConsole ConsLog ConsCompiler ConsInterp >> tmp.txt
@echo DevLinker.Link BlackBox.exe := Kernel$+ Files HostFiles StdLoader 1 Applogo.ico 2 Doclogo.ico 3 SFLogo.ico 4 CFLogo.ico 5 DtyLogo.ico 6 folderimg.ico 7 openimg.ico 8 leafimg.ico 1 Move.cur 2 Copy.cur 3 Link.cur 4 Pick.cur 5 Stop.cur 6 Hand.cur 7 Table.cur >> tmp.txt
@echo Kernel.Quit(0) >> tmp.txt
dev0.exe < tmp.txt
del tmp.txt

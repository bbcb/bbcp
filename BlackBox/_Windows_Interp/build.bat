@echo off
@echo DevCompiler.CompileThis WinApi WinOle Kernel Files Console Math Strings Meta Log Dialog HostFiles HostRegistry HostLang HostConsole Stores Converters Dates Integers Sequencers Services SMath Fonts Ports Printers Models Views Printing Mechanisms Controllers Properties Containers Documents Windows TextModels TextMappers TextRulers TextSetters TextViews TextControllers TextCmds In StdDialog StdApi StdCFrames Controls StdCmds StdScrollers StdViewSizer StdClocks StdCoder StdFolds StdDebug StdStamps StdETHConv StdHeaders StdLinks StdLoader HostDialog StdLog Out ConsLog DevMarkers DevCommanders DevSelectors DevCPM DevCPT DevCPH DevCPB DevCPE DevCPS DevCPP DevCPL486 DevCPC486 DevCPV486 DevCompiler DevLinker HostFonts DevReferences DevDebug DevComDebug ConsCompiler StdInterpreter HostWindows HostDates HostTextConv ConsInterp Init Config ObxHello0 ObxPi ObxRandom ObxTrap > tmp.txt
@echo DevLinker.Link dos BlackBoxInterp.exe := Kernel$+ Files HostFiles StdLoader >> tmp.txt
@echo Kernel.Quit(0) >> tmp.txt
dev0.exe < tmp.txt
del tmp.txt

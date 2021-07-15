@echo off
@echo DevCompiler.CompileSubs Com Comm Ctl Dev Form Ole Sql Std System Text Win Cons >> tmp.txt
@echo DevLinker.Link BlackBox.exe := Unicode Kernel$+ Utf WinKernel Files WinEnv WinFiles WinGui StdLoader WinLoader 1 BlackBox.res 1 Applogo.ico 2 Doclogo.ico 3 SFLogo.ico 4 CFLogo.ico 5 DtyLogo.ico 6 folderimg.ico 7 openimg.ico 8 leafimg.ico 1 Move.cur 2 Copy.cur 3 Link.cur 4 Pick.cur 5 Stop.cur 6 Hand.cur 7 Table.cur >> tmp.txt
@echo DevLinker.Link BBInt.exe := Kernel$+ Utf WinKernel Files WinEnv WinFiles StdLoader WinIntLoader >> tmp.txt
@echo Kernel.Quit(0) >> tmp.txt
dev0win.exe /BB_PACKED_FIRST < tmp.txt
del tmp.txt

@echo off
@echo DevCompiler.CompileThis HostDevPackedFiles > tmp.txt
@echo DevLinker.Link dos dev0new.exe := Kernel$+ Files HostFiles HostDevPackedFiles StdLoader >> tmp.txt
@echo Kernel.Quit(0) >> tmp.txt
dev0.exe < tmp.txt
del tmp.txt

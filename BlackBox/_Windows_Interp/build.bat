@echo off
@echo DevCompiler.CompileSubs Win Sql System Std Host Cons Text Form Dev Xhtml Comm Obx > tmp.txt
@echo DevLinker.Link dos BlackBoxInterp.exe := Kernel$+ Files HostEnv HostFiles StdLoader >> tmp.txt
@echo Kernel.Quit(0) >> tmp.txt
dev0.exe -BB_PACKED_FIRST < tmp.txt
del tmp.txt

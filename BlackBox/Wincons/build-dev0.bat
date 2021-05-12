@echo off
@echo DevLinker.Link dos dev0new.exe := Kernel$+ Files Utf HostEnv HostFiles HostPackedFiles StdLoader >> tmp.txt
@echo Kernel.Quit(0) >> tmp.txt
dev0.exe -BB_PACKED_FIRST < tmp.txt
del tmp.txt

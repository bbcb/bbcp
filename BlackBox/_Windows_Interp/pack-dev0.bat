@echo off
copy dev0new.exe tmp.exe
@echo DevCompiler.CompileThis HostPackedFiles DevPacker > tmp.txt
@echo DevPacker.PackThis dev0new.exe := Code/Init.ocf Host/Code/Console.ocf Code/Console.ocf Host/Code/Lang.ocf Code/Dialog.ocf Host/Code/Fonts.ocf Code/Fonts.ocf Host/Code/Windows.ocf Code/Windows.ocf Code/Ports.ocf Code/Services.ocf Code/Stores.ocf Code/Strings.ocf Code/Math.ocf Code/Sequencers.ocf Code/Models.ocf Code/Views.ocf Code/Log.ocf Code/Converters.ocf Code/Meta.ocf Code/Controllers.ocf Code/Properties.ocf Code/Containers.ocf Code/Mechanisms.ocf Code/Documents.ocf Code/Dates.ocf Code/Printers.ocf Code/Printing.ocf Host/Code/Dates.ocf Host/Code/Dialog.ocf Std/Code/Interpreter.ocf Std/Code/Dialog.ocf Std/Code/Log.ocf Text/Code/Models.ocf Text/Code/Mappers.ocf Text/Code/Rulers.ocf Text/Code/Views.ocf Text/Code/Setters.ocf Text/Code/Controllers.ocf Cons/Code/Log.ocf Code/Config.ocf Cons/Code/Interp.ocf Dev/Code/Commanders.ocf Code/Controls.ocf Std/Code/CFrames.ocf Dev/Code/Compiler.ocf Dev/Code/Markers.ocf Dev/Code/Selectors.ocf Dev/Code/CPM.ocf Dev/Code/CPT.ocf Dev/Code/CPB.ocf Dev/Code/CPP.ocf Dev/Code/CPS.ocf Dev/Code/CPE.ocf Dev/Code/CPV486.ocf Dev/Code/CPH.ocf Dev/Code/CPL486.ocf Dev/Code/CPC486.ocf Std/Code/ETHConv.ocf Host/Code/TextConv.ocf Dev/Code/Linker.ocf Cons/Code/Compiler.ocf Host/Code/Registry.ocf Dev/Code/Debug.ocf Dev/Code/ComDebug.ocf >> tmp.txt
@echo Kernel.Quit(0) >> tmp.txt
tmp.exe < tmp.txt
del tmp.txt
del tmp.exe


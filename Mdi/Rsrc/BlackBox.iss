[Setup]
AppName=BlackBox Component Builder
AppVerName={#AppVerName}
AppPublisher=BlackBox Cross-Platform Team 
AppPublisherURL=https://blackbox.oberon.org
AppVersion={#AppVersion}
VersionInfoVersion={#VersionInfoVersion}
AppCopyright=BSD 2-Clause License, Oberon microsystems, Inc. et al.
Compression=bzip
SolidCompression=yes
PrivilegesRequired=poweruser
DefaultDirName={pf}\BlackBox Component Builder MDI {#AppVersion}
UsePreviousAppDir=no
SetupIconFile=Win\Rsrc\Applogo.ico
UninstallDisplayIcon={uninstallexe}
LicenseFile=LICENSE.txt
DisableProgramGroupPage=yes

[Icons]
Name: "{userdesktop}\BlackBox Component Builder MDI {#AppVersion}"; Filename: "{app}\BlackBox.exe"; WorkingDir: "{app}"

[Registry]
Root: HKCR; Subkey: ".odc"; ValueType: string; ValueName: ""; ValueData: "odcfile"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: ".odc\ShellNew"; ValueType: string; ValueName: "FileName"; ValueData: "{app}\Empty.odc"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "odcfile"; ValueType: string; ValueName: ""; ValueData: "BlackBox Document"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "odcfile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"",1"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "odcfile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"" /o ""%1"""; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "odcfile\shell\print\command"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"" /p ""%1"""; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: ".osf"; ValueType: string; ValueName: ""; ValueData: "osffile"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "osffile"; ValueType: string; ValueName: ""; ValueData: "BlackBox Symbol File"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "osffile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"",2"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "osffile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"" /o ""%1"""; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "osffile\shell\print\command"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"" /p ""%1"""; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: ".ocf"; ValueType: string; ValueName: ""; ValueData: "ocffile"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "ocffile"; ValueType: string; ValueName: ""; ValueData: "BlackBox Code File"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "ocffile\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"",3"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "ocffile\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"" /o ""%1"""; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "ocffile\shell\print\command"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"" /p ""%1"""; Flags: deletekey uninsdeletekey

Root: HKCR; Subkey: "BlackBox.View"; ValueType: string; ValueName: ""; ValueData: "BlackBox View"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "BlackBox.View\CLSID"; ValueType: string; ValueName: ""; ValueData: "{{00000001-1000-11cf-adf0-444553540000}"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "BlackBox.View\protocol\StdFileEditing\server"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"""; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "BlackBox.View\Insertable"; ValueType: string; ValueName: ""; ValueData: ""; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}"; ValueType: string; ValueName: ""; ValueData: "BlackBox View"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\ProgID"; ValueType: string; ValueName: ""; ValueData: "BlackBox.View"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\LocalServer32"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"""; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\InProcHandler32"; ValueType: string; ValueName: ""; ValueData: "ole32.dll"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: """{app}\BlackBox.exe"",1"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\Insertable"; ValueType: string; ValueName: ""; ValueData: ""; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\AuxUserType\2"; ValueType: string; ValueName: ""; ValueData: "BlackBox View"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\AuxUserType\3"; ValueType: string; ValueName: ""; ValueData: "BlackBox"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\MiscStatus"; ValueType: string; ValueName: ""; ValueData: "529"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\DataFormats\GetSet\0"; ValueType: string; ValueName: ""; ValueData: "3,1,32,1"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\verb\-3"; ValueType: string; ValueName: ""; ValueData: "Hide,0,0"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\verb\-2"; ValueType: string; ValueName: ""; ValueData: "Open,0,0"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\verb\-1"; ValueType: string; ValueName: ""; ValueData: "Show,0,0"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\verb\0"; ValueType: string; ValueName: ""; ValueData: "&Edit,0,2"; Flags: deletekey uninsdeletekey
Root: HKCR; Subkey: "CLSID\{{00000001-1000-11cf-adf0-444553540000}\verb\1"; ValueType: string; ValueName: ""; ValueData: "&Open,0,2"; Flags: deletekey uninsdeletekey

[Files]
Source: "*"; Excludes: "*~,odc*,Output"; DestDir: "{app}\"; Flags: replacesameversion recursesubdirs

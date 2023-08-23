
#define MyAppName "MCBEMouseLocker"
#define MyAppVersion "v0.0.2"
#define MyAppPublisher "orbitgw"
#define MyAppURL "https://slimermc.com/MCBEMouseLocker/"
#define MyAppExeName "MCBEML.exe"

[Setup]
AppId={{1AF59F30-87D0-4ED8-844B-D850A618F48D}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={autopf}\{#MyAppName}
DisableProgramGroupPage=yes
LicenseFile=.\LICENSE
OutputBaseFilename="MCBEMouseLocker-{#MyAppVersion}-setup"
SetupIconFile=.\logo\MCBEML.ico
Compression=lzma
SolidCompression=yes
WizardStyle=modern

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
Source: "production\{#MyAppVersion}\MCBEMouseLocker-{#MyAppVersion}\{#MyAppExeName}"; DestDir: "{app}"; Flags: ignoreversion
Source: "production\{#MyAppVersion}\MCBEMouseLocker-{#MyAppVersion}\locker.exe"; DestDir: "{app}"; Flags: ignoreversion

[Icons]
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; Tasks: desktopicon

[Run]
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent



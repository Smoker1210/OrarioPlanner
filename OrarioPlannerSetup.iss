; =====================================================================
; SCRIPT INNO SETUP PER ORARIOPLANNER V1.0.0
; =====================================================================

#define MyAppName "OrarioPlanner"
#ifndef MyAppVersion
  #define MyAppVersion "1.0.0" ; Versione di default se non passata via riga di comando
#endif
#define MyAppPublisher "Samuele Gobbi"
#define MyAppExeName "OrarioPlanner.exe"

; Cartella eseguibile e dll
#define SourceFolder "build\release"
#define AppIcon "resources\setup\app_icon.ico"
#define SetupIcon "resources\setup\setup_icon.ico"

[Setup]
; Identificatore univoco del programma
AppId={{8F4E2D11-6A93-4B85-92A1-0C7A92E8C51B}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}

; Salva l'app nel profilo dell'utente (AppData\Local\Programs\OrarioPlanner)
DefaultDirName={userpf}\{#MyAppName}
DefaultGroupName={#MyAppName}
DisableProgramGroupPage=yes

; Configurazione per Windows a 64-bit
ArchitecturesInstallIn64BitMode=x64compatible
ArchitecturesAllowed=x64compatible

; Permessi di installazione senza UAC
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=dialog

; Output del file d'installazione generato e ICONA DEL SETUP
OutputDir=Output
OutputBaseFilename=OrarioPlanner_Setup_v{#MyAppVersion}
SetupIconFile={#SetupIcon}

; Icona nell'elenco "App e funzionalità" (Disinstallazione)
UninstallDisplayIcon={app}\{#AppIcon}

; Stile dell'interfaccia dell'installer
WizardStyle=modern
Compression=lzma2/ultra64
SolidCompression=yes

ChangesAssociations=yes

[Languages]
Name: "italian"; MessagesFile: "compiler:Languages\Italian.isl"

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"; Flags: unchecked

[Files]
; 1. Copia del file .ico dell'app nella cartella di installazione (serve per i collegamenti)
Source: "{#AppIcon}"; DestDir: "{app}"; Flags: ignoreversion

; 2. Copia del contenuto della ReleaseApp (incluso OrarioPlanner.exe e tutte le DLL)
Source: "{#SourceFolder}\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs; Excludes: "*.obj, *.cpp.obj, *.h.obj, *.moc, *.o"

[Icons]
; Menu Start
Name: "{autoprograms}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"; IconFilename: "{app}\{#AppIcon}"

; Disinstallatore
Name: "{autoprograms}\Disinstalla {#MyAppName}"; Filename: "{uninstallexe}"

; Desktop
Name: "{autodesktop}\{#MyAppName}"; Filename: "{app}\{#MyAppExeName}"; WorkingDir: "{app}"; IconFilename: "{app}\{#AppIcon}"; Tasks: desktopicon

[Run]
; Avvio dell'applicazione al termine dell'installazione
Filename: "{app}\{#MyAppExeName}"; Description: "{cm:LaunchProgram,{#StringChange(MyAppName, '&', '&&')}}"; Flags: nowait postinstall skipifsilent
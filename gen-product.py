##################################################################################
##    MCBEMouseLocker                                                           ##
##    Copyright (C) 2023 orbitgw <orbitgw@foxmail.com>                          ##
##                                                                              ##
##    This program is free software: you can redistribute it and/or modify      ##
##    it under the terms of the GNU General Public License as published by      ##
##    the Free Software Foundation, either version 3 of the License, or         ##
##    (at your option) any later version.                                       ##
##                                                                              ##
##    This program is distributed in the hope that it will be useful,           ##
##    but WITHOUT ANY WARRANTY; without even the implied warranty of            ##
##    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             ##
##    GNU General Public License for more details.                              ##
##                                                                              ##
##    You should have received a copy of the GNU General Public License         ##
##    along with this program.  If not, see <https://www.gnu.org/licenses/>.    ##
##################################################################################

import os
import zipfile
from shutil import copyfile
import json
import hashlib
version = 'v'
with open('VERSION','r') as file:
    version = version + file.read()

dir = './production/'+version + '/MCBEMouseLocker-' + version

iss_script='''
#define MyAppName "MCBEMouseLocker"
#define MyAppVersion "'''+version+'''"
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


'''


try:
    print('MCBEMouseLocker Production Generator Tools \nCopyright (C) orbitgw\n---------------------------------------------------')
    print('Product Version: '+version)
    if(not os.path.exists(dir)):
        print('Making dir: '+ dir + '...')
        os.makedirs(dir)
        print('Done')
    print('Moving MCBEML.exe -> '+dir)
    copyfile('./x64/Release/MCBEML.exe' , dir + '/MCBEML.exe')
    print('Done')
    print('Moving locker.exe -> '+dir)
    copyfile('./x64/Release/locker.exe' , dir + '/locker.exe')
    print('Done')
    print('Moving License -> '+dir)
    copyfile('./LICENSE' , dir + '/LICENSE')
    print('Done')
    print('Writing iss script ...')
    with open('MCBEML.iss','w+') as file:
        file.write(iss_script)
    print('Done')
    os.system('\"C:\Program Files (x86)\Inno Setup 6\ISCC.exe\" mcbeml.iss')
    if(not os.path.exists('Output')):
        print('Making dir: Output ...')
        os.makedirs('Output')
        print('Done')
    print('Make zipfile ...')
    zip_file = './Output/MCBEMouseLocker-' + version +'.zip'
    zip = zipfile.ZipFile(zip_file, "w", zipfile.ZIP_DEFLATED)
    for path, dirnames, filenames in os.walk(dir):
        for filename in filenames:
            zip.write(os.path.join(dir, filename), os.path.join('./MCBEMouseLocker-' + version, filename))
    zip.close()
    print('Done')
    
    print("Generate latest data file...")
    latest = {
        "version":version,
        "downloads":[
            {
                "name":"MCBEMouseLocker-"+version+"-setup.exe",
                "link":"https://github.com/orbitgw/MCBEMouseLocker/releases/download/"+version+"/MCBEMouseLocker-"+version+"-setup.exe"
            },
            {
                "name":"MCBEMouseLocker-"+version+".zip",
                "link":"https://github.com/orbitgw/MCBEMouseLocker/releases/download/"+version+"/MCBEMouseLocker-"+version+".zip"
            }
        ]
    }
    with open('./data/latest.json','w+') as file:
        file.write(json.dumps(latest))
    print("Done")

    print('Gererate Production successfully\n=================================')
    print("# SHA256")
    hashObj = hashlib.sha256()
    print('- MCBEMouseLocker-'+version+'-setup.exe `',end='')
    with open('./Output/MCBEMouseLocker-'+version+'-setup.exe','rb') as file:
        hashObj.update(file.read())
        print(hashObj.hexdigest(),end='')
    print('`')
    print('- MCBEMouseLocker-'+version+'.zip `',end='')
    with open('./Output/MCBEMouseLocker-'+version+'.zip','rb') as file:
        hashObj.update(file.read())
        print(hashObj.hexdigest(),end='')
    print('`')


except Exception as e:
    print()
    print(e)

os.system('pause>nul')
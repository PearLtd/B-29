//
//  WInstallService.c
//  WLServiceDaemon
//
//  Created by 张书睿 on 15/9/29.
//  Copyright © 2015年 张书睿. All rights reserved.
//

#include "WInstallService.h"
#ifdef _WIN32
int Install_Service()
{
    char Server_Path[100]="c:\\pear.exe";
    char Bat_Path[100]={0};
    char App_Path[100]={0};
	GetModuleFileName(NULL, App_Path, 100);
	strcpy(App_Path,App_Path);

	int i;
	for(i=0 ;i<100;i++){
        if(Server_Path[i]!=App_Path[i] && Server_Path[i]-App_Path[i]!=32){
            break;
        }
    }
    if(i==100){
        return 1;
    }

	strcpy(Bat_Path,App_Path);
	(strrchr(Bat_Path, '\\'))[0] = 0;
    strcat(Bat_Path , "\\temp.bat");

    FILE* file;
    file = fopen(Bat_Path, "w");
    if (file == NULL) return -1;
    fprintf(file,"echo off\n");
    fprintf(file,"cls\n");
    fprintf(file,":-------------------------------------\n");
    fprintf(file,"REM  --> Check for permissions\n");
    fprintf(file,">nul 2>&1 \"%%SYSTEMROOT%%\\system32\\cacls.exe\" \"%%SYSTEMROOT%%\\system32\\config\\system\"\n");
    fprintf(file,"REM --> If error flag set, we do not have admin.\n");
    fprintf(file,"if '%%errorlevel%%' NEQ '0' (\n");
    fprintf(file,"    echo Requesting administrative privileges...\n");
    fprintf(file,"    goto FIRST\n");
    fprintf(file,") else ( goto gotAdmin )\n");
    fprintf(file,":FIRST\n");
    fprintf(file,"if '%%1%%' EQU 'first' (\n");
    fprintf(file,"    goto UACPrompt\n");
    fprintf(file,") else (\n");
    fprintf(file,"     goto gotAdmin\n");
    fprintf(file,")\n");
    fprintf(file,":UACPrompt\n");
    fprintf(file,"    echo Set UAC = CreateObject^(\"Shell.Application\"^) > \"%%temp%%\\getadmin.vbs\"\n");
    fprintf(file,"    set params = %%*:\"=\"\"\n");
    fprintf(file,"echo UAC.ShellExecute \"cmd.exe\", \"/c %%~s0 %%params%%\", \"\", \"runas\", 1 >> \"%%temp%%\\getadmin.vbs\"\n");
    fprintf(file,"    \"%%temp%%\\getadmin.vbs\"\n");
    fprintf(file,"    del \"%%temp%%\\getadmin.vbs\"\n");
    fprintf(file,"    exit /B\n");
    fprintf(file,":gotAdmin\n");
    fprintf(file,"    pushd \"%%CD%%\"\n");
    fprintf(file,"    CD /D \"%%~dp0\"\n");
    fprintf(file,":--------------------------------------\n");

    ///install service
    fprintf(file,"sc stop pear\n");
    fprintf(file,"cls\n");
    fprintf(file,"echo --- Logger Service Installe\r ---\n");
    fprintf(file,"echo 5\n");
    fprintf(file,"ping -n 2 127.0.0.1>nul\n");
    fprintf(file,"echo 4\n");
    fprintf(file,"ping -n 2 127.0.0.1>nul\n");
    fprintf(file,"echo 3\n");
    fprintf(file,"ping -n 2 127.0.0.1>nul\n");
    fprintf(file,"echo 2\n");
    fprintf(file,"ping -n 2 127.0.0.1>nul\n");
    fprintf(file,"echo 1\n");
    fprintf(file,"ping -n 2 127.0.0.1>nul\n");
    fprintf(file,"copy /y ");
    fprintf(file,App_Path);
    fprintf(file," ");
    fprintf(file,Server_Path);
    fprintf(file,"\n");
    fprintf(file,"sc delete pear\n");
    fprintf(file,"cls\n");
    fprintf(file,"echo --- Logger Service Installe\r ---\n");
    fprintf(file,"sc create pear binPath= \"");
    fprintf(file,Server_Path);
    fprintf(file,"\" start= auto\n");

    ///start service
    fprintf(file,"sc start pear\n");
    fprintf(file,"pause\n");

    ///delete bat file
    fprintf(file,"del ");
    fprintf(file,Bat_Path);
    fclose(file);
    system(strcat(Bat_Path," first"));
    return 0;
}
#endif

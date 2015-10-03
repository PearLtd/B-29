//
//  WInstallService.c
//  WLServiceDaemon
//
//  Created by 张书睿 on 15/9/29.
//  Copyright © 2015年 张书睿. All rights reserved.
//

#include "WInstallService.h"
#ifdef _WIN32
char CurrentExePath[FILENAME_MAX];

int Install_Service(int argc, const char * argv[])
{
    char ServerExePath[]="c:\\Pear.exe";

    GetCurrentExePath(CurrentExePath, FILENAME_MAX);

    //the file on serverpath, already is server. return 1
    if(GetExeOnServerPath(CurrentExePath,ServerExePath)==1)
        return 1;


    //from arg judge first run, or try to get admin's file.
    //通过判断arg，判断是直接运行的程序，还是已经尝试提权的程序

    //
    if (argc > 1 && strcmp(argv[1], "Pear") == 0){
        //
        system("echo off");
        system("sc stop Pear");
        system("cls");
        system("echo --- Logger Service Installe\r ---");
        system("ping -n 5 127.0.0.1>nul");

        //copy file to ServerExePath
        char temp[FILENAME_MAX];
        sprintf(temp,"copy /y %s %s",CurrentExePath , ServerExePath);
        system(temp);

        system("sc delete Pear");

        system("cls");
        system("echo --- Logger Service Installe\r ---");


        //create service
        sprintf(temp,"sc create Pear binPath= \"%s\" start= auto" , ServerExePath);
        system(temp);

        //start service
        system("sc start Pear");

        system("pause");

        return 0;
    }
    RunSelfAsAdmin();
    return 0;
}

void RunSelfAsAdmin()
{
    GetCurrentExePath(CurrentExePath, FILENAME_MAX);
    if (ShellExecute(0, "runas", CurrentExePath, "Pear", "C:\\Windows\\System32", SW_NORMAL))
        exit(0);
}

int GetExeOnServerPath(char *CurrentExePath, char *ServerExePath){
    int i;
    for(i=0 ;i<(sizeof(ServerExePath)-1);i++){
        if(CurrentExePath[i]!=ServerExePath[i] && CurrentExePath[i]-ServerExePath[i]!=32){
            break;
        }
    }
    if(i==sizeof(ServerExePath)-1){
        return 1;
    }

}

void GetCurrentExePath(char *CurrentExePath, size_t length)
{
    GetModuleFileName(NULL, CurrentExePath, length);
}
#endif

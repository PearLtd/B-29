//
//  WService.c
//  WLServiceDaemon
//
//  Created by 张书睿 on 15/9/24.
//  Copyright © 2015年 张书睿. All rights reserved.
//

#ifdef _WIN32

#include "WService.h"

SERVICE_STATUS ServiceStatus;  // 服务状态
SERVICE_STATUS_HANDLE hStatus; // 服务状态句柄

HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;

void WStart()
{
    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = SERVICE_NAME;
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;
    //启动服务的控制分派机线程
    StartServiceCtrlDispatcher(ServiceTable);
    return;
}

void ServiceMain(int argc, char** argv){
    hStatus= RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);

    ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    ServiceStatus.dwControlsAccepted = 0;
    ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwServiceSpecificExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;

    SetServiceStatus(hStatus, &ServiceStatus);

    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL)
    {
        ServiceStatus.dwControlsAccepted = 0;
        ServiceStatus.dwCurrentState = SERVICE_STOPPED;
        ServiceStatus.dwWin32ExitCode = GetLastError();
        ServiceStatus.dwCheckPoint = 1;
        SetServiceStatus(hStatus, &ServiceStatus);
        return;
    }


    ///准备工作结束
    ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;///允许手动停止服务
    ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwCheckPoint = 0;
    SetServiceStatus(hStatus, &ServiceStatus);





    ///建立线程
    HANDLE hThread = CreateThread(NULL, 0, write, NULL, 0, NULL);
    ///等待线程结束
    WaitForSingleObject(hThread, INFINITE);

    ///关闭服务
    CloseHandle(g_ServiceStopEvent);
    ServiceStatus.dwControlsAccepted = 0;
    ServiceStatus.dwCurrentState = SERVICE_STOPPED;
    ServiceStatus.dwWin32ExitCode = 0;
    ServiceStatus.dwCheckPoint = 3;
    SetServiceStatus(hStatus, &ServiceStatus);
    return ;


}

void ServiceCtrlHandler(int CtrlCode)
{
    switch (CtrlCode)
    {
        case SERVICE_CONTROL_STOP:
        {
            if (ServiceStatus.dwCurrentState != SERVICE_RUNNING)
                break;

            ServiceStatus.dwControlsAccepted = 0;
            ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
            ServiceStatus.dwWin32ExitCode = 0;
            ServiceStatus.dwCheckPoint = 4;
            SetServiceStatus(hStatus, &ServiceStatus);


            SetEvent(g_ServiceStopEvent);

            break;
        }

        default:
        {
            break;
        }
    }

}
int write(){
    WriteToLog("服务开始运行");
    while(WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0){
        WriteToLog("a");
        Sleep(60000);
    }
    WriteToLog("服务停止运行");
    return 0;

}

#endif

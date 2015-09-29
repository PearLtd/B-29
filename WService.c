//
//  WService.c
//  WLServiceDaemon
//
//  Created by 张书睿 on 15/9/24.
//  Copyright © 2015年 张书睿. All rights reserved.
//

#ifdef _WIN32

#include "WService.h"

SERVICE_STATUS Service_Status;  // 服务状态
SERVICE_STATUS_HANDLE h_Status; // 服务状态句柄

HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;

void Win_Start()
{
    SERVICE_TABLE_ENTRY Service_Table[2];
    Service_Table[0].lpServiceName = SERVICE_NAME;
    Service_Table[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
    Service_Table[1].lpServiceName = NULL;
    Service_Table[1].lpServiceProc = NULL;
    //启动服务的控制分派机线程
    StartServiceCtrlDispatcher(Service_Table);
    return;
}

void ServiceMain(int argc, char** argv){
    h_Status= RegisterServiceCtrlHandler(SERVICE_NAME, ServiceCtrlHandler);

    Service_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    Service_Status.dwControlsAccepted = 0;
    Service_Status.dwCurrentState = SERVICE_START_PENDING;
    Service_Status.dwWin32ExitCode = 0;
    Service_Status.dwServiceSpecificExitCode = 0;
    Service_Status.dwCheckPoint = 0;

    SetServiceStatus(h_Status, &Service_Status);

    g_ServiceStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if (g_ServiceStopEvent == NULL)
    {
        Service_Status.dwControlsAccepted = 0;
        Service_Status.dwCurrentState = SERVICE_STOPPED;
        Service_Status.dwWin32ExitCode = GetLastError();
        Service_Status.dwCheckPoint = 1;
        SetServiceStatus(h_Status, &Service_Status);
        return;
    }


    ///准备工作结束
    Service_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP;///允许手动停止服务
    Service_Status.dwCurrentState = SERVICE_RUNNING;
    Service_Status.dwWin32ExitCode = 0;
    Service_Status.dwCheckPoint = 0;
    SetServiceStatus(h_Status, &Service_Status);





    ///建立线程
    HANDLE h_Thread = CreateThread(NULL, 0, write, NULL, 0, NULL);
    ///等待线程结束
    WaitForSingleObject(h_Thread, INFINITE);

    ///关闭服务
    CloseHandle(g_ServiceStopEvent);
    Service_Status.dwControlsAccepted = 0;
    Service_Status.dwCurrentState = SERVICE_STOPPED;
    Service_Status.dwWin32ExitCode = 0;
    Service_Status.dwCheckPoint = 3;
    SetServiceStatus(h_Status, &Service_Status);
    return ;


}

void ServiceCtrlHandler(int CtrlCode)
{
    switch (CtrlCode)
    {
        case SERVICE_CONTROL_STOP:
        {
            if (Service_Status.dwCurrentState != SERVICE_RUNNING)
                break;

            Service_Status.dwControlsAccepted = 0;
            Service_Status.dwCurrentState = SERVICE_STOP_PENDING;
            Service_Status.dwWin32ExitCode = 0;
            Service_Status.dwCheckPoint = 4;
            SetServiceStatus(h_Status, &Service_Status);
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
    Write_To_Log("start service");
    int i=0;
    while(WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0){
            if(i==59){
                Write_To_Log("a");
                i=0;
            }else{
                i++;
            }
        Sleep(1000);
    }
    Write_To_Log("stop service");
    return 0;

}

#endif

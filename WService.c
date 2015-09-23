#include <Windows.h>
#include <stdio.h>
#include "LogWriter.c"

#define SERVICE_NAME "MemoryStatus"

SERVICE_STATUS ServiceStatus;  // ����״̬
SERVICE_STATUS_HANDLE hStatus; // ����״̬���

HANDLE g_ServiceStopEvent = INVALID_HANDLE_VALUE;

void  ServiceMain(int argc, char** argv);
void ServiceCtrlHandler(int CtrlCode);
int write();

void WStart()
{
    SERVICE_TABLE_ENTRY ServiceTable[2];
    ServiceTable[0].lpServiceName = SERVICE_NAME;
    ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTION)ServiceMain;
    ServiceTable[1].lpServiceName = NULL;
    ServiceTable[1].lpServiceProc = NULL;
    // ��������Ŀ��Ʒ��ɻ��߳�
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


	///׼����������
	ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP;///�����ֶ�ֹͣ����
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	ServiceStatus.dwWin32ExitCode = 0;
	ServiceStatus.dwCheckPoint = 0;
	SetServiceStatus(hStatus, &ServiceStatus);





	/// �����߳�
	HANDLE hThread = CreateThread(NULL, 0, write, NULL, 0, NULL);
	///�ȴ��߳̽���
	WaitForSingleObject(hThread, INFINITE);

	/// �رշ���
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
    WriteToLog("����ʼ����");
    while(WaitForSingleObject(g_ServiceStopEvent, 0) != WAIT_OBJECT_0){
        WriteToLog("a");
       Sleep(1000);
    }
    WriteToLog("����ֹͣ����");
    return 0;

}

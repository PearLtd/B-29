//
//  WService.h
//  WLServiceDaemon
//
//  Created by 张书睿 on 15/9/24.
//  Copyright © 2015年 张书睿. All rights reserved.
//
#ifdef _WIN32
#ifndef WService_h
#define WService_h

#include <Windows.h>
#include <stdio.h>
#include "LogWriter.h"

#define SERVICE_NAME "MemoryStatus"

void  ServiceMain(int argc, char** argv);
void ServiceCtrlHandler(int CtrlCode);
int write();
void WStart();


#endif /* WService_h */
#endif /* _WIN32 */

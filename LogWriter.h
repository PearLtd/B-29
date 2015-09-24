//
//  LogWriter.h
//  WLServiceDaemon
//
//  Created by 张书睿 on 15/9/24.
//  Copyright © 2015年 张书睿. All rights reserved.
//

#ifndef LogWriter_h
#define LogWriter_h

#include <stdio.h>

#ifdef _WIN32
#define PATH "c:\\pear.txt"
#else
#define PATH "//home//abo//Work//Projects//LinuxDaemon//Test//log.txt"
#endif

int WriteToLog(char* str);
void WriteOpen();
void WriteClose();
#endif /* LogWriter_h */

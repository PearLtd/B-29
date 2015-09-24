//
//  LogWriter.c
//  WLServiceDaemon
//
//  Created by 张书睿 on 15/9/24.
//  Copyright © 2015年 张书睿. All rights reserved.
//

#include "LogWriter.h"

int IsWrite=1;///是否写log


int WriteToLog(char* str)
{
    if(IsWrite==0)
        return 0;
    FILE* log;
    log = fopen(PATH, "a+");
    if (log == NULL) return -1;
    fprintf(log, "%s\n", str);
    fclose(log);
    return 0;
}

void WriteOpen()
{
    IsWrite=1;
}
void WriteClose()
{
    IsWrite=0;
}

//
//  LDaemon.c
//  WLServiceDaemon
//
//  Created by 张书睿 on 15/9/24.
//  Copyright © 2015年 张书睿. All rights reserved.
//

#include "LDaemon.h"
#ifndef _WIN32

void Linux_Start()
{
    init_daemon();
    towrite();
    return ;
}

void init_daemon()
{
    int pid;
    pid=fork();
    if(pid<0)
        exit(1);  //创建错误，退出
    else if(pid>0) //父进程退出
        exit(0);

    setsid(); //使子进程成为组长
    pid=fork();
    if(pid>0)
        exit(0); //再次退出，使进程不是组长，这样进程就不会打开控制终端
    else if(pid<0)
        exit(1);
    return;
}

int towrite(){
    Write_To_Log("start Daemon");
    while(1){
        Write_To_Log("a");
        sleep(60);
    }
    return 0;

}
#endif // _WIN32

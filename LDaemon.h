//
//  LDaemon.h
//  WLServiceDaemon
//
//  Created by 张书睿 on 15/9/24.
//  Copyright © 2015年 张书睿. All rights reserved.
//

#ifndef LDaemon_h
#define LDaemon_h

#include <stdio.h>
#include<unistd.h>
#include<stdlib.h>

#include "LogWriter.h"

void LStart();
void init_daemon();
int towrite();
#endif /* LDaemon_h */

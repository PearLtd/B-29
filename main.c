//
//  main.c
//  WLServiceDaemon
//
//  Created by 张书睿 on 15/9/24.
//  Copyright © 2015年 张书睿. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include "WService.h"
#else
//#include "LDaemon.h"
#endif


int main(int argc, const char * argv[]) {
#ifdef _WIN32
    WStart();
#else
    LStart();
#endif
    return 0;
}
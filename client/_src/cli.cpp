#include <iostream>
#include <cstring>
#include <thread>
#include "client.h"
#define MAX 100

const string ConfigName = "../config.cfg";

int main(int argc,char *argv[])
{
    Client cli(ConfigName);
    //与服务器连接
    cli.GetConnect();
    //走到这一步说明已经连接上了 准备创建读线程和写线程
    while(1)
        {

            unsigned char Mode = cli.ModeSelect();
            if(Mode == 2)
                {
                    cli.Register();
                }
            else if(Mode == 1)
                {
                    // 如果登录成功则需要退出这个循环
                    if(cli.Login())
                        break;
                }
        }
    return 0;
}        
#ifndef _CLIENT_H_
#define _CLIENT_H__
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdlib>
#include <termios.h>
#include <fcntl.h>
#include "global.h"
#include "log.h"
#include "crc32.h"
#include "cli_config.h"

class Client
    {
        private:
                //套接字句柄

                int sockfd = -1;
                // 用来保存服务器的地址信息
                struct sockaddr_in  SIN;
                //都是单例类 不需要主动释放
                ConfigManager*      Cfg;
                CCRC32*             Crc32;
                DailyLogger*        Logger;
                int                 test;
        public:

            // 与服务器建立连接
            void GetConnect(int __domain = AF_INET, int __type = SOCK_STREAM, int __protocol = 0);
            void CloseConnect();
            void MsgSendThreadFunc(){};
            void MsgRecvThreadFunc(){};
            // 动态选择菜单,1代表login，2代表register，0表示无效输出
            bool Login();
            void Register(){};
            void displayMenu(int highlight);
            unsigned char ModeSelect();
            // 用于菜单选择的模式设置,不回显
            void setRawMode(bool enable);
            void setNonBlocking(int sockfd);
            void sendData(int sockfd, const char* buff, size_t len);
            void recvData(int sockfd, char* buff, size_t len);
        public:
            Client(const string& configName = "../config.cfg");
            ~Client();
    };
#endif
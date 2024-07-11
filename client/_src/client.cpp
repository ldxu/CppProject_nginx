#include "client.h"

Client::Client(const string& configName)
    {
        std::cout << GREEN << "--------------------------------------------------------------" << RESET << std::endl;
        Cfg = ConfigManager::getInstance();
        if(!Cfg->loadConfig(configName))
            {
                cerr << RED << "加载配置文件失败！程序退出！" << RESET << std::endl;
                // 正常退出
                exit(-1); 
            }
        else
            {
                std::cout << GREEN << "加载配置文件成功！" << RESET << std::endl;
            }

        string logname = Cfg->getString("LogName", "DailyLog");
        string logfilename = Cfg->getString("LogFileName", "/logs/log.txt");
        std::string log_directory = std::filesystem::current_path().parent_path().string();

        std::cout << log_directory << std::endl;

        Logger = DailyLogger::getInstance();
        Logger->initlogger(logname, log_directory + logfilename);
        Logger->setLogLevel(spdlog::level::debug); 
        if(!Logger)
            {
                cerr << RED << "Log初始化失败!程序退出!" << RESET << std::endl;
                exit(-1);   
            }
        else
            {
                std::cout << GREEN << "Log初始化成功!" << RESET << std::endl;
            }
        Crc32 = CCRC32::GetInstance();

        if(!Crc32)
            {
                cerr << RED << "CRC32失败!程序退出!" << RESET << std::endl;
                exit(-1);  
            }
        else
            {
                std::cout << GREEN << "CRC32初始化成功!" << RESET << std::endl;
            }
        std::cout << GREEN << "--------------------------------------------------------------" << RESET << std::endl;
    }

Client::~Client()
    {
        if(sockfd!=-1)
            {
                close(sockfd);
            }
        std::cout << GREEN << "资源回收成功！" << RESET << std::endl;
    }

void Client::GetConnect(int __domain, int __type, int __protocol)
    {
        int port            = Cfg->getInt("PORT", 8080);
        string IpAddress    = Cfg->getString("IPAddress", "127.0.0.1");
        SIN.sin_family      = AF_INET;
        SIN.sin_port        = htons(port);
        SIN.sin_addr.s_addr = inet_addr(IpAddress.c_str());
        int len             = sizeof(SIN);  
        std::cout << "checkpoint 1" << std::endl;
        sockfd = socket(__domain, __type, __protocol);
        if(sockfd == -1)
            {
                Logger->log(spdlog::level::err, "套接字创建失败!");
                cerr << RED << "套接字创建失败!程序退出!" << RESET << std::endl;
                exit(-1);
            }
        else
            {
                 Logger->log(spdlog::level::info, "套接字创建成功!");
            }

        if(connect(sockfd, (struct sockaddr*)&SIN, len)==-1)
            {
                Logger->log(spdlog::level::err, "连接失败!");
                cerr << RED << "连接失败!程序退出!" << RESET << std::endl;
                exit(-1);
            }
        else
            {
                Logger->log(spdlog::level::info, "成功从端口:{} 与服务器建立连接！服务器地址为:{}",port, IpAddress);
                std::cout << GREEN << "连接成功！" << RESET << std::endl;
            }
        //设置为非阻塞
        setNonBlocking(sockfd);
    }
void Client::CloseConnect()
    {
        close(sockfd);
        sockfd = -1;
        Logger->log(spdlog::level::info, "程序主动退出！");
    }
void Client::setRawMode(bool enable)
    {
        static struct termios oldt, newt;
        if (enable) {
            tcgetattr(STDIN_FILENO, &oldt); // 读取当前的终端设置
            newt = oldt;
            newt.c_lflag &= ~(ICANON | ECHO); // 关闭回显和规范模式
            tcsetattr(STDIN_FILENO, TCSANOW, &newt); // 设置新的终端设置
        } else {
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // 还原终端设置
        }
    }
void Client::displayMenu(int highlight)
    {
        std::cout << "\x1b[2J";  // 清屏
        std::cout << "\x1b[H";   // 光标移到左上角
        if (highlight == 0) {
            std::cout << "--->Login" << std::endl;
            std::cout << "    Register" << std::endl;
        } else {
            std::cout << "    Login" << std::endl;
            std::cout << "--->Register" << std::endl;
        }
    }
    
unsigned char Client::ModeSelect()
    {
        int highlight = 0;
        char Click;
        setRawMode(true);
        while (true) {
            displayMenu(highlight);

            Click = std::cin.get();

            if (Click == 'w') {
                    highlight = (highlight == 0) ? 1 : 0;
                } 
            else if (Click == 's') 
                {
                    highlight = (highlight == 1) ? 0 : 1;
                }
            else if (Click == '\n') {
                break;
                }
        }
        setRawMode(false);
        if(highlight == 0)
            return 1;
        else if(highlight == 1)
            return 2;
        else
            return 0;

    }

void Client::setNonBlocking(int sockfd)
    {
        int flags = fcntl(sockfd, F_GETFL, 0);
        if (flags == -1) {
            std::cerr << "Failed to get socket flags\n";
            exit(EXIT_FAILURE);
        }
        if (fcntl(sockfd, F_SETFL, flags | O_NONBLOCK) == -1) {
            std::cerr << "Failed to set non-blocking mode\n";
            exit(EXIT_FAILURE);
        }  
    }

void Client::sendData(int sockfd, const char* buff, size_t len)
    {
        size_t totalSent = 0;
        std::cout << "准备发送消息" << std::endl;
        Logger->log(spdlog::level::info, "准备发消息");
        while (totalSent < len) {
            ssize_t sent = send(sockfd, buff + totalSent, len - totalSent, 0);
            if (sent == -1) {
                if (errno == EAGAIN || errno == EWOULDBLOCK) {
                    // 如果发送缓冲区已满，稍后重试
                    continue;
                } else {
                    Logger->log(spdlog::level::err, "Send failed {}", strerror(errno));
                    std::cerr << "Send failed: " << strerror(errno) << "\n";
                    exit(EXIT_FAILURE);
                }
            }
            totalSent += sent;
    } 
    }


bool Client::Login()
    {
        Logger->log(spdlog::level::info, "用户准备登录！");
        // std::cout << "\x1b[2J";  // 清屏
        // std::cout << "\x1b[H";   // 光标移到左上角
        std::string Account, Password;

   User:std::cout << "请输入你的账户密码:" << std::endl;
        std::cin >> Account >> Password;

        if(Account.size()>=_USERNAME_LENGTH_)
            {
                std::cout << "账户名过长！应该不超过" << _USERNAME_LENGTH_ << "位！" << std::endl;
                goto User;
            }
        if(Password.size()>=_PASSWORD_LENGTH_)
            {
                std::cout << "密码过长！应该不超过" << _USERNAME_LENGTH_ << "位！" << std::endl;
                goto User;
            }

        //写入用户名和密码信息到结构体中
        char* loginbuf = (char*)malloc(sizeof(STRUCT_LOGIN));
        LPSTRUCT_LOGIN login_mem = (STRUCT_LOGIN *)loginbuf;
        //将包体内容复制到缓冲区，并且手动添加空字符
        memcpy(login_mem->username, Account.c_str(), Account.size());
        login_mem->username[Account.size()] = '\0';
        memcpy(login_mem->password, Password.c_str(), Password.size());
        login_mem->password[Password.size()] = '\0';

        //用来释放的指针
        char* Sendbuf;
        //用来发送用的指针
        char* LoginMsgBuf = static_cast<char*>(std::malloc(sizeof(STRUCT_LOGIN) + sizeof(COMM_PKG_HEADER)));
        if(!LoginMsgBuf)
            {
                Logger->log(spdlog::level::err, "内存分配失败！程序退出!");
                exit(-1);
            }

        Sendbuf = LoginMsgBuf;
        // 整个数据包的大小为包头+登录结构体a
        unsigned short MsgDataLength = sizeof(STRUCT_LOGIN) + sizeof(COMM_PKG_HEADER);
        //先写入包头信息
        LPCOMM_PKG_HEADER pkghead = reinterpret_cast<COMM_PKG_HEADER*>(LoginMsgBuf);
        
        pkghead->pkgLen = htons(MsgDataLength);
        pkghead->msgCode = htons(_CMD_LOGIN);
        //生成CRC32校验码
        pkghead->crc32  = htonl(Crc32->Get_CRC((unsigned char *)login_mem, sizeof(STRUCT_LOGIN) ));
        //移动指针位置，开始装填登录消息体
        LoginMsgBuf += sizeof(COMM_PKG_HEADER);

        memcpy(LoginMsgBuf, loginbuf, sizeof(STRUCT_LOGIN));
        //指向包体的指针

        
        sendData(sockfd, Sendbuf, MsgDataLength);
        free(loginbuf);
        free(Sendbuf);

        system("pause");
        return true;

        
    }

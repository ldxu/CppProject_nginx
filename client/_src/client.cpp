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
        string logfilename = Cfg->getString("LogFileName", "log/log.txt");

        Logger = DailyLogger::getInstance();
        Logger->initlogger(logname, logfilename);
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

void Client::GetSocket(int __domain, int __type, int __protocol)
    {
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
    }
void Client::GetConnect()
    {
        int port            = Cfg->getInt("PORT", 8080);
        string IpAddress    = Cfg->getString("IPAddress", "127.0.0.1");
        SIN.sin_family      = AF_INET;
        SIN.sin_port        = htons(port);
        SIN.sin_addr.s_addr = inet_addr(IpAddress.c_str());
        int len             = sizeof(SIN);  

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
bool Client::Login()
    {
        Logger->log(spdlog::level::info, "用户准备登录！");
        std::cout << "\x1b[2J";  // 清屏
        std::cout << "\x1b[H";   // 光标移到左上角
        std::cout << "请输入你的账户密码:" << std::endl;
        char* LoginMsgBuf = (char *)malloc(sizeof(STRUCT_LOGIN));
        LPSTRUCT_LOGIN login_mem = (STRUCT_LOGIN *)LoginMsgBuf;
        std::cin>>login_mem->username;
        std::cin>>login_mem->password;
        // 开始封装包头
        



        
    }

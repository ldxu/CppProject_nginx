#ifndef  _MY_REDIS_H_
#define  _MY_REDIS_H_
#include <iostream>
#include <hiredis/hiredis.h>
#include <string.h>
#include <vector>
class MyRedis
    {
        public:
            ~MyRedis();
            static  MyRedis* GetInstance(const std::string&IPAddress = "127.0.0.1", const int Port = 6379)
                {
                    if(!instance)
                        {
                            if(!instance)
                                {
                                    instance = new MyRedis(IPAddress, Port);
                                    static ResourceRecycle RR;
                                }
                        }
                    return instance;
                }
            class ResourceRecycle
                {
                    public:
                        ~ResourceRecycle()
                            {
                                if(MyRedis::instance)
                                    {
                                        delete MyRedis::instance;
                                        MyRedis::instance = nullptr;
                                    }
                            }
                };
            // SET KEY VALUE
            bool SetStringCommand(const std::string& key, const std::string& value);
            // GET KEY 
            std::string GetStringCommand(const std::string& key);
            // SADD KEY VALUE
            bool SaddCommand(const std::string& key, const std::string& value);
            // 得到群组成员名单
            std::vector<std::string> SmembersCommand(const std::string& key);
            // 暂时用不到
            // bool SetHashCommand(const string& key, const string& filed, const string& value);
            // bool GetHashCommand(const string& key, const string& filed);
            //用来判断群组是否存在
            unsigned char SaddExists(const std::string& key, const std::string& member);
            //主要用来判断在线用户是否存在以及账户是否存在 但是可以直接通过get指令判断，暂时不添加
            // bool StringExists(const string* key);

        private:
            static MyRedis* instance;
            MyRedis(const std::string& IPAddress, const int Port);
            redisContext*   c;
    };
#endif
#include "My_redis.h"

MyRedis* MyRedis::instance = nullptr;

MyRedis::MyRedis(const std::string& IPAddress, const int Port)
    {
        c = redisConnect(IPAddress.c_str(), Port);
        if(c !=NULL && c->err)
            {
                std::cerr << "Redis Start Error! Error:" << c->errstr << std::endl;
                exit(-1);
            }
    }
MyRedis::~MyRedis()
    {
        redisFree(c);
        c = nullptr;
    }

// true设置成功 false设置失败
bool MyRedis::SetStringCommand(const std::string& key, const std::string& value)
    {
        redisReply* reply = (redisReply*) redisCommand(c, "SET %s %s", key.c_str(), value.c_str() );
        if(reply->type == REDIS_REPLY_STATUS && (strcmp(reply->str, "OK")==0) )
            {
                std::cout << 23123 << std::endl;
                freeReplyObject(reply);
                return true;
            }
        else
            {
                freeReplyObject(reply);
                return false;
            }
    }
// 成功则返回value，不存在则是nil，执行错误failed
std::string MyRedis::GetStringCommand(const std::string& key)
    {
        redisReply* reply = (redisReply*) redisCommand(c, "GET %s", key.c_str() );
        if(reply->type == REDIS_REPLY_STRING)
            {
                freeReplyObject(reply);
                return std::string(reply->str);
            }
        else if(reply->type == REDIS_REPLY_NIL)
            {
                freeReplyObject(reply);
                return "nil";
            }
        else
            {
                freeReplyObject(reply);
                return "failed";
            }
    }
// 1代表成功 2代表失败
bool MyRedis::SaddCommand(const std::string& key, const std::string& value)
    {
         redisReply* reply = (redisReply*) redisCommand(c, "SADD %s %s", key.c_str(), value.c_str());
         if(reply->type == REDIS_REPLY_INTEGER)
            {
                freeReplyObject(reply);
                return true;
            }
        else
            {
                freeReplyObject(reply);
                return false;
            }

    }

std::vector<std::string> MyRedis::SmembersCommand(const std::string& member)
    {
        std::vector<std::string> ArrayList;
        redisReply* reply = (redisReply*) redisCommand(c, "SMEMBERS %s", member.c_str());
        if(reply->type == REDIS_REPLY_ARRAY)
            {
                for(size_t i=0;i<reply->elements;++i)
                    {
                        ArrayList.push_back(std::string(reply->element[i]->str));
                    }
                freeReplyObject(reply);
                return ArrayList;
            }
        else
            {
                freeReplyObject(reply);
                return std::vector<std::string>();
            }
    }
// 1代表存在 0代表不存在 2代表执行失败
unsigned char MyRedis::SaddExists(const std::string& key, const std::string& member)
    {
        redisReply* reply = (redisReply*) redisCommand(c, "SISMEMBER %s %s", key.c_str(), member.c_str());
        if(reply->type == REDIS_REPLY_INTEGER)
            {
                if(reply->integer == 1)
                    {
                        freeReplyObject(reply);
                        return 1;
                    }
                else
                    {
                        freeReplyObject(reply);
                        return 0;
                    }
            }
        else
            {
                freeReplyObject(reply);
                return 2;
            }
    }




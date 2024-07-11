
#ifndef __CLI_CONFIG_H__
#define __CLI_CONFIG_H__

#include <vector>
#include <iostream>
#include <libconfig.h++>

using namespace libconfig;
using namespace std;

class ConfigManager {
public:
    // 获取单例实例
    static ConfigManager* getInstance() {
        if(!Instance)
            {
                if(!Instance)
                    {
                        Instance = new ConfigManager();
                        static  ResourceRecycle RR;
                    }
            }

        return Instance;
    }
    class ResourceRecycle{
        public:
            ~ResourceRecycle()
                {
                    if(ConfigManager::Instance)
                        {
                            delete ConfigManager::Instance;
                            ConfigManager::Instance = nullptr;
                        }
                }
    };
    // 删除拷贝构造函数和赋值运算符，以确保单例模式
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;

    // 加载配置文件
    bool loadConfig(const string& filename);

    // 获取字符串类型的配置项
    string getString(const string& path, const string& defaultValue = "");

    // 获取整数类型的配置项
    int getInt(const string& path, int defaultValue = 0);

    // 获取浮点数类型的配置项
    double getFloat(const string& path, double defaultValue = 0.0);

    // 获取布尔类型的配置项
    bool getBool(const string& path, bool defaultValue = false);

private:
    Config cfg;
    static ConfigManager* Instance;
    // 私有化构造函数以确保单例模式
    ConfigManager() = default;
};


#endif

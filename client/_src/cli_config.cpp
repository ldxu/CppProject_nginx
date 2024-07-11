#include "cli_config.h"
    ConfigManager* ConfigManager::Instance = nullptr;
    
    bool ConfigManager::loadConfig(const string& filename) {
        try {
            cfg.readFile(filename.c_str());
            return true;
        } catch (const FileIOException &fioex) {
            cerr << "I/O error while reading file." << endl;
            return false;
        } catch (const ParseException &pex) {
            cerr << "Parse error at " << pex.getFile() << ":" << pex.getLine()
                 << " - " << pex.getError() << endl;
            return false;
        }
    }

    // 获取字符串类型的配置项
    string ConfigManager::getString(const string& path, const string& defaultValue) {
        try {
            return cfg.lookup(path);
        } catch (const SettingNotFoundException &nfex) {
            return defaultValue;
        }
    }

    // 获取整数类型的配置项
    int ConfigManager::getInt(const string& path, int defaultValue) {
        try {
            return cfg.lookup(path);
        } catch (const SettingNotFoundException &nfex) {
            return defaultValue;
        }
    }

    // 获取浮点数类型的配置项
    double ConfigManager::getFloat(const string& path, double defaultValue) {
        try {
            return cfg.lookup(path);
        } catch (const SettingNotFoundException &nfex) {
            return defaultValue;
        }
    }

    // 获取布尔类型的配置项
    bool ConfigManager::getBool(const string& path, bool defaultValue) {
        try {
            return cfg.lookup(path);
        } catch (const SettingNotFoundException &nfex) {
            return defaultValue;
        }
    }
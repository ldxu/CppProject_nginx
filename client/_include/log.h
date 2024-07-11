#ifndef __LOG_H__
#define __LOG_H__
// #include <iostream>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/daily_file_sink.h>
// #include <memory>
#include <mutex>
#include <string>
#include <chrono>
#include <iomanip> // for std::put_time


class DailyLogger {
public:
    // 获取单例实例，指定日志器名称和日志文件名
    static DailyLogger* getInstance() {

        if(!instance)
            {
                if(!instance)
                    {
                        instance = new DailyLogger();
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
                if(DailyLogger::instance)
                    {
                        delete DailyLogger::instance;
                        DailyLogger::instance = nullptr;
                    }
            }
    };

    
    // 设置日志级别
    void setLogLevel(spdlog::level::level_enum level);
    void initlogger(const std::string& logger_name = "Daily Log", const std::string& log_filename = "../log/log.txt");
    // 记录带参数的日志，包括时间戳
    template<typename... Args>
    void log(spdlog::level::level_enum level, const char* fmt, const Args&... args) {
        std::lock_guard<std::mutex> lock(mutex_);
        // 先格式化消息
        std::string formatted_msg = fmt::format(fmt, args...);
        logger_->log(level, formatted_msg);
    }

    ~DailyLogger();
private:
    std::shared_ptr<spdlog::logger> logger_;
    std::mutex mutex_; // 互斥锁，确保线程安全
    static DailyLogger* instance;
    DailyLogger();

    // 禁止复制和赋值
    DailyLogger(const DailyLogger&) = delete;
    DailyLogger& operator=(const DailyLogger&) = delete;

};
#endif
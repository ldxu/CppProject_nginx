#include "log.h"

DailyLogger *DailyLogger::instance = nullptr;


void DailyLogger::setLogLevel(spdlog::level::level_enum level)
    {
        logger_->set_level(level);
    }
void DailyLogger::initlogger(const std::string& logger_name, const std::string& log_filename)
    {
        logger_ = spdlog::daily_logger_format_mt(logger_name, log_filename, 0, 0);
    }

DailyLogger::~DailyLogger()
    {
        spdlog::drop_all();
    }
DailyLogger::DailyLogger(){}


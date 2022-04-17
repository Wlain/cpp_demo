//
// Created by william on 2021/10/12.
//

#include "logger.h"

#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>

//void Logger::log(Logger::Level level, const char* message, ...)
//{
//    time_t currentTime = time(nullptr);
//    char timeBuf[100];
//    strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));
//    std::vector<char> dynamicBuffer;
//    int size = 1024;
//    char stackBuffer[1024];
//    char* str = stackBuffer;
//    for (;;)
//    {
//        va_list args;
//        va_start(args, message);
//        int needed = vsnprintf(str, size - 1, message, args);
//        // the number of characters actually needed to fill the buffer.
//        if (needed >= 0 && needed < size)
//        {
//            // Successfully wrote buffer. Added a NULL terminator in case it wasn't written.
//            str[needed] = '\0';
//            va_end(args);
//            break;
//        }
//        size = needed > 0 ? (needed + 1) : (size * 2);
//        dynamicBuffer.resize(size);
//        str = &dynamicBuffer[0];
//        va_end(args);
//    }
//    switch (level)
//    {
//    case Level::Info:
//        printf("%s, [Logger::Level::Info] : %s", timeBuf, str);
//        break;
//    case Level::Warn:
//        printf("%s, [Logger::Level::Warn] : %s", timeBuf, str);
//        break;
//    case Level::Error:
//        printf("%s, [Logger::Level::Error] : %s", timeBuf, str);
//        break;
//    case Level::Debug:
//        break;
//    case Level::Fatal:
//        break;
//    }
//    printf("\n");
//}

Logger::Logger()
{
    auto stdoutSink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    auto rotatingSink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("mylog.txt", 1024 * 1024 * 10, 3);
    std::vector<spdlog::sink_ptr> sinks{ stdoutSink, rotatingSink };
    spdlog::init_thread_pool(8192, 1);
    m_logger = std::make_shared<spdlog::async_logger>("Logger", sinks.begin(), sinks.end(), spdlog::thread_pool(), spdlog::async_overflow_policy::block);
    spdlog::register_logger(m_logger);
}

Logger::~Logger()
{
    m_logger->flush();
    spdlog::drop_all();
}

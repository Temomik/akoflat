#pragma once

#include <iostream>
#include <mutex>
#include <string_view>

#include "Color.h"

#define LOG_INFO(what) Logger::Print(what, "INFO", __FILE__, __LINE__, Color::Modifier(Color::BG_GREEN));
#define LOG_WARNING(what) Logger::Print(what, "INFO", __FILE__, __LINE__, Color::Modifier(Color::BG_YELLOW));
#define LOG_ERROR(what) Logger::Print(what, "INFO", __FILE__, __LINE__, Color::Modifier(Color::BG_RED));

class Logger
{
public:
    static void Print(const std::string& what, const std::string& type,
                      const std::string_view& file, const size_t line,
                      Color::Modifier color)
    {
        std::lock_guard lock(mMutex);

        auto filename = GetFilename(file);

        Color::Modifier whitefg(Color::FG_WHITE);

        Color::Modifier reset(Color::RESET);

        std::cout << whitefg
                  << color << type << "|"
                  << GetTime() << "|"
                  << filename << "," << line << "|" << reset
                  << what << "|" << std::endl;
    }

private:
    static std::string GetFilename(const std::string_view& path)
    {
        auto pos = path.find_last_of('/');
        std::string filename;

        if (pos == std::string_view::npos)
        {
            filename = path;
        }
        else
        {
            filename = path.substr(pos + 1);
        }

        return filename;
    }

    static std::string GetTime()
    {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

        std::string s(30, '\0');
        std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return s;
    }

    inline static std::mutex mMutex;
};

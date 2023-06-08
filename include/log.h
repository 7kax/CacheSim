#pragma once
#include <string>
#include <fstream>

namespace logger
{
    class logger
    {
    private:
        std::ofstream ofs;

    public:
        logger();
        void log(std::string str);
        ~logger();
    };
    extern logger mLogger;
}
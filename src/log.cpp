#include "log.h"
#include <fstream>
using namespace std;

namespace logger
{
    logger mLogger{};

    logger::logger()
    {
        ofs.open("../log.txt");
    }

    void logger::log(string str)
    {
        ofs << str << endl;
    }

    logger::~logger()
    {
        ofs.close();
    }
}
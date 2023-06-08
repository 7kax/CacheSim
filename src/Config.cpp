#include "Config.h"
#include "log.h"
using namespace std;

namespace Config
{
    configManager cManager{};

    void configManager::initConfig(istream &in)
    {
        int temp1, temp2;
        in >> blockSize >> association >> cacheSize >> temp1 >> missCost >> temp2;
        logger::mLogger.log("blockSize: " + to_string(blockSize) + " B");
        logger::mLogger.log("association: " + to_string(association));
        logger::mLogger.log("cacheSize: " + to_string(cacheSize) + " KB");
        logger::mLogger.log("missCost: " + to_string(missCost) + " cycles");
        if (temp1 == 0)
        {
            replaceStrategy = replace::Random;
            logger::mLogger.log("replaceStrategy: Random");
        }
        else if (temp1 == 1)
        {
            replaceStrategy = replace::LRU;
            logger::mLogger.log("replaceStrategy: LRU");
        }
        if (temp2 == 0)
        {
            missStrategy = miss::WriteAllocate;
            logger::mLogger.log("missStrategy: WriteBack");
        }
        else if (temp2 == 1)
        {
            missStrategy = miss::WriteBack;
            logger::mLogger.log("missStrategy: WriteAllocate");
        }
    }

}

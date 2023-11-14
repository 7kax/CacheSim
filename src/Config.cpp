#include "objects.h"

using namespace std;

namespace CacheSim {
configManager cManager{};

void configManager::initConfig(istream &in) {
    int temp1, temp2;
    in >> blockSize >> association >> cacheSize >> temp1 >> missCost >> temp2;
    CacheSim::mLogger.log("blockSize: " + to_string(blockSize) + " B");
    CacheSim::mLogger.log("association: " + to_string(association));
    CacheSim::mLogger.log("cacheSize: " + to_string(cacheSize) + " KB");
    CacheSim::mLogger.log("missCost: " + to_string(missCost) + " cycles");
    if (temp1 == 0) {
        replaceStrategy = replace::Random;
        CacheSim::mLogger.log("replaceStrategy: Random");
    } else if (temp1 == 1) {
        replaceStrategy = replace::LRU;
        CacheSim::mLogger.log("replaceStrategy: LRU");
    }
    if (temp2 == 0) {
        missStrategy = miss::WriteAllocate;
        CacheSim::mLogger.log("missStrategy: WriteBack");
    } else if (temp2 == 1) {
        missStrategy = miss::WriteBack;
        CacheSim::mLogger.log("missStrategy: WriteAllocate");
    }
}
} // namespace CacheSim

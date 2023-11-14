#include "objects.h"

using namespace std;

namespace CacheSim {
IOManager iManager{};

const string IOManager::configMark = "-c";
const string IOManager::traceMark = "-t";
const string IOManager::outputMark = "-o";

void IOManager::initIO(int argc, char *argv[]) {
    for (int i = 1; i < argc - 1; i += 2) {
        string key = argv[i];
        string value = argv[i + 1];
        if (key == configMark) {
            CacheSim::mLogger.log("config file: " + value);
            ifstream ifs{"../" + value};
            CacheSim::cManager.initConfig(ifs);
            ifs.close();
        } else if (key == traceMark) {
            CacheSim::mLogger.log("trace file: " + value);
            traceFile = value;
        } else if (key == outputMark) {
            CacheSim::mLogger.log("output file: " + value);
            outputFile = value;
        }
    }
}

} // namespace CacheSim

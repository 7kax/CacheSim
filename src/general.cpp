#include "objects.h"

using namespace std;

namespace CacheSim {
top TOP{};

void top::initGeneral() {
    ifstream ifs{"../" + CacheSim::iManager.getTraceFile()};
    string line;
    operation currentOp;
    stringstream ss;
    while (getline(ifs, line)) {
        if (line.size() == 4)
            break;
        if (line[16] == 'W')
            currentOp.op = opType::W;
        else
            currentOp.op = opType::R;
        ss << hex << line.substr(20, 12);
        ss >> currentOp.Address;
        ss.clear();
        // cout << (int)currentOp.op << currentOp.Address << endl;
        operations.push_back(currentOp);
    }
    CacheSim::mLogger.log("trace size: " + to_string(operations.size()) +
                          " lines");
    loadCnt = 0;
    loadHit = 0;
    storeCnt = 0;
    storeHit = 0;
}
void top::run() {
    CacheSim::mCache.init();
    // cache::mCache.print();
    for (auto &o : operations) {
        if (o.op == opType::R) {
            loadCnt++;
            if (CacheSim::mCache.read(o.Address))
                loadHit++;
        } else if (o.op == opType::W) {
            storeCnt++;
            if (CacheSim::mCache.write(o.Address))
                storeHit++;
        }
    }
}
void top::showInfo() {
    CacheSim::mLogger.log("loadCnt: " + to_string(loadCnt));
    CacheSim::mLogger.log("loadHit: " + to_string(loadHit));
    CacheSim::mLogger.log("storeCnt: " + to_string(storeCnt));
    CacheSim::mLogger.log("storeHit: " + to_string(storeHit));
    CacheSim::mLogger.log("");
    double totalHitRate = static_cast<double>(this->loadHit + this->storeHit) /
                          static_cast<double>(this->loadCnt + this->storeCnt) *
                          100;
    double loadHitRate = static_cast<double>(this->loadHit) /
                         static_cast<double>(this->loadCnt) * 100;
    double storeHitRate = static_cast<double>(this->storeHit) /
                          static_cast<double>(this->storeCnt) * 100;
    unsigned long long totalRunTime =
        this->loadHit + this->storeHit +
        (CacheSim::cManager.getMissCost() + 1) *
            (this->loadCnt + this->storeCnt - this->loadHit - this->storeHit);
    double AVGLatency = static_cast<double>(totalRunTime) /
                        static_cast<double>(operations.size());
    if (!CacheSim::iManager.getOutputFile().empty()) {
        ofstream ofs{"../" + CacheSim::iManager.getOutputFile()};
        ofs << fixed << setprecision(2) << "Total Hit Rate: " << totalHitRate
            << "%\n"
            << "Load Hit Rate : " << loadHitRate << "%\n"
            << "Store Hit Rate: " << storeHitRate << "%\n"
            << "Total Run Time: " << totalRunTime << "\n"
            << "AVG MA Latency: " << AVGLatency << "\n";
        return;
    }
    cout << fixed << setprecision(2) << "Total Hit Rate: " << totalHitRate
         << "%\n"
         << "Load Hit Rate : " << loadHitRate << "%\n"
         << "Store Hit Rate: " << storeHitRate << "%\n"
         << "Total Run Time: " << totalRunTime << "\n"
         << "AVG MA Latency: " << AVGLatency;
    // cache::mCache.print();
}
} // namespace CacheSim

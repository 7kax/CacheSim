#include "general.h"
#include "IO.h"
#include "cache.h"
#include "Config.h"
#include "log.h"
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

namespace general
{
    top TOP{};

    void top::initGeneral()
    {
        ifstream ifs{"../" + IO::iManager.getTraceFile()};
        string line;
        operation currentOp;
        stringstream ss;
        while (getline(ifs, line))
        {
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
        logger::mLogger.log("trace size: " + to_string(operations.size()) + " lines");
        loadCnt = 0;
        loadHit = 0;
        storeCnt = 0;
        storeHit = 0;
    }
    void top::run()
    {
        cache::mCache.init();
        // cache::mCache.print();
        for (auto &o : operations)
        {
            if (o.op == opType::R)
            {
                loadCnt++;
                if (cache::mCache.read(o.Address))
                    loadHit++;
            }
            else if (o.op == opType::W)
            {
                storeCnt++;
                if (cache::mCache.write(o.Address))
                    storeHit++;
            }
        }
    }
    void top::showInfo()
    {
        logger::mLogger.log("loadCnt: " + to_string(loadCnt));
        logger::mLogger.log("loadHit: " + to_string(loadHit));
        logger::mLogger.log("storeCnt: " + to_string(storeCnt));
        logger::mLogger.log("storeHit: " + to_string(storeHit));
        logger::mLogger.log("");
        double totalHitRate = static_cast<double>(this->loadHit + this->storeHit) / static_cast<double>(this->loadCnt + this->storeCnt) * 100;
        double loadHitRate = static_cast<double>(this->loadHit) / static_cast<double>(this->loadCnt) * 100;
        double storeHitRate = static_cast<double>(this->storeHit) / static_cast<double>(this->storeCnt) * 100;
        unsigned long long totalRunTime = this->loadHit + this->storeHit + (Config::cManager.getMissCost() + 1) * (this->loadCnt + this->storeCnt - this->loadHit - this->storeHit);
        double AVGLatency = static_cast<double>(totalRunTime) / static_cast<double>(operations.size());
        if (!IO::iManager.getOutputFile().empty())
        {
            ofstream ofs{"../" + IO::iManager.getOutputFile()};
            ofs << fixed << setprecision(2)
                << "Total Hit Rate: " << totalHitRate << "%\n"
                << "Load Hit Rate : " << loadHitRate << "%\n"
                << "Store Hit Rate: " << storeHitRate << "%\n"
                << "Total Run Time: " << totalRunTime << "\n"
                << "AVG MA Latency: " << AVGLatency << "\n";
            return;
        }
        cout << fixed << setprecision(2)
             << "Total Hit Rate: " << totalHitRate << "%\n"
             << "Load Hit Rate : " << loadHitRate << "%\n"
             << "Store Hit Rate: " << storeHitRate << "%\n"
             << "Total Run Time: " << totalRunTime << "\n"
             << "AVG MA Latency: " << AVGLatency;
        // cache::mCache.print();
    }
}

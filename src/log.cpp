#include "objects.h"
using namespace std;

namespace CacheSim {
logger mLogger{};

logger::logger() { ofs.open("../log.txt"); }

void logger::log(string str) { ofs << str << endl; }

logger::~logger() { ofs.close(); }
} // namespace CacheSim
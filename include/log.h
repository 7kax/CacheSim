#pragma once
#include <fstream>
#include <string>

namespace CacheSim {
class logger {
  private:
    std::ofstream ofs;

  public:
    logger();
    void log(std::string str);
    ~logger();
};
} // namespace CacheSim
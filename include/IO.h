#pragma once

#include "defs.h"

namespace CacheSim {
class IOManager {
  private:
    std::string traceFile;
    std::string outputFile;
    static const std::string configMark; //-c
    static const std::string traceMark;  //-t
    static const std::string outputMark; //-o

  public:
    void initIO(int, char *[]);
    std::string getTraceFile() { return this->traceFile; }
    std::string getOutputFile() { return this->outputFile; }
};
} // namespace CacheSim
#pragma once

#include "defs.h"

namespace CacheSim {
enum replace { Random, LRU };
enum miss { WriteBack, WriteAllocate };
class configManager {
  private:
    int blockSize;
    int association;
    int cacheSize;
    int missCost;
    replace replaceStrategy;
    miss missStrategy;

  public:
    void initConfig(std::istream &); // 通过输入流初始化配置
    int getMissCost() { return this->missCost; }
    int getCacheSize() { return this->cacheSize; }
    int getBlockSize() { return this->blockSize; }
    int getAssociation() { return this->association; }
    replace getReplaceStrategy() { return this->replaceStrategy; }
    miss getMissStrategy() { return this->missStrategy; }
};
} // namespace CacheSim
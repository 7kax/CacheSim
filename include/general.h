#pragma once

#include "defs.h"

namespace CacheSim {
enum opType { R, W };
struct operation {
    address_t Address; // 访存地址
    opType op;         // 操作类型: 读/写
};
class top {
  private:
    std::vector<operation> operations; // 存储所有操作
    int loadCnt;                       // load操作的次数
    int loadHit;                       // load操作的命中次数
    int storeCnt;                      // store操作的次数
    int storeHit;                      // store操作的命中次数

  public:
    void initGeneral(); // 初始化: 读取trace文件
    void run();         // 运行: 模拟cache
    void showInfo();    // 显示信息: 命中率等
};
} // namespace CacheSim
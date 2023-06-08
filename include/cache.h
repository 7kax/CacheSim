#pragma once
#include <vector>
#include <optional>
#include <random>

namespace cache
{
    struct block
    {
        bool V{false};             // 有效位
        bool dirty{false};         // 修改位
        unsigned long long tag{0}; // 标记位
        int cnt{0};                // 计数器
    };
    class group
    {
    private:
        std::vector<block> blocks{};                        // 缓存块
        std::optional<int> findTag(unsigned long long tag); // 查找标记位, 如果查到返回块号, 否则返回空
        std::optional<int> full();                          // 查找是否有空闲块, 如果有返回块号, 否则返回空

    public:
        void setSize(int size);                    // 设置组大小
        bool read_random(unsigned long long tag);  // 随机读
        bool read_LRU(unsigned long long tag);     // LRU读
        bool write_random(unsigned long long tag); // 随机写
        bool write_LRU(unsigned long long tag);    // LRU写
        void print();                              // 打印组信息
    };
    class cache
    {
    private:
        std::vector<group> groups{};            // 缓存组
        std::default_random_engine e{};         // 随机数引擎
        std::uniform_int_distribution<int> u{}; // 均匀分布

    public:
        int getRand();                       // 获取随机数
        void init();                         // 初始化缓存
        bool read(unsigned long long addr);  // 读取数据
        bool write(unsigned long long addr); // 写入数据
        void print();                        // 打印缓存信息
    };

    extern cache mCache;

}
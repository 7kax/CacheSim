#include "cache.h"
#include "Config.h"
#include "log.h"
using namespace std;

namespace cache
{
    cache mCache{};

    int cache::getRand()
    {
        return this->u(this->e);
    }

    void cache::init()
    {
        if (Config::cManager.getAssociation() == 0)
        {
            logger::mLogger.log("----fully associative cache----");
            this->groups.resize(1);
            int groupNum = Config::cManager.getCacheSize() * 1024 / Config::cManager.getBlockSize();
            this->groups[0].setSize(groupNum);
            this->u = uniform_int_distribution<int>(0, groupNum - 1);
            logger::mLogger.log("groupSize: " + to_string(groupNum) + " blocks");
            return;
        }
        logger::mLogger.log("----" + to_string(Config::cManager.getAssociation()) + "-way associative cache----");
        int groupNum = Config::cManager.getCacheSize() * 1024 / Config::cManager.getAssociation() / Config::cManager.getBlockSize();
        int groupSize = Config::cManager.getAssociation();
        this->groups.resize(groupNum);
        for (auto &group : this->groups)
        {
            group.setSize(groupSize);
        }
        this->u = uniform_int_distribution<int>(0, groupSize - 1);
        logger::mLogger.log("groupNum: " + to_string(groupNum) + " groups");
        logger::mLogger.log("groupSize: " + to_string(groupSize) + " blocks");
    }

    bool cache::read(unsigned long long addr)
    {
        int groupNum = addr / Config::cManager.getBlockSize() % this->groups.size();
        unsigned long long blockNum = addr / Config::cManager.getBlockSize() / this->groups.size();
        // return this->groups[groupNum].read(blockNum);
        switch (Config::cManager.getReplaceStrategy())
        {
        case Config::replace::Random:
            return this->groups[groupNum].read_random(blockNum);
            break;
        case Config::replace::LRU:
            return this->groups[groupNum].read_LRU(blockNum);
            break;
        default:
            break;
        }
        return false;
    }

    bool cache::write(unsigned long long addr)
    {
        int groupNum = addr / Config::cManager.getBlockSize() % this->groups.size();
        unsigned long long blockNum = addr / Config::cManager.getBlockSize() / this->groups.size();
        // return this->groups[groupNum].write(blockNum);
        switch (Config::cManager.getReplaceStrategy())
        {
        case Config::replace::Random:
            return this->groups[groupNum].write_random(blockNum);
            break;
        case Config::replace::LRU:
            return this->groups[groupNum].write_LRU(blockNum);
            break;
        default:
            break;
        }
        return false;
    }

    void cache::print()
    {
        for (int i = 0; i < this->groups.size(); i++)
        {
            this->groups[i].print();
        }
    }

    optional<int> group::findTag(unsigned long long tag)
    {
        for (int i = 0; i < this->blocks.size(); i++)
        {
            if (this->blocks[i].V && this->blocks[i].tag == tag)
            {
                return i;
            }
        }
        return nullopt;
    }

    std::optional<int> group::full()
    {
        for (int i = 0; i < this->blocks.size(); i++)
        {
            if (!this->blocks[i].V)
            {
                return i;
            }
        }
        return nullopt;
    }

    void group::setSize(int size)
    {
        this->blocks.resize(size);
    }

    bool group::read_LRU(unsigned long long tag)
    {
        if (auto index = this->findTag(tag); index.has_value())
        {
            for (auto &block : this->blocks)
            {
                if (block.V && block.cnt < this->blocks[index.value()].cnt)
                {
                    block.cnt++;
                }
            }
            this->blocks[index.value()].cnt = 0;
            return true;
        }
        if (auto index = this->full(); index.has_value())
        {
            for (int i = 0; i < this->blocks.size(); i++)
            {
                if (i != index.value() && this->blocks[i].V)
                {
                    this->blocks[i].cnt++;
                }
                else if (i == index.value())
                {
                    this->blocks[i].V = true;
                    this->blocks[i].dirty = false;
                    this->blocks[i].cnt = 0;
                    this->blocks[i].tag = tag;
                }
            }
            return false;
        }
        for (auto &block : this->blocks)
        {
            if (block.cnt == this->blocks.size() - 1)
            {
                block.dirty = false;
                block.cnt = 0;
                block.tag = tag;
            }
            else
            {
                block.cnt++;
            }
        }
        return false;
    }

    bool group::read_random(unsigned long long tag)
    {
        if (auto index = this->findTag(tag); index.has_value())
        {
            return true;
        }
        if (auto index = this->full(); index.has_value())
        {
            this->blocks[index.value()].V = true;
            this->blocks[index.value()].dirty = false;
            this->blocks[index.value()].tag = tag;
            return false;
        }
        int index = mCache.getRand();
        this->blocks[index].V = true;
        this->blocks[index].dirty = false;
        this->blocks[index].tag = tag;
        return false;
    }

    bool group::write_LRU(unsigned long long tag)
    {
        if (auto index = this->findTag(tag); index.has_value())
        {
            for (auto &block : this->blocks)
            {
                if (block.V && block.cnt < this->blocks[index.value()].cnt)
                {
                    block.cnt++;
                }
            }
            this->blocks[index.value()].cnt = 0;
            this->blocks[index.value()].dirty = true;
            return true;
        }
        if (Config::cManager.getMissStrategy() == Config::miss::WriteAllocate)
            return false;
        if (auto index = this->full(); index.has_value())
        {
            for (int i = 0; i < this->blocks.size(); i++)
            {
                if (i != index.value() && this->blocks[i].V)
                {
                    this->blocks[i].cnt++;
                }
                else if (i == index.value())
                {
                    this->blocks[i].V = true;
                    this->blocks[i].dirty = true;
                    this->blocks[i].cnt = 0;
                    this->blocks[i].tag = tag;
                }
            }
            return false;
        }
        for (auto &block : this->blocks)
        {
            if (block.cnt == this->blocks.size() - 1)
            {
                block.dirty = true;
                block.cnt = 0;
                block.tag = tag;
            }
            else
            {
                block.cnt++;
            }
        }
        return false;
    }

    void group::print()
    {
        for (auto &block : this->blocks)
        {
            std::cout << block.V << " " << block.dirty << " " << block.tag << " " << block.cnt << ", ";
        }
        std::cout << std::endl;
    }

    bool group::write_random(unsigned long long tag)
    {
        if (auto index = this->findTag(tag); index.has_value())
        {
            this->blocks[index.value()].dirty = true;
            return true;
        }
        if (Config::cManager.getMissStrategy() == Config::miss::WriteAllocate)
            return false;
        if (auto index = this->full(); index.has_value())
        {
            this->blocks[index.value()].V = true;
            this->blocks[index.value()].dirty = true;
            this->blocks[index.value()].tag = tag;
            return false;
        }
        int index = mCache.getRand();
        this->blocks[index].V = true;
        this->blocks[index].dirty = true;
        this->blocks[index].tag = tag;
        return false;
    }
}

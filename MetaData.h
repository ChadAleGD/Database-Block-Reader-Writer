#pragma once

using namespace std;

class MetaData
{
public:
    uint16_t recordCount;
    uint16_t remainingBlockSize;

    // Constructor
    MetaData(int totalBlockSize)
    {
        recordCount = 0;
        remainingBlockSize = totalBlockSize - sizeof(int) * 2;
    }

    // Update the remaining block size, this will assume we want to remove
    void UpdateReminingBlockSize(int bytes)
    {
        remainingBlockSize -= bytes;
    }

    void UpdateRecordCount(int amount)
    {
        recordCount += amount;
    }
};
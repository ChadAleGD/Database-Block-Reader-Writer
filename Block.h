#pragma once

#include "Record.h"
#include "MetaData.h"
#include <fstream>
#include <vector>
using namespace std;

class Block
{
private:
    // Information that will be useful when reading file data
    MetaData *metaData;
    vector<Record> recordsInBlock;

public:
    // Constructor
    Block(MetaData *metaData)
    {
        this->metaData = metaData;

        cout << "Space for records " << metaData->remainingBlockSize << " bytes" << endl;
    }

    // AttemptRecordWrite will check to see if there is enough space for the new record to fit into the current block
    // If it does not then it will return false which will prompt TaxRecordWriter to create a new block and try again
    bool AttemptRecordWrite(Record record)
    {
        // Calculate new record data and storage needed
        int spaceOnMetaData = metaData->remainingBlockSize;
        int recordSize = record.GetSize();

        if (spaceOnMetaData - recordSize <= 0)
        {
            // cout << "Not enough space" << endl;
            return false;
        }

        // If the above condition is not caught we can safely add this record to the block
        // This also updates metadata information
        recordsInBlock.push_back(record);
        metaData->UpdateRecordCount(1);
        metaData->UpdateReminingBlockSize(recordSize);

        // Print out information about the block
        cout << recordSize << " bytes written" << endl;
        cout << metaData->remainingBlockSize << " bytes remaining in block" << endl;

        return true;
    }

    // Push information onto the file
    // I'm going to be passing in the reference to the same fstream created in TaxRecordWriter
    // This will create the least amount of copying of the same fstream and file name
    void writeBlockToDisk(fstream &file)
    {
        // Before writing anything make sure that file is actually open
        if (!file.is_open())
        {
            cout << "Error writing to file. File is not open." << endl;
            return;
        }

        // cout << "Writing block to disk..." << endl;

        // First write the block metadata in
        file.write((char *)&metaData->recordCount, sizeof(metaData->recordCount));
        file.write((char *)&metaData->remainingBlockSize, sizeof(metaData->remainingBlockSize));

        // Make sure the status of the file is good. Should the file not write metadata successfully
        // then file.good() will return false meaning something went wrong and no data was written
        if (!file.good())
        {
            cout << "Failed writing metadata" << endl;
            return;
        }

        // For each record that is stored in the blocks vector of blocks we will write the data
        // onto file
        for (Record &record : recordsInBlock)
        {
            // cout << "Writing Record" << endl;

            // I REMOVED ALL THE CHATGPTD HELP I SWEAR I WASN'T GOING TO ACTUALLY USE IT IN THE
            // FINAL CODE THATS WHY I WENT TO YOUR OFFICE FOR HELP o7

            // SSN
            file.write((char *)&record.ssn, sizeof(record.ssn));

            // Name
            file.write(record.name.c_str(), record.name.size() + 1);

            // Income
            file.write((char *)&record.income, sizeof(record.income));

            // State
            file.write(record.state.c_str(), 2);
        }

        // If there is any leftover space from unused bytes we have to take that into account when writing our
        // total block size. I'm using a char* right away to just make an empty string of chars the size of
        // remainingBlockSize (so for example remainingBlockSize = 5 --> [     ]) and write those empty characters
        if (metaData->remainingBlockSize > 0)
        {
            char *extraSize = new char[metaData->remainingBlockSize]();
            file.write(extraSize, metaData->remainingBlockSize);
            delete[] extraSize;
        }

        // Flush contents of the file to ensure everything wrote
        file.flush();

        // Lastly make sure that each record succesfully wrote
        if (!file.good())
        {
            cout << "Error writing records to file" << endl;
        }
    }

    // Return back the metadata of this current block
    MetaData *GetMetaData()
    {
        return metaData;
    }

    ~Block()
    {
        delete metaData;
    }
};
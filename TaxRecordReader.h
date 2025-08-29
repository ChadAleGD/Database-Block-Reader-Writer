#pragma once

#include <string>
#include <vector>
#include <fstream>
using namespace std;

#include "Record.h"

class TaxRecordReader
{
private:
  // Variables
  fstream file;
  streampos readPosition;

public:
  // Constructor
  TaxRecordReader(string fileName, unsigned int blockSize)
  {
    // I don't really need blockSize here since MetaData takes care of that for me
    readPosition = 0;

    file.open(fileName, ios::out | ios::in | ios::binary);
  }

  vector<Record> readPage()
  {
    vector<Record> page;

    // Metadata Record Count
    file.seekg(readPosition);
    uint16_t metaDataNumRec;
    file.read((char *)&metaDataNumRec, sizeof(metaDataNumRec));

    // cout << "Record Count: " << metaDataNumRec << endl;

    // Metadata Remaining Block Size
    readPosition += sizeof(uint16_t);

    file.seekg(readPosition);
    uint16_t metaDataRemainingBlockSize;
    file.read((char *)&metaDataRemainingBlockSize, sizeof(metaDataRemainingBlockSize));

    // cout << "Remaining Block Size: " << metaDataRemainingBlockSize << endl;

    readPosition += sizeof(uint16_t);

    // Metadata holds the value of how many records there are in the current
    // block that is being read
    for (int i = 0; i < metaDataNumRec; i++)
    {
      Record newRecord;

      // SSN
      file.seekg(readPosition);
      uint32_t SSN;
      file.read((char *)&SSN, sizeof(SSN));

      newRecord.ssn = SSN;

      // cout << "SSN: " << SSN << endl;

      // Name
      readPosition += sizeof(SSN);

      file.seekg(readPosition);
      string name;
      getline(file, name, '\0');

      newRecord.name = name;

      // cout << "Name: " << name << endl;

      // Income
      readPosition += name.size() + 1;

      file.seekg(readPosition);
      uint32_t income;
      file.read((char *)&income, sizeof(income));

      newRecord.income = income;

      // cout << "Income: " << income << endl;

      // State
      readPosition += sizeof(income);

      file.seekg(readPosition);
      char state[2];
      file.read((char *)&state, 2);
      state[2] = '\0';

      newRecord.state = state;

      // cout << "State: " << state << endl;

      page.push_back(newRecord);

      readPosition += 2;
    }

    // Skip empty spaces of block
    readPosition += metaDataRemainingBlockSize;
    file.seekg(readPosition);

    // cout << "Jumped to read position: " << readPosition << endl;

    return page;
  }

  // Close up the file when done
  void close()
  {
    file.close();
  }

  // Deconstructor
  ~TaxRecordReader()
  {
    file.close();
  }
};

#pragma once

#include <fstream>
#include <string.h>
#include "Block.h"
using namespace std;

class TaxRecordWriter
{

private:
  int blockSize;
  fstream file;
  Block *currentBlockWritingAt;
  streampos writePosition;

public:
  // TaxRecordWriter performs the outer most file management
  // If the file is empty, create a new empty block
  // It will ensure the writing of new blocks when needed
  TaxRecordWriter(string fileName, unsigned int blockSize)
  {
    this->blockSize = blockSize;

    writePosition = 0;

    // Opens a file
    file.open(fileName, ios::out | ios::in | ios::binary | ios::trunc);

    // Move the pointer to the start of the file
    file.seekp(0, ios::beg);

    // Create fresh MetaData and a new block with the empty MetaData
    MetaData *headMetaData = new MetaData(blockSize);
    currentBlockWritingAt = new Block(headMetaData);
  }

  // Use this as a way to pass into the block class a Record to store into memory until it is ready to be pushed onto disk.
  void writeRecord(unsigned int ssn, string name, unsigned int income, string state)
  {
    Record recordToWrite(ssn, name, income, state);

    // We need this in a loop because if we don't have an empty space on block then we need to create a new one
    // and then try again on the new block (which should never fail). Once we have succesfully written
    // the old block onto file we can use the new block as the reference and delete old block data from heap
    while (true)
    {
      if (currentBlockWritingAt->AttemptRecordWrite(recordToWrite))
      {
        // AttemptRecordWrite returns true or false and if true the record will be added to the stack
        break;
      }
      else
      {
        // If it returns false, no record is added anywhere until a new block is created.
        cout << "Not enough space, writing a new block..." << endl;

        file.seekp(writePosition);

        currentBlockWritingAt->writeBlockToDisk(file);

        writePosition = file.tellp();

        // Make sure all information was pushed double checking with flushing the file stream
        file.flush();

        // Delete the old block from heap
        delete currentBlockWritingAt;

        // Create new metadata and block information
        MetaData *metaData = new MetaData(blockSize);
        currentBlockWritingAt = new Block(metaData);
      }
    }
  }

  // Close up connections and write/flush everything to file
  void close()
  {
    currentBlockWritingAt->writeBlockToDisk(file);
    file.flush();
    file.close();
    delete currentBlockWritingAt;
  }

  // Deconstructor
  ~TaxRecordWriter()
  {
    file.close();
  }
};

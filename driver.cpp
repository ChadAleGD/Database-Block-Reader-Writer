#include <iostream>
#include <vector>
using namespace std;

#include "TaxRecordWriter.h"
#include "TaxRecordReader.h"
#include "Record.h"

int main()
{

  TaxRecordWriter writer("file.data", 64);
  writer.writeRecord(987654322, "backman", 2012, "CA");
  writer.writeRecord(128312345, "alexander", 2013, "IL");
  writer.writeRecord(463728235, "emanuel", 1899, "IN");

  writer.writeRecord(128312345, "alexander", 2013, "IL");
  writer.writeRecord(463728235, "emanuel", 1899, "IN");
  writer.writeRecord(987654322, "backman", 2012, "CA");

  writer.close();

  TaxRecordReader reader("file.data", 64);
  int numBlocks = 0;
  for (vector<Record> page = reader.readPage(); !page.empty(); page = reader.readPage())
  {
    cout << "[BLOCK]" << endl;
    for (Record record : page)
    {
      cout << record << endl;
    }
    numBlocks++;
  }
  reader.close();

  cout << "[" << numBlocks << " blocks read]" << endl;
  return 0;
}

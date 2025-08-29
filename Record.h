#pragma once

#include <string>
#include <iostream>
#include <cstdint>
using namespace std;

class Record
{
public:
  // Variables that the user will pass into record data
  uint32_t ssn;
  string name;
  uint32_t income;
  string state;

  // Empty constructor is needed when reading data so that I can populate
  // the record info as it is being read from the file itself
  Record()
  {
  }

  // This constructor is used when writing data to the file. I can populate the information right
  // away from the user input.
  Record(unsigned int ssn, string name, unsigned int income, string state)
  {
    this->ssn = ssn;
    this->name = name;
    this->income = income;
    this->state = state;
  }

  // Returns the total size of this current record by counting total byte amount
  int GetSize()
  {
    return sizeof(ssn) + sizeof(income) + name.size() + state.size();
  }

  // This was already here
  friend ostream &operator<<(ostream &os, const Record &r)
  {
    os << r.ssn << "," << r.name << "," << r.income << "," << r.state;
    return os;
  }
};

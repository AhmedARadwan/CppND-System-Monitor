#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <fstream>
#include <sstream>
#include <vector>
#include "linux_parser.h"

class Processor {
 public:
  float Utilization();
  

  // DONE: Declare any necessary private members
 private:
  bool flag = false;
  std::vector<int> prev_values;
  std::vector<int> GetCPUInfo();
  float CalculatePercentage(const std::vector<int>& curr_v, const std::vector<int>& prev_v);
  enum Activity{
    user,
    nice,
    system,
    idle,
    iowait,
    irq,
    softirq,
    steal,
    guest,
    guest_nice
  };
};

#endif
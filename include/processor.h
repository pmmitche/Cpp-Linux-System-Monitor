#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

class Processor {
 public:
  std::vector<float> GetCPUStats();
  float Utilization();

};

#endif
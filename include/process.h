#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int id);
  int Pid();                              
  void UserId();
  std::string User();                     
  std::string Command();                  
  float CpuUtilization();               
  std::string Ram();                     
  long int UpTime();                      
  bool operator<(Process const& a) const; 

 private:
  int pid_;
  int uid_;
  float cpuUtil_;
};

#endif
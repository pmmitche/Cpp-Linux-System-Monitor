#include "processor.h"
#include "linux_parser.h"

using std::cout;
using std::string;
using std::endl;

float Processor::Utilization() {
  float totalDif, idleDif, percentage;
  std::vector<float> prevStats = Processor::GetCPUStats();
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  std::vector<float> curStats = Processor::GetCPUStats();

  totalDif = curStats[1] - prevStats[1];
  idleDif = curStats[0] - curStats[0];
  percentage = (totalDif - idleDif) / totalDif;
  return percentage / 100;
}

std::vector<float> Processor::GetCPUStats() {
  string line, cpu;
  float user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  float totalIdle, nonIdle, total;
  std::ifstream statfile(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  
  if (statfile.is_open()) {
    std::getline(statfile, line);
    std::istringstream cpustream(line);
    cpustream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    
    totalIdle = idle + iowait;
    nonIdle = user + nice + system + irq + softirq + steal;
    total = totalIdle + nonIdle;
    
    std::vector<float> cpustats {totalIdle, total};
    return cpustats;
  }

  return std::vector<float> {};
}
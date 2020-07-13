#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;
using std::cout;
using std::endl;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() {
    processes_.clear();
    vector<int> pids = LinuxParser::Pids();
    for(int pid : pids){
        Process tmp_proc(pid);
        processes_.emplace_back(tmp_proc);
    }
    std::sort(processes_.begin(), processes_.end());\
    return processes_; 
}

std::string System::Kernel() {
    std::ifstream version_file;
    string os, ver, kernel;
    string line;

    version_file.open(LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
    if (version_file.is_open()) {
        std::getline(version_file, line);

        std::stringstream version_stream(line);
        version_stream >> os >> ver >> kernel;
    }

    return kernel;
}

float System::MemoryUtilization() {
  double memtotal = 0;
  double memfree = 0;
  string line;
  string key, kb;
  double value;
  std::ifstream memorystream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  
  if (memorystream.is_open()) {
    while(std::getline(memorystream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      while(linestream >> key >> value >> kb) {
        if (key == "MemFree") {
          memfree = value;
          return (memtotal - memfree) / memtotal;
        }
        memtotal = value;
      }
    }
  }
  return 0.0;
}

std::string System::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(LinuxParser::kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

int System::RunningProcesses() {
  string line;
  string key;
  int value;
  std::ifstream statstream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  
  if (statstream.is_open()) {
    while(std::getline(statstream, line)) {
      std::istringstream linestream(line);

      while(linestream >> key >> value) {
        if (key == "procs_running") {
          return value;
        }
      }
    }
  }
  return 0;
}

int System::TotalProcesses() {
  string line;
  string key;
  int value;
  std::ifstream statstream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
  
  if (statstream.is_open()) {
    while(std::getline(statstream, line)) {
      std::istringstream linestream(line);

      while(linestream >> key >> value) {
        if (key == "processes") {
          return value;
        }
      }
    }
  }
  return 0;
}

long int System::UpTime() {
  string line;
  string key;
  long value;
  std::ifstream timestream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);

  if (timestream.is_open()) {
    std::getline(timestream, line);
    std::replace(line.begin(), line.end(), '.', ' ');
    std::istringstream linestream(line);
    linestream >> value;
    return value;
  }
  return 0;
}
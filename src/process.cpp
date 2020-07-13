#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;
using std::cout;
using std::endl;

Process::Process(int pid) {
  this->pid_ = pid;
  Process::UserId();
  Process::CpuUtilization();
}

int Process::Pid() {
  return this->pid_;
}

float Process::CpuUtilization() {
  string line, key;
  int counter = 1;
  float hz = sysconf(_SC_CLK_TCK);
  float utime, stime, cutime, cstime, starttime, uptime, total_time, seconds, cpu_usage;
  
  uptime = LinuxParser::UpTime();
  string string_pid = std::to_string(this->pid_);
  std::ifstream statstream(LinuxParser::kProcDirectory + string_pid + LinuxParser::kStatFilename);
  
  if (statstream.is_open()) {
    std::getline(statstream, line);
    std::istringstream linestream(line);
    
    while (linestream >> key) {
      if (counter == 14) {
        utime = std::stof(key);
      }
      else if (counter == 15) {
        stime = std::stof(key);
      }
      else if (counter == 16) {
        cutime = std::stof(key);
      }
      else if (counter == 17) {
        cstime = std::stof(key);
      }
      if (counter == 22) {
        starttime = std::stof(key);
      }
      
      counter++;
    }
    
    total_time = utime + stime + cutime + cstime;
    seconds = uptime - (starttime / hz);
    cpu_usage = ((total_time / hz) / seconds);
    this->cpuUtil_ = cpu_usage;
    return cpu_usage;
  }
  
  return 0;
}

string Process::Command() {
  string string_pid = std::to_string(this->pid_);
  string line;
  
  std::ifstream cmdstream(LinuxParser::kProcDirectory + string_pid + LinuxParser::kCmdlineFilename);
  if (cmdstream.is_open()) {
    std::getline(cmdstream, line);
    return line;
  }

  return string();
}

string Process::Ram() {
  string string_pid = std::to_string(this->pid_);
  string key, line;
  int valueKB, valueMB;
  std::ifstream statustream(LinuxParser::kProcDirectory + string_pid + LinuxParser::kStatusFilename);
  
  if (statustream.is_open()) {
    while(std::getline(statustream, line)) {
      std::istringstream linestream(line);

      while(linestream >> key >> valueKB) {
        if (key == "VmSize:") {
          valueMB = valueKB / 1000;
          return std::to_string(valueMB);
        }
      }
    }
  }
  return string();
}


void Process::UserId() {
  string key, line;
  int value;
  string string_pid = std::to_string(this->pid_);
  std::ifstream statustream(LinuxParser::kProcDirectory + string_pid + LinuxParser::kStatusFilename);
  
  if (statustream.is_open()) {
    while(std::getline(statustream, line)) {
      std::istringstream linestream(line);

      while(linestream >> key >> value) {
        if (key == "Uid:") {
          this->uid_ = value;
        }
      }
    }
  }
  this->uid_ = -1;
}

string Process::User() {
  string key, x, line;
  int value;
  std::ifstream passwdstream(LinuxParser::kPasswordPath);
  
  if (passwdstream.is_open()) {
    while(std::getline(passwdstream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);

      while(linestream >> key >> x >> value) {
        if (value == this->uid_) {
          return key;
        }
      }
    }
  }
  return string();
}

long int Process::UpTime() {
  string line, key;
  int counter = 0;
  string string_pid = std::to_string(this->pid_);
  std::ifstream statstream(LinuxParser::kProcDirectory + string_pid + LinuxParser::kStatFilename);
  
  
  if (statstream.is_open()) {
    while(std::getline(statstream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key && counter < 21) {
        counter++;
      }
      if (counter == 21) {
        return std::stol(key) / sysconf(_SC_CLK_TCK);
      }
    }
  }
  return 0;
}

bool Process::operator<(Process const& a) const {
  if (this->cpuUtil_ > a.cpuUtil_) {
    return true;
  }
  
  return false;
}
#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  std::ifstream input_file(kProcDirectory + kMeminfoFilename);
  std::string line, key, value;
  long int mem_free, mem_total;
  if (input_file.is_open()){
    while(std::getline(input_file, line)){
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "MemTotal:"){
        mem_total = std::stoi(value);
      }
      if (key == "MemFree:"){
        mem_free = std::stoi(value);
        return (mem_total-mem_free)/float(mem_total);
      }
    } 
  }
  return 0.0;
}

// DONE: Read and return the system uptime
long int LinuxParser::UpTime() {

  std::ifstream input_file(kProcDirectory + kUptimeFilename);
  std::string line;
  long int value;
  if (input_file.is_open()){
    if (std::getline(input_file, line)){
      std::istringstream istringstream_(line);
      istringstream_ >> value;
      return value;
    }
  }
  return 0;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return sysconf(_SC_CLK_TCK)*UpTime();
}

// TODO: Read and return the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid) {
  std::ifstream input_file(kProcDirectory + std::to_string(pid) + kStatFilename);
  std::string line, value;
  long output_jiffies = 0;
  if (input_file.is_open()){
    while(std::getline(input_file, line)){
      std::istringstream istringstream_(line);
      for (int i = 0 ; i < 17 ; i++){
        istringstream_ >> value;
        if (i >= 13 && i <= 16){
          output_jiffies += atol(value.c_str());
        }
      }
      return output_jiffies;
      
      
    }
  }
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  std::ifstream input_file(kProcDirectory + kStatFilename);
  std::vector<int> values;
  std::string line, key;
  int tmp;
  long active_jiffies = 0;
  if (input_file.is_open()){
      while(std::getline(input_file, line)){
          std::istringstream linestream(line);
          linestream >> key;
          if (key == "cpu"){
              while(linestream >> tmp){
                  active_jiffies += tmp;
              }
              return active_jiffies;
          }
      } 
  }
  return 0;

}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  std::ifstream input_file(kProcDirectory + kStatFilename);
  std::vector<int> values;
  std::string line, key;
  int tmp;
  long idle_jiffies = 0;
  if (input_file.is_open()){
      while(std::getline(input_file, line)){
          std::istringstream linestream(line);
          linestream >> key;
          if (key == "cpu"){
              for (int i = 0 ; i < 5 ; i++){
                linestream >> tmp;
                if (i == 3 || i == 4){
                  idle_jiffies += tmp;
                }
              }
              return idle_jiffies;
          }
      } 
  }
  return 0;

}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { return {}; }

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  std::ifstream input_file(kProcDirectory + kStatFilename);
  std::string line, key;
  int value = 0;

  if (input_file.is_open()){
    while(std::getline(input_file, line)){
      std::istringstream istringstream_(line);
      istringstream_ >> key >> value;
      if (key == "processes"){
        return value;
      }
    }

  }
  return 0;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::ifstream input_file(kProcDirectory + kStatFilename);
  std::string line, key;
  int value;

  if (input_file.is_open()){
    while(std::getline(input_file, line)){
      std::istringstream istringstream_(line);
      istringstream_ >> key >> value;
      if (key == "procs_running"){
        return value;
      }
    }
  }
  return 0;

}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  std::ifstream input_file(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  std::string line;
  if (input_file.is_open()){
    std::getline(input_file, line);
    return line;
  }
  return string();

}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  std::ifstream input_file(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, key;
  int value;
  if (input_file.is_open()){
    while(std::getline(input_file, line)){
      std::istringstream istringstream_(line);
      istringstream_ >> key >> value;
      if (key == "VmSize:"){
        value /= 1024;
        return std::to_string(value);
      }
    }
  }
  return string();

}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  std::ifstream input_file(kProcDirectory + std::to_string(pid) + kStatusFilename);
  std::string line, key, value;
  if (input_file.is_open()){
    while(std::getline(input_file, line)){
      std::istringstream istringstream_(line);
      istringstream_ >> key >> value;
      if (key == "Uid:"){
        return value;
      }
    }
  }
  return string();

}

// DONE: Read and return the user associated with a process
string LinuxParser::User(int pid) {
  std::ifstream input_file(kPasswordPath);
  std::string line, key, value, tmp;
  if (input_file.is_open()){
    while(std::getline(input_file, line)){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream istringstream_(line);
      istringstream_ >> value >> tmp >> key;
      if (key == Uid(pid)){
        return value;
      }
    }
  }
  return string();

}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) { return 0; }

#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() {
    long systemUpTime = LinuxParser::UpTime();
    long processUpTime = LinuxParser::UpTime(pid_);
    long totalTime = LinuxParser::ActiveJiffies(pid_);
    float elapTime = float(systemUpTime) - (float(processUpTime)/sysconf(_SC_CLK_TCK));
    cpu_usage = (float(totalTime)/sysconf(_SC_CLK_TCK))/elapTime;
    return cpu_usage;
}

// DONE: Return the command that generated this process
string Process::Command() { return LinuxParser::Command(pid_); }

// DONE: Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(pid_); }

// DONE: Return the user (name) that generated this process
string Process::User() { return LinuxParser::User(pid_); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 
    return this->cpu_usage < a.cpu_usage;
}
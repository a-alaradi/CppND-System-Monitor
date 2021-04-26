#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"
using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid) : pid_(pid) {}

// DONE: Return this process's ID
int Process::Pid() const { return pid_; }

void Process::CpuUtilization(long activeTicks, long systemTicks) {
  cpu_ = static_cast<float> (activeTicks) / systemTicks;
}

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() const { return cpu_; }

// DONE: Return the command that generated this process
string Process::Command() const { return LinuxParser::Command(Process::Pid()); }

// DONE: Return this process's memory utilization
string Process::Ram() const { return LinuxParser::Ram(Process::Pid()); }

// DONE: Return the user (name) that generated this process
string Process::User() const { return LinuxParser::User(Process::Pid()); }

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() const { return LinuxParser::UpTime(Process::Pid()); }

// DONE: Overload the "less than" comparison operator for Process objects
bool Process::operator>(const Process& a) const {
  return CpuUtilization() > a.CpuUtilization();
}
bool Process::operator<(const Process& a) const {
  return CpuUtilization() < a.CpuUtilization();
}
#ifndef PROCESS_H
#define PROCESS_H

#include <string>
/*
Basic class for Process representation
It contains relevant attributes as shown below
*/
class Process {
 public:
  Process(int pid);
  int Pid() const;               // DONE: See src/process.cpp
  std::string User() const;      // DONE: See src/process.cpp
  std::string Command() const;   // DONE: See src/process.cpp
  float CpuUtilization() const;  // DONE: See src/process.cpp
  void CpuUtilization(long activeTicks, long systemTicks);
  std::string Ram() const;                 // DONE: See src/process.cpp
  long int UpTime() const;                 // DONE: See src/process.cpp
  bool operator<(Process const& a) const;  // DONE: See src/process.cpp
  bool operator>(Process const& a) const;  // DONE: See src/process.cpp

  // DONE: Declare any necessary private members
 private:
  int pid_;
  float cpu_;
};

#endif
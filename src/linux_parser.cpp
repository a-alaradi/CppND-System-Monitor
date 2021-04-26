#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "format.h"
#include <iostream>

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

// Done: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {
  string key, value, line, totalMemory, freeMemory;
  string memoryTotalKey = "MemTotal:";
  string freeMemoryKey = "MemFree:";
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == memoryTotalKey) totalMemory = value;
        if (key == freeMemoryKey) freeMemory = value;
      }
    }
  }
  return (std::stof(totalMemory) - std::stof(freeMemory)) /
         std::stof(totalMemory);
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() {
  long systemUpTime = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    line.erase(line.find(' '));
  }
  std::stringstream buffer(line);
  buffer >> systemUpTime;
  return systemUpTime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  return LinuxParser::IdleJiffies() + LinuxParser::ActiveJiffies();
}

// DONE: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) {
  string line, token;
  vector<string> values;
  long jiffies;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    while (linestream >> token) {
      values.push_back(token);
    }
  }
  if (values.size() > 21) {
    long user = stol(values[13]);
    long kernel = stol(values[14]);
    long children_user = stol(values[15]);
    long children_kernel = stol(values[16]);
    jiffies = user + kernel + children_user + children_kernel;
  }
  return jiffies;
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  vector<string> timeSnapshot = CpuUtilization();
  return (stol(timeSnapshot[CPUStates::kUser_]) + stol(timeSnapshot[CPUStates::kNice_]) +
          stol(timeSnapshot[CPUStates::kSystem_]) + stol(timeSnapshot[CPUStates::kIRQ_]) +
          stol(timeSnapshot[CPUStates::kSoftIRQ_]) + stol(timeSnapshot[CPUStates::kSteal_]));
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> timeSnapshot = CpuUtilization();
  return (stol(timeSnapshot[CPUStates::kIdle_]) + stol(timeSnapshot[CPUStates::kIOwait_]));
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, token;
  string targetToken = "cpu";
  vector<string> values;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while(getline(stream, line)) {
      std::istringstream lineStream(line);
      while(lineStream >> token) {
        if (token == targetToken) {
          while (lineStream >> token) {
            values.push_back(token);
          }
          return values;
        }
      }
    }
  }
  return values;
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() {
  string line;
  int totalProcesses;
  std::string valueName = "processes";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.rfind(valueName, 0) == 0) {
        std::string foundLine = line.erase(0,valueName.length() + 1);
        totalProcesses = std::stoi(foundLine);
      }
    }
  }
  return totalProcesses;
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line;
  int runningProcesses;
  std::string valueName = "procs_running";
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      if(line.rfind(valueName, 0) == 0) {
        std::string foundLine = line.erase(0,valueName.length() + 1);
        runningProcesses = std::stoi(foundLine);
      }
    }
  }
  return runningProcesses;
}

// DONE: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) {
  string line = "";
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  return line;
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
    string key, value, line, usedMemory;
    string memoryUsedKey = "VmSize:";
    std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
      while (std::getline(filestream, line)) {
        std::istringstream linestream(line);
        while (linestream >> key >> value) {
          if (key == memoryUsedKey) usedMemory = value;
        }
      }
    }
    return to_string((std::stoi(usedMemory) / 1000));
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string token;
  string uid = "";
  string uidValue = "Uid:";
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    while(stream >> token) {
      if (token == uidValue) {
        uid = token;
      }
    }
  }
  return uid;
}

// DONE: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid) {
  //get uid
  string uid = Uid(pid);
  string username,line;
  string processOwner = "";
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while(std::getline(stream,line)) {
      std::replace(line.begin(),line.end(), ':',' ');
      std::replace(line.begin(),line.end(), 'x',' ');
      std::istringstream linestream(line);
      while(linestream >> username >> uid) {
        if (uid == username) {
          processOwner = username;
        }
      }
    }
  }
  return processOwner;
}

// DONE: Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string line, token;
  vector<string> values;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
      getline(stream, line);
      std::istringstream lineStream(line);
      while(lineStream >> token) {
            values.push_back(token);
        }
    }
  return stol(values[21])/sysconf(_SC_CLK_TCK);
  }

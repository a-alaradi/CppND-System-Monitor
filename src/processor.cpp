#include "processor.h"
#include "linux_parser.h"

// DONE: Return the aggregate CPU utilization
// https://stackoverflow.com/a/23376195
float Processor::Utilization() {
  long idleTicks = LinuxParser::IdleJiffies();
  long totalTicks = LinuxParser::Jiffies();
  return (static_cast<float>(totalTicks) - idleTicks) / totalTicks;
}
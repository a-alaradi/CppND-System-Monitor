#include <string>

#include "format.h"

using std::string;

// DONE: Complete this helper function
//https://stackoverflow.com/a/64669321
string Format::ElapsedTime(long seconds) {
  std::string result;
  std::string h = std::to_string(seconds / 3600);
  std::string m = std::to_string((seconds % 3600) / 60);
  std::string s = std::to_string(seconds % 60);
  std::string hh = std::string(2 - h.length(), '0') + h;
  std::string mm = std::string(2 - m.length(), '0') + m;
  std::string ss = std::string(2 - s.length(), '0') + s;
  if (hh.compare("00") != 0) {
    result = hh + ':' + mm + ":" + ss;
  }
  else {
    result =  mm + ":" + ss;
  }
  return result;
}
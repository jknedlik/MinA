#include "MinA/common/Minimizer.h"
#include <cstdio>
#include <fstream>
#include <iostream>
#include <map>
#include <utility>

int main(void)
{
 Log::getLog().setLogOutput("testlog.txt");
 Log::getLog() << "test123";
 Log::getLog().flushLog();
 std::ifstream myfile;
 myfile.open("testlog.txt");
 std::string line;
 std::getline(myfile, line);
 myfile.close();
 // std::remove("testlog.txt");
 return !(line.compare("test123") == 0);
}

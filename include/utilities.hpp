#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

void changeToLower(std::string &str);
std::string timenow();
std::vector<std::string> splitMsg(std::string &msg,
                                  const std::string &delimiter = " ");

#endif

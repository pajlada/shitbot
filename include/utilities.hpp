#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <locale>
#include <ctime>
#include <iomanip>

void changeToLower(std::string& str);
std::string timenow();
std::vector<std::string> splitMsg(std::string &msg, const std::string &delimiter = " ");

#endif
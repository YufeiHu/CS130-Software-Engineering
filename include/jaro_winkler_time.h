#ifndef JAROWINKLERTIME
#define JAROWINKLERTIME

#include <string>
#include <cstring>
#include <vector>
#include <tuple>
#include <queue>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "search_algo.h"

class JaroWinklerTime : public SearchAlgo {
 private:
  float JaroWinklerDistance(const char *s, const char *t);
 public:
  std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> GetRank(
    std::string text_content,std::vector<std::tuple<int, std::string, std::string, std::string, std::string>>& meme_source);
};

#endif

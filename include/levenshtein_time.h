#ifndef LEVENSHTEINTIME
#define LEVENSHTEINTIME

#include <string>
#include <vector>
#include <cstring>
#include <tuple>
#include <queue>
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include "search_algo.h"

class LevenshteinTime : public SearchAlgo {
 private:
  int LevenshteinDistance(const char *s, const char *t);
 public:
  std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> GetRank(
    std::string text_content,std::vector<std::tuple<int, std::string, std::string, std::string, std::string>>& meme_source);
};

#endif

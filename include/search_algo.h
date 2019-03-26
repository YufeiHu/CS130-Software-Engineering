#ifndef SEARCHALGO
#define SEARCHALGO

#include <string>
#include <vector>
#include <tuple>
#include <algorithm>
#include <iostream>
#include <unordered_map>

class SearchAlgo {
 public:
  static const int NUM_OF_RESULTS;
  static const int LEAST_SEARCH_TEXT_LEN;
  static const float LEAST_MATCH_RATE;
  static const float LEAST_TOKEN_COUNTED_RATE;
  static const char* StringClean(std::string s);
  static std::vector<std::string> StringSplit(std::string s, std::string delimiter);
  virtual ~SearchAlgo() = default;
  virtual std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> GetRank(
    std::string text_content,std::vector<std::tuple<int, std::string, std::string, std::string, std::string>>& meme_source) = 0;

 protected:
  float CharMatchRate(const char*s, const char *t);
};

#endif

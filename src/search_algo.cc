#include <string>
#include <ctype.h>
#include <cstring>
#include "search_algo.h"

const int SearchAlgo::LEAST_SEARCH_TEXT_LEN = 3;
const float SearchAlgo::LEAST_MATCH_RATE = 2.0;
const float SearchAlgo::LEAST_TOKEN_COUNTED_RATE = 0.4;
const int SearchAlgo::NUM_OF_RESULTS = 3;

const char* SearchAlgo::StringClean(std::string s) {
  char* str = new char[s.length() + 1];
  std::strcpy(str, s.c_str());
  char* out = str, *put = str;
  bool first_char = false;

  for(; *str != '\0'; ++str) {
    if(isalpha(*str) || isdigit(*str) || (first_char && *str == ' ' && *(put - 1) != ' ')) {
      *put++ = tolower(*str);
      first_char = true;
     }
  }

  if (not first_char) {
    return "";
  }
  else if(*(put - 1) != ' ') {
    *put = '\0';
  } 
  else {
    *(put - 1) = '\0';
  }
  return out; 
}

std::vector<std::string> SearchAlgo::StringSplit(std::string s, std::string delimiter) {
  size_t pos = 0;
  std::string token;
  std::vector<std::string> res;
  while ((pos = s.find(delimiter)) != std::string::npos) {
      token = s.substr(0, pos);
      res.push_back(token);
      s.erase(0, pos + delimiter.length());
  }
  if (s.length() > 0) {
    res.push_back(s);
  }
  return res;
}

float SearchAlgo::CharMatchRate(const char *s1, const char *s2) {
  unsigned int max_match = 0;
  unsigned int s1len = std::strlen(s1);
  unsigned int s2len = std::strlen(s2);
  unsigned int sum = s1len;
  int perfect_match = 0;

  std::unordered_map<char, int> c_cnt;
  for (unsigned int i = 0; i < s1len; ++i) {
    if (c_cnt.find(s1[i]) != c_cnt.end()) {
      c_cnt[s1[i]] = c_cnt[s1[i]] + 1;
    } else {
      c_cnt[s1[i]] = 1;
    }
  }
  for (unsigned int i = 0; i < s2len; ++i) {
    if (i >= s1len && (c_cnt.find(s2[i - s1len]) != c_cnt.end())) {
      c_cnt[s2[i - s1len]] = c_cnt[s2[i - s1len]] + 1;
      if (c_cnt[s2[i - s1len]] > 0) {
        sum = sum + 1;
      }
    }
    if (c_cnt.find(s2[i]) != c_cnt.end()) {
      c_cnt[s2[i]] = c_cnt[s2[i]] - 1;
      if (c_cnt[s2[i]] >= 0) {
        sum -= 1;
      }
    }

    if (sum == 0) {
      ++perfect_match;
      // return 0;
    }

    max_match = std::max(max_match, s1len - sum);
  }
  const float DENOMINATOR = 0.8;
  float parameter = (float)s1len;

  if (perfect_match > 0) {
    parameter = (float)s1len / ((float)perfect_match * 2.0);
  }

  return parameter / std::max((float)max_match, DENOMINATOR);
}

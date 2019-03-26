#ifndef SEARCHFACTORY
#define SEARCHFACTORY

#include <string>
#include <iostream>

#include "levenshtein_time.h"
#include "jaro_winkler_time.h"

class SearchAlgoFactory {
 private:
  static const std::string DEFAULT_ALGO;
  
 public:
  static SearchAlgo* GetAlgo(std::string algo);
  static SearchAlgo* GetAlgo();
};

#endif

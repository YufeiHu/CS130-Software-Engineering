#include <stdexcept>
#include "search_algo_factory.h"

const std::string SearchAlgoFactory::DEFAULT_ALGO = "Levenshtein";

SearchAlgo* SearchAlgoFactory::GetAlgo(std::string algo) {
  SearchAlgo* search_algo(NULL);
  if (algo.compare("Levenshtein") == 0) {
    search_algo = new LevenshteinTime;
  }
  else if (algo.compare("Jaro Winkler") == 0) {
  	search_algo = new JaroWinklerTime;
  } else {
    throw std::invalid_argument("Unknown algorithm!"); 
  }
  return search_algo;
}
  
SearchAlgo* SearchAlgoFactory::GetAlgo() {
  return SearchAlgoFactory::GetAlgo(SearchAlgoFactory::DEFAULT_ALGO);
}

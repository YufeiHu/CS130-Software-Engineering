#include "levenshtein_time.h"

class myComparator {
 public:
  bool operator() (const std::tuple<float, std::tuple<int, std::string, std::string, std::string, std::string>>& tuple1,
                   const std::tuple<float, std::tuple<int, std::string, std::string, std::string, std::string>>& tuple2) {
    if (std::get<0>(tuple1) != std::get<0>(tuple2)) {
      return std::get<0>(tuple1)<std::get<0>(tuple2);
    } else {
      int id1 = std::get<0>(std::get<1>(tuple1));
      int id2 = std::get<0>(std::get<1>(tuple2));
      return id1 > id2;
    }
  }
};

int LevenshteinTime::LevenshteinDistance(const char *s1, const char *s2) {
  unsigned int s1len, s2len, x, y, lastdiag, olddiag;
  s1len = std::strlen(s1);
  s2len = std::strlen(s2);
  unsigned int column[s1len+1];
  for (y = 1; y <= s1len; y++)
    column[y] = y;
  for (x = 1; x <= s2len; x++) {
    column[0] = x;
    for (y = 1, lastdiag = x-1; y <= s1len; y++) {
      olddiag = column[y];
      column[y] = std::min({column[y] + 1, column[y-1] + 1, lastdiag + (s1[y-1] == s2[x-1] ? 0 : 1)});
      lastdiag = olddiag;
    }
  }
  return(column[s1len]);
}

std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> LevenshteinTime::GetRank(
            std::string text_content, std::vector<std::tuple<int, std::string, std::string, std::string, std::string>>& meme_source) {
  
  std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> res;
  std::vector<std::tuple<int, std::string, std::string, std::string, std::string>>::iterator it;
  std::priority_queue<std::tuple<float, std::tuple<int, std::string, std::string, std::string, std::string>>,
                      std::vector<std::tuple<float, std::tuple<int, std::string, std::string, std::string, std::string>>>,
                      myComparator> rank_res;
    
  // source string clean
  // for Levenstein distance use
  std::string text_content_whole = text_content;
  text_content_whole.erase(remove_if(text_content_whole.begin(), text_content_whole.end(), isspace), text_content_whole.end());
  const char* text_content_clean = SearchAlgo::StringClean(text_content_whole);
  
  // for char matching use
  const char* s = SearchAlgo::StringClean(text_content);
  unsigned int s_len = strlen(s);
  std::vector<std::string> tokens = SearchAlgo::StringSplit(s, " ");


  for(it = meme_source.begin(); it != meme_source.end(); it++) {
    float distance = 0;
    bool has_char_match = false;

    int meme_id = std::get<0>(*it);
    std::string top_text = std::get<1>(*it);
    std::string bottom_text = std::get<2>(*it);
    std::string image_name = std::get<3>(*it);
    std::string image_url = std::get<4>(*it);

    // target string clean
    std::string target = top_text + bottom_text + image_name;
    target.erase(remove_if(target.begin(), target.end(), isspace), target.end());
    const char* t = SearchAlgo::StringClean(target);
    unsigned int t_len = strlen(t);

    if (t_len == 0 || s_len == 0) {
      throw std::runtime_error(std::string("Search content cannot be empty!"));
    }

    std::vector<std::string>::iterator tokens_it;
    int num_of_counted_tokens = 0;
    for (tokens_it = tokens.begin(); tokens_it != tokens.end(); tokens_it++) {
      int token_len = tokens_it->length();
      if (token_len <= t_len && token_len >= SearchAlgo::LEAST_SEARCH_TEXT_LEN) {
          const char* token = tokens_it->c_str();
        distance += SearchAlgo::CharMatchRate(token, t);
        ++num_of_counted_tokens;
        has_char_match = true;
      }
    }

    float token_counted_rate = (float)num_of_counted_tokens / (float)tokens.size();
    if (token_counted_rate != 0) {
      distance /= (float)num_of_counted_tokens;
    }

    if (!has_char_match || token_counted_rate < SearchAlgo::LEAST_TOKEN_COUNTED_RATE || distance > SearchAlgo::LEAST_MATCH_RATE) {
      distance = (float)LevenshteinDistance(text_content_clean, t);
    }
  
    std::tuple<float, std::tuple<int, std::string, std::string, std::string, std::string>> dis_info = std::make_tuple(distance, *it);
    
    // online algo - stream processing
    if (rank_res.size() < SearchAlgo::NUM_OF_RESULTS) {
      rank_res.push(dis_info);
    } else if (dis_info < rank_res.top()) {
      rank_res.push(dis_info);
      rank_res.pop();
    }
  }

  while (rank_res.empty() == false) {
    res.push_back(std::get<1>(rank_res.top()));
    rank_res.pop();
  }
  std::reverse(res.begin(), res.end());
  return res;
}

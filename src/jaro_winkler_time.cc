#include "jaro_winkler_time.h"

#define SCALING_FACTOR 0.1

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

/* Copyright (C) 2011 Miguel Serrano
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.*/

float JaroWinklerTime::JaroWinklerDistance(const char *s, const char *a) {
  int i, j, l;
  int m = 0, t = 0;
  int sl = std::strlen(s);
  int al = std::strlen(a);
  int sflags[sl], aflags[al];
  int range = std::max(0, std::max(sl, al) / 2 - 1);
  float dw;

  if (!sl || !al)
    return 0.0;
  for (i = 0; i < al; i++)
    aflags[i] = 0;
  for (i = 0; i < sl; i++)
    sflags[i] = 0;

  /* calculate matching characters */
  for (i = 0; i < al; i++) {
    for (j = std::max(i - range, 0), l = std::min(i + range + 1, sl); j < l; j++) {
      if (a[i] == s[j] && !sflags[j]) {
        sflags[j] = 1;
        aflags[i] = 1;
        m++;
        break;
      }
    }
  }
  if (!m)
    return 0.0;

  /* calculate character transpositions */
  l = 0;
  for (i = 0; i < al; i++) {
    if (aflags[i] == 1) {
      for (j = l; j < sl; j++) {
        if (sflags[j] == 1) {
          l = j + 1;
          break;
        }
      }
      if (a[i] != s[j])
        t++;
    }
  }
  t /= 2;

  /* Jaro distance */
  dw = (((float)m / sl) + ((float)m / al) + ((float)(m - t) / m)) / 3.0;

  /* calculate common string prefix up to 4 chars */
  l = 0;
  for (i = 0; i < std::min(std::min(sl, al), 4); i++)
    if (s[i] == a[i])
      l++;

  /* Jaro-Winkler distance */
  dw = dw + (l * SCALING_FACTOR * (1 - dw));
  return dw;
}

std::vector<std::tuple<int, std::string, std::string, std::string, std::string>> JaroWinklerTime::GetRank(std::string text_content, std::vector<std::tuple<int, std::string, std::string, std::string, std::string>>& meme_source) {
  
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
  unsigned int s_len = std::strlen(s);
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
    unsigned int t_len = std::strlen(t);

    if (t_len == 0 || s_len == 0) {
      throw std::runtime_error(std::string("Search string cannot be empty!"));
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
    if (distance <= SearchAlgo::LEAST_MATCH_RATE) {
      distance *= SCALING_FACTOR;
    }
    if (!has_char_match || token_counted_rate < SearchAlgo::LEAST_TOKEN_COUNTED_RATE || distance > SearchAlgo::LEAST_MATCH_RATE) {
      distance = JaroWinklerDistance(text_content_clean, t);
    }

    std::tuple<float, std::tuple<int, std::string, std::string, std::string, std::string>> dis_info = std::make_tuple(distance, *it);
    
    // online algo - stream processing
    if (rank_res.size() < SearchAlgo::NUM_OF_RESULTS) {
      rank_res.push(dis_info);
    }
    else if (dis_info < rank_res.top()) {
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

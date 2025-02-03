#include <cinttypes>
#include <curl/curl.h>
#include <string>
#include <vector>

inline std::vector<std::string> split(std::string s, std::string delimiter) {
  size_t pos_start = 0, pos_end, delim_len = delimiter.length();
  std::string token;
  std::vector<std::string> res;

  while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
    token = s.substr(pos_start, pos_end - pos_start);
    pos_start = pos_end + delim_len;
    res.push_back(token);
  }

  res.push_back(s.substr(pos_start));
  return res;
}

inline uint32_t hash_fnv(const std::string &s) {
  const uint32_t prime = 0x01000193;
  uint32_t hash = 0x811c9dc5;
  for (char c : s) {
    hash ^= c;
    hash *= prime;
  }
  return hash;
}

inline std::string url_encode(const std::string &decoded) {
  const auto encoded_value = curl_easy_escape(
      nullptr, decoded.c_str(), static_cast<int>(decoded.length()));
  std::string result(encoded_value);
  curl_free(encoded_value);
  return result;
}

template <typename T,
          std::enable_if_t<std::is_same<T, std::string_view>::value ||
                               !std::is_rvalue_reference_v<T &&>,
                           int> = 0>
std::string_view trim_left(T &&data, std::string_view trimChars) {
  std::string_view sv{std::forward<T>(data)};
  sv.remove_prefix(std::min(sv.find_first_not_of(trimChars), sv.size()));
  return sv;
}

static constexpr char *ws = (char *)" \t\n\r\f\v";

// trim from end of string (right)
inline std::string &rtrim(std::string &s, const char *t = ws) {
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

// trim from beginning of string (left)
inline std::string &ltrim(std::string &s, const char *t = ws) {
  s.erase(0, s.find_first_not_of(t));
  return s;
}

// trim from both ends of string (right then left)
inline std::string &trim(std::string &s, const char *t = ws) {
  return ltrim(rtrim(s, t), t);
}

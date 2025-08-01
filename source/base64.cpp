
#include <vector>

#include "base64.hpp"

namespace base64
{

  std::string encode(const std::string& in){

    std::string out;
    std::size_t val {0}; 
    int valb {-6};

    for (const char& c : in) {
      val = (val << 8) + c;
      valb += 8;
      while (valb >= 0) {
        out.push_back(alphabet[(val >> valb) & 0x3F]);
        valb -= 6;
      }
    }
    if (valb > -6) {
      out.push_back(alphabet[((val << 8) >> (valb + 8)) & 0x3F]);
    }
    while (out.size() % 4) {
      out.push_back('=');
    }
    return out;
  }

  std::string decode(const std::string& in) {

    std::string out;

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) {
      T[(std::size_t)alphabet[(std::size_t)i]] = i;
    }

    int val = 0, valb = -8;
    for (const char c : in) {
      if (T[(std::size_t)c] == -1) {
        break;
      }
      val = (val << 6) + T[(std::size_t)c];
      valb += 6;
      if (valb >= 0) {
        out.push_back(char((val >> valb) & 0xFF));
        valb -= 8;
      }
    }
    return out;
  }

}
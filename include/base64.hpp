// FROM
// https://stackoverflow.com/a/34571089/5155484

#ifndef _BASE64_H_
#define _BASE64_H_

#include <string>

namespace base64
{

static const std::string alphabet =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string encode(const std::string& in);
std::string decode(const std::string& in);

}

#endif  // _BASE64_H_

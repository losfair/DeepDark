#pragma once

#include <vector>
#include <string>

namespace deepdark {

namespace string_utils {

std::vector<std::string> split(const std::string& s, const std::string& delim) {
    unsigned long p = 0, cp = 0;
    std::vector<std::string> ret;

    while((cp = s.find(delim, p)) != std::string::npos) {
        ret.push_back(s.substr(p, cp - p));
        p = cp + delim.size();
    }

    ret.push_back(s.substr(p));
    return ret;
}

std::string join(const std::vector<std::string> parts, const std::string& delim) {
    std::string ret;
    bool is_first = true;

    for(auto& p : parts) {
        if(!is_first) {
            ret += delim;
        }
        is_first = false;
        ret += p;
    }

    return ret;
}

std::string trim(const std::string& s, const char c) {
    if(s.size() == 0) {
        return std::string("");
    }

    std::string ret;
    unsigned long len = s.size();
    unsigned long start = 0, end = len - 1;

    while(start < len && s[start] == c) {
        start++;
    }

    while(s[end] == c) {
        if(end == 0) break;
        end--;
    }

    if(end < start) {
        return std::string("");
    }

    return s.substr(start, end - start + 1);
}

} // namespace string_utils

} // namespace deepdark

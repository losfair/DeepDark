#pragma once

#include <vector>
#include <string>
#include <set>

namespace deepdark {

namespace string_utils {

std::vector<std::string> split(const std::string& s, const std::string& delim, unsigned long n = 0) {
    unsigned long p = 0, cp = 0;
    std::vector<std::string> ret;

    while((cp = s.find(delim, p)) != std::string::npos) {
        if(n && ret.size() == n - 1) {
            break;
        }
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

std::string trim(const std::string& s, const std::vector<char>& _chs) {
    if(s.size() == 0) {
        return std::string("");
    }

    std::set<char> chs;
    for(char c : _chs) {
        chs.insert(c);
    }

    std::string ret;
    unsigned long len = s.size();
    unsigned long start = 0, end = len - 1;

    while(start < len && chs.find(s[start]) != chs.end()) {
        start++;
    }

    while(chs.find(s[end]) != chs.end()) {
        if(end == 0) break;
        end--;
    }

    if(end < start) {
        return std::string("");
    }

    return s.substr(start, end - start + 1);
}

std::string trim(const std::string& s, const char c) {
    std::vector<char> chs;
    chs.push_back(c);
    return trim(s, chs);
}

std::string trim(const std::string& s) {
    std::vector<char> chs;
    chs.push_back(' ');
    chs.push_back('\n');
    chs.push_back('\t');
    chs.push_back('\r');
    return trim(s, chs);
}

template<typename T> std::string append(const std::string& s, const T& other) {
    return s + std::to_string(other);
}

} // namespace string_utils

} // namespace deepdark

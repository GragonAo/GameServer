#pragma once

#include "common.h"
#include <cctype>
#include <cstdarg>
#include <cstddef>
#include <cstdio>
#include <string>
#include <vector>

namespace strutil {

// 用于比较两个字符串（不区分大小写）
inline int stricmp(char const *c1, char const *c2) {
  return engine_stricmp(c1, c2);
}

// 格式化字符串，类似于sprintf，但使用std::string
inline std::string format(const char *_format, ...) {
  std::string str;
  va_list args1;
  va_start(args1, _format);
  va_list args2;
  va_copy(args2, args1);
  
  // 计算格式化后的字符串需要的字符数
  const size_t num_of_chars = std::vsnprintf(nullptr, 0, _format, args1);
  
  // 如果需要的字符数大于当前字符串的容量，则调整容量
  if (num_of_chars > str.capacity()) {
    str.resize(num_of_chars + 1);
  }
  
  // 使用va_list格式化字符串
  std::vsnprintf(const_cast<char *>(str.data()), str.capacity(), _format, args2);
  
  va_end(args1);
  va_end(args2);

  return str;
}

// 去除字符串首尾的空白字符
inline std::string trim(const std::string &s) {
  // 从字符串开头开始去除空白字符
  std::string::const_iterator it = s.begin();
  while (it != s.end() && std::isspace(*it))
    ++it;
    
  // 从字符串末尾开始去除空白字符
  std::string::const_reverse_iterator rit = s.rbegin();
  while (rit.base() != it && isspace(*rit))
    ++rit;
    
  // 返回去除空白后的字符串
  return std::string(it, rit.base());
}

// 替换字符串中所有匹配的子字符串
inline int replace(std::string &str, const std::string &pattern,
                   const std::string newpat) {
  int count = 0;
  const size_t nsize = newpat.size();
  const size_t psize = pattern.size();
  
  // 从字符串中找到匹配的子字符串，并替换它们
  for (size_t pos = str.find(pattern, 0); pos != std::string::npos;
       pos = str.find(pattern, pos + nsize)) {
    str.replace(pos, psize, newpat);
    count++;
  }
  return count;
}

// 按分隔符分割字符串
template <typename T>
inline void split(const std::basic_string<T> &s, T c,
                  std::vector<std::basic_string<T>> &v) {
  if (s.size() == 0)
    return;
  
  typename std::basic_string<T>::size_type i = 0;
  typename std::basic_string<T>::size_type j = s.find(c);
  
  // 在找到的每个分隔符位置拆分字符串
  while (j != std::basic_string<T>::npos) {
    std::basic_string<T> buf = s.substr(i, j - i);
    if (buf.size() > 0)
      v.push_back(buf);
    i = ++j;
    j = s.find(c, j);
  }
  
  // 处理最后一个分隔符之后的部分
  if (j == std::basic_string<T>::npos) {
    std::basic_string<T> buf = s.substr(i, s.length() - i);
    if (buf.size() > 0)
      v.push_back(buf);
  }
}
} // namespace strutil

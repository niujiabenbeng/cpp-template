#ifndef CPP_TEMPLATE_COMMON_H_
#define CPP_TEMPLATE_COMMON_H_

// yet another placeholder
#define YAPLACEHOLDER

// convert macro to string
#define STRINGIFY(m) #m
#define AS_STRING(m) STRINGIFY(m)

#ifndef DISABLE_COPY_ASIGN
#define DISABLE_COPY_ASIGN(classname)   \
  classname(const classname&) = delete; \
  classname& operator=(const classname&) = delete
#endif  // DISABLE_COPY_ASIGN

#ifndef DISABLE_MOVE_ASIGN
#define DISABLE_MOVE_ASIGN(classname) \
  classname(classname&&) = delete;    \
  classname& operator=(classname&&) = delete
#endif  // DISABLE_MOVE_ASIGN

#ifndef DEFAULT_COPY_ASIGN
#define DEFAULT_COPY_ASIGN(classname)    \
  classname(const classname&) = default; \
  classname& operator=(const classname&) = default
#endif  // DEFAULT_COPY_ASIGN

#ifndef DEFAULT_MOVE_ASIGN
#define DEFAULT_MOVE_ASIGN(classname) \
  classname(classname&&) = default;   \
  classname& operator=(classname&&) = default
#endif  // DEFAULT_MOVE_ASIGN

#ifndef PLAIN_OLD_DATA_CLASS
#define PLAIN_OLD_DATA_CLASS(classname) \
  classname() = default;                \
  DEFAULT_COPY_ASIGN(classname);        \
  DEFAULT_MOVE_ASIGN(classname);        \
  ~classname() = default;
#endif  // PLAIN_OLD_DATA_CLASS

// this macro should be used under public:
// clang-format off
#ifndef SINGLETON_CLASS
#define SINGLETON_CLASS(classname) \
  ~classname() = default;          \
 private:                          \
  classname() = default;           \
  DISABLE_COPY_ASIGN(classname);   \
  DISABLE_MOVE_ASIGN(classname);   \
 public:                           \
  static_assert(true, "")
#endif  // SINGLETON_CLASS
// clang-format on

#ifndef ATOMIC_GET
#define ATOMIC_GET(mutex_, value)           \
  std::lock_guard<std::mutex> lock(mutex_); \
  return value
#endif  // ATOMIC_GET

#ifndef ATOMIC_SET
#define ATOMIC_SET(mutex_, param, value)      \
  do {                                        \
    std::lock_guard<std::mutex> lock(mutex_); \
    param = value;                            \
  } while (0)
#endif  // ATOMIC_SET

#ifndef ATOMIC_RUN
#define ATOMIC_RUN(mutex_, command)           \
  do {                                        \
    std::lock_guard<std::mutex> lock(mutex_); \
    command;                                  \
  } while (0)
#endif  // ATOMIC_RUN

// clang-format off
#include <map>
#include <set>
#include <regex>
#include <mutex>
#include <queue>
#include <cctype>
#include <chrono>
#include <future>
#include <memory>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <iomanip>
#include <streambuf>
#include <functional>
#include <condition_variable>

#include <glog/logging.h>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <jsoncpp/json/json.h>
// clang-format on

#endif  // CPP_TEMPLATE_COMMON_H_

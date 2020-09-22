#ifndef CPP_TEMPLATE_COMMON_H_
#define CPP_TEMPLATE_COMMON_H_

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

#ifndef ATOMIC_GET
#define ATOMIC_GET(mutex_, value)           \
  std::lock_guard<std::mutex> lock(mutex_); \
  return value
#endif  // ATOMIC_GET

// clang-format off
#include <map>
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
#include <streambuf>
#include <functional>
#include <condition_variable>

#include <glog/logging.h>
#include <boost/format.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <jsoncpp/json/json.h>

#endif  // CPP_TEMPLATE_COMMON_H_

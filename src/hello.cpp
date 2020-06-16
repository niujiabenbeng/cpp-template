#include "hello.h"

#include <boost/format.hpp>

std::string Hello::Greeting(const std::string& name) const {
  boost::format pattern("Hello %s");
  return (pattern % name).str();
}

#include <glog/logging.h>

#include "hello.h"

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::LogToStderr();
  LOG(INFO) << Hello().Greeting("World");
  return 0;
}

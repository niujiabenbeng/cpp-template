#include "hello.h"

#include <glog/logging.h>

int main(int argc, char *argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::LogToStderr();
  LOG(INFO) << Hello().Greeting("World");
  return 0;
}

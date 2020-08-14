#include <glog/logging.h>
#include <gtest/gtest.h>

#include "common.h"
#include "timer.h"
#include "util.h"

TEST(FileIOTest, fileio) {
  auto tempfile = boost::filesystem::unique_path().string();
  std::vector<std::string> lines = {"hello", "world"};
  EXPECT_TRUE(WriteFile(tempfile, lines));
  EXPECT_STREQ(ReadFile(tempfile).c_str(), "hello\nworld\n");
  if (boost::filesystem::exists(tempfile)) {
    boost::filesystem::remove(tempfile);
  }
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::LogToStderr();

  //// 这两个函数不适合用gtest, 所以直接输出结果
  LOG(INFO) << "Current Time: " << GetCurrentTimeMs();
  LOG(INFO) << "which gcc: " << ExecShell("which gcc");
  std::vector<float> float_vec{0.01, 0.1, 1.0, 10.0, 100.0};
  LOG(INFO) << "some numbers: " << ToString(float_vec, "%.4f");

  Timer timer;
  timer.Start();
  std::this_thread::sleep_for(std::chrono::seconds(1));
  LOG(INFO) << "Sleeping 1s uses: " << timer.Seconds() << "s";

  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

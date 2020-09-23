#include <glog/logging.h>
#include <gtest/gtest.h>

#include "common.h"
#include "thread_pool.h"
#include "timer.h"
#include "util.h"

TEST(FileIOTest, fileio) {
  auto tempfile = boost::filesystem::unique_path().string();
  std::vector<std::string> lines = {"hello", "world"};
  EXPECT_TRUE(WriteFile(tempfile, lines));
  EXPECT_EQ(ReadFile(tempfile), "hello\nworld\n");
  if (boost::filesystem::exists(tempfile)) {
    boost::filesystem::remove(tempfile);
  }
}

TEST(ThreadPoolTest, pool) {
  ThreadPool pool(4);
  auto result = pool.enqueue([](int answer) { return answer; }, 42);
  EXPECT_EQ(result.get(), 42);
}

TEST(JsonTest, json) {
  Json::Value root;
  root["one"] = 1;
  root["hello"] = "world";
  auto tempfile = boost::filesystem::unique_path().string();
  WriteJsonFile(root, tempfile);
  auto content = ReadJsonFile(tempfile);
  EXPECT_EQ(content.get("one", 0).asInt(), 1);
  EXPECT_EQ(content.get("hello", "").asString(), "world");
  if (boost::filesystem::exists(tempfile)) {
    boost::filesystem::remove(tempfile);
  }
}

int main(int argc, char* argv[]) {
  google::InitGoogleLogging(argv[0]);
  google::LogToStderr();

  // 这两个函数不适合用gtest, 所以直接输出结果
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

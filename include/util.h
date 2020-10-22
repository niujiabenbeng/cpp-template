#ifndef CPP_TEMPLATE_UTIL_H_
#define CPP_TEMPLATE_UTIL_H_

#include "common.h"

// 要用long保存time stamp, 需要64位的数据
static_assert(sizeof(long) == 8, "Type 'long' should have 8 bytes");

// 获取当前时间，以毫秒计时
long GetCurrentTimeMs();

// 格式化timestamp, format可为: "%Y-%m-%d %H:%M:%S"
std::string GetDateTimeString(long timestamp_ms, const std::string& format);

// 如果需要的话, 生成文件所在的目录
void MakeDirsForFile(const std::string& path);

// 一次性读取文件所有的内容，如打开失败返回空字符串
std::string ReadFile(const std::string& file, bool is_binary = false);

// 一次性写入文件的所有内容
bool WriteFile(const std::string& file, const char* data, const int len);
bool WriteFile(const std::string& file, const std::string& content);
bool WriteFile(const std::string& file, const std::vector<std::string>& lines);

// 解析json字符串，如失败则返回空的Json::Value
Json::Value ParseJsonString(const std::string& content);

// 将json数据转化成字符串
std::string DumpJsonValue(const Json::Value& content);

// 读取json文件并解析，如失败则返回空的Json::Value
Json::Value ReadJsonFile(const std::string& json_file);

// 写json结构到文件，必要的时候生成必须的目录
void WriteJsonFile(const Json::Value& root, const std::string& json_file);

// 运行shell命令, 出错返回空字符串
std::string ExecShell(const std::string& cmd);

// vector to string, format控制单个value的格式
template <class Dtype>
std::string ToString(const std::vector<Dtype>& values,
                     const std::string& format) {
  std::vector<std::string> string_values;
  for (size_t i = 0; i < values.size(); ++i) {
    string_values.push_back((boost::format(format) % values[i]).str());
  }
  std::string result = boost::algorithm::join(string_values, ", ");
  return std::string("[") + result + std::string("]");
}

// 返回目录中所有的文件和子目录, 结果做升序排列
std::vector<std::string> ListDirectory(
    const std::string& dirname, const std::regex& pattern = std::regex(".*"));

// 计算字符串的md5值
std::string CalcMD5(const std::string& content);

#endif  // CPP_TEMPLATE_UTIL_H_

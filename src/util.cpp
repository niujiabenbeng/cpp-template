#include "util.h"

#include <openssl/md5.h>

long GetCurrentTimeMs() {
  using milliseconds = std::chrono::milliseconds;
  auto now = std::chrono::system_clock::now().time_since_epoch();
  return std::chrono::duration_cast<milliseconds>(now).count();
}

std::string GetDateTimeString(long timestamp_ms, const std::string& format) {
  char buffer[64] = {0};
  time_t seconds = timestamp_ms / 1000;
  strftime(buffer, 64, format.c_str(), localtime(&seconds));
  return std::string(buffer);
}

void MakeDirsForFile(const std::string& path) {
  auto dirname = boost::filesystem::absolute(path).parent_path();
  if (!dirname.empty() && !boost::filesystem::exists(dirname)) {
    boost::filesystem::create_directories(dirname);
  }
}

std::string ReadFile(const std::string& file, bool is_binary) {
  std::ios_base::openmode mode = std::ios_base::in;
  if (is_binary) { mode |= std::ios_base::binary; }
  std::ifstream infile(file.c_str(), mode);
  if (!infile.is_open()) { return std::string(); }
  return std::string((std::istreambuf_iterator<char>(infile)),
                     std::istreambuf_iterator<char>());
}

bool WriteFile(const std::string& file, const char* data, const int len) {
  MakeDirsForFile(file);
  std::ofstream outfile(file, std::ios_base::binary);
  if (!outfile.is_open()) { return false; }
  outfile.write(data, len);
  return true;
}

bool WriteFile(const std::string& file, const std::string& content) {
  return WriteFile(file, content.data(), content.length());
}

bool WriteFile(const std::string& file, const std::vector<std::string>& lines) {
  return WriteFile(file, boost::algorithm::join(lines, "\n") + "\n");
}

Json::Value ParseJsonString(const std::string& content) {
  Json::Value root;
  if (content.empty()) { return root; }

  std::string error;
  Json::CharReaderBuilder builder;
  Json::CharReader* reader = builder.newCharReader();
  const char* begin = content.data();
  const char* end = begin + content.size();
  if (!reader->parse(begin, end, &root, &error)) {
    LOG(ERROR) << "failed to parse json string, error: " << error;
    root = Json::Value();
  }
  delete reader;
  return root;
}

std::string DumpJsonValue(const Json::Value& content) {
  Json::StreamWriterBuilder builder;
  builder["indentation"] = "";
  return Json::writeString(builder, content);
}

Json::Value ReadJsonFile(const std::string& json_file) {
  std::string content = ReadFile(json_file);
  if (content.empty()) {
    LOG(INFO) << "failed to read file: " << json_file;
    return Json::Value();
  }
  return ParseJsonString(content);
}

void WriteJsonFile(const Json::Value& root, const std::string& json_file) {
  MakeDirsForFile(json_file);
  std::ofstream outfile(json_file.c_str());
  CHECK(outfile.is_open()) << "failed to write to file: " << json_file;
  Json::StreamWriterBuilder builder;
  Json::StreamWriter* writer = builder.newStreamWriter();
  writer->write(root, &outfile);
  delete writer;
}

std::string ExecShell(const std::string& cmd) {
  std::string result;
  std::array<char, 128> buffer;
  FILE* pipe = popen(cmd.c_str(), "r");
  if (pipe == nullptr) {
    LOG(ERROR) << "popen() failed! " << strerror(errno);
    return std::string();
  }
  while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
    result += buffer.data();
  }
  pclose(pipe);
  return result;
}

std::vector<std::string> ListDirectory(const std::string& dirname,
                                       const std::regex& pattern) {
  namespace bf = boost::filesystem;
  std::vector<std::string> names;
  for (const auto& entry : bf::directory_iterator(dirname)) {
    auto path = entry.path().filename().string();
    if (std::regex_match(path, pattern)) { names.push_back(path); }
  }
  std::sort(names.begin(), names.end());
  return names;
}

std::string CalcMD5(const std::string& content) {
  unsigned char md5[MD5_DIGEST_LENGTH];
  MD5((unsigned char*) content.data(), content.size(), md5);
  std::string result(MD5_DIGEST_LENGTH * 2, 0);
  for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    std::sprintf(&result[2 * i], "%02x", md5[i]);
  }
  return result;
}

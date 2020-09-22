#include "util.h"

long GetCurrentTimeMs() {
  using milliseconds = std::chrono::milliseconds;
  auto now = std::chrono::system_clock::now().time_since_epoch();
  return std::chrono::duration_cast<milliseconds>(now).count();
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

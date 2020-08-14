#include "util.h"

long GetCurrentTimeMs() {
  using milliseconds = std::chrono::milliseconds;
  auto now = std::chrono::system_clock::now().time_since_epoch();
  return std::chrono::duration_cast<milliseconds>(now).count();
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
  auto path = boost::filesystem::path(file);
  auto dirname = boost::filesystem::absolute(path).parent_path();
  if (!boost::filesystem::exists(dirname)) {
    boost::filesystem::create_directories(dirname);
  }
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

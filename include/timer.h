#ifndef CPP_TEMPLATE_TIMER_H_
#define CPP_TEMPLATE_TIMER_H_

#include "common.h"

// // 本类统一时间点和时间段, 时间点取unix时间. 两者都以毫秒为单位.
// class TimeUtil {
//  public:
//   // 返回当前时间
//   static int64_t now();

//   // 返回给定时刻所在天的起始时间, 若输入为负值, 则返回今天的起始时间
//   static int64_t GetStartOfDay(int64_t timestamp = -1);

//   // 从字符串中读取时间, 实际上是: std::string -> int64_t
//   static int64_t FromString(const std::string& content);

//   // 从human readable字符串中读取时间, 格式为: 数字+单位. 单位支持如下的简写:
//   // s, sec, m, min, h, hour, d, day. 示例: "16s", "5.4 day".
//   static int64_t FromHumanReadableString(const std::string& content);

//   // 从日期字符串中读取时间, 字符串格式由format指定.
//   static int64_t FromDateTimeString(const std::string& content,
//                                     const std::string& format);

//   // 将timestamp转化为字符串, 实际上是: int64_t -> std::string
//   static std::string ToString(const int64_t timestamp);

//   // 将timestamp转化为human readable类型的字符串
//   static std::string ToHumanReadableString(const int64_t timestamp);

//   // 将timestamp转化为日期字符串, 格式由format指定
//   static std::string ToDatetimeString(const int64_t timestamp,
//                                       const std::string& format);
// };

class Timer {
 public:
  using SystemClock = std::chrono::system_clock;
  using SecondType = std::chrono::duration<float>;
  using MilliSecondType = std::chrono::duration<float, std::milli>;
  using MicroSecondType = std::chrono::duration<float, std::micro>;

  Timer();
  DEFAULT_COPY_ASIGN(Timer);
  DEFAULT_MOVE_ASIGN(Timer);
  ~Timer() = default;

  void Start(bool check_status = true);
  void Stop(bool check_status = true);

  float Seconds();
  float MilliSeconds();
  float MicroSeconds();

  float TotalSeconds();
  float TotalMilliSeconds();
  float TotalMicroSeconds();

  float AverageSeconds();
  float AverageMilliSeconds();
  float AverageMicroSeconds();

  void Accumulate();
  void ResetAccumulator();

  int count() const { return count_; }
  bool has_run_once() const { return has_run_once_; }

 private:
  SystemClock::time_point start_;
  SystemClock::time_point stop_;
  SystemClock::duration total_;
  bool is_running_ = false;
  bool has_run_once_ = false;
  bool has_accumulated_ = false;
  int count_ = 0;
};

class FrequencyCounter {
 public:
  using Duration = std::chrono::system_clock::duration;
  using TimePoint = std::chrono::system_clock::time_point;
  explicit FrequencyCounter(Duration interval = std::chrono::seconds(1));
  DEFAULT_COPY_ASIGN(FrequencyCounter);
  DEFAULT_MOVE_ASIGN(FrequencyCounter);
  ~FrequencyCounter() = default;

  void Reset();
  float Accumulate(int times = 1, float default_value = -1.0F);

 private:
  Duration interval_;
  TimePoint stamp_;
  int count_{0};
};

class DateTime {
 public:
  using SystemClock = std::chrono::system_clock;
  using Duration = SystemClock::duration;

  DateTime() = default;
  explicit DateTime(Duration timestamp) : value_(timestamp) {}
  DateTime(const std::string& content, const std::string& format);
  DEFAULT_COPY_ASIGN(DateTime);
  DEFAULT_MOVE_ASIGN(DateTime);
  ~DateTime() = default;

  static DateTime now();

  Duration::rep Seconds() const;
  Duration::rep MilliSeconds() const;

  DateTime GetStartOfDay();
  std::string Format(const std::string& format) const;

 private:
  Duration value_{Duration::zero()};
};

///////////////////////////////// class Timer //////////////////////////////////

inline Timer::Timer() {
  start_ = stop_ = SystemClock::now();
  total_ = SystemClock::duration::zero();
}

inline void Timer::Start(bool check_status) {
  CHECK(!check_status || !is_running_) << "Timer is already started.";
  start_ = SystemClock::now();
  is_running_ = true;
}

inline void Timer::Stop(bool check_status) {
  CHECK(!check_status || is_running_) << "Timer is not started yet.";
  stop_ = SystemClock::now();
  is_running_ = false;
  has_run_once_ = true;
  has_accumulated_ = false;
}

inline float Timer::Seconds() {
  using std::chrono::duration_cast;
  if (is_running_) { this->Stop(); }
  if (!has_run_once_) { return -1.0F; }
  return duration_cast<SecondType>(stop_ - start_).count();
}

inline float Timer::MilliSeconds() {
  using std::chrono::duration_cast;
  if (is_running_) { this->Stop(); }
  if (!has_run_once_) { return -1.0F; }
  return duration_cast<MilliSecondType>(stop_ - start_).count();
}

inline float Timer::MicroSeconds() {
  using std::chrono::duration_cast;
  if (is_running_) { this->Stop(); }
  if (!has_run_once_) { return -1.0F; }
  return duration_cast<MicroSecondType>(stop_ - start_).count();
}

inline float Timer::TotalSeconds() {
  using std::chrono::duration_cast;
  if (count_ <= 0) { return -1.0F; }
  return duration_cast<SecondType>(total_).count();
}

inline float Timer::TotalMilliSeconds() {
  using std::chrono::duration_cast;
  if (count_ <= 0) { return -1.0F; }
  return duration_cast<MilliSecondType>(total_).count();
}

inline float Timer::TotalMicroSeconds() {
  using std::chrono::duration_cast;
  if (count_ <= 0) { return -1.0F; }
  return duration_cast<MicroSecondType>(total_).count();
}

inline float Timer::AverageSeconds() {
  if (count_ <= 0) { return -1.0F; }
  return this->TotalSeconds() / count_;
}

inline float Timer::AverageMilliSeconds() {
  if (count_ <= 0) { return -1.0F; }
  return this->TotalMilliSeconds() / count_;
}

inline float Timer::AverageMicroSeconds() {
  if (count_ <= 0) { return -1.0F; }
  return this->TotalMicroSeconds() / count_;
}

inline void Timer::Accumulate() {
  if (is_running_) { this->Stop(); }
  CHECK(has_run_once_) << "Timer has never been run at all.";
  CHECK(!has_accumulated_) << "This interval has already been accumulated.";
  total_ += stop_ - start_;
  count_ += 1;
  has_accumulated_ = true;
}

inline void Timer::ResetAccumulator() {
  total_ = SystemClock::duration::zero();
  count_ = 0;
  has_accumulated_ = false;
}

//////////////////////////// class FrequencyCounter ////////////////////////////

inline FrequencyCounter::FrequencyCounter(Duration interval)
    : interval_(interval) {
  stamp_ = std::chrono::system_clock::now();
}

inline void FrequencyCounter::Reset() {
  count_ = 0;
  stamp_ = std::chrono::system_clock::now();
}

inline float FrequencyCounter::Accumulate(int times, float default_value) {
  count_ += times;
  auto current = std::chrono::system_clock::now();
  auto elapsed = current - stamp_;
  if (elapsed < interval_) { return default_value; }
  static_assert(std::is_same<decltype(elapsed), decltype(interval_)>::value);
  auto result = float(count_) * interval_.count() / elapsed.count();
  stamp_ = current;
  count_ = 0;
  return result;
}

//////////////////////////////// class DateTime ////////////////////////////////

inline DateTime::DateTime(const std::string& content,
                          const std::string& format) {
  std::tm tm = {};
  std::stringstream ss(content);
  ss >> std::get_time(&tm, format.c_str());
  value_ = std::chrono::seconds(std::mktime(&tm));
}

inline DateTime DateTime::now() {
  return DateTime{SystemClock::now().time_since_epoch()};
}



#endif  // CPP_TEMPLATE_TIMER_H_

#ifndef CPP_TEMPLATE_TIMER_H_
#define CPP_TEMPLATE_TIMER_H_

#include "common.h"

class Timer {
 public:
  using SystemClock = std::chrono::system_clock;
  using SecondType = std::chrono::duration<float>;
  using MilliSecondType = std::chrono::duration<float, std::milli>;
  using MicroSecondType = std::chrono::duration<float, std::micro>;

  Timer() = default;
  DEFAULT_COPY_ASIGN(Timer);
  DEFAULT_MOVE_ASIGN(Timer);
  ~Timer() = default;

  void Start(bool check_status = true) {
    CHECK(!check_status || !is_running_) << "Timer is already started.";
    start_ = SystemClock::now();
    is_running_ = true;
  }
  void Stop(bool check_status = true) {
    CHECK(!check_status || is_running_) << "Timer is not started yet.";
    stop_ = SystemClock::now();
    is_running_ = false;
    has_run_once_ = true;
    has_accumulated_ = false;
  }

  float Seconds() {
    using std::chrono::duration_cast;
    if (is_running_) { this->Stop(); }
    if (!has_run_once_) { return -1.0F; }
    return duration_cast<SecondType>(stop_ - start_).count();
  }
  float MilliSeconds() {
    using std::chrono::duration_cast;
    if (is_running_) { this->Stop(); }
    if (!has_run_once_) { return -1.0F; }
    return duration_cast<MilliSecondType>(stop_ - start_).count();
  }
  float MicroSeconds() {
    using std::chrono::duration_cast;
    if (is_running_) { this->Stop(); }
    if (!has_run_once_) { return -1.0F; }
    return duration_cast<MicroSecondType>(stop_ - start_).count();
  }

  float TotalSeconds() {
    using std::chrono::duration_cast;
    if (count_ <= 0) { return -1.0F; }
    return duration_cast<SecondType>(total_).count();
  }
  float TotalMilliSeconds() {
    using std::chrono::duration_cast;
    if (count_ <= 0) { return -1.0F; }
    return duration_cast<MilliSecondType>(total_).count();
  }
  float TotalMicroSeconds() {
    using std::chrono::duration_cast;
    if (count_ <= 0) { return -1.0F; }
    return duration_cast<MicroSecondType>(total_).count();
  }

  float AverageSeconds() {
    if (count_ <= 0) { return -1.0F; }
    return this->TotalSeconds() / count_;
  }
  float AverageMilliSeconds() {
    if (count_ <= 0) { return -1.0F; }
    return this->TotalMilliSeconds() / count_;
  }
  float AverageMicroSeconds() {
    if (count_ <= 0) { return -1.0F; }
    return this->TotalMicroSeconds() / count_;
  }

  void Accumulate() {
    if (is_running_) { this->Stop(); }
    CHECK(has_run_once_);
    CHECK(!has_accumulated_);
    total_ += stop_ - start_;
    count_ += 1;
    has_accumulated_ = true;
  }
  void ResetAccumulator() {
    total_ = SystemClock::duration::zero();
    count_ = 0;
    has_accumulated_ = false;
  }

  int count() const { return count_; }
  bool has_run_once() const { return has_run_once_; }

 private:
  SystemClock::time_point start_{SystemClock::now()};
  SystemClock::time_point stop_{SystemClock::now()};
  SystemClock::duration total_{SystemClock::duration::zero()};
  bool is_running_ = false;
  bool has_run_once_ = false;
  bool has_accumulated_ = false;
  int count_ = 0;
};

class FrequencyCounter {
 public:
  using SystemClock = std::chrono::system_clock;
  using Duration = SystemClock::duration;
  using TimePoint = SystemClock::time_point;

  FrequencyCounter() = default;
  explicit FrequencyCounter(Duration interval) : interval_(interval) {}
  DEFAULT_COPY_ASIGN(FrequencyCounter);
  DEFAULT_MOVE_ASIGN(FrequencyCounter);
  ~FrequencyCounter() = default;

  void Reset() {
    count_ = 0;
    stamp_ = SystemClock::now();
  }
  float Accumulate(int times = 1, float default_value = -1.0F) {
    count_ += times;
    auto elapsed = SystemClock::now() - stamp_;
    if (elapsed < interval_) { return default_value; }
    auto result = float(count_) * interval_.count() / elapsed.count();
    Reset();
    return result;
  }

 private:
  Duration interval_{std::chrono::seconds(1)};
  TimePoint stamp_{std::chrono::system_clock::now()};
  int count_{0};
};

#endif  // CPP_TEMPLATE_TIMER_H_

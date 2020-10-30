#ifndef CPP_TEMPLATE_TIMER_H_
#define CPP_TEMPLATE_TIMER_H_

#include "common.h"

class Timer {
 public:
  using system_clock = std::chrono::system_clock;
  using second_type = std::chrono::duration<float>;
  using millisecond_type = std::chrono::duration<float, std::milli>;
  using microsecond_type = std::chrono::duration<float, std::micro>;

  Timer() {
    start_ = stop_ = system_clock::now();
    total_ = system_clock::duration::zero();
  }
  DEFAULT_COPY_ASIGN(Timer);
  DEFAULT_MOVE_ASIGN(Timer);
  ~Timer() {}

  void Start() {
    CHECK(!is_running_) << "Timer is already started.";
    start_ = system_clock::now();
    is_running_ = true;
  }
  void Stop() {
    CHECK(is_running_) << "Timer is not started yet.";
    stop_ = system_clock::now();
    is_running_ = false;
    has_run_once_ = true;
    has_accumulated_ = false;
  }
  float MilliSeconds() {
    if (is_running_) { this->Stop(); }
    if (!has_run_once_) { return -1.0f; }
    return std::chrono::duration_cast<millisecond_type>(stop_ - start_).count();
  }
  float MicroSeconds() {
    if (is_running_) { this->Stop(); }
    if (!has_run_once_) { return -1.0f; }
    return std::chrono::duration_cast<microsecond_type>(stop_ - start_).count();
  }
  float Seconds() {
    if (is_running_) { this->Stop(); }
    if (!has_run_once_) { return -1.0f; }
    return std::chrono::duration_cast<second_type>(stop_ - start_).count();
  }

  void Accumulate() {
    if (is_running_) { this->Stop(); }
    CHECK(has_run_once_) << "Timer has never been run at all.";
    CHECK(!has_accumulated_) << "This interval has allready been accumulated.";
    total_ += stop_ - start_;
    count_ += 1;
    has_accumulated_ = true;
  }
  float TotalMilliSeconds() {
    if (count_ <= 0) { return -1.0f; }
    return std::chrono::duration_cast<millisecond_type>(total_).count();
  }
  float TotalMicroSeconds() {
    if (count_ <= 0) { return -1.0f; }
    return std::chrono::duration_cast<microsecond_type>(total_).count();
  }
  float TotalSeconds() {
    if (count_ <= 0) { return -1.0f; }
    return std::chrono::duration_cast<second_type>(total_).count();
  }
  float AverageMilliSeconds() {
    if (count_ <= 0) { return -1.0f; }
    return this->TotalMilliSeconds() / count_;
  }
  float AverageMicroSeconds() {
    if (count_ <= 0) { return -1.0f; }
    return this->TotalMicroSeconds() / count_;
  }
  float AverageSeconds() {
    if (count_ <= 0) { return -1.0f; }
    return this->TotalSeconds() / count_;
  }
  void ResetAccumulator() {
    total_ = system_clock::duration::zero();
    count_ = 0;
    has_accumulated_ = false;
  }
  int count() const { return count_; }
  bool has_run_once() const { return has_run_once_; }

 private:
  system_clock::time_point start_;
  system_clock::time_point stop_;
  system_clock::duration total_;
  bool is_running_ = false;
  bool has_run_once_ = false;
  bool has_accumulated_ = false;

  int count_ = 0;
};

class FrequencyCounter {
 public:
  using system_clock = std::chrono::system_clock;
  using second_type = std::chrono::duration<float>;

  // 这里interval以秒为单位, interval=0.001为毫秒
  explicit FrequencyCounter(float interval = 1.0f) : interval_(interval) {}
  DEFAULT_COPY_ASIGN(FrequencyCounter);
  DEFAULT_MOVE_ASIGN(FrequencyCounter);
  ~FrequencyCounter() {}

  float accumulate(int times = 1, float default_value = -1.0f) {
    if (!is_started_) {
      stamp_ = system_clock::now();
      is_started_ = true;
    }
    count_ += times;
    auto current = system_clock::now();
    auto interval = std::chrono::duration_cast<second_type>(current - stamp_);
    float result = default_value;
    if (interval >= interval_) {
      result = float(count_) * interval_.count() / interval.count();
      stamp_ = current;
      count_ = 0;
    }
    return result;
  }
  void reset() {
    count_ = 0;
    is_started_ = false;
  }

 private:
  second_type interval_;
  system_clock::time_point stamp_;
  int count_ = 0;
  bool is_started_ = false;
};

#endif  // CPP_TEMPLATE_TIMER_H_

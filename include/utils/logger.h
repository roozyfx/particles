#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>

template <typename Stream = std::ostream>
class Logger {
  Logger() : stream_(&std::cout) {}
  ~Logger() = default;

  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  void SetStream(Stream& s) {
    std::lock_guard<std::mutex> lock(mutex_);
    stream_ = &s;
  }

  Stream* stream_;
  std::mutex mutex_;

 public:
  static Logger& GetInstance(Stream& s) {
    static Logger instance_;
    instance_.SetStream(s);
    return instance_;
  }

  template <typename T>
  Logger& operator<<(const T& msg) {
    std::lock_guard<std::mutex> lock(mutex_);
    *stream_ << msg;
    return *this;
  }

  Logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
    std::lock_guard<std::mutex> lock(mutex_);
    manip(*stream_);
    return *this;
  }
};
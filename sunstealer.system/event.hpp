#pragma once

namespace ajm::system {
  class Event : public Object {
    public: Event(std::string name)
      : Object(name) { }

    public: virtual ~Event() {}

    public: void Reset() {}

    public: void Set() {}

    public: void Wait() {
      std::unique_lock<std::mutex> ul{this->mutex_};
      event.wait(ul, [&] { return stop_waiting; });
    }

    public: void WaitFor(Ustd::int32_t ms) {
      std::unique_lock<std::mutex> ul{this->mutex_};
      event.wait_for(ul, std::chrono::milliseconds(ms), [&] { return stop_waiting; });
    }

    public: void WaitUntil() {
      std::unique_lock<std::mutex> ul{this->mutex_};
      auto time = std::chrono::system_clock::now() + std::chrono::milliseconds(1000);
      event.wait_until(ul, time, [&] { return stop_waiting; });
    }

    public: bool stop_waiting = false;
    private:std::mutex mutex;
    private: std::condition_variable event;
  };
}

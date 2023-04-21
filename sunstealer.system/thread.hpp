#pragma once

namespace ajm::system
{
  class Thread : public Object {
    public: Thread(const std::string name)
      : Object(name)
    {
    }

    // ajm: ---------------------------------------------------------------------------------------
    public: void Start()
    {
      std::printf("Thread[%s]::Start().\n", this->name.c_str());
      this->thread_ = std::thread(&Thread::thread, this);
    }

    // ajm: ---------------------------------------------------------------------------------------
    public: void Stop()
    {
      std::printf("Thread[%s]::Stop().\n", this->name.c_str());
      this->exit_ = true;
      this->thread_.join();
      std::printf("Thread[%s] stopped.\n", this->name.c_str());
    }

    // ajm: ---------------------------------------------------------------------------------------
    virtual void thread() = 0;
    /* ajm: {
      while (!this->exit_)
      {
        std::this_thread::sleep_for(std::chrono::seconds{1});
      }
    }*/

    public: bool exit_;
    public: std::thread thread_;
  };
}

#pragma once

namespace std
{
  template <typename... Args> std::string format(const std::string &format, Args... args)
  {
    auto size = static_cast<size_t>(std::snprintf(nullptr, 0, format.c_str(), args...) + 1);
    std::unique_ptr<char[]> buffer(new char[size]);
    std::snprintf(buffer.get(), size, format.c_str(), args...);
    return std::string(buffer.get(), buffer.get() + size - 1);
  }
}

#pragma once

// ajm: ---------------------------------------------------------------------------------------
extern "C"
{
  ajm::framework::Device *Create(ajm::framework::ILogger *logger, std::map<std::string, std::string> parameters);
  void Destroy();
  std::string GetName();
}
